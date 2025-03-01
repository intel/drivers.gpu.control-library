//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  PowerFeature_Sample_APP.cpp
 * @brief : This file contains the 'main' function and the PowerFeature Sample APP. Program execution begins and ends there.
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

#define TARGET_BRIGHTNESS_VALUE 50000;  // 50000 in milli percentage so effectively 50%
#define SMOOTH_TRANSITION_TIME_VALUE 0; // if its 0 then immediate change. Maximum value supported is 1000ms.

ctl_result_t GResult = CTL_RESULT_SUCCESS;

/***************************************************************
 * @brief
 * Power feature Test for PSR
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestPSRPowerFeature(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                                    = CTL_RESULT_SUCCESS;
    ctl_power_optimization_caps_t PowerCaps                = { 0 };
    ctl_power_optimization_settings_t AppliedPowerSettings = { 0 };
    ctl_power_optimization_settings_t NewPowerSettings     = { 0 };

    PowerCaps.Size                                = sizeof(ctl_power_optimization_caps_t);
    AppliedPowerSettings.Size                     = sizeof(ctl_power_optimization_settings_t);
    AppliedPowerSettings.PowerOptimizationFeature = CTL_POWER_OPTIMIZATION_FLAG_PSR;
    AppliedPowerSettings.PowerSource              = CTL_POWER_SOURCE_DC;
    AppliedPowerSettings.PowerOptimizationPlan    = CTL_POWER_OPTIMIZATION_PLAN_BALANCED;

    NewPowerSettings.Size                     = sizeof(ctl_power_optimization_settings_t);
    NewPowerSettings.PowerOptimizationFeature = CTL_POWER_OPTIMIZATION_FLAG_PSR;
    NewPowerSettings.Enable                   = TRUE;
    NewPowerSettings.PowerSource              = CTL_POWER_SOURCE_DC;
    NewPowerSettings.PowerOptimizationPlan    = CTL_POWER_OPTIMIZATION_PLAN_BALANCED;

    // Get Applied PowerFeature Cap
    Result = ctlGetPowerOptimizationCaps(hDisplayOutput, &PowerCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationCaps (PSR)");

    if (CTL_POWER_OPTIMIZATION_FLAG_PSR != (PowerCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_PSR))
    {
        APP_LOG_WARN("PSR is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    APP_LOG_INFO("PSR is supported");

    // Set Current PowerFeature
    Result = ctlSetPowerOptimizationSetting(hDisplayOutput, &NewPowerSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlSetPowerOptimizationSetting (PSR)");

    // Get Applied PowerFeature
    Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (PSR)");
    APP_LOG_INFO("PSR Enable = %d", AppliedPowerSettings.Enable);

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Power feature Test for DPST
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestDPSTPowerFeature(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                                    = CTL_RESULT_SUCCESS;
    ctl_power_optimization_settings_t NewPowerSettings     = { 0 };
    ctl_power_optimization_settings_t AppliedPowerSettings = { 0 };
    ctl_power_optimization_caps_t PowerCaps                = { 0 };

    PowerCaps.Size                                = sizeof(ctl_power_optimization_caps_t);
    AppliedPowerSettings.Size                     = sizeof(ctl_power_optimization_settings_t);
    AppliedPowerSettings.PowerOptimizationFeature = CTL_POWER_OPTIMIZATION_FLAG_DPST;
    AppliedPowerSettings.PowerSource              = CTL_POWER_SOURCE_DC;
    AppliedPowerSettings.PowerOptimizationPlan    = CTL_POWER_OPTIMIZATION_PLAN_BALANCED;

    Result = ctlGetPowerOptimizationCaps(hDisplayOutput, &PowerCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationCaps (DPST)");

    if (CTL_POWER_OPTIMIZATION_FLAG_DPST != (PowerCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_DPST))
    {
        APP_LOG_WARN("DPST is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (DPST)");

    if (CTL_POWER_OPTIMIZATION_DPST_FLAG_BKLT != (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.SupportedFeatures & CTL_POWER_OPTIMIZATION_DPST_FLAG_BKLT))
    {
        APP_LOG_WARN("BKLT is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    APP_LOG_INFO("DPST is supported");
    APP_LOG_INFO("DPST Enable = %d", AppliedPowerSettings.Enable);
    APP_LOG_INFO("DPST MinLevel = %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MinLevel);
    APP_LOG_INFO("DPST MaxLevel = %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MaxLevel);

    if (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.EnabledFeatures & CTL_POWER_OPTIMIZATION_DPST_FLAG_EPSM)
    {
        APP_LOG_INFO("DPST EPSM enabled");
    }

    uint8_t Levels[2]                                             = { AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MinLevel, AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MaxLevel };
    NewPowerSettings.Size                                         = sizeof(ctl_power_optimization_settings_t);
    NewPowerSettings.PowerOptimizationFeature                     = CTL_POWER_OPTIMIZATION_FLAG_DPST;
    NewPowerSettings.Enable                                       = TRUE;
    NewPowerSettings.FeatureSpecificData.DPSTInfo.EnabledFeatures = CTL_POWER_OPTIMIZATION_DPST_FLAG_EPSM | CTL_POWER_OPTIMIZATION_DPST_FLAG_BKLT; // BKLT bit should be set to enable Intel DPST
    NewPowerSettings.PowerSource                                  = CTL_POWER_SOURCE_DC;
    NewPowerSettings.PowerOptimizationPlan                        = CTL_POWER_OPTIMIZATION_PLAN_BALANCED;

    for (uint8_t Count = 0; Count < 2; Count++)
    {
        // Set PowerFeature
        NewPowerSettings.FeatureSpecificData.DPSTInfo.Level = Levels[Count];

        Result = ctlSetPowerOptimizationSetting(hDisplayOutput, &NewPowerSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlSetPowerOptimizationSetting (DPST)");

        // Get PowerFeature
        Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (DPST)");

        if (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.Level != NewPowerSettings.FeatureSpecificData.DPSTInfo.Level)
        {
            APP_LOG_ERROR("Current and Applied levels mismatched: %d, %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.Level, NewPowerSettings.FeatureSpecificData.DPSTInfo.Level);
        }
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Power feature Test for OPST
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestOPSTPowerFeature(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                                    = CTL_RESULT_SUCCESS;
    ctl_power_optimization_settings_t NewPowerSettings     = { 0 };
    ctl_power_optimization_settings_t AppliedPowerSettings = { 0 };
    ctl_power_optimization_caps_t PowerCaps                = { 0 };

    PowerCaps.Size                                = sizeof(ctl_power_optimization_caps_t);
    AppliedPowerSettings.Size                     = sizeof(ctl_power_optimization_settings_t);
    AppliedPowerSettings.PowerOptimizationFeature = CTL_POWER_OPTIMIZATION_FLAG_DPST;
    AppliedPowerSettings.PowerSource              = CTL_POWER_SOURCE_DC;
    AppliedPowerSettings.PowerOptimizationPlan    = CTL_POWER_OPTIMIZATION_PLAN_BALANCED;

    Result = ctlGetPowerOptimizationCaps(hDisplayOutput, &PowerCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationCaps (OPST)");

    Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (OPST)");

    if ((CTL_POWER_OPTIMIZATION_FLAG_DPST != (PowerCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_DPST)) ||
        (CTL_POWER_OPTIMIZATION_DPST_FLAG_OPST != (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.SupportedFeatures & CTL_POWER_OPTIMIZATION_DPST_FLAG_OPST)))
    {
        APP_LOG_WARN("OPST is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    APP_LOG_INFO("OPST is supported");
    APP_LOG_INFO("GetPowerSettings.FeatureSpecificData.DPSTInfo.EnabledFeatures = CTL_POWER_OPTIMIZATION_DPST_FLAG_OPST");
    APP_LOG_INFO("OPST MinLevel = %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MinLevel);
    APP_LOG_INFO("OPST MaxLevel = %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MaxLevel);

    NewPowerSettings.Size                                         = sizeof(ctl_power_optimization_settings_t);
    NewPowerSettings.PowerOptimizationFeature                     = CTL_POWER_OPTIMIZATION_FLAG_DPST;
    NewPowerSettings.Enable                                       = TRUE;
    NewPowerSettings.FeatureSpecificData.DPSTInfo.EnabledFeatures = CTL_POWER_OPTIMIZATION_DPST_FLAG_OPST;
    NewPowerSettings.PowerSource                                  = CTL_POWER_SOURCE_DC;
    NewPowerSettings.PowerOptimizationPlan                        = CTL_POWER_OPTIMIZATION_PLAN_BALANCED;

    uint8_t Levels[2] = { AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MinLevel, AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MaxLevel };

    for (uint8_t Count = 0; Count < 2; Count++)
    {
        // Set PowerFeature
        NewPowerSettings.FeatureSpecificData.DPSTInfo.Level = Levels[Count];

        Result = ctlSetPowerOptimizationSetting(hDisplayOutput, &NewPowerSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlSetPowerOptimizationSetting (OPST)");

        // Get PowerFeature
        Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (OPST)");

        if (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.Level != NewPowerSettings.FeatureSpecificData.DPSTInfo.Level)
        {
            APP_LOG_ERROR("Current and Applied levels  mismatched: %d, %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.Level, NewPowerSettings.FeatureSpecificData.DPSTInfo.Level);
        }
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Power feature Test for ELP
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestELPPowerFeature(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                                    = CTL_RESULT_SUCCESS;
    ctl_power_optimization_settings_t NewPowerSettings     = { 0 };
    ctl_power_optimization_settings_t AppliedPowerSettings = { 0 };
    ctl_power_optimization_caps_t PowerCaps                = { 0 };

    PowerCaps.Size                                = sizeof(ctl_power_optimization_caps_t);
    AppliedPowerSettings.Size                     = sizeof(ctl_power_optimization_settings_t);
    AppliedPowerSettings.PowerOptimizationFeature = CTL_POWER_OPTIMIZATION_FLAG_DPST;
    AppliedPowerSettings.PowerSource              = CTL_POWER_SOURCE_DC;
    AppliedPowerSettings.PowerOptimizationPlan    = CTL_POWER_OPTIMIZATION_PLAN_BALANCED;

    Result = ctlGetPowerOptimizationCaps(hDisplayOutput, &PowerCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationCaps (ELP)");

    Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (ELP)");

    if ((CTL_POWER_OPTIMIZATION_FLAG_DPST != (PowerCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_DPST)) ||
        (CTL_POWER_OPTIMIZATION_DPST_FLAG_ELP != (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.SupportedFeatures & CTL_POWER_OPTIMIZATION_DPST_FLAG_ELP)))
    {
        APP_LOG_WARN("ELP is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    APP_LOG_INFO("GetPowerSettings.FeatureSpecificData.DPSTInfo.EnabledFeatures = CTL_POWER_OPTIMIZATION_DPST_FLAG_ELP");
    APP_LOG_INFO("ELP MinLevel = %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MinLevel);
    APP_LOG_INFO("ELP MaxLevel = %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MaxLevel);

    uint8_t Levels[2]                                             = { AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MinLevel, AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MaxLevel };
    NewPowerSettings.Size                                         = sizeof(ctl_power_optimization_settings_t);
    NewPowerSettings.PowerOptimizationFeature                     = CTL_POWER_OPTIMIZATION_FLAG_DPST;
    NewPowerSettings.Enable                                       = TRUE;
    NewPowerSettings.FeatureSpecificData.DPSTInfo.EnabledFeatures = CTL_POWER_OPTIMIZATION_DPST_FLAG_ELP;
    NewPowerSettings.PowerSource                                  = CTL_POWER_SOURCE_DC;
    NewPowerSettings.PowerOptimizationPlan                        = CTL_POWER_OPTIMIZATION_PLAN_BALANCED;

    for (uint8_t Count = 0; Count < 2; Count++)
    {
        // Set PowerFeature
        NewPowerSettings.FeatureSpecificData.DPSTInfo.Level = Levels[Count];

        Result = ctlSetPowerOptimizationSetting(hDisplayOutput, &NewPowerSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlSetPowerOptimizationSetting (ELP)");

        Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (ELP)");

        if (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.Level != NewPowerSettings.FeatureSpecificData.DPSTInfo.Level)
        {
            APP_LOG_ERROR("Current and Applied levels mismatched: %d, %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.Level, NewPowerSettings.FeatureSpecificData.DPSTInfo.Level);
        }
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Brightness control Test
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestBrightnessControl(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                                                 = CTL_RESULT_SUCCESS;
    ctl_set_brightness_t NewBrightnessSettings                          = { 0 };
    ctl_get_brightness_t AppliedBrightnessSettings                      = { 0 };
    bool IsDisplayActive                                                = false;
    bool IsCompanionDisplay                                             = false;
    AppliedBrightnessSettings.Size                                      = sizeof(ctl_get_brightness_t);
    ctl_adapter_display_encoder_properties_t StDisplayEncoderProperties = { 0 }; // display encoder properties
    StDisplayEncoderProperties.Size                                     = sizeof(ctl_adapter_display_encoder_properties_t);
    ctl_display_properties_t DisplayProperties                          = { 0 }; // display properties
    DisplayProperties.Size                                              = sizeof(ctl_display_properties_t);

    // check if the display is active.
    Result = ctlGetDisplayProperties(hDisplayOutput, &DisplayProperties);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDisplayProperties (Brightness)");

    if (0 == (DisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE))
    {
        Result = CTL_RESULT_ERROR_DISPLAY_NOT_ACTIVE;
        goto Exit;
    }

    // check for companion display , SET/GET call should be called for only companion display.
    Result = ctlGetAdaperDisplayEncoderProperties(hDisplayOutput, &StDisplayEncoderProperties);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetAdaperDisplayEncoderProperties (Brightness)");

    IsCompanionDisplay = StDisplayEncoderProperties.EncoderConfigFlags & CTL_ENCODER_CONFIG_FLAG_COMPANION_DISPLAY;
    IsDisplayActive    = DisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE;

    if (IsDisplayActive && IsCompanionDisplay)
    {
        NewBrightnessSettings.Size = sizeof(ctl_set_brightness_t);

        // Target Brightness value is in Milli percentage, range (0,100000).
        NewBrightnessSettings.TargetBrightness         = TARGET_BRIGHTNESS_VALUE;      // 50000 in milli percentage so effectively 50%
        NewBrightnessSettings.SmoothTransitionTimeInMs = SMOOTH_TRANSITION_TIME_VALUE; // if its 0 then immediate change. Maximum value supported is 1000ms.

        Result = ctlSetBrightnessSetting(hDisplayOutput, &NewBrightnessSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlSetBrightnessSetting (Brightness)");

        // Get PowerFeature brightness
        Result = ctlGetBrightnessSetting(hDisplayOutput, &AppliedBrightnessSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetBrightnessSetting (Brightness)");

        if (AppliedBrightnessSettings.TargetBrightness != NewBrightnessSettings.TargetBrightness)
        {
            APP_LOG_ERROR("Current and Applied TargetBrightness mismatched: %d, %d", AppliedBrightnessSettings.TargetBrightness, NewBrightnessSettings.TargetBrightness);
        }

        APP_LOG_INFO("Current brightness = %d", AppliedBrightnessSettings.CurrentBrightness);
        APP_LOG_INFO("Target brightness = %d", AppliedBrightnessSettings.TargetBrightness);
    }
    else
    {
        return CTL_RESULT_ERROR_INVALID_OPERATION_TYPE;
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Power feature Test for APD
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestApdPowerFeature(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                                    = CTL_RESULT_SUCCESS;
    ctl_power_optimization_settings_t NewPowerSettings     = { 0 };
    ctl_power_optimization_settings_t AppliedPowerSettings = { 0 };
    ctl_power_optimization_caps_t PowerCaps                = { 0 };

    PowerCaps.Size                                = sizeof(ctl_power_optimization_caps_t);
    AppliedPowerSettings.Size                     = sizeof(ctl_power_optimization_settings_t);
    AppliedPowerSettings.PowerOptimizationFeature = CTL_POWER_OPTIMIZATION_FLAG_DPST;
    AppliedPowerSettings.PowerSource              = CTL_POWER_SOURCE_DC;

    Result = ctlGetPowerOptimizationCaps(hDisplayOutput, &PowerCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationCaps (APD)");

    Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (APD)");

    if ((CTL_POWER_OPTIMIZATION_FLAG_DPST != (PowerCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_DPST)) ||
        (CTL_POWER_OPTIMIZATION_DPST_FLAG_APD != (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.SupportedFeatures & CTL_POWER_OPTIMIZATION_DPST_FLAG_APD)))
    {
        APP_LOG_WARN("APD is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    APP_LOG_INFO("APD MinLevel = %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MinLevel);
    APP_LOG_INFO("APD MaxLevel = %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MaxLevel);

    uint8_t Levels[2] = { AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MinLevel, AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MaxLevel };

    NewPowerSettings.Size                                         = sizeof(ctl_power_optimization_settings_t);
    NewPowerSettings.PowerOptimizationFeature                     = CTL_POWER_OPTIMIZATION_FLAG_DPST;
    NewPowerSettings.Enable                                       = TRUE;
    NewPowerSettings.FeatureSpecificData.DPSTInfo.EnabledFeatures = CTL_POWER_OPTIMIZATION_DPST_FLAG_APD;
    NewPowerSettings.PowerSource                                  = CTL_POWER_SOURCE_DC;

    for (uint8_t Count = 0; Count < 2; Count++)
    {
        // Set PowerFeature
        NewPowerSettings.FeatureSpecificData.DPSTInfo.Level = Levels[Count];

        Result = ctlSetPowerOptimizationSetting(hDisplayOutput, &NewPowerSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlSetPowerOptimizationSetting (APD)");

        Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (APD)");

        if (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.Level != NewPowerSettings.FeatureSpecificData.DPSTInfo.Level)
        {
            APP_LOG_ERROR("Current and Applied levels mismatched: %d, %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.Level, NewPowerSettings.FeatureSpecificData.DPSTInfo.Level);
        }
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Power feature Test for PIXOPTIX
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestPixOptixPowerFeature(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                                    = CTL_RESULT_SUCCESS;
    ctl_power_optimization_settings_t NewPowerSettings     = { 0 };
    ctl_power_optimization_settings_t AppliedPowerSettings = { 0 };
    ctl_power_optimization_caps_t PowerCaps                = { 0 };

    PowerCaps.Size                                = sizeof(ctl_power_optimization_caps_t);
    AppliedPowerSettings.Size                     = sizeof(ctl_power_optimization_settings_t);
    AppliedPowerSettings.PowerOptimizationFeature = CTL_POWER_OPTIMIZATION_FLAG_DPST;
    AppliedPowerSettings.PowerSource              = CTL_POWER_SOURCE_DC;

    Result = ctlGetPowerOptimizationCaps(hDisplayOutput, &PowerCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationCaps (PIXOPTIX)");

    Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (PIXOPTIX)");

    if ((CTL_POWER_OPTIMIZATION_FLAG_DPST != (PowerCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_DPST)) ||
        (CTL_POWER_OPTIMIZATION_DPST_FLAG_PIXOPTIX != (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.SupportedFeatures & CTL_POWER_OPTIMIZATION_DPST_FLAG_PIXOPTIX)))
    {
        APP_LOG_WARN("PIXOPTIX is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    NewPowerSettings.Size                                         = sizeof(ctl_power_optimization_settings_t);
    NewPowerSettings.PowerOptimizationFeature                     = CTL_POWER_OPTIMIZATION_FLAG_DPST;
    NewPowerSettings.Enable                                       = TRUE;
    NewPowerSettings.FeatureSpecificData.DPSTInfo.EnabledFeatures = CTL_POWER_OPTIMIZATION_DPST_FLAG_PIXOPTIX;
    NewPowerSettings.PowerSource                                  = CTL_POWER_SOURCE_DC;

    Result = ctlSetPowerOptimizationSetting(hDisplayOutput, &NewPowerSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlSetPowerOptimizationSetting (PIXOPTIX)");

    Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (PIXOPTIX)");

    if (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.EnabledFeatures & CTL_POWER_OPTIMIZATION_DPST_FLAG_PIXOPTIX)
    {
        APP_LOG_INFO("PIXOPTIX is enabled");
    }
    else
    {
        APP_LOG_ERROR("PIXOPTIX is not enabled");
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Power feature Test for Alrr
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestAlrrFeature(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                                        = CTL_RESULT_SUCCESS;
    ctl_power_optimization_caps_t PowerOptimizationCaps        = { 0 };
    ctl_power_optimization_settings_t PowerOptimizationSetting = { 0 };

    PowerOptimizationCaps.Size = sizeof(ctl_pfnGetPowerOptimizationCaps_t);
    Result                     = ctlGetPowerOptimizationCaps(hDisplayOutput, &PowerOptimizationCaps);

    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationCaps (ALRR)");

    if (PowerOptimizationCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_LRR)
    {
        PowerOptimizationSetting.PowerOptimizationFeature = CTL_POWER_OPTIMIZATION_FLAG_LRR;
        PowerOptimizationSetting.Size                     = sizeof(ctl_power_optimization_settings_t);
        Result                                            = ctlGetPowerOptimizationSetting(hDisplayOutput, &PowerOptimizationSetting);

        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting");

        if (PowerOptimizationSetting.FeatureSpecificData.LRRInfo.CurrentLRRTypes & CTL_POWER_OPTIMIZATION_LRR_FLAG_ALRR)
        {
            APP_LOG_INFO("ALRR is enabled");
        }
        else
        {
            APP_LOG_WARN("ALRR is not enabled");
        }
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Power feature Test for Fbc
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestFbcPowerFeature(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                                        = CTL_RESULT_SUCCESS;
    ctl_power_optimization_caps_t PowerOptimizationCaps        = { 0 };
    ctl_power_optimization_settings_t PowerOptimizationSetting = { 0 };

    PowerOptimizationCaps.Size = sizeof(ctl_pfnGetPowerOptimizationCaps_t);
    Result                     = ctlGetPowerOptimizationCaps(hDisplayOutput, &PowerOptimizationCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationCaps (FBC)");

    if (CTL_POWER_OPTIMIZATION_FLAG_FBC != (PowerOptimizationCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_FBC))
    {
        APP_LOG_WARN("FBC is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }
    else
    {
        APP_LOG_INFO("FBC is supported");
    }

    if (PowerOptimizationCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_FBC)
    {
        PowerOptimizationSetting.PowerOptimizationFeature = CTL_POWER_OPTIMIZATION_FLAG_FBC;
        PowerOptimizationSetting.Size                     = sizeof(ctl_power_optimization_settings_t);
        Result                                            = ctlGetPowerOptimizationSetting(hDisplayOutput, &PowerOptimizationSetting);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting");

        APP_LOG_INFO("FBC Enable Status= %d", PowerOptimizationSetting.Enable);
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Power feature Test for CABC
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestCABCPowerFeature(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                                    = CTL_RESULT_SUCCESS;
    ctl_power_optimization_settings_t NewPowerSettings     = { 0 };
    ctl_power_optimization_settings_t AppliedPowerSettings = { 0 };
    ctl_power_optimization_caps_t PowerCaps                = { 0 };

    PowerCaps.Size                                = sizeof(ctl_power_optimization_caps_t);
    AppliedPowerSettings.Size                     = sizeof(ctl_power_optimization_settings_t);
    AppliedPowerSettings.PowerOptimizationFeature = CTL_POWER_OPTIMIZATION_FLAG_DPST;
    AppliedPowerSettings.PowerSource              = CTL_POWER_SOURCE_DC;
    AppliedPowerSettings.PowerOptimizationPlan    = CTL_POWER_OPTIMIZATION_PLAN_BALANCED;

    Result = ctlGetPowerOptimizationCaps(hDisplayOutput, &PowerCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationCaps (CABC)");

    Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (CABC)");

    if ((CTL_POWER_OPTIMIZATION_FLAG_DPST != (PowerCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_DPST)) ||
        (CTL_POWER_OPTIMIZATION_DPST_FLAG_PANEL_CABC != (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.SupportedFeatures & CTL_POWER_OPTIMIZATION_DPST_FLAG_PANEL_CABC)))
    {
        APP_LOG_WARN("CABC is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    APP_LOG_INFO("GetPowerSettings.FeatureSpecificData.DPSTInfo.EnabledFeatures = CTL_POWER_OPTIMIZATION_DPST_FLAG_PANEL_CABC");
    APP_LOG_INFO("CABC MinLevel = %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MinLevel);
    APP_LOG_INFO("CABC MaxLevel = %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MaxLevel);

    uint8_t Levels[2]                                             = { AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MinLevel, AppliedPowerSettings.FeatureSpecificData.DPSTInfo.MaxLevel };
    NewPowerSettings.Size                                         = sizeof(ctl_power_optimization_settings_t);
    NewPowerSettings.PowerOptimizationFeature                     = CTL_POWER_OPTIMIZATION_FLAG_DPST;
    NewPowerSettings.Enable                                       = TRUE;
    NewPowerSettings.FeatureSpecificData.DPSTInfo.EnabledFeatures = CTL_POWER_OPTIMIZATION_DPST_FLAG_PANEL_CABC;
    NewPowerSettings.PowerSource                                  = CTL_POWER_SOURCE_DC;
    NewPowerSettings.PowerOptimizationPlan                        = CTL_POWER_OPTIMIZATION_PLAN_BALANCED;

    for (uint8_t Count = 0; Count < 2; Count++)
    {
        // Set PowerFeature
        NewPowerSettings.FeatureSpecificData.DPSTInfo.Level = Levels[Count];

        Result = ctlSetPowerOptimizationSetting(hDisplayOutput, &NewPowerSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlSetPowerOptimizationSetting (CABC)");

        Result = ctlGetPowerOptimizationSetting(hDisplayOutput, &AppliedPowerSettings);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationSetting (CABC)");

        if (AppliedPowerSettings.FeatureSpecificData.DPSTInfo.Level != NewPowerSettings.FeatureSpecificData.DPSTInfo.Level)
        {
            APP_LOG_ERROR("Current and Applied levels mismatched: %d, %d", AppliedPowerSettings.FeatureSpecificData.DPSTInfo.Level, NewPowerSettings.FeatureSpecificData.DPSTInfo.Level);
        }
    }

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
            APP_LOG_WARN("Display %d is not attached, skipping the call for this display", DisplayIndex);
            continue;
        }
        else
        {
            APP_LOG_INFO("Attached Display Count: %d", DisplayIndex);
        }

        // NOTE: Currently DPST/ELP/OPST combination is not supported
        Result = TestPSRPowerFeature(hDisplayOutput[DisplayIndex]);

        STORE_AND_RESET_ERROR(Result);

        Result = TestDPSTPowerFeature(hDisplayOutput[DisplayIndex]);

        STORE_AND_RESET_ERROR(Result);

        Result = TestOPSTPowerFeature(hDisplayOutput[DisplayIndex]);

        STORE_AND_RESET_ERROR(Result);

        Result = TestELPPowerFeature(hDisplayOutput[DisplayIndex]);

        STORE_AND_RESET_ERROR(Result);

        Result = TestBrightnessControl(hDisplayOutput[DisplayIndex]);

        STORE_AND_RESET_ERROR(Result);

        Result = TestApdPowerFeature(hDisplayOutput[DisplayIndex]);

        STORE_AND_RESET_ERROR(Result);

        Result = TestPixOptixPowerFeature(hDisplayOutput[DisplayIndex]);

        STORE_AND_RESET_ERROR(Result);

        Result = TestAlrrFeature(hDisplayOutput[DisplayIndex]);

        STORE_AND_RESET_ERROR(Result);

        Result = TestFbcPowerFeature(hDisplayOutput[DisplayIndex]);

        STORE_AND_RESET_ERROR(Result);

        Result = TestCABCPowerFeature(hDisplayOutput[DisplayIndex]);

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
