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

extern "C" {

    // Check if the system supports retro scaling
    bool HasRetroScalingSupport(ctl_device_adapter_handle_t hDevice) {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_retro_scaling_caps_t RetroScalingCaps = { 0 };

        Result = ctlGetSupportedRetroScalingCapability(hDevice, &RetroScalingCaps);
        if (Result != CTL_RESULT_SUCCESS) {
            // Handle error
            return false;
        }

        return RetroScalingCaps.SupportedRetroScaling;
    }

    // Get the current retro scaling state
    bool GetRetroScaling(ctl_device_adapter_handle_t hDevice) {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_retro_scaling_settings_t RetroScalingSettings = { 0 };

        Result = ctlGetSetRetroScaling(hDevice, &RetroScalingSettings);
        if (Result != CTL_RESULT_SUCCESS) {
            // Handle error
            return false;
        }

        return RetroScalingSettings.Enable;
    }

    // Set the retro scaling state
    bool SetRetroScaling(ctl_device_adapter_handle_t hDevice, bool enable) {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_retro_scaling_settings_t RetroScalingSettings = { 0 };

        RetroScalingSettings.Enable = true;
        RetroScalingSettings.Size = sizeof(ctl_retro_scaling_settings_t);

        Result = ctlGetSetRetroScaling(hDevice, &RetroScalingSettings);
        if (Result != CTL_RESULT_SUCCESS) {
            // Handle error
            return false;
        }

        return true;
    }

    // Get the current retro scaling mode
    byte GetRetroScalingMode(ctl_device_adapter_handle_t hDevice) {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_retro_scaling_settings_t RetroScalingSettings = { 0 };

        Result = ctlGetSetRetroScaling(hDevice, &RetroScalingSettings);
        if (Result != CTL_RESULT_SUCCESS) {
            // Handle error
            return 0;
        }

        return RetroScalingSettings.RetroScalingType;
    }

    // Set the retro scaling mode
    bool SetRetroScalingMode(ctl_device_adapter_handle_t hDevice, byte mode) {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_retro_scaling_settings_t RetroScalingSettings = { 0 };

        RetroScalingSettings.RetroScalingType = RetroScalingSettings.RetroScalingType | mode;

        Result = ctlGetSetRetroScaling(hDevice, &RetroScalingSettings);
        if (Result != CTL_RESULT_SUCCESS) {
            // Handle error
            return false;
        }

        return true;
    }

    // Get the list of Intel device handles
    ctl_device_adapter_handle_t* GetIntelDevices(ctl_api_handle_t hAPIHandle, uint32_t* pAdapterCount) {
        ctl_result_t Result = CTL_RESULT_SUCCESS;
        ctl_device_adapter_handle_t* hDevices = NULL;

        // Get the number of Intel Adapters
        Result = ctlEnumerateDevices(hAPIHandle, pAdapterCount, hDevices);
        if (Result != CTL_RESULT_SUCCESS) {
            // Handle error
            return NULL;
        }

        // Allocate memory for the device handles
        hDevices = (ctl_device_adapter_handle_t*)malloc(sizeof(ctl_device_adapter_handle_t) * (*pAdapterCount));
        if (hDevices == NULL) {
            // Handle memory allocation failure
            return NULL;
        }

        // Get the device handles
        Result = ctlEnumerateDevices(hAPIHandle, pAdapterCount, hDevices);
        if (Result != CTL_RESULT_SUCCESS) {
            // Handle error
            return NULL;
        }

        // Return the device handles
        return hDevices;
    }
}