//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  Media_Sample_APP.cpp
 * @brief This file contains the 'main' function. Program execution begins and ends there.
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
#define STORE_RESET_ERROR(Result)     \
    if (Result != CTL_RESULT_SUCCESS) \
        GResult = Result;             \
    Result = CTL_RESULT_SUCCESS;

/***************************************************************
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
char *GetVideoProcessingFeatureName(ctl_video_processing_feature_t FeatureType)
{
    switch (FeatureType)
    {
        case CTL_VIDEO_PROCESSING_FEATURE_FILM_MODE_DETECTION:
            return "Film mode detection";
        case CTL_VIDEO_PROCESSING_FEATURE_NOISE_REDUCTION:
            return "Noise reduction";
        case CTL_VIDEO_PROCESSING_FEATURE_SHARPNESS:
            return "Sharpness";
        case CTL_VIDEO_PROCESSING_FEATURE_ADAPTIVE_CONTRAST_ENHANCEMENT:
            return "Adaptive contrast enhancement";
        case CTL_VIDEO_PROCESSING_FEATURE_STANDARD_COLOR_CORRECTION:
            return "Standard color correction";
        case CTL_VIDEO_PROCESSING_FEATURE_TOTAL_COLOR_CORRECTION:
            return "Total color correction";
        case CTL_VIDEO_PROCESSING_FEATURE_SKIN_TONE_ENHANCEMENT:
            return "Skin tone enhancement";
        default:
            return "No name";
    }
}

/****************************************************************************
 * @brief Print Video Processing feature details
 *
 * @details
 *     - Video Processing feature details
 *
 * @returns
 *     -
 ****************************************************************************/
void PrintVideoProcessingFeatureDetail(ctl_video_processing_feature_getset_t *pFeatureDetails)
{
    if (pFeatureDetails)
    {
        printf("Video Processing Feature supported: %s (%d)\n", GetVideoProcessingFeatureName(pFeatureDetails->FeatureType), pFeatureDetails->FeatureType);

        switch (pFeatureDetails->ValueType)
        {
            case CTL_PROPERTY_VALUE_TYPE_ENUM:
            {
                printf("  Enabled value: %d\n", pFeatureDetails->Value.EnumType.EnableType);
            }
            break;
            case CTL_PROPERTY_VALUE_TYPE_BOOL:
            {
                printf("  Enabled value: %d\n", pFeatureDetails->Value.BoolType.Enable);
            }
            break;
            case CTL_PROPERTY_VALUE_TYPE_FLOAT:
            {
                printf("  Enabled: %d, Value: %f\n", pFeatureDetails->Value.FloatType.Enable, pFeatureDetails->Value.FloatType.Value);
            }
            break;
            case CTL_PROPERTY_VALUE_TYPE_INT32:
            {
                printf("  Enabled: %d, Value: %d\n", pFeatureDetails->Value.IntType.Enable, pFeatureDetails->Value.IntType.Value);
            }
            break;
            case CTL_PROPERTY_VALUE_TYPE_UINT32:
            {
                printf("  Enabled: %d, Value: %d\n", pFeatureDetails->Value.UIntType.Enable, pFeatureDetails->Value.UIntType.Value);
            }
            break;
            case CTL_PROPERTY_VALUE_TYPE_CUSTOM:
            {
            }
            break;

            default:
                return;
        }
    }
}

/***************************************************************************
 * @brief Control FMD feature
 *
 * @details
 *     - Control FMD features
 *
 * @returns
 *     - ::CTL_RESULT_SUCCESS
 *     - ::CTL_RESULT_ERROR_UNINITIALIZED
 *     - ::CTL_RESULT_ERROR_DEVICE_LOST
 *     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
 *         + `nullptr == pInitDesc`
 *         + `nullptr == phAPIHandle`
 *     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
 **************************************************************************/
ctl_result_t ControlFMDFeature(ctl_device_adapter_handle_t hDAhandle, ctl_video_processing_feature_details_t *pFeatureVideoProcDetails)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if (NULL == pFeatureVideoProcDetails)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    printf("media FMD supported\n");

    ctl_video_processing_feature_getset_t FeatureSetDetails = { 0 };
    ctl_video_processing_feature_getset_t FeatureGetDetails = { 0 };
    FeatureSetDetails.Size                                  = sizeof(ctl_video_processing_feature_getset_t);
    FeatureGetDetails.Size                                  = sizeof(ctl_video_processing_feature_getset_t);
    FeatureSetDetails.FeatureType                           = CTL_VIDEO_PROCESSING_FEATURE_FILM_MODE_DETECTION;
    FeatureSetDetails.bSet                                  = true;
    FeatureSetDetails.ValueType                             = CTL_PROPERTY_VALUE_TYPE_BOOL;
    FeatureSetDetails.Value.BoolType.Enable                 = true;

    Result = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureSetDetails);
    if (Result == CTL_RESULT_SUCCESS)
    {
        printf("ctlSetVideoProcessingFeatureDetails\n");
        PrintVideoProcessingFeatureDetail(&FeatureSetDetails);
    }

    FeatureGetDetails.FeatureType = CTL_VIDEO_PROCESSING_FEATURE_FILM_MODE_DETECTION;
    FeatureGetDetails.bSet        = false;
    FeatureGetDetails.ValueType   = CTL_PROPERTY_VALUE_TYPE_BOOL;
    Result                        = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureGetDetails);
    if (Result == CTL_RESULT_SUCCESS)
    {
        printf("ctlGetVideoProcessingFeatureDetails\n");
        PrintVideoProcessingFeatureDetail(&FeatureGetDetails);
    }
    return Result;
}

