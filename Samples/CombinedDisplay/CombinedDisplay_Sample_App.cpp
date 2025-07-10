//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file CombinedDisplay_Sample_App.cpp
 * This sample app enumerates adapters and enumerates displays for each adapter. For each adapter,
 * combined display operations are called and performed. User needs to input operation type and display positions.
 * Validation checks are strongly encouraged.
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

enum ChildInfoIndex
{
    CHILDINFO_DISPLAY_NUM = 0,
    CHILDINFO_FBSRC_LEFT,
    CHILDINFO_FBSRC_TOP,
    CHILDINFO_FBSRC_RIGHT,
    CHILDINFO_FBSRC_BOTTOM,
    CHILDINFO_FBPOS_LEFT,
    CHILDINFO_FBPOS_TOP,
    CHILDINFO_FBPOS_RIGHT,
    CHILDINFO_FBPOS_BOTTOM,
    CHILDINFO_ORIENTATION,
    CHILDINFO_TARGETMODE_WIDTH,
    CHILDINFO_TARGETMODE_HEIGHT,
};

/***************************************************************
 * @brief PrintUsage
 * Print usage of Combined Display sample app
 * @param pArgv[]
 * @return void
 ***************************************************************/
void PrintUsage(char *pArgv[])
{
    APP_LOG_INFO("Combined Display Sample Test Application.");
    APP_LOG_INFO("Usage: %s [Combined Display Port] <Config File>", pArgv[0]);
    APP_LOG_INFO("Combined Display Port - 0 | 1");
    APP_LOG_INFO("Combined Display port number you want to disable or query. Default is 0.");
    APP_LOG_INFO("Config File - sample config file");
    APP_LOG_INFO("Enable.cfg - Enabling 1x2 mode of Combined Display with 1080p displays");
    APP_LOG_INFO("Disable.cfg - Disabling Combined Display");
    APP_LOG_INFO("Query.cfg - Querying current Combined Display topology");
}

