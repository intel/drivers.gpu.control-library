//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  Color_Sample_APP.cpp
 * @brief This file contains the 'main' function and the Color Sample APP. Program execution begins and ends there.
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
#include "ColorAlgorithms_App.h"

ctl_result_t GResult = CTL_RESULT_SUCCESS;

#define SATURATION_FACTOR_BASE 1.0
#define DEFAULT_COLOR_SAT SATURATION_FACTOR_BASE
#define MIN_COLOR_SAT (SATURATION_FACTOR_BASE - 0.25)
#define MAX_COLOR_SAT (SATURATION_FACTOR_BASE + 0.25)
#define UV_MAX_POINT 0.4375
#define UV_MIN_POINT1 0.0029297
#define UV_MIN_POINT2 0.01074
#define UV_MAX_VAL 255.0
#define CLIP_DOUBLE(Val, Min, Max) (((Val) < (Min)) ? (Min) : (((Val) > (Max)) ? (Max) : (Val)))
#define ABS_DOUBLE(x) ((x) < 0 ? (-x) : (x))
#define DEFAULT_GAMMA_VALUE 2.2

// Convert RGB to YUV
const double RGB2YCbCr709[3][3] = { { 0.2126, 0.7152, 0.0722 }, { -0.1146, -0.3854, 0.5000 }, { 0.5000, -0.4542, -0.0458 } };

// Convert YUV to RGB
const double YCbCr2RGB709[3][3] = { { 1.0000, 0.0000, 1.5748 }, { 1.0000, -0.1873, -0.4681 }, { 1.0000, 1.8556, 0.0000 } };

// convert RGB to XYZ
const double RGB2XYZ_709[3][3] = { { 0.41239080, 0.35758434, 0.18048079 }, { 0.21263901, 0.71516868, 0.07219232 }, { 0.01933082, 0.11919478, 0.95053215 } };

const double Pi    = 3.1415926535897932;
const double Beta  = 0.018053968510807;
const double Alpha = 1.09929682680944;

typedef struct
{
    double R;
    double G;
    double B;
} IPIXELD;

typedef struct
{
    double Y;
    double Cb;
    double Cr;
} IPIXELD_YCbCr;

typedef enum HUE_ANCHOR
{
    HUE_ANCHOR_RED_SATURATION         = 0,
    HUE_ANCHOR_YELLOW_SATURATION      = 1,
    HUE_ANCHOR_GREEN_SATURATION       = 2,
    HUE_ANCHOR_CYAN_SATURATION        = 3,
    HUE_ANCHOR_BLUE_SATURATION        = 4,
    HUE_ANCHOR_MAGENTA_SATURATION     = 5,
    HUE_ANCHOR_COLOR_COUNT_SATURATION = 6
} HUE_ANCHOR;

typedef enum
{
    GAMMA_ENCODING_TYPE_SRGB    = 0, // Gamma encoding SRGB
    GAMMA_ENCODING_TYPE_REC709  = 1, // Gamma encoding REC709
    GAMMA_ENCODING_TYPE_REC2020 = 2, // Gamma encoding REC2020
} GAMMA_ENCODING_TYPE;

// https://en.wikipedia.org/wiki/CIE_1931_color_space
typedef struct
{
    double X;
    double Y;
    double Z;
} IPIXEL_XYZ;

// https://en.wikipedia.org/wiki/CIELAB_color_space
typedef struct
{
    double L;
    double A;
    double B;
} IPIXEL_Lab;

typedef struct
{
    double RedSat;
    double YellowSat;
    double GreenSat;
    double CyanSat;
    double BlueSat;
    double MagentaSat;
} PARTIAL_SATURATION_WEIGHTS;

const IPIXEL_XYZ RefWhitePointSRGB = { 95.0489, 100.0, 108.8840 };

/***************************************************************
 * @brief
 * PerformRGB2YCbCr: Conversion of RGB to YCbCr
 * @param InPixel, OutPixel
 * @return void
 ***************************************************************/
void PerformRGB2YCbCr(ctl_pixtx_3dlut_sample_t InPixel, IPIXELD_YCbCr &OutPixel)
{
    // Numbers used are BT 701 coefficients used for RGB to YCbCr conversion.
    // https://en.wikipedia.org/wiki/YUV#Conversion_to/from_RGB

    OutPixel.Y  = RGB2YCbCr709[0][0] * InPixel.Red + RGB2YCbCr709[0][1] * InPixel.Green + RGB2YCbCr709[0][2] * InPixel.Blue;
    OutPixel.Cb = RGB2YCbCr709[1][0] * InPixel.Red + RGB2YCbCr709[1][1] * InPixel.Green + RGB2YCbCr709[1][2] * InPixel.Blue;
    OutPixel.Cr = RGB2YCbCr709[2][0] * InPixel.Red + RGB2YCbCr709[2][1] * InPixel.Green + RGB2YCbCr709[2][2] * InPixel.Blue;
}

/***************************************************************
 * @brief
 * PerformYCbCr2RGB: Conversion of YCbCr to RGB
 * @param InPixel, OutPixel
 * @return void
 ***************************************************************/
void PerformYCbCr2RGB(IPIXELD_YCbCr InPixel, ctl_pixtx_3dlut_sample_t &OutPixel)
{
    // BT 701 coefficients
    // Numbers used are BT 701 coefficients used for YCbCr to RGB conversion.
    // https://en.wikipedia.org/wiki/YUV#Conversion_to/from_RGB

    OutPixel.Red   = InPixel.Y + YCbCr2RGB709[0][2] * InPixel.Cr;
    OutPixel.Green = InPixel.Y + YCbCr2RGB709[1][1] * InPixel.Cb + YCbCr2RGB709[1][2] * InPixel.Cr;
    OutPixel.Blue  = InPixel.Y + YCbCr2RGB709[2][1] * InPixel.Cb;

    OutPixel.Red   = CLIP_DOUBLE(OutPixel.Red, 0.0, 1.0);
    OutPixel.Green = CLIP_DOUBLE(OutPixel.Green, 0.0, 1.0);
    OutPixel.Blue  = CLIP_DOUBLE(OutPixel.Blue, 0.0, 1.0);
}

/***************************************************************
 * @brief
 * IsDefaultPartialSaturationSettings
 * @param pSatWeights
 * @return bool
 ***************************************************************/
bool IsDefaultPartialSaturationSettings(double *pSatWeights)
{
    for (uint8_t i = 0; i < 6; i++)
    {
        if (DEFAULT_COLOR_SAT != pSatWeights[i])
        {
            return false;
        }
    }

    return true;
}

/***************************************************************
 * @brief
 * InitializePartialSaturationAnchorValues
 * @param pBasicColors
 * @return void
 ***************************************************************/
void InitializePartialSaturationAnchorValues(double *pBasicColors)
{
    pBasicColors[HUE_ANCHOR_RED_SATURATION]     = 0.11111097848067050; // Output of CalculateColorAngle() with pure red input (1.0, 0, 0);
    pBasicColors[HUE_ANCHOR_YELLOW_SATURATION]  = 0.28571682845597457; // Output of CalculateColorAngle() with pure yellow input (1.0, 1.0, 0);
    pBasicColors[HUE_ANCHOR_GREEN_SATURATION]   = 0.37782403959790056; // Output of CalculateColorAngle() with pure green input (0, 1.0, 0);
    pBasicColors[HUE_ANCHOR_CYAN_SATURATION]    = 0.54552642779786065; // Output of CalculateColorAngle() with pure cyan input (0, 1.0, 1.0);
    pBasicColors[HUE_ANCHOR_BLUE_SATURATION]    = 0.85079006538296154; // Output of CalculateColorAngle() with pure blue input (0, 0, 1.0);
    pBasicColors[HUE_ANCHOR_MAGENTA_SATURATION] = 0.91174274251260268; // Output of CalculateColorAngle() with pure magenta input (1.0, 0, 1.0);
}

/***************************************************************
 * @brief
 * CreateDefault3DLut
 * @param pLUT, LutDepth, pSamplingPosition
 * @return void
 ***************************************************************/
void CreateDefault3DLut(ctl_pixtx_3dlut_sample_t *pLUT, uint8_t LutDepth, double *pSamplingPosition)
{
    for (uint8_t R = 0; R < LutDepth; R++)
    {
        for (uint8_t G = 0; G < LutDepth; G++)
        {
            for (uint8_t B = 0; B < LutDepth; B++)
            {
                pLUT->Red   = pSamplingPosition[R];
                pLUT->Green = pSamplingPosition[G];
                pLUT->Blue  = pSamplingPosition[B];
                pLUT++;
            }
        }
    }
}

/***************************************************************
 * @brief
 * InterpolateSaturationFactor
 * @param ColorAngle, pBasicColors, pColorSaturation
 * @return double
 ***************************************************************/
double InterpolateSaturationFactor(double ColorAngle, double *pBasicColors, double *pColorSaturation)
{
    double SatFactor = SATURATION_FACTOR_BASE;
    double Slope, SatFactor1, SatFactor2, Diff;

    // ColorIndex1 represents index of anchor color previous to colorAngle
    // ColorIndex2 represents index of anchor color next to colorAngle
    HUE_ANCHOR ColorIndex1, ColorIndex2;
    ColorIndex1 = (HUE_ANCHOR)((uint8_t)HUE_ANCHOR_COLOR_COUNT_SATURATION - 1);

    for (uint8_t i = 0; i < (uint8_t)HUE_ANCHOR_COLOR_COUNT_SATURATION; i++)
    {
        if (pBasicColors[i] <= ColorAngle)
        {
            ColorIndex1 = (HUE_ANCHOR)i;
        }
    }

    ColorIndex2 = (HUE_ANCHOR)((ColorIndex1 + 1) % HUE_ANCHOR_COLOR_COUNT_SATURATION);

    SatFactor1 = pColorSaturation[(uint8_t)ColorIndex1];
    SatFactor2 = pColorSaturation[(uint8_t)ColorIndex2];

    if ((uint8_t)ColorIndex1 <= 4 || ColorAngle >= pBasicColors[(uint8_t)ColorIndex1])
    {
        Diff  = ColorAngle - pBasicColors[ColorIndex1];
        Slope = (SatFactor2 - SatFactor1) / (pBasicColors[ColorIndex2] - pBasicColors[ColorIndex1]);
    }
    else
    {
        Diff  = ColorAngle - pBasicColors[ColorIndex1] + 1.0;
        Slope = (SatFactor2 - SatFactor1) / (pBasicColors[ColorIndex2] - pBasicColors[ColorIndex1] + 1.0);
    }

    SatFactor = SatFactor1 + Slope * Diff;

    return SatFactor;
}

/***************************************************************
 * @brief
 * GetSRGBDecodingValue
 * @param Input
 * @return double
 ***************************************************************/
double GetSRGBDecodingValue(double Input)
{

    // https://en.wikipedia.org/wiki/SRGB#Transfer_function_(%22gamma%22)

    double Output;

    if (Input <= 0.04045)
    {
        Output = Input / 12.92;
    }
    else
    {
        Output = pow(((Input + 0.055) / 1.055), 2.4);
    }

    return Output;
}

/***************************************************************
 * @brief
 * GetSRGBEncodingValue
 * @param Input
 * @return double
 ***************************************************************/
double GetSRGBEncodingValue(double Input)
{
    /*
    https://en.wikipedia.org/wiki/SRGB#The_forward_transformation_.28CIE_xyY_or_CIE_XYZ_to_sRGB.29
    */

    double Output;

    if (Input <= 0.0031308)
    {
        Output = Input * 12.92;
    }
    else
    {
        Output = (1.055 * pow(Input, 1.0 / 2.4)) - 0.055;
    }

    return Output;
}

/***************************************************************
 * @brief
 * GetRec709DecodingValue
 * @param Input
 * @return double
 ***************************************************************/
double GetRec709DecodingValue(double Input)
{
    /*
    https://en.wikipedia.org/wiki/Rec._709
    */

    double Output;

    if (Input < 0.081)
    {
        Output = Input / 4.5;
    }
    else
    {
        Output = pow(((Input + 0.099) / 1.099), (1 / 0.45));
    }

    return Output;
}

/***************************************************************
 * @brief
 * GetRec709EncodingValue
 * @param Input
 * @return double
 ***************************************************************/
double GetRec709EncodingValue(double Input)
{
    /*
    https://en.wikipedia.org/wiki/Rec._709
    */

    double Output;

    if (Input < 0.018)
    {
        Output = Input * 4.5;
    }
    else
    {
        Output = (1.099 * pow(Input, 0.45)) - 0.099;
    }

    return Output;
}

