//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file DynamicContrastEnhancement_Sample_App.cpp
 * This sample app performs the following operations for each adapter:
 * (1) Gets the feature status, and if feature is enabled, returns the histogram, and calculates the average luminance
 * (2) Sets the target brightness according to the user-provided brightness percent value and brightness phase-in speed multiplier
 */

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
using namespace std;

#define CTL_APIEXPORT // caller of control API DLL shall define this before
                      // including control_api.h
#include "igcl_api.h"
#include "GenericIGCLApp.h"

ctl_result_t GResult                    = CTL_RESULT_SUCCESS;
static bool IsSetOp                     = FALSE;
static bool Enable                      = FALSE;
static uint32_t TargetBrightnessPercent = 0;
static double PhaseinSpeedMultiplier    = 0.0;

#define BLC_PWM_LOW_PRECISION_FACTOR 100

/***************************************************************
 * @brief PrintUsage
 * Print usage of Dynamic Contrast Enhancement sample app
 * @param pAppName
 * @return void
 ***************************************************************/
void PrintUsage(char *pAppName)
{
    printf("Dynamic Contrast Enhancement Sample Application\n");
    printf("\nUsage: %s <Operation> <Set Operation Data>\n", pAppName);
    printf("\n<Operation>\n");
    printf("\tget : get the feature status and, if feature is enabled, returns the average luminance and current histogram\n");
    printf("\tset : to enable/disable feature, and to set the brightness with the phase-in speed multiplier when feature is enabled\n");
    printf("<Set Operation Data> :\n");
    printf("\tenable : to enable the feature\n");
    printf("\tdisable : to disable the feature\n");
    printf("\tenable <brightness percentage> <phase-in speed multiplier> : to set brightness with phase-in speed multiplier\n");
    printf("\t\t<brightness percentage> : 10 to 100 (applicable if feature is enabled)\n");
    printf("\t\t<phase-in speed multiplier> : 0 (no phase-in) or 1.0 to 2.0 (applicable if feature is enabled)\n");
    printf("Examples:\n");
    printf("\t%s get\n", pAppName);
    printf("\t%s set enable\n", pAppName);
    printf("\t%s set enable 50 0\n", pAppName);
    printf("\t%s set enable 75 1.2\n", pAppName);
    printf("\t%s set disable\n", pAppName);
}

/***************************************************************
 * @brief ParseTargetBrightnessPercent
 * Parse target brightness percent value
 * @param pArg
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t ParseTargetBrightnessPercent(char *pArg)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    for (uint8_t i = 0; i < strlen(pArg); i++)
    {
        if (0 == isdigit(pArg[i]))
        {
            Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
            break;
        }
    }

    if (CTL_RESULT_SUCCESS == Result)
    {
        TargetBrightnessPercent = atoi(pArg);
        if (TargetBrightnessPercent < 10 || TargetBrightnessPercent > 100)
        {
            Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
        }
    }

    return Result;
}

/***************************************************************
 * @brief ParsePhaseinSpeedMultiplier
 * Parse phase-in speed multiplier value
 * @param pArg
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t ParsePhaseinSpeedMultiplier(char *pArg)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    PhaseinSpeedMultiplier = atof(pArg);

    if (PhaseinSpeedMultiplier == 0.0)
    {
        if (0 != strcmp(pArg, "0") && 0 != strcmp(pArg, "0.0"))
        {
            Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
        }
    }
    else
    {
        if (pArg[0] == '0') // invalid input that begins with 0 (e.g., 01)
        {
            Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
        }
        else if (PhaseinSpeedMultiplier < 1.0 || PhaseinSpeedMultiplier > 2.0)
        {
            Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
        }
    }

    return Result;
}

/***************************************************************
 * @brief ParseSetOperationArgs
 * Parse the set operation command line arguments
 * @param argc, pArgv[]
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t ParseSetOperationArgs(int argc, char *pArgv[])
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if (0 != strcmp(pArgv[1], "set"))
    {
        Result = CTL_RESULT_ERROR_INVALID_OPERATION_TYPE;
        EXIT_ON_ERROR(Result);
    }

    if (0 == strcmp(pArgv[2], "enable"))
    {
        Enable = TRUE;
    }
    else if (0 == strcmp(pArgv[2], "disable") && argc == 3)
    {
        Enable = FALSE;
    }
    else
    {
        Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
        EXIT_ON_ERROR(Result);
    }

    if (argc == 5)
    {
        Result = ParseTargetBrightnessPercent(pArgv[3]);
        if (CTL_RESULT_SUCCESS != Result)
        {
            EXIT_ON_ERROR(Result);
        }
        Result = ParsePhaseinSpeedMultiplier(pArgv[4]);
        if (CTL_RESULT_SUCCESS != Result)
        {
            EXIT_ON_ERROR(Result);
        }
    }

Exit:
    if (CTL_RESULT_SUCCESS == Result)
    {
        IsSetOp = TRUE;
    }
    else
    {
        PrintUsage(pArgv[0]);
    }

    return Result;
}

/***************************************************************
 * @brief GetCmdlineArgs
 * Get command line arguments
 * @param argc, pArgv[]
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t GetCmdlineArgs(int argc, char *pArgv[])
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if ((argc < 2 || argc > 5) || argc == 4)
    {
        PrintUsage(pArgv[0]);
        Result = CTL_RESULT_ERROR_INVALID_ARGUMENT;
    }
    else if (argc == 2)
    {
        if (0 == strcmp(pArgv[1], "get"))
        {
            IsSetOp = FALSE;
        }
        else
        {
            PrintUsage(pArgv[0]);
            Result = CTL_RESULT_ERROR_INVALID_OPERATION_TYPE;
        }
    }
    else
    {
        Result = ParseSetOperationArgs(argc, pArgv);
    }

    return Result;
}

/***************************************************************
 * @brief GetSRGBDecodingValue
 * Calculate the normalized SRGB transformed value
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
 * @brief CalculateAverageLuminance
 * Calculate the average luminance
 * @param DceArgs
 * @return double
 ***************************************************************/
