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

    printf("======================Endurance Gaming test -> Per Application settings======================\n");

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
        printf(" Set3DProperty.ApplicationName = %s\n", Set3DProperty.ApplicationName);
        printf(" SetEnduranceGaming.EGControl = %d\n", SetEnduranceGaming.EGControl);
        printf(" SetEnduranceGaming.EGMode = %d\n", SetEnduranceGaming.EGMode);
        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlGetSet3DFeature(Set EnduranceGaming) returned failure code: 0x%X\n", Result);

            return Result;
        }
        else
        {
            printf("ctlGetSet3DFeature returned success(Set EnduranceGaming)\n");
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
            printf("ctlGetSet3DFeature(Get EnduranceGaming) returned failure code: 0x%X\n", Result);

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
            printf("ctlGetSet3DFeature returned success(Get EnduranceGaming)\n");

            printf(" Get3DProperty.ApplicationName = %s\n", Get3DProperty.ApplicationName);
            printf(" pGetEnduranceGaming->EGControl = %d\n", pGetEnduranceGaming->EGControl);
            printf(" pGetEnduranceGaming->EGMode = %d\n", pGetEnduranceGaming->EGMode);
        }
    }

    if (nullptr != Get3DProperty.pCustomValue)
    {
        free(Get3DProperty.pCustomValue);
        Get3DProperty.pCustomValue = NULL;
    }

    printf("======================Endurance Gaming test -> Global settings======================\n");

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
        printf(" SetEnduranceGaming.EGControl = %d\n", SetEnduranceGaming.EGControl);
        printf(" SetEnduranceGaming.EGMode = %d\n", SetEnduranceGaming.EGMode);
        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlGetSet3DFeature(Set EnduranceGaming) returned failure code: 0x%X\n", Result);

            return Result;
        }
        else
        {
            printf("ctlGetSet3DFeature returned success(Set EnduranceGaming)\n");
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
            printf("ctlGetSet3DFeature(Get EnduranceGaming) returned failure code: 0x%X\n", Result);

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
            printf("ctlGetSet3DFeature returned success(Get EnduranceGaming)\n");

            printf(" pGetEnduranceGamingGlobal->EGControl = %d\n", pGetEnduranceGamingGlobal->EGControl);
            printf(" pGetEnduranceGamingGlobal->EGMode = %d\n", pGetEnduranceGamingGlobal->EGMode);
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

    printf("======================Gaming flips test -> Per Application settings =====================\n");
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

    // Gaming flips Global GET/SET

    printf("======================Gaming flips test -> Global settings======================\n");
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
            printf("ctlGetSupported3DCapabilities returned failure code: 0x%X\n", Result);
            return Result;
        }
        else
        {
            printf("ctlGetSupported3DCapabilities returned success\n");
            FeatureCaps3D.pFeatureDetails = (ctl_3d_feature_details_t *)malloc(sizeof(ctl_3d_feature_details_t) * FeatureCaps3D.NumSupportedFeatures);
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
                printf("ctlGetSupported3DCapabilities returned failure code when called with NumSupportedFeatures set: 0x%X\n", Result);
                if (nullptr != FeatureCaps3D.pFeatureDetails)
                {
                    free(FeatureCaps3D.pFeatureDetails);
                    FeatureCaps3D.pFeatureDetails = nullptr;
                }
                return Result;
            }
            if (nullptr != FeatureCaps3D.pFeatureDetails)
            {
                free(FeatureCaps3D.pFeatureDetails);
                FeatureCaps3D.pFeatureDetails = nullptr;
            }
        }
    }

    return Result;
}