/***************************************************************
 * @brief
 * GetRec2020DecodingValue
 * @param Input
 * @return double
 ***************************************************************/
double GetRec2020DecodingValue(double Input)
{
    /*
https://en.wikipedia.org/wiki/Rec._2020
*/

    double Output;
    Input = abs(Input);

    if (Input < Beta * 4.5)
    {
        Output = Input / 4.5;
    }
    else
    {
        Output = (pow((Input + Alpha - 1) / Alpha, 1 / 0.45));
    }

    return Output;
}

/***************************************************************
 * @brief
 * GetRec2020EncodingValue
 * @param Input
 * @return double
 ***************************************************************/
double GetRec2020EncodingValue(double Input)
{
    /*
https://en.wikipedia.org/wiki/Rec._2020
*/

    double Output;

    if (Input < Beta)
    {
        Output = 4.5 * Input;
    }
    else
    {
        Output = Alpha * pow(Input, 0.45) - (Alpha - 1);
    }

    return Output;
}

/***************************************************************
 * @brief
 * CIELabTxFn
 * @param Input
 * @return double
 ***************************************************************/
double CIELabTxFn(double Input)
{
    double RetVal = 0;

    /*
    https://en.wikipedia.org/wiki/CIELAB_color_space#From_CIEXYZ_to_CIELAB
    */

    if (Input > pow(6.0 / 29.0, 3.0))
    {
        RetVal = pow(Input, (1.0 / 3.0));
    }
    else
    {
        RetVal = Input * (pow(29.0 / 6.0, 2.0) / 3.0) + (4.0 / 29.0);
    }

    return RetVal;
}

/***************************************************************
 * @brief
 * GetCIELab
 * @param Color, WhitePoint, ColorLab
 * @return void
 ***************************************************************/
void GetCIELab(IPIXEL_XYZ Color, IPIXEL_XYZ WhitePoint, IPIXEL_Lab &ColorLab)
{
    /*
    https://en.wikipedia.org/wiki/CIELAB_color_space#From_CIEXYZ_to_CIELAB
    */

    Color.X /= WhitePoint.X;
    Color.Y /= WhitePoint.Y;
    Color.Z /= WhitePoint.Z;

    ColorLab.L = 116.0 * (CIELabTxFn(Color.Y)) - 16;
    ColorLab.A = 500.0 * (CIELabTxFn(Color.X) - CIELabTxFn(Color.Y));
    ColorLab.B = 200.0 * (CIELabTxFn(Color.Y) - CIELabTxFn(Color.Z));
}

/***************************************************************
 * @brief
 * CalculateColorAngle
 * @param R,G,B
 * @return void
 ***************************************************************/
double CalculateColorAngle(double R, double G, double B)
{
    double RGB[3] = { 0 };
    IPIXEL_XYZ XYZ;
    IPIXEL_Lab Lab;

    RGB[0] = GetSRGBDecodingValue(R);
    RGB[1] = GetSRGBDecodingValue(G);
    RGB[2] = GetSRGBDecodingValue(B);

    MatrixMult3x3With3x1(RGB2XYZ_709, RGB, (double *)&XYZ);

    XYZ.X *= 100.0;
    XYZ.Y *= 100.0;
    XYZ.Z *= 100.0;

    GetCIELab(XYZ, RefWhitePointSRGB, Lab);

    double Angle = atan2(Lab.B, Lab.A); // https://en.wikipedia.org/wiki/Hue

    // convert [-Pi, Pi] range to [0, 2 * Pi] range
    if (Angle < 0)
    {
        Angle = 2.0 * Pi - (ABS_DOUBLE(Angle));
    }

    Angle /= (2.0 * Pi);

    return Angle;
}

/***************************************************************
 * @brief
 * ChangePixelSaturation
 * @param PixelRGB, pBasicColors, pSatWeights
 * @return void
 ***************************************************************/
void ChangePixelSaturation(ctl_pixtx_3dlut_sample_t &PixelRGB, double *pBasicColors, double *pSatWeights)
{
    double SatFactor  = SATURATION_FACTOR_BASE;
    double ColorAngle = 0;
    IPIXELD_YCbCr PixelYCbCr;

    if ((PixelRGB.Red == PixelRGB.Green) && (PixelRGB.Green == PixelRGB.Blue))
    {
        return; // Do not process Grey pixels
    }

    PerformRGB2YCbCr(PixelRGB, PixelYCbCr);

    ColorAngle = CalculateColorAngle(PixelRGB.Red, PixelRGB.Green, PixelRGB.Blue);

    //--- Calculate Sat Factor (Step 1) ---
    SatFactor = InterpolateSaturationFactor(ColorAngle, pBasicColors, pSatWeights);

    //--- Over Saturation Limiter (Step 2) ---
    double UVmax = max((PixelYCbCr.Cb), ABS_DOUBLE(PixelYCbCr.Cr));
    UVmax *= 2.0;

    if ((UVmax >= UV_MAX_POINT) && (SatFactor > SATURATION_FACTOR_BASE))
    {
        SatFactor = SATURATION_FACTOR_BASE;
    }
    else if (SatFactor > SATURATION_FACTOR_BASE)
    {
        // Limit SatFactor according to original saturation
        double a  = (SatFactor - SATURATION_FACTOR_BASE) * (UV_MAX_POINT - UVmax) / UV_MAX_POINT;
        SatFactor = SATURATION_FACTOR_BASE + a;
    }

    //--- Grey Pixels Saturation Limiter (Step 3) ---
    if (SatFactor > SATURATION_FACTOR_BASE)
    {
        double dSat = SatFactor - SATURATION_FACTOR_BASE;

        if (UV_MIN_POINT1 >= UVmax)
        {
            dSat = 0;
        }
        else if (UV_MIN_POINT1 < UVmax && UVmax <= UV_MIN_POINT2)
        {
            dSat *= (UVmax - UV_MIN_POINT1);
        }

        SatFactor = SATURATION_FACTOR_BASE + dSat;
    }

    //--- Calculate New U,V values ---
    PixelYCbCr.Cb = SatFactor * PixelYCbCr.Cb;
    PixelYCbCr.Cr = SatFactor * PixelYCbCr.Cr;

    PerformYCbCr2RGB(PixelYCbCr, PixelRGB);
}

/***************************************************************
 * @brief
 * GenerateHueSaturationMatrix
 * @param Hue, Saturation, CoEff[3][3]
 * @return void
 ***************************************************************/
void GenerateHueSaturationMatrix(double Hue, double Saturation, double CoEff[3][3])
{
    double HueShift                          = Hue * Pi / 180.0;
    double C                                 = cos(HueShift);
    double S                                 = sin(HueShift);
    double HueRotationMatrix[3][3]           = { { 1.0, 0.0, 0.0 }, { 0.0, C, -S }, { 0.0, S, C } };
    double SaturationEnhancementMatrix[3][3] = { { 1.0, 0.0, 0.0 }, { 0.0, Saturation, 0.0 }, { 0.0, 0.0, Saturation } };
    double YCbCr2RGB709[3][3]                = { { 1.0000, 0.0000, 1.5748 }, { 1.0000, -0.1873, -0.4681 }, { 1.0000, 1.8556, 0.0000 } };
    double RGB2YCbCr709[3][3]                = { { 0.2126, 0.7152, 0.0722 }, { -0.1146, -0.3854, 0.5000 }, { 0.5000, -0.4542, -0.0458 } };

    double Result[3][3];

    // Use Bt.709 coefficients for RGB to YCbCr conversion
    MatrixMult3x3(YCbCr2RGB709, SaturationEnhancementMatrix, Result);
    MatrixMult3x3(Result, HueRotationMatrix, Result);
    MatrixMult3x3(Result, RGB2YCbCr709, Result);

    memcpy_s(CoEff, sizeof(Result), Result, sizeof(Result));
}

/***************************************************************
 * @brief
 * Generate3DLutFromPSWeights
 * @param pLUT, LutDepth, pSatWeights
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t Generate3DLutFromPSWeights(ctl_pixtx_3dlut_sample_t *pLUT, uint8_t LutDepth, double *pSatWeights)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    ctl_pixtx_3dlut_sample_t Pix;
    double BasicColors[HUE_ANCHOR_COLOR_COUNT_SATURATION];

    bool IsDefaultConfig = IsDefaultPartialSaturationSettings(pSatWeights);

    InitializePartialSaturationAnchorValues(BasicColors);

    double *pSamplingPosition = (double *)malloc(LutDepth * sizeof(double));
    EXIT_ON_MEM_ALLOC_FAILURE(pSamplingPosition, "pSamplingPosition");

    for (uint8_t i = 0; i < LutDepth; i++)
    {
        pSamplingPosition[i] = (double)i / (double)(LutDepth - 1);
    }

    if (IsDefaultConfig)
    {
        CreateDefault3DLut(pLUT, LutDepth, pSamplingPosition);
        goto Exit;
    }

    for (uint8_t R = 0; R < LutDepth; R++)
    {
        for (uint8_t G = 0; G < LutDepth; G++)
        {
            for (uint8_t B = 0; B < LutDepth; B++)
            {
                Pix.Red   = pSamplingPosition[R];
                Pix.Green = pSamplingPosition[G];
                Pix.Blue  = pSamplingPosition[B];

                ChangePixelSaturation(Pix, BasicColors, pSatWeights);

                *pLUT++ = Pix;
            }
        }
    }

Exit:
    CTL_FREE_MEM(pSamplingPosition);
    return Result;
}

/***************************************************************
 * @brief
 * ApplyPartialSaturation
 * For Get Partial saturaion, User has to store the partial saturation values during set call to use it later (can be used as get call)
 * General steps for Partial Saturaion
 * 1. Create a 3DLUT based on mentioned algorithm for given saturation weights
 * 2. Do a set 3DLUT call with created 3DLUT.
 * @param hDisplayOutput, pBlockConfig, SatWeights
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t ApplyPartialSaturation(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_block_config_t *pBlockConfig, PARTIAL_SATURATION_WEIGHTS SatWeights)
{
    ctl_result_t Result                = CTL_RESULT_SUCCESS;
    ctl_pixtx_block_config_t LutConfig = *pBlockConfig;
    LutConfig.Size                     = sizeof(ctl_pixtx_block_config_t);
    uint8_t LutDepth                   = LutConfig.Config.ThreeDLutConfig.NumSamplesPerChannel;

    ctl_pixtx_pipe_set_config_t SetPixTxArgs = { 0 };
    SetPixTxArgs.Size                        = sizeof(ctl_pixtx_pipe_set_config_t);
    SetPixTxArgs.OpertaionType               = CTL_PIXTX_CONFIG_OPERTAION_TYPE_SET_CUSTOM;
    SetPixTxArgs.NumBlocks                   = 1;          // We are enabling only one block
    SetPixTxArgs.pBlockConfigs               = &LutConfig; // for 3DLUT block

    // Create a valid 3D LUT.
    const uint32_t LutSize                         = LutDepth * LutDepth * LutDepth;
    LutConfig.Config.ThreeDLutConfig.pSampleValues = (ctl_pixtx_3dlut_sample_t *)malloc(LutSize * sizeof(ctl_pixtx_3dlut_sample_t));

    EXIT_ON_MEM_ALLOC_FAILURE(LutConfig.Config.ThreeDLutConfig.pSampleValues, "LutConfig.Config.ThreeDLutConfig.pSampleValues");

    memset(LutConfig.Config.ThreeDLutConfig.pSampleValues, 0, LutSize * sizeof(ctl_pixtx_3dlut_sample_t));

    Result = Generate3DLutFromPSWeights(LutConfig.Config.ThreeDLutConfig.pSampleValues, LutDepth, (double *)&SatWeights);
    LOG_AND_EXIT_ON_ERROR(Result, "Generate3DLutFromPSWeights");

    Result = ctlPixelTransformationSetConfig(hDisplayOutput, &SetPixTxArgs);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlPixelTransformationSetConfig");

Exit:
    CTL_FREE_MEM(LutConfig.Config.ThreeDLutConfig.pSampleValues);
    return Result;
}

/***************************************************************
 * @brief
 * Generate OneDLut From brightness, contrast, gamma values
 * @param pOneDLutConfig, Contrast, PanelGamma, Brightness
 * @return void
 ***************************************************************/
