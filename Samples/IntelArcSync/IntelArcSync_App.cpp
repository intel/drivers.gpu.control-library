//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  IntelArcSync_APP.cpp
 * @brief This file contains the 'main' function. Program execution begins and ends there.
 *
 */

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>

#define CTL_APIEXPORT // caller of control API DLL shall define this before including igcl_api.h

#include "igcl_api.h"

/***************************************************************
 * @brief
 * Example for getting Intel Arc Sync capabilities for selected monitor.
 ***************************************************************/
bool IsIntelArcSyncSupportedOnMonitor(ctl_display_output_handle_t hDisplayOutput, bool PrintCaps)
{
    ctl_intel_arc_sync_monitor_params_t StIntelArcSyncMonitorParams;

    StIntelArcSyncMonitorParams.Size    = sizeof(ctl_intel_arc_sync_monitor_params_t);
    StIntelArcSyncMonitorParams.Version = 0;
    const ctl_result_t Result           = ctlGetIntelArcSyncInfoForMonitor(hDisplayOutput, &StIntelArcSyncMonitorParams);
    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("\nERROR: Failed in ctlGetIntelArcSyncInfoForMonitor. ErrorCode= 0x%X", Result);
        return FALSE;
    }

    if (true == PrintCaps)
    {
        printf("\n\nINFO: Intel Arc Sync caps for monitor");
        printf("\n\tIsIntelArcSyncSupported=%d", StIntelArcSyncMonitorParams.IsIntelArcSyncSupported);
    }

    if (TRUE != StIntelArcSyncMonitorParams.IsIntelArcSyncSupported)
    {
        return FALSE;
    }

    if (true == PrintCaps)
    {
        printf("\n\tMaxRrInHz=%.4f", StIntelArcSyncMonitorParams.MaximumRefreshRateInHz);
        printf("\n\tMinRrInHz=%.4f", StIntelArcSyncMonitorParams.MinimumRefreshRateInHz);
        printf("\n\tMaxFrameTimeDecreaseInUs=%d", StIntelArcSyncMonitorParams.MaxFrameTimeDecreaseInUs);
        printf("\n\tMaxFrameTimeIncreaseInUs=%d", StIntelArcSyncMonitorParams.MaxFrameTimeIncreaseInUs);
    }

    return TRUE;
}

/***************************************************************
 * @brief
 * Example for getting current Intel Arc Sync profile for selected monitor
 * @param hDisplayOutput Display Output Handle
 ***************************************************************/
void PrintCurrentIntelArcSyncProfileConfig(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_intel_arc_sync_profile_params_t StIntelArcSyncProfileParams;

    StIntelArcSyncProfileParams.Size    = sizeof(ctl_intel_arc_sync_profile_params_t);
    StIntelArcSyncProfileParams.Version = 0;
    const ctl_result_t Result           = ctlGetIntelArcSyncProfile(hDisplayOutput, &StIntelArcSyncProfileParams);
    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("\nERROR: Failed in ctlGetIntelArcSyncProfile. ErrorCode= 0x%X", Result);
        return;
    }

    printf("\n\nINFO: Intel Arc Sync Profile info for monitor");
    switch (StIntelArcSyncProfileParams.IntelArcSyncProfile)
    {
        case CTL_INTEL_ARC_SYNC_PROFILE_INVALID:
            printf("\n\tProfileName= INVALID");
            break;
        case CTL_INTEL_ARC_SYNC_PROFILE_RECOMMENDED:
            printf("\n\tProfileName= RECOMMENDED");
            break;
        case CTL_INTEL_ARC_SYNC_PROFILE_EXCELLENT:
            printf("\n\tProfileName= EXCELLENT");
            break;
        case CTL_INTEL_ARC_SYNC_PROFILE_GOOD:
            printf("\n\tProfileName= GOOD");
            break;
        case CTL_INTEL_ARC_SYNC_PROFILE_COMPATIBLE:
            printf("\n\tProfileName= COMPATIBLE");
            break;
        case CTL_INTEL_ARC_SYNC_PROFILE_OFF:
            printf("\n\tProfileName= OFF");
            break;
        case CTL_INTEL_ARC_SYNC_PROFILE_VESA:
            printf("\n\tProfileName= VESA");
            break;
        case CTL_INTEL_ARC_SYNC_PROFILE_CUSTOM:
            printf("\n\tProfileName= CUSTOM");
            break;
        default:
            break;
    }

    printf("\n\tMaxRrInHz=%.4f", StIntelArcSyncProfileParams.MaxRefreshRateInHz);
    printf("\n\tMinRrInHz=%.4f", StIntelArcSyncProfileParams.MinRefreshRateInHz);
    printf("\n\tMaxFrameTimeDecreaseInUs=%d", StIntelArcSyncProfileParams.MaxFrameTimeDecreaseInUs);
    printf("\n\tMaxFrameTimeIncreaseInUs=%d", StIntelArcSyncProfileParams.MaxFrameTimeIncreaseInUs);
}

