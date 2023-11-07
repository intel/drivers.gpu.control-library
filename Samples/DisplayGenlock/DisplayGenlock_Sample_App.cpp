//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  DisplayGenlock_Sample_App.cpp
 * @brief This file contains the 'main' function. Program execution begins and ends there.
 *
 */

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <cassert>
#include <string>
#include <fstream>
using namespace std;

#define CTL_APIEXPORT // caller of control API DLL shall define this before including igcl_api.h

#include "igcl_api.h"
#include "GenericIGCLApp.h"

typedef struct _genlock_sample_args
{
    bool IsGenlockPrimary;
    ctl_genlock_operation_t Operation;
    ctl_display_output_types_t PortType;
    uint32_t NumberOfPrimaryAdapters;
} genlock_sample_args;

static bool IsGetVBlankTs = false;

ctl_result_t GResult = CTL_RESULT_SUCCESS;

/***************************************************************
 * @brief PrintTopology
 * Print topology
 * @param pGenlockArgs
 * @return void
 ***************************************************************/
void PrintTopology(ctl_genlock_args_t *pGenlockArgs, uint32_t AdapterCount)
{
    printf("========= Genlock Topology =========\n");
    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        printf("Adapter %u:\n", AdapterIndex);
        printf("\tIsGenlockEnabled : %s\n", pGenlockArgs[AdapterIndex].IsGenlockEnabled ? "true" : "false");
        printf("\tIsGenlockPossible : %s\n", pGenlockArgs[AdapterIndex].IsGenlockPossible ? "true" : "false");
        if (true == pGenlockArgs[AdapterIndex].IsGenlockEnabled)
        {
            printf("\tIsPrimaryGenlockSystem : %s\n", pGenlockArgs[AdapterIndex].GenlockTopology.IsPrimaryGenlockSystem ? "true" : "false");
            printf("\tNumber of Genlock displays : %d\n", pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays);
            for (int8_t DisplayIndex = 0; DisplayIndex < pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays; DisplayIndex++)
            {
                printf("\tDisplay %u:\n", DisplayIndex);
                printf("\t\tDisplay output handle : %p\n", pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo[DisplayIndex].hDisplayOutput);
                printf("\t\tPrimary pipe : %s\n", pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo[DisplayIndex].IsPrimary ? "true" : "false");
            }
        }
    }
    printf("====================================\n");
}

/***************************************************************
 * @brief PrintUsage
 * Print usage of Genlock sample app
 * @param pArgv[]
 * @return void
 ***************************************************************/
void PrintUsage(char *pArgv[])
{
    printf("Genlock Sample Test Application.\n");
    printf("\nUsage: %s [Operation] <-g Genlock_Mode> <-d Display_Type> <-n Number_Of_Primary_Adapters>\n", pArgv[0]);
    printf("Operation\n");
    printf("\t-timing : Get target mode timing details supported by each display\n");
    printf("\t-validate : Validate Genlock configuration\n");
    printf("\t-enable : Enable Genlock\n");
    printf("\t-topology : Get current Genlock topology\n");
    printf("\t-disable : Disable Genlock\n");
    printf("\t-vblankts : Get VBlank Timestamp\n");
    printf("\t-help : Display usage\n");
    printf("-g [Genlock mode]\n");
    printf("\tprimary : primary system providing ref clock and ref sync (default)\n");
    printf("\tsecondary : secondary system referring ref clock and ref sync from primary\n");
    printf("-d [Display type]\n");
    printf("\tdp : Display Port (default)\n");
    printf("\thdmi : HDMI\n");
    printf("-n [Number of Primary Adapters]\n");
    printf("\tSpecify how many adapters you want to enable as a primary (default : 1)\n");
}

