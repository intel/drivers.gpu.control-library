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
#include <list>
using namespace std;

#define CTL_APIEXPORT // caller of control API DLL shall define this before including igcl_api.h

#include "igcl_api.h"
#include "GenericIGCLApp.h"

#define NUMBER_OF_PRIMARY_ADAPTERS 1

typedef struct _genlock_sample_args
{
    bool IsGenlockPrimary;
    ctl_genlock_operation_t Operation;
    ctl_display_output_types_t PortType;
    uint32_t PrimaryBusId;
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
    PRINT_LOGS("========= Genlock Topology =========");
    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        APP_LOG_INFO("Adapter %u:", AdapterIndex);
        APP_LOG_INFO("\tIsGenlockEnabled : %s", pGenlockArgs[AdapterIndex].IsGenlockEnabled ? "true" : "false");
        APP_LOG_INFO("\tIsGenlockPossible : %s", pGenlockArgs[AdapterIndex].IsGenlockPossible ? "true" : "false");
        if (true == pGenlockArgs[AdapterIndex].IsGenlockEnabled)
        {
            APP_LOG_INFO("\tIsPrimaryGenlockSystem : %s", pGenlockArgs[AdapterIndex].GenlockTopology.IsPrimaryGenlockSystem ? "true" : "false");
            APP_LOG_INFO("\tNumber of Genlock displays : %d", pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays);
            for (int8_t DisplayIndex = 0; DisplayIndex < pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays; DisplayIndex++)
            {
                APP_LOG_INFO("\tDisplay %u:", DisplayIndex);
                APP_LOG_INFO("\t\tDisplay output handle : %p", pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo[DisplayIndex].hDisplayOutput);
                APP_LOG_INFO("\t\tPrimary pipe : %s", pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo[DisplayIndex].IsPrimary ? "true" : "false");
            }
        }
    }
    PRINT_LOGS("====================================");
}

/***************************************************************
 * @brief PrintUsage
 * Print usage of Genlock sample app
 * @param pArgv[]
 * @return void
 ***************************************************************/