void CreateOneDLutFromBCG(ctl_pixtx_1dlut_config_t *pOneDLutConfig, IPIXELD Contrast, IPIXELD PanelGamma, IPIXELD Brightness)
{
    IPIXELD RelativeGamma;
    double *pRedLut, *pGreenLut, *pBlueLut;
    double NormalizedOutputR, NormalizedOutputG, NormalizedOutputB;
    double NormalizedInput;
    double NormalizedOutputContrastBrightnessGammaR, NormalizedOutputContrastBrightnessGammaG, NormalizedOutputContrastBrightnessGammaB;

    RelativeGamma.R = 2.2 / PanelGamma.R; // Assuming pixels are encoded with SRGB Gamma (2.2).
    RelativeGamma.G = 2.2 / PanelGamma.G;
    RelativeGamma.B = 2.2 / PanelGamma.B;

    // Adjust inputs such that output is somewhat visible
    Contrast.R = CLIP_DOUBLE(Contrast.R, 0.75, 1.25);
    Contrast.G = CLIP_DOUBLE(Contrast.G, 0.75, 1.25);
    Contrast.B = CLIP_DOUBLE(Contrast.B, 0.75, 1.25);

    // Adjust inputs such that output is somewhat visible
    RelativeGamma.R = CLIP_DOUBLE(RelativeGamma.R, 0.75, 1.25);
    RelativeGamma.G = CLIP_DOUBLE(RelativeGamma.G, 0.75, 1.25);
    RelativeGamma.B = CLIP_DOUBLE(RelativeGamma.B, 0.75, 1.25);

    // Adjust inputs such that output is somewhat visible
    Brightness.R = CLIP_DOUBLE(Brightness.R, -0.25, 0.25);
    Brightness.G = CLIP_DOUBLE(Brightness.G, -0.25, 0.25);
    Brightness.B = CLIP_DOUBLE(Brightness.B, -0.25, 0.25);

    // Create 1DLUT from given input
    pRedLut   = pOneDLutConfig->pSampleValues;
    pGreenLut = pRedLut + pOneDLutConfig->NumSamplesPerChannel;
    pBlueLut  = pGreenLut + pOneDLutConfig->NumSamplesPerChannel;

    for (uint32_t i = 0; i < pOneDLutConfig->NumSamplesPerChannel; i++)
    {
        NormalizedInput = (double)i / (double)(pOneDLutConfig->NumSamplesPerChannel - 1);

        NormalizedOutputR = ((NormalizedInput * Contrast.R) + Brightness.R);
        NormalizedOutputG = ((NormalizedInput * Contrast.G) + Brightness.G);
        NormalizedOutputB = ((NormalizedInput * Contrast.B) + Brightness.B);

        NormalizedOutputR = CLIP_DOUBLE(NormalizedOutputR, 0.0, 1.0);
        NormalizedOutputG = CLIP_DOUBLE(NormalizedOutputG, 0.0, 1.0);
        NormalizedOutputB = CLIP_DOUBLE(NormalizedOutputB, 0.0, 1.0);

        NormalizedOutputContrastBrightnessGammaR = pow(NormalizedOutputR, RelativeGamma.R);
        NormalizedOutputContrastBrightnessGammaG = pow(NormalizedOutputG, RelativeGamma.G);
        NormalizedOutputContrastBrightnessGammaB = pow(NormalizedOutputB, RelativeGamma.B);

        pRedLut[i]   = NormalizedOutputContrastBrightnessGammaR;
        pGreenLut[i] = NormalizedOutputContrastBrightnessGammaG;
        pBlueLut[i]  = NormalizedOutputContrastBrightnessGammaB;
    }
}

/***************************************************************
 * @brief
 * ApplyBrightnessContrastGamma
 * @param hDisplayOutput, pBlockConfig, Contrast, Gamma, Brightness
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t ApplyBrightnessContrastGamma(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_block_config_t *pBlockConfig, IPIXELD Contrast, IPIXELD Gamma, IPIXELD Brightness)
{
    ctl_result_t Result                          = CTL_RESULT_SUCCESS;
    ctl_pixtx_block_config_t LutConfig           = *pBlockConfig;
    LutConfig.Size                               = sizeof(ctl_pixtx_block_config_t);
    const uint32_t LutSize                       = LutConfig.Config.OneDLutConfig.NumSamplesPerChannel * LutConfig.Config.OneDLutConfig.NumChannels;
    LutConfig.Config.OneDLutConfig.pSampleValues = (double *)malloc(LutSize * sizeof(double));

    ctl_pixtx_pipe_set_config_t SetPixTxArgs = { 0 };
    SetPixTxArgs.Size                        = sizeof(ctl_pixtx_pipe_set_config_t);
    SetPixTxArgs.OpertaionType               = CTL_PIXTX_CONFIG_OPERTAION_TYPE_SET_CUSTOM;
    SetPixTxArgs.NumBlocks                   = 1;          // We are enabling only one block
    SetPixTxArgs.pBlockConfigs               = &LutConfig; // for 1DLUT block

    EXIT_ON_MEM_ALLOC_FAILURE(LutConfig.Config.OneDLutConfig.pSampleValues, "LutConfig.Config.OneDLutConfig.pSampleValues");

    CreateOneDLutFromBCG(&LutConfig.Config.OneDLutConfig, Contrast, Gamma, Brightness);

    Result = ctlPixelTransformationSetConfig(hDisplayOutput, &SetPixTxArgs);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlPixelTransformationSetConfig");

Exit:
    CTL_FREE_MEM(LutConfig.Config.OneDLutConfig.pSampleValues);
    return Result;
}

/***************************************************************
 * @brief
 * GetPixTxCapability
 * @param hDisplayOutput ,pPixTxCaps
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t GetPixTxCapability(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    Result = ctlPixelTransformationGetConfig(hDisplayOutput, pPixTxCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlPixelTransformationGetConfig for query type capability");

    // Number of blocks
    APP_LOG_INFO("GetPixTxCapsArgs.NumBlocks = %d", pPixTxCaps->NumBlocks);

    if (NULL == pPixTxCaps->pBlockConfigs)
    {
        goto Exit;
    }

    for (uint8_t i = 0; i < pPixTxCaps->NumBlocks; i++)
    {
        ctl_pixtx_1dlut_config_t *pOneDLutConfig = &pPixTxCaps->pBlockConfigs[i].Config.OneDLutConfig;

        // Block specific information
        APP_LOG_INFO("pPixTxCaps->pBlockConfigs[%d].BlockId = %d", i, pPixTxCaps->pBlockConfigs[i].BlockId);
        if (CTL_PIXTX_BLOCK_TYPE_1D_LUT == pPixTxCaps->pBlockConfigs[i].BlockType)
        {
            APP_LOG_INFO("Block type is CTL_PIXTX_BLOCK_TYPE_1D_LUT");
            APP_LOG_INFO("pPixTxCaps->pBlockConfigs[%d].Config.OneDLutConfig.NumChannels = %d", i, pOneDLutConfig->NumChannels);
            APP_LOG_INFO("pPixTxCaps->pBlockConfigs[%d].Config.OneDLutConfig.NumSamplesPerChannel = %d", i, pOneDLutConfig->NumSamplesPerChannel);
            APP_LOG_INFO("pPixTxCaps->pBlockConfigs[%d].Config.OneDLutConfig.SamplingType = %d", i, pOneDLutConfig->SamplingType);
        }
        else if (CTL_PIXTX_BLOCK_TYPE_3X3_MATRIX == pPixTxCaps->pBlockConfigs[i].BlockType)
        {
            APP_LOG_INFO("Block type is CTL_PIXTX_BLOCK_TYPE_3X3_MATRIX");
        }
        else if (CTL_PIXTX_BLOCK_TYPE_3D_LUT == pPixTxCaps->pBlockConfigs[i].BlockType)
        {
            APP_LOG_INFO("Block type is CTL_PIXTX_BLOCK_TYPE_3D_LUT");
            APP_LOG_INFO("pPixTxCaps->pBlockConfigs[%d].Config.ThreeDLutConfig.NumSamplesPerChannel = %d", i, pPixTxCaps->pBlockConfigs[i].Config.ThreeDLutConfig.NumSamplesPerChannel);
        }
        else if (CTL_PIXTX_BLOCK_TYPE_3X3_MATRIX_AND_OFFSETS == pPixTxCaps->pBlockConfigs[i].BlockType)
        {
            APP_LOG_INFO("Block type is CTL_PIXTX_BLOCK_TYPE_3X3_MATRIX_AND_OFFSETS");
        }
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * ApplyLinearCSC DGLUT->CSC->GLUT
 * @param hDisplayOutput
 * @param pPixTxCaps
 * @return
 ***************************************************************/
ctl_result_t ApplyLinearCSC(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if (nullptr == hDisplayOutput)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    if (nullptr == pPixTxCaps)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    // One approach could be check for CSC with offsets block, the block right before CSC with offset block is DGLUT and the Block Right after CSC with Offsets block is GLUT.

    // In HDR mode only one 1DLUT block will be reported and that is of GLUT. So, need to take last occurrence of 1DLUT block in consideration.

    int32_t DGLUTIndex, CscIndex, GLUTIndex;
    DGLUTIndex = CscIndex = GLUTIndex = -1;
    uint32_t OneDLutOccurances        = 0;

    for (uint32_t i = 0; i < pPixTxCaps->NumBlocks; i++)
    {
        if (CTL_PIXTX_BLOCK_TYPE_1D_LUT == pPixTxCaps->pBlockConfigs[i].BlockType)
        {
            if ((CTL_PIXTX_BLOCK_TYPE_3X3_MATRIX_AND_OFFSETS == pPixTxCaps->pBlockConfigs[i + 1].BlockType) && (CTL_PIXTX_BLOCK_TYPE_1D_LUT == pPixTxCaps->pBlockConfigs[i + 2].BlockType))
            {
                DGLUTIndex = i;
                CscIndex   = i + 1;
                GLUTIndex  = i + 2;
            }
            break;
        }
    }

    if (DGLUTIndex < 0 || CscIndex < 0 || GLUTIndex < 0)
    {
        APP_LOG_ERROR("Invalid Index for DGLUT/CSC/GLUT");
        return CTL_RESULT_ERROR_INVALID_ARGUMENT;
    }

    ctl_pixtx_block_config_t DGLUTConfig = pPixTxCaps->pBlockConfigs[DGLUTIndex];
    ctl_pixtx_block_config_t CSCConfig   = pPixTxCaps->pBlockConfigs[CscIndex];
    ctl_pixtx_block_config_t GLUTConfig  = pPixTxCaps->pBlockConfigs[GLUTIndex];

    ctl_pixtx_pipe_set_config_t SetPixTxArgs = { 0 };
    SetPixTxArgs.Size                        = sizeof(ctl_pixtx_pipe_set_config_t);
    SetPixTxArgs.OpertaionType               = CTL_PIXTX_CONFIG_OPERTAION_TYPE_SET_CUSTOM;
    SetPixTxArgs.NumBlocks                   = 3; // We are trying to set only one block
    SetPixTxArgs.pBlockConfigs               = (ctl_pixtx_block_config_t *)malloc(SetPixTxArgs.NumBlocks * sizeof(ctl_pixtx_block_config_t));
    EXIT_ON_MEM_ALLOC_FAILURE(SetPixTxArgs.pBlockConfigs, " SetPixTxArgs.pBlockConfigs");

    memset(SetPixTxArgs.pBlockConfigs, 0, SetPixTxArgs.NumBlocks * sizeof(ctl_pixtx_block_config_t));

    // DGLUT values
    const uint32_t DGLutSize                                                = DGLUTConfig.Config.OneDLutConfig.NumSamplesPerChannel * DGLUTConfig.Config.OneDLutConfig.NumChannels;
    SetPixTxArgs.pBlockConfigs[0].BlockId                                   = DGLUTConfig.BlockId;
    SetPixTxArgs.pBlockConfigs[0].BlockType                                 = DGLUTConfig.BlockType;
    SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.NumChannels          = DGLUTConfig.Config.OneDLutConfig.NumChannels;
    SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.NumSamplesPerChannel = DGLUTConfig.Config.OneDLutConfig.NumSamplesPerChannel;
    SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.SamplingType         = DGLUTConfig.Config.OneDLutConfig.SamplingType;
    SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.pSampleValues        = (double *)malloc(DGLutSize * sizeof(double));

    EXIT_ON_MEM_ALLOC_FAILURE(SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.pSampleValues, " SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.pSampleValues");

    memset(SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.pSampleValues, 0, DGLutSize * sizeof(double));

    for (uint32_t i = 0; i < (DGLutSize / DGLUTConfig.Config.OneDLutConfig.NumChannels); i++)
    {
        double Input                                                        = (double)i / (double)(DGLutSize - 1);
        SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.pSampleValues[i] = GetSRGBDecodingValue(Input);
    }

    // CSC Values
    double PostOffsets[3] = { 0, 0, 0 };
    double PreOffsets[3]  = { 0, 0, 0 };
    double Matrix[3][3]   = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } }; // Identity Matrix
    // double Matrix[3][3]                     = { { 0, 0, 1 }, { 0, 1, 0 }, { 1, 0, 0 } }; // Red Blue swap Matrix
    SetPixTxArgs.pBlockConfigs[1].BlockId   = CSCConfig.BlockId;
    SetPixTxArgs.pBlockConfigs[1].BlockType = CSCConfig.BlockType;

    // Create a valid CSC Matrix.
    for (uint32_t i = 0; i < 3; i++)
    {
        CSCConfig.Config.MatrixConfig.PreOffsets[i] = PreOffsets[i];
    }
    for (uint32_t i = 0; i < 3; i++)
    {
        CSCConfig.Config.MatrixConfig.PostOffsets[i] = PostOffsets[i];
    }

    memcpy_s(SetPixTxArgs.pBlockConfigs[1].Config.MatrixConfig.Matrix, sizeof(SetPixTxArgs.pBlockConfigs[1].Config.MatrixConfig.Matrix), Matrix, sizeof(Matrix));

    // GLUT Values
    // Create a valid 1D LUT.
    const uint32_t GLutSize                                                 = GLUTConfig.Config.OneDLutConfig.NumSamplesPerChannel * GLUTConfig.Config.OneDLutConfig.NumChannels;
    SetPixTxArgs.pBlockConfigs[2].BlockId                                   = GLUTConfig.BlockId;
    SetPixTxArgs.pBlockConfigs[2].BlockType                                 = GLUTConfig.BlockType;
    SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.NumChannels          = GLUTConfig.Config.OneDLutConfig.NumChannels;
    SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.NumSamplesPerChannel = GLUTConfig.Config.OneDLutConfig.NumSamplesPerChannel;
    SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.SamplingType         = GLUTConfig.Config.OneDLutConfig.SamplingType;
    SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.pSampleValues        = (double *)malloc(GLutSize * sizeof(double));

    EXIT_ON_MEM_ALLOC_FAILURE(SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.pSampleValues, " SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.pSampleValues");

    memset(SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.pSampleValues, 0, GLutSize * sizeof(double));

    double *pRedLut   = SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.pSampleValues;
    double *pGreenLut = pRedLut + SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.NumSamplesPerChannel;
    double *pBlueLut  = pGreenLut + SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.NumSamplesPerChannel;

    for (uint32_t i = 0; i < (GLutSize / SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.NumChannels); i++)
    {
        double Input = (double)i / (double)((GLutSize / SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.NumChannels) - 1);
        pRedLut[i] = pGreenLut[i] = pBlueLut[i] = GetSRGBEncodingValue(Input);
    }

    Result = ctlPixelTransformationSetConfig(hDisplayOutput, &SetPixTxArgs);
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlPixelTransformationSetConfig");

