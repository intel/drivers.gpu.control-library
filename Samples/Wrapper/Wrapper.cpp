//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  Wrapper.cpp
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
ctl_api_handle_t GAPIHandle;

extern "C" {

    ctl_retro_scaling_caps_t GetRetroScalingCaps(ctl_device_adapter_handle_t hDevice)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_retro_scaling_caps_t RetroScalingCaps = { 0 };
        RetroScalingCaps.Size = sizeof(ctl_retro_scaling_caps_t);

        Result = ctlGetSupportedRetroScalingCapability(hDevice, &RetroScalingCaps);
        return RetroScalingCaps;
    }

    ctl_retro_scaling_settings_t GetRetroScalingSettings(ctl_device_adapter_handle_t hDevice)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_retro_scaling_settings_t RetroScalingSettings = { 0 };
        RetroScalingSettings.Get = true;
        RetroScalingSettings.RetroScalingType = RetroScalingSettings.RetroScalingType | CTL_RETRO_SCALING_TYPE_FLAG_INTEGER;
        RetroScalingSettings.Size = sizeof(ctl_retro_scaling_settings_t);

        Result = ctlGetSetRetroScaling(hDevice, &RetroScalingSettings);
        return RetroScalingSettings;
    }

    bool SetRetroScalingSettings(ctl_device_adapter_handle_t hDevice, ctl_retro_scaling_settings_t retroScalingSettings)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_retro_scaling_settings_t RetroScalingSettings = { 0 };
        RetroScalingSettings.Enable = retroScalingSettings.Enable;
        RetroScalingSettings.RetroScalingType = retroScalingSettings.RetroScalingType;
        RetroScalingSettings.Size = sizeof(ctl_retro_scaling_settings_t);

        Result = ctlGetSetRetroScaling(hDevice, &RetroScalingSettings);
        return (Result == CTL_RESULT_SUCCESS);
    }

    ctl_scaling_caps_t GetScalingCaps(ctl_device_adapter_handle_t hDevice, uint32_t idx)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;

        ctl_scaling_caps_t ScalingCaps = { 0 };
        ScalingCaps = { 0 };
        ScalingCaps.Size = sizeof(ctl_scaling_caps_t);

        // Enumerate all the possible target display's for the adapters
        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        if (Result == CTL_RESULT_SUCCESS)
        {
            if (DisplayCount > 0)
            {
                hDisplayOutput = (ctl_display_output_handle_t*)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);

                Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
                if (Result == CTL_RESULT_SUCCESS)
                {
                    if (idx <= DisplayCount)
                    {
                        Result = ctlGetSupportedScalingCapability(hDisplayOutput[idx], &ScalingCaps);
                    }
                }
            }
        }

        return ScalingCaps;
    }

    ctl_scaling_settings_t GetScalingSettings(ctl_device_adapter_handle_t hDevice, uint32_t idx)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;

        ctl_scaling_caps_t ScalingCaps = GetScalingCaps(hDevice, idx);
        ctl_scaling_settings_t ScalingSetting = { 0 };
        ScalingSetting.Size = sizeof(ctl_scaling_settings_t);

        // fill custom scaling details only if it is supported
        if (0x1F == ScalingCaps.SupportedScaling)
        {
            // Enumerate all the possible target display's for the adapters
            Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
            if (Result == CTL_RESULT_SUCCESS)
            {
                if (DisplayCount > 0)
                {
                    hDisplayOutput = (ctl_display_output_handle_t*)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);

                    Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
                    if (Result == CTL_RESULT_SUCCESS)
                    {
                        if (idx <= DisplayCount)
                        {
                            Result = ctlGetCurrentScaling(hDisplayOutput[idx], &ScalingSetting);
                        }
                    }
                }
            }
        }

        return ScalingSetting;
    }

    bool SetScalingSettings(ctl_device_adapter_handle_t hDevice, uint32_t idx, ctl_scaling_settings_t* scalingSettings)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;

        ctl_scaling_caps_t ScalingCaps = GetScalingCaps(hDevice, idx);
        ctl_scaling_settings_t ScalingSetting = { 0 };
        ScalingSetting.Size = sizeof(ctl_scaling_settings_t);
        bool ModeSet;

        // fill custom scaling details only if it is supported
        if (0x1F == ScalingCaps.SupportedScaling)
        {
            // Enumerate all the possible target display's for the adapters
            Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
            if (Result == CTL_RESULT_SUCCESS)
            {
                if (DisplayCount > 0)
                {
                    hDisplayOutput = (ctl_display_output_handle_t*)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);

                    Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
                    if (Result == CTL_RESULT_SUCCESS)
                    {
                        if (idx <= DisplayCount)
                        {
                            // check if hardware modeset required to apply custom scaling
                            ModeSet = ((TRUE == scalingSettings->Enable) && (CTL_SCALING_TYPE_FLAG_CUSTOM == scalingSettings->ScalingType)) ? FALSE : TRUE;

                            // filling custom scaling details
                            ScalingSetting = { 0 };
                            ScalingSetting.Enable = scalingSettings->Enable;
                            ScalingSetting.ScalingType = scalingSettings->ScalingType;
                            ScalingSetting.Size = sizeof(ctl_scaling_settings_t);
                            ScalingSetting.HardwareModeSet = (TRUE == ModeSet) ? TRUE : FALSE;
                            ScalingSetting.CustomScalingX = scalingSettings->CustomScalingX;
                            ScalingSetting.CustomScalingY = scalingSettings->CustomScalingY;

                            Result = ctlSetCurrentScaling(hDisplayOutput[idx], &ScalingSetting);
                        }
                    }
                }
            }
        }

        return (Result == CTL_RESULT_SUCCESS);
    }

    ctl_sharpness_caps_t GetSharpnessCaps(ctl_device_adapter_handle_t hDevice, uint32_t idx)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;

        ctl_sharpness_caps_t SharpnessCaps = { 0 };
        SharpnessCaps.Size = sizeof(ctl_sharpness_caps_t);

        // Enumerate all the possible target display's for the adapters
        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        if (Result == CTL_RESULT_SUCCESS)
        {
            if (DisplayCount > 0)
            {
                hDisplayOutput = (ctl_display_output_handle_t*)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);

                Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
                if (Result == CTL_RESULT_SUCCESS)
                {
                    if (idx <= DisplayCount)
                    {
                        Result = ctlGetSharpnessCaps(hDisplayOutput[idx], &SharpnessCaps);
                    }
                }
            }
        }

        return SharpnessCaps;
    }

    ctl_sharpness_settings_t GetSharpnessSettings(ctl_device_adapter_handle_t hDevice, uint32_t idx)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;

        ctl_sharpness_settings_t GetSharpness = { 0 };
        GetSharpness.Size = sizeof(ctl_sharpness_settings_t);

        // Enumerate all the possible target display's for the adapters
        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        if (Result == CTL_RESULT_SUCCESS)
        {
            if (DisplayCount > 0)
            {
                hDisplayOutput = (ctl_display_output_handle_t*)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);

                Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
                if (Result == CTL_RESULT_SUCCESS)
                {
                    if (idx <= DisplayCount)
                    {
                        Result = ctlGetCurrentSharpness(hDisplayOutput[idx], &GetSharpness);
                    }
                }
            }
        }

        return GetSharpness;
    }

    bool SetSharpnessSettings(ctl_device_adapter_handle_t hDevice, uint32_t idx, ctl_sharpness_settings_t* sharpnessSettings)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;

        ctl_sharpness_settings_t SetSharpness = { 0 };
        SetSharpness.FilterType = sharpnessSettings->FilterType;
        SetSharpness.Enable = sharpnessSettings->Enable;
        SetSharpness.Intensity = sharpnessSettings->Intensity; // Current Implementation is to support 0-100.
        SetSharpness.Size = sizeof(ctl_sharpness_settings_t);

        // Enumerate all the possible target display's for the adapters
        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        if (Result == CTL_RESULT_SUCCESS)
        {
            if (DisplayCount > 0)
            {
                hDisplayOutput = (ctl_display_output_handle_t*)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);

                Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
                if (Result == CTL_RESULT_SUCCESS)
                {
                    if (idx <= DisplayCount)
                    {
                        Result = ctlSetCurrentSharpness(hDisplayOutput[idx], &SetSharpness);
                    }
                }
            }
        }

        return (Result == CTL_RESULT_SUCCESS);
    }

    // Get the list of Intel device handles
    ctl_device_adapter_handle_t* GetIntelDevices(ctl_api_handle_t hAPIHandle, uint32_t* pAdapterCount)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;
        ctl_device_adapter_handle_t* hDevices = NULL;

        // Get the number of Intel Adapters
        Result = ctlEnumerateDevices(hAPIHandle, pAdapterCount, hDevices);
        if (Result != CTL_RESULT_SUCCESS)
        {
            // Handle error
            return NULL;
        }

        // Allocate memory for the device handles
        hDevices = (ctl_device_adapter_handle_t*)malloc(sizeof(ctl_device_adapter_handle_t) * (*pAdapterCount));
        if (hDevices == NULL)
        {
            // Handle memory allocation failure
            return NULL;
        }

        // Get the device handles
        Result = ctlEnumerateDevices(hAPIHandle, pAdapterCount, hDevices);
        if (Result != CTL_RESULT_SUCCESS)
        {
            // Handle error
            return NULL;
        }

        // Return the device handles
        return hDevices;
    }

    ctl_api_handle_t Init()
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_init_args_t CtlInitArgs;

        ZeroMemory(&CtlInitArgs, sizeof(ctl_init_args_t));

        CtlInitArgs.AppVersion = CTL_MAKE_VERSION(CTL_IMPL_MAJOR_VERSION, CTL_IMPL_MINOR_VERSION);
        CtlInitArgs.flags = 0;
        CtlInitArgs.Size = sizeof(CtlInitArgs);
        CtlInitArgs.Version = 0;

        Result = ctlInit(&CtlInitArgs, &GAPIHandle);

        if (Result != CTL_RESULT_SUCCESS)
        {
            // Handle error
        }

        // Initialization successful
        return GAPIHandle;
    }

    bool Terminate()
    {
        ctlClose(GAPIHandle);
        return GResult;
    }
}