/***************************************************************************
 * @brief Control DN feature
 *
 * @details
 *     - Control DN features
 *
 * @returns
 *     - ::CTL_RESULT_SUCCESS
 *     - ::CTL_RESULT_ERROR_UNINITIALIZED
 *     - ::CTL_RESULT_ERROR_DEVICE_LOST
 *     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
 *         + `nullptr == pInitDesc`
 *         + `nullptr == phAPIHandle`
 *     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
 **************************************************************************/
ctl_result_t ControlDNFeature(ctl_device_adapter_handle_t hDAhandle, ctl_video_processing_feature_details_t *pFeatureVideoProcDetails)
{
    ctl_result_t Result          = CTL_RESULT_SUCCESS;
    uint32_t NoiseReductionLevel = 52;

    ctl_video_processing_feature_caps_t NoiseCaps           = { 0 };
    ctl_video_processing_feature_details_t NoiseCapsDetails = { 0 };
    ctl_video_processing_feature_getset_t FeatureSetDetails = { 0 };
    ctl_video_processing_feature_getset_t FeatureGetDetails = { 0 };

    NoiseCaps.Size         = sizeof(ctl_video_processing_feature_caps_t);
    NoiseCapsDetails.Size  = sizeof(ctl_video_processing_feature_details_t);
    FeatureSetDetails.Size = sizeof(ctl_video_processing_feature_getset_t);
    FeatureGetDetails.Size = sizeof(ctl_video_processing_feature_getset_t);

    if (NULL == pFeatureVideoProcDetails)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    NoiseCapsDetails.FeatureType  = CTL_VIDEO_PROCESSING_FEATURE_NOISE_REDUCTION;
    NoiseCapsDetails.pCustomValue = (void *)malloc(sizeof(ctl_video_processing_noise_reduction_info_t));
    if (NULL == NoiseCapsDetails.pCustomValue)
    {
        Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
        goto free_exit;
    }

    NoiseCapsDetails.CustomValueSize = sizeof(ctl_video_processing_noise_reduction_info_t);

    NoiseCaps.NumSupportedFeatures = 1;
    NoiseCaps.pFeatureDetails      = &NoiseCapsDetails;
    // Query Denoise Specific Caps
    Result = ctlGetSupportedVideoProcessingCapabilities(hDAhandle, &NoiseCaps);
    if (Result == CTL_RESULT_SUCCESS)
    {
        FeatureSetDetails.FeatureType     = CTL_VIDEO_PROCESSING_FEATURE_NOISE_REDUCTION;
        FeatureSetDetails.bSet            = true;
        FeatureSetDetails.ValueType       = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
        FeatureSetDetails.CustomValueSize = sizeof(ctl_video_processing_noise_reduction_t);
        FeatureSetDetails.pCustomValue    = malloc(FeatureSetDetails.CustomValueSize);
        if (FeatureSetDetails.pCustomValue == NULL)
        {
            Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
            goto free_exit;
        }

        ctl_video_processing_noise_reduction_t *pCustomValueSet = (ctl_video_processing_noise_reduction_t *)FeatureSetDetails.pCustomValue;
        pCustomValueSet->noise_reduction.Enable                 = true;
        // Check if the setting is in the range of [min, max].
        ctl_video_processing_noise_reduction_info_t *pNoiseReductionInfo = (ctl_video_processing_noise_reduction_info_t *)NoiseCapsDetails.pCustomValue;
        if ((NoiseReductionLevel <= pNoiseReductionInfo->noise_reduction.RangeInfo.max_possible_value) && (NoiseReductionLevel >= pNoiseReductionInfo->noise_reduction.RangeInfo.min_possible_value))
        {
            pCustomValueSet->noise_reduction.Value = NoiseReductionLevel;
        }
        else
        {
            pCustomValueSet->noise_reduction.Value = pNoiseReductionInfo->noise_reduction.RangeInfo.default_value;
        }

        pCustomValueSet->noise_reduction_auto_detect.Enable = false;

        Result = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureSetDetails);
        if (Result == CTL_RESULT_SUCCESS)
        {
            printf("ctlSetVideoProcessingFeatureDetails\n");
            PrintVideoProcessingFeatureDetail(&FeatureSetDetails);
        }

        FeatureGetDetails.FeatureType     = CTL_VIDEO_PROCESSING_FEATURE_NOISE_REDUCTION;
        FeatureGetDetails.bSet            = false;
        FeatureGetDetails.ValueType       = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
        FeatureGetDetails.CustomValueSize = sizeof(ctl_video_processing_noise_reduction_t);
        FeatureGetDetails.pCustomValue    = malloc(FeatureGetDetails.CustomValueSize);
        if (FeatureGetDetails.pCustomValue == NULL)
        {
            Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
            goto free_exit;
        }
        Result = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureGetDetails);
        if (Result == CTL_RESULT_SUCCESS)
        {
            printf("ctlGetVideoProcessingFeatureDetails\n");
            PrintVideoProcessingFeatureDetail(&FeatureGetDetails);
        }
    }

free_exit:
    if (NoiseCapsDetails.pCustomValue)
    {
        free(NoiseCapsDetails.pCustomValue);
        NoiseCapsDetails.pCustomValue = NULL;
    }

    if (FeatureSetDetails.pCustomValue)
    {
        free(FeatureSetDetails.pCustomValue);
        FeatureSetDetails.pCustomValue = NULL;
    }

    if (FeatureGetDetails.pCustomValue != NULL)
    {
        free(FeatureGetDetails.pCustomValue);
        FeatureGetDetails.pCustomValue = NULL;
    }

    return Result;
}

