//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  GenericIGCLApp.h
 * @brief
 *
 */

#pragma once

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <vector>

#include "igcl_api.h"

#define STORE_AND_RESET_ERROR(Result) \
    if (CTL_RESULT_SUCCESS != Result) \
        GResult = Result;             \
    Result = CTL_RESULT_SUCCESS;

#define CTL_FREE_MEM(Ptr) \
    if (NULL != Ptr)      \
    {                     \
        free(Ptr);        \
        Ptr = NULL;       \
    }

#define EXIT_ON_ERROR(Result)         \
    if (CTL_RESULT_SUCCESS != Result) \
        goto Exit;

#define LOG_AND_EXIT_ON_ERROR(Result, ErrRtrndByFunc)                               \
    if (CTL_RESULT_SUCCESS != Result)                                               \
    {                                                                               \
        printf("[ERROR] %s returned failure code: 0x%X\n", ErrRtrndByFunc, Result); \
        goto Exit;                                                                  \
    }

#define EXIT_ON_MEM_ALLOC_FAILURE(Ptr, AllocatedVarName)                     \
    if (NULL == Ptr)                                                         \
    {                                                                        \
        Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;                      \
        printf("[ERROR] Memory Allocation Failed: %s \n", AllocatedVarName); \
        goto Exit;                                                           \
    }

#define LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, ErrRtrndByFunc)                 \
    if (CTL_RESULT_SUCCESS != Result)                                               \
    {                                                                               \
        printf("[ERROR] %s returned failure code: 0x%X\n", ErrRtrndByFunc, Result); \
        STORE_AND_RESET_ERROR(Result);                                              \
    }

// Application logging macro
#define APP_LOG_INFO(fmt, ...) printf("[INFO] " fmt "\n", ##__VA_ARGS__)
#define APP_LOG_WARN(fmt, ...) printf("[WARN] " fmt "\n", ##__VA_ARGS__)
#define APP_LOG_ERROR(fmt, ...) printf("[ERROR] " fmt "\n", ##__VA_ARGS__)

#define PRINT_LOGS(...)  \
    printf(__VA_ARGS__); \
    printf("\n")

#define CONTROL_BIT(_i) (1 << _i)

typedef enum _feature_control_type
{
    ENDURANCE_GAMING_CONTROLS = 0, ///< Endurance gaming controls
    ENDURANCE_GAMING_MODES    = 1, ///< Endurance gaming modes
} feature_control_type;

/***************************************************************
 * @brief Method to get 3D feature name
 ***************************************************************/
inline char *Get3DFeatureName(ctl_3d_feature_t FeatureType)
{
    switch (FeatureType)
    {
        case CTL_3D_FEATURE_FRAME_PACING:
            return "Frame pacing";
        case CTL_3D_FEATURE_ENDURANCE_GAMING:
            return "Endurance gaming";
        case CTL_3D_FEATURE_FRAME_LIMIT:
            return "Frame limit";
        case CTL_3D_FEATURE_ANISOTROPIC:
            return "Anisotropic";
        case CTL_3D_FEATURE_CMAA:
            return "CMAA";
        case CTL_3D_FEATURE_TEXTURE_FILTERING_QUALITY:
            return "TEXTURE_FILTERING_QUALITY";
        case CTL_3D_FEATURE_ADAPTIVE_TESSELLATION:
            return "Adaptive tessellation";
        case CTL_3D_FEATURE_SHARPENING_FILTER:
            return "Sharpening filter";
        case CTL_3D_FEATURE_MSAA:
            return "MSAA";
        case CTL_3D_FEATURE_GAMING_FLIP_MODES:
            return "Gaming flip modes";
        case CTL_3D_FEATURE_ADAPTIVE_SYNC_PLUS:
            return "Adaptive sync plus";
        case CTL_3D_FEATURE_APP_PROFILES:
            return "Application profile";
        case CTL_3D_FEATURE_APP_PROFILE_DETAILS:
            return "Application profile custom details";
        case CTL_3D_FEATURE_EMULATED_TYPED_64BIT_ATOMICS:
            return "Emulated typed 64 bit atomics";
        case CTL_3D_FEATURE_VRR_WINDOWED_BLT:
            return "VRR windowed blt";
        case CTL_3D_FEATURE_GLOBAL_OR_PER_APP:
            return "global or per app settings";
        case CTL_3D_FEATURE_LOW_LATENCY:
            return "Low Latency";
        case CTL_3D_FEATURE_FRAME_GENERATION:
            return "XeSS Frame Generation";
        default:
            return "No Name";
    }
}

