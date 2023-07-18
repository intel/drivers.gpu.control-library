//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file Edid_Mgmt_Sample_App.cpp
 * @brief This file contains the Edid Management Sample App & the 'main' function. Program execution begins and ends there.
 *
 */

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>
#include <fstream>
using namespace std;

#define CTL_APIEXPORT // caller of control API DLL shall define this before
                      // including igcl_api.h
#include "igcl_api.h"
#include "GenericIGCLApp.h"

static ctl_result_t GResult                        = CTL_RESULT_SUCCESS;
static ctl_edid_management_optype_t EdidMgmtOpType = CTL_EDID_MANAGEMENT_OPTYPE_MAX; // init to MAX by default, test will run all scenarios by default
static uint32_t TgtId                              = 0;
static bool IsCustomEdid                           = false;
static uint32_t AdapterNumber                      = 0;
static uint8_t EdidOverrideBuf[] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x10, 0xAC, 0x16, 0xF0, 0x4C, 0x4E, 0x37, 0x30, 0x17, 0x15, 0x01, 0x03, 0x80, 0x34, 0x20, 0x78, 0xEA, 0x1E,
                                     0xC5, 0xAE, 0x4F, 0x34, 0xB1, 0x26, 0x0E, 0x50, 0x54, 0xA5, 0x4B, 0x00, 0x81, 0x80, 0xA9, 0x40, 0xD1, 0x00, 0x71, 0x4F, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                                     0x01, 0x01, 0x28, 0x3C, 0x80, 0xA0, 0x70, 0xB0, 0x23, 0x40, 0x30, 0x20, 0x36, 0x00, 0x06, 0x44, 0x21, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x4A,
                                     0x32, 0x35, 0x37, 0x4D, 0x31, 0x36, 0x31, 0x30, 0x37, 0x4E, 0x4C, 0x0A, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x49, 0x47, 0x43, 0x4C, 0x20, 0x44, 0x45, 0x4C, 0x55,
                                     0x32, 0x34, 0x31, 0x30, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x38, 0x4C, 0x1E, 0x51, 0x11, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xEC, 0x02, 0x03,
                                     0x29, 0xF1, 0x50, 0x90, 0x05, 0x04, 0x03, 0x02, 0x07, 0x16, 0x01, 0x1F, 0x12, 0x13, 0x14, 0x20, 0x15, 0x11, 0x06, 0x23, 0x09, 0x07, 0x07, 0x67, 0x03, 0x0C,
                                     0x00, 0x10, 0x00, 0x38, 0x2D, 0x83, 0x01, 0x00, 0x00, 0xE3, 0x05, 0x03, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C, 0x45, 0x00, 0x06,
                                     0x44, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20, 0x58, 0x2C, 0x25, 0x00, 0x06, 0x44, 0x21, 0x00, 0x00, 0x9E, 0x01, 0x1D, 0x00,
                                     0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x28, 0x55, 0x00, 0x06, 0x44, 0x21, 0x00, 0x00, 0x1E, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96,
                                     0x00, 0x06, 0x44, 0x21, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E };

/***************************************************************
 * @brief PrintUsage
 * Print usage of EDID management sample app
 * @param pArgv[]
 * @return void
 ***************************************************************/
void PrintUsage(char *pArgv[])
{
    printf("EDID Management Sample Test Application.\n");
    printf("\nUsage: %s [Operation] <-a Adapter#> <-t Target_ID> <-e Bin_Filename>\n", pArgv[0]);
    printf("\nOperation\n");
    printf("\t'-lock' : Locks Monitor EDID\n");
    printf("\t'-unlock' : Unlock Previous Operation\n");
    printf("\t'-over' : Override EDID\n");
    printf("\t'-rem' : Remove EDID\n");
    printf("\t'-read' : Read Current Active EDID\n");
    printf("\t'-help' : Display usage\n");
    printf("-a [Adapter #] : Optional Arg\n");
    printf("\tSpecify adapter number [min:0, max:4]\n");
    printf("\t0 (default) - applies to all adapters\n");
    printf("\t1 - applies to the first enumerated adapter\n");
    printf("\t2 - applies to the second enumerated adapter\n");
    printf("\t3 - applies to the third enumerated adapter\n");
    printf("\t4 - applies to the fourth enumerated adapter\n");
    printf("-t [Target ID] : Optional Arg\n");
    printf("\tSpecify Target ID in Hex. e.g '1040'\n");
    printf("-e [path\\to\\EDID binary] : Optional Arg\n");
    printf("\tSpecify EDID binary\n");
}