/***************************************************************
 * @brief
 * Example for changing current Intel Arc Sync profile for selected monitor
 * @param hDisplayOutput Display Output Handle
 * @param Profile Intel Arc Sync Profile
 ***************************************************************/
void ChangeIntelArcSyncProfile(ctl_display_output_handle_t hDisplayOutput, ctl_intel_arc_sync_profile_t Profile)
{
    ctl_intel_arc_sync_profile_params_t StIntelArcSyncProfileParams;

    StIntelArcSyncProfileParams                     = { 0 };
    StIntelArcSyncProfileParams.Size                = sizeof(ctl_intel_arc_sync_profile_params_t);
    StIntelArcSyncProfileParams.Version             = 0;
    StIntelArcSyncProfileParams.IntelArcSyncProfile = Profile;
    const ctl_result_t Result                       = ctlSetIntelArcSyncProfile(hDisplayOutput, &StIntelArcSyncProfileParams);
    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("\nERROR: Failed in ctlSetIntelArcSyncProfile. ErrorCode= 0x%X", Result);
        return;
    }
    printf("\nINFO: Successfully changed the Intel Arc Sync profile");
}

/***************************************************************
 * @brief
 * Apply a custom Intel Arc Sync profile for selected monitor
 ***************************************************************/
ctl_result_t ApplyCustomIntelArcSyncProfile(ctl_display_output_handle_t hDisplayOutput, float MinRr, float MaxRr, uint32_t SfditValue, uint32_t SfddtValue)
{
    ctl_intel_arc_sync_profile_params_t StIntelArcSyncProfileParams = { 0 };
    ctl_result_t Result;

    StIntelArcSyncProfileParams.Size                = sizeof(ctl_intel_arc_sync_profile_params_t);
    StIntelArcSyncProfileParams.Version             = 0;
    StIntelArcSyncProfileParams.IntelArcSyncProfile = CTL_INTEL_ARC_SYNC_PROFILE_CUSTOM;

    // Min/Max RR must be in panel supported range
    StIntelArcSyncProfileParams.MinRefreshRateInHz = MinRr;
    StIntelArcSyncProfileParams.MaxRefreshRateInHz = MaxRr;

    StIntelArcSyncProfileParams.MaxFrameTimeIncreaseInUs = SfditValue;
    StIntelArcSyncProfileParams.MaxFrameTimeDecreaseInUs = SfddtValue;

    Result = ctlSetIntelArcSyncProfile(hDisplayOutput, &StIntelArcSyncProfileParams);
    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("\nERROR: Failed in ctlSetIntelArcSyncProfile. ErrorCode= 0x%X", Result);
        return Result;
    }
    printf("\nINFO: Successfully applied a custom Intel Arc Sync profile.");
    return CTL_RESULT_SUCCESS;
}

/***************************************************************
 * @brief
 * Init.
 ***************************************************************/