/***************************************************************
 * @brief
 * Helper function/macros
 * @param
 * @return char*
 ***************************************************************/
inline char *GetProfileTypeName(ctl_3d_tier_type_flags_t Flag)
{
    if ((CTL_3D_TIER_TYPE_FLAG_COMPATIBILITY & Flag) && (CTL_3D_TIER_TYPE_FLAG_PERFORMANCE & Flag))
        return "Compatibility and Performance";
    else if (CTL_3D_TIER_TYPE_FLAG_COMPATIBILITY & Flag)
        return "Compatibility";
    else if (CTL_3D_TIER_TYPE_FLAG_PERFORMANCE & Flag)
        return "Performance";

    return "Unknown tier type";
}

/***************************************************************
 * @brief
 * Helper function/macros
 * @param
 * @return char*
 ***************************************************************/
inline char *GetProfileTierName(ctl_3d_tier_profile_flags_t Flags, char *pTierName, rsize_t NameLength)
{
    strcpy_s(pTierName, NameLength, "");

    if (0 == Flags)
    {
        strcat_s(pTierName, NameLength, "None");
    }
    else
    {
        if (Flags & CTL_3D_TIER_PROFILE_FLAG_TIER_1)
            strcat_s(pTierName, NameLength, "Tier1, ");
        if (Flags & CTL_3D_TIER_PROFILE_FLAG_TIER_2)
            strcat_s(pTierName, NameLength, "Tier2, ");
        if (Flags & CTL_3D_TIER_PROFILE_FLAG_TIER_RECOMMENDED)
            strcat_s(pTierName, NameLength, "Recommended Tier");
    }

    return pTierName;
}

/***************************************************************
 * @brief Method to print 3d feature support across UMD in a generic way
 *
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
inline void Print3DFeatureSupport(ctl_3d_feature_details_t *pFeatureDetails)
{
    ctl_3d_feature_misc_flags_t FeatureSupport;
    FeatureSupport = pFeatureDetails->FeatureMiscSupport;
    if (CTL_3D_FEATURE_MISC_FLAG_DX11 & FeatureSupport)
    {
        printf("[INFO]  pFeatureDetails->FeatureMiscSupport = CTL_3D_FEATURE_MISC_FLAG_DX11\n");
    }
    if (CTL_3D_FEATURE_MISC_FLAG_DX12 & FeatureSupport)
    {
        printf("[INFO]  pFeatureDetails->FeatureMiscSupport = CTL_3D_FEATURE_MISC_FLAG_DX12\n");
    }
    if (CTL_3D_FEATURE_MISC_FLAG_VULKAN & FeatureSupport)
    {
        printf("[INFO]  pFeatureDetails->FeatureMiscSupport = CTL_3D_FEATURE_MISC_FLAG_VULKAN\n");
    }
    if (CTL_3D_FEATURE_MISC_FLAG_LIVE_CHANGE & FeatureSupport)
    {
        printf("[INFO]  pFeatureDetails->FeatureMiscSupport = CTL_3D_FEATURE_MISC_FLAG_LIVE_CHANGE\n");
    }
    return;
}

/***************************************************************
 * @brief Method to Print Endurance Gaming Control
 *
 * @param Control
 * @return
 ***************************************************************/