/**
 * \brief Wrapper to call ATL API.
 *
 * \param[in] hDisplayOutput
 * \param[in] OpType
 * \param[in] EdidType
 * \param[in, out] pEdidSz
 * \param[in, out] pEdidBuf
 * \return ctl_result_t
 */
ctl_result_t EdidMgmtApi(ctl_display_output_handle_t hDisplayOutput, const ctl_edid_management_optype_t &OpType, const ctl_edid_type_t &EdidType = CTL_EDID_TYPE_CURRENT, uint32_t *pEdidSz = nullptr,
                         uint8_t *const pEdidBuf = nullptr)
{
    ctl_result_t Result;
    ctl_edid_management_args_t EdidMgmtArgs{ 0 };

    EdidMgmtArgs.Size     = sizeof(EdidMgmtArgs);
    EdidMgmtArgs.OpType   = OpType;
    EdidMgmtArgs.EdidType = EdidType;
    EdidMgmtArgs.EdidSize = (pEdidSz) ? (*pEdidSz) : 0;
    EdidMgmtArgs.pEdidBuf = pEdidBuf;

    Result = ctlEdidManagement(hDisplayOutput, &EdidMgmtArgs);
    if (CTL_RESULT_SUCCESS != Result)
    {
        return Result;
    }

    if ((EdidMgmtArgs.OutFlags & CTL_EDID_MANAGEMENT_OUT_FLAG_DISPLAY_CONNECTED) != 0)
    {
        printf("info: Out flags : Physical Display is connected.\n");
    }

    if (CTL_EDID_MANAGEMENT_OPTYPE_READ_EDID == OpType)
    {
        // fill out args
        if (pEdidSz)
        {
            *pEdidSz = EdidMgmtArgs.EdidSize;
            printf("EDID Size %d.\n", EdidMgmtArgs.EdidSize);
        }
        if (pEdidBuf)
        {
            // print EDID
            for (uint32_t j = 0; j < EdidMgmtArgs.EdidSize; j++)
            {
                printf("0x%02X ", pEdidBuf[j]);
            }
            printf("\n");
        }
        if ((EdidMgmtArgs.OutFlags & CTL_EDID_MANAGEMENT_OUT_FLAG_SUPPLIED_EDID) != 0)
        {
            printf("info: Out flags : Current EDID is IGCL Supplied.\n");
        }
        else if ((EdidMgmtArgs.OutFlags & CTL_EDID_MANAGEMENT_OUT_FLAG_MONITOR_EDID) != 0)
        {
            printf("info: Out flags : Current EDID is Monitor Data.\n");
        }
    }

    // wait based on out flag, to allow time for Config Switches by OS
    if ((EdidMgmtArgs.OutFlags & CTL_EDID_MANAGEMENT_OUT_FLAG_OS_CONN_NOTIFICATION) != 0)
    {
        printf("info: Waiting for 5 secs to allow any OS Config Switch.\n");
        Sleep(5000);
    }
    return Result;
}
/***************************************************************
 * @brief Reference code to use EDID management API.
 * When App is launched w/o any args, all test scenarios will be run.
 *
 * @param hDisplayOutput
 * @param DisplayProperties
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestEDIDManagement(ctl_display_output_handle_t hDisplayOutput, ctl_display_properties_t &DisplayProperties)
{
    ctl_result_t Result    = CTL_RESULT_SUCCESS; // default to SUCCESS to ignore unplugged displays
    uint32_t EdidSize      = 0;
    uint8_t *pEdidBuf      = nullptr;
    bool IsDisplayAttached = (0 != (DisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED));

    printf("Info: Start EDID Management Tests for TargetID: %x, Type %d, Config 0x%x, DisplayMuxType %d, PConOutType %d.\n", DisplayProperties.Os_display_encoder_handle.WindowsDisplayEncoderID,
           DisplayProperties.Type, DisplayProperties.DisplayConfigFlags, DisplayProperties.AttachedDisplayMuxType, DisplayProperties.ProtocolConverterOutput);
    if (TgtId > 0) // individual display control
    {
        // Below operations possible on detached target
        switch (EdidMgmtOpType)
        {
            case CTL_EDID_MANAGEMENT_OPTYPE_LOCK_EDID:
                // For Sample and quick testing purpose, lock edid (supplied) for detached display when commandline arg is passed for specific target id.
                if (true == IsDisplayAttached)
                {
                    Result = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_LOCK_EDID, CTL_EDID_TYPE_MONITOR);
                }
                else
                {
                    EdidSize = sizeof(EdidOverrideBuf);
                    Result   = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_LOCK_EDID, CTL_EDID_TYPE_OVERRIDE, &EdidSize, EdidOverrideBuf);
                }
                LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:LOCK supplied EDID");
                printf("Info: Passed Test Lock supplied EDID.\n");
                break;
            case CTL_EDID_MANAGEMENT_OPTYPE_UNLOCK_EDID:
                Result = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_UNLOCK_EDID);
                LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:UNLOCK EDID");
                printf("Info: Passed Test Unlock EDID.\n");
                break;
            case CTL_EDID_MANAGEMENT_OPTYPE_OVERRIDE_EDID:
                EdidSize = sizeof(EdidOverrideBuf);
                Result   = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_OVERRIDE_EDID, CTL_EDID_TYPE_OVERRIDE, &EdidSize, EdidOverrideBuf);
                LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:override EDID");
                printf("Info: Passed Test override EDID.\n");
                break;
            case CTL_EDID_MANAGEMENT_OPTYPE_UNDO_OVERRIDE_EDID:
                Result = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_UNDO_OVERRIDE_EDID);
                LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:remove EDID");
                printf("Info: Passed Test remove overridden EDID.\n");
                break;
            case CTL_EDID_MANAGEMENT_OPTYPE_READ_EDID:
                // Pass 1: read EDID size
                EdidSize = 0;
                Result   = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_READ_EDID, CTL_EDID_TYPE_CURRENT, &EdidSize);
                LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:READ EDID 1");
                // Pass 2: read block
                pEdidBuf = (uint8_t *)malloc(EdidSize);
                EXIT_ON_MEM_ALLOC_FAILURE(pEdidBuf, "pEdidBuf");
                Result = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_READ_EDID, CTL_EDID_TYPE_CURRENT, &EdidSize, pEdidBuf);
                LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:READ EDID 2");
                printf("\nInfo: Passed Test read EDID.\n");
                break;
            default:
                break;
        }
    }
    else if (IsDisplayAttached) // attached displays
    {
        if (CTL_EDID_MANAGEMENT_OPTYPE_LOCK_EDID == EdidMgmtOpType || CTL_EDID_MANAGEMENT_OPTYPE_MAX == EdidMgmtOpType)
        {
            if (true == IsCustomEdid)
            {
                EdidSize = sizeof(EdidOverrideBuf);
                Result   = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_LOCK_EDID, CTL_EDID_TYPE_OVERRIDE, &EdidSize, EdidOverrideBuf);
            }
            else
            {
                Result = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_LOCK_EDID, CTL_EDID_TYPE_MONITOR);
            }
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:LOCK Monitor EDID");
            printf("Info: Passed Test Lock Monitor EDID.\n");
        }

        if (CTL_EDID_MANAGEMENT_OPTYPE_UNLOCK_EDID == EdidMgmtOpType || CTL_EDID_MANAGEMENT_OPTYPE_MAX == EdidMgmtOpType)
        {
            Result = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_UNLOCK_EDID);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:UNLOCK EDID");
            printf("Info: Passed Test Unlock EDID.\n");
        }

        if (CTL_EDID_MANAGEMENT_OPTYPE_OVERRIDE_EDID == EdidMgmtOpType || CTL_EDID_MANAGEMENT_OPTYPE_MAX == EdidMgmtOpType)
        {
            EdidSize = sizeof(EdidOverrideBuf);
            Result   = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_OVERRIDE_EDID, CTL_EDID_TYPE_OVERRIDE, &EdidSize, EdidOverrideBuf);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:override EDID");
            printf("Info: Passed Test override EDID.\n");
        }

        if (CTL_EDID_MANAGEMENT_OPTYPE_READ_EDID == EdidMgmtOpType || CTL_EDID_MANAGEMENT_OPTYPE_MAX == EdidMgmtOpType)
        {
            // Pass 1: read EDID size
            EdidSize = 0;
            Result   = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_READ_EDID, CTL_EDID_TYPE_CURRENT, &EdidSize);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:READ EDID 1");
            // Pass 2: read block
            pEdidBuf = (uint8_t *)malloc(EdidSize);
            EXIT_ON_MEM_ALLOC_FAILURE(pEdidBuf, "pEdidBuf");
            Result = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_READ_EDID, CTL_EDID_TYPE_CURRENT, &EdidSize, pEdidBuf);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:READ EDID 2");

            CTL_FREE_MEM(pEdidBuf);
            printf("\nInfo: Passed Test read EDID.\n");
        }

        if (CTL_EDID_MANAGEMENT_OPTYPE_UNDO_OVERRIDE_EDID == EdidMgmtOpType || CTL_EDID_MANAGEMENT_OPTYPE_MAX == EdidMgmtOpType)
        {
            Result = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_UNDO_OVERRIDE_EDID);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:remove EDID");
            // Check EDID data after removing override.

            // Pass 1: read EDID size
            EdidSize = 0;
            Result   = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_READ_EDID, CTL_EDID_TYPE_CURRENT, &EdidSize);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:READ EDID 1");
            // Pass 2: read block
            pEdidBuf = (uint8_t *)malloc(EdidSize);
            EXIT_ON_MEM_ALLOC_FAILURE(pEdidBuf, "pEdidBuf");
            Result = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_READ_EDID, CTL_EDID_TYPE_CURRENT, &EdidSize, pEdidBuf);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:READ EDID 2");

            CTL_FREE_MEM(pEdidBuf);
            printf("\nInfo: Passed Test remove overridden EDID.\n");
        }
        // When ran the app w/o any commandline args,
        // test EDID lock with Supplied EDID. For demo purpose.
        if (CTL_EDID_MANAGEMENT_OPTYPE_MAX == EdidMgmtOpType)
        {
            EdidSize = sizeof(EdidOverrideBuf);
            Result   = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_LOCK_EDID, CTL_EDID_TYPE_OVERRIDE, &EdidSize, EdidOverrideBuf);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:LOCK supplied EDID");
            printf("Info: Passed Test Lock supplied EDID.\n");

            // unlock display
            Result = EdidMgmtApi(hDisplayOutput, CTL_EDID_MANAGEMENT_OPTYPE_UNLOCK_EDID);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEdidManagement:UNLOCK EDID");
            printf("Info: Passed Test Unlock EDID.\n");
        }
    }

Exit:
    printf("Info: Exit EDID Management Tests for TargetID: %x.\n\n", DisplayProperties.Os_display_encoder_handle.WindowsDisplayEncoderID);
    CTL_FREE_MEM(pEdidBuf);
    return Result;
}

/***************************************************************
 * @brief EnumerateDisplayHandles
 * Only for demonstration purpose, API is called for each of the display output handle in below snippet.
 * User has to filter through the available display output handle and has to call the API with particular display output handle.
 * @param hDisplayOutput
 * @param DisplayCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t EnumerateDisplayHandles(ctl_display_output_handle_t *hDisplayOutput, uint32_t DisplayCount)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    // iterating all displays for demo purpose.
    for (uint32_t DisplayIndex = 0; DisplayIndex < DisplayCount; DisplayIndex++)
    {
        ctl_display_properties_t DisplayProperties = { 0 };
        DisplayProperties.Size                     = sizeof(ctl_display_properties_t);

        Result = ctlGetDisplayProperties(hDisplayOutput[DisplayIndex], &DisplayProperties);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDisplayProperties");

        bool IsDisplayAttached = (0 != (DisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED));

        if ((TgtId != 0) && (TgtId != DisplayProperties.Os_display_encoder_handle.WindowsDisplayEncoderID))
            continue;

        Result = TestEDIDManagement(hDisplayOutput[DisplayIndex], DisplayProperties);
        STORE_AND_RESET_ERROR(Result);
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief EnumerateTargetDisplays
 * Enumerates all the possible target display's for the adapters
 * @param AdapterCount
 * @param hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t EnumerateTargetDisplays(uint32_t AdapterCount, ctl_device_adapter_handle_t *hDevices)
{
    ctl_display_output_handle_t *hDisplayOutput = NULL;
    ctl_result_t Result                         = CTL_RESULT_SUCCESS;
    uint32_t DisplayCount                       = 0;

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        if ((0 != AdapterNumber) && ((AdapterIndex + 1) != AdapterNumber))
        {
            continue;
        }

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
            continue;
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
 * @brief Find option and get option argument from command arguments
 * @param Argc
 * @param pArgv
 * @param Option
 * @param OptionArg
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t FindOptionArg(int32_t Argc, char *pArgv[], const string &Option, string &OptionArg)
{
    for (int32_t i = 0; i < Argc; ++i)
    {
        string Opt = pArgv[i];
        if (0 == Opt.find(Option))
        {
            if ((i + 1) < Argc)
                OptionArg = pArgv[i + 1];

            return CTL_RESULT_SUCCESS;
        }
    }

    return CTL_RESULT_ERROR_INVALID_ARGUMENT;
}

/***************************************************************
 * @brief Main Function which calls the Sample EDID management API
 * @param Argc
 * @param pArgv
 * @return int
 ***************************************************************/