void PrintUsage(char *pArgv[])
{
    APP_LOG_INFO("Genlock Sample Test Application.");
    APP_LOG_INFO("Usage: %s [Operation] <-g Genlock_Mode> <-d Display_Type> <-n Number_Of_Primary_Adapters>", pArgv[0]);
    APP_LOG_INFO("Operation");
    APP_LOG_INFO("\t-timing : Get target mode timing details supported by each display");
    APP_LOG_INFO("\t-validate : Validate Genlock configuration");
    APP_LOG_INFO("\t-enable : Enable Genlock");
    APP_LOG_INFO("\t-topology : Get current Genlock topology");
    APP_LOG_INFO("\t-disable : Disable Genlock");
    APP_LOG_INFO("\t-vblankts : Get VBlank Timestamp");
    APP_LOG_INFO("\t-help : Display usage");
    APP_LOG_INFO("-g [Genlock mode]");
    APP_LOG_INFO("\tprimary : primary system providing ref clock and ref sync (default)");
    APP_LOG_INFO("\tsecondary : secondary system referring ref clock and ref sync from primary");
    APP_LOG_INFO("-d [Display type]");
    APP_LOG_INFO("\tdp : Display Port (default)");
    APP_LOG_INFO("\thdmi : HDMI");
    APP_LOG_INFO("-p [PCIe BUS ID of Primary Adapter]");
    APP_LOG_INFO("\tDesignate PCIe BUS ID of adapter you want to set as a primary (default : 0)");
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
    pGenlockSampleArgs->PortType         = CTL_DISPLAY_OUTPUT_TYPES_DISPLAYPORT;
    pGenlockSampleArgs->IsGenlockPrimary = true;
    pGenlockSampleArgs->PrimaryBusId     = 0;

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
    APP_LOG_INFO("[in] Genlock Mode: %s", pGenlockSampleArgs->IsGenlockPrimary == true ? "primary" : "secondary");
    // Display Type option
    if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-d", OptionArg))
    {
        if (("" != OptionArg) && (strstr(OptionArg.c_str(), "hdmi") != NULL))
        {
            pGenlockSampleArgs->PortType = CTL_DISPLAY_OUTPUT_TYPES_HDMI;
        }
    }
    APP_LOG_INFO("[in] Display Type: %s", pGenlockSampleArgs->PortType == CTL_DISPLAY_OUTPUT_TYPES_HDMI ? "hdmi" : "dp");
    // Primary BUS ID option
    if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-p", OptionArg))
    {
        if (("" != OptionArg))
        {
            for (uint32_t Index = 0; Index < OptionArg.length(); Index++)
            {
                if (!isdigit(OptionArg[Index]))
                {
                    APP_LOG_ERROR("Invalid PCIe BUS ID.");
                    Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
                    EXIT_ON_ERROR(Result);
                }
            }
            pGenlockSampleArgs->PrimaryBusId = stoul(OptionArg, 0, 10);
        }
    }
    APP_LOG_INFO("[in] BUS ID of primary adapter: %u", pGenlockSampleArgs->PrimaryBusId);

Exit:
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
            APP_LOG_WARN("Invalid Display Count. Skipping display enumeration for adapter: %d", AdapterIndex);
            continue;
        }

        hDisplayOutput = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], &DisplayCount, hDisplayOutput);
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        APP_LOG_INFO("Adapter %u:", AdapterIndex);
        for (uint8_t DisplayIndex = 0; DisplayIndex < DisplayCount; DisplayIndex++)
        {
            ctl_display_properties_t stDisplayProperties = {};
            stDisplayProperties.Size                     = sizeof(ctl_display_properties_t);

            if (NULL == hDisplayOutput[DisplayIndex])
            {
                APP_LOG_ERROR("\thDisplayOutput[%d] is NULL.", DisplayIndex);
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
                APP_LOG_INFO("\tTarget ID: %d Child[%d] vblank timestamp: %I64d", stDisplayProperties.Os_display_encoder_handle.WindowsDisplayEncoderID, i, VblankTsArgs.VblankTS[i]);
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

    PRINT_LOGS("========= Genlock Validate =========");
    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        APP_LOG_INFO("Adapter[%u]: IsGenlockPossible : %s", AdapterIndex, pGenlockArgs[AdapterIndex].IsGenlockPossible ? "true" : "false");
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
            APP_LOG_ERROR("Invalid display count for adapter: %d", AdapterIndex);
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
                APP_LOG_ERROR("hDisplayOutput[%d] is NULL.", DisplayIndex);
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
            APP_LOG_ERROR("Adatper[%d] does not have any active displays for the selected display type. ActiveDisplayCount is %d.", AdapterIndex, ActiveDisplayCount);
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
        CTL_FREE_MEM(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo);
        pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo =
        (ctl_genlock_display_info_t *)malloc(sizeof(ctl_genlock_display_info_t) * pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays);
        EXIT_ON_MEM_ALLOC_FAILURE(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo, "pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo");
        memset(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo, 0, sizeof(ctl_genlock_display_info_t) * pGenlockArgs[AdapterIndex].GenlockTopology.NumGenlockDisplays);

        CTL_FREE_MEM(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockModeList);
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
 * @brief GenlockEnable
 * Enable Display Genlock
 * @param hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t GenlockEnable(ctl_device_adapter_handle_t *hDevices, uint32_t AdapterCount, genlock_sample_args GenlockSampleArgs, ctl_genlock_args_t *pGenlockArgs)
{
    ctl_result_t Result                             = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t PrimaryAdapter      = NULL;
    ctl_device_adapter_handle_t *hSecondaryAdapters = NULL;
    uint8_t PrimaryAdapterCount                     = 0;
    uint8_t SecondaryAdapterCount                   = 0;
    uint8_t SecondaryAdapterIndex                   = 0;

    // The primary adapter must be enabled before enabling other secondary adapters
    // Number of primary adapters should be one. Other adapters are configured as a secondary
    if (true == GenlockSampleArgs.IsGenlockPrimary)
    {
        PrimaryAdapterCount   = NUMBER_OF_PRIMARY_ADAPTERS;
        SecondaryAdapterCount = AdapterCount - PrimaryAdapterCount;
        hSecondaryAdapters    = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * SecondaryAdapterCount);
        for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
        {
            LUID deviceID;
            ctl_device_adapter_properties_t stdeviceAdapterProperties = { 0 };

            stdeviceAdapterProperties.Size           = sizeof(ctl_device_adapter_properties_t);
            stdeviceAdapterProperties.pDeviceID      = &deviceID;
            stdeviceAdapterProperties.device_id_size = sizeof(LUID);
            stdeviceAdapterProperties.Version        = 2;

            // Fetch device adapter properties into stdeviceAdapterProperties
            Result = ctlGetDeviceProperties(hDevices[AdapterIndex], &stdeviceAdapterProperties);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDeviceProperties");

            if (((GenlockSampleArgs.PrimaryBusId != 0) && (stdeviceAdapterProperties.adapter_bdf.bus == GenlockSampleArgs.PrimaryBusId)) ||
                ((GenlockSampleArgs.PrimaryBusId == 0) && (AdapterIndex == 0)))
            {
                PrimaryAdapter = hDevices[AdapterIndex];
            }
            else
            {
                hSecondaryAdapters[SecondaryAdapterIndex] = hDevices[AdapterIndex];
                SecondaryAdapterIndex++;
            }
        }

        // Enabling a primary adapter
        Result = InitGenlockArgs(&PrimaryAdapter, PrimaryAdapterCount, GenlockSampleArgs, pGenlockArgs);
        LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs for a primary adapter");

        Result = TestGenlockEnable(&PrimaryAdapter, pGenlockArgs, PrimaryAdapterCount);
        LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockEnable - Primary");

        // Enabling secondary adapters
        if (SecondaryAdapterCount > 0)
        {
            GenlockSampleArgs.IsGenlockPrimary = false;
            Result                             = InitGenlockArgs(hSecondaryAdapters, SecondaryAdapterCount, GenlockSampleArgs, pGenlockArgs);
            LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs for secondary adapters");

            Result = TestGenlockEnable(hSecondaryAdapters, pGenlockArgs, SecondaryAdapterCount);
            LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockEnable - Secondary");
        }
    }
    else
    {
        // Configuring all adapters as a secondary
        Result = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs);
        LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
        Result = TestGenlockEnable(hDevices, pGenlockArgs, AdapterCount);
        LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockEnable - Secondary Adapters");
    }

Exit:
    CTL_FREE_MEM(hSecondaryAdapters);

    return Result;
}

