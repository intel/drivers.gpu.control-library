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
using namespace std;

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
        RetroScalingSettings.Size = sizeof(ctl_retro_scaling_settings_t);

        Result = ctlGetSetRetroScaling(hDevice, &RetroScalingSettings);
        return RetroScalingSettings;
    }

    bool SetRetroScalingSettings(ctl_device_adapter_handle_t hDevice, ctl_retro_scaling_settings_t retroScalingSettings)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_retro_scaling_settings_t RetroScalingSettings = { 0 };
        RetroScalingSettings.Get = false;
        RetroScalingSettings.Enable = retroScalingSettings.Enable;
        RetroScalingSettings.RetroScalingType = retroScalingSettings.RetroScalingType;
        RetroScalingSettings.Size = sizeof(ctl_retro_scaling_settings_t);

        Result = ctlGetSetRetroScaling(hDevice, &RetroScalingSettings);
        return (Result == CTL_RESULT_SUCCESS);
    }

    ctl_result_t GetScalingCaps(ctl_device_adapter_handle_t hDevice, uint32_t idx, ctl_scaling_caps_t* ScalingCaps)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;
        ScalingCaps->Size = sizeof(ctl_scaling_caps_t);

        // Enumerate all the possible target display's for the adapters
        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        if (DisplayCount <= 0)
        {
            printf("Invalid Display Count\n");
            goto Exit;
        }

        hDisplayOutput = (ctl_display_output_handle_t*)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        Result = ctlGetSupportedScalingCapability(hDisplayOutput[idx], ScalingCaps);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSupportedScalingCapability");

        cout << "======== GetScalingCaps ========" << endl;
        cout << "ScalingCaps.SupportedScaling: " << ScalingCaps->SupportedScaling << endl;

    Exit:
        CTL_FREE_MEM(hDisplayOutput);
        return Result;
    }

    ctl_result_t GetScalingSettings(ctl_device_adapter_handle_t hDevice, uint32_t idx, ctl_scaling_settings_t* ScalingSetting)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;
        ctl_scaling_caps_t ScalingCaps = { 0 };
        ScalingSetting->Size = sizeof(ctl_scaling_settings_t);

        Result = GetScalingCaps(hDevice, idx, &ScalingCaps);
        LOG_AND_EXIT_ON_ERROR(Result, "GetScalingCaps");

        // Enumerate all the possible target display's for the adapters
        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        if (DisplayCount <= 0)
        {
            printf("Invalid Display Count\n");
            goto Exit;
        }

        hDisplayOutput = (ctl_display_output_handle_t*)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        if (0 != ScalingCaps.SupportedScaling)
        {
            Result = ctlGetCurrentScaling(hDisplayOutput[idx], ScalingSetting);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlGetCurrentScaling");

            cout << "======== GetScalingSettings ========" << endl;
            cout << "ScalingSetting.Enable: " << ScalingSetting->Enable << endl;
            cout << "ScalingSetting.ScalingType: " << ScalingSetting->ScalingType << endl;
            cout << "ScalingSetting.HardwareModeSet: " << ScalingSetting->HardwareModeSet << endl;
            cout << "ScalingSetting.CustomScalingX: " << ScalingSetting->CustomScalingX << endl;
            cout << "ScalingSetting.CustomScalingY: " << ScalingSetting->CustomScalingY << endl;
        }

    Exit:
        CTL_FREE_MEM(hDisplayOutput);
        return Result;
    }

    ctl_result_t SetScalingSettings(ctl_device_adapter_handle_t hDevice, uint32_t idx, ctl_scaling_settings_t ScalingSetting)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;
        ctl_scaling_caps_t ScalingCaps = { 0 };
        bool ModeSet;

        Result = GetScalingCaps(hDevice, idx, &ScalingCaps);
        LOG_AND_EXIT_ON_ERROR(Result, "GetScalingCaps");

        // Enumerate all the possible target display's for the adapters
        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        if (DisplayCount <= 0)
        {
            printf("Invalid Display Count\n");
            goto Exit;
        }

        hDisplayOutput = (ctl_display_output_handle_t*)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        if (0 != ScalingCaps.SupportedScaling)
        {
            // filling custom scaling details
            ScalingSetting.Size = sizeof(ctl_scaling_settings_t);

            // fill custom scaling details only if it is supported
            if (0x1F == ScalingCaps.SupportedScaling)
            {
                // check if hardware modeset required to apply custom scaling
                ModeSet = ((TRUE == ScalingSetting.Enable) && (CTL_SCALING_TYPE_FLAG_CUSTOM == ScalingSetting.ScalingType)) ? FALSE : TRUE;
                ScalingSetting.HardwareModeSet = (TRUE == ModeSet) ? TRUE : FALSE;
            }

            cout << "======== SetScalingSettings ========" << endl;
            cout << "ScalingSetting.Enable: " << ScalingSetting.Enable << endl;
            cout << "ScalingSetting.ScalingType: " << ScalingSetting.ScalingType << endl;
            cout << "ScalingSetting.HardwareModeSet: " << ScalingSetting.HardwareModeSet << endl;
            cout << "ScalingSetting.CustomScalingX: " << ScalingSetting.CustomScalingX << endl;
            cout << "ScalingSetting.CustomScalingY: " << ScalingSetting.CustomScalingY << endl;
        }

        Result = ctlSetCurrentScaling(hDisplayOutput[idx], &ScalingSetting);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlSetCurrentScaling");

    Exit:
        CTL_FREE_MEM(hDisplayOutput);
        return Result;
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
            STORE_AND_RESET_ERROR(Result);
            if (DisplayCount > 0)
            {
                hDisplayOutput = (ctl_display_output_handle_t*)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);

                Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
                if (Result == CTL_RESULT_SUCCESS)
                {
                    STORE_AND_RESET_ERROR(Result);
                    if (idx <= DisplayCount)
                    {
                        Result = ctlGetSharpnessCaps(hDisplayOutput[idx], &SharpnessCaps);
                    }
                }
            }
        }

        CTL_FREE_MEM(hDisplayOutput);
        return SharpnessCaps;
    }

    ctl_result_t GetSharpnessSettings(ctl_device_adapter_handle_t hDevice, uint32_t idx, ctl_sharpness_settings_t* GetSharpness)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;
        ctl_sharpness_caps_t SharpnessCaps = { 0 };
        GetSharpness->Size = sizeof(ctl_sharpness_settings_t);
        SharpnessCaps.Size = sizeof(ctl_sharpness_caps_t);

        // Enumerate all the possible target display's for the adapters
        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        if (DisplayCount <= 0)
        {
            printf("Invalid Display Count\n");
            goto Exit;
        }

        hDisplayOutput = (ctl_display_output_handle_t*)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        // Get Sharpness caps
        Result = ctlGetSharpnessCaps(hDisplayOutput[idx], &SharpnessCaps);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSharpnessCaps");

        SharpnessCaps.pFilterProperty = (ctl_sharpness_filter_properties_t*)malloc(SharpnessCaps.NumFilterTypes * sizeof(ctl_sharpness_filter_properties_t));
        Result = ctlGetSharpnessCaps(hDisplayOutput[idx], &SharpnessCaps);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSharpnessCaps");

        Result = ctlGetCurrentSharpness(hDisplayOutput[idx], GetSharpness);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetCurrentSharpness");

        cout << "======== GetSharpnessSettings ========" << endl;
        cout << "GetSharpness.Enable: " << GetSharpness->Enable << endl;
        cout << "GetSharpness.FilterType: " << GetSharpness->FilterType << endl;
        cout << "GetSharpness.Intensity: " << GetSharpness->Intensity << endl;

    Exit:
        CTL_FREE_MEM(hDisplayOutput);
        return Result;
    }

    ctl_result_t SetSharpnessSettings(ctl_device_adapter_handle_t hDevice, uint32_t idx, ctl_sharpness_settings_t SetSharpness)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;
        ctl_sharpness_caps_t SharpnessCaps = { 0 };
        SharpnessCaps.Size = sizeof(ctl_sharpness_caps_t);

        // Enumerate all the possible target display's for the adapters
        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        if (DisplayCount <= 0)
        {
            printf("Invalid Display Count\n");
            goto Exit;
        }

        hDisplayOutput = (ctl_display_output_handle_t*)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

        Result = ctlEnumerateDisplayOutputs(hDevice, &DisplayCount, hDisplayOutput);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        // Get Sharpness caps
        Result = ctlGetSharpnessCaps(hDisplayOutput[idx], &SharpnessCaps);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSharpnessCaps");

        SharpnessCaps.pFilterProperty = (ctl_sharpness_filter_properties_t*)malloc(SharpnessCaps.NumFilterTypes * sizeof(ctl_sharpness_filter_properties_t));
        Result = ctlGetSharpnessCaps(hDisplayOutput[idx], &SharpnessCaps);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSharpnessCaps");

        cout << "======== SetSharpnessSettings ========" << endl;
        cout << "SetSharpness.Enable: " << SetSharpness.Enable << endl;
        cout << "SetSharpness.FilterType: " << SetSharpness.FilterType << endl;
        cout << "SetSharpness.Intensity: " << SetSharpness.Intensity << endl;

        Result = ctlSetCurrentSharpness(hDisplayOutput[idx], &SetSharpness);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlSetCurrentSharpness");

    Exit:
        CTL_FREE_MEM(hDisplayOutput);
        return Result;
    }

    // Get the list of Intel device handles
    ctl_device_adapter_handle_t* GetDevices(ctl_api_handle_t hAPIHandle, uint32_t* pAdapterCount)
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
        AllocConsole();
        freopen("CONOUT$", "w", stdout); // Redirect stdout to console

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