/***************************************************************************
 * @brief Control Sharpness feature
 *
 * @details
 *     - Control Sharpness feature
 *
 * @returns
 *     - ::CTL_RESULT_SUCCESS
 *     - ::CTL_RESULT_ERROR_UNINITIALIZED
 *     - ::CTL_RESULT_ERROR_DEVICE_LOST
 *     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
 *         + `nullptr == pInitDesc`
 *         + `nullptr == phAPIHandle`
 *     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
 **************************************************************************/
ctl_result_t ControlSharpnessFeature(ctl_device_adapter_handle_t hDAhandle, ctl_video_processing_feature_details_t *pFeatureVideoProcDetails)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if (NULL == pFeatureVideoProcDetails)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    printf("media Sharpness supported\n");

    ctl_video_processing_feature_getset_t FeatureSetDetails = { 0 };
    ctl_video_processing_feature_getset_t FeatureGetDetails = { 0 };
    FeatureSetDetails.Size                                  = sizeof(ctl_video_processing_feature_getset_t);
    FeatureGetDetails.Size                                  = sizeof(ctl_video_processing_feature_getset_t);
    FeatureSetDetails.FeatureType                           = CTL_VIDEO_PROCESSING_FEATURE_SHARPNESS;
    FeatureSetDetails.bSet                                  = true;
    FeatureSetDetails.ValueType                             = CTL_PROPERTY_VALUE_TYPE_UINT32;
    FeatureSetDetails.Value.UIntType.Enable                 = true;
    FeatureSetDetails.Value.UIntType.Value                  = 44;

    Result = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureSetDetails);
    if (Result == CTL_RESULT_SUCCESS)
    {
        printf("ctlSetVideoProcessingFeatureDetails\n");
        PrintVideoProcessingFeatureDetail(&FeatureSetDetails);
    }

    FeatureGetDetails.FeatureType = CTL_VIDEO_PROCESSING_FEATURE_SHARPNESS;
    FeatureGetDetails.bSet        = false;
    FeatureGetDetails.ValueType   = CTL_PROPERTY_VALUE_TYPE_UINT32;
    Result                        = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureGetDetails);
    if (Result == CTL_RESULT_SUCCESS)
    {
        printf("ctlGetVideoProcessingFeatureDetails\n");
        PrintVideoProcessingFeatureDetail(&FeatureGetDetails);
    }
    return Result;
}

/***************************************************************************
 * @brief Control ACE feature
 *
 * @details
 *     - Control ACE feature
 *
 * @returns
 *     - ::CTL_RESULT_SUCCESS
 *     - ::CTL_RESULT_ERROR_UNINITIALIZED
 *     - ::CTL_RESULT_ERROR_DEVICE_LOST
 *     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
 *         + `nullptr == pInitDesc`
 *         + `nullptr == phAPIHandle`
 *     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
 **************************************************************************/
ctl_result_t ControlACEFeature(ctl_device_adapter_handle_t hDAhandle, ctl_video_processing_feature_details_t *pFeatureVideoProcDetails)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if (NULL == pFeatureVideoProcDetails)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    printf("media ACE supported\n");

    // Get Ace feature details which is capable of supporting
    ctl_video_processing_feature_caps_t AceCaps             = { 0 };
    ctl_video_processing_feature_details_t AceCapsDetails   = { 0 };
    ctl_video_processing_feature_getset_t FeatureSetDetails = { 0 };
    ctl_video_processing_feature_getset_t FeatureGetDetails = { 0 };

    ctl_video_processing_adaptive_contrast_enhancement_info_t *pAceInfo = nullptr;

    AceCapsDetails.FeatureType     = CTL_VIDEO_PROCESSING_FEATURE_ADAPTIVE_CONTRAST_ENHANCEMENT;
    AceCapsDetails.CustomValueSize = sizeof(ctl_video_processing_adaptive_contrast_enhancement_info_t);
    AceCapsDetails.pCustomValue    = (void *)calloc(1, sizeof(ctl_video_processing_adaptive_contrast_enhancement_info_t));
    if (NULL == AceCapsDetails.pCustomValue)
    {
        Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
        goto free_exit;
    }

    AceCaps.Size                 = sizeof(ctl_video_processing_feature_caps_t);
    AceCaps.NumSupportedFeatures = 1;
    AceCaps.pFeatureDetails      = &AceCapsDetails;
    Result                       = ctlGetSupportedVideoProcessingCapabilities(hDAhandle, &AceCaps);
    if (Result != CTL_RESULT_SUCCESS)
    {
        printf("Media ACE ctlGetSupportedVideoProcessingCapabilities return failure!\n");
        goto free_exit;
    }
    pAceInfo = (ctl_video_processing_adaptive_contrast_enhancement_info_t *)AceCapsDetails.pCustomValue;
    printf("ctlVideoProcessingFeatureDetails: adaptive_contrast_enhancement_coexistence_supported %d\n", pAceInfo->adaptive_contrast_enhancement_coexistence_supported);

    FeatureSetDetails.Size            = sizeof(ctl_video_processing_feature_getset_t);
    FeatureGetDetails.Size            = sizeof(ctl_video_processing_feature_getset_t);
    FeatureSetDetails.FeatureType     = CTL_VIDEO_PROCESSING_FEATURE_ADAPTIVE_CONTRAST_ENHANCEMENT;
    FeatureSetDetails.bSet            = true;
    FeatureSetDetails.ValueType       = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
    FeatureSetDetails.CustomValueSize = sizeof(ctl_video_processing_adaptive_contrast_enhancement_t);
    FeatureSetDetails.pCustomValue    = calloc(1, FeatureSetDetails.CustomValueSize);
    if (FeatureSetDetails.pCustomValue == NULL)
    {
        Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
        goto free_exit;
    }
    ctl_video_processing_adaptive_contrast_enhancement_t *pCustomValueSet = (ctl_video_processing_adaptive_contrast_enhancement_t *)FeatureSetDetails.pCustomValue;
    pCustomValueSet->adaptive_contrast_enhancement.Enable                 = true;
    pCustomValueSet->adaptive_contrast_enhancement.Value                  = 5;
    pCustomValueSet->adaptive_contrast_enhancement_coexistence.Enable     = true;
    Result                                                                = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureSetDetails);
    if (Result == CTL_RESULT_SUCCESS)
    {
        printf("ctlSetVideoProcessingFeatureDetails\n");
        PrintVideoProcessingFeatureDetail(&FeatureSetDetails);
    }

    FeatureGetDetails.FeatureType     = CTL_VIDEO_PROCESSING_FEATURE_ADAPTIVE_CONTRAST_ENHANCEMENT;
    FeatureGetDetails.bSet            = false;
    FeatureGetDetails.ValueType       = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
    FeatureGetDetails.CustomValueSize = sizeof(ctl_video_processing_adaptive_contrast_enhancement_t);
    FeatureGetDetails.pCustomValue    = calloc(1, FeatureGetDetails.CustomValueSize);
    if (FeatureGetDetails.pCustomValue == NULL)
    {
        Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
        goto free_exit;
    }
    Result = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureGetDetails);
    if (Result == CTL_RESULT_SUCCESS)
    {
        printf("ctlGetVideoProcessingFeatureDetails\n");
        PrintVideoProcessingFeatureDetail(&FeatureGetDetails);
    }

