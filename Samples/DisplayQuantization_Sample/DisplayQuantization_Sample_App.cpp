//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file DisplayQuantization_Sample_App.cpp
 * This sample app enumerates adapters and enumerates displays for each adapter to print out
 * display quantization range settings. User needs to input operation type, display ID and
 * display quantization range. Validation checks are strongly encouraged.
 */

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <sstream>
#include <fstream>
#include <string.h>
using namespace std;

#define CTL_APIEXPORT // caller of control API DLL shall define this before
                      // including control_api.h
#include "igcl_api.h"
#include "GenericIGCLApp.h"

ctl_result_t GResult = CTL_RESULT_SUCCESS;

/***************************************************************
 * @brief PrintUsage
 * Print usage of Display Quantization sample app
 * @param
 * @return void
 ***************************************************************/
void PrintUsage()
{
    APP_LOG_INFO("Display Quantization Sample Test Application.");
    APP_LOG_INFO("Usage 1: ListDisplay");
    APP_LOG_INFO("Usage 2: Set [Display ID] [Quantization Range]");
    APP_LOG_INFO("Display ID - ID can be fetched from Usage 1");
    APP_LOG_INFO("Quantization Range - 0(Default) | 1(Limited) | 2(Full)");
}

/***************************************************************
 * @brief PrintCurrentDisplay
 * Print selected display informations and settings
 * @param stDisplayProperties, stDisplaySettings
 * @return void
 ***************************************************************/
void PrintCurrentDisplay(const ctl_display_properties_t &stDisplayProperties, const ctl_display_settings_t &stDisplaySettings)
{
    bool IsDisplayQuantizationValid        = stDisplaySettings.ValidFlags & CTL_DISPLAY_SETTING_FLAG_QUANTIZATION_RANGE;
    char *displayPortType                  = "";
    char *quantizationRangeMode            = "";
    char *quantizationRangeChangeSupported = "Not Supported";

    // Display Port Type Output
    if (stDisplayProperties.Type == CTL_DISPLAY_OUTPUT_TYPES_DISPLAYPORT)
    {
        displayPortType = "DP";
    }
    else if (stDisplayProperties.Type == CTL_DISPLAY_OUTPUT_TYPES_HDMI)
    {
        displayPortType = "HDMI";
    }
    else
    {
        displayPortType = "Others";
    }

    // Quantization Range Mode Output
    if (stDisplaySettings.QuantizationRange == CTL_DISPLAY_SETTING_QUANTIZATION_RANGE_DEFAULT)
    {
        quantizationRangeMode = "Default";
    }
    else if (stDisplaySettings.QuantizationRange == CTL_DISPLAY_SETTING_QUANTIZATION_RANGE_LIMITED_RANGE)
    {
        quantizationRangeMode = "Limited";
    }
    else if (stDisplaySettings.QuantizationRange == CTL_DISPLAY_SETTING_QUANTIZATION_RANGE_FULL_RANGE)
    {
        quantizationRangeMode = "Full";
    }

    // Quantization Range Mode Supported
    if (IsDisplayQuantizationValid)
    {
        quantizationRangeChangeSupported = "Supported";
    }

    // Print output to console
    PRINT_LOGS("==========================================================");
    APP_LOG_INFO("Display ID: %u", stDisplayProperties.Os_display_encoder_handle.WindowsDisplayEncoderID);
    APP_LOG_INFO("Display Port: %s", displayPortType);
    APP_LOG_INFO("Display Quantization Range Mode: %s", quantizationRangeMode);
    APP_LOG_INFO("Display Quantization Setting Change Supported: %s", quantizationRangeChangeSupported);
    PRINT_LOGS("==========================================================");
}

/***************************************************************
 * @brief PrintDisplayQuantizationConfig
 * Print displays informations and configs
 * @param hDisplayOutput, ActiveDisplayCount
 * @return void
 ***************************************************************/
void PrintDisplayQuantizationConfig(const ctl_display_output_handle_t *hDisplayOutput, const int ActiveDisplayCount)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    PRINT_LOGS("========= Display Quantization Configuration =============");
    for (int i = 0; i < ActiveDisplayCount; i++)
    {
        ctl_display_properties_t stDisplayProperties = {};
        stDisplayProperties.Size                     = sizeof(ctl_display_properties_t);

        ctl_display_settings_t stDisplaySettings = {};
        stDisplaySettings.Size                   = sizeof(ctl_display_settings_t);
        stDisplaySettings.Version                = 0;
        stDisplaySettings.Set                    = false;

        Result = ctlGetDisplayProperties(hDisplayOutput[i], &stDisplayProperties);
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlGetDisplayProperties");

        Result = ctlGetSetDisplaySettings(hDisplayOutput[i], &stDisplaySettings);
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlGetSetDisplaySettings");

        PrintCurrentDisplay(stDisplayProperties, stDisplaySettings);
    }
    PRINT_LOGS("==========================================================");
}