bool QuiteEventThread = false;
DWORD WINAPI CtlEventThread(LPVOID ThreadParameterPtr)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    ctl_wait_property_change_args_t Args = { 0 };

    printf("CtlEventThread: Entering thread\n");

    Args.PropertyType    = CTL_PROPERTY_TYPE_FLAG_MEDIA | CTL_PROPERTY_TYPE_FLAG_DISPLAY | CTL_PROPERTY_TYPE_FLAG_3D;
    Args.Size            = sizeof(Args);
    Args.TimeOutMilliSec = 500; // 0.5sec

    printf("CtlEventThread: Calling ctlWaitForPropertyChange() and getting to blocked state...\n");

    do
    {
        Result = ctlWaitForPropertyChange((ctl_device_adapter_handle_t)ThreadParameterPtr, &Args); // blocking call
        if (CTL_RESULT_ERROR_WAIT_TIMEOUT == Result)
            continue;
    } while (false == QuiteEventThread);

    printf("CtlEventThread: ctlWaitForPropertyChange() unblocked return value = %d\n", Result);
    printf("CtlEventThread: Exiting thread\n");

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

    printf("Do you want to listen for events (L), listen via a thread (T), quit (Q) or do series of set calls (any other key)? ");
    int Key = _getch();
    printf("\n");
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
                    printf("CreateThread failed!\n");
                    Result = CTL_RESULT_ERROR_OS_CALL;
                    break;
                }
                else
                {
                    printf("Created thread ID = 0x%X\n", ThreadID);
                }
            }
            else
            {
                ctl_wait_property_change_args_t Args = { 0 };

                Args.PropertyType    = CTL_PROPERTY_TYPE_FLAG_MEDIA | CTL_PROPERTY_TYPE_FLAG_DISPLAY | CTL_PROPERTY_TYPE_FLAG_3D;
                Args.Size            = sizeof(Args);
                Args.TimeOutMilliSec = 0xFFFFFFFF; // INFINITE;

                printf("Calling ctlWaitForPropertyChange() and getting to blocked state...\n");

                Result = ctlWaitForPropertyChange(hAdapter, &Args); // blocking call

                printf("ctlWaitForPropertyChange() unblocked return value = %d\n", Result);
            }
        }
        else
        {
            printf("Trying a 3D set call & restoring it (overrall 2 set calls)...\n");

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
            printf("Press any key to signal thread to exit...");
            int Key = _getch();
            printf("\n");
            QuiteEventThread = true;
            break; // exit from loop
        }
        else
        {
            printf("Quit(q) or continue (any other key)? ");
            int Key = _getch();
            printf("\n");
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
        printf("\nDoing a GetAILProfile(%s, %s)\n", ApplicationName ? ApplicationName : "Global", AppProfileGet.TierType == CTL_3D_TIER_TYPE_FLAG_COMPATIBILITY ? "Compatibility" : "Performance");

        Result = ctlGetSet3DFeature(hDevices, &Get3DProperty);

        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlGetSet3DFeature(Get AppProfile) returned failure code: 0x%X\n", Result);
            if (Result == CTL_RESULT_ERROR_DATA_NOT_FOUND)
            {
                printf(" Reason: Value not found\n");
                printf(" Use DefaultEnabledTierProfiles (=%s) for UI default selection if required\n", GetProfileTierName(AppProfileGet.DefaultEnabledTierProfiles, TierName, 100));
            }
        }
        else if (Get3DProperty.CustomValueSize > 0)
        {
            printf("ctlGetSet3DFeature returned success(Get AppProfile)\n");
            printf(" App name = %s\n", ApplicationName == NULL ? "Global" : ApplicationName);
            printf(" AppProfileGet.TierType = %s\n", GetProfileTypeName(AppProfileGet.TierType));
            printf(" AppProfileGet.EnabledTierProfiles = %s\n", GetProfileTierName(AppProfileGet.EnabledTierProfiles, TierName, 100));
            printf(" AppProfileGet.DefaultEnabledTierProfiles = %s\n", GetProfileTierName(AppProfileGet.DefaultEnabledTierProfiles, TierName, 100));
            printf(" AppProfileGet.CustomizationSupportedTierProfiles = %s\n", GetProfileTierName(AppProfileGet.CustomizationSupportedTierProfiles, TierName, 100));
            printf(" AppProfileGet.CustomizationEnabledTierProfiles = %s\n", GetProfileTierName(AppProfileGet.CustomizationEnabledTierProfiles, TierName, 100));
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
        printf("\nDoing a SetAILProfile(%s, %s)\n", ApplicationName ? ApplicationName : "Global", AppProfileSet.TierType == CTL_3D_TIER_TYPE_FLAG_COMPATIBILITY ? "Compatibility" : "Performance");

        Result = ctlGetSet3DFeature(hDevices, &Set3DProperty);

        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlGetSet3DFeature(Set AppProfile) returned failure code: 0x%X\n", Result);
            if (CTL_RESULT_ERROR_DATA_NOT_FOUND == Result)
                printf("Reason: Value not found\n");
            return Result;
        }
        else if (Set3DProperty.CustomValueSize > 0)
        {
            printf("ctlGetSet3DFeature returned success(Set AppProfile)\n");
            printf(" App name = %s\n", ApplicationName == NULL ? "Global" : ApplicationName);
            printf(" AppProfileSet.TierType = %s\n", GetProfileTypeName(AppProfileSet.TierType));
            printf(" AppProfileSet.EnabledTierProfiles = %s\n", GetProfileTierName(AppProfileSet.EnabledTierProfiles, TierName, 100));
            printf(" AppProfileSet.CustomizationSupportedTierProfiles = %s\n", GetProfileTierName(AppProfileSet.CustomizationSupportedTierProfiles, TierName, 100));
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

    printf("======================Gaming App Profile test======================\n");

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

    printf("======================CtlCMAAGamingFeatureTest======================\n");

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
    printf(" Get3DProperty.Value.EnumType.EnableType = %d\n", Get3DProperty.Value.EnumType.EnableType);
    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("Returned failure code: 0x%X\n", Result);
        if (CTL_RESULT_ERROR_DATA_NOT_FOUND != Result) // if value not present, it's expected, so continue with set/get verification
            return Result;
    }
    else
    {
        printf("ctlGetSet3DFeature returned success\n");
        InitialValue = Get3DProperty.Value.EnumType.EnableType;
    }

    // Set feature - global
    Set3DProperty.FeatureType               = CTL_3D_FEATURE_CMAA;
    Set3DProperty.bSet                      = TRUE;
    Set3DProperty.ValueType                 = CTL_PROPERTY_VALUE_TYPE_ENUM;
    Set3DProperty.Version                   = 0;
    Set3DProperty.Value.EnumType.EnableType = CTL_3D_CMAA_TYPES_OVERRIDE_MSAA;
    Result                                  = ctlGetSet3DFeature(hDevices, &Set3DProperty);
    printf(" Set3DProperty.Value.EnumType.EnableType = CTL_3D_CMAA_TYPES_OVERRIDE_MSAA\n");
    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("Returned failure code: 0x%X\n", Result);
        return Result;
    }
    else
    {
        printf("ctlGetSet3DFeature returned success\n");
    }

    // Get feature back & check - global
    Get3DProperty.FeatureType = CTL_3D_FEATURE_CMAA;
    Get3DProperty.bSet        = FALSE;
    Get3DProperty.ValueType   = CTL_PROPERTY_VALUE_TYPE_ENUM;
    Get3DProperty.Version     = 0;
    Result                    = ctlGetSet3DFeature(hDevices, &Get3DProperty);
    printf(" Get3DProperty.Value.EnumType.EnableType = %d\n", Get3DProperty.Value.EnumType.EnableType);
    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("Returned failure code: 0x%X\n", Result);
        return Result;
    }
    else
    {
        printf("ctlGetSet3DFeature returned success\n");
        if (Get3DProperty.Value.EnumType.EnableType != Set3DProperty.Value.EnumType.EnableType)
        {
            printf("ERROR: Value didn't get set!\n");
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
    printf(" Set3DProperty.Value.EnumType.EnableType = %d\n", Set3DProperty.Value.EnumType.EnableType);
    if (CTL_RESULT_SUCCESS != Result)
    {
        printf("Returned failure code: 0x%X\n", Result);
        return Result;
    }
    else
    {
        printf("ctlGetSet3DFeature returned success\n");
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

    printf("======================Global Or Per App test======================\n");

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
        printf(" Set3DProperty.ApplicationName = %s\n", Set3DProperty.ApplicationName);
        printf(" Set3DProperty.Value.EnumType.EnableType = %d\n", Set3DProperty.Value.EnumType.EnableType);

        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlGetSet3DFeature(Set Global Over Per App Setting (Set call)) returned failure code: 0x%X\n", Result);

            return Result;
        }
        else
        {
            printf("ctlGetSet3DFeature returned success(Set Global Over Per App Setting (Set call))\n");
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
            printf("ctlGetSet3DFeature(Set Global Over Per App Setting (Get call)) returned failure code: 0x%X\n", Result);

            return Result;
        }
        else
        {
            printf("ctlGetSet3DFeature returned success(Set Global Over Per App Setting (Get call))\n");
            printf(" Get3DProperty.ApplicationName = %s\n", Get3DProperty.ApplicationName);
            printf(" Get3DProperty.Value.EnumType.EnableTypee = %d\n", Get3DProperty.Value.EnumType.EnableType);
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
    Result = ctlInit(&CtlInitArgs, &hAPIHandle);

    if (CTL_RESULT_SUCCESS == Result)
    {
        // Initialization successful
        // Get the list of Intel Adapters
        Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);

        if (CTL_RESULT_SUCCESS == Result)
        {
            hDevices = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * AdapterCount);
            if (nullptr == hDevices)
            {
                return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
            }

            Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);
        }
        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlEnumerateDevices returned failure code: 0x%X\n", Result);
            goto free_exit;
        }

        for (Index = 0; Index < AdapterCount; Index++)
        {
            if (CTL_RESULT_SUCCESS == Result)
            {

                Result = CtlGet3DFeatureCaps(hDevices[Index]);

                if (CTL_RESULT_SUCCESS != Result)
                {
                    printf("CtlGet3DCaps returned failure code: 0x%X\n", Result);
                }
                STORE_RESET_ERROR(Result);

                Result = CtlEnduranceGamingTest(hDevices[Index]);

                if (CTL_RESULT_SUCCESS != Result)
                {
                    printf("CtlEnduranceGamingTest failure code: 0x%X\n", Result);
                }
                STORE_RESET_ERROR(Result);

                Result = CtlGamingFlipTest(hDevices[Index]);

                if (CTL_RESULT_SUCCESS != Result)
                {
                    printf("CtlGamingFlipTest failure code: 0x%X\n", Result);
                }
                STORE_RESET_ERROR(Result);

                CtlGamingAppProfile(hDevices[Index]);
                if (CTL_RESULT_SUCCESS != Result)
                {
                    printf("CtlGamingAppProfile failure code: 0x%X\n", Result);
                }
                STORE_RESET_ERROR(Result);

                CtlCMAAGamingFeatureTest(hDevices[Index]);
                if (CTL_RESULT_SUCCESS != Result)
                {
                    printf("CtlCMAAGamingFeatureTest failure code: 0x%X\n", Result);
                }
                STORE_RESET_ERROR(Result);

                CtlGlobalOrPerAppTest(hDevices[Index]);
                if (CTL_RESULT_SUCCESS != Result)
                {
                    printf("CtlGlobalOrPerAppTest failure code: 0x%X\n", Result);
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
free_exit:

    ctlClose(hAPIHandle);

    if (hDevices != nullptr)
    {
        free(hDevices);
        hDevices = nullptr;
    }

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