free_exit:
    if (AceCapsDetails.pCustomValue != NULL)
    {
        free(AceCapsDetails.pCustomValue);
        AceCapsDetails.pCustomValue = NULL;
    }

    if (FeatureSetDetails.pCustomValue != NULL)
    {
        free(FeatureSetDetails.pCustomValue);
        FeatureSetDetails.pCustomValue = NULL;
    }

    if (FeatureGetDetails.pCustomValue != NULL)
    {
        free(FeatureGetDetails.pCustomValue);
        FeatureGetDetails.pCustomValue = NULL;
    }
    return Result;
}

/***************************************************************************
 * @brief Control SCC feature
 *
 * @details
 *     - Control SCC features
 *
 * @returns
 *     - ::CTL_RESULT_SUCCESS
 *     - ::CTL_RESULT_ERROR_UNINITIALIZED
 *     - ::CTL_RESULT_ERROR_DEVICE_LOST
 *     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
 *         + `nullptr == pInitDesc`
 *         + `nullptr == phAPIHandle`
 *     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
 **************************************************************************/
ctl_result_t ControlSCCFeature(ctl_device_adapter_handle_t hDAhandle, ctl_video_processing_feature_details_t *pFeatureVideoProcDetails)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    ctl_video_processing_feature_caps_t SCCCaps             = { 0 };
    ctl_video_processing_feature_details_t SCCCapsDetails   = { 0 };
    ctl_video_processing_feature_getset_t FeatureSetDetails = { 0 };
    ctl_video_processing_feature_getset_t FeatureGetDetails = { 0 };

    FeatureSetDetails.Size = sizeof(ctl_video_processing_feature_getset_t);
    FeatureGetDetails.Size = sizeof(ctl_video_processing_feature_getset_t);
    SCCCapsDetails.Size    = sizeof(ctl_video_processing_feature_details_t);
    SCCCaps.Size           = sizeof(ctl_video_processing_feature_caps_t);

    // Set Value
    float Brightness = 50;
    float Contrast   = 5;
    float Hue        = 50;
    float Saturation = 5;

    if (NULL == pFeatureVideoProcDetails)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    // Caps query for Standard Color Control
    SCCCapsDetails.FeatureType  = CTL_VIDEO_PROCESSING_FEATURE_STANDARD_COLOR_CORRECTION;
    SCCCapsDetails.pCustomValue = (void *)malloc(sizeof(ctl_video_processing_standard_color_correction_info_t));
    if (NULL == SCCCapsDetails.pCustomValue)
    {
        Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
        goto free_exit;
    }

    SCCCapsDetails.CustomValueSize = sizeof(ctl_video_processing_standard_color_correction_info_t);

    SCCCaps.NumSupportedFeatures = 1;
    SCCCaps.pFeatureDetails      = &SCCCapsDetails;
    // Query SCC Specific Caps
    Result = ctlGetSupportedVideoProcessingCapabilities(hDAhandle, &SCCCaps);

    if (Result == CTL_RESULT_SUCCESS)
    {
        FeatureSetDetails.FeatureType     = CTL_VIDEO_PROCESSING_FEATURE_STANDARD_COLOR_CORRECTION;
        FeatureSetDetails.bSet            = true;
        FeatureSetDetails.ValueType       = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
        FeatureSetDetails.CustomValueSize = sizeof(ctl_video_processing_standard_color_correction_t);
        FeatureSetDetails.pCustomValue    = malloc(FeatureSetDetails.CustomValueSize);
        if (FeatureSetDetails.pCustomValue == NULL)
        {
            Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
            goto free_exit;
        }

        ctl_video_processing_standard_color_correction_t *pCustomValueSet = (ctl_video_processing_standard_color_correction_t *)FeatureSetDetails.pCustomValue;
        ctl_video_processing_standard_color_correction_info_t *pCaps      = (ctl_video_processing_standard_color_correction_info_t *)SCCCapsDetails.pCustomValue;
        pCustomValueSet->standard_color_correction_enable                 = true;
        // Check if Brightness is in the range of [min, max].
        if (Brightness <= pCaps->brightness.RangeInfo.max_possible_value && Brightness >= pCaps->brightness.RangeInfo.min_possible_value)
        {
            pCustomValueSet->brightness = Brightness;
        }
        else
        {
            pCustomValueSet->brightness = pCaps->brightness.RangeInfo.default_value;
        }
        // Check if Contrast is in the range of [min, max].
        if (Contrast <= pCaps->contrast.RangeInfo.max_possible_value && Contrast >= pCaps->contrast.RangeInfo.min_possible_value)
        {
            pCustomValueSet->contrast = Contrast;
        }
        else
        {
            pCustomValueSet->contrast = pCaps->contrast.RangeInfo.default_value;
        }
        // Check if Hue is in the range of [min, max].
        if (Hue <= pCaps->hue.RangeInfo.max_possible_value && Hue >= pCaps->hue.RangeInfo.min_possible_value)
        {
            pCustomValueSet->hue = Hue;
        }
        else
        {
            pCustomValueSet->hue = pCaps->hue.RangeInfo.default_value;
        }
        // Check if Saturation is in the range of [min, max].
        if (Saturation <= pCaps->saturation.RangeInfo.max_possible_value && Saturation >= pCaps->saturation.RangeInfo.min_possible_value)
        {
            pCustomValueSet->saturation = Saturation;
        }
        else
        {
            pCustomValueSet->saturation = pCaps->saturation.RangeInfo.default_value;
        }
        // Set SCC Feature
        Result = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureSetDetails);
        if (Result == CTL_RESULT_SUCCESS)
        {
            printf("ctlSetVideoProcessingFeatureDetails\n");
            PrintVideoProcessingFeatureDetail(&FeatureSetDetails);
        }
        // Get SCC value
        FeatureGetDetails.FeatureType     = CTL_VIDEO_PROCESSING_FEATURE_STANDARD_COLOR_CORRECTION;
        FeatureGetDetails.bSet            = false;
        FeatureGetDetails.ValueType       = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
        FeatureGetDetails.CustomValueSize = sizeof(ctl_video_processing_standard_color_correction_t);
        FeatureGetDetails.pCustomValue    = malloc(FeatureGetDetails.CustomValueSize);
        if (FeatureGetDetails.pCustomValue == NULL)
        {
            Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
            goto free_exit;
        }
        Result = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureGetDetails);
        if (Result == CTL_RESULT_SUCCESS)
        {
            printf("ctlGetVideoProcessingFeatureDetails\n");
            PrintVideoProcessingFeatureDetail(&FeatureGetDetails);
        }
    }

