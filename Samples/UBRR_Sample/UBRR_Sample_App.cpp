//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  UBRR_Sample_APP.cpp
 * @brief This file contains the 'main' function & the UBRR Sample APP. Program execution begins and ends there.
 *
 */

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define CTL_APIEXPORT // caller of control API DLL shall define this before
                      // including igcl_api.h
#include "igcl_api.h"
#include "GenericIGCLApp.h"

ctl_result_t GResult = CTL_RESULT_SUCCESS;

/***************************************************************
 * @brief EnumerateDisplayHandles
 * Only for demonstration purpose, API is called for each of the display output handle in below snippet.
 * User has to filter through the available display output handle and has to call the API with particular display output handle.
 * @param hDisplayOutput, DisplayCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t EnumerateDisplayHandles(ctl_display_output_handle_t *hDisplayOutput, uint32_t DisplayCount)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if (0 == DisplayCount)
    {
        APP_LOG_ERROR("Invalid Display Count ");
        goto Exit;
    }

    // Only for demonstration purpose, API is called for each of the display output handle in below snippet.
    // User has to filter through the available display output handle and has to call the API with particular display output handle.
    for (uint32_t DisplayIndex = 0; DisplayIndex < DisplayCount; DisplayIndex++)
    {
        ctl_power_optimization_caps_t PowerOptimizationCaps        = { 0 };
        ctl_power_optimization_settings_t PowerOptimizationSetting = { 0 };

        if (NULL == hDisplayOutput[DisplayIndex])
        {
            continue;
        }

        PowerOptimizationCaps.Size = sizeof(ctl_pfnGetPowerOptimizationCaps_t);
        Result                     = ctlGetPowerOptimizationCaps(hDisplayOutput[DisplayIndex], &PowerOptimizationCaps);

        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationCaps");

        if (PowerOptimizationCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_LRR)
        {
            PowerOptimizationSetting.PowerOptimizationFeature = CTL_POWER_OPTIMIZATION_FLAG_LRR;
            PowerOptimizationSetting.Size                     = sizeof(ctl_power_optimization_settings_t);
            Result                                            = ctlGetPowerOptimizationSetting(hDisplayOutput[DisplayIndex], &PowerOptimizationSetting);

            LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting");
        }

        if (PowerOptimizationSetting.FeatureSpecificData.LRRInfo.SupportedLRRTypes & CTL_POWER_OPTIMIZATION_LRR_FLAG_UBLRR)
        {
            if (PowerOptimizationSetting.FeatureSpecificData.LRRInfo.bRequirePSRDisable)
            {
                // Disable PSR.
            }

            PowerOptimizationSetting                          = { 0 };
            PowerOptimizationSetting.PowerOptimizationFeature = CTL_POWER_OPTIMIZATION_FLAG_LRR;

            PowerOptimizationSetting.FeatureSpecificData.LRRInfo.CurrentLRRTypes = CTL_POWER_OPTIMIZATION_LRR_FLAG_UBLRR;
            PowerOptimizationSetting.Enable                                      = TRUE;
            PowerOptimizationSetting.Size                                        = sizeof(ctl_power_optimization_settings_t);
            Result                                                               = ctlSetPowerOptimizationSetting(hDisplayOutput[DisplayIndex], &PowerOptimizationSetting);

            LOG_AND_EXIT_ON_ERROR(Result, "ctlSetPowerOptimizationSetting");
        }
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief EnumerateTargetDisplays
 * Enumerates all the possible target display's for the adapters
 * @param hDisplayOutput, AdapterCount, hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t EnumerateTargetDisplays(uint32_t AdapterCount, ctl_device_adapter_handle_t *hDevices)
{
    ctl_display_output_handle_t *hDisplayOutput = NULL;
    ctl_result_t Result                         = CTL_RESULT_SUCCESS;
    uint32_t DisplayCount                       = 0;

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        // enumerate all the possible target display's for the adapters
        // first step is to get the count
        DisplayCount = 0;

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], &DisplayCount, hDisplayOutput);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_WARN("ctlEnumerateDisplayOutputs returned failure code: 0x%X", Result);
            STORE_AND_RESET_ERROR(Result);
            continue;
        }
        else if (DisplayCount <= 0)
        {
            APP_LOG_WARN("Invalid Display Count. skipping display enumeration for adapter:%d", AdapterIndex);
            continue;
        }

        hDisplayOutput = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);

        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], &DisplayCount, hDisplayOutput);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        // Only for demonstration purpose, API is called for each of the display output handle in below snippet.
        // User has to filter through the available display output handle and has to call the API with particular display output handle.
        Result = EnumerateDisplayHandles(hDisplayOutput, DisplayCount);
        STORE_AND_RESET_ERROR(Result);

        CTL_FREE_MEM(hDisplayOutput);
    }

Exit:

    CTL_FREE_MEM(hDisplayOutput);
    return Result;
}

/***************************************************************
 * @brief
 * Main Function which calls the Sample UBBR API
 * @param
 * @return int
 ***************************************************************/
int main()
{
    ctl_result_t Result                   = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hDevices = NULL;
    // Get a handle to the DLL module.
    uint32_t AdapterCount = 0;
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    ctl_init_args_t CtlInitArgs;
    ctl_api_handle_t hAPIHandle;

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
        Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);
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

    Result = EnumerateTargetDisplays(AdapterCount, hDevices);

    if (CTL_RESULT_SUCCESS != Result)
    {
        APP_LOG_ERROR("EnumerateTargetDisplays returned failure code: 0x%X", Result);
        STORE_AND_RESET_ERROR(Result);
    }

Exit:

    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDevices);
    APP_LOG_INFO("Overrall test result is 0x%X", GResult);
    return GResult;
}