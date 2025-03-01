//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  Sample_ControlAPP.cpp
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
#include <cassert>

#include "igcl_api.h"
#include "GenericIGCLApp.h"

// To call Level0 directly
// #include "ze_ddi.h"

/***************************************************************
 * @brief Global result value
 ***************************************************************/
ctl_result_t GResult = CTL_RESULT_SUCCESS;
#define STORE_RESET_ERROR(result)     \
    if (result != CTL_RESULT_SUCCESS) \
        GResult = result;             \
    result = CTL_RESULT_SUCCESS;
#define INVALID_ADAPTER_BDF 0xffui8

// To call level zero api's directly .Please include the level zero header files .
#ifdef _ZE_DDI_H
/***************************************************************
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
ctl_result_t CtlLevel0HandleTest(ctl_device_adapter_handle_t hDevices)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;
    ze_device_handle_t Ze_device;
    ze_device_module_properties_t ZemoduleProperties = { 0 };
    HINSTANCE hLevel0Loader                          = NULL;

    Result = ctlGetZeDevice(hDevices, &Ze_device, &(void *)hLevel0Loader);
    if (CTL_RESULT_SUCCESS == Result && Ze_device != NULL)
    {
        std::cout << "Success: ctlDevice = " << hDevices << " ze_device = " << Ze_device << std::endl;
    }
    else
    {
        std::cout << "Error: Not able to get ze_device!\n";
        STORE_RESET_ERROR(Result);
    }

    // Try to get a level0 function using getprocaddress()
    if (hLevel0Loader)
    {
        ze_pfnDeviceGetModuleProperties_t pfnDeviceGetModuleProperties = (ze_pfnDeviceGetModuleProperties_t)GetProcAddress(hLevel0Loader, "zeDeviceGetModuleProperties");
        if (pfnDeviceGetModuleProperties)
        {
            std::cout << "Success: Obtained level0 zeDeviceGetModuleProperties "
                         "function\n";

            if (Ze_device != NULL)
            {
                if (pfnDeviceGetModuleProperties(Ze_device, &ZemoduleProperties) == ZE_RESULT_SUCCESS)
                {
                    std::cout << "zemoduleProperties::spirvVersionSupported = " << ZemoduleProperties.spirvVersionSupported << std::endl;
                }
                else
                {
                    std::cout << "pfnDeviceGetModuleProperties() returned failure\n";
                }
            }
        }
        else
        {
            std::cout << "Error: Not able to get level0 loader function\n";
        }

#if 0
        // try releasing the lib - a mistake which an app might do and then
        // call control API again!
        FreeLibrary(hLevel0Loader);
        pfnDeviceGetModuleProperties = (ze_pfnDeviceGetModuleProperties_t)GetProcAddress(hLevel0Loader, "zeDeviceGetModuleProperties");
        if (pfnDeviceGetModuleProperties)
        {
            std::cout << "Error: Able to get zeDeviceGetModuleProperties() "
                         "even after FreeLibrary in app! Need investigation on "
                         "why level0 didn't unload - Negative testing!\n";
        }
        else
        {
            std::cout << "Success: After FreeLibrary() not able to get "
                         "function from Level0, we are ok\n";
        }
#endif
    }
    else
    {
        std::cout << "Error: Not able to get level0 instance\n";
    }
    return Result;
}
#endif

/***************************************************************
 * @brief
 * Sharpness interface usage model
 * @param
 * @return
 ***************************************************************/