/***************************************************************
 * @brief GenlockDisable
 * Disable Display Genlock
 * @param hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t GenlockDisable(ctl_device_adapter_handle_t *hDevices, uint32_t AdapterCount, genlock_sample_args GenlockSampleArgs, ctl_genlock_args_t *pGenlockArgs)
{
    ctl_result_t Result                             = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hPrimaryAdapters   = NULL;
    ctl_device_adapter_handle_t *hSecondaryAdapters = NULL;
    uint8_t GenlockedAdapterCount                   = 0;
    uint8_t PrimaryAdapterCount                     = 0;
    uint8_t SecondaryAdapterCount                   = 0;

    // The primary adapter must be disabled last after secondary adapters are disabled
    if (true == GenlockSampleArgs.IsGenlockPrimary)
    {
        // Secondary adapters should be disabled firstly
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
            APP_LOG_ERROR("There is no Genlocked adapter.");
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

        CTL_FREE_MEM(hPrimaryAdapters);
    }
    else
    {
        Result = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs);
        LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
        Result = TestGenlockDisable(hDevices, pGenlockArgs, AdapterCount);
    }

Exit:
    CTL_FREE_MEM(hPrimaryAdapters);
    CTL_FREE_MEM(hSecondaryAdapters);

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
    ctl_result_t Result              = CTL_RESULT_SUCCESS;
    ctl_genlock_args_t *pGenlockArgs = NULL;

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
            Result = GenlockEnable(hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs);
            break;
        case CTL_GENLOCK_OPERATION_DISABLE:
            Result = GenlockDisable(hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs);
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

            for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
            {
                CTL_FREE_MEM(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo);
                CTL_FREE_MEM(pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockModeList);
            }
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

    try
    {
        Result = ctlInit(&CtlInitArgs, &hAPIHandle);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlInit");
    }
    catch (const std::bad_array_new_length &e)
    {
        APP_LOG_ERROR("%s ", e.what());
    }

    // Initialization successful
    // Get the list of Intel Adapters
    try
    {
        Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, NULL);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");
    }
    catch (const std::bad_array_new_length &e)
    {
        APP_LOG_ERROR("%s ", e.what());
    }

    hDevices = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * AdapterCount);
    EXIT_ON_MEM_ALLOC_FAILURE(hDevices, "hDevices");

    try
    {
        Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");
    }
    catch (const std::bad_array_new_length &e)
    {
        APP_LOG_ERROR("%s ", e.what());
    }

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

    APP_LOG_INFO("Overall test result is 0x%X", GResult);
    return GResult;
}