inline void PrintEnduranceGamingControl(uint32_t Control)
{
    switch (Control)
    {
        case CTL_3D_ENDURANCE_GAMING_CONTROL_TURN_OFF:
            printf("[INFO]  pEGCaps->EGControlCaps.SupportedTypes = CTL_3D_ENDURANCE_GAMING_CONTROL_TURN_OFF\n");

            break;

        case CTL_3D_ENDURANCE_GAMING_CONTROL_TURN_ON:
            printf("[INFO]  pEGCaps->EGControlCaps.SupportedTypes = CTL_3D_ENDURANCE_GAMING_CONTROL_TURN_ON\n");

            break;

        case CTL_3D_ENDURANCE_GAMING_CONTROL_AUTO:
            printf("[INFO]  pEGCaps->EGControlCaps.SupportedTypes = CTL_3D_ENDURANCE_GAMING_CONTROL_AUTO\n");

            break;

        default:
            return;
    }
}

/***************************************************************
 * @brief Method to Print Endurance Gaming Mode
 *
 * @param Control
 * @return
 ***************************************************************/
inline void PrintEnduranceGamingMode(uint32_t Mode)
{
    switch (Mode)
    {
        case CTL_3D_ENDURANCE_GAMING_MODE_BETTER_PERFORMANCE:
            printf("[INFO]  pEGCaps->EGModeCaps.SupportedTypes = CTL_3D_ENDURANCE_GAMING_MODE_BETTER_PERFORMANCE\n");

            break;

        case CTL_3D_ENDURANCE_GAMING_MODE_BALANCED:
            printf("[INFO]  pEGCaps->EGModeCaps.SupportedTypes = CTL_3D_ENDURANCE_GAMING_MODE_BALANCED\n");

            break;

        case CTL_3D_ENDURANCE_GAMING_MODE_MAXIMUM_BATTERY:
            printf("[INFO]  pEGCaps->EGModeCaps.SupportedTypes = CTL_3D_ENDURANCE_GAMING_MODE_MAXIMUM_BATTERY\n");

            break;

        default:
            return;
    }
}

/***************************************************************
 * @brief Method to print 3d feature setting type
 *
 * @param Setting
 * @param SettingType
 * @return
 ***************************************************************/
inline void Print3DFeatureSupportedSettingType(uint32_t Setting, feature_control_type ControlType)
{
    switch (ControlType)
    {
        case ENDURANCE_GAMING_CONTROLS:

            PrintEnduranceGamingControl(Setting);

            break;

        case ENDURANCE_GAMING_MODES:

            PrintEnduranceGamingMode(Setting);

            break;

        default:
            return;
    }
}

/***************************************************************
 * @brief Method to print 3d feature supported settings in a generic way
 *
 * @param SupportMask
 * @param Settings
 * @param SettingType
 * @return
 ***************************************************************/
inline void Print3DFeatureSupportedSettings(uint64_t SupportMask, std::vector<uint32_t> Settings, feature_control_type ControlType)
{
    for (uint8_t i = 0; i < Settings.size(); i++)
    {
        uint32_t Setting = CONTROL_BIT(Settings[i]);

        if (Setting & SupportMask)
        {
            Print3DFeatureSupportedSettingType(Settings[i], ControlType);
        }
    }
}