ctl_result_t Init(ctl_api_handle_t *pAPIHandle, ctl_device_adapter_handle_t **pDevices, uint32_t *pIntelAdapterCount)
{
    ctl_init_args_t CtlInitArgs;

    if (nullptr == pDevices)
    {
        return CTL_RESULT_ERROR_INVALID_ARGUMENT;
    }

    *pDevices           = nullptr;
    *pIntelAdapterCount = 0;

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Initialize Control API
    ZeroMemory(&CtlInitArgs, sizeof(ctl_init_args_t));
    CtlInitArgs.AppVersion = CTL_MAKE_VERSION(CTL_IMPL_MAJOR_VERSION, CTL_IMPL_MINOR_VERSION);
    CtlInitArgs.flags      = 0;
    CtlInitArgs.Size       = sizeof(CtlInitArgs);
    CtlInitArgs.Version    = 0;
    ctl_result_t Result    = ctlInit(&CtlInitArgs, pAPIHandle);
    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("\nERROR: Failed to initialize ControlAPI. Error Code= 0x%X", Result);
        return Result;
    }

    // Get the list of Intel Adapters
    Result = ctlEnumerateDevices(*pAPIHandle, pIntelAdapterCount, *pDevices);
    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("\nERROR: Failed to get the list of Intel Adapters. Error Code= 0x%X", Result);
        return Result;
    }

    // Allocate memory to adapter objects and enumerate
    *pDevices = static_cast<ctl_device_adapter_handle_t *>(malloc(sizeof(ctl_device_adapter_handle_t) * (*pIntelAdapterCount)));
    if (nullptr == *pDevices)
    {
        printf("\nERROR: Failed to allocate memory to adapter handle.");
        return CTL_RESULT_ERROR_OUT_OF_DEVICE_MEMORY;
    }

    Result = ctlEnumerateDevices(*pAPIHandle, pIntelAdapterCount, *pDevices);
    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("\nERROR: Failed to enumerate adapter. Error Code= 0x%X", Result);
        return Result;
    }

    return CTL_RESULT_SUCCESS;
}

/***************************************************************
 * @brief
 * Destroy.
 ***************************************************************/
void Destroy(ctl_api_handle_t hAPIHandle, ctl_device_adapter_handle_t *hDevices)
{
    ctlClose(hAPIHandle);

    if (hDevices != nullptr)
    {
        free(hDevices);
    }
}

/***************************************************************
 * @brief
 * GetActiveDisplays.
 ***************************************************************/
ctl_result_t GetAllDisplays(ctl_device_adapter_handle_t *pDevices, uint32_t IntelAdapterCount, ctl_display_output_handle_t **pDisplayOutput, uint32_t *pDisplayCount)
{
    if (nullptr == pDisplayOutput)
    {
        return CTL_RESULT_ERROR_INVALID_ARGUMENT;
    }

    for (uint32_t AdapterIndex = 0; AdapterIndex < IntelAdapterCount; AdapterIndex++)
    {
        pDisplayCount[AdapterIndex]  = 0;
        pDisplayOutput[AdapterIndex] = nullptr;

        // Enumerate all the possible target display's for the adapters
        ctl_result_t Result = ctlEnumerateDisplayOutputs(pDevices[AdapterIndex], &pDisplayCount[AdapterIndex], pDisplayOutput[AdapterIndex]);
        if (0 == pDisplayCount[AdapterIndex])
        {
            continue;
        }

        pDisplayOutput[AdapterIndex] = static_cast<ctl_display_output_handle_t *>(malloc(sizeof(ctl_display_output_handle_t) * pDisplayCount[AdapterIndex]));
        if (nullptr == pDisplayOutput[AdapterIndex])
        {
            printf("\nERROR: Failed to allocate memory to display handle.");
            return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
        }

        Result = ctlEnumerateDisplayOutputs(pDevices[AdapterIndex], &pDisplayCount[AdapterIndex], pDisplayOutput[AdapterIndex]);
        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("\nERROR: Failed to enumerate display handle. Error Code= 0x%X", Result);
            return Result;
        }
    }

    return CTL_RESULT_SUCCESS;
}

/***************************************************************
 * @brief
 * GetIntelArcSyncCaps.
 ***************************************************************/