Exit:
    if (NULL != SetPixTxArgs.pBlockConfigs)
    {
        CTL_FREE_MEM(SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.pSampleValues);
        CTL_FREE_MEM(SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.pSampleValues);
    }
    CTL_FREE_MEM(SetPixTxArgs.pBlockConfigs);
    return Result;
}

/***************************************************************
 * @brief
 * GetGammaDecodedValue
 * @param Input,EncodingType
 * @return void
 ***************************************************************/
double GetGammaDecodedValue(double Input, GAMMA_ENCODING_TYPE EncodingType)
{
    double DecodedValue;

    switch (EncodingType)
    {
        case GAMMA_ENCODING_TYPE_SRGB:
            DecodedValue = GetSRGBDecodingValue(Input);
            break;
        case GAMMA_ENCODING_TYPE_REC709:
            DecodedValue = GetRec709DecodingValue(Input);
            break;
        case GAMMA_ENCODING_TYPE_REC2020:
            DecodedValue = GetRec2020DecodingValue(Input);
            break;
        default:
            DecodedValue = GetSRGBDecodingValue(Input);
            break;
    }

    return DecodedValue;
}

/***************************************************************
 * @brief
 * RotateAndScalePanelColorSpaceToContentColorSpace DGLUT->CSC->GLUT
 * @param hDisplayOutput
 * @param pPixTxCaps
 * @return
 ***************************************************************/
ctl_result_t RotateAndScalePanelColorSpaceToContentColorSpace(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps)
{
    // PanelColorSpace,ContentColorSpace, GammaValue, EncodingType are provided by User

    // Sample PanelColorSpace
    static ColorSpace PanelColorSpace = {
        { 0.3144, 0.3364, 43.6710 }, // white x,y,Y
        { 0.6756, 0.3159, 11.4200 }, // red
        { 0.2381, 0.7154, 31.0450 }, // green
        { 0.1409, 0.0556, 3.1100 }   // blue
    };

    // ContentColorSpace, standard SRGB color space  https://en.wikipedia.org/wiki/SRGB
    static ColorSpace ContentColorSpace = {
        { 0.3127, 0.3290, 1.0 }, // white x,y,Y
        { 0.64, 0.33, 0.2126 },  // red
        { 0.30, 0.60, 0.7152 },  // green
        { 0.15, 0.060, 0.0722 }  // blue
    };

    double GammaValue                = DEFAULT_GAMMA_VALUE; // User provided GammaValue
    GAMMA_ENCODING_TYPE EncodingType = GAMMA_ENCODING_TYPE_SRGB;

    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if (nullptr == hDisplayOutput)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    if (nullptr == pPixTxCaps)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    // One approach could be check for CSC with offsets block, the block right before CSC with offset block is DGLUT and the Block Right after CSC with Offsets block is GLUT.
    int32_t DGLUTIndex, CscIndex, GLUTIndex;
    DGLUTIndex = CscIndex = GLUTIndex = -1;
    uint32_t OneDLutOccurances        = 0;

    for (uint32_t i = 0; i < pPixTxCaps->NumBlocks; i++)
    {
        if (CTL_PIXTX_BLOCK_TYPE_1D_LUT == pPixTxCaps->pBlockConfigs[i].BlockType)
        {
            if ((CTL_PIXTX_BLOCK_TYPE_3X3_MATRIX_AND_OFFSETS == pPixTxCaps->pBlockConfigs[i + 1].BlockType) && (CTL_PIXTX_BLOCK_TYPE_1D_LUT == pPixTxCaps->pBlockConfigs[i + 2].BlockType))
            {
                DGLUTIndex = i;
                CscIndex   = i + 1;
                GLUTIndex  = i + 2;
            }
            break;
        }
    }

    if (DGLUTIndex < 0 || CscIndex < 0 || GLUTIndex < 0)
    {
        APP_LOG_ERROR("Invalid Index for DGLUT/CSC/GLUT");
        return CTL_RESULT_ERROR_INVALID_ARGUMENT;
    }

    ctl_pixtx_block_config_t DGLUTConfig = pPixTxCaps->pBlockConfigs[DGLUTIndex];
    ctl_pixtx_block_config_t CSCConfig   = pPixTxCaps->pBlockConfigs[CscIndex];
    ctl_pixtx_block_config_t GLUTConfig  = pPixTxCaps->pBlockConfigs[GLUTIndex];

    ctl_pixtx_pipe_set_config_t SetPixTxArgs = { 0 };
    SetPixTxArgs.Size                        = sizeof(ctl_pixtx_pipe_set_config_t);
    SetPixTxArgs.OpertaionType               = CTL_PIXTX_CONFIG_OPERTAION_TYPE_SET_CUSTOM;
    SetPixTxArgs.NumBlocks                   = 3; // We are trying to set only one block
    SetPixTxArgs.pBlockConfigs               = (ctl_pixtx_block_config_t *)malloc(SetPixTxArgs.NumBlocks * sizeof(ctl_pixtx_block_config_t));
    EXIT_ON_MEM_ALLOC_FAILURE(SetPixTxArgs.pBlockConfigs, " SetPixTxArgs.pBlockConfigs");

    memset(SetPixTxArgs.pBlockConfigs, 0, SetPixTxArgs.NumBlocks * sizeof(ctl_pixtx_block_config_t));

    // DGLUT values
    const uint32_t DGLutSize                                                = DGLUTConfig.Config.OneDLutConfig.NumSamplesPerChannel * DGLUTConfig.Config.OneDLutConfig.NumChannels;
    SetPixTxArgs.pBlockConfigs[0].BlockId                                   = DGLUTConfig.BlockId;
    SetPixTxArgs.pBlockConfigs[0].BlockType                                 = DGLUTConfig.BlockType;
    SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.NumChannels          = DGLUTConfig.Config.OneDLutConfig.NumChannels;
    SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.NumSamplesPerChannel = DGLUTConfig.Config.OneDLutConfig.NumSamplesPerChannel;
    SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.SamplingType         = DGLUTConfig.Config.OneDLutConfig.SamplingType;
    SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.pSampleValues        = (double *)malloc(DGLutSize * sizeof(double));

    EXIT_ON_MEM_ALLOC_FAILURE(SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.pSampleValues, " SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.pSampleValues");

    memset(SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.pSampleValues, 0, DGLutSize * sizeof(double));

    for (uint32_t i = 0; i < (DGLutSize / DGLUTConfig.Config.OneDLutConfig.NumChannels); i++)
    {
        double Input                                                        = (double)i / (double)(DGLutSize - 1);
        SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.pSampleValues[i] = GetGammaDecodedValue(Input, EncodingType);
    }

    // CSC

    double PostOffsets[3] = { 0, 0, 0 };
    double PreOffsets[3]  = { 0, 0, 0 };
    double GeneratedOutputMatrix[3][3];

    CreateMatrixToScaleAndRotatePanelToContentColorSpace(PanelColorSpace, ContentColorSpace, GeneratedOutputMatrix);

    SetPixTxArgs.pBlockConfigs[1].BlockId   = CSCConfig.BlockId;
    SetPixTxArgs.pBlockConfigs[1].BlockType = CSCConfig.BlockType;

    // Create a valid CSC Matrix.
    for (uint32_t i = 0; i < 3; i++)
    {
        CSCConfig.Config.MatrixConfig.PreOffsets[i] = PreOffsets[i];
    }
    for (uint32_t i = 0; i < 3; i++)
    {
        CSCConfig.Config.MatrixConfig.PostOffsets[i] = PostOffsets[i];
    }

    memcpy_s(SetPixTxArgs.pBlockConfigs[1].Config.MatrixConfig.Matrix, sizeof(SetPixTxArgs.pBlockConfigs[1].Config.MatrixConfig.Matrix), GeneratedOutputMatrix, sizeof(GeneratedOutputMatrix));

    // GLUT Values
    // Create a valid 1D LUT.
    const uint32_t GLutSize                                                 = GLUTConfig.Config.OneDLutConfig.NumSamplesPerChannel * GLUTConfig.Config.OneDLutConfig.NumChannels;
    SetPixTxArgs.pBlockConfigs[2].BlockId                                   = GLUTConfig.BlockId;
    SetPixTxArgs.pBlockConfigs[2].BlockType                                 = GLUTConfig.BlockType;
    SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.NumChannels          = GLUTConfig.Config.OneDLutConfig.NumChannels;
    SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.NumSamplesPerChannel = GLUTConfig.Config.OneDLutConfig.NumSamplesPerChannel;
    SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.SamplingType         = GLUTConfig.Config.OneDLutConfig.SamplingType;
    SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.pSampleValues        = (double *)malloc(GLutSize * sizeof(double));

    EXIT_ON_MEM_ALLOC_FAILURE(SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.pSampleValues, " SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.pSampleValues");

    memset(SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.pSampleValues, 0, GLutSize * sizeof(double));

    double *pRedLut   = SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.pSampleValues;
    double *pGreenLut = pRedLut + SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.NumSamplesPerChannel;
    double *pBlueLut  = pGreenLut + SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.NumSamplesPerChannel;

    for (uint32_t i = 0; i < (GLutSize / SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.NumChannels); i++)
    {
        double Input  = (double)i / (double)((GLutSize / SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.NumChannels) - 1);
        double Output = 0;
        Output        = (double)INT_MAX * pow(Input, (1.0 / GammaValue));
        Output += 0.5;
        Output     = min(Output, INT_MAX);
        pRedLut[i] = pGreenLut[i] = pBlueLut[i] = Output;
    }

    Result = ctlPixelTransformationSetConfig(hDisplayOutput, &SetPixTxArgs);
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlPixelTransformationSetConfig");

Exit:
    if (NULL != SetPixTxArgs.pBlockConfigs)
    {
        CTL_FREE_MEM(SetPixTxArgs.pBlockConfigs[0].Config.OneDLutConfig.pSampleValues);
        CTL_FREE_MEM(SetPixTxArgs.pBlockConfigs[2].Config.OneDLutConfig.pSampleValues);
    }
    CTL_FREE_MEM(SetPixTxArgs.pBlockConfigs);
    return Result;
}