/***************************************************************
 * @brief ParseArguments
 * Parse the OpType, NumOutputs, and Combined Desktop Width / Height from a config file
 * @param pCDArgFile, pCombinedDisplayArgs
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t ParseArguments(const char *pCDArgFile, ctl_combined_display_args_t *pCombinedDisplayArgs)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;
    string LineString;

    // Start reading the file
    ifstream ConfigFile(pCDArgFile);

    if (false == ConfigFile.is_open())
    {
        APP_LOG_ERROR("Cannot open a config file.");
        Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
        EXIT_ON_ERROR(Result);
    }

    while (getline(ConfigFile, LineString))
    {
        string Name, Value;
        size_t DelimiterPos;
        LineString.erase(remove_if(LineString.begin(), LineString.end(), isspace), LineString.end());
        if (LineString[0] == '#' || LineString.empty())
        {
            continue;
        }
        // Read config name and value
        DelimiterPos = LineString.find("=");
        Name         = LineString.substr(0, DelimiterPos);
        Value        = LineString.substr(DelimiterPos + 1);
        DelimiterPos = Value.find("#");
        if (DelimiterPos != string::npos)
        {
            Value.erase(DelimiterPos);
        }

        // Print lines
        APP_LOG_INFO("%s = %s", Name.c_str(), Value.c_str());

        // Fill out combined display config arguments
        pCombinedDisplayArgs->Size    = sizeof(ctl_combined_display_args_t);
        pCombinedDisplayArgs->Version = 1;
        // Operation Type
        if (0 == Name.compare("OpType"))
        {
            pCombinedDisplayArgs->OpType = (ctl_combined_display_optype_t)(stoi(Value));
        }
        // Number of display outputs
        if (0 == Name.compare("NumOutputs"))
        {
            pCombinedDisplayArgs->NumOutputs = stoi(Value);
            if (pCombinedDisplayArgs->OpType == CTL_COMBINED_DISPLAY_OPTYPE_DISABLE || pCombinedDisplayArgs->OpType == CTL_COMBINED_DISPLAY_OPTYPE_QUERY_CONFIG)
            {
                // No need to parse more
                goto Exit;
            }
        }
        // Combined desktop width
        else if (0 == Name.compare("CombinedDesktopWidth"))
        {
            pCombinedDisplayArgs->CombinedDesktopWidth = stoi(Value);
        }
        // Combined desktop height
        else if (0 == Name.compare("CombinedDesktopHeight"))
        {
            pCombinedDisplayArgs->CombinedDesktopHeight = stoi(Value);
        }
    }

Exit:

    ConfigFile.close();

    return Result;
}

/***************************************************************
 * @brief ParseChildInfoArguments
 * Parse child display info arguments from a config file
 * @param pCDArgFile, pCombinedDisplayArgs
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t ParseChildInfoArguments(const char *pCDArgFile, ctl_combined_display_args_t *pCombinedDisplayArgs)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;
    string LineString;
    bool IsSkipLine;

    // Start reading the file
    ifstream ConfigFile(pCDArgFile);

    if (false == ConfigFile.is_open())
    {
        APP_LOG_ERROR("Cannot open a config file.");
        Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
        EXIT_ON_ERROR(Result);
    }

    while (getline(ConfigFile, LineString))
    {
        string Name, Value;
        size_t DelimiterPos;

        IsSkipLine = false;
        LineString.erase(remove_if(LineString.begin(), LineString.end(), isspace), LineString.end());
        if (LineString[0] == '#' || LineString.empty())
        {
            continue;
        }
        // Read config name and value
        DelimiterPos = LineString.find("=");
        Name         = LineString.substr(0, DelimiterPos);
        Value        = LineString.substr(DelimiterPos + 1);
        DelimiterPos = Value.find("#");
        if (DelimiterPos != string::npos)
        {
            Value.erase(DelimiterPos);
        }

        // Child Display Info
        if (0 == Name.compare("ChildInfo"))
        {
            string SubValue;
            string Delimiter = ",";
            uint32_t i       = 0;
            uint32_t Index   = 0;

            Value.erase(remove(Value.begin(), Value.end(), '{'), Value.end());
            Value.erase(remove(Value.begin(), Value.end(), '}'), Value.end());

            DelimiterPos = Value.find(",");
            while (DelimiterPos != string::npos)
            {
                SubValue = Value.substr(0, DelimiterPos);
                Value.erase(0, DelimiterPos + Delimiter.length());
                switch (Index)
                {
                    case CHILDINFO_DISPLAY_NUM:
                        i = stoi(SubValue);
                        // Index of the selected display must be less than NumOutputs
                        if (i >= pCombinedDisplayArgs->NumOutputs)
                        {
                            goto Exit;
                        }
                        break;
                    case CHILDINFO_FBSRC_LEFT:
                        pCombinedDisplayArgs->pChildInfo[i].FbSrc.Left = stoi(SubValue);
                        break;
                    case CHILDINFO_FBSRC_TOP:
                        pCombinedDisplayArgs->pChildInfo[i].FbSrc.Top = stoi(SubValue);
                        break;
                    case CHILDINFO_FBSRC_RIGHT:
                        pCombinedDisplayArgs->pChildInfo[i].FbSrc.Right = stoi(SubValue);
                        break;
                    case CHILDINFO_FBSRC_BOTTOM:
                        pCombinedDisplayArgs->pChildInfo[i].FbSrc.Bottom = stoi(SubValue);
                        break;
                    case CHILDINFO_FBPOS_LEFT:
                        pCombinedDisplayArgs->pChildInfo[i].FbPos.Left = stoi(SubValue);
                        break;
                    case CHILDINFO_FBPOS_TOP:
                        pCombinedDisplayArgs->pChildInfo[i].FbPos.Top = stoi(SubValue);
                        break;
                    case CHILDINFO_FBPOS_RIGHT:
                        pCombinedDisplayArgs->pChildInfo[i].FbPos.Right = stoi(SubValue);
                        break;
                    case CHILDINFO_FBPOS_BOTTOM:
                        pCombinedDisplayArgs->pChildInfo[i].FbPos.Bottom = stoi(SubValue);
                        break;
                    case CHILDINFO_ORIENTATION:
                        uint32_t DisplayOrientation;
                        DisplayOrientation = stoi(SubValue);
                        switch (DisplayOrientation)
                        {
                            //  Rotation - optionally specify 0 or 180 degree, default 0
                            case 0:
                                pCombinedDisplayArgs->pChildInfo[i].DisplayOrientation = CTL_DISPLAY_ORIENTATION_0;
                                break;
                            case 2:
                                pCombinedDisplayArgs->pChildInfo[i].DisplayOrientation = CTL_DISPLAY_ORIENTATION_180;
                                break;
                            default:
                                pCombinedDisplayArgs->pChildInfo[i].DisplayOrientation = CTL_DISPLAY_ORIENTATION_0;
                                break;
                        }
                        break;
                    case CHILDINFO_TARGETMODE_WIDTH:
                        pCombinedDisplayArgs->pChildInfo[i].TargetMode.Width = stoi(SubValue);
                        break;
                    case CHILDINFO_TARGETMODE_HEIGHT:
                        pCombinedDisplayArgs->pChildInfo[i].TargetMode.Height = stoi(SubValue);
                        break;
                    default:
                        break;
                }
                Index++;
                DelimiterPos = Value.find(",");
            }
            SubValue                                                   = Value.substr(0, DelimiterPos);
            pCombinedDisplayArgs->pChildInfo[i].TargetMode.RefreshRate = stof(SubValue);
        }
    }

Exit:

    ConfigFile.close();

    return Result;
}

/***************************************************************
 * @brief ParseDisplayOrderArguments
 * Parse display order arguments from a config file
 * @param pCombinedDisplayArgs, SelectedDisplays[], pCDArgFile
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t ParseDisplayOrderArguments(uint8_t NumOutputs, const char *pCDArgFile, uint32_t SelectedDisplays[])
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;
    string LineString;

    // Start reading the file
    ifstream ConfigFile(pCDArgFile);

    if (false == ConfigFile.is_open())
    {
        APP_LOG_ERROR("Cannot open a config file.");
        Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
        EXIT_ON_ERROR(Result);
    }

    while (getline(ConfigFile, LineString))
    {
        string Name, Value;
        size_t DelimiterPos;

        LineString.erase(remove_if(LineString.begin(), LineString.end(), isspace), LineString.end());
        if (LineString[0] == '#' || LineString.empty())
        {
            continue;
        }
        // Read config name and value
        DelimiterPos = LineString.find("=");
        Name         = LineString.substr(0, DelimiterPos);
        Value        = LineString.substr(DelimiterPos + 1);
        DelimiterPos = Value.find("#");
        if (DelimiterPos != string::npos)
        {
            Value.erase(DelimiterPos);
        }

        // Display outputs order
        if (0 == Name.compare("DisplayOrder"))
        {
            string Order;
            string Delimiter = ",";
            int32_t Index    = 0;

            while ((DelimiterPos = Value.find(",")) != string::npos)
            {
                Order = Value.substr(0, DelimiterPos);
                Value.erase(0, DelimiterPos + Delimiter.length());
                SelectedDisplays[Index++] = stoi(Order);
                if (Index >= NumOutputs - 1)
                {
                    break;
                }
            }
            Order                   = move(Value);
            SelectedDisplays[Index] = stoi(Order);
            break;
        }
    }

Exit:

    ConfigFile.close();

    return Result;
}

/***************************************************************
 * @brief PrintCombinedConfig
 * Print Combined Display configuration
 * @param CombinedDisplayArgs
 * @return void
 ***************************************************************/
