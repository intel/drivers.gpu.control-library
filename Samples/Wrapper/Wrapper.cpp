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

typedef struct ctl_telemetry_data
{
    // GPU TDP
    bool gpuEnergySupported = false;
    double gpuEnergyValue;

    // GPU Voltage
    bool gpuVoltageSupported = false;
    double gpuVoltagValue;

    // GPU Core Frequency
    bool gpuCurrentClockFrequencySupported = false;
    double gpuCurrentClockFrequencyValue;

    // GPU Core Temperature
    bool gpuCurrentTemperatureSupported = false;
    double gpuCurrentTemperatureValue;

    // GPU Usage
    bool globalActivitySupported = false;
    double globalActivityValue;

    // Render Engine Usage
    bool renderComputeActivitySupported = false;
    double renderComputeActivityValue;

    // Media Engine Usage
    bool mediaActivitySupported = false;
    double mediaActivityValue;

    // VRAM Power Consumption
    bool vramEnergySupported = false;
    double vramEnergyValue;

    // VRAM Voltage
    bool vramVoltageSupported = false;
    double vramVoltageValue;

    // VRAM Frequency
    bool vramCurrentClockFrequencySupported = false;
    double vramCurrentClockFrequencyValue;

    // VRAM Read Bandwidth
    bool vramReadBandwidthSupported = false;
    double vramReadBandwidthValue;

    // VRAM Write Bandwidth
    bool vramWriteBandwidthSupported = false;
    double vramWriteBandwidthValue;

    // VRAM Temperature
    bool vramCurrentTemperatureSupported = false;
    double vramCurrentTemperatureValue;

    // Fanspeed (n Fans)
    bool fanSpeedSupported = false;
    double fanSpeedValue;
};

ctl_api_handle_t hAPIHandle;
ctl_device_adapter_handle_t* hDevices;