free_exit:
    if (SCCCapsDetails.pCustomValue)
    {
        free(SCCCapsDetails.pCustomValue);
        SCCCapsDetails.pCustomValue = NULL;
    }

    if (FeatureSetDetails.pCustomValue)
    {
        free(FeatureSetDetails.pCustomValue);
        FeatureSetDetails.pCustomValue = NULL;
    }

    if (FeatureGetDetails.pCustomValue != NULL)
    {
        free(FeatureGetDetails.pCustomValue);
        FeatureGetDetails.pCustomValue = NULL;
    }

    return Result;
}

/***************************************************************************
 * @brief Control TCC feature
 *
 * @details
 *     - Control TCC features
 *
 * @returns
 *     - ::CTL_RESULT_SUCCESS
 *     - ::CTL_RESULT_ERROR_UNINITIALIZED
 *     - ::CTL_RESULT_ERROR_DEVICE_LOST
 *     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
 *         + `nullptr == pInitDesc`
 *         + `nullptr == phAPIHandle`
 *     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
 **************************************************************************/
ctl_result_t ControlTCCFeature(ctl_device_adapter_handle_t hDAhandle, ctl_video_processing_feature_details_t *pFeatureVideoProcDetails)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    ctl_video_processing_feature_caps_t TCCCaps             = { 0 };
    ctl_video_processing_feature_details_t TCCCapsDetails   = { 0 };
    ctl_video_processing_feature_getset_t FeatureSetDetails = { 0 };
    ctl_video_processing_feature_getset_t FeatureGetDetails = { 0 };

    FeatureSetDetails.Size = sizeof(ctl_video_processing_feature_getset_t);
    FeatureGetDetails.Size = sizeof(ctl_video_processing_feature_getset_t);
    TCCCapsDetails.Size    = sizeof(ctl_video_processing_feature_details_t);
    TCCCaps.Size           = sizeof(ctl_video_processing_feature_caps_t);

    uint32_t SettingValue = 160;

    if (NULL == pFeatureVideoProcDetails)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    TCCCapsDetails.FeatureType  = CTL_VIDEO_PROCESSING_FEATURE_TOTAL_COLOR_CORRECTION;
    TCCCapsDetails.pCustomValue = (void *)malloc(sizeof(ctl_video_processing_total_color_correction_info_t));
    if (NULL == TCCCapsDetails.pCustomValue)
    {
        Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
        goto free_exit;
    }

    TCCCapsDetails.CustomValueSize = sizeof(ctl_video_processing_total_color_correction_info_t);

    TCCCaps.NumSupportedFeatures = 1;
    TCCCaps.pFeatureDetails      = &TCCCapsDetails;
    // Query TCC Specific Caps
    Result = ctlGetSupportedVideoProcessingCapabilities(hDAhandle, &TCCCaps);
    if (Result != CTL_RESULT_SUCCESS)
    {
        goto free_exit;
    }

    FeatureSetDetails.FeatureType     = CTL_VIDEO_PROCESSING_FEATURE_TOTAL_COLOR_CORRECTION;
    FeatureSetDetails.bSet            = true;
    FeatureSetDetails.ValueType       = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
    FeatureSetDetails.CustomValueSize = sizeof(ctl_video_processing_total_color_correction_t);
    FeatureSetDetails.pCustomValue    = malloc(FeatureSetDetails.CustomValueSize);
    if (FeatureSetDetails.pCustomValue == NULL)
    {
        Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
        goto free_exit;
    }

    ctl_video_processing_total_color_correction_info_t *pCapsCustomValue = (ctl_video_processing_total_color_correction_info_t *)TCCCapsDetails.pCustomValue;
    ctl_video_processing_total_color_correction_t *pCustomValueSet       = (ctl_video_processing_total_color_correction_t *)FeatureSetDetails.pCustomValue;
    pCustomValueSet->total_color_correction_enable                       = true;
    if (SettingValue > pCapsCustomValue->blue.RangeInfo.max_possible_value && SettingValue < pCapsCustomValue->blue.RangeInfo.min_possible_value)
    {
        SettingValue = pCapsCustomValue->blue.RangeInfo.default_value;
    }

    pCustomValueSet->blue    = SettingValue;
    pCustomValueSet->cyan    = SettingValue;
    pCustomValueSet->green   = SettingValue;
    pCustomValueSet->red     = SettingValue;
    pCustomValueSet->yellow  = SettingValue;
    pCustomValueSet->magenta = SettingValue;

    Result = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureSetDetails);
    if (Result == CTL_RESULT_SUCCESS)
    {
        printf("ctlSetVideoProcessingFeatureDetails\n");
        PrintVideoProcessingFeatureDetail(&FeatureSetDetails);
    }

    FeatureGetDetails.FeatureType     = CTL_VIDEO_PROCESSING_FEATURE_TOTAL_COLOR_CORRECTION;
    FeatureGetDetails.bSet            = false;
    FeatureGetDetails.ValueType       = CTL_PROPERTY_VALUE_TYPE_CUSTOM;
    FeatureGetDetails.CustomValueSize = sizeof(ctl_video_processing_total_color_correction_t);
    FeatureGetDetails.pCustomValue    = malloc(FeatureGetDetails.CustomValueSize);
    if (FeatureGetDetails.pCustomValue == NULL)
    {
        Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
        goto free_exit;
    }
    Result = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureGetDetails);
    if (Result == CTL_RESULT_SUCCESS)
    {
        printf("ctlGetVideoProcessingFeatureDetails\n");
        PrintVideoProcessingFeatureDetail(&FeatureGetDetails);
    }

