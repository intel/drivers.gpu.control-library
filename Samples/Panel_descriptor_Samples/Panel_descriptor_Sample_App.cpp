//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  Panel_descriptor_Sample_APP.cpp
 * @brief This file contains the Panel_descriptor_Sample_APP & the 'main' function. Program execution begins and ends there.
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
 * GetPanelDescriptor: Sample app to use PanelDescriptor API
 * @param ctl_display_output_handle_t, ctl_panel_descriptor_access_args_t
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t GetPanelDescriptor(ctl_display_output_handle_t hDisplayOutput, ctl_panel_descriptor_access_args_t *pPanelDescArgs)
{
    ctl_result_t Result         = CTL_RESULT_SUCCESS;
    uint32_t DescriptorDataSize = 0;

    Result = ctlPanelDescriptorAccess(hDisplayOutput, pPanelDescArgs);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlPanelDescriptorAccess");

    //  Print the descriptor data size
    DescriptorDataSize = pPanelDescArgs->DescriptorDataSize;
    APP_LOG_INFO("DescriptorDataSize = : %d", DescriptorDataSize);

    // Once we have the size of the descriptor data, do another call to ctlPanelDescriptorAccess with DescriptorDataSize as the value derived from 1st ctlPanelDescriptorAccess
    // call.
    pPanelDescArgs->Size               = sizeof(ctl_panel_descriptor_access_args_t);
    pPanelDescArgs->OpType             = CTL_OPERATION_TYPE_READ; // Currently only Read operation is supported. Write operationnot supported.
    pPanelDescArgs->BlockNumber        = 0;                       // Block number
    pPanelDescArgs->DescriptorDataSize = DescriptorDataSize;
    pPanelDescArgs->pDescriptorData    = (uint8_t *)malloc(DescriptorDataSize * sizeof(uint8_t)); // Allocate memory for the descriptor data

    EXIT_ON_MEM_ALLOC_FAILURE(pPanelDescArgs->pDescriptorData, "PanelDescArgs.pDescriptorData");

    Result = ctlPanelDescriptorAccess(hDisplayOutput, pPanelDescArgs);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlPanelDescriptorAccess");

    APP_LOG_INFO("Panel Descriptor Data on block %d: ", pPanelDescArgs->BlockNumber);
    for (uint32_t i = 0; i < pPanelDescArgs->DescriptorDataSize; i += 16)
    {
        for (uint32_t j = i; j < (i + 16); j++)
        {
            APP_LOG_INFO("0x%02X ", pPanelDescArgs->pDescriptorData[j]);
        }
    }

    // EXTENSION BLOCKS READ : For EDID, Need to get the number of extensions blocks from 127th byte of base block
    uint32_t NumberOfExtnBlocks = pPanelDescArgs->pDescriptorData[126]; // 127th byte specifies the number of extension blocks

    if (0 == NumberOfExtnBlocks)
    {
        APP_LOG_ERROR("No Extn Block found ");
        goto Exit;
    }

    for (uint32_t BlockIndex = 0; BlockIndex < NumberOfExtnBlocks; BlockIndex++)
    {
        ctl_panel_descriptor_access_args_t ExtBlockPanelDescArgs = { 0 };
        uint32_t ExtBlockDescriptorDataSize                      = 0;

        // Need to query the size of the descriptor first and then need to query for the descriptor data
        // For size query, make ctlPanelDescriptorAccess call with DescriptorDataSize equals 0, It returns the size of the descriptor data.
        ExtBlockPanelDescArgs.Size               = sizeof(ctl_panel_descriptor_access_args_t);
        ExtBlockPanelDescArgs.OpType             = CTL_OPERATION_TYPE_READ; // Currently only Read operation is supported. Write operation is not supported.
        ExtBlockPanelDescArgs.BlockNumber        = BlockIndex + 1;          // Blocks numbered 1, 2, 3 ...
        ExtBlockPanelDescArgs.DescriptorDataSize = ExtBlockDescriptorDataSize;

        Result = ctlPanelDescriptorAccess(hDisplayOutput, &ExtBlockPanelDescArgs);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlPanelDescriptorAccess for querying DescriptorDataSize for extension block");

        //  Print the descriptor data size
        ExtBlockDescriptorDataSize = ExtBlockPanelDescArgs.DescriptorDataSize;
        APP_LOG_INFO("DescriptorDataSize for extension block = : %d", ExtBlockDescriptorDataSize);

        // Once we have the size of the descriptor data, do another call to ctlPanelDescriptorAccess with DescriptorDataSize as the value derived from 1st
        // ctlPanelDescriptorAcces call.
        ExtBlockPanelDescArgs                    = { 0 };
        ExtBlockPanelDescArgs.Size               = sizeof(ctl_panel_descriptor_access_args_t);
        ExtBlockPanelDescArgs.OpType             = CTL_OPERATION_TYPE_READ; // Currently only Read operation is supported. Write operationnot supported.
        ExtBlockPanelDescArgs.BlockNumber        = BlockIndex + 1;          // Block number
        ExtBlockPanelDescArgs.DescriptorDataSize = ExtBlockDescriptorDataSize;
        ExtBlockPanelDescArgs.pDescriptorData    = (uint8_t *)malloc(ExtBlockDescriptorDataSize * sizeof(uint8_t)); // Allocate memory for the descriptor data

        EXIT_ON_MEM_ALLOC_FAILURE(ExtBlockPanelDescArgs.pDescriptorData, "ExtBlockPanelDescArgs.pDescriptorData");

        Result = ctlPanelDescriptorAccess(hDisplayOutput, &ExtBlockPanelDescArgs);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlPanelDescriptorAccess for extension block %d returned failure code: 0x%X", BlockIndex, Result);
            CTL_FREE_MEM(ExtBlockPanelDescArgs.pDescriptorData);
            goto Exit;
        }

        APP_LOG_INFO("Panel Descriptor Data on block %d: ", ExtBlockPanelDescArgs.BlockNumber);
        for (uint32_t i = 0; i < pPanelDescArgs->DescriptorDataSize; i += 16)
        {
            for (uint32_t j = i; j < (i + 16); j++)
            {
                APP_LOG_INFO("0x%02X ", ExtBlockPanelDescArgs.pDescriptorData[j]);
            }
        }

        CTL_FREE_MEM(ExtBlockPanelDescArgs.pDescriptorData);
    }