/***************************************************************
 * @brief
 * Set DeGamma Lut
 * @param hDisplayOutput
 * @param *pPixTxCaps
 * @param DGLUTIndex
 * @return
 ***************************************************************/
ctl_result_t SetDeGammaLut(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps, int32_t DGLUTIndex)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if (nullptr == hDisplayOutput)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    if (nullptr == pPixTxCaps)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    ctl_pixtx_pipe_set_config_t SetPixTxArgs        = { 0 };
    ctl_pixtx_block_config_t LutConfig              = pPixTxCaps->pBlockConfigs[DGLUTIndex];
    LutConfig.Size                                  = sizeof(ctl_pixtx_block_config_t);
    LutConfig.Config.OneDLutConfig.pSamplePositions = NULL;
    double *pLut;

    // Create a valid 1D LUT.
    const uint32_t LutSize                       = LutConfig.Config.OneDLutConfig.NumSamplesPerChannel * LutConfig.Config.OneDLutConfig.NumChannels;
    LutConfig.Config.OneDLutConfig.pSampleValues = (double *)malloc(LutSize * sizeof(double));

    EXIT_ON_MEM_ALLOC_FAILURE(LutConfig.Config.OneDLutConfig.pSampleValues, " LutConfig.Config.OneDLutConfig.pSampleValues");

    memset(LutConfig.Config.OneDLutConfig.pSampleValues, 0, LutSize * sizeof(double));

    SetPixTxArgs.Size          = sizeof(ctl_pixtx_pipe_set_config_t);
    SetPixTxArgs.OpertaionType = CTL_PIXTX_CONFIG_OPERTAION_TYPE_SET_CUSTOM;
    SetPixTxArgs.NumBlocks     = 1;          // We are enabling only one block
    SetPixTxArgs.pBlockConfigs = &LutConfig; // for 1DLUT block

    pLut = LutConfig.Config.OneDLutConfig.pSampleValues;

    for (uint32_t i = 0; i < (LutSize / LutConfig.Config.OneDLutConfig.NumChannels); i++)
    {
        double Input = (double)i / (double)(LutSize - 1);
        pLut[i]      = GetSRGBDecodingValue(Input);
        // pLut[i] = Input; // unity
    }

    Result = ctlPixelTransformationSetConfig(hDisplayOutput, &SetPixTxArgs);

    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlPixelTransformationSetConfig");

Exit:
    CTL_FREE_MEM(LutConfig.Config.OneDLutConfig.pSampleValues);
    return Result;
}

/***************************************************************
 * @brief
 * Get DeGamma
 * @param ctl_display_output_handle_t ,ctl_pixtx_pipe_get_config_t, int32_t
 * @return
 ***************************************************************/
ctl_result_t GetDeGammaLut(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps, int32_t DGLUTIndex)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if (nullptr == hDisplayOutput)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    if (nullptr == pPixTxCaps)
    {
        return CTL_RESULT_ERROR_INVALID_NULL_POINTER;
    }

    ctl_pixtx_pipe_get_config_t GetPixTxCurrentArgs = { 0 };
    GetPixTxCurrentArgs.Size                        = sizeof(ctl_pixtx_pipe_get_config_t);
    ctl_pixtx_block_config_t LutConfig              = pPixTxCaps->pBlockConfigs[DGLUTIndex];

    GetPixTxCurrentArgs.QueryType                = CTL_PIXTX_CONFIG_QUERY_TYPE_CURRENT;
    GetPixTxCurrentArgs.NumBlocks                = 1;          // We are trying to query only one block
    GetPixTxCurrentArgs.pBlockConfigs            = &LutConfig; // Providing Lut config
    LutConfig.Config.OneDLutConfig.pSampleValues = (double *)malloc(LutConfig.Config.OneDLutConfig.NumSamplesPerChannel * LutConfig.Config.OneDLutConfig.NumChannels * sizeof(double));

    EXIT_ON_MEM_ALLOC_FAILURE(LutConfig.Config.OneDLutConfig.pSampleValues, "LutConfig.Config.OneDLutConfig.pSampleValues");

    memset(LutConfig.Config.OneDLutConfig.pSampleValues, 0, LutConfig.Config.OneDLutConfig.NumSamplesPerChannel * LutConfig.Config.OneDLutConfig.NumChannels * sizeof(double));

    Result = ctlPixelTransformationGetConfig(hDisplayOutput, &GetPixTxCurrentArgs);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlPixelTransformationGetConfig");

    APP_LOG_INFO("DEGamma values : LutConfig.Config.OneDLutConfig.pSampleValues");

    uint32_t LutDataSize = LutConfig.Config.OneDLutConfig.NumSamplesPerChannel * LutConfig.Config.OneDLutConfig.NumChannels;
    APP_LOG_INFO("LutDataSize = %d ", LutDataSize);

    APP_LOG_INFO("DeGamma values : LutConfig.Config.OneDLutConfig.pSampleValues");

    for (uint32_t i = 0; i < LutDataSize; i++)
    {
        APP_LOG_INFO("[%d] = %f", i, LutConfig.Config.OneDLutConfig.pSampleValues[i]);
    }

Exit:
    CTL_FREE_MEM(LutConfig.Config.OneDLutConfig.pSampleValues);
    return Result;
}