int main(int32_t Argc, char *pArgv[])
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;
    ctl_init_args_t CtlInitArgs;
    ctl_api_handle_t hAPIHandle;
    ctl_device_adapter_handle_t *hDevices = NULL;
    string OptionArg;

    ZeroMemory(&CtlInitArgs, sizeof(ctl_init_args_t));

    CtlInitArgs.AppVersion = CTL_MAKE_VERSION(CTL_IMPL_MAJOR_VERSION, CTL_IMPL_MINOR_VERSION);
    CtlInitArgs.flags      = 0;
    CtlInitArgs.Size       = sizeof(CtlInitArgs);
    CtlInitArgs.Version    = 0;

    Result = ctlInit(&CtlInitArgs, &hAPIHandle);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlInit");

    // Get a handle to the DLL module.
    uint32_t AdapterCount = 0;
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // parse test args
    // Operation option
    if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-lock", OptionArg))
    {
        EdidMgmtOpType = CTL_EDID_MANAGEMENT_OPTYPE_LOCK_EDID;
    }
    else if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-unlock", OptionArg))
    {
        EdidMgmtOpType = CTL_EDID_MANAGEMENT_OPTYPE_UNLOCK_EDID;
    }
    else if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-over", OptionArg))
    {
        EdidMgmtOpType = CTL_EDID_MANAGEMENT_OPTYPE_OVERRIDE_EDID;
    }
    else if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-rem", OptionArg))
    {
        EdidMgmtOpType = CTL_EDID_MANAGEMENT_OPTYPE_UNDO_OVERRIDE_EDID;
    }
    else if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-read", OptionArg))
    {
        EdidMgmtOpType = CTL_EDID_MANAGEMENT_OPTYPE_READ_EDID;
    }
    else if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-help", OptionArg))
    {
        PrintUsage(pArgv);
        Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
        EXIT_ON_ERROR(Result);
    }

    // Adapter number option
    if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-a", OptionArg))
    {
        if ("" != OptionArg)
        {
            for (uint32_t Index = 0; Index < OptionArg.length(); Index++)
            {
                if (!isdigit(OptionArg[Index]))
                {
                    printf("Invalid Adapter Number.\n");
                    Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
                    EXIT_ON_ERROR(Result);
                }
            }
            AdapterNumber = stoul(OptionArg, 0, 16);
            printf("Adapter Number: %d\n", AdapterNumber);
        }
    }
    // Target ID option
    if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-t", OptionArg))
    {
        if ("" != OptionArg)
        {
            for (uint32_t Index = 0; Index < OptionArg.length(); Index++)
            {
                if (!isxdigit(OptionArg[Index]))
                {
                    printf("Invalid Target ID.\n");
                    Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
                    EXIT_ON_ERROR(Result);
                }
            }
            TgtId = stoul(OptionArg, 0, 16);
            printf("Test or Target ID: 0x%X\n", TgtId);
        }
    }
    // EDID binary option
    if (CTL_RESULT_SUCCESS == FindOptionArg(Argc, pArgv, "-e", OptionArg))
    {
        if ("" != OptionArg)
        {
            ifstream EdidFile;
            streampos FileSize;

            EdidFile.open(OptionArg, ios::in | ios::binary | ios::ate);

            if (false == EdidFile.is_open())
            {
                printf("Cannot open a EDID file.\n");
                Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
                EXIT_ON_ERROR(Result);
            }

            FileSize = EdidFile.tellg();
            ZeroMemory(&EdidOverrideBuf, sizeof(EdidOverrideBuf));
            EdidFile.seekg(0, ios::beg);
            EdidFile.read((char *)&EdidOverrideBuf, FileSize);
            EdidFile.close();
            IsCustomEdid = true;
        }
        else
        {
            printf("Fail to find the EDID binary file.\n");
            Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
            EXIT_ON_ERROR(Result);
        }
    }

    // Initialization successful
    // Get the list of Intel Adapters
    Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");

    hDevices = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * AdapterCount);
    EXIT_ON_MEM_ALLOC_FAILURE(hDevices, "hDevices");

    Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");

    Result = EnumerateTargetDisplays(AdapterCount, hDevices);

    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("EnumerateTargetDisplays returned failure code: 0x%X\n", Result);
        STORE_AND_RESET_ERROR(Result);
    }

Exit:
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