free_exit:
    if (FeatureSetDetails.pCustomValue != NULL)
    {
        free(FeatureSetDetails.pCustomValue);
        FeatureSetDetails.pCustomValue = NULL;
    }
    if (FeatureGetDetails.pCustomValue != NULL)
    {
        free(FeatureGetDetails.pCustomValue);
        FeatureGetDetails.pCustomValue = NULL;
    }
    if (TCCCapsDetails.pCustomValue != NULL)
    {
        free(TCCCapsDetails.pCustomValue);
        TCCCapsDetails.pCustomValue = NULL;
    }
    return Result;
}

/***************************************************************************
 * @brief Control STE feature
 *
 * @details
 *     - Control STE features
 *
 * @returns
 *     - ::CTL_RESULT_SUCCESS
 *     - ::CTL_RESULT_ERROR_UNINITIALIZED
 *     - ::CTL_RESULT_ERROR_DEVICE_LOST
 *     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
 *         + `nullptr == pInitDesc`
 *         + `nullptr == phAPIHandle`
 *     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
 **************************************************************************/
ctl_result_t ControlSTEFeature(ctl_device_adapter_handle_t hDAhandle, ctl_video_processing_feature_details_t *pFeatureVideoProcDetails)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if (NULL == pFeatureVideoProcDetails)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    printf("media STE supported\n");

    ctl_video_processing_feature_getset_t FeatureSetDetails = { 0 };
    ctl_video_processing_feature_getset_t FeatureGetDetails = { 0 };
    FeatureSetDetails.Size                                  = sizeof(ctl_video_processing_feature_getset_t);
    FeatureGetDetails.Size                                  = sizeof(ctl_video_processing_feature_getset_t);
    FeatureSetDetails.FeatureType                           = CTL_VIDEO_PROCESSING_FEATURE_SKIN_TONE_ENHANCEMENT;
    FeatureSetDetails.bSet                                  = true;
    FeatureSetDetails.ValueType                             = CTL_PROPERTY_VALUE_TYPE_UINT32;
    FeatureSetDetails.Value.UIntType.Enable                 = true;
    FeatureSetDetails.Value.UIntType.Value                  = 3;

    Result = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureSetDetails);
    if (Result == CTL_RESULT_SUCCESS)
    {
        printf("ctlSetVideoProcessingFeatureDetails\n");
        PrintVideoProcessingFeatureDetail(&FeatureSetDetails);
    }

    FeatureGetDetails.FeatureType = CTL_VIDEO_PROCESSING_FEATURE_SKIN_TONE_ENHANCEMENT;
    FeatureGetDetails.bSet        = false;
    FeatureGetDetails.ValueType   = CTL_PROPERTY_VALUE_TYPE_UINT32;
    Result                        = ctlGetSetVideoProcessingFeature(hDAhandle, &FeatureGetDetails);
    if (Result == CTL_RESULT_SUCCESS)
    {
        printf("ctlGetVideoProcessingFeatureDetails\n");
        PrintVideoProcessingFeatureDetail(&FeatureGetDetails);
    }
    return Result;
}
/***********************************************************************
 * @brief Control Media features
 *
 * @details
 *     - Control Media features
 *
 * @returns
 *     - ::CTL_RESULT_SUCCESS
 *     - ::CTL_RESULT_ERROR_UNINITIALIZED
 *     - ::CTL_RESULT_ERROR_DEVICE_LOST
 *     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
 *         + `nullptr == pInitDesc`
 *         + `nullptr == phAPIHandle`
 *     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
 *************************************************************************/