/***************************************************************
 * @brief
 * SetGammaLut
 * @param hDisplayOutput ,pPixTxCaps, OneDLUTIndex
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t SetGammaLut(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps, int32_t OneDLUTIndex)
{
    ctl_result_t Result                = CTL_RESULT_SUCCESS;
    ctl_pixtx_block_config_t LutConfig = pPixTxCaps->pBlockConfigs[OneDLUTIndex];

    LutConfig.Size                                  = sizeof(ctl_pixtx_block_config_t);
    LutConfig.Config.OneDLutConfig.SamplingType     = CTL_PIXTX_LUT_SAMPLING_TYPE_UNIFORM;
    LutConfig.Config.OneDLutConfig.NumChannels      = 3;
    LutConfig.Config.OneDLutConfig.pSamplePositions = NULL;

    ctl_pixtx_pipe_set_config_t SetPixTxArgs = { 0 };
    SetPixTxArgs.Size                        = sizeof(ctl_pixtx_pipe_set_config_t);
    SetPixTxArgs.OpertaionType               = CTL_PIXTX_CONFIG_OPERTAION_TYPE_SET_CUSTOM;
    SetPixTxArgs.NumBlocks                   = 1;          // We are enabling only one block
    SetPixTxArgs.pBlockConfigs               = &LutConfig; // for 1DLUT block

    double *pRedLut, *pGreenLut, *pBlueLut;
    double LutMultiplier;

    // Create a valid 1D LUT.
    const uint32_t LutSize                       = LutConfig.Config.OneDLutConfig.NumSamplesPerChannel * LutConfig.Config.OneDLutConfig.NumChannels;
    LutConfig.Config.OneDLutConfig.pSampleValues = (double *)malloc(LutSize * sizeof(double));

    EXIT_ON_MEM_ALLOC_FAILURE(LutConfig.Config.OneDLutConfig.pSampleValues, " LutConfig.Config.OneDLutConfig.pSampleValues");

    pRedLut   = LutConfig.Config.OneDLutConfig.pSampleValues;
    pGreenLut = pRedLut + LutConfig.Config.OneDLutConfig.NumSamplesPerChannel;
    pBlueLut  = pGreenLut + LutConfig.Config.OneDLutConfig.NumSamplesPerChannel;

    // Applying a LUT which reduces the Red channel values by 30%.in linear format
    // Based on the Pixel Encoding type , encode the multiplier 0.7 with the same function
    // For example if Encoding is ST2084 then OETF2084(0.7) -> 0.962416136
    //             if Encoding is SRGB   then sRGB(0.7)     -> 0.854305832

    if (CTL_PIXTX_GAMMA_ENCODING_TYPE_ST2084 == pPixTxCaps->OutputPixelFormat.EncodingType)
    {
        LutMultiplier = 0.962416136;
    }
    else if (CTL_PIXTX_GAMMA_ENCODING_TYPE_SRGB == pPixTxCaps->OutputPixelFormat.EncodingType)
    {
        LutMultiplier = 0.854305832;
    }
    else
    {
        LutMultiplier = 1.0;
    }

    // When calling Set for just OneDLUT the LUT is expected to be a Relative Correction LUT
    for (uint32_t i = 0; i < LutConfig.Config.OneDLutConfig.NumSamplesPerChannel; i++)
    {
        double Input = (double)i / (double)(LutConfig.Config.OneDLutConfig.NumSamplesPerChannel - 1);
        pRedLut[i] = pGreenLut[i] = pBlueLut[i] = Input;
        pRedLut[i] *= LutMultiplier;
    }

    Result = ctlPixelTransformationSetConfig(hDisplayOutput, &SetPixTxArgs);
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlPixelTransformationSetConfig");

Exit:
    CTL_FREE_MEM(LutConfig.Config.OneDLutConfig.pSampleValues);
    return Result;
}

/***************************************************************
 * @brief
 * GetGammaLut
 * @param hDisplayOutput ,pPixTxCaps, OneDLUTIndex
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t GetGammaLut(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps, int32_t OneDLUTIndex)
{
    ctl_pixtx_pipe_get_config_t GetPixTxCurrentArgs = { 0 };
    GetPixTxCurrentArgs.Size                        = sizeof(ctl_pixtx_pipe_get_config_t);
    ctl_pixtx_block_config_t LutConfig              = pPixTxCaps->pBlockConfigs[OneDLUTIndex];
    ctl_result_t Result                             = CTL_RESULT_SUCCESS;

    GetPixTxCurrentArgs.QueryType                = CTL_PIXTX_CONFIG_QUERY_TYPE_CURRENT;
    GetPixTxCurrentArgs.NumBlocks                = 1;          // We are trying to query only one block
    GetPixTxCurrentArgs.pBlockConfigs            = &LutConfig; // Providing Lut config
    LutConfig.Config.OneDLutConfig.pSampleValues = (double *)malloc(LutConfig.Config.OneDLutConfig.NumSamplesPerChannel * LutConfig.Config.OneDLutConfig.NumChannels * sizeof(double));

    EXIT_ON_MEM_ALLOC_FAILURE(LutConfig.Config.OneDLutConfig.pSampleValues, "LutConfig.Config.OneDLutConfig.pSampleValues");

    memset(LutConfig.Config.OneDLutConfig.pSampleValues, 0, LutConfig.Config.OneDLutConfig.NumSamplesPerChannel * LutConfig.Config.OneDLutConfig.NumChannels * sizeof(double));

    Result = ctlPixelTransformationGetConfig(hDisplayOutput, &GetPixTxCurrentArgs);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlPixelTransformationGetConfig");

    uint32_t LutDataSize = LutConfig.Config.OneDLutConfig.NumSamplesPerChannel * LutConfig.Config.OneDLutConfig.NumChannels;
    APP_LOG_INFO("LutDataSize = %d ", LutDataSize);

    APP_LOG_INFO("Gamma values : LutConfig.Config.OneDLutConfig.pSampleValues");

    for (uint32_t i = 0; i < LutDataSize; i++)
    {
        APP_LOG_INFO("[%d] = %f", i, LutConfig.Config.OneDLutConfig.pSampleValues[i]);
    }

Exit:
    CTL_FREE_MEM(LutConfig.Config.OneDLutConfig.pSampleValues);
    return Result;
}

/***************************************************************
 * @brief
 * SetCsc
 * Iterate through blocks PixTxCaps and find the CSC block.
 * @param hDisplayOutput, pPixTxCaps, CscBlockIndex
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t SetCsc(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps, int32_t CscBlockIndex)
{
    ctl_result_t Result                = CTL_RESULT_SUCCESS;
    ctl_pixtx_block_config_t CSCConfig = pPixTxCaps->pBlockConfigs[CscBlockIndex];
    CSCConfig.Size                     = sizeof(ctl_pixtx_block_config_t);
    CSCConfig.BlockType                = CTL_PIXTX_BLOCK_TYPE_3X3_MATRIX;
    double PostOffsets[3]              = { 1, 0, 0 };
    double PreOffsets[3]               = { 1, 0, 0 };
    double Matrix[3][3]                = { { 1.2, 0, 0 }, { 0, 1.2, 0 }, { 0, 0, 1.2 } }; // Contrast enhancement by 20%

    // Create a valid CSC Matrix.
    for (uint8_t i = 0; i < 3; i++)
    {
        CSCConfig.Config.MatrixConfig.PreOffsets[i] = PreOffsets[i];
    }

    for (uint8_t i = 0; i < 3; i++)
    {
        CSCConfig.Config.MatrixConfig.PostOffsets[i] = PostOffsets[i];
    }

    memcpy_s(CSCConfig.Config.MatrixConfig.Matrix, sizeof(CSCConfig.Config.MatrixConfig.Matrix), Matrix, sizeof(Matrix));

    ctl_pixtx_pipe_set_config_t SetPixTxArgs = { 0 };
    SetPixTxArgs.Size                        = sizeof(ctl_pixtx_pipe_set_config_t);
    SetPixTxArgs.OpertaionType               = CTL_PIXTX_CONFIG_OPERTAION_TYPE_SET_CUSTOM;
    SetPixTxArgs.NumBlocks                   = 1;          // We are trying to set only one block
    SetPixTxArgs.pBlockConfigs               = &CSCConfig; // for CSC block
    SetPixTxArgs.pBlockConfigs->BlockId      = pPixTxCaps->pBlockConfigs[CscBlockIndex].BlockId;

    Result = ctlPixelTransformationSetConfig(hDisplayOutput, &SetPixTxArgs);
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlPixelTransformationSetConfig");

    return Result;
}

/***************************************************************
 * @brief
 * PixTxRestoreDefault
 * @param hDisplayOutput ,pPixTxCaps
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t PixTxRestoreDefault(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps)
{
    ctl_result_t Result                      = CTL_RESULT_SUCCESS;
    ctl_pixtx_pipe_set_config_t SetPixTxArgs = { 0 };
    SetPixTxArgs.Size                        = sizeof(ctl_pixtx_pipe_set_config_t);
    SetPixTxArgs.OpertaionType               = CTL_PIXTX_CONFIG_OPERTAION_TYPE_RESTORE_DEFAULT;
    SetPixTxArgs.NumBlocks                   = pPixTxCaps->NumBlocks;
    SetPixTxArgs.pBlockConfigs               = pPixTxCaps->pBlockConfigs;

    Result = ctlPixelTransformationSetConfig(hDisplayOutput, &SetPixTxArgs);
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlPixelTransformationSetConfig");

    return Result;
}

/***************************************************************
 * @brief
 * ApplyHueSaturation
 * General steps for Hue Saturaion
 * Create a CSC matrix based on mentioned algorithm for given hue-sat values
 * Do a set CSC call with created matrix.
 * Iterate through blocks PixTxCaps and find the CSC block.
 * @param hDisplayOutput , pPixTxCaps, CscBlockIndex, Hue, Saturation
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t ApplyHueSaturation(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps, int32_t CscBlockIndex, double Hue, double Saturation)
{
    ctl_result_t Result                = CTL_RESULT_SUCCESS;
    ctl_pixtx_block_config_t CSCConfig = pPixTxCaps->pBlockConfigs[CscBlockIndex];
    CSCConfig.Size                     = sizeof(ctl_pixtx_block_config_t);
    Saturation                         = CLIP_DOUBLE(Saturation, 0.75, 1.25);
    Hue                                = CLIP_DOUBLE(Hue, 0, 359);

    if ((0 == Hue) && (0 == Saturation))
    {
        // If Hue and Saturation both are set to default, CSC coefficients should be identity matrix.
        memset(CSCConfig.Config.MatrixConfig.Matrix, 0, sizeof(CSCConfig.Config.MatrixConfig.Matrix));

        CSCConfig.Config.MatrixConfig.Matrix[0][0] = CSCConfig.Config.MatrixConfig.Matrix[1][1] = CSCConfig.Config.MatrixConfig.Matrix[2][2] = 1.0;
    }
    else
    {
        // Below function generates CSC matrix(Non Linear) for given Hue and Satuartion values.
        GenerateHueSaturationMatrix(Hue, Saturation, CSCConfig.Config.MatrixConfig.Matrix);
    }

    ctl_pixtx_pipe_set_config_t SetPixTxArgs = { 0 };
    SetPixTxArgs.Size                        = sizeof(ctl_pixtx_pipe_set_config_t);
    SetPixTxArgs.OpertaionType               = CTL_PIXTX_CONFIG_OPERTAION_TYPE_SET_CUSTOM;
    SetPixTxArgs.NumBlocks                   = 1;          // We are trying to set only one block
    SetPixTxArgs.pBlockConfigs               = &CSCConfig; // for CSC block
    SetPixTxArgs.pBlockConfigs->BlockId      = pPixTxCaps->pBlockConfigs[CscBlockIndex].BlockId;

    Result = ctlPixelTransformationSetConfig(hDisplayOutput, &SetPixTxArgs);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlPixelTransformationSetConfig");

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Set3DLut
 * @param hDisplayOutput, pPixTxCaps, ThreeDLUTIndex
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t Set3DLut(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps, int32_t ThreeDLUTBlockIndex)
{
    ctl_result_t Result                = CTL_RESULT_SUCCESS;
    ctl_pixtx_block_config_t LutConfig = pPixTxCaps->pBlockConfigs[ThreeDLUTBlockIndex];
    LutConfig.Size                     = sizeof(ctl_pixtx_block_config_t);

    ctl_pixtx_pipe_set_config_t SetPixTxArgs = { 0 };
    SetPixTxArgs.Size                        = sizeof(ctl_pixtx_pipe_set_config_t);
    SetPixTxArgs.OpertaionType               = CTL_PIXTX_CONFIG_OPERTAION_TYPE_SET_CUSTOM;
    SetPixTxArgs.NumBlocks                   = 1;                                                          // We are enabling only one block
    SetPixTxArgs.pBlockConfigs               = &LutConfig;                                                 // for 3DLUT block
    SetPixTxArgs.Flags                       = CTL_PIXTX_PIPE_SET_CONFIG_FLAG_PERSIST_ACROSS_POWER_EVENTS; // This flag is required to maintain the persistance across power events.

    // Create a valid 3D LUT.
    uint8_t LutDepth                               = LutConfig.Config.ThreeDLutConfig.NumSamplesPerChannel;
    const uint32_t LutSize                         = LutDepth * LutDepth * LutDepth;
    double *pSamplePos                             = NULL;
    LutConfig.Config.ThreeDLutConfig.pSampleValues = (ctl_pixtx_3dlut_sample_t *)malloc(LutSize * sizeof(ctl_pixtx_3dlut_sample_t));

    EXIT_ON_MEM_ALLOC_FAILURE(LutConfig.Config.ThreeDLutConfig.pSampleValues, "LutConfig.Config.ThreeDLutConfig.pSampleValues");

    memset(LutConfig.Config.ThreeDLutConfig.pSampleValues, 0, LutSize * sizeof(ctl_pixtx_3dlut_sample_t));

    pSamplePos = (double *)malloc(LutDepth * sizeof(double));
    EXIT_ON_MEM_ALLOC_FAILURE(pSamplePos, "pSamplePos");

    memset(pSamplePos, 0, LutDepth * sizeof(double));

    for (uint8_t i = 0; i < LutDepth; i++)
    {
        pSamplePos[i] = (double)i / (double)(LutDepth - 1); // Input should be in the range of 0 to 1.0
    }

    uint32_t LutSampleOffset = 0;

    for (uint32_t R = 0; R < LutDepth; R++)
    {
        for (uint32_t G = 0; G < LutDepth; G++)
        {
            for (uint32_t B = 0; B < LutDepth; B++)
            {
                LutConfig.Config.ThreeDLutConfig.pSampleValues[LutSampleOffset].Red   = pSamplePos[R];
                LutConfig.Config.ThreeDLutConfig.pSampleValues[LutSampleOffset].Green = pSamplePos[G];
                LutConfig.Config.ThreeDLutConfig.pSampleValues[LutSampleOffset].Blue  = pSamplePos[B];
                LutSampleOffset++;
            }
        }
    }

    Result = ctlPixelTransformationSetConfig(hDisplayOutput, &SetPixTxArgs);
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlPixelTransformationSetConfig");

Exit:
    CTL_FREE_MEM(LutConfig.Config.ThreeDLutConfig.pSampleValues);
    CTL_FREE_MEM(pSamplePos);
    return Result;
}

/***************************************************************
 * @brief
 * Get3dLut
 * @param hDisplayOutput, pPixTxCaps, ThreeDLUTIndex
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t Get3dLut(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps, int32_t ThreeDLUTIndex)
{
    ctl_result_t Result                             = CTL_RESULT_SUCCESS;
    ctl_pixtx_pipe_get_config_t GetPixTxCurrentArgs = { 0 };
    ctl_pixtx_block_config_t LutConfig              = pPixTxCaps->pBlockConfigs[ThreeDLUTIndex];
    LutConfig.Size                                  = sizeof(ctl_pixtx_block_config_t);
    GetPixTxCurrentArgs.Size                        = sizeof(ctl_pixtx_pipe_get_config_t);
    GetPixTxCurrentArgs.QueryType                   = CTL_PIXTX_CONFIG_QUERY_TYPE_CURRENT;
    GetPixTxCurrentArgs.NumBlocks                   = 1;          // We are trying to query only one block
    GetPixTxCurrentArgs.pBlockConfigs               = &LutConfig; // 3dLut config

    const uint32_t LutSize = LutConfig.Config.ThreeDLutConfig.NumSamplesPerChannel * LutConfig.Config.ThreeDLutConfig.NumSamplesPerChannel * LutConfig.Config.ThreeDLutConfig.NumSamplesPerChannel;
    LutConfig.Config.ThreeDLutConfig.pSampleValues = (ctl_pixtx_3dlut_sample_t *)malloc(LutSize * sizeof(ctl_pixtx_3dlut_sample_t));

    EXIT_ON_MEM_ALLOC_FAILURE(LutConfig.Config.ThreeDLutConfig.pSampleValues, "LutConfig.Config.ThreeDLutConfig.pSampleValues");

    memset(LutConfig.Config.ThreeDLutConfig.pSampleValues, 0, LutSize * sizeof(ctl_pixtx_3dlut_sample_t));

    Result = ctlPixelTransformationGetConfig(hDisplayOutput, &GetPixTxCurrentArgs);
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlPixelTransformationGetConfig");

Exit:
    CTL_FREE_MEM(LutConfig.Config.ThreeDLutConfig.pSampleValues);
    return Result;
}

/***************************************************************
 * @brief
 * Get Set DeGamma
 * Iterate through blocks PixTxCaps and find the LUT.
 * @param ctl_display_output_handle_t ,ctl_pixtx_pipe_get_config_t
 * @return
 ***************************************************************/
void GetSetDeGamma(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps)
{
    // One approach could be check for CSC with offsets block, the block right before CSC with offset block is DGLUT.

    ctl_result_t Result = CTL_RESULT_SUCCESS;
    int32_t DGLUTIndex  = -1;

    for (uint32_t i = 0; i < pPixTxCaps->NumBlocks; i++)
    {
        if ((CTL_PIXTX_BLOCK_TYPE_1D_LUT == pPixTxCaps->pBlockConfigs[i].BlockType) && (CTL_PIXTX_BLOCK_TYPE_3X3_MATRIX_AND_OFFSETS == pPixTxCaps->pBlockConfigs[i + 1].BlockType))
        {
            DGLUTIndex = i;
            break;
        }
    }

    if (DGLUTIndex < 0)
    {
        APP_LOG_ERROR("Invalid DGLut Index");
        goto Exit;
    }

    // Set DeGamma
    Result = SetDeGammaLut(hDisplayOutput, pPixTxCaps, DGLUTIndex);
    if (CTL_RESULT_SUCCESS != Result)
    {
        APP_LOG_ERROR("SetDeGammaLut call failed");
        STORE_AND_RESET_ERROR(Result);
    }
    else
    {
        // Get DeGamma
        Result = GetDeGammaLut(hDisplayOutput, pPixTxCaps, DGLUTIndex);
        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_ERROR("GetDeGamma call failed");
            LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "GetDeGammaLut");
        }
    }

