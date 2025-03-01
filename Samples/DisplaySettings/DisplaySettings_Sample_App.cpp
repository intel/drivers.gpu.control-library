//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  DisplaySettings_Sample_App.cpp
 * @brief : This file contains the 'main' function and the DisplaySettings Sample APP. Program execution begins and ends there.
 *
 */

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <assert.h>

#define CTL_APIEXPORT // caller of control API DLL shall define this before
                      // including igcl_api.h
#include "igcl_api.h"
#include "GenericIGCLApp.h"

#define API_VERSION 0;

ctl_result_t GResult = CTL_RESULT_SUCCESS;

/***************************************************************
 * @brief
 * Sample test for Get/Set Quantization Range
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestToGetSetQuantizationRange(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                           = CTL_RESULT_SUCCESS;
    ctl_display_settings_t AppliedDisplaySettings = { 0 };
    ctl_display_settings_t NewDisplaySettings     = { 0 };
    bool IsControllable, IsSupported = FALSE;

    // GET CALL
    AppliedDisplaySettings.Version = API_VERSION;
    AppliedDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    AppliedDisplaySettings.Set     = FALSE;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &AppliedDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (Quantization Range GET CALL)");

    IsControllable = (CTL_DISPLAY_SETTING_FLAG_QUANTIZATION_RANGE & AppliedDisplaySettings.ControllableFlags) ? TRUE : FALSE;
    IsSupported    = (CTL_DISPLAY_SETTING_FLAG_QUANTIZATION_RANGE & AppliedDisplaySettings.SupportedFlags) ? TRUE : FALSE;

    if ((FALSE == IsControllable) || (FALSE == IsSupported))
    {
        APP_LOG_WARN("Get/Set Quantization Range is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    APP_LOG_INFO(" Current Applied Quantization Range is %d ", AppliedDisplaySettings.QuantizationRange);

    // SET CALL
    NewDisplaySettings.Version = API_VERSION;
    NewDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    NewDisplaySettings.Set     = TRUE;

    NewDisplaySettings.ValidFlags        = CTL_DISPLAY_SETTING_FLAG_QUANTIZATION_RANGE;
    NewDisplaySettings.QuantizationRange = CTL_DISPLAY_SETTING_QUANTIZATION_RANGE_FULL_RANGE;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &NewDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (Quantization Range SET CALL)");

    // GET CALL
    AppliedDisplaySettings         = { 0 };
    AppliedDisplaySettings.Version = API_VERSION;
    AppliedDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    AppliedDisplaySettings.Set     = FALSE;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &AppliedDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (Quantization Range GET CALL)");
    APP_LOG_INFO(" Current Quantization Range is %d ", AppliedDisplaySettings.QuantizationRange);

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Sample test for Get/Set PictureAspectRatio
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestToGetSetPictureAspectRatio(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                           = CTL_RESULT_SUCCESS;
    ctl_display_settings_t AppliedDisplaySettings = { 0 };
    ctl_display_settings_t NewDisplaySettings     = { 0 };
    bool IsControllable, IsSupported = FALSE;

    // GET CALL
    AppliedDisplaySettings.Version = API_VERSION;
    AppliedDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    AppliedDisplaySettings.Set     = FALSE;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &AppliedDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (Picture Aspect Ratio GET CALL)");

    IsControllable = (CTL_DISPLAY_SETTING_FLAG_PICTURE_AR & AppliedDisplaySettings.ControllableFlags) ? TRUE : FALSE;
    IsSupported    = (CTL_DISPLAY_SETTING_FLAG_PICTURE_AR & AppliedDisplaySettings.SupportedFlags) ? TRUE : FALSE;

    if ((FALSE == IsControllable) || (FALSE == IsSupported))
    {
        APP_LOG_WARN("Get/Set PictureAspectRatio is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    APP_LOG_INFO(" Supported Picture Aspect Ratio is %d ", AppliedDisplaySettings.SupportedPictureAR);
    APP_LOG_INFO(" Current Applied Picture Aspect Ratio is %d ", AppliedDisplaySettings.PictureAR);

    // CALL TO SET 16_9
    if (CTL_DISPLAY_SETTING_PICTURE_AR_FLAG_AR_16_9 & AppliedDisplaySettings.SupportedPictureAR)
    {

        NewDisplaySettings.Version = API_VERSION;
        NewDisplaySettings.Size    = sizeof(ctl_display_settings_t);
        NewDisplaySettings.Set     = TRUE;

        NewDisplaySettings.ValidFlags = CTL_DISPLAY_SETTING_FLAG_PICTURE_AR;
        NewDisplaySettings.PictureAR  = CTL_DISPLAY_SETTING_PICTURE_AR_FLAG_AR_16_9;

        Result = ctlGetSetDisplaySettings(hDisplayOutput, &NewDisplaySettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (Picture Aspect Ratio SET CALL)");
    }

    // GET CALL
    AppliedDisplaySettings         = { 0 };
    AppliedDisplaySettings.Version = API_VERSION;
    AppliedDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    AppliedDisplaySettings.Set     = FALSE;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &AppliedDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (Picture Aspect Ratio GET CALL)");
    APP_LOG_INFO(" Current Picture Aspect Ratio is %d ", AppliedDisplaySettings.PictureAR);

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Sample test for Get/Set ContentType
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestToGetSetContentType(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                           = CTL_RESULT_SUCCESS;
    ctl_display_settings_t AppliedDisplaySettings = { 0 };
    ctl_display_settings_t NewDisplaySettings     = { 0 };
    bool IsControllable, IsSupported = FALSE;

    // GET CALL
    AppliedDisplaySettings.Version = API_VERSION;
    AppliedDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    AppliedDisplaySettings.Set     = FALSE;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &AppliedDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (ContentType GET CALL)");

    IsControllable = (CTL_DISPLAY_SETTING_FLAG_CONTENT_TYPE & AppliedDisplaySettings.ControllableFlags) ? TRUE : FALSE;
    IsSupported    = (CTL_DISPLAY_SETTING_FLAG_CONTENT_TYPE & AppliedDisplaySettings.SupportedFlags) ? TRUE : FALSE;

    if ((FALSE == IsControllable) || (FALSE == IsSupported))
    {
        APP_LOG_WARN("Get/Set ContentType is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    APP_LOG_INFO(" Current Applied ContentType is %d ", AppliedDisplaySettings.ContentType);

    // CALL TO SET GAMING CONTENT TYPE
    NewDisplaySettings.Version = API_VERSION;
    NewDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    NewDisplaySettings.Set     = TRUE;

    NewDisplaySettings.ValidFlags  = CTL_DISPLAY_SETTING_FLAG_CONTENT_TYPE;
    NewDisplaySettings.ContentType = CTL_DISPLAY_SETTING_CONTENT_TYPE_GAMING;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &NewDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (ContentType SET CALL)");

    // GET CALL
    AppliedDisplaySettings         = { 0 };
    AppliedDisplaySettings.Version = API_VERSION;
    AppliedDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    AppliedDisplaySettings.Set     = FALSE;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &AppliedDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (ContentType GET CALL)");
    APP_LOG_INFO(" Current ContentType is %d ", AppliedDisplaySettings.ContentType);

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Sample test for Get/Set LowLatency
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestToGetSetLowLatency(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                           = CTL_RESULT_SUCCESS;
    ctl_display_settings_t AppliedDisplaySettings = { 0 };
    ctl_display_settings_t NewDisplaySettings     = { 0 };
    bool IsControllable, IsSupported = FALSE;

    // GET CALL
    AppliedDisplaySettings.Version = API_VERSION;
    AppliedDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    AppliedDisplaySettings.Set     = FALSE;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &AppliedDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (LowLatency GET CALL)");

    IsControllable = (CTL_DISPLAY_SETTING_FLAG_LOW_LATENCY & AppliedDisplaySettings.ControllableFlags) ? TRUE : FALSE;
    IsSupported    = (CTL_DISPLAY_SETTING_FLAG_LOW_LATENCY & AppliedDisplaySettings.SupportedFlags) ? TRUE : FALSE;

    if ((FALSE == IsControllable) || (FALSE == IsSupported))
    {
        APP_LOG_WARN("Get/Set LowLatency is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    APP_LOG_INFO(" Current Applied LowLatency is %d ", AppliedDisplaySettings.LowLatency);

    // CALL TO ENABLE HDR10+ LOW_LATENCY
    NewDisplaySettings.Version = API_VERSION;
    NewDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    NewDisplaySettings.Set     = TRUE;

    NewDisplaySettings.ValidFlags = CTL_DISPLAY_SETTING_FLAG_LOW_LATENCY;
    NewDisplaySettings.LowLatency = CTL_DISPLAY_SETTING_LOW_LATENCY_ENABLED;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &NewDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (LowLatency SET CALL)");

    // GET CALL
    AppliedDisplaySettings         = { 0 };
    AppliedDisplaySettings.Version = API_VERSION;
    AppliedDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    AppliedDisplaySettings.Set     = FALSE;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &AppliedDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (LowLatency GET CALL)");
    APP_LOG_INFO(" Current LowLatency is %d ", AppliedDisplaySettings.LowLatency);

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Sample test for Get/Set SourceTonemapping
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestToGetSetSourceTonemapping(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                           = CTL_RESULT_SUCCESS;
    ctl_display_settings_t AppliedDisplaySettings = { 0 };
    ctl_display_settings_t NewDisplaySettings     = { 0 };
    bool IsControllable, IsSupported = FALSE;

    // GET CALL
    AppliedDisplaySettings.Version = API_VERSION;
    AppliedDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    AppliedDisplaySettings.Set     = FALSE;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &AppliedDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (SourceTM GET CALL)");

    IsControllable = (CTL_DISPLAY_SETTING_FLAG_SOURCE_TM & AppliedDisplaySettings.ControllableFlags) ? TRUE : FALSE;
    IsSupported    = (CTL_DISPLAY_SETTING_FLAG_SOURCE_TM & AppliedDisplaySettings.SupportedFlags) ? TRUE : FALSE;

    if ((FALSE == IsControllable) || (FALSE == IsSupported))
    {
        APP_LOG_WARN("Get/Set SourceTM is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    APP_LOG_INFO(" Current Applied SourceTM is %d ", AppliedDisplaySettings.SourceTM);

    // CALL TO ENABLE HDR10+ SOURCETM
    NewDisplaySettings.Version = API_VERSION;
    NewDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    NewDisplaySettings.Set     = TRUE;

    NewDisplaySettings.ValidFlags = CTL_DISPLAY_SETTING_FLAG_SOURCE_TM;
    NewDisplaySettings.SourceTM   = CTL_DISPLAY_SETTING_SOURCETM_ENABLED;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &NewDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (SourceTM SET CALL)");

    // GET CALL
    AppliedDisplaySettings         = { 0 };
    AppliedDisplaySettings.Version = API_VERSION;
    AppliedDisplaySettings.Size    = sizeof(ctl_display_settings_t);
    AppliedDisplaySettings.Set     = FALSE;

    Result = ctlGetSetDisplaySettings(hDisplayOutput, &AppliedDisplaySettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDisplaySettings (SourceTM GET CALL)");
    APP_LOG_INFO(" Current SourceTM is %d ", AppliedDisplaySettings.SourceTM);

Exit:
    return Result;
}

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

    for (uint32_t DisplayIndex = 0; DisplayIndex < DisplayCount; DisplayIndex++)
    {
        ctl_display_properties_t DisplayProperties = {};
        DisplayProperties.Size                     = sizeof(ctl_display_properties_t);

        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput[DisplayIndex], "hDisplayOutput");

        Result = ctlGetDisplayProperties(hDisplayOutput[DisplayIndex], &DisplayProperties);

        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDisplayProperties");

        bool IsDisplayAttached = DisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED;

        if (FALSE == IsDisplayAttached)
        {
            continue;
        }

        APP_LOG_INFO("Attached Display Count: %d", DisplayIndex);

        // Get/Set Quantization Range
        Result = TestToGetSetQuantizationRange(hDisplayOutput[DisplayIndex]);
        STORE_AND_RESET_ERROR(Result);

        // Get/Set Picture Aspect Ratio
        Result = TestToGetSetPictureAspectRatio(hDisplayOutput[DisplayIndex]);
        STORE_AND_RESET_ERROR(Result);

        // Get/Set Content type
        Result = TestToGetSetContentType(hDisplayOutput[DisplayIndex]);
        STORE_AND_RESET_ERROR(Result);

        // Get/Set HDR10+ Low Latency Flag
        Result = TestToGetSetLowLatency(hDisplayOutput[DisplayIndex]);
        STORE_AND_RESET_ERROR(Result);

        // Get/Set HDR10+ Source Tonemapping Flag
        Result = TestToGetSetSourceTonemapping(hDisplayOutput[DisplayIndex]);
        STORE_AND_RESET_ERROR(Result);
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
ctl_result_t EnumerateTargetDisplays(ctl_display_output_handle_t *hDisplayOutput, uint32_t AdapterCount, ctl_device_adapter_handle_t *hDevices)
{
    ctl_result_t Result   = CTL_RESULT_SUCCESS;
    uint32_t DisplayCount = 0;

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
            APP_LOG_WARN("Invalid Display Count. skipping display enumration for adapter:%d", AdapterIndex);
            continue;
        }

        hDisplayOutput = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);

        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], &DisplayCount, hDisplayOutput);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_WARN("ctlEnumerateDisplayOutputs returned failure code: 0x%X", Result);
            STORE_AND_RESET_ERROR(Result);
        }

        // Only for demonstration purpose, API is called for each of the display output handle in below snippet.
        // User has to filter through the available display output handle and has to call the API with particular display output handle.
        Result = EnumerateDisplayHandles(hDisplayOutput, DisplayCount);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_WARN("EnumerateDisplayHandles returned failure code: 0x%X", Result);
        }

        CTL_FREE_MEM(hDisplayOutput);
    }

Exit:

    CTL_FREE_MEM(hDisplayOutput);
    return Result;
}

/***************************************************************
 * @brief Main Function which calls the Sample Display Settings API
 * @param
 * @return int
 ***************************************************************/