ctl_result_t CtlSharpnessTest(ctl_display_output_handle_t hDisplayOutput)
{
    ctl_result_t Result                   = CTL_RESULT_SUCCESS;
    ctl_sharpness_settings_t SetSharpness = { 0 };
    ctl_sharpness_settings_t GetSharpness = { 0 };
    ctl_sharpness_caps_t SharpnessCaps    = { 0 };

    SetSharpness.Size  = sizeof(ctl_sharpness_settings_t);
    GetSharpness.Size  = sizeof(ctl_sharpness_settings_t);
    SharpnessCaps.Size = sizeof(ctl_sharpness_caps_t);

    SetSharpness.FilterType = CTL_SHARPNESS_FILTER_TYPE_FLAG_NON_ADAPTIVE;
    SetSharpness.Enable     = 1;
    SetSharpness.Intensity  = 85; // Current Implementation is to support 0-100.

    // Get Sharpness caps
    if (NULL != hDisplayOutput)
    {
        Result = ctlGetSharpnessCaps(hDisplayOutput, &SharpnessCaps);

        if (Result != CTL_RESULT_SUCCESS)
        {
            printf("ctlGetSharpnessCaps returned failure code: 0x%X\n", Result);
            return Result;
        }
        else
        {
            printf("ctlGetSharpnessCaps returned success\n");
            SharpnessCaps.pFilterProperty = (ctl_sharpness_filter_properties_t *)malloc(SharpnessCaps.NumFilterTypes * sizeof(ctl_sharpness_filter_properties_t));
            Result                        = ctlGetSharpnessCaps(hDisplayOutput, &SharpnessCaps);
            if (Result != CTL_RESULT_SUCCESS)
            {
                printf("ctlGetSharpnessCaps when called to get caps after malloc "
                       "returned failure code: 0x%X\n",
                       Result);
                free(SharpnessCaps.pFilterProperty);
                return Result;
            }
            else
            {
                printf("sharpnessCaps.SupportedFilterFlags = %d\n", SharpnessCaps.SupportedFilterFlags);
                printf("sharpnessCaps.NumFilterTypes = %d\n", SharpnessCaps.NumFilterTypes);
                for (int i = 0; i < SharpnessCaps.NumFilterTypes; i++)
                {
                    printf(" sharpnessCaps.pFilterProperty[%d].FilterType = %d\n", i, SharpnessCaps.pFilterProperty[i].FilterType);
                    printf(" sharpnessCaps.pFilterProperty[%d].FilterDetails.min_possible_"
                           "value = %f\n",
                           i, SharpnessCaps.pFilterProperty[i].FilterDetails.min_possible_value);
                    printf(" sharpnessCaps.pFilterProperty[%d].FilterDetails.max_possible_"
                           "value = %f\n",
                           i, SharpnessCaps.pFilterProperty[i].FilterDetails.max_possible_value);
                    printf(" sharpnessCaps.pFilterProperty[%d].FilterDetails.step_size = %f\n", i, SharpnessCaps.pFilterProperty[i].FilterDetails.step_size);
                }
                free(SharpnessCaps.pFilterProperty);
            }
        }
    }
    if ((Result == CTL_RESULT_SUCCESS) &&
        ((SharpnessCaps.SupportedFilterFlags == CTL_SHARPNESS_FILTER_TYPE_FLAG_NON_ADAPTIVE) || (SharpnessCaps.SupportedFilterFlags == CTL_SHARPNESS_FILTER_TYPE_FLAG_ADAPTIVE)))
    {
        // SetSharpness
        if (NULL != hDisplayOutput)
        {
            if (SharpnessCaps.SupportedFilterFlags == CTL_SHARPNESS_FILTER_TYPE_FLAG_NON_ADAPTIVE)
            {
                SetSharpness.FilterType = CTL_SHARPNESS_FILTER_TYPE_FLAG_NON_ADAPTIVE;
            }
            else if (SharpnessCaps.SupportedFilterFlags == CTL_SHARPNESS_FILTER_TYPE_FLAG_ADAPTIVE)
            {
                SetSharpness.FilterType = CTL_SHARPNESS_FILTER_TYPE_FLAG_ADAPTIVE;
            }
            else
            {
                Result = CTL_RESULT_ERROR_INVALID_SHARPNESS_FILTER_FLAG;
                printf("Result: 0x%X FilterType %d", Result, SharpnessCaps.SupportedFilterFlags);
                return Result;
            }
            SetSharpness.FilterType = SharpnessCaps.SupportedFilterFlags;
            Result                  = ctlSetCurrentSharpness(hDisplayOutput, &SetSharpness);

            if (Result != CTL_RESULT_SUCCESS)
            {
                printf("ctlSetCurrentSharpness returned failure code: "
                       "0x%X\n",
                       Result);
                return Result;
            }
            else
            {
                printf("ctlSetCurrentSharpness returned success\n");
            }
        }
    }

    if (Result == CTL_RESULT_SUCCESS)
    {
        // GetSharpness
        if (NULL != hDisplayOutput)
        {
            Result = ctlGetCurrentSharpness(hDisplayOutput, &GetSharpness);

            if (Result != CTL_RESULT_SUCCESS)
            {
                printf("ctlGetCurrentSharpness returned failure code: "
                       "0x%X\n",
                       Result);
                return Result;
            }
            else
            {
                printf("ctlGetCurrentSharpness returned success\n");
                printf("GetSharpness.Enable = %d\n", GetSharpness.Enable);
                printf("GetSharpness.FilterType = %d\n", GetSharpness.FilterType);
                printf("GetSharpness.Intensity = %f\n", GetSharpness.Intensity);
            }
        }
    }

    return Result;
}

