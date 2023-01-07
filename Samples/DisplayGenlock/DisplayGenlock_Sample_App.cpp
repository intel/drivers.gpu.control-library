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

#define CTL_APIEXPORT // caller of control API DLL shall define this before including igcl_api.h

#include "igcl_api.h"
#include "GenericIGCLApp.h"

typedef struct _genlock_sample_args
{
    bool IsGenlockPrimary;
    ctl_genlock_operation_t Operation;
    ctl_display_output_types_t PortType;
} genlock_sample_args;

ctl_result_t GResult = CTL_RESULT_SUCCESS;

/***************************************************************
 * @brief PrintTopology
 * Print topology
 * @param pGenlockArgs
 * @return void
 ***************************************************************/
void PrintTopology(ctl_genlock_args_t **pGenlockArgs, uint32_t AdapterCount)
{
    printf("========= Genlock Topology =========\n");
    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        printf("Adapter %u:\n", AdapterIndex);
        printf("\tIsGenlockEnabled : %s\n", pGenlockArgs[AdapterIndex]->IsGenlockEnabled ? "true" : "false");
        printf("\tIsGenlockPossible : %s\n", pGenlockArgs[AdapterIndex]->IsGenlockPossible ? "true" : "false");
        if (true == pGenlockArgs[AdapterIndex]->IsGenlockEnabled)
        {
            printf("\tIsPrimaryGenlockSystem : %s\n", pGenlockArgs[AdapterIndex]->GenlockTopology.IsPrimaryGenlockSystem ? "true" : "false");
            printf("\tNumber of Genlock displays : %d\n", pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays);
            for (int8_t DisplayIndex = 0; DisplayIndex < pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays; DisplayIndex++)
            {
                printf("\tDisplay %u:\n", DisplayIndex);
                printf("\t\tDisplay output handle : %p\n", pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockDisplayInfo[DisplayIndex].hDisplayOutput);
                printf("\t\tPrimary pipe : %s\n", pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockDisplayInfo[DisplayIndex].IsPrimary ? "true" : "false");
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
    printf("\nUsage: %s <Genlock mode> <port type> <operation>\n", pArgv[0]);
    printf("\nGenlock mode - primary | secondary\n");
    printf("\tprimary - primary system providing ref clock and ref sync\n");
    printf("\tsecondary - secondary system referring ref clock and ref sync from primary\n");
    printf("port type - dp | hdmi\n");
    printf("\tdp - Display Port\n");
    printf("\thdmi - HDMI\n");
    printf("operation - timing | validate | enable | topology | disable\n");
    printf("\ttiming - get target mode timing details supported by each display\n");
    printf("\tvalidate - validate Genlock configuration\n");
    printf("\tenable - enable Genlock\n");
    printf("\ttopology - get current Genlock topology\n");
    printf("\tdisable - disable Genlock\n");
}

/***************************************************************
 * @brief GetCmdlineArgs
 * Get command line arguments
 * @param argc, pArgv[], pGenlockSampleArgs
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t GetCmdlineArgs(int argc, char *pArgv[], genlock_sample_args *pGenlockSampleArgs)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    switch (argc)
    {
        case 4:
            if (0 == strcmp(pArgv[1], "secondary"))
            {
                pGenlockSampleArgs->IsGenlockPrimary = false;
            }
            else if (0 == strcmp(pArgv[1], "primary"))
            {
                pGenlockSampleArgs->IsGenlockPrimary = true;
            }
            else
            {
                PrintUsage(pArgv);
                Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
            }

            if (0 == strcmp(pArgv[2], "dp"))
            {
                pGenlockSampleArgs->PortType = CTL_DISPLAY_OUTPUT_TYPES_DISPLAYPORT;
            }
            else if (0 == strcmp(pArgv[2], "hdmi"))
            {
                pGenlockSampleArgs->PortType = CTL_DISPLAY_OUTPUT_TYPES_HDMI;
            }
            else
            {
                PrintUsage(pArgv);
                Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
            }

            if (0 == strcmp(pArgv[3], "timing"))
            {
                pGenlockSampleArgs->Operation = CTL_GENLOCK_OPERATION_GET_TIMING_DETAILS;
            }
            else if (0 == strcmp(pArgv[3], "validate"))
            {
                pGenlockSampleArgs->Operation = CTL_GENLOCK_OPERATION_VALIDATE;
            }
            else if (0 == strcmp(pArgv[3], "enable"))
            {
                pGenlockSampleArgs->Operation = CTL_GENLOCK_OPERATION_ENABLE;
            }
            else if (0 == strcmp(pArgv[3], "disable"))
            {
                pGenlockSampleArgs->Operation = CTL_GENLOCK_OPERATION_DISABLE;
            }
            else if (0 == strcmp(pArgv[3], "topology"))
            {
                pGenlockSampleArgs->Operation = CTL_GENLOCK_OPERATION_GET_TOPOLOGY;
            }
            else
            {
                PrintUsage(pArgv);
                Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
            }
            break;

        default:
            PrintUsage(pArgv);
            Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
            break;
    }

    return Result;
}

/***************************************************************
 * @brief FindBestCommonTiming
 * Find best common target mode timing and fill it to CommonTargetMode
 * @param pGenlockArgs, AdapterCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t FindBestCommonTiming(ctl_genlock_args_t **pGenlockArgs, uint32_t AdapterCount)
{
    ctl_result_t Result                                   = CTL_RESULT_SUCCESS;
    ctl_genlock_target_mode_list_t *pGenlockModeListFixed = NULL;
    ctl_genlock_target_mode_list_t *pGenlockModeList      = NULL;
    ctl_display_timing_t *pTargetModes                    = NULL;
    ctl_display_timing_t *pPreferredTargetMode            = NULL;
    uint8_t ModeIndex                                     = 0;

    // Find the best mode timing in the first display
    pGenlockModeListFixed = &pGenlockArgs[0]->GenlockTopology.pGenlockModeList[0];
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
    pGenlockArgs[0]->GenlockTopology.CommonTargetMode = *pPreferredTargetMode;

    // Check if the preferred mode timing is common in the first adapter
    for (uint8_t DisplayIndex = 1; DisplayIndex < pGenlockArgs[0]->GenlockTopology.NumGenlockDisplays; DisplayIndex++)
    {
        pGenlockModeList = &pGenlockArgs[0]->GenlockTopology.pGenlockModeList[DisplayIndex];
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
        for (uint8_t DisplayIndex = 0; DisplayIndex < pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays; DisplayIndex++)
        {
            pGenlockModeList = &pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockModeList[DisplayIndex];
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
            pGenlockArgs[AdapterIndex]->GenlockTopology.CommonTargetMode = *pPreferredTargetMode;
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
ctl_result_t TestGenlockGetTimingDetails(ctl_device_adapter_handle_t *hDevices, ctl_genlock_args_t **pGenlockArgs, uint32_t AdapterCount)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;
    ctl_genlock_target_mode_list_t *pGenlockModeList;
    ctl_device_adapter_handle_t hFailureDeviceAdapter = NULL;

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        pGenlockArgs[AdapterIndex]->Operation = CTL_GENLOCK_OPERATION_GET_TIMING_DETAILS;
        for (uint8_t ModeListIndex = 0; ModeListIndex < pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays; ModeListIndex++)
        {
            pGenlockModeList           = &pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockModeList[ModeListIndex];
            pGenlockModeList->NumModes = 0;
        }
    }

    // Get number of target modes
    Result = ctlGetSetDisplayGenlock(hDevices, pGenlockArgs, AdapterCount, &hFailureDeviceAdapter);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplayGenlock");

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        for (uint8_t ModeListIndex = 0; ModeListIndex < pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays; ModeListIndex++)
        {
            pGenlockModeList = &pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockModeList[ModeListIndex];
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
ctl_result_t TestGenlockValidate(ctl_device_adapter_handle_t *hDevices, ctl_genlock_args_t **pGenlockArgs, uint32_t AdapterCount)
{
    ctl_result_t Result                               = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t hFailureDeviceAdapter = NULL;

    // Get timing details
    Result = TestGenlockGetTimingDetails(hDevices, pGenlockArgs, AdapterCount);
    LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockGetTimingDetails");

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        pGenlockArgs[AdapterIndex]->Operation = CTL_GENLOCK_OPERATION_VALIDATE;
        if (true == pGenlockArgs[AdapterIndex]->GenlockTopology.IsPrimaryGenlockSystem)
        {
            pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockDisplayInfo[0].IsPrimary = true;
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
        printf("Adapter[%u]: IsGenlockPossible : %s\n", AdapterIndex, pGenlockArgs[AdapterIndex]->IsGenlockPossible ? "true" : "false");
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
ctl_result_t TestGenlockGetTopology(ctl_device_adapter_handle_t *hDevices, ctl_genlock_args_t **pGenlockArgs, uint32_t AdapterCount)
{
    ctl_result_t Result                               = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t hFailureDeviceAdapter = NULL;

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        pGenlockArgs[AdapterIndex]->Operation = CTL_GENLOCK_OPERATION_GET_TOPOLOGY;
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
ctl_result_t TestGenlockEnable(ctl_device_adapter_handle_t *hDevices, ctl_genlock_args_t **pGenlockArgs, uint32_t AdapterCount)
{
    ctl_result_t Result                               = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t hFailureDeviceAdapter = NULL;

    Result = TestGenlockValidate(hDevices, pGenlockArgs, AdapterCount);
    LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockValidate");

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        pGenlockArgs[AdapterIndex]->Operation = CTL_GENLOCK_OPERATION_ENABLE;
        if (true == pGenlockArgs[AdapterIndex]->GenlockTopology.IsPrimaryGenlockSystem)
        {
            pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockDisplayInfo[0].IsPrimary = true;
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
ctl_result_t TestGenlockDisable(ctl_device_adapter_handle_t *hDevices, ctl_genlock_args_t **pGenlockArgs, uint32_t AdapterCount)
{
    ctl_result_t Result                               = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t hFailureDeviceAdapter = NULL;

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        pGenlockArgs[AdapterIndex]->Operation = CTL_GENLOCK_OPERATION_DISABLE;
    }

    Result = ctlGetSetDisplayGenlock(hDevices, pGenlockArgs, AdapterCount, &hFailureDeviceAdapter);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplayGenlock");

Exit:
    return Result;
}

/***************************************************************
 * @brief InitGenlockArgs
 * Initialize Genlock Arguments
 * @param hDevices, AdapterCount, GenlockSampleArgs, pGenlockArgs
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t InitGenlockArgs(ctl_device_adapter_handle_t *hDevices, uint32_t AdapterCount, const genlock_sample_args GenlockSampleArgs, ctl_genlock_args_t **pGenlockArgs)
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
        ZeroMemory(pGenlockArgs[AdapterIndex], sizeof(ctl_genlock_args_t));
        pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays = 0;
        pGenlockArgs[AdapterIndex]->Operation                          = CTL_GENLOCK_OPERATION_VALIDATE;
    }
    Result = ctlGetSetDisplayGenlock(hDevices, pGenlockArgs, AdapterCount, &hFailureDeviceAdapter);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplayGenlock");

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        ActiveDisplayCount                                             = 0;
        MaxNumDisplayOutputs                                           = pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays;
        pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays = 0;

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

        if (0 == ActiveDisplayCount)
        {
            printf("There are no active displays for the selected port. ActiveDisplayCount is %d.\n", ActiveDisplayCount);
            Result = CTL_RESULT_ERROR_DISPLAY_NOT_ATTACHED;
            EXIT_ON_ERROR(Result);
        }

        pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays     = ActiveDisplayCount;
        pGenlockArgs[AdapterIndex]->GenlockTopology.IsPrimaryGenlockSystem = GenlockSampleArgs.IsGenlockPrimary;

        // Allocate dynamic memories
        pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockDisplayInfo =
        (ctl_genlock_display_info_t *)malloc(sizeof(ctl_genlock_display_info_t) * pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays);
        EXIT_ON_MEM_ALLOC_FAILURE(pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockDisplayInfo, "pGenlockArgs[AdapterIndex].GenlockTopology.pGenlockDisplayInfo");
        memset(pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockDisplayInfo, 0, sizeof(ctl_genlock_display_info_t) * pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays);

        pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockModeList =
        (ctl_genlock_target_mode_list_t *)malloc(sizeof(ctl_genlock_target_mode_list_t) * pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays);
        EXIT_ON_MEM_ALLOC_FAILURE(pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockModeList, "pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockModeList");
        memset(pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockModeList, 0, sizeof(ctl_genlock_target_mode_list_t) * pGenlockArgs[AdapterIndex]->GenlockTopology.NumGenlockDisplays);

        for (uint8_t DisplayIndex = 0; DisplayIndex < ActiveDisplayCount; DisplayIndex++)
        {
            pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockDisplayInfo[DisplayIndex].hDisplayOutput = hActiveDisplayOutputs[DisplayIndex];
            pGenlockArgs[AdapterIndex]->GenlockTopology.pGenlockModeList[DisplayIndex].hDisplayOutput    = hActiveDisplayOutputs[DisplayIndex];
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
 * @brief TestDisplayGenlock
 * Test Display Genlock
 * @param hDevices, AdapterCount, GenlockSampleArgs
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestDisplayGenlock(ctl_device_adapter_handle_t *hDevices, uint32_t AdapterCount, genlock_sample_args GenlockSampleArgs)
{
    ctl_result_t Result                             = CTL_RESULT_SUCCESS;
    ctl_genlock_args_t *pGenlockArgs                = NULL;
    ctl_device_adapter_handle_t hPrimaryAdapter     = NULL;
    ctl_device_adapter_handle_t *hSecondaryAdapters = NULL;
    uint8_t SecondaryAdapterCount                   = 0;

    pGenlockArgs = (ctl_genlock_args_t *)malloc(sizeof(ctl_genlock_args_t) * AdapterCount);
    EXIT_ON_MEM_ALLOC_FAILURE(pGenlockArgs, "pGenlockArgs");

    switch (GenlockSampleArgs.Operation)
    {
        case CTL_GENLOCK_OPERATION_GET_TIMING_DETAILS:
            Result = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, &pGenlockArgs);
            LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
            Result = TestGenlockGetTimingDetails(hDevices, &pGenlockArgs, AdapterCount);
            break;
        case CTL_GENLOCK_OPERATION_VALIDATE:
            Result = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, &pGenlockArgs);
            LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
            Result = TestGenlockValidate(hDevices, &pGenlockArgs, AdapterCount);
            break;
        case CTL_GENLOCK_OPERATION_ENABLE:
            // The primary adapter must be enabled first before other secondary adapters are enabled
            if (true == GenlockSampleArgs.IsGenlockPrimary)
            {
                // Enable primary target on the primary adapter#0
                Result = InitGenlockArgs(hDevices, 1, GenlockSampleArgs, &pGenlockArgs);
                LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");

                // Select pipe#0 as primary pipe
                pGenlockArgs[0].GenlockTopology.pGenlockDisplayInfo[0].IsPrimary = true;

                Result = TestGenlockEnable(hDevices, &pGenlockArgs, AdapterCount);
                LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockEnable");

                if (AdapterCount >= 2)
                {
                    // Other adapters are secondary systems for Genlock
                    pGenlockArgs[0].GenlockTopology.pGenlockDisplayInfo[0].IsPrimary = false;
                    GenlockSampleArgs.IsGenlockPrimary                               = false;

                    Result = InitGenlockArgs(&hDevices[1], AdapterCount - 1, GenlockSampleArgs, &pGenlockArgs);
                    LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
                    Result = TestGenlockEnable(&hDevices[1], &pGenlockArgs, AdapterCount - 1);
                }
            }
            else
            {
                Result = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, &pGenlockArgs);
                LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
                Result = TestGenlockEnable(hDevices, &pGenlockArgs, AdapterCount);
            }
            break;
        case CTL_GENLOCK_OPERATION_DISABLE:
            // The primary adapter must be disabled last after secondary adapters are disabled
            if (true == GenlockSampleArgs.IsGenlockPrimary)
            {
                if (AdapterCount >= 2)
                {
                    // Secondary adapters should be disabled first
                    // Get topology to check which adapter is a primary
                    GenlockSampleArgs.Operation = CTL_GENLOCK_OPERATION_GET_TOPOLOGY;
                    Result                      = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, &pGenlockArgs);
                    LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
                    Result = TestGenlockGetTopology(hDevices, &pGenlockArgs, AdapterCount);
                    LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockGetTopology");

                    hSecondaryAdapters    = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * AdapterCount - 1);
                    SecondaryAdapterCount = 0;

                    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
                    {
                        if (true == pGenlockArgs[AdapterIndex].GenlockTopology.IsPrimaryGenlockSystem)
                        {
                            hPrimaryAdapter = hDevices[AdapterIndex];
                        }
                        else if (true == pGenlockArgs[AdapterIndex].IsGenlockEnabled)
                        {
                            hSecondaryAdapters[SecondaryAdapterCount++] = hDevices[AdapterIndex];
                        }
                    }

                    GenlockSampleArgs.Operation        = CTL_GENLOCK_OPERATION_DISABLE;
                    GenlockSampleArgs.IsGenlockPrimary = false;
                    // Disable secondary adapters
                    if (0 != SecondaryAdapterCount)
                    {
                        Result = InitGenlockArgs(hSecondaryAdapters, SecondaryAdapterCount, GenlockSampleArgs, &pGenlockArgs);
                        LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
                        Result = TestGenlockDisable(hSecondaryAdapters, &pGenlockArgs, SecondaryAdapterCount);
                        LOG_AND_EXIT_ON_ERROR(Result, "TestGenlockEnable");
                    }
                    CTL_FREE_MEM(hSecondaryAdapters);
                    GenlockSampleArgs.IsGenlockPrimary = true;
                }
                else
                {
                    hPrimaryAdapter = hDevices[0];
                }

                // Disable primary adapter
                Result = InitGenlockArgs(&hPrimaryAdapter, 1, GenlockSampleArgs, &pGenlockArgs);
                LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
                Result = TestGenlockDisable(&hPrimaryAdapter, &pGenlockArgs, 1);
            }
            else
            {
                Result = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, &pGenlockArgs);
                LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
                Result = TestGenlockDisable(hDevices, &pGenlockArgs, AdapterCount);
            }
            break;
        case CTL_GENLOCK_OPERATION_GET_TOPOLOGY:
            Result = InitGenlockArgs(hDevices, AdapterCount, GenlockSampleArgs, &pGenlockArgs);
            LOG_AND_EXIT_ON_ERROR(Result, "InitGenlockArgs");
            Result = TestGenlockGetTopology(hDevices, &pGenlockArgs, AdapterCount);
            break;
        default:
            Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
            EXIT_ON_ERROR(Result);
            break;
    }

Exit:
    // Free dynamic memories
    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
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
    CTL_FREE_MEM(pGenlockArgs);

    return Result;
}

/***************************************************************
 * @brief Main Function
 * @param
 * @return ctl_result_t
 ***************************************************************/
int main(int argc, char *pArgv[])
{
    ctl_result_t Result                   = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hDevices = nullptr;
    uint32_t AdapterCount                 = 0;
    ctl_init_args_t CtlInitArgs;
    ctl_api_handle_t hAPIHandle           = nullptr;
    genlock_sample_args GenlockSampleArgs = { 0 };
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Get cmdline arguments
    Result = GetCmdlineArgs(argc, pArgv, &GenlockSampleArgs);
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

    Result = TestDisplayGenlock(hDevices, AdapterCount, GenlockSampleArgs);
    LOG_AND_EXIT_ON_ERROR(Result, "TestDisplayGenlock");

Exit:
    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDevices);

    printf("Overall test result is 0x%X\n", GResult);
    return GResult;
}