/***************************************************************
 * @brief Method to print 3d feature details struct in a generic way
 *
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
inline void Print3DFeatureDetail(ctl_3d_feature_details_t *pFeatureDetails)
{
    if (pFeatureDetails)
    {
        printf("[INFO] 3D Feature supported: %s (%d)\n", Get3DFeatureName(pFeatureDetails->FeatureType), pFeatureDetails->FeatureType);
        printf("[INFO] Feature conflict mask flag: 0x%llX\n", pFeatureDetails->ConflictingFeatures);
        printf("[INFO] Supported per application: %d\n", pFeatureDetails->PerAppSupport);
        printf("[INFO] Supports live change: %s\n", (pFeatureDetails->FeatureMiscSupport & CTL_3D_FEATURE_MISC_FLAG_LIVE_CHANGE) ? "Yes" : "No");
        Print3DFeatureSupport(pFeatureDetails);

        switch (pFeatureDetails->ValueType)
        {
            case CTL_PROPERTY_VALUE_TYPE_ENUM:
            {
                printf("[INFO] Default value: %d\n", pFeatureDetails->Value.EnumType.DefaultType);
                if (CTL_3D_FEATURE_GAMING_FLIP_MODES == pFeatureDetails->FeatureType)
                {
                    ctl_gaming_flip_mode_flags_t GamingflipCaps = (ctl_gaming_flip_mode_flags_t)pFeatureDetails->Value.EnumType.SupportedTypes;

                    if (CTL_GAMING_FLIP_MODE_FLAG_APPLICATION_DEFAULT & GamingflipCaps)
                    {
                        printf("[INFO] pFeatureDetails->Value.EnumType.SupportedTypes = CTL_GAMING_FLIP_MODE_FLAG_APPLICATION_DEFAULT\n");
                    }
                    if (CTL_GAMING_FLIP_MODE_FLAG_VSYNC_OFF & GamingflipCaps)
                    {
                        printf("[INFO] pFeatureDetails->Value.EnumType.SupportedTypes = CTL_GAMING_FLIP_MODE_FLAG_VSYNC_OFF\n");
                    }
                    if (CTL_GAMING_FLIP_MODE_FLAG_VSYNC_ON & GamingflipCaps)
                    {
                        printf("[INFO] pFeatureDetails->Value.EnumType.SupportedTypes = CTL_GAMING_FLIP_MODE_FLAG_VSYNC_ON\n");
                    }
                    if (CTL_GAMING_FLIP_MODE_FLAG_SMOOTH_SYNC & GamingflipCaps)
                    {
                        printf("[INFO] pFeatureDetails->Value.EnumType.SupportedTypes = CTL_GAMING_FLIP_MODE_FLAG_SMOOTH_SYNC\n");
                    }
                    if (CTL_GAMING_FLIP_MODE_FLAG_SPEED_FRAME & GamingflipCaps)
                    {
                        printf("[INFO] pFeatureDetails->Value.EnumType.SupportedTypes = CTL_GAMING_FLIP_MODE_FLAG_SPEED_FRAME\n");
                    }
                    if (CTL_GAMING_FLIP_MODE_FLAG_CAPPED_FPS & GamingflipCaps)
                    {
                        printf("[INFO] pFeatureDetails->Value.EnumType.SupportedTypes = CTL_GAMING_FLIP_MODE_FLAG_CAPPED_FPS\n");
                    }
                }
                // printf("  Enabled value: %d\n", pFeatureDetails->Value.EnumType.EnableType);
            }
            break;
            case CTL_PROPERTY_VALUE_TYPE_BOOL:
            {
                printf("[INFO] Default value: %d\n", pFeatureDetails->Value.BoolType.DefaultState);
                // printf("  Enabled value: %d\n", pFeatureDetails->Value.BoolType.Enable);
            }
            break;
            case CTL_PROPERTY_VALUE_TYPE_FLOAT:
            {
                printf("[INFO] Min possible value: %f\n", pFeatureDetails->Value.FloatType.RangeInfo.min_possible_value);
                printf("[INFO] Max possible value: %f\n", pFeatureDetails->Value.FloatType.RangeInfo.max_possible_value);
                printf("[INFO] Step size: %f\n", pFeatureDetails->Value.FloatType.RangeInfo.step_size);
                printf("[INFO] Default value: %f\n", pFeatureDetails->Value.FloatType.RangeInfo.default_value);
                // printf("  Current value: %f\n", pFeatureDetails->Value.FloatType.Value);
            }
            break;
            case CTL_PROPERTY_VALUE_TYPE_INT32:
            {
                printf("[INFO] Min possible value: %d\n", pFeatureDetails->Value.IntType.RangeInfo.min_possible_value);
                printf("[INFO] Max possible value: %d\n", pFeatureDetails->Value.IntType.RangeInfo.max_possible_value);
                printf("[INFO] Step size: %d\n", pFeatureDetails->Value.IntType.RangeInfo.step_size);
                printf("[INFO] Default value: %d\n", pFeatureDetails->Value.IntType.RangeInfo.default_value);
                // printf("  Current value: %d\n", pFeatureDetails->Value.IntType.Value);
            }
            break;
            case CTL_PROPERTY_VALUE_TYPE_CUSTOM:
            {
                if (pFeatureDetails->pCustomValue == NULL)
                {
                    printf("[WARN] Empty custom data\n");
                    break;
                }

                switch (pFeatureDetails->FeatureType)
                {
                    case CTL_3D_FEATURE_ADAPTIVE_SYNC_PLUS:
                    {
                        ctl_adaptivesync_caps_t *pASCaps = (ctl_adaptivesync_caps_t *)pFeatureDetails->pCustomValue;
                        printf("[INFO] AdaptiveBalanceSupported = %d\n", pASCaps->AdaptiveBalanceSupported);
                        printf("[INFO] AdaptiveBalanceStrengthCaps.DefaultEnable = %d\n", pASCaps->AdaptiveBalanceStrengthCaps.DefaultEnable);
                        printf("[INFO] AdaptiveBalanceStrengthCaps.RangeInfo.default_value = %f\n", pASCaps->AdaptiveBalanceStrengthCaps.RangeInfo.default_value);
                        printf("[INFO] AdaptiveBalanceStrengthCaps.RangeInfo.min_possible_value = %f\n", pASCaps->AdaptiveBalanceStrengthCaps.RangeInfo.min_possible_value);
                        printf("[INFO] AdaptiveBalanceStrengthCaps.RangeInfo.max_possible_value = %f\n", pASCaps->AdaptiveBalanceStrengthCaps.RangeInfo.max_possible_value);
                        printf("[INFO] AdaptiveBalanceStrengthCaps.RangeInfo.step_size = %f\n", pASCaps->AdaptiveBalanceStrengthCaps.RangeInfo.step_size);
                    }
                    break;

                    case CTL_3D_FEATURE_ENDURANCE_GAMING:
                    {
                        ctl_endurance_gaming_caps_t *pEGCaps = (ctl_endurance_gaming_caps_t *)pFeatureDetails->pCustomValue;
                        std::vector<uint32_t> EGControls     = { CTL_3D_ENDURANCE_GAMING_CONTROL_TURN_OFF, CTL_3D_ENDURANCE_GAMING_CONTROL_TURN_ON, CTL_3D_ENDURANCE_GAMING_CONTROL_AUTO };
                        std::vector<uint32_t> EGModes        = { CTL_3D_ENDURANCE_GAMING_MODE_BETTER_PERFORMANCE, CTL_3D_ENDURANCE_GAMING_MODE_BALANCED, CTL_3D_ENDURANCE_GAMING_MODE_MAXIMUM_BATTERY };
                        printf("[INFO] pEGCaps->EGControlCaps.DefaultType = %d\n", pEGCaps->EGControlCaps.DefaultType);
                        printf("[INFO] pEGCaps->EGModeCaps.DefaultType = %d\n", pEGCaps->EGModeCaps.DefaultType);

                        Print3DFeatureSupportedSettings(pEGCaps->EGControlCaps.SupportedTypes, move(EGControls), ENDURANCE_GAMING_CONTROLS);
                        Print3DFeatureSupportedSettings(pEGCaps->EGModeCaps.SupportedTypes, move(EGModes), ENDURANCE_GAMING_MODES);
                    }
                    break;

                    case CTL_3D_FEATURE_APP_PROFILES:
                    {
                        ctl_3d_app_profiles_caps_t *pCaps = (ctl_3d_app_profiles_caps_t *)pFeatureDetails->pCustomValue;
                        printf("[INFO] pCaps->SupportedTierTypes = %s\n", GetProfileTypeName(pCaps->SupportedTierTypes));
                    }
                    break;

                    default:
                    {
                        printf("[ERROR]  Unknown feature type with custom capabilities! ERROR!!\n");
                    }
                    break;
                }
            }
            break;

            default:
                return;
        }
    }
}