void GetIntelArcSyncCaps()
{
    ctl_api_handle_t hAPIHandle;
    ctl_device_adapter_handle_t *hDevices = nullptr;
    ctl_display_output_handle_t *hDisplayOutput[4]; // to store display handles of up to 4 adapters
    uint32_t IntelAdapterCount = 0, DisplayCount[4];
    ctl_result_t Result;

    // Initialize
    if (CTL_RESULT_SUCCESS != Init(&hAPIHandle, &hDevices, &IntelAdapterCount))
    {
        Destroy(hAPIHandle, hDevices);
        return;
    }

    // Get All displays
    if (CTL_RESULT_SUCCESS != GetAllDisplays(hDevices, IntelAdapterCount, hDisplayOutput, DisplayCount))
    {
        Destroy(hAPIHandle, hDevices);
        return;
    }

    // Print Intel Arc Sync caps for all active displays
    for (uint32_t AdapterIndex = 0; AdapterIndex < IntelAdapterCount; AdapterIndex++)
    {
        for (uint32_t DisplayIndex = 0; DisplayIndex < DisplayCount[AdapterIndex]; DisplayIndex++)
        {
            if (nullptr == hDisplayOutput[AdapterIndex])
            {
                continue;
            }

            ctl_display_properties_t stDisplayProperties = {};
            stDisplayProperties.Size                     = sizeof(ctl_display_properties_t);

            Result = ctlGetDisplayProperties(hDisplayOutput[AdapterIndex][DisplayIndex], &stDisplayProperties);
            if (CTL_RESULT_SUCCESS != Result)
            {
                printf("\nERROR: Failed to get display properties. Error Code= 0x%X", Result);
                Destroy(hAPIHandle, hDevices);
                return;
            }

            const bool active   = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE;
            const bool attached = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED;
            if ((false == active) || (false == attached))
            {
                continue;
            }

            IsIntelArcSyncSupportedOnMonitor(hDisplayOutput[AdapterIndex][DisplayIndex], true);
        }
    }

    // Cleanup
    Destroy(hAPIHandle, hDevices);
}

/***************************************************************
 * @brief
 * GetIntelArcSyncProfileDetails.
 ***************************************************************/
void GetIntelArcSyncProfileDetails()
{
    ctl_api_handle_t hAPIHandle;
    ctl_device_adapter_handle_t *hDevices = nullptr;
    ctl_display_output_handle_t *hDisplayOutput[4]; // to store display handles of up to 4 adapters
    uint32_t IntelAdapterCount = 0, DisplayCount[4];
    ctl_result_t Result;

    // Initialize
    if (CTL_RESULT_SUCCESS != Init(&hAPIHandle, &hDevices, &IntelAdapterCount))
    {
        Destroy(hAPIHandle, hDevices);
        return;
    }

    // Get All displays
    if (CTL_RESULT_SUCCESS != GetAllDisplays(hDevices, IntelAdapterCount, hDisplayOutput, DisplayCount))
    {
        Destroy(hAPIHandle, hDevices);
        return;
    }

    // Print Intel Arc Sync active profile for all active displays
    for (uint32_t AdapterIndex = 0; AdapterIndex < IntelAdapterCount; AdapterIndex++)
    {
        for (uint32_t DisplayIndex = 0; DisplayIndex < DisplayCount[AdapterIndex]; DisplayIndex++)
        {
            if (nullptr == hDisplayOutput[AdapterIndex])
            {
                continue;
            }

            ctl_display_properties_t stDisplayProperties = {};
            stDisplayProperties.Size                     = sizeof(ctl_display_properties_t);

            Result = ctlGetDisplayProperties(hDisplayOutput[AdapterIndex][DisplayIndex], &stDisplayProperties);
            if (CTL_RESULT_SUCCESS != Result)
            {
                printf("\nERROR: Failed to get display properties. Error Code= 0x%X", Result);
                Destroy(hAPIHandle, hDevices);
                return;
            }

            const bool active   = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE;
            const bool attached = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED;
            if ((false == active) || (false == attached))
            {
                continue;
            }

            if (true == IsIntelArcSyncSupportedOnMonitor(hDisplayOutput[AdapterIndex][DisplayIndex], false))
            {
                // Get Intel Arc Sync Profile
                PrintCurrentIntelArcSyncProfileConfig(hDisplayOutput[AdapterIndex][DisplayIndex]);
            }
        }
    }

    // Cleanup
    Destroy(hAPIHandle, hDevices);
}

/***************************************************************
 * @brief
 * GetIntelArcSyncProfileNames.
 ***************************************************************/
void GetIntelArcSyncProfileNames()
{
    printf("\nRECOMMENDED");
    printf("\nEXCELLENT");
    printf("\nGOOD");
    printf("\nCOMPATIBLE");
    printf("\nOFF");
    printf("\nVESA");
    printf("\nCUSTOM");
}

/***************************************************************
 * @brief
 * SetIntelArcSyncProfile.
 ***************************************************************/