double CalculateAverageLuminance(const ctl_dce_args_t DceArgs)
{
    uint32_t TotalNumPixels          = 0;
    double DeGammaLUT                = 0.0;
    double TotalLuminance            = 0.0;
    double AverageLuminance          = 0.0;
    double BinIndexNormalizingFactor = 0.0;

    BinIndexNormalizingFactor = 1.0 / (double)(DceArgs.NumBins - 1);
    for (uint32_t Index = 0; Index < DceArgs.NumBins; Index++)
    {
        TotalNumPixels += DceArgs.pHistogram[Index];
        DeGammaLUT = GetSRGBDecodingValue((double)Index * BinIndexNormalizingFactor);
        TotalLuminance += (double)DceArgs.pHistogram[Index] * DeGammaLUT;
    }

    if (0 != TotalNumPixels)
    {
        AverageLuminance = (TotalLuminance / TotalNumPixels) * BLC_PWM_LOW_PRECISION_FACTOR;
    }

    return AverageLuminance;
}

/***************************************************************
 * @brief PrintGetOperationOutputs
 * Print feature status, calculated avg luminance and histogram
 * @param DceArgs
 * @return void
 ***************************************************************/
void PrintGetOperationOutputs(const ctl_dce_args_t DceArgs)
{
    printf("IsSupported = %d\n", DceArgs.IsSupported);
    printf("Enable = %d\n", DceArgs.Enable);

    if (TRUE == DceArgs.IsSupported && TRUE == DceArgs.Enable && DceArgs.NumBins > 0)
    {
        double AverageLuminance = CalculateAverageLuminance(DceArgs);
        printf("Average Luminance: %f\n", AverageLuminance);

        for (uint32_t Index = 0; Index < DceArgs.NumBins; Index++)
        {
            printf("Histogram Bin[%d] = %d\n", Index, DceArgs.pHistogram[Index]);
        }
    }
}

/***************************************************************
 * @brief TestDynamicContrastEnhancement
 * Sample test for Dynamic Contrast Enhancement feature
 * @param hDevices, AdapterCount
 * @return ctl_result_t
 ***************************************************************/