void PrintCombinedConfig(const ctl_combined_display_args_t CombinedDisplayArgs)
{
    ctl_rect_t FbSrc, FbPos;
    ctl_child_display_target_mode_t TargetMode;

    PRINT_LOGS("========= Combined Display Configuration =========");
    APP_LOG_INFO("Number of Display Outputs: %u", CombinedDisplayArgs.NumOutputs);
    APP_LOG_INFO("Combined Display Width: %u", CombinedDisplayArgs.CombinedDesktopWidth);
    APP_LOG_INFO("Combined Display Height: %u", CombinedDisplayArgs.CombinedDesktopHeight);
    for (uint8_t i = 0; i < CombinedDisplayArgs.NumOutputs; i++)
    {
        FbSrc      = CombinedDisplayArgs.pChildInfo[i].FbSrc;
        FbPos      = CombinedDisplayArgs.pChildInfo[i].FbPos;
        TargetMode = CombinedDisplayArgs.pChildInfo[i].TargetMode;

        APP_LOG_INFO("Display[%u]: ", i);
        APP_LOG_INFO("{%u,%u,%u,%u},", FbSrc.Left, FbSrc.Top, FbSrc.Right, FbSrc.Bottom);
        APP_LOG_INFO("{%u,%u,%u,%u},", FbPos.Left, FbPos.Top, FbPos.Right, FbPos.Bottom);
        APP_LOG_INFO("%u,", CombinedDisplayArgs.pChildInfo[i].DisplayOrientation);
        APP_LOG_INFO("{%u,%u,%.1f}", TargetMode.Width, TargetMode.Height, TargetMode.RefreshRate);
    }
    PRINT_LOGS("==========================================================");
}

