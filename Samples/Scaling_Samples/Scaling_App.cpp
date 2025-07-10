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
ctl_result_t ScalingTest(ctl_device_adapter_handle_t hDevices, uint8_t ScaleType, uint32_t PerX = 0, uint32_t PerY = 0)
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

    APP_LOG_INFO("ctlGetSupportedRetroScalingCapability returned Caps: 0x%X", RetroScalingCaps.SupportedRetroScaling);

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

    APP_LOG_INFO("ctlGetSetRetroScaling returned Enable: 0x%X type:0x%x", RetroScalingSettings.Enable, RetroScalingSettings.RetroScalingType);

    // Enumerate all the possible target display's for the adapters
    Result = ctlEnumerateDisplayOutputs(hDevices, &DisplayCount, hDisplayOutput);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

    if (DisplayCount <= 0)
    {
        APP_LOG_ERROR("Invalid Display Count");
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
            APP_LOG_WARN("ctlGetSupportedScalingCapability returned failure code: 0x%X", Result);
            STORE_AND_RESET_ERROR(Result);
            continue;
        }

        APP_LOG_INFO("ctlGetSupportedScalingCapability returned caps: 0x%X", ScalingCaps.SupportedScaling);

        PRINT_LOGS("******* Supported Scaling types ********");

        if (CTL_SCALING_TYPE_FLAG_IDENTITY & ScalingCaps.SupportedScaling)
        {
            APP_LOG_INFO("CTL_SCALING_TYPE_FLAG_IDENTITY(1) is supported");
        }
        if (CTL_SCALING_TYPE_FLAG_CENTERED & ScalingCaps.SupportedScaling)
        {
            APP_LOG_INFO("CTL_SCALING_TYPE_FLAG_CENTERED(2) is supported");
        }
        if (CTL_SCALING_TYPE_FLAG_STRETCHED & ScalingCaps.SupportedScaling)
        {
            APP_LOG_INFO("CTL_SCALING_TYPE_FLAG_STRETCHED(4) is supported");
        }
        if (CTL_SCALING_TYPE_FLAG_ASPECT_RATIO_CENTERED_MAX & ScalingCaps.SupportedScaling)
        {
            APP_LOG_INFO("CTL_SCALING_TYPE_FLAG_ASPECT_RATIO_CENTERED_MAX(8) is supported");
        }
        if (CTL_SCALING_TYPE_FLAG_CUSTOM & ScalingCaps.SupportedScaling)
        {
            APP_LOG_INFO("CTL_SCALING_TYPE_FLAG_CUSTOM(16) is supported");
        }

        if (0 != ScalingCaps.SupportedScaling)
        {
            ScalingSetting.Size    = sizeof(ctl_scaling_settings_t);
            ScalingSetting.Version = 1;
            Result                 = ctlGetCurrentScaling(hDisplayOutput[i], &ScalingSetting);

            if (CTL_RESULT_SUCCESS != Result)
            {
                APP_LOG_WARN("ctlGetCurrentScaling returned failure code: 0x%X", Result);
                STORE_AND_RESET_ERROR(Result);
                continue;
            }
            APP_LOG_INFO("ctlGetCurrentScaling returned Enable: 0x%X ScalingType:0x%x PreferredScalingType:0x%x", ScalingSetting.Enable, ScalingSetting.ScalingType,
                         ScalingSetting.PreferredScalingType);
        }

        // fill custom scaling details only if it is supported
        if ((CTL_SCALING_TYPE_FLAG_CUSTOM & ScalingCaps.SupportedScaling) && (CTL_SCALING_TYPE_FLAG_CUSTOM == ScaleType))
        {
            // check if hardware modeset required to apply custom scaling
            ModeSet = ((TRUE == ScalingSetting.Enable) && (CTL_SCALING_TYPE_FLAG_CUSTOM == ScalingSetting.ScalingType)) ? FALSE : TRUE;
            // filling custom scaling details
            ScalingSetting                 = { 0 };
            ScalingSetting.Enable          = true;
            ScalingSetting.ScalingType     = CTL_SCALING_TYPE_FLAG_CUSTOM;
            ScalingSetting.Size            = sizeof(ctl_scaling_settings_t);
            ScalingSetting.HardwareModeSet = (TRUE == ModeSet) ? TRUE : FALSE;
            ScalingSetting.Version         = 1;

            APP_LOG_INFO("*** PerX:%d PerY:%d ***", PerX, PerY);

            ScalingSetting.CustomScalingX = PerX;
            ScalingSetting.CustomScalingY = PerY;
        }
        else
        {
            // filling custom scaling details
            ScalingSetting             = { 0 };
            ScalingSetting.Enable      = true;
            ScalingSetting.ScalingType = ScaleType;
            ScalingSetting.Size        = sizeof(ctl_scaling_settings_t);
            ScalingSetting.Version     = 1;
        }

        APP_LOG_INFO("ScalingSetting.ScalingType:%d", ScalingSetting.ScalingType);

        Result = ctlSetCurrentScaling(hDisplayOutput[i], &ScalingSetting);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_WARN("ctlSetCurrentScaling returned failure code: 0x%X", Result);
            STORE_AND_RESET_ERROR(Result);
            continue;
        }
        // check if the applied scaling was successful
        ScalingSetting         = { 0 };
        ScalingSetting.Size    = sizeof(ctl_scaling_settings_t);
        ScalingSetting.Version = 1;
        Result                 = ctlGetCurrentScaling(hDisplayOutput[i], &ScalingSetting);
        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_WARN("ctlGetCurrentScaling returned failure code: 0x%X", Result);
            STORE_AND_RESET_ERROR(Result);
            continue;
        }

        APP_LOG_INFO("ctlGetCurrentScaling returned Enable: 0x%X ScalingType:0x%x PreferredScalingType:0x%x", ScalingSetting.Enable, ScalingSetting.ScalingType, ScalingSetting.PreferredScalingType);

        if (CTL_SCALING_TYPE_FLAG_CUSTOM == ScalingSetting.ScalingType)
        {
            APP_LOG_INFO("ScalingSetting.CustomScalingX:%d", ScalingSetting.CustomScalingX);
            APP_LOG_INFO("ScalingSetting.CustomScalingY:%d", ScalingSetting.CustomScalingY);
        }
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
int main(int argc, char *pArgv[])
{
    uint8_t ScaleType = 0;
    uint32_t X = 0, Y = 0;

    if (argc < 2)
    {
        APP_LOG_ERROR("Enter Scale type!\"");
        return 0;
    }

    // Converting string type to integer type
    // using function "atoi( argument)"

    if (2 == argc)
    {
        ScaleType = atoi(pArgv[1]);
    }
    else if (4 == argc)
    {
        ScaleType = atoi(pArgv[1]);
        X         = atoi(pArgv[2]);
        Y         = atoi(pArgv[3]);
    }

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

    for (uint32_t i = 0; i < AdapterCount; i++)
    {
        Result = ScalingTest(hDevices[i], ScaleType, X, Y);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_WARN("ctlGetCurrentScaling returned failure code: 0x%X", Result);
            STORE_AND_RESET_ERROR(Result);
            continue;
        }
    }

Exit:

    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDevices);
    APP_LOG_INFO("Overrall test result is 0x%X", GResult);
    return GResult;
}