int main()
{
    ctl_result_t Result                                     = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hDevices                   = NULL;
    ctl_display_output_handle_t *hDisplayOutput             = NULL;
    ctl_device_adapter_properties_t DeviceAdapterProperties = { 0 };
    // Get a handle to the DLL module.
    uint32_t AdapterCount = 0;
    uint32_t DisplayCount = 0;
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    ctl_init_args_t CtlInitArgs;
    ctl_api_handle_t hAPIHandle;
    CtlInitArgs.AppVersion = CTL_MAKE_VERSION(CTL_IMPL_MAJOR_VERSION, CTL_IMPL_MINOR_VERSION);
    CtlInitArgs.flags      = 0;
    CtlInitArgs.Size       = sizeof(CtlInitArgs);
    CtlInitArgs.Version    = 0;
    ZeroMemory(&CtlInitArgs.ApplicationUID, sizeof(ctl_application_id_t));

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

    Result = EnumerateTargetDisplays(hDisplayOutput, AdapterCount, hDevices);

    if (CTL_RESULT_SUCCESS != Result)
    {
        APP_LOG_ERROR("EnumerateTargetDisplays returned failure code: 0x%X", Result);
        STORE_AND_RESET_ERROR(Result);
    }

Exit:

    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDisplayOutput);
    CTL_FREE_MEM(hDevices);
    APP_LOG_INFO("Overrall test result is 0x%X", GResult);
    return GResult;
}