/***************************************************************
 * @brief Get's custom 3d struct data buffer for known data types
 * place_holder_for_Detailed_desc
 * @param ctl_3d_feature_getset_t*
 * @return true/false
 ***************************************************************/
inline bool Get3DCustomStruct(ctl_3d_feature_getset_t *p3DGetSetArgs)
{
    if ((NULL == p3DGetSetArgs) || (CTL_PROPERTY_VALUE_TYPE_CUSTOM != p3DGetSetArgs->ValueType))
        return false;

    p3DGetSetArgs->CustomValueSize = 0;
    p3DGetSetArgs->pCustomValue    = NULL;
    if (CTL_3D_FEATURE_ENDURANCE_GAMING == p3DGetSetArgs->FeatureType)
    {
        p3DGetSetArgs->CustomValueSize = sizeof(ctl_endurance_gaming_t);
    }
    else if (CTL_3D_FEATURE_ADAPTIVE_SYNC_PLUS == p3DGetSetArgs->FeatureType)
    {
        p3DGetSetArgs->CustomValueSize = sizeof(ctl_adaptivesync_getset_t);
    }
    else if (CTL_3D_FEATURE_APP_PROFILES == p3DGetSetArgs->FeatureType)
    {
        p3DGetSetArgs->CustomValueSize = sizeof(ctl_3d_app_profiles_t);
    }
    else
    {
        // TODO: A new custom property got added, which is not known to this driver
        // Check with IGCL developer and add the special case like for EG above
        // This shouldn't behappening as runtime is in sync with driver and runtime
        // custom values should be in sync with driver's known custom data types
        return false;
    }

    if (p3DGetSetArgs->CustomValueSize)
        p3DGetSetArgs->pCustomValue = malloc(p3DGetSetArgs->CustomValueSize);

    if (p3DGetSetArgs->pCustomValue)
    {
        // clear the custom struct
        memset(p3DGetSetArgs->pCustomValue, 0, p3DGetSetArgs->CustomValueSize);

        // if any in parameters are required, set it up with some default ones - just to generalize code
        if (CTL_3D_FEATURE_APP_PROFILES == p3DGetSetArgs->FeatureType)
        {
            ctl_3d_app_profiles_t *pAppProfile = (ctl_3d_app_profiles_t *)p3DGetSetArgs->pCustomValue;
            pAppProfile->TierType              = CTL_3D_TIER_TYPE_FLAG_COMPATIBILITY;
        }
        return true;
    }

    return false;
}

/***************************************************************
 * @brief
 * Various gaming feature get/set usage model
 * @param
 * @return
 ***************************************************************/
ctl_result_t CtlGet3DGlobalTest(ctl_device_adapter_handle_t hDevices)
{
    ctl_result_t Result                 = CTL_RESULT_SUCCESS;
    ctl_3d_feature_caps_t FeatureCaps3D = { 0 };

    FeatureCaps3D.Size = sizeof(ctl_3d_feature_caps_t);
    Result             = ctlGetSupported3DCapabilities(hDevices, &FeatureCaps3D);
    if (Result != CTL_RESULT_SUCCESS)
    {
        printf("ctlGetSupported3DCapabilities returned failure code: 0x%X\n", Result);
        return Result;
    }
    else
    {
        printf("ctlGetSupported3DCapabilities returned success. No of gaming features = %d\n", FeatureCaps3D.NumSupportedFeatures);
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

                // try a get call with a huge app name
                printf("Calling ctlGetSet3DFeature()\n");
                ctl_3d_feature_getset_t Feature3D = { 0 };
                Feature3D.bSet                    = false;
                Feature3D.FeatureType             = FeatureCaps3D.pFeatureDetails[i].FeatureType;
                Feature3D.Size                    = sizeof(Feature3D);
                Feature3D.ValueType               = FeatureCaps3D.pFeatureDetails[i].ValueType;
                Feature3D.ApplicationName         = "a123456789012345.exe";
                Feature3D.ApplicationNameLength   = (int8_t)strlen(Feature3D.ApplicationName);
                if (CTL_PROPERTY_VALUE_TYPE_CUSTOM == Feature3D.ValueType)
                {
                    if (false == Get3DCustomStruct(&Feature3D))
                    {
                        printf("A new custom feature was added which is not known to the driver\n");
                        continue; // some error, continue with next value
                    }
                }
                Result = ctlGetSet3DFeature(hDevices, &Feature3D);
                printf("ctlGetSet3DFeature returned 0x%X for FeatureType = %d\n", Result, Feature3D.FeatureType);
                Result = CTL_RESULT_SUCCESS;

                // free up the additional custom block which got allocated for both get/set struct & caps
                if (Feature3D.pCustomValue)
                {
                    free(Feature3D.pCustomValue);
                    Feature3D.pCustomValue = NULL;
                }
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
            free(FeatureCaps3D.pFeatureDetails);
            return Result;
        }
        free(FeatureCaps3D.pFeatureDetails);
    }

    return Result;
}