Exit:
    return;
}

/***************************************************************
 * @brief
 * Get Set Gamma
 * Iterate through blocks PixTxCaps and find the LUT.
 * @param ctl_display_output_handle_t ,ctl_pixtx_pipe_get_config_t
 * @return
 ***************************************************************/
void GetSetGamma(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    // One approach could be check for CSC with offsets block, the block right after CSC with offset block is GLUT.
    // In HDR mode only one 1DLUT block will be reported and that is of GLUT. So, need to take last occurrence of 1DLUT block in consideration.

    int32_t OneDLUTIndex = -1;

    for (uint8_t i = 0; i < pPixTxCaps->NumBlocks; i++)
    {
        // Need to consider the last 1DLUT block for Gamma
        if (CTL_PIXTX_BLOCK_TYPE_1D_LUT == pPixTxCaps->pBlockConfigs[i].BlockType)
        {
            OneDLUTIndex = i;
        }
    }

    if (OneDLUTIndex < 0)
    {
        APP_LOG_ERROR("Invalid OneDLut Index");
        goto Exit;
    }

    // Set Gamma
    Result = SetGammaLut(hDisplayOutput, pPixTxCaps, OneDLUTIndex);

    if (CTL_RESULT_SUCCESS != Result)
    {
        APP_LOG_ERROR("SetGammaLut call failed");
        STORE_AND_RESET_ERROR(Result);
    }
    else
    {
        // Get Gamma
        Result = GetGammaLut(hDisplayOutput, pPixTxCaps, OneDLUTIndex);
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "GetGammaLut");
    }

Exit:
    return;
}

/***************************************************************
 * @brief
 * GetSet3DLUT
 * Get 3DLUT is not supported currently, User has to store the 3DLUT during set call to use it later (can be used as get call)
 * Iterate through blocks PixTxCaps and find the 3DLUT block.
 * @param ctl_display_output_handle_t ,ctl_pixtx_pipe_get_config_t
 * @return
 ***************************************************************/
void GetSet3DLUT(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps)
{
    ctl_result_t Result         = CTL_RESULT_SUCCESS;
    int32_t ThreeDLutBlockIndex = -1;

    for (uint8_t i = 0; i < pPixTxCaps->NumBlocks; i++)
    {
        if (CTL_PIXTX_BLOCK_TYPE_3D_LUT == pPixTxCaps->pBlockConfigs[i].BlockType)
        {
            ThreeDLutBlockIndex = i;
            break;
        }
    }

    if (ThreeDLutBlockIndex < 0)
    {
        APP_LOG_ERROR("Invalid ThreeDLut Index");
        goto Exit;
    }

    Result = Set3DLut(hDisplayOutput, pPixTxCaps, ThreeDLutBlockIndex);

    if (CTL_RESULT_SUCCESS != Result)
    {
        APP_LOG_ERROR("Set3DLut call failed");
        STORE_AND_RESET_ERROR(Result);
    }
    else
    {
        // Get 3dLut
        Result = Get3dLut(hDisplayOutput, pPixTxCaps, ThreeDLutBlockIndex);
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "Get3dLut");
    }

Exit:
    return;
}