extern "C" {

    double deltatimestamp = 0;
    double prevtimestamp = 0;
    double curtimestamp = 0;
    double prevgpuEnergyCounter = 0;
    double curgpuEnergyCounter = 0;
    double curglobalActivityCounter = 0;
    double prevglobalActivityCounter = 0;
    double currenderComputeActivityCounter = 0;
    double prevrenderComputeActivityCounter = 0;
    double curmediaActivityCounter = 0;
    double prevmediaActivityCounter = 0;
    double curvramEnergyCounter = 0;
    double prevvramEnergyCounter = 0;
    double curvramReadBandwidthCounter = 0;
    double prevvramReadBandwidthCounter = 0;
    double curvramWriteBandwidthCounter = 0;
    double prevvramWriteBandwidthCounter = 0;

    ctl_result_t GetRetroScalingCaps(ctl_device_adapter_handle_t hDevice, ctl_retro_scaling_caps_t* RetroScalingCaps)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;
        RetroScalingCaps->Size = sizeof(ctl_retro_scaling_caps_t);

        Result = ctlGetSupportedRetroScalingCapability(hDevice, RetroScalingCaps);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSupportedRetroScalingCapability");

        cout << "======== GetRetroScalingCaps ========" << endl;
        cout << "RetroScalingCaps.SupportedRetroScaling: " << RetroScalingCaps->SupportedRetroScaling << endl;

    Exit:
        return Result;
    }

    ctl_result_t GetRetroScalingSettings(ctl_device_adapter_handle_t hDevice, ctl_retro_scaling_settings_t* RetroScalingSettings)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;
        RetroScalingSettings->Get = TRUE;
        RetroScalingSettings->Size = sizeof(ctl_retro_scaling_settings_t);

        Result = ctlGetSetRetroScaling(hDevice, RetroScalingSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetRetroScaling");

        cout << "======== GetRetroScalingSettings ========" << endl;
        cout << "RetroScalingSettings.Get: " << RetroScalingSettings->Get << endl;
        cout << "RetroScalingSettings.Enable: " << RetroScalingSettings->Enable << endl;
        cout << "RetroScalingSettings.RetroScalingType: " << RetroScalingSettings->RetroScalingType << endl;

    Exit:
        return Result;
    }

    ctl_result_t SetRetroScalingSettings(ctl_device_adapter_handle_t hDevice, ctl_retro_scaling_settings_t RetroScalingSettings)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;
        RetroScalingSettings.Get = FALSE;
        RetroScalingSettings.Size = sizeof(ctl_retro_scaling_settings_t);

        Result = ctlGetSetRetroScaling(hDevice, &RetroScalingSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetRetroScaling");

        cout << "======== SetRetroScalingSettings ========" << endl;
        cout << "RetroScalingSettings.Get: " << RetroScalingSettings.Get << endl;
        cout << "RetroScalingSettings.Enable: " << RetroScalingSettings.Enable << endl;
        cout << "RetroScalingSettings.RetroScalingType: " << RetroScalingSettings.RetroScalingType << endl;

    Exit:
        return Result;
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

    ctl_result_t GetSharpnessCaps(ctl_device_adapter_handle_t hDevice, uint32_t idx, ctl_sharpness_caps_t* SharpnessCaps)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;
        SharpnessCaps->Size = sizeof(ctl_sharpness_caps_t);

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
        Result = ctlGetSharpnessCaps(hDisplayOutput[idx], SharpnessCaps);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSharpnessCaps");

        cout << "======== GetSharpnessCaps ========" << endl;
        cout << "SharpnessCaps.SupportedFilterFlags: " << SharpnessCaps->SupportedFilterFlags << endl;

    Exit:
        CTL_FREE_MEM(hDisplayOutput);
        return Result;
    }

    ctl_result_t GetSharpnessSettings(ctl_device_adapter_handle_t hDevice, uint32_t idx, ctl_sharpness_settings_t* GetSharpness)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        ctl_display_output_handle_t* hDisplayOutput = NULL;
        uint32_t DisplayCount = 0;
        GetSharpness->Size = sizeof(ctl_sharpness_settings_t);

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

    ctl_result_t GetEnduranceGamingCaps(ctl_device_adapter_handle_t hDevice, ctl_endurance_gaming_caps_t* pCaps)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        // ask IGCL what it supports
        ctl_3d_feature_details_t details[CTL_3D_FEATURE_MAX] = {};
        ctl_3d_feature_caps_t caps = {};
        caps.Size = sizeof(caps);
        caps.Version = 1;
        caps.NumSupportedFeatures = CTL_3D_FEATURE_MAX;
        caps.pFeatureDetails = details;

        Result = ctlGetSupported3DCapabilities(hDevice, &caps);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSupported3DCapabilities");

        // find the Endurance‐Gaming entry
        bool found = false;
        for (uint32_t i = 0; i < caps.NumSupportedFeatures; ++i) {
            if (details[i].FeatureType == CTL_3D_FEATURE_ENDURANCE_GAMING) {
                // the API tells us how each enum parameter is supported
                // the feature's PropertyInfo.EnumType holds SupportedTypes + DefaultType
                // but ctl_endurance_gaming_caps_t has two enum‐caps: EGControlCaps & EGModeCaps
                // IGCL packs both into pCustomValue as ctl_endurance_gaming_caps_t
                auto igclCaps = reinterpret_cast<ctl_endurance_gaming_caps_t*>(details[i].pCustomValue);

                *pCaps = *igclCaps;
                found = true;
                break;
            }
        }

        if (!found)
            Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;

    Exit:
        return Result;
    }

    ctl_result_t GetEnduranceGamingSettings(ctl_device_adapter_handle_t hDevice, ctl_endurance_gaming2_t* pSettings)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;
        ctl_3d_feature_getset_t fs = {};

        fs.Size = sizeof(fs);
        fs.Version = 1;
        fs.FeatureType = CTL_3D_FEATURE_ENDURANCE_GAMING;
        fs.ApplicationName = nullptr;        // global
        fs.ApplicationNameLength = 0;
        fs.bSet = false;          // GET
        fs.ValueType = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
        fs.CustomValueSize = sizeof(*pSettings);
        fs.pCustomValue = pSettings;

        Result = ctlGetSet3DFeature(hDevice, &fs);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSet3DFeature (GetEnduranceGamingSettings)");

    Exit:
        return Result;
    }

    ctl_result_t SetEnduranceGamingSettings(ctl_device_adapter_handle_t hDevice, ctl_endurance_gaming_t settings)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;
        ctl_3d_feature_getset_t fs = {};
        ctl_endurance_gaming2_t eg2 = {};

        // copy only the control + mode into the v2 type
        eg2.EGControl = settings.EGControl;
        eg2.EGMode = settings.EGMode;
        // eg2.IsFPRequired, eg2.TargetFPS etc are out‐params

        fs.Size = sizeof(fs);
        fs.Version = 1;
        fs.FeatureType = CTL_3D_FEATURE_ENDURANCE_GAMING;
        fs.ApplicationName = nullptr;
        fs.ApplicationNameLength = 0;
        fs.bSet = true;           // SET
        fs.ValueType = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
        fs.CustomValueSize = sizeof(eg2);
        fs.pCustomValue = &eg2;

        Result = ctlGetSet3DFeature(hDevice, &fs);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSet3DFeature (SetEnduranceGamingSettings)");

    Exit:
        return Result;
    }

    // Get the list of Intel device handles
    ctl_device_adapter_handle_t* EnumerateDevices(uint32_t* pAdapterCount)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;
        hDevices = NULL;

        // Get the number of Intel Adapters
        Result = ctlEnumerateDevices(hAPIHandle, pAdapterCount, hDevices);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");

        // Allocate memory for the device handles
        hDevices = (ctl_device_adapter_handle_t*)malloc(sizeof(ctl_device_adapter_handle_t) * (*pAdapterCount));
        EXIT_ON_MEM_ALLOC_FAILURE(hDevices, "EnumerateDevices");

        // Get the device handles
        Result = ctlEnumerateDevices(hAPIHandle, pAdapterCount, hDevices);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");

    Exit:
        return hDevices;
    }

    ctl_result_t GetDeviceProperties(ctl_device_adapter_handle_t hDevice, ctl_device_adapter_properties_t* StDeviceAdapterProperties)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        StDeviceAdapterProperties->Size = sizeof(ctl_device_adapter_properties_t);
        StDeviceAdapterProperties->pDeviceID = malloc(sizeof(LUID));
        StDeviceAdapterProperties->device_id_size = sizeof(LUID);
        StDeviceAdapterProperties->Version = 2;

        Result = ctlGetDeviceProperties(hDevice, StDeviceAdapterProperties);

        if (CTL_RESULT_ERROR_UNSUPPORTED_VERSION == Result) // reduce version if required & recheck
        {
            printf("ctlGetDeviceProperties() version mismatch - Reducing version to 0 and retrying\n");
            StDeviceAdapterProperties->Version = 0;
            Result = ctlGetDeviceProperties(hDevice, StDeviceAdapterProperties);
        }

        cout << "======== GetDeviceProperties ========" << endl;
        cout << "StDeviceAdapterProperties.Name: " << StDeviceAdapterProperties->name << endl;

        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDeviceProperties");

    Exit:
        return Result;
    }

    ctl_result_t GetTelemetryData(ctl_device_adapter_handle_t hDevice, ctl_telemetry_data* TelemetryData)
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;
        ctl_power_telemetry_t pPowerTelemetry = {};
        pPowerTelemetry.Size = sizeof(ctl_power_telemetry_t);

        Result = ctlPowerTelemetryGet(hDevice, &pPowerTelemetry);
        if (Result != CTL_RESULT_SUCCESS)
            goto Exit;

        prevtimestamp = curtimestamp;
        curtimestamp = pPowerTelemetry.timeStamp.value.datadouble;
        deltatimestamp = curtimestamp - prevtimestamp;

        if (pPowerTelemetry.gpuEnergyCounter.bSupported)
        {
            TelemetryData->gpuEnergySupported = true;
            prevgpuEnergyCounter = curgpuEnergyCounter;
            curgpuEnergyCounter = pPowerTelemetry.gpuEnergyCounter.value.datadouble;

            TelemetryData->gpuEnergyValue = (curgpuEnergyCounter - prevgpuEnergyCounter) / deltatimestamp;
        }

        TelemetryData->gpuVoltageSupported = pPowerTelemetry.gpuVoltage.bSupported;
        TelemetryData->gpuVoltagValue = pPowerTelemetry.gpuVoltage.value.datadouble;

        TelemetryData->gpuCurrentClockFrequencySupported = pPowerTelemetry.gpuCurrentClockFrequency.bSupported;
        TelemetryData->gpuCurrentClockFrequencyValue = pPowerTelemetry.gpuCurrentClockFrequency.value.datadouble;

        TelemetryData->gpuCurrentTemperatureSupported = pPowerTelemetry.gpuCurrentTemperature.bSupported;
        TelemetryData->gpuCurrentTemperatureValue = pPowerTelemetry.gpuCurrentTemperature.value.datadouble;

        if (pPowerTelemetry.globalActivityCounter.bSupported)
        {
            TelemetryData->globalActivitySupported = true;
            prevglobalActivityCounter = curglobalActivityCounter;
            curglobalActivityCounter = pPowerTelemetry.globalActivityCounter.value.datadouble;

            TelemetryData->globalActivityValue = 100 * (curglobalActivityCounter - prevglobalActivityCounter) / deltatimestamp;
        }

        if (pPowerTelemetry.renderComputeActivityCounter.bSupported)
        {
            TelemetryData->renderComputeActivitySupported = true;
            prevrenderComputeActivityCounter = currenderComputeActivityCounter;
            currenderComputeActivityCounter = pPowerTelemetry.renderComputeActivityCounter.value.datadouble;

            TelemetryData->renderComputeActivityValue = 100 * (currenderComputeActivityCounter - prevrenderComputeActivityCounter) / deltatimestamp;
        }

        if (pPowerTelemetry.mediaActivityCounter.bSupported)
        {
            TelemetryData->mediaActivitySupported = true;
            prevmediaActivityCounter = curmediaActivityCounter;
            curmediaActivityCounter = pPowerTelemetry.mediaActivityCounter.value.datadouble;

            TelemetryData->mediaActivityValue = 100 * (curmediaActivityCounter - prevmediaActivityCounter) / deltatimestamp;
        }

        if (pPowerTelemetry.vramEnergyCounter.bSupported)
        {
            TelemetryData->vramEnergySupported = true;
            prevvramEnergyCounter = curvramEnergyCounter;
            curvramEnergyCounter = pPowerTelemetry.vramEnergyCounter.value.datadouble;

            TelemetryData->vramEnergyValue = (curvramEnergyCounter - prevvramEnergyCounter) / deltatimestamp;
        }

        TelemetryData->vramVoltageSupported = pPowerTelemetry.vramVoltage.bSupported;
        TelemetryData->vramVoltageValue = pPowerTelemetry.vramVoltage.value.datadouble;

        TelemetryData->vramCurrentClockFrequencySupported = pPowerTelemetry.vramCurrentClockFrequency.bSupported;
        TelemetryData->vramCurrentClockFrequencyValue = pPowerTelemetry.vramCurrentClockFrequency.value.datadouble;

        if (pPowerTelemetry.vramReadBandwidthCounter.bSupported)
        {
            TelemetryData->vramReadBandwidthSupported = true;
            prevvramReadBandwidthCounter = curvramReadBandwidthCounter;
            curvramReadBandwidthCounter = pPowerTelemetry.vramReadBandwidthCounter.value.datadouble;

            TelemetryData->vramReadBandwidthValue = (curvramReadBandwidthCounter - prevvramReadBandwidthCounter) / deltatimestamp;
        }

        if (pPowerTelemetry.vramWriteBandwidthCounter.bSupported)
        {
            TelemetryData->vramWriteBandwidthSupported = true;
            prevvramWriteBandwidthCounter = curvramWriteBandwidthCounter;
            curvramWriteBandwidthCounter = pPowerTelemetry.vramWriteBandwidthCounter.value.datadouble;

            TelemetryData->vramWriteBandwidthValue = (curvramWriteBandwidthCounter - prevvramWriteBandwidthCounter) / deltatimestamp;
        }

        TelemetryData->vramCurrentTemperatureSupported = pPowerTelemetry.vramCurrentTemperature.bSupported;
        TelemetryData->vramCurrentTemperatureValue = pPowerTelemetry.vramCurrentTemperature.value.datadouble;

        TelemetryData->fanSpeedSupported = pPowerTelemetry.fanSpeed[0].bSupported;
        TelemetryData->fanSpeedValue = pPowerTelemetry.fanSpeed[0].value.datadouble;

    Exit:
    return Result;
    }

    ctl_result_t IntializeIgcl()
    {
        ctl_result_t Result = CTL_RESULT_SUCCESS;

        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

        ctl_init_args_t ctlInitArgs;
        ctlInitArgs.AppVersion = CTL_MAKE_VERSION(CTL_IMPL_MAJOR_VERSION, CTL_IMPL_MINOR_VERSION);
        ctlInitArgs.flags = CTL_INIT_FLAG_USE_LEVEL_ZERO;
        ctlInitArgs.Size = sizeof(ctlInitArgs);
        ctlInitArgs.Version = 0;
        ZeroMemory(&ctlInitArgs.ApplicationUID, sizeof(ctl_application_id_t));
        Result = ctlInit(&ctlInitArgs, &hAPIHandle);

        return Result;
    }

    void CloseIgcl()
    {
        ctlClose(hAPIHandle);

        if (hDevices != nullptr)
        {
            free(hDevices);
            hDevices = nullptr;
        }
    }
}