/***************************************************************
 * @brief Find option and get option argument from command arguments
 * @param Argc
 * @param pArgv
 * @param Option
 * @param OptionArg
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t FindOptionArg(int32_t Argc, char *pArgv[], const string &Option, string &OptionArg)
{
    for (int32_t i = 0; i < Argc; ++i)
    {
        string Opt = pArgv[i];
        if (0 == Opt.find(Option))
        {
            if (0 != strcmp(Option.c_str(), Opt.c_str()))
                continue;

            if ((i + 1) < Argc)
                OptionArg = pArgv[i + 1];

            return CTL_RESULT_SUCCESS;
        }
    }

    return CTL_RESULT_ERROR_INVALID_ARGUMENT;
}

/***************************************************************
 * @brief GetCmdlineArgs
 * Get command line arguments
 * @param Argc, pArgv[], pGenlockSampleArgs
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t GetCmdlineArgs(int Argc, char *pArgv[], genlock_sample_args *pGenlockSampleArgs)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;
    string OptionArg;

    // initialize Genlock Sample Args
    pGenlockSampleArgs->PortType                = CTL_DISPLAY_OUTPUT_TYPES_DISPLAYPORT;
    pGenlockSampleArgs->NumberOfPrimaryAdapters = 1;
    pGenlockSampleArgs->IsGenlockPrimary        = true;

    if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-timing", OptionArg))
    {
        pGenlockSampleArgs->Operation = CTL_GENLOCK_OPERATION_GET_TIMING_DETAILS;
    }
    else if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-validate", OptionArg))
    {
        pGenlockSampleArgs->Operation = CTL_GENLOCK_OPERATION_VALIDATE;
    }
    else if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-enable", OptionArg))
    {
        pGenlockSampleArgs->Operation = CTL_GENLOCK_OPERATION_ENABLE;
    }
    else if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-disable", OptionArg))
    {
        pGenlockSampleArgs->Operation = CTL_GENLOCK_OPERATION_DISABLE;
    }
    else if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-topology", OptionArg))
    {
        pGenlockSampleArgs->Operation = CTL_GENLOCK_OPERATION_GET_TOPOLOGY;
    }
    else if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-vblankts", OptionArg))
    {
        IsGetVBlankTs = true;
    }
    else
    {
        PrintUsage(pArgv);
        Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
        EXIT_ON_ERROR(Result);
    }

    // Genlock Mode option
    if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-g", OptionArg))
    {
        if (("" != OptionArg) && (strstr(OptionArg.c_str(), "secondary") != NULL))
        {
            pGenlockSampleArgs->IsGenlockPrimary = false;
        }
    }
    printf("[in] Genlock Mode: %s\n", pGenlockSampleArgs->IsGenlockPrimary == true ? "primary" : "secondary");
    // Display Type option
    if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-d", OptionArg))
    {
        if (("" != OptionArg) && (strstr(OptionArg.c_str(), "hdmi") != NULL))
        {
            pGenlockSampleArgs->PortType = CTL_DISPLAY_OUTPUT_TYPES_HDMI;
        }
    }
    printf("[in] Display Type: %s\n", pGenlockSampleArgs->PortType == CTL_DISPLAY_OUTPUT_TYPES_HDMI ? "hdmi" : "dp");
    // Number of Primary Adapters option
    if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-n", OptionArg))
    {
        if (("" != OptionArg))
        {
            for (uint32_t Index = 0; Index < OptionArg.length(); Index++)
            {
                if (!isdigit(OptionArg[Index]))
                {
                    printf("Invalid Number of primary adapters.\n");
                    Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
                    EXIT_ON_ERROR(Result);
                }
            }
            pGenlockSampleArgs->NumberOfPrimaryAdapters = stoul(OptionArg, 0, 16);
        }
    }
    printf("[in] Number of primary adapters: %u\n", pGenlockSampleArgs->NumberOfPrimaryAdapters);

Exit:
    return Result;
}

/***************************************************************
 * @brief FindBestCommonTiming
 * Find best common target mode timing and fill it to CommonTargetMode
 * @param pGenlockArgs, AdapterCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t FindBestCommonTiming(ctl_genlock_args_t *pGenlockArgs, uint32_t AdapterCount)
{
    ctl_result_t Result                                   = CTL_RESULT_SUCCESS;
    ctl_genlock_target_mode_list_t *pGenlockModeListFixed = NULL;
    ctl_genlock_target_mode_list_t *pGenlockModeList      = NULL;
    ctl_display_timing_t *pTargetModes                    = NULL;
    ctl_display_timing_t *pPreferredTargetMode            = NULL;
    uint8_t ModeIndex                                     = 0;

    // Find the best mode timing in the first display
    pGenlockModeListFixed = &pGenlockArgs[0].GenlockTopology.pGenlockModeList[0];
    for (ModeIndex = 0; ModeIndex < pGenlockModeListFixed->NumModes; ModeIndex++)
    {
        pTargetModes = &pGenlockModeListFixed->pTargetModes[ModeIndex];
        if ((NULL == pPreferredTargetMode) ||
            ((pTargetModes->HActive >= pPreferredTargetMode->HActive) && (pTargetModes->VActive >= pPreferredTargetMode->VActive) && (pTargetModes->RefreshRate > pPreferredTargetMode->RefreshRate)))
        {
            pPreferredTargetMode = pTargetModes;
        }
    }

    if (NULL == pPreferredTargetMode)
    {
        Result = CTL_RESULT_ERROR_DATA_NOT_FOUND;
        LOG_AND_EXIT_ON_ERROR(Result, "No common timings");
    }
    pGenlockArgs[0].GenlockTopology.CommonTargetMode = *pPreferredTargetMode;

    // Check if the preferred mode timing is common in the first adapter
    for (uint8_t DisplayIndex = 1; DisplayIndex < pGenlockArgs[0].GenlockTopology.NumGenlockDisplays; DisplayIndex++)
    {
        pGenlockModeList = &pGenlockArgs[0].GenlockTopology.pGenlockModeList[DisplayIndex];
        for (ModeIndex = 0; ModeIndex < pGenlockModeList->NumModes; ModeIndex++)
        {
            pTargetModes = &pGenlockModeListFixed->pTargetModes[ModeIndex];
            if ((pPreferredTargetMode->PixelClock == pTargetModes->PixelClock) && (pPreferredTargetMode->HActive == pTargetModes->HActive) && (pPreferredTargetMode->HTotal == pTargetModes->HTotal) &&
                (pPreferredTargetMode->VActive == pTargetModes->VActive) && (pPreferredTargetMode->VTotal == pTargetModes->VTotal) && (pPreferredTargetMode->RefreshRate == pTargetModes->RefreshRate))
            {
                break;
            }
        }
        if (ModeIndex == pGenlockModeList->NumModes)
        {
            pPreferredTargetMode = NULL;
            Result               = CTL_RESULT_ERROR_DATA_NOT_FOUND;
            LOG_AND_EXIT_ON_ERROR(Result, "No common timings");
        }
    }

    // Check if the preferred mode of the 1st adapter is common among displays on all other adapters
    for (uint32_t AdapterIndex = 1; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        for (uint8_t DisplayIndex = 0; DisplayIndex < pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays; DisplayIndex++)
        {
            pGenlockModeList = &pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockModeList[DisplayIndex];
            for (ModeIndex = 0; ModeIndex < pGenlockModeList->NumModes; ModeIndex++)
            {
                pTargetModes = &pGenlockModeListFixed->pTargetModes[ModeIndex];
                if ((pPreferredTargetMode->PixelClock == pTargetModes->PixelClock) && (pPreferredTargetMode->HActive == pTargetModes->HActive) &&
                    (pPreferredTargetMode->HTotal == pTargetModes->HTotal) && (pPreferredTargetMode->VActive == pTargetModes->VActive) && (pPreferredTargetMode->VTotal == pTargetModes->VTotal) &&
                    (pPreferredTargetMode->RefreshRate == pTargetModes->RefreshRate))
                {
                    break;
                }
            }
            if (ModeIndex == pGenlockModeList->NumModes)
            {
                pPreferredTargetMode = NULL;
                Result               = CTL_RESULT_ERROR_DATA_NOT_FOUND;
                LOG_AND_EXIT_ON_ERROR(Result, "No common timings");
            }
        }

        if (NULL == pPreferredTargetMode)
        {
            LOG_AND_EXIT_ON_ERROR(Result, "No common timings");
        }
        else
        {
            pGenlockArgs[AdapterIndex].GenlockTopology.CommonTargetMode = *pPreferredTargetMode;
        }
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief TestGenlockGetTimingDetails
 * Test getting timing details
 * @param hDevices, pGenlockArgs, AdapterCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestGenlockGetTimingDetails(ctl_device_adapter_handle_t *hDevices, ctl_genlock_args_t *pGenlockArgs, uint32_t AdapterCount)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;
    ctl_genlock_target_mode_list_t *pGenlockModeList;
    ctl_device_adapter_handle_t hFailureDeviceAdapter = NULL;

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        pGenlockArgs[AdapterIndex].Operation = CTL_GENLOCK_OPERATION_GET_TIMING_DETAILS;
        for (uint8_t ModeListIndex = 0; ModeListIndex < pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays; ModeListIndex++)
        {
            pGenlockModeList           = &pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockModeList[ModeListIndex];
            pGenlockModeList->NumModes = 0;
        }
    }

    // Get number of target modes
    Result = ctlGetSetDisplayGenlock(hDevices, pGenlockArgs, AdapterCount, &hFailureDeviceAdapter);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplayGenlock");

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        for (uint8_t ModeListIndex = 0; ModeListIndex < pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays; ModeListIndex++)
        {
            pGenlockModeList = &pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockModeList[ModeListIndex];
            if (NULL == pGenlockModeList->pTargetModes)
            {
                pGenlockModeList->pTargetModes = (ctl_display_timing_t *)malloc(pGenlockModeList->NumModes * sizeof(ctl_display_timing_t));
                EXIT_ON_MEM_ALLOC_FAILURE(pGenlockModeList->pTargetModes, "pGenlockModeList->pTargetModes");
            }
        }
    }

    // Get mode timings details
    Result = ctlGetSetDisplayGenlock(hDevices, pGenlockArgs, AdapterCount, &hFailureDeviceAdapter);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplayGenlock");

Exit:
    return Result;
}

/***************************************************************
 * @brief TestGenlockValidate
 * Test validating configuration
 * @param hDevices, pGenlockArgs, AdapterCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestGenlockValidate(ctl_device_adapter_handle_t *hDevices, ctl_genlock_args_t *pGenlockArgs, uint32_t AdapterCount)
{
    ctl_result_t Result                               = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t hFailureDeviceAdapter = NULL;

    // Get timing details
    Result = TestGenlockGetTimingDetails(hDevices, pGenlockArgs, AdapterCount);
    LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockGetTimingDetails");

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        pGenlockArgs[AdapterIndex].Operation = CTL_GENLOCK_OPERATION_VALIDATE;
        if (true == pGenlockArgs[AdapterIndex].GenlockTopology.IsPrimaryGenlockSystem)
        {
            pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo[0].IsPrimary = true;
        }
    }

    // Find and fill best common timing
    Result = FindBestCommonTiming(pGenlockArgs, AdapterCount);
    LOG_AND_EXIT_ON_ERROR(Result, "FindBestCommonTiming");

    Result = ctlGetSetDisplayGenlock(hDevices, pGenlockArgs, AdapterCount, &hFailureDeviceAdapter);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplayGenlock");

    printf("========= Genlock Validate =========\n");
    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        printf("Adapter[%u]: IsGenlockPossible : %s\n", AdapterIndex, pGenlockArgs[AdapterIndex].IsGenlockPossible ? "true" : "false");
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief TestGenlockGetTopology
 * Test getting current topology
 * @param hDevices, pGenlockArgs, AdapterCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestGenlockGetTopology(ctl_device_adapter_handle_t *hDevices, ctl_genlock_args_t *pGenlockArgs, uint32_t AdapterCount)
{
    ctl_result_t Result                               = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t hFailureDeviceAdapter = NULL;

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        pGenlockArgs[AdapterIndex].Operation = CTL_GENLOCK_OPERATION_GET_TOPOLOGY;
    }
    Result = ctlGetSetDisplayGenlock(hDevices, pGenlockArgs, AdapterCount, &hFailureDeviceAdapter);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplayGenlock");

    PrintTopology(pGenlockArgs, AdapterCount);

Exit:
    return Result;
}

/***************************************************************
 * @brief TestGenlockEnable
 * Test enabling Genlock
 * @param hDevices, pGenlockArgs, AdapterCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestGenlockEnable(ctl_device_adapter_handle_t *hDevices, ctl_genlock_args_t *pGenlockArgs, uint32_t AdapterCount)
{
    ctl_result_t Result                               = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t hFailureDeviceAdapter = NULL;

    Result = TestGenlockValidate(hDevices, pGenlockArgs, AdapterCount);
    LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockValidate");

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        pGenlockArgs[AdapterIndex].Operation = CTL_GENLOCK_OPERATION_ENABLE;
        if (true == pGenlockArgs[AdapterIndex].GenlockTopology.IsPrimaryGenlockSystem)
        {
            pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo[0].IsPrimary = true;
        }
    }

    Result = ctlGetSetDisplayGenlock(hDevices, pGenlockArgs, AdapterCount, &hFailureDeviceAdapter);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplayGenlock");

    Result = TestGenlockGetTopology(hDevices, pGenlockArgs, AdapterCount);
    LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockGetTopology");

Exit:
    return Result;
}

/***************************************************************
 * @brief TestGenlockDisable
 * Test disabling Genlock
 * @param hDevices, pGenlockArgs, AdapterCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestGenlockDisable(ctl_device_adapter_handle_t *hDevices, ctl_genlock_args_t *pGenlockArgs, uint32_t AdapterCount)
{
    ctl_result_t Result                               = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t hFailureDeviceAdapter = NULL;

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        pGenlockArgs[AdapterIndex].Operation = CTL_GENLOCK_OPERATION_DISABLE;
    }

    Result = ctlGetSetDisplayGenlock(hDevices, pGenlockArgs, AdapterCount, &hFailureDeviceAdapter);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplayGenlock");

    Result = TestGenlockGetTopology(hDevices, pGenlockArgs, AdapterCount);
    LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockGetTopology");

Exit:
    return Result;
}

/***************************************************************
 * @brief InitGenlockArgs
 * Initialize Genlock Arguments
 * @param hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t InitGenlockArgs(ctl_device_adapter_handle_t *hDevices, uint32_t AdapterCount, const genlock_sample_args GenlockSampleArgs, ctl_genlock_args_t *pGenlockArgs)
{
    ctl_result_t Result                                = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t hFailureDeviceAdapter  = NULL;
    ctl_display_output_handle_t *hDisplayOutput        = NULL;
    ctl_display_output_handle_t *hActiveDisplayOutputs = NULL;
    uint32_t DisplayCount                              = 0;
    uint8_t ActiveDisplayCount                         = 0;
    uint8_t MaxNumDisplayOutputs                       = 0;
    bool IsDisplayActive                               = false;
    bool IsDisplayAttached                             = false;

    // Get maximum number of display outputs supported
    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        ZeroMemory(&pGenlockArgs[AdapterIndex], sizeof(ctl_genlock_args_t));
        pGenlockArgs[AdapterIndex].Size                               = sizeof(ctl_genlock_args_t);
        pGenlockArgs[AdapterIndex].Version                            = 0;
        pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays = 0;
        pGenlockArgs[AdapterIndex].Operation                          = CTL_GENLOCK_OPERATION_VALIDATE;
    }
    Result = ctlGetSetDisplayGenlock(hDevices, pGenlockArgs, AdapterCount, &hFailureDeviceAdapter);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplayGenlock");

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        ActiveDisplayCount                                            = 0;
        MaxNumDisplayOutputs                                          = pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays;
        pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays = 0;

        // Enumerate all the possible target displays for the adapters
        // First step is to get the count
        DisplayCount = 0;

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], &DisplayCount, NULL);

        if (CTL_RESULT_SUCCESS != Result)
        {
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");
        }
        else if (DisplayCount <= 0)
        {
            printf("Invalid display count for adapter: %d\n", AdapterIndex);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");
        }

        hDisplayOutput = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

        hActiveDisplayOutputs = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * MaxNumDisplayOutputs);
        EXIT_ON_MEM_ALLOC_FAILURE(hActiveDisplayOutputs, "hActiveDisplayOutputs");
        memset(hActiveDisplayOutputs, 0, sizeof(ctl_display_output_handle_t) * MaxNumDisplayOutputs);

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], &DisplayCount, hDisplayOutput);
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        for (uint8_t DisplayIndex = 0; DisplayIndex < DisplayCount; DisplayIndex++)
        {
            ctl_display_properties_t stDisplayProperties = {};
            stDisplayProperties.Size                     = sizeof(ctl_display_properties_t);

            if (NULL == hDisplayOutput[DisplayIndex])
            {
                printf("hDisplayOutput[%d] is NULL.\n", DisplayIndex);
                Result = CTL_RESULT_ERROR_INVALID_NULL_HANDLE;
                EXIT_ON_ERROR(Result);
            }

            Result = ctlGetDisplayProperties(hDisplayOutput[DisplayIndex], &stDisplayProperties);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDisplayProperties");

            ctl_adapter_display_encoder_properties_t stDisplayEncoderProperties = {};
            stDisplayEncoderProperties.Size                                     = sizeof(ctl_adapter_display_encoder_properties_t);

            Result = ctlGetAdaperDisplayEncoderProperties(hDisplayOutput[DisplayIndex], &stDisplayEncoderProperties);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlGetAdaperDisplayEncoderProperties");

            IsDisplayActive   = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE;
            IsDisplayAttached = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED;

            // Filter active display outputs
            if (IsDisplayActive && IsDisplayAttached && (GenlockSampleArgs.PortType == stDisplayEncoderProperties.Type))
            {
                hActiveDisplayOutputs[ActiveDisplayCount++] = hDisplayOutput[DisplayIndex];
            }
        }

        if ((CTL_GENLOCK_OPERATION_GET_TOPOLOGY != GenlockSampleArgs.Operation) && (0 == ActiveDisplayCount))
        {
            printf("Adatper[%d] does not have any active displays for the selected display type. ActiveDisplayCount is %d.\n", AdapterIndex, ActiveDisplayCount);
            Result = CTL_RESULT_ERROR_DISPLAY_NOT_ATTACHED;
            EXIT_ON_ERROR(Result);
        }

        if (CTL_GENLOCK_OPERATION_GET_TOPOLOGY == GenlockSampleArgs.Operation)
        {
            // Set maximum number of displays to get info of all displays under Genlock
            pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays = MaxNumDisplayOutputs;
        }
        else
        {
            pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays = ActiveDisplayCount;
        }
        pGenlockArgs[AdapterIndex].GenlockTopology.IsPrimaryGenlockSystem = GenlockSampleArgs.IsGenlockPrimary;

        // Allocate dynamic memories
        pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo =
        (ctl_genlock_display_info_t *)malloc(sizeof(ctl_genlock_display_info_t) * pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays);
        EXIT_ON_MEM_ALLOC_FAILURE(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo, "pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo");
        memset(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo, 0, sizeof(ctl_genlock_display_info_t) * pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays);

        pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockModeList =
        (ctl_genlock_target_mode_list_t *)malloc(sizeof(ctl_genlock_target_mode_list_t) * pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays);
        EXIT_ON_MEM_ALLOC_FAILURE(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockModeList, "pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockModeList");
        memset(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockModeList, 0, sizeof(ctl_genlock_target_mode_list_t) * pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays);

        for (uint8_t DisplayIndex = 0; DisplayIndex < ActiveDisplayCount; DisplayIndex++)
        {
            pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo[DisplayIndex].hDisplayOutput = hActiveDisplayOutputs[DisplayIndex];
            pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockModeList[DisplayIndex].hDisplayOutput    = hActiveDisplayOutputs[DisplayIndex];
        }

        // Free dynamically allocated memories
        CTL_FREE_MEM(hActiveDisplayOutputs);
        CTL_FREE_MEM(hDisplayOutput);
    }

Exit:
    // Free dynamically allocated memories
    CTL_FREE_MEM(hActiveDisplayOutputs);
    CTL_FREE_MEM(hDisplayOutput);

    return Result;
}

/***************************************************************
 * @brief GetVblankTimestamp
 * get vblank time stamp
 * @param hDevices, AdapterCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t GetVblankTimestamp(ctl_device_adapter_handle_t *hDevices, uint32_t AdapterCount)
{
    ctl_result_t Result                                = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t hFailureDeviceAdapter  = NULL;
    ctl_display_output_handle_t *hDisplayOutput        = NULL;
    ctl_display_output_handle_t *hActiveDisplayOutputs = NULL;
    uint32_t DisplayCount                              = 0;
    uint8_t ActiveDisplayCount                         = 0;
    uint8_t MaxNumDisplayOutputs                       = 0;
    bool IsDisplayActive                               = false;
    bool IsDisplayAttached                             = false;
    ctl_vblank_ts_args_t VblankTsArgs;

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        ActiveDisplayCount = 0;

        // Enumerate all the possible target displays for the adapters
        // First step is to get the count
        DisplayCount = 0;

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], &DisplayCount, NULL);

        if (CTL_RESULT_SUCCESS != Result)
        {
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");
        }
        else if (DisplayCount <= 0)
        {
            printf("Invalid Display Count. Skipping display enumeration for adapter: %d\n", AdapterIndex);
            continue;
        }

        hDisplayOutput = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], &DisplayCount, hDisplayOutput);
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        printf("Adapter %u:\n", AdapterIndex);
        for (uint8_t DisplayIndex = 0; DisplayIndex < DisplayCount; DisplayIndex++)
        {
            ctl_display_properties_t stDisplayProperties = {};
            stDisplayProperties.Size                     = sizeof(ctl_display_properties_t);

            if (NULL == hDisplayOutput[DisplayIndex])
            {
                printf("\thDisplayOutput[%d] is NULL.\n", DisplayIndex);
                Result = CTL_RESULT_ERROR_INVALID_NULL_HANDLE;
                EXIT_ON_ERROR(Result);
            }

            Result = ctlGetDisplayProperties(hDisplayOutput[DisplayIndex], &stDisplayProperties);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDisplayProperties");

            ctl_adapter_display_encoder_properties_t stDisplayEncoderProperties = {};
            stDisplayEncoderProperties.Size                                     = sizeof(ctl_adapter_display_encoder_properties_t);

            Result = ctlGetAdaperDisplayEncoderProperties(hDisplayOutput[DisplayIndex], &stDisplayEncoderProperties);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlGetAdaperDisplayEncoderProperties");

            IsDisplayActive   = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE;
            IsDisplayAttached = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED;

            memset(&VblankTsArgs, 0, sizeof(ctl_vblank_ts_args_t));

            // Filter active display outputs
            if (IsDisplayActive && IsDisplayAttached)
            {
                // get genlock status (vblank time stamp)
                Result = ctlGetVblankTimestamp(hDisplayOutput[DisplayIndex], &VblankTsArgs);
                LOG_AND_EXIT_ON_ERROR(Result, "ctlGetVblankTimestamp");
            }

            for (uint8_t i = 0; i < VblankTsArgs.NumOfTargets; i++)
            {
                printf("\tTarget ID: %d Child[%d] vblank timestamp: %I64d\n", stDisplayProperties.Os_display_encoder_handle.WindowsDisplayEncoderID, i, VblankTsArgs.VblankTS[i]);
            }
        }

        // Free dynamically allocated memories
        CTL_FREE_MEM(hDisplayOutput);
    }

Exit:
    CTL_FREE_MEM(hDisplayOutput);

    return Result;
}

/***************************************************************
 * @brief TestDisplayGenlock
 * Test Display Genlock
 * @param hDevices, AdapterCount, GenlockSampleArgs
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestDisplayGenlock(ctl_device_adapter_handle_t *hDevices, uint32_t AdapterCount, genlock_sample_args GenlockSampleArgs)
{
    ctl_result_t Result                             = CTL_RESULT_SUCCESS;
    ctl_genlock_args_t *pGenlockArgs                = NULL;
    ctl_device_adapter_handle_t *hPrimaryAdapters   = NULL;
    ctl_device_adapter_handle_t *hSecondaryAdapters = NULL;
    uint8_t GenlockedAdapterCount                   = 0;
    uint8_t PrimaryAdapterCount                     = 0;
    uint8_t SecondaryAdapterCount                   = 0;

    pGenlockArgs = (ctl_genlock_args_t *)malloc(sizeof(ctl_genlock_args_t) * AdapterCount);
    EXIT_ON_MEM_ALLOC_FAILURE(pGenlockArgs, "pGenlockArgs");

    switch (GenlockSampleArgs.Operation)
    {
        case CTL_GENLOCK_OPERATION_GET_TIMING_DETAILS:
            Result = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs);
            LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
            Result = TestGenlockGetTimingDetails(hDevices, pGenlockArgs, AdapterCount);
            break;
        case CTL_GENLOCK_OPERATION_VALIDATE:
            Result = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs);
            LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
            Result = TestGenlockValidate(hDevices, pGenlockArgs, AdapterCount);
            break;
        case CTL_GENLOCK_OPERATION_ENABLE:
            // The primary adapter must be enabled first before other secondary adapters are enabled
            if (true == GenlockSampleArgs.IsGenlockPrimary)
            {
                // Number of primary adapters cannot be zero or bigger than plugged-in adapters count
                if ((0 == GenlockSampleArgs.NumberOfPrimaryAdapters) || (AdapterCount < GenlockSampleArgs.NumberOfPrimaryAdapters))
                {
                    printf("Number of primary adapters cannot be zero or bigger than plugged-in adapters count.\n");
                    Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
                    CTL_FREE_MEM(pGenlockArgs);
                    return Result;
                }
                // Enable primary target on the primary adapter#0
                Result = InitGenlockArgs(hDevices, GenlockSampleArgs.NumberOfPrimaryAdapters, GenlockSampleArgs, pGenlockArgs);
                LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");

                Result = TestGenlockEnable(hDevices, pGenlockArgs, GenlockSampleArgs.NumberOfPrimaryAdapters);
                LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockEnable - Primary");

                SecondaryAdapterCount = AdapterCount - GenlockSampleArgs.NumberOfPrimaryAdapters;
                if (SecondaryAdapterCount > 0)
                {
                    // Other adapters are secondary systems for Genlock
                    GenlockSampleArgs.IsGenlockPrimary = false;
                    Result                             = InitGenlockArgs(&hDevices[GenlockSampleArgs.NumberOfPrimaryAdapters], SecondaryAdapterCount, GenlockSampleArgs, pGenlockArgs);
                    LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
                    Result = TestGenlockEnable(&hDevices[GenlockSampleArgs.NumberOfPrimaryAdapters], pGenlockArgs, SecondaryAdapterCount);
                    LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockEnable - Secondary");
                }
            }
            else
            {
                Result = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs);
                LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
                Result = TestGenlockEnable(hDevices, pGenlockArgs, AdapterCount);
                LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockEnable - Primary");
            }
            break;
        case CTL_GENLOCK_OPERATION_DISABLE:
            // The primary adapter must be disabled last after secondary adapters are disabled
            if (true == GenlockSampleArgs.IsGenlockPrimary)
            {
                // Secondary adapters should be disabled first
                // Get topology to check which adapter is a primary
                GenlockSampleArgs.Operation = CTL_GENLOCK_OPERATION_GET_TOPOLOGY;
                Result                      = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs);
                LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
                Result = TestGenlockGetTopology(hDevices, pGenlockArgs, AdapterCount);
                LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockGetTopology");

                // Count primary and secondary adapters
                GenlockedAdapterCount = 0;
                PrimaryAdapterCount   = 0;
                SecondaryAdapterCount = 0;
                for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
                {
                    if (true == pGenlockArgs[AdapterIndex].IsGenlockEnabled)
                    {
                        GenlockedAdapterCount++;
                        if (false == pGenlockArgs[AdapterIndex].GenlockTopology.IsPrimaryGenlockSystem)
                        {
                            SecondaryAdapterCount++;
                        }
                    }
                }

                if (0 == GenlockedAdapterCount)
                {
                    printf("There is no Genlocked adapter.\n");
                    Result = CTL_RESULT_ERROR_NOT_INITIALIZED;
                    EXIT_ON_ERROR(Result);
                }

                PrimaryAdapterCount = GenlockedAdapterCount - SecondaryAdapterCount;
                if (0 != PrimaryAdapterCount)
                {
                    hPrimaryAdapters = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * PrimaryAdapterCount);
                    EXIT_ON_MEM_ALLOC_FAILURE(hPrimaryAdapters, "hPrimaryAdapters");
                }
                if (0 != SecondaryAdapterCount)
                {
                    hSecondaryAdapters = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * SecondaryAdapterCount);
                    EXIT_ON_MEM_ALLOC_FAILURE(hSecondaryAdapters, "hSecondaryAdapters");
                }

                PrimaryAdapterCount   = 0;
                SecondaryAdapterCount = 0;
                for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
                {
                    if ((true == pGenlockArgs[AdapterIndex].IsGenlockEnabled) && (true == pGenlockArgs[AdapterIndex].GenlockTopology.IsPrimaryGenlockSystem) && (NULL != hPrimaryAdapters))
                    {
                        hPrimaryAdapters[PrimaryAdapterCount++] = hDevices[AdapterIndex];
                    }
                    else if ((true == pGenlockArgs[AdapterIndex].IsGenlockEnabled) && (false == pGenlockArgs[AdapterIndex].GenlockTopology.IsPrimaryGenlockSystem) && (NULL != hSecondaryAdapters))
                    {
                        hSecondaryAdapters[SecondaryAdapterCount++] = hDevices[AdapterIndex];
                    }
                }

                GenlockSampleArgs.Operation = CTL_GENLOCK_OPERATION_DISABLE;
                // Disable secondary adapters
                GenlockSampleArgs.IsGenlockPrimary = false;
                if (0 < SecondaryAdapterCount)
                {
                    Result = InitGenlockArgs(hSecondaryAdapters, SecondaryAdapterCount, GenlockSampleArgs, pGenlockArgs);
                    LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
                    Result = TestGenlockDisable(hSecondaryAdapters, pGenlockArgs, SecondaryAdapterCount);
                    LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockDisable");
                }
                CTL_FREE_MEM(hSecondaryAdapters);

                // Disable primary adapter
                GenlockSampleArgs.IsGenlockPrimary = true;
                if (0 < PrimaryAdapterCount)
                {
                    Result = InitGenlockArgs(hPrimaryAdapters, PrimaryAdapterCount, GenlockSampleArgs, pGenlockArgs);
                    LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
                    Result = TestGenlockDisable(hPrimaryAdapters, pGenlockArgs, PrimaryAdapterCount);
                }
            }
            else
            {
                Result = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs);
                LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
                Result = TestGenlockDisable(hDevices, pGenlockArgs, AdapterCount);
            }
            break;
        case CTL_GENLOCK_OPERATION_GET_TOPOLOGY:
            Result = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs);
            LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
            Result = TestGenlockGetTopology(hDevices, pGenlockArgs, AdapterCount);
            break;
        default:
            Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
            CTL_FREE_MEM(pGenlockArgs);
            return Result;
    }

Exit:
    // Free dynamic memories
    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        if (NULL != pGenlockArgs)
        {

            if (CTL_GENLOCK_OPERATION_GET_TIMING_DETAILS == pGenlockArgs[AdapterIndex].Operation)
            {
                for (uint8_t ModeListIndex = 0; ModeListIndex < pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays; ModeListIndex++)
                {
                    CTL_FREE_MEM(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockModeList[ModeListIndex].pTargetModes);
                }
            }
            CTL_FREE_MEM(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo);
            CTL_FREE_MEM(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockModeList);
        }
    }

    CTL_FREE_MEM(pGenlockArgs);

    return Result;
}

/***************************************************************
 * @brief Main Function
 * @param
 * @return ctl_result_t
 ***************************************************************/