/***************************************************************
 * @brief TestBrightnessContrastGamma
 * For Get Desktop Gamma, User has to store the Contrast, Gamma and Brightness values during set call to use it later (can be used as get call)
 * General steps for Desktop Gamma
 * 1.Create a 1DLUT based on mentioned algorithm for given Contrast, Gamma and Brightness
 * 2.Do a set 1DLUT call with created 1DLUT.
 * @param hDisplayOutput, pPixTxCaps
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestBrightnessContrastGamma(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps)
{
    ctl_result_t Result            = CTL_RESULT_SUCCESS;
    int32_t DesktopGammaBlockIndex = -1;

    for (uint8_t i = 0; i < pPixTxCaps->NumBlocks; i++)
    {
        // Need to consider the last 1DLUT block for Gamma
        if (CTL_PIXTX_BLOCK_TYPE_1D_LUT == pPixTxCaps->pBlockConfigs[i].BlockType)
        {
            DesktopGammaBlockIndex = i;
        }
    }

    if (DesktopGammaBlockIndex < 0)
    {
        APP_LOG_ERROR("ctlPixelTransformationGetConfig did not report 1DLUT capability");
        STORE_AND_RESET_ERROR(Result);
    }
    else
    {
        IPIXELD Contrast   = { 0 };
        IPIXELD PanelGamma = { 0 };
        IPIXELD Brightness = { 0 };

        // Contrast 0.75 -> 1.25
        Contrast.R = 1.1; // 10% contrast enhancement
        Contrast.G = 1.1;
        Contrast.B = 1.1;

        PanelGamma.R = 2.1; // assuming a panel with 2.1 gamma
        PanelGamma.G = 2.1;
        PanelGamma.B = 2.1;

        // Brightness -0.25 -> +0.25
        Brightness.R = 0.0;
        Brightness.G = 0.0;
        Brightness.B = 0.0;

        Result = ApplyBrightnessContrastGamma(hDisplayOutput, &pPixTxCaps->pBlockConfigs[DesktopGammaBlockIndex], Contrast, PanelGamma, Brightness);
        LOG_AND_EXIT_ON_ERROR(Result, "ApplyPartialSaturation");
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief TestPartialSaturation
 * For Get Partial saturaion, User has to store the partial saturation values during set call to use it later (can be used as get call)
 * General steps for Partial Saturaion
 * 1. Create a 3DLUT based on mentioned algorithm for given saturation weights
 * 2. Do a set 3DLUT call with created 3DLUT.
 * @param hDisplayOutput, pPixTxCaps
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestPartialSaturation(ctl_display_output_handle_t hDisplayOutput, ctl_pixtx_pipe_get_config_t *pPixTxCaps)
{
    ctl_result_t Result          = CTL_RESULT_SUCCESS;
    int32_t PartialSatBlockIndex = -1;

    for (uint8_t i = 0; i < pPixTxCaps->NumBlocks; i++)
    {
        if (CTL_PIXTX_BLOCK_TYPE_3D_LUT == pPixTxCaps->pBlockConfigs[i].BlockType)
        {
            PartialSatBlockIndex = i;
            break;
        }
    }

    if (PartialSatBlockIndex < 0)
    {
        APP_LOG_ERROR("ctlPixelTransformationGetConfig did not report 3DLUT capability for partial saturation");
        STORE_AND_RESET_ERROR(Result);
    }

    // Partial saturation weights range 0.75 -> 1.25
    PARTIAL_SATURATION_WEIGHTS SatWeights = { 0 };
    SatWeights.RedSat                     = 1.0;
    SatWeights.GreenSat                   = 0.75;
    SatWeights.BlueSat                    = 1.0;
    SatWeights.CyanSat                    = 1.0;
    SatWeights.MagentaSat                 = 1.0;
    SatWeights.YellowSat                  = 0.0;

    Result = ApplyPartialSaturation(hDisplayOutput, &pPixTxCaps->pBlockConfigs[PartialSatBlockIndex], SatWeights);
    LOG_AND_EXIT_ON_ERROR(Result, "ApplyPartialSaturation");

Exit:
    return Result;
}

/***************************************************************
 * @brief TestPixTxGetSetConfig
 * General steps for pixel transformation
 *  1. Query capability of the color pipeline -> ctlPixelTransformationGetConfig
 *  2. Configure particular block of interest with required inputs (Please consider capability of each block while providing inputs)
 *  3. Set up the required block config -> ctlPixelTransformationSetConfig
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestPixTxGetSetConfig(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    // 1st query about the number of blocks supported (pass PixTxCaps.pBlockConfigs as NULL to get number of blocks supported) and then allocate memeory accordingly in second call to get details of
    // each pBlockConfigs
    ctl_pixtx_pipe_get_config_t PixTxCaps = { 0 };
    PixTxCaps.Size                        = sizeof(ctl_pixtx_pipe_get_config_t);
    PixTxCaps.QueryType                   = CTL_PIXTX_CONFIG_QUERY_TYPE_CAPABILITY;

    Result = GetPixTxCapability(hDisplayOutput, &PixTxCaps); // API call will return the number of blocks supported in PixTxCaps.NumBlocks.

    if (0 == PixTxCaps.NumBlocks)
    {
        Result = CTL_RESULT_ERROR_INVALID_SIZE;
        LOG_AND_EXIT_ON_ERROR(Result, "ctlPixelTransformationGetConfig for query type capability");
    }

    // Logical Pipeline details in current mode
    APP_LOG_INFO("Logical Pipeline Input Color Model : %d , Color Space %d, Encoding Type: %d ", PixTxCaps.InputPixelFormat.ColorModel, PixTxCaps.InputPixelFormat.ColorSpace,
                 PixTxCaps.InputPixelFormat.EncodingType);
    APP_LOG_INFO("Logical Pipeline Output Color Model : %d , Color Space %d, Encoding Type: %d , BPC: %d ", PixTxCaps.InputPixelFormat.ColorModel, PixTxCaps.InputPixelFormat.ColorSpace,
                 PixTxCaps.InputPixelFormat.EncodingType, PixTxCaps.OutputPixelFormat.BitsPerColor);

    const uint8_t NumBlocksToQuery = PixTxCaps.NumBlocks; // Query about the blocks in the pipeline

    // Allocate required memory as per number of blocks supported.
    PixTxCaps.pBlockConfigs = (ctl_pixtx_block_config_t *)malloc(NumBlocksToQuery * sizeof(ctl_pixtx_block_config_t));
    EXIT_ON_MEM_ALLOC_FAILURE(PixTxCaps.pBlockConfigs, "PixTxCaps.pBlockConfigs");

    memset(PixTxCaps.pBlockConfigs, 0, NumBlocksToQuery * sizeof(ctl_pixtx_block_config_t));

    // Query capability of each block, number of blocks etc
    Result = GetPixTxCapability(hDisplayOutput, &PixTxCaps);

    // Apply multiple blocks together, for Ex : LinearCSC Degamma->CSC->Gamma
    Result = ApplyLinearCSC(hDisplayOutput, &PixTxCaps);
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ApplyLinearCSC");

    // Set DeGamma & Get DeGamma
    GetSetDeGamma(hDisplayOutput, &PixTxCaps);

    // Set Gamma & Get Gamma
    GetSetGamma(hDisplayOutput, &PixTxCaps);

    // Apply Desktop Gamma : Contrast, Gamma and Brightness
    Result = TestBrightnessContrastGamma(hDisplayOutput, &PixTxCaps);
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "TestBrightnessContrastGamma");

    // Set CSC
    int32_t CscBlockIndex = -1;

    for (uint8_t i = 0; i < PixTxCaps.NumBlocks; i++)
    {
        if (CTL_PIXTX_BLOCK_TYPE_3X3_MATRIX == PixTxCaps.pBlockConfigs[i].BlockType)
        {
            CscBlockIndex = i;
            break;
        }
    }

    if (CscBlockIndex < 0)
    {
        APP_LOG_ERROR("ctlPixelTransformationGetConfig did not report CSC capability");
        STORE_AND_RESET_ERROR(Result);
    }
    else
    {
        Result = SetCsc(hDisplayOutput, &PixTxCaps, CscBlockIndex);
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "SetCsc");
    }

    // Apply Hue Saturation
    int32_t HueSatBlockIndex = -1;

    for (uint8_t i = 0; i < PixTxCaps.NumBlocks; i++)
    {
        if (CTL_PIXTX_BLOCK_TYPE_3X3_MATRIX == PixTxCaps.pBlockConfigs[i].BlockType)
        {
            HueSatBlockIndex = i;
            break;
        }
    }

    Result = ApplyHueSaturation(hDisplayOutput, &PixTxCaps, HueSatBlockIndex, 120, 0.75); // Hue 0 -> 359 , Saturation -> 0.75 to 1.25
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ApplyHueSaturation");

    // Set & Get 3DLUT
    GetSet3DLUT(hDisplayOutput, &PixTxCaps);

    // Apply Partial Saturation
    Result = TestPartialSaturation(hDisplayOutput, &PixTxCaps);
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "TestPartialSaturation");

    // Restore default Gamma, CSC & 3DLUT
    Result = PixTxRestoreDefault(hDisplayOutput, &PixTxCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "PixTxRestoreDefault");

Exit:
    CTL_FREE_MEM(PixTxCaps.pBlockConfigs);
    return Result;
}

/***************************************************************
 * @brief
 * Sample test for Get/Set Lace Config
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestLaceGetSetConfigForFixedAgressiveness(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                         = CTL_RESULT_SUCCESS;
    ctl_lace_config_t AppliedLaceConfigSettings = { 0 };
    ctl_lace_config_t NewLaceConfigSettings     = { 0 };
    ctl_power_optimization_caps_t PowerCaps     = { 0 };

    PowerCaps.Size = sizeof(ctl_power_optimization_caps_t);
    Result         = ctlGetPowerOptimizationCaps(hDisplayOutput, &PowerCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationCaps (LACE)");

    if (CTL_POWER_OPTIMIZATION_FLAG_LACE != (PowerCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_LACE))
    {
        APP_LOG_WARN("LACE is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    NewLaceConfigSettings.Version                                    = 0;
    NewLaceConfigSettings.Size                                       = sizeof(ctl_lace_config_t);
    NewLaceConfigSettings.Enabled                                    = TRUE;
    NewLaceConfigSettings.OpTypeSet                                  = CTL_SET_OPERATION_CUSTOM;
    NewLaceConfigSettings.Trigger                                    = CTL_LACE_TRIGGER_FLAG_FIXED_AGGRESSIVENESS; // Set Lace Config call for fixed Aggr Percent Mode
    NewLaceConfigSettings.LaceConfig.FixedAggressivenessLevelPercent = 50;

    Result = ctlSetLACEConfig(hDisplayOutput, &NewLaceConfigSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlSetLACEConfig");

    // Get Lace Config Call for current flag
    AppliedLaceConfigSettings           = { 0 };
    AppliedLaceConfigSettings.Version   = 0;
    AppliedLaceConfigSettings.Size      = sizeof(ctl_lace_config_t);
    AppliedLaceConfigSettings.OpTypeGet = CTL_GET_OPERATION_FLAG_CURRENT;

    Result = ctlGetLACEConfig(hDisplayOutput, &AppliedLaceConfigSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetLACEConfig");

Exit:
    return Result;
}

/***************************************************************
 * @brief
 * Sample test for Get/Set Lace Config
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestLaceGetSetConfigForALS(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                         = CTL_RESULT_SUCCESS;
    ctl_lace_config_t AppliedLaceConfigSettings = { 0 };
    ctl_lace_config_t NewLaceConfigSettings     = { 0 };
    ctl_power_optimization_caps_t PowerCaps     = { 0 };
    AppliedLaceConfigSettings.Version           = 0;
    AppliedLaceConfigSettings.Size              = sizeof(ctl_lace_config_t);
    AppliedLaceConfigSettings.OpTypeGet         = CTL_GET_OPERATION_FLAG_CAPABILITY;
    uint32_t MaxNumEntries                      = 0;

    PowerCaps.Size = sizeof(ctl_power_optimization_caps_t);
    Result         = ctlGetPowerOptimizationCaps(hDisplayOutput, &PowerCaps);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetPowerOptimizationCaps (LACE)");

    if (CTL_POWER_OPTIMIZATION_FLAG_LACE != (PowerCaps.SupportedFeatures & CTL_POWER_OPTIMIZATION_FLAG_LACE))
    {
        APP_LOG_WARN("LACE is not supported");
        Result = CTL_RESULT_ERROR_UNSUPPORTED_FEATURE;
        goto Exit;
    }

    // Caps calls to get MaxNumEntries for memory allocation
    Result = ctlGetLACEConfig(hDisplayOutput, &AppliedLaceConfigSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetLACEConfig");

    MaxNumEntries = AppliedLaceConfigSettings.LaceConfig.AggrLevelMap.MaxNumEntries;

    NewLaceConfigSettings.Version   = 0;
    NewLaceConfigSettings.Size      = sizeof(ctl_lace_config_t);
    NewLaceConfigSettings.Enabled   = TRUE;
    NewLaceConfigSettings.OpTypeSet = CTL_SET_OPERATION_CUSTOM;
    NewLaceConfigSettings.Trigger   = CTL_LACE_TRIGGER_FLAG_AMBIENT_LIGHT;

    NewLaceConfigSettings.LaceConfig.AggrLevelMap.NumEntries = 6;
    NewLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable =
    (ctl_lace_lux_aggr_map_entry_t *)malloc(sizeof(ctl_lace_lux_aggr_map_entry_t) * NewLaceConfigSettings.LaceConfig.AggrLevelMap.NumEntries);
    EXIT_ON_MEM_ALLOC_FAILURE(NewLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable, "NewLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable");
    NewLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable[0] = { 100, 20 };
    NewLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable[1] = { 400, 40 };
    NewLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable[2] = { 500, 50 };
    NewLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable[3] = { 600, 60 };
    NewLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable[4] = { 800, 80 };
    NewLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable[5] = { 1500, 100 };

    Result = ctlSetLACEConfig(hDisplayOutput, &NewLaceConfigSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlSetLACEConfig");

    // Get Lace Config Call for current flag
    AppliedLaceConfigSettings                                                = { 0 };
    AppliedLaceConfigSettings.Version                                        = 0;
    AppliedLaceConfigSettings.Size                                           = sizeof(ctl_lace_config_t);
    AppliedLaceConfigSettings.OpTypeGet                                      = CTL_GET_OPERATION_FLAG_CURRENT;
    AppliedLaceConfigSettings.LaceConfig.AggrLevelMap.NumEntries             = MaxNumEntries;
    AppliedLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable = (ctl_lace_lux_aggr_map_entry_t *)malloc(sizeof(ctl_lace_lux_aggr_map_entry_t) * MaxNumEntries);

    EXIT_ON_MEM_ALLOC_FAILURE(AppliedLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable, "AppliedLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable");

    Result = ctlGetLACEConfig(hDisplayOutput, &AppliedLaceConfigSettings);
    LOG_AND_EXIT_ON_ERROR(Result, "ctlGetLACEConfig");

Exit:
    CTL_FREE_MEM(AppliedLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable);
    CTL_FREE_MEM(NewLaceConfigSettings.LaceConfig.AggrLevelMap.pLuxToAggrMappingTable);
    return Result;
}

/***************************************************************
 * @brief
 * Sample Test To Override Color Model And Color Depth
 * @param hDisplayOutput
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestToOverrideColorModelAndColorDepth(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                                       = CTL_RESULT_SUCCESS;
    ctl_get_set_wire_format_config_t CurrentWireFormatSetting = { 0 };
    ctl_get_set_wire_format_config_t NewWireFormatSetting     = { 0 };

    // GET call
    CurrentWireFormatSetting.Size      = sizeof(ctl_get_set_wire_format_config_t);
    CurrentWireFormatSetting.Operation = CTL_WIRE_FORMAT_OPERATION_TYPE_GET;
    Result                             = ctlGetSetWireFormat(hDisplayOutput, &CurrentWireFormatSetting);
    LOG_AND_EXIT_ON_ERROR(Result, "GET Call using ctlGetSetWireFormat");

    APP_LOG_INFO("Current ColorModel %d", CurrentWireFormatSetting.WireFormat.ColorModel);
    APP_LOG_INFO("Current ColorDepth %d", CurrentWireFormatSetting.WireFormat.ColorDepth);

    for (uint32_t Index = 0; Index < CTL_MAX_WIREFORMAT_COLOR_MODELS_SUPPORTED; Index++)
    {
        APP_LOG_INFO("Supported ColorModel %d", CurrentWireFormatSetting.SupportedWireFormat[Index].ColorModel);
        APP_LOG_INFO("Supported ColorDepth %d", CurrentWireFormatSetting.SupportedWireFormat[Index].ColorDepth);
    }

    // SET call
    NewWireFormatSetting.Size                  = sizeof(ctl_get_set_wire_format_config_t);
    NewWireFormatSetting.Operation             = CTL_WIRE_FORMAT_OPERATION_TYPE_SET;
    NewWireFormatSetting.WireFormat.ColorModel = CTL_WIRE_FORMAT_COLOR_MODEL_RGB;
    NewWireFormatSetting.WireFormat.ColorDepth = CTL_OUTPUT_BPC_FLAG_8BPC;

    Result = ctlGetSetWireFormat(hDisplayOutput, &NewWireFormatSetting);
    LOG_AND_EXIT_ON_ERROR(Result, "SET Call using ctlGetSetWireFormat");

    // Restore to default
    NewWireFormatSetting.Size      = sizeof(ctl_get_set_wire_format_config_t);
    NewWireFormatSetting.Operation = CTL_WIRE_FORMAT_OPERATION_TYPE_RESTORE_DEFAULT;
    Result                         = ctlGetSetWireFormat(hDisplayOutput, &NewWireFormatSetting);
    LOG_AND_EXIT_ON_ERROR(Result, "RESTORE DEFAULT Call using ctlGetSetWireFormat");

Exit:
    return Result;
}

/***************************************************************
 * @brief TestColorForEnumDisplayHandles
 * Only for demonstration purpose, API is called for each of the display output handle in below snippet.
 * User has to filter through the available display output handle and has to call the API with particular display output handle.
 * @param hDisplayOutput, DisplayCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestColorForEnumDisplayHandles(ctl_display_output_handle_t *hDisplayOutput, uint32_t DisplayCount)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    for (uint32_t DisplayIndex = 0; DisplayIndex < DisplayCount; DisplayIndex++)
    {
        ctl_display_properties_t DisplayProperties = { 0 };
        DisplayProperties.Size                     = sizeof(ctl_display_properties_t);

        Result = ctlGetDisplayProperties(hDisplayOutput[DisplayIndex], &DisplayProperties);
        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDisplayProperties");

        bool IsDisplayActive   = DisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE;
        bool IsDisplayAttached = DisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED;

        if (FALSE == IsDisplayActive || FALSE == IsDisplayAttached)
        {
            APP_LOG_WARN("Display %d is not attached/Active, skipping the call for this display", DisplayIndex);
            continue;
        }

        Result = TestPixTxGetSetConfig(hDisplayOutput[DisplayIndex]);
        STORE_AND_RESET_ERROR(Result);

        Result = TestLaceGetSetConfigForFixedAgressiveness(hDisplayOutput[DisplayIndex]);
        STORE_AND_RESET_ERROR(Result);

        Result = TestLaceGetSetConfigForALS(hDisplayOutput[DisplayIndex]);
        STORE_AND_RESET_ERROR(Result);

        Result = TestToOverrideColorModelAndColorDepth(hDisplayOutput[DisplayIndex]);
        STORE_AND_RESET_ERROR(Result);
    }

Exit:
    return Result;
}

/***************************************************************
 * @brief TestDisplays
 * Enumerates all the possible target display's for the adapters
 * @param AdapterCount, hDevices
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestDisplays(uint32_t AdapterCount, ctl_device_adapter_handle_t *hDevices)
{
    ctl_result_t Result                         = CTL_RESULT_SUCCESS;
    ctl_display_output_handle_t *hDisplayOutput = NULL;
    uint32_t DisplayCount                       = 0;

    for (uint32_t AdapterIndex = 0; AdapterIndex < AdapterCount; AdapterIndex++)
    {
        // enumerate all the possible target display's for the adapters
        // first step is to get the count
        DisplayCount = 0;

        Result = ctlEnumerateDisplayOutputs(hDevices[AdapterIndex], &DisplayCount, NULL);

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
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        // Only for demonstration purpose, API is called for each of the display output handle in below snippet.
        // User has to filter through the available display output handle and has to call the API with particular display output handle.
        Result = TestColorForEnumDisplayHandles(hDisplayOutput, DisplayCount);

        if (CTL_RESULT_SUCCESS != Result)
        {
            APP_LOG_WARN("TestColorForEnumDisplayHandles returned failure code: 0x%X", Result);
        }

        CTL_FREE_MEM(hDisplayOutput);
    }

Exit:

    CTL_FREE_MEM(hDisplayOutput);
    return Result;
}

/***************************************************************
 * @brief Main Function which calls the Color sample API
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

    Result = TestDisplays(AdapterCount, hDevices);
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "TestDisplays");

Exit:

    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDevices);
    APP_LOG_INFO("Overrall test result is 0x%X", GResult);
    return GResult;
}
