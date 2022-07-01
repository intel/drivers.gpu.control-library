//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  Scaling_APP.cpp
 * @brief This file contains the 'main' function. Program execution begins and ends there.
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
 * @brief
 * Scaling Test
 * @param hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t ScalingTest(ctl_device_adapter_handle_t hDevices)
{
    ctl_display_output_handle_t *hDisplayOutput       = NULL;
    uint32_t DisplayCount                             = 0;
    ctl_result_t Result                               = CTL_RESULT_SUCCESS;
    ctl_retro_scaling_caps_t RetroScalingCaps         = { 0 };
    ctl_retro_scaling_settings_t RetroScalingSettings = { 0 };
    ctl_scaling_caps_t ScalingCaps                    = { 0 };
    ctl_scaling_settings_t ScalingSetting             = { 0 };
    bool ModeSet;

    // Test Retro scaling
    // Retro scaling is adapter capability
    RetroScalingSettings  = { 0 };
    RetroScalingCaps      = { 0 };
    RetroScalingCaps.Size = sizeof(ctl_retro_scaling_caps_t);

    Result = ctlGetSupportedRetroScalingCapability(hDevices, &RetroScalingCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSupportedRetroScalingCapability");

    printf("ctlGetSupportedRetroScalingCapability returned Caps: 0x%X\n", RetroScalingCaps.SupportedRetroScaling);

    // Test Integer Scaling
    RetroScalingSettings.Enable           = true;
    RetroScalingSettings.RetroScalingType = RetroScalingSettings.RetroScalingType | CTL_RETRO_SCALING_TYPE_FLAG_INTEGER;
    RetroScalingSettings.Size             = sizeof(ctl_retro_scaling_settings_t);

    Result = ctlGetSetRetroScaling(hDevices, &RetroScalingSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetRetroScaling");

    // Check if applied correctly
    RetroScalingSettings                  = { 0 };
    RetroScalingSettings.Get              = true;
    RetroScalingSettings.RetroScalingType = RetroScalingSettings.RetroScalingType | CTL_RETRO_SCALING_TYPE_FLAG_INTEGER;
    RetroScalingSettings.Size             = sizeof(ctl_retro_scaling_settings_t);

    Result = ctlGetSetRetroScaling(hDevices, &RetroScalingSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetRetroScaling");

    printf("ctlGetSetRetroScaling returned Enable: 0x%X type:0x%x\n", RetroScalingSettings.Enable, RetroScalingSettings.RetroScalingType);

    // Enumerate all the possible target display's for the adapters
    Result = ctlEnumerateDisplayOutputs(hDevices, &DisplayCount, hDisplayOutput);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

    if (DisplayCount <= 0)
    {
        printf("Invalid Display Count\n");
        goto Exit;
    }

    hDisplayOutput = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
    EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

    Result = ctlEnumerateDisplayOutputs(hDevices, &DisplayCount, hDisplayOutput);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

    // Only for demonstration purpose, API is called for each of the display output handle in below snippet.
    // User has to filter through the available display output handle and has to call the API with particular display output handle.
    for (uint32_t i = 0; i < DisplayCount; i++)
    {
        ScalingCaps      = { 0 };
        ScalingSetting   = { 0 };
        ScalingCaps.Size = sizeof(ctl_scaling_caps_t);

        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput[i], "hDisplayOutput");

        Result = ctlGetSupportedScalingCapability(hDisplayOutput[i], &ScalingCaps);

        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlGetSupportedScalingCapability returned failure code: 0x%X\n", Result);
            STORE_AND_RESET_ERROR(Result);
            continue;
        }

        printf("ctlGetSupportedScalingCapability returned caps: 0x%X\n", ScalingCaps.SupportedScaling);

        if (0 != ScalingCaps.SupportedScaling)
        {
            ScalingSetting.Size = sizeof(ctl_scaling_settings_t);
            Result              = ctlGetCurrentScaling(hDisplayOutput[i], &ScalingSetting);

            if (CTL_RESULT_SUCCESS != Result)
            {
                printf("ctlGetCurrentScaling returned failure code: 0x%X\n", Result);
                STORE_AND_RESET_ERROR(Result);
                continue;
            }
            printf("ctlGetCurrentScaling returned Enable: 0x%X type:0x%x\n", ScalingSetting.Enable, ScalingSetting.ScalingType);
        }

        // fill custom scaling details only if it is supported
        if (0x1F == ScalingCaps.SupportedScaling)
        {
            // check if hardware modeset required to apply custom scaling
            ModeSet = ((TRUE == ScalingSetting.Enable) && (CTL_SCALING_TYPE_FLAG_CUSTOM == ScalingSetting.ScalingType)) ? FALSE : TRUE;
            // filling custom scaling details
            ScalingSetting                 = { 0 };
            ScalingSetting.Enable          = true;
            ScalingSetting.ScalingType     = CTL_SCALING_TYPE_FLAG_CUSTOM;
            ScalingSetting.Size            = sizeof(ctl_scaling_settings_t);
            ScalingSetting.HardwareModeSet = (TRUE == ModeSet) ? TRUE : FALSE;
            ScalingSetting.CustomScalingX  = 1000;
            ScalingSetting.CustomScalingY  = 1000;
        }
        Result = ctlSetCurrentScaling(hDisplayOutput[i], &ScalingSetting);

        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlSetCurrentScaling returned failure code: 0x%X\n", Result);
            STORE_AND_RESET_ERROR(Result);
            continue;
        }
        // check if the applied scaling was successful
        ScalingSetting      = { 0 };
        ScalingSetting.Size = sizeof(ctl_scaling_settings_t);
        Result              = ctlGetCurrentScaling(hDisplayOutput[i], &ScalingSetting);
        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlGetCurrentScaling returned failure code: 0x%X\n", Result);
            STORE_AND_RESET_ERROR(Result);
            continue;
        }
        printf("ctlGetCurrentScaling returned Enable: 0x%X type:0x%x\n", ScalingSetting.Enable, ScalingSetting.ScalingType);
    }

Exit:
    CTL_FREE_MEM(hDisplayOutput);
    return Result;
}

/***************************************************************
 * @brief Main Function which calls the Scaling API
 * main
 * @param
 * @return
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

    ctl_retro_scaling_caps_t RetroScalingCaps         = { 0 };
    ctl_retro_scaling_settings_t RetroScalingSettings = { 0 };
    ctl_scaling_caps_t ScalingCaps                    = { 0 };
    ctl_scaling_settings_t ScalingSetting             = { 0 };

    CtlInitArgs.AppVersion = CTL_MAKE_VERSION(CTL_IMPL_MAJOR_VERSION, CTL_IMPL_MINOR_VERSION);
    CtlInitArgs.flags      = 0;
    CtlInitArgs.Size       = sizeof(CtlInitArgs);
    CtlInitArgs.Version    = 0;

    Result = ctlInit(&CtlInitArgs, &hAPIHandle);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlInit");

    // Initialization successful
    // Get the list of Intel Adapters
    Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");

    hDevices = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * AdapterCount);
    EXIT_ON_MEM_ALLOC_FAILURE(hDevices, "hDevices");

    Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");

    for (uint32_t i = 0; i < AdapterCount; i++)
    {
        Result = ScalingTest(hDevices[i]);

        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlGetCurrentScaling returned failure code: 0x%X\n", Result);
            STORE_AND_RESET_ERROR(Result);
            continue;
        }
    }

Exit:

    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDevices);
    printf("Overrall test result is 0x%X\n", GResult);
    return GResult;
}