Exit:
    CTL_FREE_MEM(pPanelDescArgs->pDescriptorData);
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

    if (0 == DisplayCount)
    {
        APP_LOG_ERROR("Invalid Display Count ");
        goto Exit;
    }

    for (uint32_t DisplayIndex = 0; DisplayIndex < DisplayCount; DisplayIndex++)
    {
        ctl_display_properties_t DisplayProperties = { 0 };
        DisplayProperties.Size                     = sizeof(ctl_display_properties_t);

        Result = ctlGetDisplayProperties(hDisplayOutput[DisplayIndex], &DisplayProperties);

        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDisplayProperties");

        bool IsDisplayAttached = DisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED;

        if (FALSE == IsDisplayAttached)
        {
            APP_LOG_WARN("Display %d is not attached, skipping the call for this display", DisplayIndex);
            continue;
        }

        // Panel descriptor access
        // Need to query the size of the descriptor first and then need to query for the descriptor data
        // For size query, make ctlPanelDescriptorAccess call with DescriptorDataSize equals 0, It returns the size of the descriptor data.
        ctl_panel_descriptor_access_args_t PanelDescArgs;
        PanelDescArgs               = { 0 };
        uint32_t DescriptorDataSize = 0;

        PanelDescArgs.Size               = sizeof(ctl_panel_descriptor_access_args_t);
        PanelDescArgs.OpType             = CTL_OPERATION_TYPE_READ; // Currently only Read operation is supported. Write operation is not supported.
        PanelDescArgs.BlockNumber        = 0;                       // Block number
        PanelDescArgs.DescriptorDataSize = DescriptorDataSize;

        Result = GetPanelDescriptor(hDisplayOutput[DisplayIndex], &PanelDescArgs);

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
ctl_result_t EnumerateTargetDisplays(uint32_t AdapterCount, ctl_device_adapter_handle_t *hDevices)
{
    ctl_display_output_handle_t *hDisplayOutput = NULL;
    ctl_result_t Result                         = CTL_RESULT_SUCCESS;
    uint32_t DisplayCount                       = 0;

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        // enumerate all the possible target display's for the adapters
        // first step is to get the count
        DisplayCount = 0;

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], &DisplayCount, hDisplayOutput);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlEnumerateDisplayOutputs returned failure code: 0x%X", Result);
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
            APP_LOG_ERROR("ctlEnumerateDisplayOutputs returned failure code: 0x%X", Result);
            STORE_AND_RESET_ERROR(Result);
        }

        // Only for demonstration purpose, API is called for each of the display output handle in below snippet.
        // User has to filter through the available display output handle and has to call the API with particular display output handle.
        Result = EnumerateDisplayHandles(hDisplayOutput, DisplayCount);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("EnumerateDisplayHandles returned failure code: 0x%X", Result);
        }

        CTL_FREE_MEM(hDisplayOutput);
    }

Exit:

    CTL_FREE_MEM(hDisplayOutput);
    return Result;
}

/***************************************************************
 * @brief Main Function which calls the Sample Panel Descriptor API
 * @param
 * @return int
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

    Result = EnumerateTargetDisplays(AdapterCount, hDevices);

    if (CTL_RESULT_SUCCESS != Result)
    {
        APP_LOG_ERROR("EnumerateTargetDisplays returned failure code: 0x%X", Result);
        STORE_AND_RESET_ERROR(Result);
    }

Exit:
    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDevices);
    APP_LOG_INFO("Overrall test result is 0x%X", GResult);
    return GResult;
}