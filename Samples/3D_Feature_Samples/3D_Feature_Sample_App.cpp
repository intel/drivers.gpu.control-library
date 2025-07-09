//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  3D_Feature_Sample_App.cpp
 * @brief This file contains the 'main' function. Program execution begins and ends there.
 *
 */

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#define CTL_APIEXPORT // caller of control API DLL shall define this before
                      // including control_api.h
#include "igcl_api.h"
#include "GenericIGCLApp.h"

/***************************************************************
 * @brief Global result value
 ***************************************************************/
ctl_result_t GResult = CTL_RESULT_SUCCESS;
#define STORE_RESET_ERROR(result)     \
    if (result != CTL_RESULT_SUCCESS) \
        GResult = result;             \
    result = CTL_RESULT_SUCCESS;

char TierName[100] = "";

/***************************************************************
 * @brief
 * Method to test Endurance Gaming
 * @param hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t CtlEnduranceGamingTest(ctl_device_adapter_handle_t hDevices)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    PRINT_LOGS("======================Endurance Gaming test -> Per Application settings======================");

    char *pAppName                        = "GTA5.exe";
    ctl_3d_feature_getset_t Get3DProperty = { 0 };
    ctl_3d_feature_getset_t Set3DProperty = { 0 };

    Set3DProperty.Size = sizeof(Set3DProperty);
    Get3DProperty.Size = sizeof(Get3DProperty);

    // Set Endurance gaming only
    ctl_endurance_gaming_t SetEnduranceGaming = { 0 };
    Set3DProperty.FeatureType                 = CTL_3D_FEATURE_ENDURANCE_GAMING;
    Set3DProperty.bSet                        = TRUE;
    Set3DProperty.CustomValueSize             = sizeof(ctl_endurance_gaming_t);
    SetEnduranceGaming.EGControl              = CTL_3D_ENDURANCE_GAMING_CONTROL_TURN_ON;
    SetEnduranceGaming.EGMode                 = CTL_3D_ENDURANCE_GAMING_MODE_BALANCED;
    Set3DProperty.pCustomValue                = &SetEnduranceGaming;
    Set3DProperty.ValueType                   = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
    Set3DProperty.ApplicationName             = pAppName;
    Set3DProperty.ApplicationNameLength       = (int8_t)strlen(pAppName);
    Set3DProperty.Version                     = 0;

    if (NULL != hDevices)
    {
        Result = ctlGetSet3DFeature(hDevices, &Set3DProperty);
        APP_LOG_INFO(" Set3DProperty.ApplicationName = %s", Set3DProperty.ApplicationName);
        APP_LOG_INFO(" SetEnduranceGaming.EGControl = %d", SetEnduranceGaming.EGControl);
        APP_LOG_INFO(" SetEnduranceGaming.EGMode = %d", SetEnduranceGaming.EGMode);
        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlGetSet3DFeature(Set EnduranceGaming) returned failure code: 0x%X", Result);

            return Result;
        }
        else
        {
            APP_LOG_INFO("ctlGetSet3DFeature returned success(Set EnduranceGaming)");
        }
    }

    // Get Endurance gaming only
    Get3DProperty.FeatureType           = CTL_3D_FEATURE_ENDURANCE_GAMING;
    Get3DProperty.bSet                  = FALSE;
    Get3DProperty.CustomValueSize       = sizeof(ctl_endurance_gaming_t);
    Get3DProperty.pCustomValue          = (ctl_endurance_gaming_t *)malloc(sizeof(ctl_endurance_gaming_t));
    Get3DProperty.ValueType             = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
    Get3DProperty.ApplicationName       = pAppName;
    Get3DProperty.ApplicationNameLength = (int8_t)strlen(pAppName);
    Get3DProperty.Version               = 0;

    if (NULL != hDevices)
    {
        Result = ctlGetSet3DFeature(hDevices, &Get3DProperty);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlGetSet3DFeature(Get EnduranceGaming) returned failure code: 0x%X", Result);

            if (nullptr != Get3DProperty.pCustomValue)
            {
                free(Get3DProperty.pCustomValue);
                Get3DProperty.pCustomValue = nullptr;
            }

            return Result;
        }
        else
        {
            ctl_endurance_gaming_t *pGetEnduranceGaming = (ctl_endurance_gaming_t *)Get3DProperty.pCustomValue;
            APP_LOG_INFO("ctlGetSet3DFeature returned success(Get EnduranceGaming)");

            APP_LOG_INFO(" Get3DProperty.ApplicationName = %s", Get3DProperty.ApplicationName);
            APP_LOG_INFO(" pGetEnduranceGaming->EGControl = %d", pGetEnduranceGaming->EGControl);
            APP_LOG_INFO(" pGetEnduranceGaming->EGMode = %d", pGetEnduranceGaming->EGMode);
        }
    }

    if (nullptr != Get3DProperty.pCustomValue)
    {
        free(Get3DProperty.pCustomValue);
        Get3DProperty.pCustomValue = NULL;
    }

    PRINT_LOGS("======================Endurance Gaming test -> Global settings======================");

    Get3DProperty      = { 0 };
    Set3DProperty      = { 0 };
    SetEnduranceGaming = { 0 };

    Set3DProperty.Size = sizeof(Set3DProperty);
    Get3DProperty.Size = sizeof(Get3DProperty);

    Set3DProperty.FeatureType     = CTL_3D_FEATURE_ENDURANCE_GAMING;
    Set3DProperty.bSet            = TRUE;
    Set3DProperty.CustomValueSize = sizeof(ctl_endurance_gaming_t);
    SetEnduranceGaming.EGControl  = CTL_3D_ENDURANCE_GAMING_CONTROL_AUTO;
    SetEnduranceGaming.EGMode     = CTL_3D_ENDURANCE_GAMING_MODE_BETTER_PERFORMANCE;
    Set3DProperty.pCustomValue    = &SetEnduranceGaming;
    Set3DProperty.ValueType       = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
    Set3DProperty.Version         = 0;

    if (NULL != hDevices)
    {
        Result = ctlGetSet3DFeature(hDevices, &Set3DProperty);
        APP_LOG_INFO(" SetEnduranceGaming.EGControl = %d", SetEnduranceGaming.EGControl);
        APP_LOG_INFO(" SetEnduranceGaming.EGMode = %d", SetEnduranceGaming.EGMode);
        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlGetSet3DFeature(Set EnduranceGaming) returned failure code: 0x%X", Result);

            return Result;
        }
        else
        {
            APP_LOG_INFO("ctlGetSet3DFeature returned success(Set EnduranceGaming)");
        }
    }

    // Get Endurance gaming only
    Get3DProperty.FeatureType     = CTL_3D_FEATURE_ENDURANCE_GAMING;
    Get3DProperty.bSet            = FALSE;
    Get3DProperty.CustomValueSize = sizeof(ctl_endurance_gaming_t);
    Get3DProperty.pCustomValue    = (ctl_endurance_gaming_t *)malloc(sizeof(ctl_endurance_gaming_t));
    Get3DProperty.ValueType       = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
    Get3DProperty.Version         = 0;

    if (NULL != hDevices)
    {
        Result = ctlGetSet3DFeature(hDevices, &Get3DProperty);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlGetSet3DFeature(Get EnduranceGaming) returned failure code: 0x%X", Result);

            if (nullptr != Get3DProperty.pCustomValue)
            {
                free(Get3DProperty.pCustomValue);
                Get3DProperty.pCustomValue = nullptr;
            }

            return Result;
        }
        else
        {
            ctl_endurance_gaming_t *pGetEnduranceGamingGlobal = (ctl_endurance_gaming_t *)Get3DProperty.pCustomValue;
            APP_LOG_INFO("ctlGetSet3DFeature returned success(Get EnduranceGaming)");

            APP_LOG_INFO(" pGetEnduranceGamingGlobal->EGControl = %d", pGetEnduranceGamingGlobal->EGControl);
            APP_LOG_INFO(" pGetEnduranceGamingGlobal->EGMode = %d", pGetEnduranceGamingGlobal->EGMode);
        }
    }

    if (nullptr != Get3DProperty.pCustomValue)
    {
        free(Get3DProperty.pCustomValue);
        Get3DProperty.pCustomValue = NULL;
    }

    return Result;
}

/***************************************************************
 * @brief
 * Method to test Gaming flips
 * @param hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t CtlGamingFlipTest(ctl_device_adapter_handle_t hDevices)
{
    // Gaming flips Per APP GET/SET
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    PRINT_LOGS("======================Gaming flips test -> Per Application settings =====================");
    char *pAppName                        = "GTA5.exe";
    ctl_3d_feature_getset_t Get3DProperty = { 0 };
    ctl_3d_feature_getset_t Set3DProperty = { 0 };

    Set3DProperty.Size = sizeof(Set3DProperty);
    Get3DProperty.Size = sizeof(Get3DProperty);

    // Set Gaming flips modes only
    Set3DProperty.FeatureType               = CTL_3D_FEATURE_GAMING_FLIP_MODES;
    Set3DProperty.bSet                      = TRUE;
    Set3DProperty.CustomValueSize           = 0;
    Set3DProperty.pCustomValue              = NULL;
    Set3DProperty.ApplicationName           = pAppName;
    Set3DProperty.ApplicationNameLength     = (int8_t)strlen(pAppName);
    Set3DProperty.ValueType                 = CTL_PROPERTY_VALUE_TYPE_ENUM;
    Set3DProperty.Value.EnumType.EnableType = CTL_GAMING_FLIP_MODE_FLAG_SMOOTH_SYNC;
    Set3DProperty.Version                   = 0;

    if (NULL != hDevices)
    {
        Result = ctlGetSet3DFeature(hDevices, &Set3DProperty);
        APP_LOG_INFO(" Set3DProperty.Value.EnumType.EnableType = %d", Set3DProperty.Value.EnumType.EnableType);
        APP_LOG_INFO(" Set3DProperty.ApplicationName = %s", Set3DProperty.ApplicationName);
        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlGetSet3DFeature returned failure code: 0x%X", Result);

            return Result;
        }
        else
        {
            APP_LOG_INFO("ctlGetSet3DFeature returned success");
        }

        // Get Gaming flips modes only
        Get3DProperty.FeatureType           = CTL_3D_FEATURE_GAMING_FLIP_MODES;
        Get3DProperty.bSet                  = FALSE;
        Get3DProperty.CustomValueSize       = 0;
        Get3DProperty.pCustomValue          = NULL;
        Get3DProperty.ApplicationName       = pAppName;
        Get3DProperty.ApplicationNameLength = (int8_t)strlen(pAppName);
        Get3DProperty.ValueType             = CTL_PROPERTY_VALUE_TYPE_ENUM;
        Get3DProperty.Version               = 0;

        if (NULL != hDevices)
        {
            Result = ctlGetSet3DFeature(hDevices, &Get3DProperty);
            if (CTL_RESULT_SUCCESS != Result)
            {
                APP_LOG_ERROR("ctlGetSet3DFeature returned failure code: 0x%X", Result);

                return Result;
            }
            else
            {
                APP_LOG_INFO("ctlGetSet3DFeature returned success");
                APP_LOG_INFO(" Get3DProperty.Value.EnumType.EnableType = %d", Get3DProperty.Value.EnumType.EnableType);
                APP_LOG_INFO(" Get3DProperty.ApplicationName = %s", Get3DProperty.ApplicationName);
            }
        }
    }

    // Gaming flips Global GET/SET

    PRINT_LOGS("======================Gaming flips test -> Global settings======================");
    Get3DProperty = { 0 };
    Set3DProperty = { 0 };

    Set3DProperty.Size = sizeof(Set3DProperty);
    Get3DProperty.Size = sizeof(Get3DProperty);

    // Set Gaming flips modes only
    Set3DProperty.FeatureType               = CTL_3D_FEATURE_GAMING_FLIP_MODES;
    Set3DProperty.bSet                      = TRUE;
    Set3DProperty.CustomValueSize           = 0;
    Set3DProperty.pCustomValue              = NULL;
    Set3DProperty.ValueType                 = CTL_PROPERTY_VALUE_TYPE_ENUM;
    Set3DProperty.Value.EnumType.EnableType = CTL_GAMING_FLIP_MODE_FLAG_CAPPED_FPS;
    Set3DProperty.Version                   = 0;

    if (NULL != hDevices)
    {
        Result = ctlGetSet3DFeature(hDevices, &Set3DProperty);
        APP_LOG_INFO(" Set3DProperty.Value.EnumType.EnableType = %d", Set3DProperty.Value.EnumType.EnableType);
        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlGetSet3DFeature returned failure code: 0x%X", Result);

            return Result;
        }
        else
        {
            APP_LOG_INFO("ctlGetSet3DFeature returned success");
        }

        // Get Gaming flips modes only
        Get3DProperty.FeatureType     = CTL_3D_FEATURE_GAMING_FLIP_MODES;
        Get3DProperty.bSet            = FALSE;
        Get3DProperty.CustomValueSize = 0;
        Get3DProperty.pCustomValue    = NULL;
        Get3DProperty.ValueType       = CTL_PROPERTY_VALUE_TYPE_ENUM;
        Get3DProperty.Version         = 0;

        if (NULL != hDevices)
        {
            Result = ctlGetSet3DFeature(hDevices, &Get3DProperty);
            if (CTL_RESULT_SUCCESS != Result)
            {
                APP_LOG_ERROR("ctlGetSet3DFeature returned failure code: 0x%X", Result);

                return Result;
            }
            else
            {
                APP_LOG_INFO("ctlGetSet3DFeature returned success");
                APP_LOG_INFO(" Get3DProperty.Value.EnumType.EnableType = %d", Get3DProperty.Value.EnumType.EnableType);
            }
        }
    }

    return Result;
}

/***************************************************************
 * @brief
 * Method to get 3D feture capabilities
 * @param hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t CtlGet3DFeatureCaps(ctl_device_adapter_handle_t hDevices)
{
    ctl_result_t Result                 = CTL_RESULT_SUCCESS;
    ctl_3d_feature_caps_t FeatureCaps3D = { 0 };

    FeatureCaps3D.Size = sizeof(ctl_3d_feature_caps_t);
    if (NULL != hDevices)
    {
        Result = ctlGetSupported3DCapabilities(hDevices, &FeatureCaps3D);
        if (Result != CTL_RESULT_SUCCESS)
        {
            APP_LOG_ERROR("ctlGetSupported3DCapabilities returned failure code: 0x%X", Result);
            return Result;
        }
        else
        {
            APP_LOG_INFO("ctlGetSupported3DCapabilities returned success");
            FeatureCaps3D.pFeatureDetails = (ctl_3d_feature_details_t *)malloc(sizeof(ctl_3d_feature_details_t) * FeatureCaps3D.NumSupportedFeatures);
            if (FeatureCaps3D.pFeatureDetails == NULL)
            {
                return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
            }

            memset(FeatureCaps3D.pFeatureDetails, 0x0, sizeof(ctl_3d_feature_details_t) * FeatureCaps3D.NumSupportedFeatures);
            Result = ctlGetSupported3DCapabilities(hDevices, &FeatureCaps3D);
            if (Result == CTL_RESULT_SUCCESS)
            {
                for (uint32_t i = 0; i < FeatureCaps3D.NumSupportedFeatures; i++)
                {
                    if (FeatureCaps3D.pFeatureDetails[i].ValueType == CTL_PROPERTY_VALUE_TYPE_CUSTOM && FeatureCaps3D.pFeatureDetails[i].CustomValueSize > 0 &&
                        FeatureCaps3D.pFeatureDetails[i].pCustomValue == NULL)
                    {
                        FeatureCaps3D.pFeatureDetails[i].pCustomValue = malloc(FeatureCaps3D.pFeatureDetails[i].CustomValueSize);

                        // fill up a single caps struct call to get the capability of a custom feature
                        ctl_3d_feature_caps_t Temp = { 0 };
                        Temp.NumSupportedFeatures  = 1;
                        Temp.pFeatureDetails       = &FeatureCaps3D.pFeatureDetails[i];
                        Temp.Size                  = sizeof(ctl_3d_feature_caps_t);
                        Temp.Version               = 0;
                        Result                     = ctlGetSupported3DCapabilities(hDevices, &Temp);
                    }
                    if (Result == CTL_RESULT_SUCCESS)
                        Print3DFeatureDetail(&FeatureCaps3D.pFeatureDetails[i]);

                    // free up the additional custom block which got allocated
                    if (FeatureCaps3D.pFeatureDetails[i].pCustomValue)
                    {
                        free(FeatureCaps3D.pFeatureDetails[i].pCustomValue);
                        FeatureCaps3D.pFeatureDetails[i].pCustomValue = NULL;
                    }
                }
            }
            else
            {
                APP_LOG_ERROR("ctlGetSupported3DCapabilities returned failure code when called with NumSupportedFeatures set: 0x%X", Result);
                if (nullptr != FeatureCaps3D.pFeatureDetails)
                {
                    free(FeatureCaps3D.pFeatureDetails);
                    FeatureCaps3D.pFeatureDetails = nullptr;
                }
                return Result;
            }

            free(FeatureCaps3D.pFeatureDetails);
            FeatureCaps3D.pFeatureDetails = nullptr;
        }
    }

    return Result;
}

bool QuiteEventThread = false;
DWORD WINAPI CtlEventThread(LPVOID ThreadParameterPtr)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    ctl_wait_property_change_args_t Args = { 0 };

    APP_LOG_INFO("CtlEventThread: Entering thread");

    Args.PropertyType    = CTL_PROPERTY_TYPE_FLAG_MEDIA | CTL_PROPERTY_TYPE_FLAG_DISPLAY | CTL_PROPERTY_TYPE_FLAG_3D;
    Args.Size            = sizeof(Args);
    Args.TimeOutMilliSec = 500; // 0.5sec

    APP_LOG_INFO("CtlEventThread: Calling ctlWaitForPropertyChange() and getting to blocked state...");

    do
    {
        Result = ctlWaitForPropertyChange((ctl_device_adapter_handle_t)ThreadParameterPtr, &Args); // blocking call
        if (CTL_RESULT_ERROR_WAIT_TIMEOUT == Result)
            continue;
    } while (false == QuiteEventThread);

    APP_LOG_INFO("CtlEventThread: ctlWaitForPropertyChange() unblocked return value = %d", Result);
    APP_LOG_INFO("CtlEventThread: Exiting thread");

    return Result;
}

/***************************************************************
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
ctl_result_t CtlTestEvents(ctl_device_adapter_handle_t hAdapter)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;
    bool Listener       = false;
    bool SpawnThread    = false;
    DWORD ThreadID      = 0;
    HANDLE ThreadHandle = NULL;

    PRINT_LOGS("Do you want to listen for events (L), listen via a thread (T), quit (Q) or do series of set calls (any other key)? ");
    int Key = _getch();
    if ((Key == 'L') || (Key == 'l'))
    {
        Listener = true;
    }
    else if ((Key == 'T') || (Key == 't'))
    {
        SpawnThread = true;
        Listener    = true;
    }
    else if ((Key == 'Q') || (Key == 'q'))
        return Result;

    do
    {
        if (Listener)
        {
            if (SpawnThread)
            {
                ThreadHandle = CreateThread(NULL, 0, CtlEventThread, hAdapter, 0, &ThreadID);

                if (NULL == ThreadHandle)
                {
                    APP_LOG_ERROR("CreateThread failed!");
                    Result = CTL_RESULT_ERROR_OS_CALL;
                    break;
                }
                else
                {
                    APP_LOG_INFO("Created thread ID = 0x%X", ThreadID);
                }
            }
            else
            {
                ctl_wait_property_change_args_t Args = { 0 };

                Args.PropertyType    = CTL_PROPERTY_TYPE_FLAG_MEDIA | CTL_PROPERTY_TYPE_FLAG_DISPLAY | CTL_PROPERTY_TYPE_FLAG_3D;
                Args.Size            = sizeof(Args);
                Args.TimeOutMilliSec = 0xFFFFFFFF; // INFINITE;

                APP_LOG_INFO("Calling ctlWaitForPropertyChange() and getting to blocked state...");

                Result = ctlWaitForPropertyChange(hAdapter, &Args); // blocking call

                APP_LOG_INFO("ctlWaitForPropertyChange() unblocked return value = %d", Result);
            }
        }
        else
        {
            APP_LOG_INFO("Trying a 3D set call & restoring it (overrall 2 set calls)...");

            // try setting various parameters, or just the app once more
            ctl_3d_feature_getset_t Feature3D         = { 0 };
            ctl_endurance_gaming_t SetEnduranceGaming = { 0 };
            Feature3D.bSet                            = true;
            Feature3D.FeatureType                     = CTL_3D_FEATURE_ENDURANCE_GAMING;
            Feature3D.Size                            = sizeof(Feature3D);
            Feature3D.ValueType                       = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
            Feature3D.CustomValueSize                 = sizeof(ctl_endurance_gaming_t);
            SetEnduranceGaming.EGControl              = CTL_3D_ENDURANCE_GAMING_CONTROL_TURN_ON;
            SetEnduranceGaming.EGMode                 = CTL_3D_ENDURANCE_GAMING_MODE_BETTER_PERFORMANCE;
            Feature3D.pCustomValue                    = &SetEnduranceGaming;

            Result = ctlGetSet3DFeature(hAdapter, &Feature3D);

            if (Result == CTL_RESULT_SUCCESS)
            {
                ctl_3d_feature_getset_t Feature3D         = { 0 };
                ctl_endurance_gaming_t SetEnduranceGaming = { 0 };
                Feature3D.bSet                            = true;
                Feature3D.FeatureType                     = CTL_3D_FEATURE_ENDURANCE_GAMING;
                Feature3D.Size                            = sizeof(Feature3D);
                Feature3D.ValueType                       = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
                Feature3D.CustomValueSize                 = sizeof(ctl_endurance_gaming_t);
                SetEnduranceGaming.EGControl              = CTL_3D_ENDURANCE_GAMING_CONTROL_TURN_OFF;
                SetEnduranceGaming.EGMode                 = CTL_3D_ENDURANCE_GAMING_MODE_MAX;
                Feature3D.pCustomValue                    = &SetEnduranceGaming;

                Result = ctlGetSet3DFeature(hAdapter, &Feature3D);
            }
        }

        if (SpawnThread)
        {
            PRINT_LOGS("Press any key to signal thread to exit...");
            int Key          = _getch();
            QuiteEventThread = true;
            break; // exit from loop
        }
        else
        {
            PRINT_LOGS("Quit(q) or continue (any other key)? ");
            int Key = _getch();
            if ((Key == 'Q') || (Key == 'q'))
            {
                QuiteEventThread = true;
                break; // exit from loop
            }
        }
    } while (true);

    if (SpawnThread && ThreadHandle)
    {
        // wait for thread to terminate?
        WaitForSingleObject(ThreadHandle, INFINITE);
    }

    free(ThreadHandle);
    return Result;
}

/***************************************************************
 * @brief
 * Generic method to get & print app profile
 * @param hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t CtlGetGamingAppProfile(ctl_device_adapter_handle_t hDevices, ctl_3d_app_profiles_t &AppProfileGet, char *ApplicationName = NULL)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    // Get App Profile
    ctl_3d_feature_getset_t Get3DProperty = { 0 };
    Get3DProperty.Size                    = sizeof(Get3DProperty);
    Get3DProperty.FeatureType             = CTL_3D_FEATURE_APP_PROFILES;
    Get3DProperty.bSet                    = FALSE;
    Get3DProperty.CustomValueSize         = sizeof(ctl_3d_app_profiles_t);
    Get3DProperty.pCustomValue            = &AppProfileGet;
    Get3DProperty.ValueType               = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
    Get3DProperty.Version                 = 0;
    Get3DProperty.ApplicationName         = ApplicationName;
    Get3DProperty.ApplicationNameLength   = (ApplicationName != NULL) ? (int8_t)strlen(ApplicationName) : 0;

    if (NULL != hDevices)
    {
        APP_LOG_INFO("Doing a GetAILProfile(%s, %s)", ApplicationName ? ApplicationName : "Global", AppProfileGet.TierType == CTL_3D_TIER_TYPE_FLAG_COMPATIBILITY ? "Compatibility" : "Performance");

        Result = ctlGetSet3DFeature(hDevices, &Get3DProperty);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlGetSet3DFeature(Get AppProfile) returned failure code: 0x%X", Result);
            if (Result == CTL_RESULT_ERROR_DATA_NOT_FOUND)
            {
                APP_LOG_ERROR(" Reason: Value not found");
                APP_LOG_INFO(" Use DefaultEnabledTierProfiles (=%s) for UI default selection if required", GetProfileTierName(AppProfileGet.DefaultEnabledTierProfiles, TierName, 100));
            }
        }
        else if (Get3DProperty.CustomValueSize > 0)
        {
            APP_LOG_INFO("ctlGetSet3DFeature returned success(Get AppProfile)");
            APP_LOG_INFO(" App name = %s", ApplicationName == NULL ? "Global" : ApplicationName);
            APP_LOG_INFO(" AppProfileGet.TierType = %s", GetProfileTypeName(AppProfileGet.TierType));
            APP_LOG_INFO(" AppProfileGet.EnabledTierProfiles = %s", GetProfileTierName(AppProfileGet.EnabledTierProfiles, TierName, 100));
            APP_LOG_INFO(" AppProfileGet.DefaultEnabledTierProfiles = %s", GetProfileTierName(AppProfileGet.DefaultEnabledTierProfiles, TierName, 100));
            APP_LOG_INFO(" AppProfileGet.CustomizationSupportedTierProfiles = %s", GetProfileTierName(AppProfileGet.CustomizationSupportedTierProfiles, TierName, 100));
            APP_LOG_INFO(" AppProfileGet.CustomizationEnabledTierProfiles = %s", GetProfileTierName(AppProfileGet.CustomizationEnabledTierProfiles, TierName, 100));
        }
    }

    return Result;
}

/***************************************************************
 * @brief
 * Generic method to set & print app profile
 * @param hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t CtlSetGamingAppProfile(ctl_device_adapter_handle_t hDevices, ctl_3d_app_profiles_t &AppProfileSet, char *ApplicationName = NULL)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    // Get App Profile
    ctl_3d_feature_getset_t Set3DProperty = { 0 };
    Set3DProperty.Size                    = sizeof(Set3DProperty);
    Set3DProperty.FeatureType             = CTL_3D_FEATURE_APP_PROFILES;
    Set3DProperty.bSet                    = TRUE;
    Set3DProperty.CustomValueSize         = sizeof(ctl_3d_app_profiles_t);
    Set3DProperty.pCustomValue            = &AppProfileSet;
    Set3DProperty.ValueType               = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
    Set3DProperty.Version                 = 0;
    Set3DProperty.ApplicationName         = ApplicationName;
    Set3DProperty.ApplicationNameLength   = (ApplicationName != NULL) ? (int8_t)strlen(ApplicationName) : 0;

    if (NULL != hDevices)
    {
        APP_LOG_INFO("Doing a SetAILProfile(%s, %s)", ApplicationName ? ApplicationName : "Global", AppProfileSet.TierType == CTL_3D_TIER_TYPE_FLAG_COMPATIBILITY ? "Compatibility" : "Performance");

        Result = ctlGetSet3DFeature(hDevices, &Set3DProperty);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlGetSet3DFeature(Set AppProfile) returned failure code: 0x%X", Result);
            if (CTL_RESULT_ERROR_DATA_NOT_FOUND == Result)
                APP_LOG_ERROR("Reason: Value not found");
            return Result;
        }
        else if (Set3DProperty.CustomValueSize > 0)
        {
            APP_LOG_INFO("ctlGetSet3DFeature returned success(Set AppProfile)");
            APP_LOG_INFO(" App name = %s", ApplicationName == NULL ? "Global" : ApplicationName);
            APP_LOG_INFO(" AppProfileSet.TierType = %s", GetProfileTypeName(AppProfileSet.TierType));
            APP_LOG_INFO(" AppProfileSet.EnabledTierProfiles = %s", GetProfileTierName(AppProfileSet.EnabledTierProfiles, TierName, 100));
            APP_LOG_INFO(" AppProfileSet.CustomizationSupportedTierProfiles = %s", GetProfileTierName(AppProfileSet.CustomizationSupportedTierProfiles, TierName, 100));
        }
    }

    return Result;
}

/***************************************************************
 * @brief
 * Method to test Endurance Gaming
 * @param hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t CtlGamingAppProfile(ctl_device_adapter_handle_t hDevices)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    PRINT_LOGS("======================Gaming App Profile test======================");

    ctl_3d_app_profiles_t AppProfile = { 0 };

    // get profile for some xyz.exe
    AppProfile.TierType = CTL_3D_TIER_TYPE_FLAG_COMPATIBILITY;
    CtlGetGamingAppProfile(hDevices, AppProfile, "xyz.exe");

    // Set global app profile - might fail if app doesn't have the required privilages
    AppProfile.TierType            = CTL_3D_TIER_TYPE_FLAG_COMPATIBILITY;
    AppProfile.EnabledTierProfiles = CTL_3D_TIER_PROFILE_FLAG_TIER_1;
    CtlSetGamingAppProfile(hDevices, AppProfile);

    // Get global App Profile
    AppProfile.TierType = CTL_3D_TIER_TYPE_FLAG_COMPATIBILITY;
    CtlGetGamingAppProfile(hDevices, AppProfile);

    // Get global App Profile
    AppProfile.TierType = CTL_3D_TIER_TYPE_FLAG_PERFORMANCE;
    CtlGetGamingAppProfile(hDevices, AppProfile);

    return Result;
}

/***************************************************************
 * @brief
 * Method to test Endurance Gaming
 * @param hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t CtlCMAAGamingFeatureTest(ctl_device_adapter_handle_t hDevices)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if (NULL == hDevices)
        return CTL_RESULT_ERROR_INVALID_NULL_HANDLE;

    PRINT_LOGS("======================CtlCMAAGamingFeatureTest======================");

    ctl_3d_feature_getset_t Get3DProperty = { 0 };
    ctl_3d_feature_getset_t Set3DProperty = { 0 };
    uint32_t InitialValue                 = 0;

    Set3DProperty.Size = sizeof(Set3DProperty);
    Get3DProperty.Size = sizeof(Get3DProperty);

    // Get feature - global
    Get3DProperty.FeatureType = CTL_3D_FEATURE_CMAA;
    Get3DProperty.bSet        = FALSE;
    Get3DProperty.ValueType   = CTL_PROPERTY_VALUE_TYPE_ENUM;
    Get3DProperty.Version     = 0;
    Result                    = ctlGetSet3DFeature(hDevices, &Get3DProperty);
    APP_LOG_INFO(" Get3DProperty.Value.EnumType.EnableType = %d", Get3DProperty.Value.EnumType.EnableType);
    if (CTL_RESULT_SUCCESS != Result)
    {
        APP_LOG_ERROR("Returned failure code: 0x%X", Result);
        if (CTL_RESULT_ERROR_DATA_NOT_FOUND != Result) // if value not present, it's expected, so continue with set/get verification
            return Result;
    }
    else
    {
        APP_LOG_INFO("ctlGetSet3DFeature returned success");
        InitialValue = Get3DProperty.Value.EnumType.EnableType;
    }

    // Set feature - global
    Set3DProperty.FeatureType               = CTL_3D_FEATURE_CMAA;
    Set3DProperty.bSet                      = TRUE;
    Set3DProperty.ValueType                 = CTL_PROPERTY_VALUE_TYPE_ENUM;
    Set3DProperty.Version                   = 0;
    Set3DProperty.Value.EnumType.EnableType = CTL_3D_CMAA_TYPES_OVERRIDE_MSAA;
    Result                                  = ctlGetSet3DFeature(hDevices, &Set3DProperty);
    APP_LOG_INFO(" Set3DProperty.Value.EnumType.EnableType = CTL_3D_CMAA_TYPES_OVERRIDE_MSAA");
    if (CTL_RESULT_SUCCESS != Result)
    {
        APP_LOG_ERROR("Returned failure code: 0x%X", Result);
        return Result;
    }
    else
    {
        APP_LOG_INFO("ctlGetSet3DFeature returned success");
    }

    // Get feature back & check - global
    Get3DProperty.FeatureType = CTL_3D_FEATURE_CMAA;
    Get3DProperty.bSet        = FALSE;
    Get3DProperty.ValueType   = CTL_PROPERTY_VALUE_TYPE_ENUM;
    Get3DProperty.Version     = 0;
    Result                    = ctlGetSet3DFeature(hDevices, &Get3DProperty);
    APP_LOG_INFO(" Get3DProperty.Value.EnumType.EnableType = %d", Get3DProperty.Value.EnumType.EnableType);
    if (CTL_RESULT_SUCCESS != Result)
    {
        APP_LOG_ERROR("Returned failure code: 0x%X", Result);
        return Result;
    }
    else
    {
        APP_LOG_INFO("ctlGetSet3DFeature returned success");
        if (Get3DProperty.Value.EnumType.EnableType != Set3DProperty.Value.EnumType.EnableType)
        {
            APP_LOG_ERROR("ERROR: Value didn't get set!");
            return CTL_RESULT_ERROR_UNKNOWN;
        }
    }

    // set the initial value back
    Set3DProperty.FeatureType               = CTL_3D_FEATURE_CMAA;
    Set3DProperty.bSet                      = TRUE;
    Set3DProperty.ValueType                 = CTL_PROPERTY_VALUE_TYPE_ENUM;
    Set3DProperty.Version                   = 0;
    Set3DProperty.Value.EnumType.EnableType = InitialValue;
    Result                                  = ctlGetSet3DFeature(hDevices, &Set3DProperty);
    APP_LOG_INFO(" Set3DProperty.Value.EnumType.EnableType = %d", Set3DProperty.Value.EnumType.EnableType);
    if (CTL_RESULT_SUCCESS != Result)
    {
        APP_LOG_ERROR("Returned failure code: 0x%X", Result);
        return Result;
    }
    else
    {
        APP_LOG_INFO("ctlGetSet3DFeature returned success");
    }

    return Result;
}

/***************************************************************
 * @brief
 * Method to test Endurance Gaming
 * @param hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t CtlGlobalOrPerAppTest(ctl_device_adapter_handle_t hDevices)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;
    char *pAppName      = "GTA5.exe";

    PRINT_LOGS("======================Global Or Per App test======================");

    ctl_3d_feature_getset_t Get3DProperty = { 0 };
    ctl_3d_feature_getset_t Set3DProperty = { 0 };

    Set3DProperty.Size = sizeof(Set3DProperty);
    Get3DProperty.Size = sizeof(Get3DProperty);

    // Set global or per app only
    Set3DProperty.FeatureType               = CTL_3D_FEATURE_GLOBAL_OR_PER_APP;
    Set3DProperty.bSet                      = TRUE;
    Set3DProperty.CustomValueSize           = 0;
    Set3DProperty.pCustomValue              = NULL;
    Set3DProperty.ApplicationName           = pAppName;
    Set3DProperty.ApplicationNameLength     = (int8_t)strlen(pAppName);
    Set3DProperty.ValueType                 = CTL_PROPERTY_VALUE_TYPE_ENUM;
    Set3DProperty.Value.EnumType.EnableType = CTL_3D_GLOBAL_OR_PER_APP_TYPES_GLOBAL;
    Set3DProperty.Version                   = 0;

    if (NULL != hDevices)
    {
        Result = ctlGetSet3DFeature(hDevices, &Set3DProperty);
        APP_LOG_INFO(" Set3DProperty.ApplicationName = %s", Set3DProperty.ApplicationName);
        APP_LOG_INFO(" Set3DProperty.Value.EnumType.EnableType = %d", Set3DProperty.Value.EnumType.EnableType);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlGetSet3DFeature(Set Global Over Per App Setting (Set call)) returned failure code: 0x%X", Result);

            return Result;
        }
        else
        {
            APP_LOG_INFO("ctlGetSet3DFeature returned success(Set Global Over Per App Setting (Set call))");
        }
    }

    // Get global or per app only
    Get3DProperty.FeatureType           = CTL_3D_FEATURE_GLOBAL_OR_PER_APP;
    Get3DProperty.bSet                  = FALSE;
    Get3DProperty.CustomValueSize       = 0;
    Get3DProperty.pCustomValue          = NULL;
    Get3DProperty.ApplicationName       = pAppName;
    Get3DProperty.ApplicationNameLength = (int8_t)strlen(pAppName);
    Get3DProperty.ValueType             = CTL_PROPERTY_VALUE_TYPE_ENUM;
    Get3DProperty.Version               = 0;

    if (NULL != hDevices)
    {
        Result = ctlGetSet3DFeature(hDevices, &Get3DProperty);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlGetSet3DFeature(Set Global Over Per App Setting (Get call)) returned failure code: 0x%X", Result);

            return Result;
        }
        else
        {
            APP_LOG_INFO("ctlGetSet3DFeature returned success(Set Global Over Per App Setting (Get call))");
            APP_LOG_INFO(" Get3DProperty.ApplicationName = %s", Get3DProperty.ApplicationName);
            APP_LOG_INFO(" Get3DProperty.Value.EnumType.EnableTypee = %d", Get3DProperty.Value.EnumType.EnableType);
        }
    }

    return Result;
}


/***************************************************************
 * @brief
 * Method to test Frame Limit
 * @param hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t CtlTestFrameLimit(ctl_device_adapter_handle_t hDevices)
{
    // Frame Limit Per APP GET/SET
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    printf("======================Frame Limit test -> Per Application settings =====================\n");
    char *pAppName                        = "GTA5.exe";
    ctl_3d_feature_getset_t Get3DProperty = { 0 };
    ctl_3d_feature_getset_t Set3DProperty = { 0 };

    Set3DProperty.Size = sizeof(Set3DProperty);
    Get3DProperty.Size = sizeof(Get3DProperty);

    // Set Frame Limit
    Set3DProperty.FeatureType           = CTL_3D_FEATURE_FRAME_LIMIT;
    Set3DProperty.bSet                  = TRUE;
    Set3DProperty.CustomValueSize       = 0;
    Set3DProperty.pCustomValue          = NULL;
    Set3DProperty.ApplicationName       = pAppName;
    Set3DProperty.ApplicationNameLength = (int8_t)strlen(pAppName);
    Set3DProperty.ValueType             = CTL_PROPERTY_VALUE_TYPE_INT32;
    Set3DProperty.Value.IntType.Enable  = true;
    Set3DProperty.Value.IntType.Value   = 30;
    Set3DProperty.Version               = 0;

    if (NULL != hDevices)
    {
        Result = ctlGetSet3DFeature(hDevices, &Set3DProperty);
        printf(" Set3DProperty.Value.IntType.Enable = %d\n", Set3DProperty.Value.IntType.Enable);
        printf(" Set3DProperty.Value.IntType.Value = %d\n", Set3DProperty.Value.IntType.Value);
        printf(" Set3DProperty.ApplicationName = %s\n", Set3DProperty.ApplicationName);
        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlGetSet3DFeature returned failure code: 0x%X\n", Result);

            return Result;
        }
        else
        {
            printf("ctlGetSet3DFeature returned success\n");
        }

        // Get Frame Limit
        Get3DProperty.FeatureType           = CTL_3D_FEATURE_FRAME_LIMIT;
        Get3DProperty.bSet                  = FALSE;
        Get3DProperty.CustomValueSize       = 0;
        Get3DProperty.pCustomValue          = NULL;
        Get3DProperty.ApplicationName       = pAppName;
        Get3DProperty.ApplicationNameLength = (int8_t)strlen(pAppName);
        Get3DProperty.ValueType             = CTL_PROPERTY_VALUE_TYPE_INT32;
        Get3DProperty.Version               = 0;

        if (NULL != hDevices)
        {
            Result = ctlGetSet3DFeature(hDevices, &Get3DProperty);

            if (CTL_RESULT_SUCCESS != Result)
            {
                printf("ctlGetSet3DFeature returned failure code: 0x%X\n", Result);

                return Result;
            }
            else
            {
                printf("ctlGetSet3DFeature returned success\n");
                printf(" Get3DProperty.Value.IntType.Enable = %d\n", Get3DProperty.Value.IntType.Enable);
                printf(" Get3DProperty.Value.IntType.Value = %d\n", Get3DProperty.Value.IntType.Value);
                printf(" Get3DProperty.ApplicationName = %s\n", Get3DProperty.ApplicationName);
            }
        }
    }

    // Frame Limit Global GET/SET

    printf("======================Frame Limit test -> Global settings======================\n");
    Get3DProperty = { 0 };
    Set3DProperty = { 0 };

    Set3DProperty.Size = sizeof(Set3DProperty);
    Get3DProperty.Size = sizeof(Get3DProperty);

    // Set Frame Limit
    Set3DProperty.FeatureType          = CTL_3D_FEATURE_FRAME_LIMIT;
    Set3DProperty.bSet                 = TRUE;
    Set3DProperty.CustomValueSize      = 0;
    Set3DProperty.pCustomValue         = NULL;
    Set3DProperty.ValueType            = CTL_PROPERTY_VALUE_TYPE_INT32;
    Set3DProperty.Value.IntType.Enable = true;
    Set3DProperty.Value.IntType.Value  = 60;
    Set3DProperty.Version              = 0;

    if (NULL != hDevices)
    {
        Result = ctlGetSet3DFeature(hDevices, &Set3DProperty);
        printf(" Set3DProperty.Value.IntType.Enable = %d\n", Set3DProperty.Value.IntType.Enable);
        printf(" Set3DProperty.Value.IntType.Value = %d\n", Set3DProperty.Value.IntType.Value);
        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlGetSet3DFeature returned failure code: 0x%X\n", Result);

            return Result;
        }
        else
        {
            printf("ctlGetSet3DFeature returned success\n");
        }

        // Get Frame Limit
        Get3DProperty.FeatureType     = CTL_3D_FEATURE_FRAME_LIMIT;
        Get3DProperty.bSet            = FALSE;
        Get3DProperty.CustomValueSize = 0;
        Get3DProperty.pCustomValue    = NULL;
        Get3DProperty.ValueType       = CTL_PROPERTY_VALUE_TYPE_INT32;
        Get3DProperty.Version         = 0;

        if (NULL != hDevices)
        {
            Result = ctlGetSet3DFeature(hDevices, &Get3DProperty);
            if (CTL_RESULT_SUCCESS != Result)
            {
                printf("ctlGetSet3DFeature returned failure code: 0x%X\n", Result);

                return Result;
            }
            else
            {
                printf("ctlGetSet3DFeature returned success\n");
                printf(" Get3DProperty.Value.IntType.Enable = %d\n", Get3DProperty.Value.IntType.Enable);
                printf(" Get3DProperty.Value.IntType.Value = %d\n", Get3DProperty.Value.IntType.Value);
            }
        }
    }

    return Result;
}

/***************************************************************
 * @brief
 * Method to test Low Latency
 * @param hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t CtlTestLowLatency(ctl_device_adapter_handle_t hDevices)
{
    // Low Latency Per APP GET/SET
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    printf("======================Low Latency test -> Per Application settings =====================\n");
    char *pAppName                        = "GTA5.exe";
    ctl_3d_feature_getset_t Get3DProperty = { 0 };
    ctl_3d_feature_getset_t Set3DProperty = { 0 };

    Set3DProperty.Size = sizeof(Set3DProperty);
    Get3DProperty.Size = sizeof(Get3DProperty);

    // Set Low Latency mode
    Set3DProperty.FeatureType               = CTL_3D_FEATURE_LOW_LATENCY;
    Set3DProperty.bSet                      = TRUE;
    Set3DProperty.CustomValueSize           = 0;
    Set3DProperty.pCustomValue              = NULL;
    Set3DProperty.ApplicationName           = pAppName;
    Set3DProperty.ApplicationNameLength     = (int8_t)strlen(pAppName);
    Set3DProperty.ValueType                 = CTL_PROPERTY_VALUE_TYPE_ENUM;
    Set3DProperty.Value.EnumType.EnableType = CTL_3D_LOW_LATENCY_TYPES_TURN_ON_BOOST_MODE_ON;
    Set3DProperty.Version                   = 0;

    if (NULL != hDevices)
    {
        Result = ctlGetSet3DFeature(hDevices, &Set3DProperty);
        printf(" Set3DProperty.Value.EnumType.EnableType = %d\n", Set3DProperty.Value.EnumType.EnableType);
        printf(" Set3DProperty.ApplicationName = %s\n", Set3DProperty.ApplicationName);
        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlGetSet3DFeature returned failure code: 0x%X\n", Result);

            return Result;
        }
        else
        {
            printf("ctlGetSet3DFeature returned success\n");
        }

        // Get Low Latency mode
        Get3DProperty.FeatureType           = CTL_3D_FEATURE_LOW_LATENCY;
        Get3DProperty.bSet                  = FALSE;
        Get3DProperty.CustomValueSize       = 0;
        Get3DProperty.pCustomValue          = NULL;
        Get3DProperty.ApplicationName       = pAppName;
        Get3DProperty.ApplicationNameLength = (int8_t)strlen(pAppName);
        Get3DProperty.ValueType             = CTL_PROPERTY_VALUE_TYPE_ENUM;
        Get3DProperty.Version               = 0;

        if (NULL != hDevices)
        {
            Result = ctlGetSet3DFeature(hDevices, &Get3DProperty);

            if (CTL_RESULT_SUCCESS != Result)
            {
                printf("ctlGetSet3DFeature returned failure code: 0x%X\n", Result);

                return Result;
            }
            else
            {
                printf("ctlGetSet3DFeature returned success\n");
                printf(" Get3DProperty.Value.EnumType.EnableType = %d\n", Get3DProperty.Value.EnumType.EnableType);
                printf(" Get3DProperty.ApplicationName = %s\n", Get3DProperty.ApplicationName);
            }
        }
    }

    // Low Latency Global GET/SET

    printf("======================Low Latency test -> Global settings======================\n");
    Get3DProperty = { 0 };
    Set3DProperty = { 0 };

    Set3DProperty.Size = sizeof(Set3DProperty);
    Get3DProperty.Size = sizeof(Get3DProperty);

    // Set Low Latency mode
    Set3DProperty.FeatureType               = CTL_3D_FEATURE_LOW_LATENCY;
    Set3DProperty.bSet                      = TRUE;
    Set3DProperty.CustomValueSize           = 0;
    Set3DProperty.pCustomValue              = NULL;
    Set3DProperty.ValueType                 = CTL_PROPERTY_VALUE_TYPE_ENUM;
    Set3DProperty.Value.EnumType.EnableType = CTL_3D_LOW_LATENCY_TYPES_TURN_ON;
    Set3DProperty.Version                   = 0;

    if (NULL != hDevices)
    {
        Result = ctlGetSet3DFeature(hDevices, &Set3DProperty);
        printf(" Set3DProperty.Value.EnumType.EnableType = %d\n", Set3DProperty.Value.EnumType.EnableType);
        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlGetSet3DFeature returned failure code: 0x%X\n", Result);

            return Result;
        }
        else
        {
            printf("ctlGetSet3DFeature returned success\n");
        }

        // Get Low Latency mode
        Get3DProperty.FeatureType     = CTL_3D_FEATURE_LOW_LATENCY;
        Get3DProperty.bSet            = FALSE;
        Get3DProperty.CustomValueSize = 0;
        Get3DProperty.pCustomValue    = NULL;
        Get3DProperty.ValueType       = CTL_PROPERTY_VALUE_TYPE_ENUM;
        Get3DProperty.Version         = 0;

        if (NULL != hDevices)
        {
            Result = ctlGetSet3DFeature(hDevices, &Get3DProperty);
            if (CTL_RESULT_SUCCESS != Result)
            {
                printf("ctlGetSet3DFeature returned failure code: 0x%X\n", Result);

                return Result;
            }
            else
            {
                printf("ctlGetSet3DFeature returned success\n");
                printf(" Get3DProperty.Value.EnumType.EnableType = %d\n", Get3DProperty.Value.EnumType.EnableType);
            }
        }
    }

    return Result;
}

int main()
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    ctl_device_adapter_handle_t *hDevices                   = nullptr;
    ctl_device_adapter_properties_t DeviceAdapterProperties = { 0 };
    // Get a handle to the DLL module.
    uint32_t AdapterCount = 0;
    uint32_t Index        = 0;
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

    if (CTL_RESULT_SUCCESS == Result)
    {
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

        if (CTL_RESULT_SUCCESS == Result)
        {
            hDevices = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * AdapterCount);
            if (nullptr == hDevices)
            {
                return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
            }

            try
            {
                Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);
                LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");
            }
            catch (const std::bad_array_new_length &e)
            {
                APP_LOG_ERROR("%s ", e.what());
            }
        }
        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("ctlEnumerateDevices returned failure code: 0x%X", Result);
            goto Exit;
        }

        for (Index = 0; Index < AdapterCount; Index++)
        {
            if (CTL_RESULT_SUCCESS == Result)
            {

                try
                {
                    Result = CtlGet3DFeatureCaps(hDevices[Index]);
                }
                catch (const std::bad_array_new_length &e)
                {
                    APP_LOG_ERROR("%s ", e.what());
                }

                if (CTL_RESULT_SUCCESS != Result)
                {
                    APP_LOG_ERROR("CtlGet3DCaps returned failure code: 0x%X", Result);
                }
                STORE_RESET_ERROR(Result);

                Result = CtlEnduranceGamingTest(hDevices[Index]);

                if (CTL_RESULT_SUCCESS != Result)
                {
                    APP_LOG_ERROR("CtlEnduranceGamingTest failure code: 0x%X", Result);
                }
                STORE_RESET_ERROR(Result);

                Result = CtlGamingFlipTest(hDevices[Index]);

                if (CTL_RESULT_SUCCESS != Result)
                {
                    APP_LOG_ERROR("CtlGamingFlipTest failure code: 0x%X", Result);
                }
                STORE_RESET_ERROR(Result);

                Result = CtlGamingAppProfile(hDevices[Index]);
                if (CTL_RESULT_SUCCESS != Result)
                {
                    APP_LOG_ERROR("CtlGamingAppProfile failure code: 0x%X", Result);
                }
                STORE_RESET_ERROR(Result);

                Result = CtlCMAAGamingFeatureTest(hDevices[Index]);
                if (CTL_RESULT_SUCCESS != Result)
                {
                    APP_LOG_ERROR("CtlCMAAGamingFeatureTest failure code: 0x%X", Result);
                }
                STORE_RESET_ERROR(Result);

                Result = CtlGlobalOrPerAppTest(hDevices[Index]);
                if (CTL_RESULT_SUCCESS != Result)
                {
                    APP_LOG_ERROR("CtlGlobalOrPerAppTest failure code: 0x%X", Result);
                }
                STORE_RESET_ERROR(Result);

                Result = CtlTestFrameLimit(hDevices[Index]);
                if (CTL_RESULT_SUCCESS != Result)
                {
                    APP_LOG_ERROR("CtlTestFrameLimit failure code: 0x%X", Result);
                }
                STORE_RESET_ERROR(Result);

                Result = CtlTestLowLatency(hDevices[Index]);
                if (CTL_RESULT_SUCCESS != Result)
                {
                    APP_LOG_ERROR("CtlTestLowLatency failure code: 0x%X", Result);
                }
                STORE_RESET_ERROR(Result);

                // Test eventing
                CtlTestEvents(hDevices[0]); // for now just first adapter!
            }
        }
    }
    else
    {
        STORE_RESET_ERROR(Result);
    }
Exit:

    ctlClose(hAPIHandle);

    if (hDevices != nullptr)
    {
        free(hDevices);
        hDevices = nullptr;
    }

    APP_LOG_INFO("Overrall test result is 0x%X", GResult);

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