ctl_result_t ControlMediaFeatures(ctl_device_adapter_handle_t hDAhandle)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    printf("media subfunction supported\n");

    ctl_video_processing_feature_caps_t FeatureCapsVideoProc = { 0 };

    FeatureCapsVideoProc.Size = sizeof(ctl_video_processing_feature_caps_t);
    // If FeatureCapsVideoProc.pSupportedFeatures is null pointer, API will return the number of supported features.
    Result = ctlGetSupportedVideoProcessingCapabilities(hDAhandle, &FeatureCapsVideoProc);
    if (Result != CTL_RESULT_SUCCESS)
    {
        printf("ctlGetSupportedVideoProcessingCapabilities returned failure code: 0x%X\n", Result);
        goto Free_exit;
    }

    printf("ctlGetSupportedVideoProcessingCapabilities returned success\n");
    FeatureCapsVideoProc.pFeatureDetails = (ctl_video_processing_feature_details_t *)calloc(FeatureCapsVideoProc.NumSupportedFeatures, sizeof(ctl_video_processing_feature_details_t));

    if (FeatureCapsVideoProc.pFeatureDetails)
    {
        // If FeatureCapsVideoProc.pSupportedFeatures is not null pointer, API will return supported features.
        Result = ctlGetSupportedVideoProcessingCapabilities(hDAhandle, &FeatureCapsVideoProc);
        if (Result == CTL_RESULT_SUCCESS)
        {
            for (uint32_t i = 0; i < FeatureCapsVideoProc.NumSupportedFeatures; i++)
            {
                // Get more feature details of the capability such as mode, max/min value
                ctl_video_processing_feature_details_t *pFeatureVideoProcDetails = FeatureCapsVideoProc.pFeatureDetails + i;

                switch (pFeatureVideoProcDetails->FeatureType)
                {
                    case CTL_VIDEO_PROCESSING_FEATURE_FILM_MODE_DETECTION:
                        // Film Mode Detection
                        // FMD mode is Bool
                        if (pFeatureVideoProcDetails->ValueType == CTL_PROPERTY_VALUE_TYPE_BOOL)
                        {
                            Result = ControlFMDFeature(hDAhandle, pFeatureVideoProcDetails);
                            printf("ControlMediaFeatures FMD returned code: 0x%X\n", Result);
                        }
                        break;
                    case CTL_VIDEO_PROCESSING_FEATURE_NOISE_REDUCTION:
                        // Noise reduction(DN)
                        // DN mode is Custom
                        if (pFeatureVideoProcDetails->ValueType == CTL_PROPERTY_VALUE_TYPE_CUSTOM)
                        {
                            Result = ControlDNFeature(hDAhandle, pFeatureVideoProcDetails);
                            printf("ControlMediaFeatures DN returned code: 0x%X\n", Result);
                        }
                        break;
                    case CTL_VIDEO_PROCESSING_FEATURE_SHARPNESS:
                        // Sharpness
                        // Sharpness mode is UInt32
                        if (pFeatureVideoProcDetails->ValueType == CTL_PROPERTY_VALUE_TYPE_UINT32)
                        {
                            Result = ControlSharpnessFeature(hDAhandle, pFeatureVideoProcDetails);
                            printf("ControlMediaFeatures Sharpness returned code: 0x%X\n", Result);
                        }
                        break;
                    case CTL_VIDEO_PROCESSING_FEATURE_ADAPTIVE_CONTRAST_ENHANCEMENT:
                        // Adaptive contrast enhancement(ACE)
                        // ACE mode is Custom
                        if (pFeatureVideoProcDetails->ValueType == CTL_PROPERTY_VALUE_TYPE_CUSTOM)
                        {
                            Result = ControlACEFeature(hDAhandle, pFeatureVideoProcDetails);
                            printf("ControlMediaFeatures ACE returned code: 0x%X\n", Result);
                        }
                        break;
                    case CTL_VIDEO_PROCESSING_FEATURE_STANDARD_COLOR_CORRECTION:
                        // Standard color correction(SCC)
                        // SCC mode is CTL_PROPERTY_VALUE_TYPE_CUSTOM
                        if (pFeatureVideoProcDetails->ValueType == CTL_PROPERTY_VALUE_TYPE_CUSTOM)
                        {
                            Result = ControlSCCFeature(hDAhandle, pFeatureVideoProcDetails);
                            printf("ControlMediaFeatures SCC returned code: 0x%X\n", Result);
                        }
                        break;
                    case CTL_VIDEO_PROCESSING_FEATURE_TOTAL_COLOR_CORRECTION:
                        // Total color correction(TCC)
                        // TCC mode is CTL_PROPERTY_VALUE_TYPE_CUSTOM
                        if (pFeatureVideoProcDetails->ValueType == CTL_PROPERTY_VALUE_TYPE_CUSTOM)
                        {
                            Result = ControlTCCFeature(hDAhandle, pFeatureVideoProcDetails);
                            printf("ControlMediaFeatures TCC returned code: 0x%X\n", Result);
                        }
                        break;
                    case CTL_VIDEO_PROCESSING_FEATURE_SKIN_TONE_ENHANCEMENT:
                        // Skin Tone Enhancement(STE)
                        // STE mode is UInt32
                        if (pFeatureVideoProcDetails->ValueType == CTL_PROPERTY_VALUE_TYPE_UINT32)
                        {
                            Result = ControlSTEFeature(hDAhandle, pFeatureVideoProcDetails);
                            printf("ControlMediaFeatures STE returned code: 0x%X\n", Result);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        else
        {
            printf("ctlGetSupportedVideoProcessingCapabilities returned failure code when called with NumSupportedFeatures set: 0x%X\n", Result);
        }

        free(FeatureCapsVideoProc.pFeatureDetails);
        FeatureCapsVideoProc.pFeatureDetails = nullptr;
    }
    else
    {
        printf("FeatureCapsVideoProc FeatureDetails Array memory allocation failed\n");
    }

Free_exit:
    if (Result != CTL_RESULT_SUCCESS)
    {
        printf("ctlControlMediaFeatures returned failure code: 0x%X\n", Result);
    }
    return Result;
}
/***********************************************************************
 * @brief main function
 *
 * @details
 *     - main function
 *
 * @returns
 *     - ::CTL_RESULT_SUCCESS
 *     - ::CTL_RESULT_ERROR_UNINITIALIZED
 *     - ::CTL_RESULT_ERROR_DEVICE_LOST
 *     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
 *         + `nullptr == pInitDesc`
 *         + `nullptr == phAPIHandle`
 *     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
 *************************************************************************/
int main()
{
    ctl_result_t Result                                       = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hDevices                     = nullptr;
    ctl_device_adapter_properties_t StDeviceAdapterProperties = { 0 };
    // Get a handle to the DLL module.
    uint32_t Adapter_count = 0;
    uint32_t Display_count = 0;
    uint32_t Index         = 0;
    uint32_t Display_index = 0;

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
        printf("%s \n", e.what());
    }

    if (CTL_RESULT_SUCCESS == Result)
    {
        // Initialization successful
        // Get the list of Intel Adapters

        try
        {
            Result = ctlEnumerateDevices(hAPIHandle, &Adapter_count, hDevices);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");
        }
        catch (const std::bad_array_new_length &e)
        {
            printf("%s \n", e.what());
        }

        if (CTL_RESULT_SUCCESS == Result)
        {
            hDevices = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * Adapter_count);
            if (hDevices == NULL)
            {
                GResult = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
                goto Exit;
            }
            try
            {
                Result = ctlEnumerateDevices(hAPIHandle, &Adapter_count, hDevices);
                LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");
            }
            catch (const std::bad_array_new_length &e)
            {
                printf("%s \n", e.what());
            }
        }
        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlEnumerateDevices returned failure code: 0x%X\n", Result);
            goto Exit;
        }

        for (Index = 0; Index < Adapter_count; Index++)
        {
            if (NULL != hDevices[Index])
            {
                LUID AdapterID;
                StDeviceAdapterProperties.Size           = sizeof(ctl_device_adapter_properties_t);
                StDeviceAdapterProperties.pDeviceID      = malloc(sizeof(LUID));
                StDeviceAdapterProperties.device_id_size = sizeof(LUID);
                StDeviceAdapterProperties.Version        = 2;

                if (NULL == StDeviceAdapterProperties.pDeviceID)
                {
                    return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
                }

                Result = ctlGetDeviceProperties(hDevices[Index], &StDeviceAdapterProperties);

                if (Result != CTL_RESULT_SUCCESS)
                {
                    printf("ctlGetDeviceProperties returned failure code: 0x%X\n", Result);
                    break;
                }

                if (CTL_DEVICE_TYPE_GRAPHICS != StDeviceAdapterProperties.device_type)
                {
                    printf("This is not a Graphics device \n");

                    if (NULL != StDeviceAdapterProperties.pDeviceID)
                    {
                        free(StDeviceAdapterProperties.pDeviceID);
                    }
                    continue;
                }

                if (NULL != StDeviceAdapterProperties.pDeviceID)
                {
                    AdapterID = *(reinterpret_cast<LUID *>(StDeviceAdapterProperties.pDeviceID));
                    printf(" Adapter ID %lu\n", AdapterID.LowPart);
                }

                if (0x8086 == StDeviceAdapterProperties.pci_vendor_id)
                {
                    printf(" Intel Adapter Name  %s\n", StDeviceAdapterProperties.name);
                    printf("Vendor id  0x%X\n", StDeviceAdapterProperties.pci_vendor_id);
                    printf("Device id: 0x%X\n", StDeviceAdapterProperties.pci_device_id);
                    printf("SubSys id 0x%X\n", StDeviceAdapterProperties.pci_subsys_id);
                    printf("SubSys Vendor id 0x%X\n", StDeviceAdapterProperties.pci_subsys_vendor_id);
                    printf("Rev id: 0x%X\n", StDeviceAdapterProperties.rev_id);
                }

                // get media properties
                if (CTL_RESULT_SUCCESS == Result)
                {
                    if (StDeviceAdapterProperties.supported_subfunction_flags & CTL_SUPPORTED_FUNCTIONS_FLAG_MEDIA)
                    {
                        printf("media subfunction supported\n");
                        Result = ControlMediaFeatures(hDevices[Index]);
                    }
                    if (Result != CTL_RESULT_SUCCESS)
                    {
                        printf("ctlControlMediaFeatures returned failure code: 0x%X\n", Result);
                    }
                    STORE_RESET_ERROR(Result);
                }

                if (NULL != StDeviceAdapterProperties.pDeviceID)
                {
                    free(StDeviceAdapterProperties.pDeviceID);
                }
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

    printf("Overrall test result is 0x%X\n", GResult);
    return GResult;
}