void SetIntelArcSyncProfile(char *pProfileName, float MinRr, float MaxRr, uint32_t SfditValue, uint32_t SfddtValue)
{
    ctl_intel_arc_sync_profile_t Profile;
    ctl_api_handle_t hAPIHandle;
    ctl_device_adapter_handle_t *hDevices = nullptr;
    ctl_display_output_handle_t *hDisplayOutput[4]; // to store display handles of up to 4 adapters
    uint32_t IntelAdapterCount = 0, DisplayCount[4];
    ctl_result_t Result;

    if ("RECOMMENDED" == std::string(pProfileName))
    {
        Profile = CTL_INTEL_ARC_SYNC_PROFILE_RECOMMENDED;
    }
    else if ("EXCELLENT" == std::string(pProfileName))
    {
        Profile = CTL_INTEL_ARC_SYNC_PROFILE_EXCELLENT;
    }
    else if ("GOOD" == std::string(pProfileName))
    {
        Profile = CTL_INTEL_ARC_SYNC_PROFILE_GOOD;
    }
    else if ("COMPATIBLE" == std::string(pProfileName))
    {
        Profile = CTL_INTEL_ARC_SYNC_PROFILE_COMPATIBLE;
    }
    else if ("OFF" == std::string(pProfileName))
    {
        Profile = CTL_INTEL_ARC_SYNC_PROFILE_OFF;
    }
    else if ("VESA" == std::string(pProfileName))
    {
        Profile = CTL_INTEL_ARC_SYNC_PROFILE_VESA;
    }
    else if ("CUSTOM" == std::string(pProfileName))
    {
        Profile = CTL_INTEL_ARC_SYNC_PROFILE_CUSTOM;
    }
    else
    {
        printf("\nERROR: Invalid profile name");
        return;
    }

    printf("\nINFO: Changing Intel Arc Sync profile to %s", pProfileName);

    // Initialize
    if (CTL_RESULT_SUCCESS != Init(&hAPIHandle, &hDevices, &IntelAdapterCount))
    {
        Destroy(hAPIHandle, hDevices);
        return;
    }

    // Get All displays
    if (CTL_RESULT_SUCCESS != GetAllDisplays(hDevices, IntelAdapterCount, hDisplayOutput, DisplayCount))
    {
        Destroy(hAPIHandle, hDevices);
        return;
    }

    for (uint32_t AdapterIndex = 0; AdapterIndex < IntelAdapterCount; AdapterIndex++)
    {
        for (uint32_t DisplayIndex = 0; DisplayIndex < DisplayCount[AdapterIndex]; DisplayIndex++)
        {
            if (nullptr == hDisplayOutput[AdapterIndex])
            {
                continue;
            }

            ctl_display_properties_t stDisplayProperties = {};
            stDisplayProperties.Size                     = sizeof(ctl_display_properties_t);

            Result = ctlGetDisplayProperties(hDisplayOutput[AdapterIndex][DisplayIndex], &stDisplayProperties);
            if (CTL_RESULT_SUCCESS != Result)
            {
                printf("\nERROR: Failed to get display properties. Error Code= 0x%X", Result);
                Destroy(hAPIHandle, hDevices);
                return;
            }

            const bool active   = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE;
            const bool attached = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED;
            if ((false == active) || (false == attached))
            {
                continue;
            }

            if (true == IsIntelArcSyncSupportedOnMonitor(hDisplayOutput[AdapterIndex][DisplayIndex], false))
            {
                if (CTL_INTEL_ARC_SYNC_PROFILE_CUSTOM == Profile)
                {
                    ApplyCustomIntelArcSyncProfile(hDisplayOutput[AdapterIndex][DisplayIndex], MinRr, MaxRr, SfditValue, SfddtValue);
                }
                else
                {
                    ChangeIntelArcSyncProfile(hDisplayOutput[AdapterIndex][DisplayIndex], Profile);
                }
                PrintCurrentIntelArcSyncProfileConfig(hDisplayOutput[AdapterIndex][DisplayIndex]);
            }
        }
    }

    // Cleanup
    Destroy(hAPIHandle, hDevices);
}

/***************************************************************
 * @brief
 * Print usage.
 ***************************************************************/