/***************************************************************
 * @brief CtlGetDisplayPropertiesTest
 * Display properties test
 * @param
 * @return
 ***************************************************************/
ctl_result_t CtlGetDisplayPropertiesTest(ctl_device_adapter_handle_t hAdapter, ctl_display_output_handle_t hDisplayOutput, ctl_display_properties_t *pStdisplayproperties)
{
    ctl_result_t Result     = CTL_RESULT_SUCCESS;
    bool Isdisplay_active   = false;
    bool Isdisplay_attached = false;

    if (NULL != hDisplayOutput)
    {
        Result = ctlGetDisplayProperties(hDisplayOutput, pStdisplayproperties);

        if (Result != CTL_RESULT_SUCCESS)
        {
            printf("ctlGetDisplayOutputProperties returned failure code: "
                   "0x%X\n",
                   Result);
            STORE_RESET_ERROR(Result);
        }
        else
        {
            Isdisplay_active   = pStdisplayproperties->DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE;
            Isdisplay_attached = pStdisplayproperties->DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED;
            std::cout << "Display Target ID is " << pStdisplayproperties->Os_display_encoder_handle.WindowsDisplayEncoderID << "\n";
            std::cout << "DisplayConfigFlags is " << pStdisplayproperties->DisplayConfigFlags << "\n";
            std::cout << "Is Display Active : " << Isdisplay_active << "\n";
            std::cout << "Is Display Attached : " << Isdisplay_attached << "\n";
        }
        pStdisplayproperties->DisplayConfigFlags                                = 0;
        pStdisplayproperties->Os_display_encoder_handle.WindowsDisplayEncoderID = 0;
    }
    return Result;
}

/***************************************************************
 * @brief
 * Get Display properties wrapper
 * @param
 * @return
 ***************************************************************/
void GetDisplayPropertiesTest(ctl_device_adapter_handle_t *hDevices, ctl_display_output_handle_t *hDisplayOutput, ctl_result_t *pResult, uint32_t Index, uint32_t DisplayCount)
{
    ctl_display_properties_t DisplayProperties = {};
    DisplayProperties.Size                     = sizeof(ctl_display_properties_t);
    uint32_t DisplayIndex                      = 0;

    for (DisplayIndex = 0; DisplayIndex < DisplayCount; DisplayIndex++)
    {
        *pResult = CtlGetDisplayPropertiesTest(hDevices[Index], hDisplayOutput[DisplayIndex], &DisplayProperties);
        STORE_RESET_ERROR(*pResult);
    }
}

/***************************************************************
 * @brief
 * Encoder properties test
 * @param
 * @return
 ***************************************************************/
ctl_result_t CtlGetDisplayEncoderPropertiesTest(ctl_display_output_handle_t hDisplayOutput, ctl_adapter_display_encoder_properties_t *pStdisplayencoderproperties)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    if (NULL != hDisplayOutput)
    {
        Result = ctlGetAdaperDisplayEncoderProperties(hDisplayOutput, pStdisplayencoderproperties);

        if (Result != CTL_RESULT_SUCCESS)
        {
            printf("ctlGetAdaperDisplayEncoderProperties returned failure code: "
                   "0x%X\n",
                   Result);
            STORE_RESET_ERROR(Result);
        }
        else
        {
            std::cout << "Encoder Target ID is " << pStdisplayencoderproperties->Os_display_encoder_handle.WindowsDisplayEncoderID << "\n";
        }
    }
    return Result;
}