/***************************************************************
 * @brief SetDisplayQuantizationConfig
 * Set Display Quantization configuration. Read and print out new configurations.
 * @param hDisplayOutput, ActiveDisplayCount, SelectedDisplayID, SelectedMode
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t SetDisplayQuantizationConfig(const ctl_display_output_handle_t *hDisplayOutput, const int ActiveDisplayCount, const uint32_t pSelectedDisplayID, const uint8_t pSelectedMode)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;
    bool IsDisplayFound = false;

    PRINT_LOGS("========= Display Quantization Set Configuration ============");
    for (int i = 0; i < ActiveDisplayCount; i++)
    {
        ctl_display_properties_t stDisplayProperties = {};
        stDisplayProperties.Size                     = sizeof(ctl_display_properties_t);

        ctl_display_settings_t stDisplaySettings = {};
        stDisplaySettings.Size                   = sizeof(ctl_display_settings_t);
        stDisplaySettings.Version                = 0;
        stDisplaySettings.Set                    = false;

        Result = ctlGetDisplayProperties(hDisplayOutput[i], &stDisplayProperties);
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlGetDisplayProperties");

        if (stDisplayProperties.Os_display_encoder_handle.WindowsDisplayEncoderID != pSelectedDisplayID)
        {
            // skipped if ids not matched
            continue;
        }

        IsDisplayFound = true;
        APP_LOG_INFO("Display ID %u is found.", pSelectedDisplayID);

        // Read Current Settings
        Result = ctlGetSetDisplaySettings(hDisplayOutput[i], &stDisplaySettings);
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlGetSetDisplaySettings");

        bool IsDisplayQuantizationValid = stDisplaySettings.ValidFlags & CTL_DISPLAY_SETTING_FLAG_QUANTIZATION_RANGE;
        if (IsDisplayQuantizationValid)
        {
            bool _isSettingsValueValid = true;

            // Set quantization range selection
            if (pSelectedMode == 0)
            {
                stDisplaySettings.QuantizationRange = CTL_DISPLAY_SETTING_QUANTIZATION_RANGE_DEFAULT;
            }
            else if (pSelectedMode == 1)
            {
                stDisplaySettings.QuantizationRange = CTL_DISPLAY_SETTING_QUANTIZATION_RANGE_LIMITED_RANGE;
            }
            else if (pSelectedMode == 2)
            {
                stDisplaySettings.QuantizationRange = CTL_DISPLAY_SETTING_QUANTIZATION_RANGE_FULL_RANGE;
            }
            else
            {
                _isSettingsValueValid = false;
                APP_LOG_WARN("Display quantization range settings value is not available!");
            }

            if (_isSettingsValueValid)
            {
                // Apply the setting
                APP_LOG_INFO("Applying new display quantization range setting.");
                stDisplaySettings.Set = TRUE;
                Result                = ctlGetSetDisplaySettings(hDisplayOutput[i], &stDisplaySettings);
                LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlGetSetDisplaySettings");

                // Read the new settings
                stDisplaySettings.Set = FALSE;
                Result                = ctlGetSetDisplaySettings(hDisplayOutput[i], &stDisplaySettings);
                LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlGetSetDisplaySettings");

                // Print out to console on the new configurations
                if (Result == CTL_RESULT_SUCCESS)
                {
                    APP_LOG_INFO("New settings applied!");
                    APP_LOG_INFO("Reading new configuration");
                }
                else
                {
                    APP_LOG_ERROR("New settings failed to apply!");
                    APP_LOG_INFO("Reading current configuration");
                }
                PrintCurrentDisplay(stDisplayProperties, stDisplaySettings);
            }
        }
        else
        {
            // Case for display quantization range cannot be controlled by user
            APP_LOG_WARN("Display quantization setting control is not allowed for this display");
        }
    }

    if (IsDisplayFound == false)
    {
        Result = CTL_RESULT_ERROR_NOT_AVAILABLE;
        APP_LOG_ERROR("Display is not found!");
    }
    PRINT_LOGS("==========================================================");

    return Result;
}

/***************************************************************
 * @brief TestDisplayQuantization
 * Read displays information and configure display quantization range settings.
 * @param AdapterCount, hDevices, pCDArg, SelectedDisplayID, SelectedMode
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestDisplayQuantization(uint32_t AdapterCount, ctl_device_adapter_handle_t *hDevices, const char *pCDArg, const uint32_t pSelectedDisplayID, const uint8_t pSelectedMode)
{
    ctl_result_t Result                                 = CTL_RESULT_SUCCESS;
    ctl_display_output_handle_t *pHDisplayOutput        = NULL;
    ctl_display_output_handle_t *pHActiveDisplayOutputs = NULL;
    uint32_t DisplayCount                               = 0;
    uint32_t NumActiveOutputs                           = 0;
    uint32_t ActiveDisplayCount                         = 0;

    for (uint32_t Index = 0; Index < AdapterCount; Index++)
    {
        DisplayCount       = 0;
        ActiveDisplayCount = 0;

        // Enumerate all the possible target displays for the adapters
        // First step is to get the count
        Result = ctlEnumerateDisplayOutputs(hDevices[Index], &DisplayCount, NULL);

        if (CTL_RESULT_SUCCESS != Result)
        {
            LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");
            continue;
        }
        else if (DisplayCount <= 0)
        {
            APP_LOG_WARN("Invalid Display Count. Skipping display enumeration for adapter: %d", Index);
            continue;
        }

        pHDisplayOutput = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
        EXIT_ON_MEM_ALLOC_FAILURE(pHDisplayOutput, "pHDisplayOutput");

        pHActiveDisplayOutputs = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
        EXIT_ON_MEM_ALLOC_FAILURE(pHActiveDisplayOutputs, "pHActiveDisplayOutputs");
        memset(pHActiveDisplayOutputs, 0, sizeof(ctl_display_output_handle_t) * DisplayCount);

        Result = ctlEnumerateDisplayOutputs(hDevices[Index], &DisplayCount, pHDisplayOutput);
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        for (uint32_t i = 0; i < DisplayCount; i++)
        {
            ctl_display_properties_t stDisplayProperties = {};
            stDisplayProperties.Size                     = sizeof(ctl_display_properties_t);
            bool IsDisplayActive                         = false;
            bool IsDisplayAttached                       = false;

            if (NULL == pHDisplayOutput[i])
            {
                APP_LOG_ERROR("pHDisplayOutput[%d] is NULL.", i);
                Result = CTL_RESULT_ERROR_INVALID_NULL_HANDLE;
                EXIT_ON_ERROR(Result);
            }
            // Get display properties
            Result = ctlGetDisplayProperties(pHDisplayOutput[i], &stDisplayProperties);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDisplayProperties");

            IsDisplayActive   = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE;
            IsDisplayAttached = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED;

            // Filter active display outputs
            if (IsDisplayActive && IsDisplayAttached)
            {
                pHActiveDisplayOutputs[ActiveDisplayCount++] = pHDisplayOutput[i];
            }
        }

        // Command received from the input
        NumActiveOutputs = ActiveDisplayCount;
        if (strcmp(pCDArg, "ListDisplay") == 0)
        {
            PrintDisplayQuantizationConfig(pHActiveDisplayOutputs, NumActiveOutputs);
        }
        else if (strcmp(pCDArg, "Set") == 0)
        {
            Result = SetDisplayQuantizationConfig(pHActiveDisplayOutputs, NumActiveOutputs, pSelectedDisplayID, pSelectedMode);
        }

        // Free allocations at the end of each iteration to avoid leaks
        CTL_FREE_MEM(pHDisplayOutput);
        CTL_FREE_MEM(pHActiveDisplayOutputs);
    }

Exit:

    CTL_FREE_MEM(pHDisplayOutput);
    CTL_FREE_MEM(pHActiveDisplayOutputs);

    return Result;
}

/***************************************************************
 * @brief Main Function which calls Display Quantization API
 * @param argc, pArgv[]
 * @return int
 ***************************************************************/