void PrintUsage()
{
    printf("\nUsage: IntelArcSync_App.exe [option] [sub-option]");
    printf("\n\nA sample app to control Intel Arc Sync profile options.");
    printf("\n\nOptions");
    printf("\nget");
    printf("\n\tcaps\t\tPrint Intel Arc Sync caps");
    printf("\n\tprofile\t\tPrint active Intel Arc Sync profile details");
    printf("\n\tprofile-names\t\tPrint the list of profile names available");
    printf("\n\nset");
    printf("\n\tprofile\t\tSet the Intel Arc Sync profile");
    printf("\n\t\t<profile-name>\t\tName of Intel Arc Sync profile to apply");
    printf("\n\t\t[<min-rr>]\t\tUsed only in CUSTOM profile. Min RR value in Hz.");
    printf("\n\t\t[<max-rr>]\t\tUsed only in CUSTOM profile. Max RR value in Hz.");
    printf("\n\t\t[<sfdit-value>]\t\tUsed only in CUSTOM profile. SFDIT value in micro seconds.");
    printf("\n\t\t[<sfddt-value>]\t\tUsed only in CUSTOM profile. SFDDT value in micro seconds.");
    printf("\n\n\nExamples:");
    printf("\nIntelArcSync_App.exe get caps");
    printf("\nIntelArcSync_App.exe get profile");
    printf("\nIntelArcSync_App.exe get profile-names");
    printf("\nIntelArcSync_App.exe set profile RECOMMENDED");
    printf("\nIntelArcSync_App.exe set profile OFF");
    printf("\nIntelArcSync_App.exe set profile EXCELLENT");
    printf("\nIntelArcSync_App.exe set profile CUSTOM 60 90 5000 5000");
}

/***************************************************************
 * @brief
 * main
 ***************************************************************/
int main(int argc, char *pArgv[])
{
    // Handle command line arguments
    if (argc <= 1)
    {
        PrintUsage();
        return 0;
    }

    const auto Option = std::string(pArgv[1]);
    if ("get" == Option)
    {
        if (argc <= 2)
        {
            PrintUsage();
            return -1;
        }

        const auto SubOption = std::string(pArgv[2]);

        if ("caps" == SubOption)
        {
            try
            {
                GetIntelArcSyncCaps();
            }
            catch (const std::bad_array_new_length &e)
            {
                printf("%s \n", e.what());
            }
        }
        else if ("profile" == SubOption)
        {
            try
            {
                GetIntelArcSyncProfileDetails();
            }
            catch (const std::bad_array_new_length &e)
            {
                printf("%s \n", e.what());
            }
        }
        else if ("profile-names" == SubOption)
        {
            GetIntelArcSyncProfileNames();
        }
        else
        {
            printf("ERROR: Invalid command line option.");
            PrintUsage();
            return -1;
        }
    }
    else if ("set" == Option)
    {
        if (argc <= 2)
        {
            PrintUsage();
            return -1;
        }
        const auto SubOption = std::string(pArgv[2]);
        if ("profile" == SubOption)
        {
            if (argc <= 3)
            {
                PrintUsage();
                return -1;
            }
            const auto ProfileName = std::string(pArgv[3]);

            if ("CUSTOM" == ProfileName)
            {
                if (argc <= 7)
                {
                    printf("ERROR: Invalid command line option.");
                    PrintUsage();
                    return -1;
                }
                float MinRr, MaxRr;
                uint32_t SfditValue, SfddtValue;
                sscanf_s(pArgv[4], "%f", &MinRr);
                sscanf_s(pArgv[5], "%f", &MaxRr);
                sscanf_s(pArgv[6], "%d", &SfditValue);
                sscanf_s(pArgv[7], "%d", &SfddtValue);
                try
                {
                    SetIntelArcSyncProfile(pArgv[3], MinRr, MaxRr, SfditValue, SfddtValue);
                }
                catch (const std::bad_array_new_length &e)
                {
                    printf("%s \n", e.what());
                }
            }
            else
            {
                try
                {
                    SetIntelArcSyncProfile(pArgv[3], 0, 0, 0, 0);
                }
                catch (const std::bad_array_new_length &e)
                {
                    printf("%s \n", e.what());
                }
            }
        }
        else
        {
            printf("ERROR: Invalid command line option.");
            PrintUsage();
            return -1;
        }
    }
    else
    {
        printf("ERROR: Invalid command line option.");
        PrintUsage();
        return -1;
    }

    return 0;
}