/***************************************************************
 * @brief
 * Get Encoder properties wrapper
 * @param
 * @return
 ***************************************************************/
void GetDisplayEncoderPropertiesTest(ctl_display_output_handle_t *hDisplayOutput, ctl_result_t *pResult, uint32_t DisplayCount)
{
    ctl_adapter_display_encoder_properties_t DisplayEncoderProperties = {};
    DisplayEncoderProperties.Size                                     = sizeof(ctl_adapter_display_encoder_properties_t);
    uint32_t DisplayIndex                                             = 0;

    for (DisplayIndex = 0; DisplayIndex < DisplayCount; DisplayIndex++)
    {
        *pResult = CtlGetDisplayEncoderPropertiesTest(hDisplayOutput[DisplayIndex], &DisplayEncoderProperties);
        STORE_RESET_ERROR(*pResult);
    }
}

/***************************************************************
 * @brief
 * Example function which shows how to wait on property changes
 * @param
 * @return
 ***************************************************************/
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
 * Example function which shows how to wait on property changes
 * @param
 * @return
 ***************************************************************/
ctl_result_t CtlTestEvents(ctl_device_adapter_handle_t hAdapter)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;
#ifdef TEST_ENABLE_SETCALLS
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
            ctl_3d_feature_getset_t Feature3D   = { 0 };
            Feature3D.bSet                      = true;
            Feature3D.FeatureType               = CTL_3D_FEATURE_ANISOTROPIC;
            Feature3D.Size                      = sizeof(Feature3D);
            Feature3D.ValueType                 = CTL_PROPERTY_VALUE_TYPE_ENUM;
            Feature3D.Value.EnumType.EnableType = CTL_3D_ANISOTROPIC_TYPES_4X;

            Result = ctlGetSet3DFeature(hAdapter, &Feature3D);

            if (Result == CTL_RESULT_SUCCESS)
            {
                ctl_3d_feature_getset_t Feature3D   = { 0 };
                Feature3D.bSet                      = true;
                Feature3D.FeatureType               = CTL_3D_FEATURE_ANISOTROPIC;
                Feature3D.Size                      = sizeof(Feature3D);
                Feature3D.ValueType                 = CTL_PROPERTY_VALUE_TYPE_ENUM;
                Feature3D.Value.EnumType.EnableType = CTL_3D_ANISOTROPIC_TYPES_APP_CHOICE;

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
#endif

    return Result;
}

/***************************************************************
 * @brief
 * Print driver version info
 * @param
 * @return
 ***************************************************************/
void PrintAdapterProperties(ctl_device_adapter_properties_t *pStDeviceAdapterProperties)
{
    char DriverVersion[25] = "";
    LARGE_INTEGER LIDriverVersion;
    LIDriverVersion.QuadPart = pStDeviceAdapterProperties->driver_version;
    sprintf(DriverVersion, "%d.%d.%d.%d", HIWORD(LIDriverVersion.HighPart), LOWORD(LIDriverVersion.HighPart), HIWORD(LIDriverVersion.LowPart), LOWORD(LIDriverVersion.LowPart));

    printf("Intel Graphics Driver Version : %s\n", DriverVersion);
    printf("GOP Version : %lld.%lld.%lld\n", pStDeviceAdapterProperties->firmware_version.major_version, pStDeviceAdapterProperties->firmware_version.minor_version,
           pStDeviceAdapterProperties->firmware_version.build_number);

    printf("Intel Adapter Name: %s\n", pStDeviceAdapterProperties->name);
    printf("Vendor ID: 0x%X\n", pStDeviceAdapterProperties->pci_vendor_id);
    printf("Device ID: 0x%X\n", pStDeviceAdapterProperties->pci_device_id);
    printf("SubSys id 0x%X\n", pStDeviceAdapterProperties->pci_subsys_id);
    printf("SubSys Vendor id 0x%X\n", pStDeviceAdapterProperties->pci_subsys_vendor_id);
    printf("Rev ID: 0x%X\n", pStDeviceAdapterProperties->rev_id);
    printf("Graphics Frequency: %dMHz\n", pStDeviceAdapterProperties->Frequency);
    printf("num_eus_per_sub_slice: %d\n", pStDeviceAdapterProperties->num_eus_per_sub_slice);
    printf("num_slices: %d\n", pStDeviceAdapterProperties->num_slices);
    printf("num_sub_slices_per_slice: %d\n", pStDeviceAdapterProperties->num_sub_slices_per_slice);
    printf("num_xe_cores: %d\n", pStDeviceAdapterProperties->num_xe_cores);
    printf("Graphics HW type: %s\n", pStDeviceAdapterProperties->graphics_adapter_properties & CTL_ADAPTER_PROPERTIES_FLAG_INTEGRATED ? "Integrated" : "External GFX");

    if ((INVALID_ADAPTER_BDF == pStDeviceAdapterProperties->adapter_bdf.bus) && (INVALID_ADAPTER_BDF == pStDeviceAdapterProperties->adapter_bdf.device) &&
        (INVALID_ADAPTER_BDF == pStDeviceAdapterProperties->adapter_bdf.function))
    {
        printf("ctlGetDeviceProperties returned invalid adapter BDF.\n");
    }
    else
    {
        printf("adapter_bdf.bus:%d\n", pStDeviceAdapterProperties->adapter_bdf.bus);
        printf("adapter_bdf.device:%d\n", pStDeviceAdapterProperties->adapter_bdf.device);
        printf("adapter_bdf.function:%d\n", pStDeviceAdapterProperties->adapter_bdf.function);
    }
}