int main(int Argc, char *pArgv[])
{
    ctl_result_t Result                   = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hDevices = nullptr;
    uint32_t AdapterCount                 = 0;
    ctl_init_args_t CtlInitArgs;
    ctl_api_handle_t hAPIHandle           = nullptr;
    genlock_sample_args GenlockSampleArgs = { 0 };
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Get cmdline arguments
    Result = GetCmdlineArgs(Argc, pArgv, &GenlockSampleArgs);
    LOG_AND_EXIT_ON_ERROR(Result, "GetCmdlineArgs");

    ZeroMemory(&CtlInitArgs, sizeof(ctl_init_args_t));
    CtlInitArgs.AppVersion = CTL_MAKE_VERSION(CTL_IMPL_MAJOR_VERSION, CTL_IMPL_MINOR_VERSION);
    CtlInitArgs.flags      = 0;
    CtlInitArgs.Size       = sizeof(CtlInitArgs);
    CtlInitArgs.Version    = 0;

    Result = ctlInit(&CtlInitArgs, &hAPIHandle);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlInit");

    // Initialization successful
    // Get the list of Intel Adapters
    Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, NULL);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");

    hDevices = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * AdapterCount);
    EXIT_ON_MEM_ALLOC_FAILURE(hDevices, "hDevices");

    Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");

    if (true == IsGetVBlankTs)
    {
        Result = GetVblankTimestamp(hDevices, AdapterCount);
        if (CTL_RESULT_SUCCESS == Result)
            goto Exit;
        LOG_AND_EXIT_ON_ERROR(Result, "GetVblankTimestamp");
    }

    Result = TestDisplayGenlock(hDevices, AdapterCount, GenlockSampleArgs);
    LOG_AND_EXIT_ON_ERROR(Result, "TestDisplayGenlock");

Exit:
    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDevices);

    printf("Overall test result is 0x%X\n", GResult);
    return GResult;
}
