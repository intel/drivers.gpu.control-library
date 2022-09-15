//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  CustomMode_Sample_APP.cpp
 * @brief : This file contains the 'main' function and the Custom Mode Sample APP. Program execution begins and ends there.
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

#define VOID_POINTER_INCREMENT(p, n) ((void *)((char *)(p) + (n)))

ctl_result_t GResult = CTL_RESULT_SUCCESS;

typedef struct _mode
{
    uint32_t X;
    uint32_t Y;
} mode;

/***************************************************************
 * @brief
 * Get Custom Modes
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t GetCustomModes(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                                = CTL_RESULT_SUCCESS;
    ctl_get_set_custom_mode_args_t GetNumofCustomModes = { 0 };
    ctl_get_set_custom_mode_args_t GetCustomModes      = { 0 };
    ctl_custom_src_mode_t *pCustomSourceModes          = NULL;
    ctl_custom_src_mode_t *pCustomSourceModesList      = NULL;
    uint8_t NumOfCustomModes                           = 0;
    size_t pCustomModeSourceSize                       = 0;

    GetNumofCustomModes.NumOfModes         = 0;
    GetNumofCustomModes.CustomModeOpType   = CTL_CUSTOM_MODE_OPERATION_TYPES_GET_CUSTOM_SOURCE_MODES;
    GetNumofCustomModes.pCustomSrcModeList = NULL;
    GetNumofCustomModes.Size               = sizeof(ctl_get_set_custom_mode_args_t);
    GetNumofCustomModes.Version            = 0;

    // Get number of custom modes already added.
    Result = ctlGetSetCustomMode(hDisplayOutput, &GetNumofCustomModes);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetCustomMode");

    NumOfCustomModes = GetNumofCustomModes.NumOfModes;

    if (0 == NumOfCustomModes)
    {
        printf("\n No Custom modes added.\n");
        goto Exit;
    }
    else
    {
        printf("\n Number of custom modes:%d", NumOfCustomModes);
    }

    pCustomModeSourceSize = NumOfCustomModes * sizeof(ctl_custom_src_mode_t);

    pCustomSourceModes = (ctl_custom_src_mode_t *)malloc(pCustomModeSourceSize);

    EXIT_ON_MEM_ALLOC_FAILURE(pCustomSourceModes, "pCustomSourceModes");

    memset(pCustomSourceModes, 0, sizeof(pCustomSourceModes));

    GetCustomModes.NumOfModes         = NumOfCustomModes;
    GetCustomModes.CustomModeOpType   = CTL_CUSTOM_MODE_OPERATION_TYPES_GET_CUSTOM_SOURCE_MODES;
    GetCustomModes.pCustomSrcModeList = pCustomSourceModes;
    GetCustomModes.Size               = sizeof(ctl_get_set_custom_mode_args_t);
    GetCustomModes.Version            = 0;

    // Get custom modes already added.
    Result = ctlGetSetCustomMode(hDisplayOutput, &GetCustomModes);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetCustomMode");

    pCustomSourceModesList = pCustomSourceModes;

    printf("\n No.\tSourceX\tSourceY\n");
    for (uint8_t i = 0; i < NumOfCustomModes; i++)
    {
        printf(" %d\t%d\t%d\n", i, pCustomSourceModesList->SourceX, pCustomSourceModesList->SourceY);
        pCustomSourceModesList++;
    }

    printf("\n");

Exit:
    CTL_FREE_MEM(pCustomSourceModes);
    return Result;
}

/***************************************************************
 * @brief
 * Add Custom Mode(At a time only one custom mode add operation is supported)
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t AddCustomModes(ctl_display_output_handle_t hDisplayOutput, uint32_t SourceX, uint32_t SourceY)
{
    ctl_result_t Result                          = CTL_RESULT_SUCCESS;
    ctl_get_set_custom_mode_args_t AddCustomMode = { 0 };
    ctl_custom_src_mode_t *pCustomSourceMode     = NULL;

    pCustomSourceMode = (ctl_custom_src_mode_t *)malloc(sizeof(ctl_custom_src_mode_t));

    EXIT_ON_MEM_ALLOC_FAILURE(pCustomSourceMode, "pCustomSourceMode");

    memset(pCustomSourceMode, 0, sizeof(pCustomSourceMode));

    AddCustomMode.NumOfModes         = 1;
    AddCustomMode.CustomModeOpType   = CTL_CUSTOM_MODE_OPERATION_TYPES_ADD_CUSTOM_SOURCE_MODE;
    AddCustomMode.pCustomSrcModeList = pCustomSourceMode;
    AddCustomMode.Size               = sizeof(ctl_get_set_custom_mode_args_t);
    AddCustomMode.Version            = 0;

    pCustomSourceMode->SourceX = SourceX;
    pCustomSourceMode->SourceY = SourceY;

    printf("\nAdding Custom mode X:%d Y:%d\n", SourceX, SourceY);

    // Add Custom mode.
    Result = ctlGetSetCustomMode(hDisplayOutput, &AddCustomMode);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetCustomMode");

Exit:
    CTL_FREE_MEM(pCustomSourceMode);
    return Result;
}

/***************************************************************
 * @brief
 * Add Custom Mode(At a time only one custom mode add operation is supported)
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t RemoveCustomModes(ctl_display_output_handle_t hDisplayOutput)
{
    // This is a referance example to remove custom modes.
    // Here we are removing 2 modes at a time.
    // Single or multiple mode removal at a single call is possible.

    ctl_result_t Result                              = CTL_RESULT_SUCCESS;
    ctl_get_set_custom_mode_args_t RemoveCustomModes = { 0 };
    ctl_custom_src_mode_t *pCustomSourceModes        = NULL;
    ctl_custom_src_mode_t *pCustomSourceModesList    = NULL;
    uint8_t NumOfModesToRemove                       = 2;
    mode Modes[2]                                    = { { 822, 522 }, { 824, 524 } };
    size_t pCustomModeSourceSize                     = 0;

    pCustomModeSourceSize = NumOfModesToRemove * sizeof(ctl_custom_src_mode_t);

    pCustomSourceModes = (ctl_custom_src_mode_t *)malloc(pCustomModeSourceSize);

    EXIT_ON_MEM_ALLOC_FAILURE(pCustomSourceModes, "pCustomSourceModes");

    memset(pCustomSourceModes, 0, sizeof(pCustomSourceModes));

    RemoveCustomModes.NumOfModes         = NumOfModesToRemove;
    RemoveCustomModes.CustomModeOpType   = CTL_CUSTOM_MODE_OPERATION_TYPES_REMOVE_CUSTOM_SOURCE_MODES;
    RemoveCustomModes.pCustomSrcModeList = pCustomSourceModes;
    RemoveCustomModes.Size               = sizeof(ctl_get_set_custom_mode_args_t);
    RemoveCustomModes.Version            = 0;

    pCustomSourceModesList = pCustomSourceModes;

    for (uint8_t i = 0; i < NumOfModesToRemove; i++)
    {
        pCustomSourceModesList->SourceX = Modes[i].X;
        pCustomSourceModesList->SourceY = Modes[i].Y;
        pCustomSourceModesList++;
        printf("Removing Custom mode X:%d Y:%d\n", Modes[i].X, Modes[i].Y);
    }

    // Add Custom mode.
    Result = ctlGetSetCustomMode(hDisplayOutput, &RemoveCustomModes);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetCustomMode");

Exit:
    CTL_FREE_MEM(pCustomSourceModes);
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
            printf("Display %d is not attached, skipping the call for this display\n", DisplayIndex);
            continue;
        }
        else
        {
            printf("Attached Display Count: %d\n", DisplayIndex);
        }

        // At a time only one custom mode add operation is supported

        Result = AddCustomModes(hDisplayOutput[DisplayIndex], 822, 522);

        STORE_AND_RESET_ERROR(Result);

        Result = AddCustomModes(hDisplayOutput[DisplayIndex], 1248, 702);

        STORE_AND_RESET_ERROR(Result);

        Result = AddCustomModes(hDisplayOutput[DisplayIndex], 824, 524);

        STORE_AND_RESET_ERROR(Result);

        Result = GetCustomModes(hDisplayOutput[DisplayIndex]);

        STORE_AND_RESET_ERROR(Result);

        // Remove custom mode supports one or multiple custom mode removel at a time.

        Result = RemoveCustomModes(hDisplayOutput[DisplayIndex]);

        STORE_AND_RESET_ERROR(Result);

        Result = GetCustomModes(hDisplayOutput[DisplayIndex]);

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
            printf("ctlEnumerateDisplayOutputs returned failure code: 0x%X\n", Result);
            STORE_AND_RESET_ERROR(Result);
            continue;
        }
        else if (DisplayCount <= 0)
        {
            printf("Invalid Display Count. skipping display enumration for adapter:%d\n", AdapterIndex);
            continue;
        }

        hDisplayOutput = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);

        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], &DisplayCount, hDisplayOutput);

        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlEnumerateDisplayOutputs returned failure code: 0x%X\n", Result);
            STORE_AND_RESET_ERROR(Result);
        }

        // Only for demonstration purpose, API is called for each of the display output handle in below snippet.
        // User has to filter through the available display output handle and has to call the API with particular display output handle.
        Result = EnumerateDisplayHandles(hDisplayOutput, DisplayCount);

        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("EnumerateDisplayHandles returned failure code: 0x%X\n", Result);
        }

        CTL_FREE_MEM(hDisplayOutput);
    }

Exit:

    CTL_FREE_MEM(hDisplayOutput);
    return Result;
}

/***************************************************************
 * @brief Main Function which calls the Sample Power feature API
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

    Result = EnumerateTargetDisplays(hDisplayOutput, AdapterCount, hDevices);

    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("EnumerateTargetDisplays returned failure code: 0x%X\n", Result);
        STORE_AND_RESET_ERROR(Result);
    }

Exit:

    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDisplayOutput);
    CTL_FREE_MEM(hDevices);
    printf("Overrall test result is 0x%X\n", GResult);
    return GResult;
}