/***************************************************************
 * @brief
 * To test an additional caller which will do it's own ctlInit
 * ctlEnumerate etc.
 * @param
 * @return
 ***************************************************************/
ctl_result_t CtlTestAdditionalCaller(void)
{
    ctl_result_t Result    = CTL_RESULT_SUCCESS;
    uint32_t Adapter_count = 0;

    // Blindly call ctlInit() again, do some work, then close
    ctl_init_args_t CtlInitArgs;
    ctl_api_handle_t hAPIHandle;
    ctl_device_adapter_handle_t *hDevices = nullptr;
    CtlInitArgs.AppVersion                = CTL_MAKE_VERSION(CTL_IMPL_MAJOR_VERSION, CTL_IMPL_MINOR_VERSION);
    CtlInitArgs.flags                     = 0;
    CtlInitArgs.Size                      = sizeof(CtlInitArgs);
    CtlInitArgs.Version                   = 0;
    // Init App UID appropriately
    ZeroMemory(&CtlInitArgs.ApplicationUID, sizeof(ctl_application_id_t));

    ctl_runtime_path_args_t Ctl_RuntimeArgs = { 0 };
    Ctl_RuntimeArgs.DeviceID                = 0x4096;
    Ctl_RuntimeArgs.RevID                   = 0x07;
    Ctl_RuntimeArgs.Size                    = sizeof(Ctl_RuntimeArgs);
    Ctl_RuntimeArgs.Version                 = 0;
    ctlSetRuntimePath(&Ctl_RuntimeArgs);

    Result = ctlInit(&CtlInitArgs, &hAPIHandle);

    if (CTL_RESULT_SUCCESS == Result)
    {
        // Initialization successful
        // Get the list of Intel Adapters

        Result = ctlEnumerateDevices(hAPIHandle, &Adapter_count, hDevices);

        if (CTL_RESULT_SUCCESS == Result)
        {
            hDevices = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * Adapter_count);
            if (hDevices == NULL)
            {
                return CTL_RESULT_ERROR_UNKNOWN;
            }
            Result = ctlEnumerateDevices(hAPIHandle, &Adapter_count, hDevices);
        }
        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlEnumerateDevices returned failure code: 0x%X\n", Result);
        }
        printf("ctlEnumerateDevices returned %d adapters\n", Adapter_count);
    }

    ctlClose(hAPIHandle);

    if (hDevices)
        free(hDevices);

    return Result;
}