int main(int argc, char *pArgv[])
{
    const int MaxRangeModeSelection       = 3;
    ctl_result_t Result                   = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hDevices = nullptr;
    uint32_t AdapterCount                 = 0;
    uint32_t pSelectedDisplayID           = 0;
    uint8_t pSelectedQuantizationMode     = 0;
    ctl_init_args_t CtlInitArgs;
    ctl_api_handle_t hAPIHandle = nullptr;
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Get cmdline arguments
    const char *pCDArg = nullptr;
    switch (argc)
    {
        case 2:
            pCDArg = pArgv[1];
            break;
        case 4:
            pCDArg                    = pArgv[1];
            pSelectedDisplayID        = stoi(pArgv[2]);
            pSelectedQuantizationMode = stoi(pArgv[3]);

            // verify command input
            if (strcmp(pCDArg, "Set") == 0)
            {
                Result = CTL_RESULT_SUCCESS;

                if (pSelectedQuantizationMode < MaxRangeModeSelection)
                {
                    Result = CTL_RESULT_SUCCESS;
                }
                else
                {
                    Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
                }
            }
            else
            {
                PrintUsage();
                Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
            }
            break;
        default:
            PrintUsage();
            Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
            break;
    }
    LOG_AND_EXIT_ON_ERROR(Result, "Get cmdline arguments");

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

    try
    {
        Result = TestDisplayQuantization(AdapterCount, hDevices, pCDArg, pSelectedDisplayID, pSelectedQuantizationMode);
        LOG_AND_EXIT_ON_ERROR(Result, "TestDisplayQuantization");
    }
    catch (const std::bad_array_new_length &e)
    {
        APP_LOG_ERROR("%s ", e.what());
    }
    catch (const std::ios_base::failure &e)
    {
        APP_LOG_ERROR("%s ", e.what());
    }
    catch (const std::bad_cast &e)
    {
        APP_LOG_ERROR("%s ", e.what());
    }

Exit:

    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDevices);

    APP_LOG_INFO("Overall test result is 0x%X", GResult);
    return GResult;
}