/***************************************************************
 * @brief FillCombinedDisplayArgs
 * Fill Combined Display arguments
 * @param hDevices, pCDArgFile, MaxNumDisplayOutputs, pCombinedDisplayArgs
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t FillCombinedDisplayArgs(ctl_device_adapter_handle_t *hDevices, const char *pCDArgFile, uint8_t MaxNumDisplayOutputs, ctl_combined_display_args_t *pCombinedDisplayArgs)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    // Parse arguments
    Result = ParseArguments(pCDArgFile, pCombinedDisplayArgs);
    LOG_AND_EXIT_ON_ERROR(Result, "ParseArguments");

    // pCombinedDisplayArgs->NumOutputs should not be zero
    if (0 == pCombinedDisplayArgs->NumOutputs)
    {
        Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
        LOG_AND_EXIT_ON_ERROR(Result, "pCombinedDisplayArgs->NumOutputs is 0");
    }

    // Allocate memory for pChildInfo
    if ((pCombinedDisplayArgs->OpType == CTL_COMBINED_DISPLAY_OPTYPE_IS_SUPPORTED_CONFIG) || (pCombinedDisplayArgs->OpType == CTL_COMBINED_DISPLAY_OPTYPE_ENABLE))
    {
        pCombinedDisplayArgs->pChildInfo = (ctl_combined_display_child_info_t *)malloc(sizeof(ctl_combined_display_child_info_t) * pCombinedDisplayArgs->NumOutputs);
        EXIT_ON_MEM_ALLOC_FAILURE(pCombinedDisplayArgs->pChildInfo, "pCombinedDisplayArgs->pChildInfo");

        Result = ParseChildInfoArguments(pCDArgFile, pCombinedDisplayArgs);
        LOG_AND_EXIT_ON_ERROR(Result, "ParseDisplayOrderChildInfoArguments");
    }
    else if (pCombinedDisplayArgs->OpType == CTL_COMBINED_DISPLAY_OPTYPE_DISABLE)
    {
        pCombinedDisplayArgs->NumOutputs = 1;
        pCombinedDisplayArgs->pChildInfo = (ctl_combined_display_child_info_t *)malloc(sizeof(ctl_combined_display_child_info_t) * pCombinedDisplayArgs->NumOutputs);
        EXIT_ON_MEM_ALLOC_FAILURE(pCombinedDisplayArgs->pChildInfo, "pCombinedDisplayArgs->pChildInfo");
        memset(pCombinedDisplayArgs->pChildInfo, 0, sizeof(ctl_combined_display_child_info_t) * pCombinedDisplayArgs->NumOutputs);
    }
    else if (pCombinedDisplayArgs->OpType == CTL_COMBINED_DISPLAY_OPTYPE_QUERY_CONFIG)
    {
        pCombinedDisplayArgs->pChildInfo = (ctl_combined_display_child_info_t *)malloc(sizeof(ctl_combined_display_child_info_t) * MaxNumDisplayOutputs);
        EXIT_ON_MEM_ALLOC_FAILURE(pCombinedDisplayArgs->pChildInfo, "pCombinedDisplayArgs->pChildInfo");
        memset(pCombinedDisplayArgs->pChildInfo, 0, sizeof(ctl_combined_display_child_info_t) * MaxNumDisplayOutputs);
    }

Exit:

    return Result;
}

/***************************************************************
 * @brief TestCombinedDisplay
 * Configure Combined Display with parameters from the config file
 * @param AdapterCount, hDevices, pCDArgFile, CombinedPort
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestCombinedDisplay(uint32_t AdapterCount, ctl_device_adapter_handle_t *hDevices, const char *pCDArgFile, uint8_t CombinedPort)
{
    ctl_result_t Result                                   = CTL_RESULT_SUCCESS;
    ctl_display_output_handle_t *pHDisplayOutput          = NULL;
    ctl_display_output_handle_t *pHCombinedDisplayOutputs = NULL;
    ctl_display_output_handle_t *pHActiveDisplayOutputs   = NULL;
    uint32_t *pSelectedDisplays                           = NULL;
    ctl_combined_display_args_t CombinedDisplayArgs       = { 0 };
    uint32_t DisplayCount                                 = 0;
    uint32_t NumActiveOutputs                             = 0;
    uint8_t ActiveDisplayCount                            = 0;
    uint8_t CombinedDisplayCount                          = 0;
    uint8_t MaxNumDisplayOutputs                          = 0;

    for (uint32_t Index = 0; Index < AdapterCount; Index++)
    {
        DisplayCount         = 0;
        ActiveDisplayCount   = 0;
        CombinedDisplayCount = 0;

        // Get maximum number of display outputs supported
        memset(&CombinedDisplayArgs, 0, sizeof(ctl_combined_display_args_t));
        CombinedDisplayArgs.OpType = CTL_COMBINED_DISPLAY_OPTYPE_IS_SUPPORTED_CONFIG;
        Result                     = ctlGetSetCombinedDisplay(hDevices[Index], &CombinedDisplayArgs);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlCombinedDisplay");

        MaxNumDisplayOutputs           = CombinedDisplayArgs.NumOutputs;
        CombinedDisplayArgs.NumOutputs = 0;

        // Fill Combined Display arguments
        Result = FillCombinedDisplayArgs(hDevices, pCDArgFile, MaxNumDisplayOutputs, &CombinedDisplayArgs);
        LOG_AND_EXIT_ON_ERROR(Result, "FillCombinedDisplayArgs");

        if ((CombinedDisplayArgs.OpType == CTL_COMBINED_DISPLAY_OPTYPE_IS_SUPPORTED_CONFIG) || (CombinedDisplayArgs.OpType == CTL_COMBINED_DISPLAY_OPTYPE_ENABLE))
        {
            pSelectedDisplays = (uint32_t *)malloc(sizeof(uint32_t) * CombinedDisplayArgs.NumOutputs);
            EXIT_ON_MEM_ALLOC_FAILURE(pSelectedDisplays, "pSelectedDisplays");
            memset(pSelectedDisplays, 0, sizeof(uint32_t) * CombinedDisplayArgs.NumOutputs);

            Result = ParseDisplayOrderArguments(CombinedDisplayArgs.NumOutputs, pCDArgFile, pSelectedDisplays);
            LOG_AND_EXIT_ON_ERROR(Result, "ParseDisplayOrderArguments");
        }

        // Enumerate all the possible target displays for the adapters
        // First step is to get the count
        Result = ctlEnumerateDisplayOutputs(hDevices[Index], &DisplayCount, NULL);

        if (CTL_RESULT_SUCCESS != Result)
        {
            LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");
            CTL_FREE_MEM(pSelectedDisplays);
            continue;
        }
        else if (DisplayCount <= 0)
        {
            APP_LOG_WARN("Invalid Display Count. Skipping display enumeration for adapter: %d", Index);
            CTL_FREE_MEM(pSelectedDisplays);
            continue;
        }

        pHDisplayOutput = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
        EXIT_ON_MEM_ALLOC_FAILURE(pHDisplayOutput, "pHDisplayOutput");

        pHActiveDisplayOutputs = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * MaxNumDisplayOutputs);
        EXIT_ON_MEM_ALLOC_FAILURE(pHActiveDisplayOutputs, "pHActiveDisplayOutputs");
        memset(pHActiveDisplayOutputs, 0, sizeof(ctl_display_output_handle_t) * MaxNumDisplayOutputs);

        pHCombinedDisplayOutputs = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * MaxNumDisplayOutputs);
        EXIT_ON_MEM_ALLOC_FAILURE(pHCombinedDisplayOutputs, "pHCombinedDisplayOutputs");
        memset(pHCombinedDisplayOutputs, 0, sizeof(ctl_display_output_handle_t) * MaxNumDisplayOutputs);

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

            Result = ctlGetDisplayProperties(pHDisplayOutput[i], &stDisplayProperties);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDisplayProperties");

            ctl_adapter_display_encoder_properties_t stDisplayEncoderProperties = {};
            stDisplayEncoderProperties.Size                                     = sizeof(ctl_adapter_display_encoder_properties_t);

            Result = ctlGetAdaperDisplayEncoderProperties(pHDisplayOutput[i], &stDisplayEncoderProperties);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlGetAdaperDisplayEncoderProperties");

            // Filter Combined Display output for disable and query operations
            if (CTL_ENCODER_CONFIG_FLAG_COLLAGE_DISPLAY & stDisplayEncoderProperties.EncoderConfigFlags)
            {
                pHCombinedDisplayOutputs[CombinedDisplayCount++] = pHDisplayOutput[i];
            }

            uint32_t CombinedAllowedEncoderTypes =
            CTL_ENCODER_CONFIG_FLAG_TYPEC_CAPABLE | CTL_ENCODER_CONFIG_FLAG_TBT_CAPABLE | CTL_ENCODER_CONFIG_FLAG_DITHERING_SUPPORTED | CTL_ENCODER_CONFIG_FLAG_INTERNAL_DISPLAY;
            bool IsCombinedAvailable = false;

            IsCombinedAvailable = (0 == stDisplayEncoderProperties.EncoderConfigFlags) || (stDisplayEncoderProperties.EncoderConfigFlags & CombinedAllowedEncoderTypes);
            IsDisplayActive     = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE;
            IsDisplayAttached   = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED;

            // Filter active display outputs
            if (IsDisplayActive && IsDisplayAttached && IsCombinedAvailable)
            {
                pHActiveDisplayOutputs[ActiveDisplayCount++] = pHDisplayOutput[i];
            }
        }
        NumActiveOutputs = ActiveDisplayCount;

        if ((CombinedDisplayArgs.OpType == CTL_COMBINED_DISPLAY_OPTYPE_IS_SUPPORTED_CONFIG) || (CombinedDisplayArgs.OpType == CTL_COMBINED_DISPLAY_OPTYPE_ENABLE))
        {
            if (CombinedDisplayArgs.NumOutputs > NumActiveOutputs)
            {
                APP_LOG_ERROR("The input NumOutputs of %u is greater than the system's NumActiveOutputs of %u.", CombinedDisplayArgs.NumOutputs, NumActiveOutputs);
                Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
                EXIT_ON_ERROR(Result);
            }

            // Only 0/180 degree rotation is supported currently
            for (uint8_t i = 0; i < CombinedDisplayArgs.NumOutputs; i++)
            {
                if ((CTL_DISPLAY_ORIENTATION_0 != CombinedDisplayArgs.pChildInfo[i].DisplayOrientation) && (CTL_DISPLAY_ORIENTATION_180 != CombinedDisplayArgs.pChildInfo[i].DisplayOrientation))
                {
                    APP_LOG_ERROR("Only 0/180 degree rotation is supported.");
                    Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
                    EXIT_ON_ERROR(Result);
                }
            }

            // Reshuffle pHActiveDisplayOutputs
            for (uint8_t i = 0; i < CombinedDisplayArgs.NumOutputs; i++)
            {
                CombinedDisplayArgs.pChildInfo[i].hDisplayOutput = pHActiveDisplayOutputs[pSelectedDisplays[i]];
            }

            Result = ctlGetSetCombinedDisplay(hDevices[Index], &CombinedDisplayArgs);
            if (CTL_RESULT_SUCCESS == Result && CombinedDisplayArgs.IsSupported == false)
            {
                APP_LOG_WARN("The following Combined Display configuration is not supported");
                PrintCombinedConfig(CombinedDisplayArgs);
            }
        }
        else if (CombinedDisplayArgs.OpType == CTL_COMBINED_DISPLAY_OPTYPE_DISABLE)
        {
            if (CombinedPort >= MaxNumDisplayOutputs)
            {
                Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
                LOG_AND_EXIT_ON_ERROR(Result, "CombinedPort");
            }

            CombinedDisplayArgs.hCombinedDisplayOutput = pHCombinedDisplayOutputs[CombinedPort];
            if (NULL != CombinedDisplayArgs.hCombinedDisplayOutput)
            {
                Result = ctlGetSetCombinedDisplay(hDevices[Index], &CombinedDisplayArgs);
            }
        }
        else if (CombinedDisplayArgs.OpType == CTL_COMBINED_DISPLAY_OPTYPE_QUERY_CONFIG)
        {
            if (CombinedPort >= MaxNumDisplayOutputs)
            {
                Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
                LOG_AND_EXIT_ON_ERROR(Result, "CombinedPort");
            }

            CombinedDisplayArgs.hCombinedDisplayOutput = pHCombinedDisplayOutputs[CombinedPort];
            if (NULL != CombinedDisplayArgs.hCombinedDisplayOutput)
            {
                Result = ctlGetSetCombinedDisplay(hDevices[Index], &CombinedDisplayArgs);
                // Print out the current config
                if ((CTL_RESULT_SUCCESS == Result) && (CTL_COMBINED_DISPLAY_OPTYPE_QUERY_CONFIG == CombinedDisplayArgs.OpType))
                {
                    PrintCombinedConfig(CombinedDisplayArgs);
                }
            }
        }

        // Free handle display output pointers
        CTL_FREE_MEM(pHDisplayOutput);
        CTL_FREE_MEM(pHActiveDisplayOutputs);
        CTL_FREE_MEM(pHCombinedDisplayOutputs);
        CTL_FREE_MEM(CombinedDisplayArgs.pChildInfo);
        CTL_FREE_MEM(pSelectedDisplays);
    }

Exit:

    CTL_FREE_MEM(pHDisplayOutput);
    CTL_FREE_MEM(pHActiveDisplayOutputs);
    CTL_FREE_MEM(pHCombinedDisplayOutputs);
    CTL_FREE_MEM(CombinedDisplayArgs.pChildInfo);
    CTL_FREE_MEM(pSelectedDisplays);

    return Result;
}

/***************************************************************
 * @brief Main Function which calls Combined Display API
 * @param
 * @return int
 ***************************************************************/
int main(int argc, char *pArgv[])
{
    ctl_result_t Result                   = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hDevices = nullptr;
    uint32_t AdapterCount                 = 0;
    uint8_t CombinedPort                  = 0;
    ctl_init_args_t CtlInitArgs;
    ctl_api_handle_t hAPIHandle = nullptr;
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Get cmdline arguments
    const char *pCDArgFile = nullptr;
    switch (argc)
    {
        case 2:
            pCDArgFile = pArgv[1];
            break;
        case 3:
            CombinedPort = stoi(pArgv[1]);
            pCDArgFile   = pArgv[2];
            break;
        default:
            PrintUsage(pArgv);
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
        Result = TestCombinedDisplay(AdapterCount, hDevices, pCDArgFile, CombinedPort);
        LOG_AND_EXIT_ON_ERROR(Result, "TestCombinedDisplay");
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