void PrintDetailsFromSysman(ctl_device_adapter_handle_t hDevice)
{
    uint32_t FrequencyHandlerCount = 0;
    ctl_result_t res               = ctlEnumFrequencyDomains(hDevice, &FrequencyHandlerCount, nullptr);
    if ((res == CTL_RESULT_SUCCESS) && FrequencyHandlerCount > 0)
    {
        ctl_freq_handle_t *pFrequencyHandle = new ctl_freq_handle_t[FrequencyHandlerCount];
        res                                 = ctlEnumFrequencyDomains(hDevice, &FrequencyHandlerCount, pFrequencyHandle);

        for (uint32_t i = 0; i < FrequencyHandlerCount; i++)
        {
            ctl_freq_properties_t freqProperties = { 0 };
            freqProperties.Size                  = sizeof(ctl_freq_properties_t);
            res                                  = ctlFrequencyGetProperties(pFrequencyHandle[i], &freqProperties);

            if ((CTL_RESULT_SUCCESS == res) && (CTL_FREQ_DOMAIN_GPU == freqProperties.type))
            {
                printf("Max freq from L0 (ctl_freq_properties_t::max) = %f\n", freqProperties.max);
            }
        }

        delete[] pFrequencyHandle;
        pFrequencyHandle = nullptr;
    }
    if (res != CTL_RESULT_SUCCESS || 0 == FrequencyHandlerCount)
        printf("Couldn't get P0 from L0. Freq domains = %d, Error = 0x%X\n", FrequencyHandlerCount, res);

    return;
}

ctl_result_t CtlAdapterTesting(ctl_device_adapter_handle_t *hDevices, uint32_t Adapter_count)
{
    ctl_result_t Result                                       = CTL_RESULT_SUCCESS;
    ctl_display_output_handle_t *hDisplayOutput               = nullptr;
    ctl_device_adapter_properties_t StDeviceAdapterProperties = { 0 };

    uint32_t Display_count = 0;
    uint32_t Index         = 0;
    uint32_t Display_index = 0;

    LUID AdapterID;
    StDeviceAdapterProperties.Size           = sizeof(ctl_device_adapter_properties_t);
    StDeviceAdapterProperties.pDeviceID      = malloc(sizeof(LUID));
    StDeviceAdapterProperties.device_id_size = sizeof(LUID);
    StDeviceAdapterProperties.Version        = 3;

    for (Index = 0; Index < Adapter_count; Index++)
    {
        if (NULL != hDevices[Index])
        {
            printf("\n\n*** Testing adapter #%d ***\n", Index);

            Result = ctlGetDeviceProperties(hDevices[Index], &StDeviceAdapterProperties);

            if (CTL_RESULT_ERROR_UNSUPPORTED_VERSION == Result) // reduce version if required & recheck
            {
                printf("ctlGetDeviceProperties() version mismatch - Reducing version to 0 and retrying\n");
                StDeviceAdapterProperties.Version = 0;
                Result                            = ctlGetDeviceProperties(hDevices[Index], &StDeviceAdapterProperties);
            }

            if (Result != CTL_RESULT_SUCCESS)
            {
                printf("ctlGetDeviceProperties returned failure code: 0x%X\n", Result);
                continue;
            }

            if (CTL_DEVICE_TYPE_GRAPHICS != StDeviceAdapterProperties.device_type)
            {
                printf("This is not a Graphics device \n");
                continue;
            }

            if (NULL != StDeviceAdapterProperties.pDeviceID)
            {
                AdapterID = *(reinterpret_cast<LUID *>(StDeviceAdapterProperties.pDeviceID));
                std::cout << "Adapter ID " << AdapterID.LowPart << "\n";
            }

            if (0x8086 != StDeviceAdapterProperties.pci_vendor_id)
                continue;

            PrintAdapterProperties(&StDeviceAdapterProperties);

            // get max/P0 from L0 & print the same here
            try
            {
                PrintDetailsFromSysman(hDevices[Index]);
            }
            catch (const std::bad_array_new_length &e)
            {
                printf("%s \n", e.what());
            }

            // enumerate all the possible target display's for the adapters
            // first step is to get the count
            Display_count = 0;
            Result        = ctlEnumerateDisplayOutputs(hDevices[Index], &Display_count, hDisplayOutput);

            printf("ctlEnumerateDisplayOutputs returned %d encoders\n", Display_count);

            if (CTL_RESULT_SUCCESS == Result && (Display_count > 0))
            {
                hDisplayOutput = (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * Display_count);
                if (NULL == hDisplayOutput)
                {
                    return CTL_RESULT_ERROR_UNKNOWN;
                }
                Result = ctlEnumerateDisplayOutputs(hDevices[Index], &Display_count, hDisplayOutput);
            }

            if (Result != CTL_RESULT_SUCCESS)
            {
                printf("ctlEnumerateDisplayOutputs returned failure code: 0x%X\n", Result);
                STORE_RESET_ERROR(Result);
            }

            // get display encoder properties
            if (CTL_RESULT_SUCCESS == Result && hDisplayOutput)
            {
                GetDisplayEncoderPropertiesTest(hDisplayOutput, &Result, Display_count);
            }

            // get display properties
            if (CTL_RESULT_SUCCESS == Result && hDisplayOutput)
            {
                GetDisplayPropertiesTest(hDevices, hDisplayOutput, &Result, Index, Display_count);
            }

            // Sharpness Test
            if (CTL_RESULT_SUCCESS == Result)
            {
                if (nullptr == hDisplayOutput)
                    return CTL_RESULT_ERROR_UNKNOWN;

                for (Display_index = 0; Display_index < Display_count; Display_index++)
                {
                    Result = CtlSharpnessTest(hDisplayOutput[Display_index]);
                    STORE_RESET_ERROR(Result);
                }
            }

            // get 3D global properties
            if (CTL_RESULT_SUCCESS == Result)
            {
                try
                {
                    Result = CtlGet3DGlobalTest(hDevices[Index]);
                    STORE_RESET_ERROR(Result);
                }
                catch (const std::bad_array_new_length &e)
                {
                    printf("%s \n", e.what());
                }
            }
        }
        if (hDisplayOutput != nullptr)
        {
            free(hDisplayOutput);
            hDisplayOutput = nullptr;
        }
    }

    return Result;
}

