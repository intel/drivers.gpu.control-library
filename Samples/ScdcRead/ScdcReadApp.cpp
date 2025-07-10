//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  ScdcReadApp.cpp
 * @brief This file contains the SCDC Read & the 'main' function. Program execution begins and ends there.
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
 * @brief ScdcReadFromPanel
 * Reference code to read SCDC data from panel
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t ScdcReadFromPanel(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result           = CTL_RESULT_SUCCESS;
    ctl_i2c_access_args_t I2CArgs = { 0 }; // I2C Access
    uint32_t Count                = 0;

    I2CArgs.Size   = sizeof(ctl_i2c_access_args_t);
    I2CArgs.OpType = CTL_OPERATION_TYPE_READ;

    EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

    I2CArgs.Address = 0xA8;
    printf("\nReading SCDC...\n");

    I2CArgs.Offset   = 0x00;
    I2CArgs.DataSize = 0x80;

    Result = ctlI2CAccess(hDisplayOutput, &I2CArgs);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlI2CAccess for I2C read");

    //  Print the data
    for (uint32_t i = 0; i < I2CArgs.DataSize / 16; i++)
    {
        for (uint32_t j = 0; j < 16; j++)
            printf("0x%02X ", I2CArgs.Data[j + (i * 16)]);
        printf("\n\n");
    }

Exit:

    return Result;
}

/***************************************************************
 * @brief ReadScdcData
 *
 * @param hDisplayOutput, DisplayCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t ReadScdcData(ctl_display_output_handle_t *hDisplayOutput, uint32_t DisplayCount)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    for (uint32_t DisplayIndex = 0; DisplayIndex < DisplayCount; DisplayIndex++)
    {
        ctl_display_properties_t DisplayProperties = { 0 };
        DisplayProperties.Size                     = sizeof(ctl_display_properties_t);

        Result = ctlGetDisplayProperties(hDisplayOutput[DisplayIndex], &DisplayProperties);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDisplayProperties");

        bool IsDisplayAttached = (0 != (DisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED));

        if (FALSE == IsDisplayAttached)
        {
            printf("Display %d is not attached, skipping the call for this display\n", DisplayIndex);
            continue;
        }

        if ((DisplayProperties.Type != CTL_DISPLAY_OUTPUT_TYPES_HDMI) &&
            ((DisplayProperties.ProtocolConverterType == 0) || (DisplayProperties.ProtocolConverterOutput != CTL_DISPLAY_OUTPUT_TYPES_HDMI)))
        {
            printf("Display %d is not HDMI, skipping the call for this display\n", DisplayIndex);
            continue;
        }

        Result = ScdcReadFromPanel(hDisplayOutput[DisplayIndex]);
        STORE_AND_RESET_ERROR(Result);
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief EnumerateTargetDisplays
 * Enumerates all the possible target display's for the adapters
 * @param hDisplayOutput, AdapterCount, hDevices, **hDisplayOutput, *pDisplayCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t EnumerateTargetDisplays(uint32_t AdapterCount, ctl_device_adapter_handle_t *hDevices, ctl_display_output_handle_t **hDisplayOutput, uint32_t *pDisplayCount)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        // enumerate all the possible target display's for the adapters
        // first step is to get the count
        *pDisplayCount = 0;

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], pDisplayCount, *hDisplayOutput);

        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlEnumerateDisplayOutputs returned failure to get # of displays code: 0x%X\n", Result);
            STORE_AND_RESET_ERROR(Result);
            continue;
        }
        else if (*pDisplayCount <= 0)
        {
            printf("Invalid Display Count. skipping display enumration for adapter:%d\n", AdapterIndex);
            continue;
        }

        *hDisplayOutput = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * (*pDisplayCount));
        EXIT_ON_MEM_ALLOC_FAILURE(hDisplayOutput, "hDisplayOutput");

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], pDisplayCount, *hDisplayOutput);

        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlEnumerateDisplayOutputs returned failure to get handles code: 0x%X\n", Result);
            STORE_AND_RESET_ERROR(Result);
        }
    }

Exit:

    return Result;
}

/***************************************************************
 * @brief Main Function which calls the Sample I2CAuxAccess API for SCDC reads
 * @param
 * @return int
 ***************************************************************/
int main()
{
    ctl_result_t Result                         = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hDevices       = NULL;
    ctl_display_output_handle_t *hDisplayOutput = NULL;
    uint32_t DisplayCount;

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
        printf("%s \n", e.what());
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
        printf("%s \n", e.what());
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
        printf("%s \n", e.what());
    }

    Result = EnumerateTargetDisplays(AdapterCount, hDevices, &hDisplayOutput, &DisplayCount);

    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("EnumerateTargetDisplays returned failure code: 0x%X\n", Result);
        STORE_AND_RESET_ERROR(Result);
    }

    // Only for demonstration purpose, API is called for each of the display output handle in below snippet.
    // User has to filter through the available display output handle and has to call the API with particular display output handle.
    Result = ReadScdcData(hDisplayOutput, DisplayCount);

    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("EnumerateDisplayHandles returned failure code: 0x%X\n", Result);
    }

Exit:

    CTL_FREE_MEM(hDisplayOutput);

    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDevices);
    printf("Overrall test result is 0x%X\n", GResult);
    return GResult;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add
//   Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project
//   and select the .sln file