ctl_result_t TestDynamicContrastEnhancement(ctl_device_adapter_handle_t *hDevices, uint32_t AdapterCount)
{
    ctl_result_t Result                          = CTL_RESULT_SUCCESS;
    ctl_dce_args_t DceArgs                       = { 0 };
    ctl_display_output_handle_t *pDisplayOutputs = NULL;
    uint32_t DisplayCount                        = 0;
    bool IsDisplayActive                         = false;
    bool IsDisplayAttached                       = false;
    bool IsInternalDisplay                       = false;

    DceArgs.Set = IsSetOp;
    if (TRUE == DceArgs.Set)
    {
        DceArgs.Enable                  = Enable;
        DceArgs.TargetBrightnessPercent = TargetBrightnessPercent;
        DceArgs.PhaseinSpeedMultiplier  = PhaseinSpeedMultiplier;
    }

    for (uint32_t Index = 0; Index < AdapterCount; Index++)
    {
        DisplayCount = 0;

        // Enumerate all the possible target displays for the adapters
        // First step is to get the count
        Result = ctlEnumerateDisplayOutputs(hDevices[Index], &DisplayCount, NULL);

        if (CTL_RESULT_SUCCESS != Result)
        {
            LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");
            continue;
        }
        else if (DisplayCount <= 0)
        {
            printf("Invalid Display Count. Skipping display enumeration for adapter: %d\n", Index);
            continue;
        }

        pDisplayOutputs = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * DisplayCount);
        EXIT_ON_MEM_ALLOC_FAILURE(pDisplayOutputs, "pDisplayOutputs");

        Result = ctlEnumerateDisplayOutputs(hDevices[Index], &DisplayCount, pDisplayOutputs);
        LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "ctlEnumerateDisplayOutputs");

        for (uint32_t i = 0; i < DisplayCount; i++)
        {
            ctl_display_properties_t stDisplayProperties = {};
            stDisplayProperties.Size                     = sizeof(ctl_display_properties_t);

            if (NULL == pDisplayOutputs[i])
            {
                printf("pDisplayOutputs[%d] is NULL.\n", i);
                Result = CTL_RESULT_ERROR_INVALID_NULL_HANDLE;
                EXIT_ON_ERROR(Result);
            }

            Result = ctlGetDisplayProperties(pDisplayOutputs[i], &stDisplayProperties);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlGetDisplayProperties");

            ctl_adapter_display_encoder_properties_t stDisplayEncoderProperties = {};
            stDisplayEncoderProperties.Size                                     = sizeof(ctl_adapter_display_encoder_properties_t);

            Result = ctlGetAdaperDisplayEncoderProperties(pDisplayOutputs[i], &stDisplayEncoderProperties);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlGetAdaperDisplayEncoderProperties");

            IsDisplayActive   = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE;
            IsDisplayAttached = stDisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED;

            if (TRUE == IsDisplayActive && TRUE == IsDisplayAttached)
            {
                IsInternalDisplay = stDisplayEncoderProperties.EncoderConfigFlags & CTL_ENCODER_CONFIG_FLAG_INTERNAL_DISPLAY;
                if (FALSE == IsInternalDisplay)
                {
                    printf("Display %d is not an internal display\n", i);
                    continue;
                }

                // For the Get operation, call first time to get the NumBins
                Result = ctlGetSetDynamicContrastEnhancement(pDisplayOutputs[i], &DceArgs);
                LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDynamicContrastEnhancement");

                // For the Get operation, call second time to fill histogram of NumBins * uint32_t size
                if (FALSE == DceArgs.Set)
                {
                    if (TRUE == DceArgs.IsSupported && TRUE == DceArgs.Enable && DceArgs.NumBins > 0)
                    {
                        DceArgs.pHistogram = (uint32_t *)malloc(sizeof(uint32_t) * DceArgs.NumBins);
                        EXIT_ON_MEM_ALLOC_FAILURE(DceArgs.pHistogram, "DceArgs.pHistogram");

                        Result = ctlGetSetDynamicContrastEnhancement(pDisplayOutputs[i], &DceArgs);
                        LOG_AND_EXIT_ON_ERROR(Result, "ctlGetSetDynamicContrastEnhancement");
                    }

                    PrintGetOperationOutputs(DceArgs);
                }
            }
        }

        // Free handle display output pointers
        CTL_FREE_MEM(pDisplayOutputs);
    }

Exit:
    CTL_FREE_MEM(pDisplayOutputs);
    CTL_FREE_MEM(DceArgs.pHistogram)
    return Result;
}

/***************************************************************
 * @brief Main Function that calls the Dynamic Contrast Enhancement API
 * @param Argc
 * @param pArgv[]
 * @return int
 ***************************************************************/
int main(int argc, char *pArgv[])
{
    ctl_result_t Result                   = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hDevices = nullptr;
    uint32_t AdapterCount                 = 0;
    ctl_init_args_t CtlInitArgs;
    ctl_api_handle_t hAPIHandle = nullptr;
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Get cmdline arguments
    Result = GetCmdlineArgs(argc, pArgv);
    LOG_AND_EXIT_ON_ERROR(Result, "GetCmdlineArgs");

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
        printf("%s \n", e.what());
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
        printf("%s \n", e.what());
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
        printf("%s \n", e.what());
    }

    Result = TestDynamicContrastEnhancement(hDevices, AdapterCount);
    LOG_AND_STORE_RESET_RESULT_ON_ERROR(Result, "TestDynamicContrastEnhancement");

Exit:

    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDevices);

    printf("Overall test result is 0x%X\n", GResult);

    return GResult;
}