/***************************************************************
 * @brief Main Function
 *
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
int main()
{
    ctl_result_t Result                                       = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hDevices                     = nullptr;
    ctl_display_output_handle_t *hDisplayOutput               = nullptr;
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
    CtlInitArgs.flags      = 0; // CTL_INIT_FLAG_USE_LEVEL_ZERO;
    CtlInitArgs.Size       = sizeof(CtlInitArgs);
    CtlInitArgs.Version    = 0;
    // Init App UID appropriately
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
        printf("IGCL Version in platform: %d.%d\n", CTL_MAJOR_VERSION(CtlInitArgs.SupportedVersion), CTL_MINOR_VERSION(CtlInitArgs.SupportedVersion));

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
                return ERROR;
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
        printf("ctlEnumerateDevices returned %d adapters\n", Adapter_count);

        StDeviceAdapterProperties.Size           = sizeof(ctl_device_adapter_properties_t);
        StDeviceAdapterProperties.pDeviceID      = malloc(sizeof(LUID));
        StDeviceAdapterProperties.device_id_size = sizeof(LUID);
        StDeviceAdapterProperties.Version        = 2;

        if (NULL == StDeviceAdapterProperties.pDeviceID)
        {
            return ERROR;
        }

        // test an additional caller from same process
        try
        {
            CtlTestAdditionalCaller();
        }
        catch (const std::bad_array_new_length &e)
        {
            printf("%s \n", e.what());
        }

        try
        {
            CtlAdapterTesting(hDevices, Adapter_count);
        }
        catch (const std::ios_base::failure &e)
        {
            printf("%s \n", e.what());
        }

#ifdef _ZE_DDI_H
        // test level0 handle
        if ((CTL_RESULT_SUCCESS == Result) && (Adapter_count > 0))
        {
            for (unsigned int i = 0; i < Adapter_count; i++)
            {
                Result = CtlLevel0HandleTest(hDevices[i]);
                if (Result != CTL_RESULT_SUCCESS)
                {
                    printf("ctllevel0handle returned failure code: 0x%X\n", Result);
                }
            }
        }

#endif
        // Test eventing
        if ((CTL_RESULT_SUCCESS == Result) && (Adapter_count > 0))
        {
            CtlTestEvents(hDevices[0]); // for now just first adapter!
        }
    }
    else
    {
        STORE_RESET_ERROR(Result);
    }
Exit:

    if (NULL != StDeviceAdapterProperties.pDeviceID)
    {
        free(StDeviceAdapterProperties.pDeviceID);
        StDeviceAdapterProperties.pDeviceID = NULL;
    }

    ctlClose(hAPIHandle);

    CTL_FREE_MEM(hDisplayOutput);
    CTL_FREE_MEM(hDevices);

    printf("Overrall test result is 0x%X\n", GResult);
    return GResult;
}
