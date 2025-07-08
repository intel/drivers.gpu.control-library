//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  Sample_OverclockAPP.cpp
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
#include <vector>
#include <string>
#include "igcl_api.h"
#include "GenericIGCLApp.h"
#include <map>

void OverclockProperties(ctl_device_adapter_handle_t hDAhandle);
void OverclockFrequencyOffset(ctl_device_adapter_handle_t hDAhandle);
void OverclockVoltageOffset(ctl_device_adapter_handle_t hDAhandle);
void OverclockLockFrequency(ctl_device_adapter_handle_t hDAhandle);
void OverclockPowerLimit(ctl_device_adapter_handle_t hDAhandle);
void OverclockTemperatureLimit(ctl_device_adapter_handle_t hDAhandle);
void OverclockVramMemSpeedLimit(ctl_device_adapter_handle_t hDAhandle);
void OverclockVFCurveReadWrite(ctl_device_adapter_handle_t hDAhandle);
void OverclockPowerTelemetry(ctl_device_adapter_handle_t hDAhandle);

std::string DecodeCtlDataType(ctl_data_type_t Type)
{
    static const std::map<ctl_data_type_t, std::string> dataTypeStringMap = { { CTL_DATA_TYPE_INT8, "INT8" },
                                                                              { CTL_DATA_TYPE_UINT8, "UINT8" },
                                                                              { CTL_DATA_TYPE_INT16, "INT16" },
                                                                              { CTL_DATA_TYPE_UINT16, "UINT16" },
                                                                              { CTL_DATA_TYPE_INT32, "INT32" },
                                                                              { CTL_DATA_TYPE_UINT32, "UINT32" },
                                                                              { CTL_DATA_TYPE_INT64, "INT64" },
                                                                              { CTL_DATA_TYPE_UINT64, "UINT64" },
                                                                              { CTL_DATA_TYPE_FLOAT, "FLOAT" },
                                                                              { CTL_DATA_TYPE_DOUBLE, "DOUBLE" },
                                                                              { CTL_DATA_TYPE_STRING_ASCII, "STRING_ASCII" },
                                                                              { CTL_DATA_TYPE_STRING_UTF16, "STRING_UTF16" },
                                                                              { CTL_DATA_TYPE_STRING_UTF132, "STRING_UTF132" } };

    auto it = dataTypeStringMap.find(Type);
    if (it != dataTypeStringMap.end())
    {
        return it->second;
    }
    return "Unknown datatype";
}

std::string DecodeCtlUnits(ctl_units_t Units)
{
    static const std::map<ctl_units_t, std::string> unitsStringMap = { { CTL_UNITS_FREQUENCY_MHZ, "Frequency in MHz" },
                                                                       { CTL_UNITS_OPERATIONS_GTS, "GigaOperations per Second" },
                                                                       { CTL_UNITS_OPERATIONS_MTS, "MegaOperations per Second" },
                                                                       { CTL_UNITS_VOLTAGE_VOLTS, "Voltage in Volts" },
                                                                       { CTL_UNITS_POWER_WATTS, "Power in Watts" },
                                                                       { CTL_UNITS_TEMPERATURE_CELSIUS, "Temperature in Celsius" },
                                                                       { CTL_UNITS_ENERGY_JOULES, "Energy in Joules" },
                                                                       { CTL_UNITS_TIME_SECONDS, "Time in Seconds" },
                                                                       { CTL_UNITS_MEMORY_BYTES, "Memory in Bytes" },
                                                                       { CTL_UNITS_ANGULAR_SPEED_RPM, "Angular Speed in RPM" },
                                                                       { CTL_UNITS_POWER_MILLIWATTS, "Power in Milli Watts" },
                                                                       { CTL_UNITS_PERCENT, "Units in Percentage" },
                                                                       { CTL_UNITS_MEM_SPEED_GBPS, "Units in Gigabyte Per Second" },
                                                                       { CTL_UNITS_VOLTAGE_MILLIVOLTS, "Voltage in MilliVolts" },
                                                                       { CTL_UNITS_BANDWIDTH_MBPS, "Bandwidth in MegaBytes Per Second" } };

    auto it = unitsStringMap.find(Units);
    if (it != unitsStringMap.end())
    {
        return it->second;
    }
    return "Unknown unit";
}

std::string DecodeVFCurveDetails(ctl_vf_curve_details_t VFCurveDetails)
{
    static const std::map<ctl_vf_curve_details_t, std::string> vfCurveDetailsMap = { { CTL_VF_CURVE_DETAILS_SIMPLIFIED, "[CTL_VF_CURVE_DETAILS_SIMPLIFIED]" },
                                                                                     { CTL_VF_CURVE_DETAILS_MEDIUM, "[CTL_VF_CURVE_DETAILS_MEDIUM]" },
                                                                                     { CTL_VF_CURVE_DETAILS_ELABORATE, "[CTL_VF_CURVE_DETAILS_ELABORATE]" } };

    auto it = vfCurveDetailsMap.find(VFCurveDetails);
    if (it != vfCurveDetailsMap.end())
    {
        return it->second;
    }
    return "Unknown vfcurvedetails";
}

std::string DecodeBoolean(bool Value)
{
    return Value ? "true" : "false";
}

// Decoding the return code for the most common error codes.
std::string DecodeRetCode(ctl_result_t Res)
{
    static const std::map<ctl_result_t, std::string> retCodeMap = { { CTL_RESULT_SUCCESS, "[CTL_RESULT_SUCCESS]" },
                                                                    { CTL_RESULT_ERROR_CORE_OVERCLOCK_NOT_SUPPORTED, "[CTL_RESULT_ERROR_CORE_OVERCLOCK_NOT_SUPPORTED]" },
                                                                    { CTL_RESULT_ERROR_CORE_OVERCLOCK_VOLTAGE_OUTSIDE_RANGE, "[CTL_RESULT_ERROR_CORE_OVERCLOCK_VOLTAGE_OUTSIDE_RANGE]" },
                                                                    { CTL_RESULT_ERROR_CORE_OVERCLOCK_FREQUENCY_OUTSIDE_RANGE, "[CTL_RESULT_ERROR_CORE_OVERCLOCK_FREQUENCY_OUTSIDE_RANGE]" },
                                                                    { CTL_RESULT_ERROR_CORE_OVERCLOCK_POWER_OUTSIDE_RANGE, "[CTL_RESULT_ERROR_CORE_OVERCLOCK_POWER_OUTSIDE_RANGE]" },
                                                                    { CTL_RESULT_ERROR_CORE_OVERCLOCK_TEMPERATURE_OUTSIDE_RANGE, "[CTL_RESULT_ERROR_CORE_OVERCLOCK_TEMPERATURE_OUTSIDE_RANGE]" },
                                                                    { CTL_RESULT_ERROR_GENERIC_START, "[CTL_RESULT_ERROR_GENERIC_START]" },
                                                                    { CTL_RESULT_ERROR_CORE_OVERCLOCK_RESET_REQUIRED, "[CTL_RESULT_ERROR_CORE_OVERCLOCK_RESET_REQUIRED]" },
                                                                    { CTL_RESULT_ERROR_CORE_OVERCLOCK_IN_VOLTAGE_LOCKED_MODE, "[CTL_RESULT_ERROR_CORE_OVERCLOCK_IN_VOLTAGE_LOCKED_MODE]" },
                                                                    { CTL_RESULT_ERROR_CORE_OVERCLOCK_WAIVER_NOT_SET, "[CTL_RESULT_ERROR_CORE_OVERCLOCK_WAIVER_NOT_SET]" },
                                                                    { CTL_RESULT_ERROR_NOT_INITIALIZED, "[CTL_RESULT_ERROR_NOT_INITIALIZED]" },
                                                                    { CTL_RESULT_ERROR_ALREADY_INITIALIZED, "[CTL_RESULT_ERROR_ALREADY_INITIALIZED]" },
                                                                    { CTL_RESULT_ERROR_DEVICE_LOST, "[CTL_RESULT_ERROR_DEVICE_LOST]" },
                                                                    { CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS, "[CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS]" },
                                                                    { CTL_RESULT_ERROR_NOT_AVAILABLE, "[CTL_RESULT_ERROR_NOT_AVAILABLE]" },
                                                                    { CTL_RESULT_ERROR_UNINITIALIZED, "[CTL_RESULT_ERROR_UNINITIALIZED]" },
                                                                    { CTL_RESULT_ERROR_UNSUPPORTED_VERSION, "[CTL_RESULT_ERROR_UNSUPPORTED_VERSION]" },
                                                                    { CTL_RESULT_ERROR_UNSUPPORTED_FEATURE, "[CTL_RESULT_ERROR_UNSUPPORTED_FEATURE]" },
                                                                    { CTL_RESULT_ERROR_INVALID_ARGUMENT, "[CTL_RESULT_ERROR_INVALID_ARGUMENT]" },
                                                                    { CTL_RESULT_ERROR_INVALID_NULL_HANDLE, "[CTL_RESULT_ERROR_INVALID_NULL_HANDLE]" },
                                                                    { CTL_RESULT_ERROR_INVALID_NULL_POINTER, "[CTL_RESULT_ERROR_INVALID_NULL_POINTER]" },
                                                                    { CTL_RESULT_ERROR_INVALID_SIZE, "[CTL_RESULT_ERROR_INVALID_SIZE]" },
                                                                    { CTL_RESULT_ERROR_UNSUPPORTED_SIZE, "[CTL_RESULT_ERROR_UNSUPPORTED_SIZE]" },
                                                                    { CTL_RESULT_ERROR_NOT_IMPLEMENTED, "[CTL_RESULT_ERROR_NOT_IMPLEMENTED]" },
                                                                    { CTL_RESULT_ERROR_ZE_LOADER, "[CTL_RESULT_ERROR_ZE_LOADER]" },
                                                                    { CTL_RESULT_ERROR_INVALID_OPERATION_TYPE, "[CTL_RESULT_ERROR_INVALID_OPERATION_TYPE]" },
                                                                    { CTL_RESULT_ERROR_DATA_READ, "[CTL_RESULT_ERROR_DATA_READ]" },
                                                                    { CTL_RESULT_ERROR_DATA_WRITE, "[CTL_RESULT_ERROR_DATA_WRITE]" },
                                                                    { CTL_RESULT_ERROR_CORE_OVERCLOCK_INVALID_CUSTOM_VF_CURVE, "[CTL_RESULT_ERROR_CORE_OVERCLOCK_INVALID_CUSTOM_VF_CURVE]" },
                                                                    { CTL_RESULT_ERROR_UNKNOWN, "[CTL_RESULT_ERROR_UNKNOWN]" },
                                                                    { CTL_RESULT_ERROR_CORE_OVERCLOCK_DEPRECATED_API, "[CTL_RESULT_ERROR_CORE_OVERCLOCK_DEPRECATED_API]" } };

    auto it = retCodeMap.find(Res);
    if (it != retCodeMap.end())
    {
        return it->second;
    }
    return "[Unknown Error]";
}

/***************************************************************
 * @brief OverclockProperties
 * Overclock Properties: The function ctlOverclockGetProperties
 * retrieves all the necessary information to populate a GUI. For
 * each property the following is provided:
 *      Min
 *      Max
 *      Step
 *      Default
 *      Reference
 *      Supported
 *
 * This aimed to populate the GUI controls.
 * @param
 * @return
 ***************************************************************/
void OverclockProperties(ctl_device_adapter_handle_t hDAhandle)
{
    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    OcProperties.Version             = 1;

    ctl_result_t Status = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockProperties => ctlOverclockGetProperties Error: %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }

    // Is Overclocking supported by this adapter?
    PRINT_LOGS("\nOc Supported? %s", DecodeBoolean(OcProperties.bSupported).c_str());

    // Slider for frequency offset
    PRINT_LOGS("\nGpu Frequency Offset Supported? %s ", DecodeBoolean(OcProperties.gpuFrequencyOffset.bSupported).c_str());
    PRINT_LOGS("\nGpu Frequency Offset Is Relative? %s", DecodeBoolean(OcProperties.gpuFrequencyOffset.bRelative).c_str());
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", DecodeBoolean(OcProperties.gpuFrequencyOffset.bReference).c_str());
    PRINT_LOGS("\nGpu Frequency Offset Default: %lf", OcProperties.gpuFrequencyOffset.Default);
    PRINT_LOGS("\nGpu Frequency Offset Min: %lf", OcProperties.gpuFrequencyOffset.min);
    PRINT_LOGS("\nGpu Frequency Offset Max: %lf", OcProperties.gpuFrequencyOffset.max);
    PRINT_LOGS("\nGpu Frequency Offset Reference: %lf", OcProperties.gpuFrequencyOffset.reference);
    PRINT_LOGS("\nGpu Frequency Offset Step: %lf", OcProperties.gpuFrequencyOffset.step);
    PRINT_LOGS("\nGpu Frequency Offset Units:: %s\n", DecodeCtlUnits(OcProperties.gpuFrequencyOffset.units).c_str());

    // Slider for voltage offset
    PRINT_LOGS("\nGpu Voltage Offset Supported? %s ", DecodeBoolean(OcProperties.gpuVoltageOffset.bSupported).c_str());
    PRINT_LOGS("\nGpu Voltage Offset Is Relative? %s", DecodeBoolean(OcProperties.gpuVoltageOffset.bRelative).c_str());
    PRINT_LOGS("\nGpu Voltage Offset Have Reference? %s", DecodeBoolean(OcProperties.gpuVoltageOffset.bReference).c_str());
    PRINT_LOGS("\nGpu Voltage Offset Default: %lf", OcProperties.gpuVoltageOffset.Default);
    PRINT_LOGS("\nGpu Voltage Offset Min : %lf", OcProperties.gpuVoltageOffset.min);
    PRINT_LOGS("\nGpu Voltage Offset Max: %lf", OcProperties.gpuVoltageOffset.max);
    PRINT_LOGS("\nGpu Voltage Offset Reference: %lf", OcProperties.gpuVoltageOffset.reference);
    PRINT_LOGS("\nGpu Voltage Offset Step: %lf", OcProperties.gpuVoltageOffset.step);
    PRINT_LOGS("\nGpu Voltage Offset Units: %s\n", DecodeCtlUnits(OcProperties.gpuVoltageOffset.units).c_str());

    // Slider for VRAM Mem Speed Limit
    PRINT_LOGS("\nVram Memory Speed Limit Supported? %s ", DecodeBoolean(OcProperties.vramMemSpeedLimit.bSupported).c_str());
    PRINT_LOGS("\nVram Memory Speed Limit Is Relative? %s", DecodeBoolean(OcProperties.vramMemSpeedLimit.bRelative).c_str());
    PRINT_LOGS("\nVram Memory Speed Limit Have Reference? %s", DecodeBoolean(OcProperties.vramMemSpeedLimit.bReference).c_str());
    PRINT_LOGS("\nVram Memory Speed Limit Default: %lf", OcProperties.vramMemSpeedLimit.Default);
    PRINT_LOGS("\nVram Memory Speed Limit Min : %lf", OcProperties.vramMemSpeedLimit.min);
    PRINT_LOGS("\nVram Memory Speed Limit Max: %lf", OcProperties.vramMemSpeedLimit.max);
    PRINT_LOGS("\nVram Memory Speed Limit Reference: %lf", OcProperties.vramMemSpeedLimit.reference);
    PRINT_LOGS("\nVram Memory Speed Limit Step: %lf", OcProperties.vramMemSpeedLimit.step);
    PRINT_LOGS("\nVram Memory Speed Limit Units: %s\n", DecodeCtlUnits(OcProperties.vramMemSpeedLimit.units).c_str());

    // Slider for Power Limit
    PRINT_LOGS("\nPower Limit Supported? %s ", DecodeBoolean(OcProperties.powerLimit.bSupported).c_str());
    PRINT_LOGS("\nPower Limit Is Relative? %s", DecodeBoolean(OcProperties.powerLimit.bRelative).c_str());
    PRINT_LOGS("\nPower Limit Have Reference? %s", DecodeBoolean(OcProperties.powerLimit.bReference).c_str());
    PRINT_LOGS("\nPower Limit Default: %lf", OcProperties.powerLimit.Default);
    PRINT_LOGS("\nPower Limit Min: %lf", OcProperties.powerLimit.min);
    PRINT_LOGS("\nPower Limit Max: %lf", OcProperties.powerLimit.max);
    PRINT_LOGS("\nPower Limit Reference: %lf", OcProperties.powerLimit.reference);
    PRINT_LOGS("\nPower Limit Step: %lf", OcProperties.powerLimit.step);
    PRINT_LOGS("\nPower Limit Units: %s\n", DecodeCtlUnits(OcProperties.powerLimit.units).c_str());

    // Slider for Temperature Limit
    PRINT_LOGS("\nTemperature Limit Supported? %s ", DecodeBoolean(OcProperties.temperatureLimit.bSupported).c_str());
    PRINT_LOGS("\nTemperature Limit Is Relative? %s", DecodeBoolean(OcProperties.temperatureLimit.bRelative).c_str());
    PRINT_LOGS("\nTemperature Limit Have Reference? %s", DecodeBoolean(OcProperties.temperatureLimit.bReference).c_str());
    PRINT_LOGS("\nTemperature Limit Default: %lf", OcProperties.temperatureLimit.Default);
    PRINT_LOGS("\nTemperature Limit Min: %lf", OcProperties.temperatureLimit.min);
    PRINT_LOGS("\nTemperature Limit Max: %lf", OcProperties.temperatureLimit.max);
    PRINT_LOGS("\nTemperature Limit Reference: %lf", OcProperties.temperatureLimit.reference);
    PRINT_LOGS("\nTemperature Limit Step: %lf", OcProperties.temperatureLimit.step);
    PRINT_LOGS("\nTemperature Limit Units: %s\n", DecodeCtlUnits(OcProperties.temperatureLimit.units).c_str());

    // Voltage Frequency Curve Property
    PRINT_LOGS("\nVF Curve R/W Supported? %s ", DecodeBoolean(OcProperties.gpuVFCurveVoltageLimit.bSupported).c_str());
    PRINT_LOGS("\nVF Curve Voltage Axis Min: %lf", OcProperties.gpuVFCurveVoltageLimit.min);
    PRINT_LOGS("\nVF Curve Voltage Axis Max: %lf", OcProperties.gpuVFCurveVoltageLimit.max);
    PRINT_LOGS("\nVF Curve Voltage Axis Step: %lf", OcProperties.gpuVFCurveVoltageLimit.step);
    PRINT_LOGS("\nVF Curve Voltage Axis Units: %s\n", DecodeCtlUnits(OcProperties.gpuVFCurveVoltageLimit.units).c_str());

    PRINT_LOGS("\nVF Curve R/W Supported? %s ", DecodeBoolean(OcProperties.gpuVFCurveFrequencyLimit.bSupported).c_str());
    PRINT_LOGS("\nVF Curve Freq Axis Min: %lf", OcProperties.gpuVFCurveFrequencyLimit.min);
    PRINT_LOGS("\nVF Curve Freq Axis Max: %lf", OcProperties.gpuVFCurveFrequencyLimit.max);
    PRINT_LOGS("\nVF Curve Freq Axis Step: %lf", OcProperties.gpuVFCurveFrequencyLimit.step);
    PRINT_LOGS("\nVF Curve Freq Axis Units: %s", DecodeCtlUnits(OcProperties.gpuVFCurveFrequencyLimit.units).c_str());
}

/***************************************************************
 * @brief OverclockFrequencyOffset
 *
 * Overclock Frequency Offset: The function ctlOverclockGpuFrequencyOffsetSetV2
 * allows to set a positive frequency offset.
 * @param
 * @return
 ***************************************************************/
void OverclockFrequencyOffset(ctl_device_adapter_handle_t hDAhandle)
{
    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    OcProperties.Version             = 1;

    // Step 1: Reading Overclock Properties
    ctl_result_t Status = ctlOverclockGetProperties(hDAhandle, &OcProperties);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockFrequencyOffset => ctlOverclockGetProperties Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }

    // Step 2: Reading Current Frequency Offset
    // Output Frequency Offset units are returned in ctl_oc_properties_t::gpuFrequencyOffset::units returned from ctlOverclockGetProperties()
    double GPUFrequencyOffset = 0.0;
    Status                    = ctlOverclockGpuFrequencyOffsetGetV2(hDAhandle, &GPUFrequencyOffset);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockFrequencyOffset => ctlOverclockGpuFrequencyOffsetGetV2 Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\n\nCurrent Frequency Offset: %lf, %s", GPUFrequencyOffset, DecodeCtlUnits(OcProperties.gpuFrequencyOffset.units).c_str());

    // Step 3: Writing New Frequency Offset by increasing it by (Step * 5.0) MHz from min value
    // Input Frequency Offset units are given in ctl_oc_properties_t::gpuFrequencyOffset::units returned from ctlOverclockGetProperties()
    // Currently ctl_oc_properties_t::gpuFrequencyOffset::units for both Alchemist and Battlemage are CTL_UNITS_FREQUENCY_MHZ units
    GPUFrequencyOffset = OcProperties.gpuFrequencyOffset.min + (OcProperties.gpuFrequencyOffset.step * 5.0);
    Status             = ctlOverclockGpuFrequencyOffsetSetV2(hDAhandle, GPUFrequencyOffset);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockFrequencyOffset => ctlOverclockGpuFrequencyOffsetSetV2 Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nSetting New Frequency Offset: %lf, %s", GPUFrequencyOffset, DecodeCtlUnits(OcProperties.gpuFrequencyOffset.units).c_str());

    // Step 4: Reading back New Frequency Offset
    // Output Frequency Offset units are returned in ctl_oc_properties_t::gpuFrequencyOffset::units returned from ctlOverclockGetProperties()
    GPUFrequencyOffset = 0.0;
    Status             = ctlOverclockGpuFrequencyOffsetGetV2(hDAhandle, &GPUFrequencyOffset);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockFrequencyOffset => ctlOverclockGpuFrequencyOffsetGetV2 after setting, Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nReading New Frequency Offset: %lf, %s", GPUFrequencyOffset, DecodeCtlUnits(OcProperties.gpuFrequencyOffset.units).c_str());

    // Step 5: Resetting Frequency Offset
    // Input Frequency Offset units are given in ctl_oc_properties_t::gpuFrequencyOffset::units returned from ctlOverclockGetProperties()
    // Currently ctl_oc_properties_t::gpuFrequencyOffset::units for both Alchemist and Battlemage are CTL_UNITS_FREQUENCY_MHZ units
    GPUFrequencyOffset = OcProperties.gpuFrequencyOffset.Default;
    Status             = ctlOverclockGpuFrequencyOffsetSetV2(hDAhandle, GPUFrequencyOffset);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockFrequencyOffset => ctlOverclockGpuFrequencyOffsetSetV2 after resetting, Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nResetting Frequency Offset to Default Value: %lf, %s", GPUFrequencyOffset, DecodeCtlUnits(OcProperties.gpuFrequencyOffset.units).c_str());
}

/***************************************************************
 * @brief OverclockVoltageOffset
 *
 * Overclock Voltage Offset: The function ctlOverclockGpuMaxVoltageOffsetSetV2
 * allows to set a positive voltage offset.
 * @param
 * @return
 ***************************************************************/
void OverclockVoltageOffset(ctl_device_adapter_handle_t hDAhandle)
{
    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    OcProperties.Version             = 1;

    // Step 1: Reading Overclock Properties
    ctl_result_t Status = ctlOverclockGetProperties(hDAhandle, &OcProperties);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVoltageOffset => ctlOverclockGetProperties Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }

    // Step 2: Reading Current Voltage Offset
    // Output Voltage Offset units are returned in ctl_oc_properties_t::gpuVoltageOffset::units returned from ctlOverclockGetProperties()
    double VoltageOffset = 0.0;
    Status               = ctlOverclockGpuMaxVoltageOffsetGetV2(hDAhandle, &VoltageOffset);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVoltageOffset => ctlOverclockGpuMaxVoltageOffsetGetV2 Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\n\nCurrent Voltage Offset: %lf, %s", VoltageOffset, DecodeCtlUnits(OcProperties.gpuVoltageOffset.units).c_str());

    // Step 3: Calling Waiver Set first before writing the Voltage Offset
    Status = ctlOverclockWaiverSet(hDAhandle);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVoltageOffset => ctlOverclockWaiverSet Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nOverclock Waiver Set Succesfully for ctlOverclockGpuMaxVoltageOffsetSetV2 API !!");

    // Step 4: Writing New Voltage Offset by increasing it by (Step * 5.0) from min value
    // Input Voltage Offset units are given in ctl_oc_properties_t::gpuVoltageOffset::units returned from ctlOverclockGetProperties()
    // Currently ctl_oc_properties_t::gpuVoltageOffset::units for Alchemist are CTL_UNITS_VOLTAGE_VOLTS units, for Battlemage are CTL_UNITS_PERCENT units
    VoltageOffset = OcProperties.gpuVoltageOffset.min + (OcProperties.gpuVoltageOffset.step * 5.0);
    Status        = ctlOverclockGpuMaxVoltageOffsetSetV2(hDAhandle, VoltageOffset);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVoltageOffset => ctlOverclockGpuMaxVoltageOffsetSetV2 Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nSetting New Voltage Offset: %lf, %s", VoltageOffset, DecodeCtlUnits(OcProperties.gpuVoltageOffset.units).c_str());

    // Step 5: Reading back New Voltage Offset
    // Output Voltage Offset units are returned in ctl_oc_properties_t::gpuVoltageOffset::units returned from ctlOverclockGetProperties()
    VoltageOffset = 0.0;
    Status        = ctlOverclockGpuMaxVoltageOffsetGetV2(hDAhandle, &VoltageOffset);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVoltageOffset => ctlOverclockGpuMaxVoltageOffsetGetV2 after setting, Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nReading New Voltage Offset: %lf %s", VoltageOffset, DecodeCtlUnits(OcProperties.gpuVoltageOffset.units).c_str());

    // Step 6: Resetting Voltage Offset
    // Input Voltage Offset units are given in ctl_oc_properties_t::gpuVoltageOffset::units returned from ctlOverclockGetProperties()
    // Currently ctl_oc_properties_t::gpuVoltageOffset::units for Alchemist are CTL_UNITS_VOLTAGE_VOLTS units, for Battlemage are CTL_UNITS_PERCENT units
    VoltageOffset = OcProperties.gpuVoltageOffset.Default;
    Status        = ctlOverclockGpuMaxVoltageOffsetSetV2(hDAhandle, VoltageOffset);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVoltageOffset => ctlOverclockGpuMaxVoltageOffsetSetV2 after resetting, Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nResetting Voltage Offset to Default Value: %lf, %s", VoltageOffset, DecodeCtlUnits(OcProperties.gpuVoltageOffset.units).c_str());
}

/***************************************************************
 * @brief OverclockLockFrequency
 * Overclock Lock Frequency: The function ctlOverclockGpuLockSet
 * allows to fix the frequency and voltage, useful for OC scanners
 * or performance testing.
 * @param
 * @return
 ***************************************************************/
void OverclockLockFrequency(ctl_device_adapter_handle_t hDAhandle)
{
    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    OcProperties.Version             = 1;

    // Step 1: Reading Overclock Properties
    ctl_result_t Status = ctlOverclockGetProperties(hDAhandle, &OcProperties);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockLockFrequency => ctlOverclockGetProperties Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }

    // ctlOverclockGpuLockGet, ctlOverclockGpuLockSet APIs are only supported for Alchemist, not supported for Battlemage
    // Step 2: Reading Current Locked OC VF Pair
    ctl_oc_vf_pair_t VfPair = {};
    Status                  = ctlOverclockGpuLockGet(hDAhandle, &VfPair);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockLockFrequency => ctlOverclockGpuLockGet Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nCurrent Locked OC VF Pair - Locked Frequency: %lf MHz, Locked Voltage: %lf mV", VfPair.Frequency, VfPair.Voltage);

    // Step 3: Calling Waiver Set first before writing the Locked OC VF Pair
    Status = ctlOverclockWaiverSet(hDAhandle);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockLockFrequency => ctlOverclockWaiverSet Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }

    // Step 3: Setting New Locked OC VF Pair
    // Set slighly more frequency and voltage
    // 50 MHz over reference frequency
    VfPair           = {};
    VfPair.Frequency = OcProperties.gpuFrequencyOffset.reference + 50.0;
    // 50 mV over reference voltage. Convert to mV first.
    VfPair.Voltage = OcProperties.gpuVoltageOffset.reference * 1000.0 + 50.0;
    Status         = ctlOverclockGpuLockSet(hDAhandle, VfPair);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockLockFrequency => ctlOverclockGpuLockSet Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nSetting New Locked OC VF Pair - Locked Frequency: %lf MHz, Locked Voltage: %lf mV", VfPair.Frequency, VfPair.Voltage);

    // Step 4: Reading back New Locked OC VF Pair
    VfPair = {};
    Status = ctlOverclockGpuLockGet(hDAhandle, &VfPair);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockLockFrequency => ctlOverclockGpuLockGet Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nReading New Locked OC VF Pair - Locked Frequency: %lf MHz, Locked Voltage: %lf mV", VfPair.Frequency, VfPair.Voltage);

    // Step 5: Resetting Locked OC VF Pair
    VfPair = {};
    Status = ctlOverclockGpuLockSet(hDAhandle, VfPair);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockLockFrequency => ctlOverclockGpuLockSet Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nResetting Locked OC VF Pair - Locked Frequency: %lf MHz, Locked Voltage: %lf mV", VfPair.Frequency, VfPair.Voltage);
}

/*****************************************************************
 * @brief OverclockPowerLimit
 *
 * Overclock Power Limit: The function ctlOverclockPowerLimitSetV2
 * allows to increase or decrease the power (TDP) budget for the
 * adapter.
 * @param
 * @return
 *****************************************************************/
void OverclockPowerLimit(ctl_device_adapter_handle_t hDAhandle)
{
    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    OcProperties.Version             = 1;

    // Step 1: Reading Overclock Properties
    ctl_result_t Status = ctlOverclockGetProperties(hDAhandle, &OcProperties);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockPowerLimit => ctlOverclockGetProperties Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }

    // Step 2: Reading Current Sustained Power Limit
    // Output Sustained Power Limit units are returned in ctl_oc_properties_t::powerLimit::units returned from ctlOverclockGetProperties()
    double CurrentPowerLimit = 0.0;
    Status                   = ctlOverclockPowerLimitGetV2(hDAhandle, &CurrentPowerLimit);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockPowerLimit => ctlOverclockPowerLimitGetV2 Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\n\nCurrent Sustained Power Limit: %lf, %s", CurrentPowerLimit, DecodeCtlUnits(OcProperties.powerLimit.units).c_str());

    // Step 3: Writing New Sustained Power Limit by increasing it by (Step * 5.0) from min value
    // Input Sustained Power Limit units are given in ctl_oc_properties_t::powerLimit::units returned from ctlOverclockGetProperties()
    // Currently ctl_oc_properties_t::powerLimit::units for Alchemist are CTL_UNITS_POWER_WATTS units, for Battlemage are CTL_UNITS_PERCENT units
    CurrentPowerLimit = OcProperties.powerLimit.min + (OcProperties.powerLimit.step * 5.0);
    Status            = ctlOverclockPowerLimitSetV2(hDAhandle, CurrentPowerLimit);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockPowerLimit => ctlOverclockPowerLimitSetV2 Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nSetting New Sustained Power Limit: %lf, %s", CurrentPowerLimit, DecodeCtlUnits(OcProperties.powerLimit.units).c_str());

    // Step 4: Reading back New Sustained Power Limit
    // Output Sustained Power Limit units are returned in ctl_oc_properties_t::powerLimit::units returned from ctlOverclockGetProperties()
    CurrentPowerLimit = 0.0;
    Status            = ctlOverclockPowerLimitGetV2(hDAhandle, &CurrentPowerLimit);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockPowerLimit => ctlOverclockPowerLimitGetV2 after setting, Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nReading New Sustained Power Limit: %lf, %s", CurrentPowerLimit, DecodeCtlUnits(OcProperties.powerLimit.units).c_str());

    // Step 5: Resetting Sustained Power Limit
    // Input Sustained Power Limit units are given in ctl_oc_properties_t::powerLimit::units returned from ctlOverclockGetProperties()
    // Currently ctl_oc_properties_t::powerLimit::units for Alchemist are CTL_UNITS_POWER_WATTS units, for Battlemage are CTL_UNITS_PERCENT units
    CurrentPowerLimit = OcProperties.powerLimit.Default;
    Status            = ctlOverclockPowerLimitSetV2(hDAhandle, CurrentPowerLimit);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockPowerLimit => ctlOverclockPowerLimitSetV2 after resetting, Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nResetting Sustained Power Limit to Default Value: %lf %s", CurrentPowerLimit, DecodeCtlUnits(OcProperties.powerLimit.units).c_str());
}

/***************************************************************
 * @brief OverclockTemperatureLimit
 *
 * Overclock Temperature Limit: The function ctlOverclockTemperatureLimitSetV2
 * allows to increase or decrease the temperature limit. If the temperature reaches
 * the limit, frequency will be throttled to be within the temperature limit.
 * adapter.
 * @param
 * @return
 ***************************************************************/
void OverclockTemperatureLimit(ctl_device_adapter_handle_t hDAhandle)
{
    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    OcProperties.Version             = 1;

    // Step 1: Reading Overclock Properties
    ctl_result_t Status = ctlOverclockGetProperties(hDAhandle, &OcProperties);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockTemperatureLimit => ctlOverclockGetProperties Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }

    // Step 2: Reading Current Temperature Limit
    // Output Temperature Limit units are returned in ctl_oc_properties_t::temperatureLimit::units returned from ctlOverclockGetProperties()
    double CurrentTemperatureLimit = 0.0;
    Status                         = ctlOverclockTemperatureLimitGetV2(hDAhandle, &CurrentTemperatureLimit);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockTemperatureLimit => ctlOverclockTemperatureLimitGetV2 Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\n\nCurrent Temperature Limit: %lf, %s", CurrentTemperatureLimit, DecodeCtlUnits(OcProperties.temperatureLimit.units).c_str());

    // Step 3: Writing New Temperature Limit by increasing it by (Step * 5.0) from min value
    // Input Temperature Limit units are given in ctl_oc_properties_t::temperatureLimit::units returned from ctlOverclockGetProperties()
    // Currently ctl_oc_properties_t::temperatureLimit::units for Alchemist are CTL_UNITS_TEMPERATURE_CELSIUS units, for Battlemage are CTL_UNITS_PERCENT units
    CurrentTemperatureLimit = OcProperties.temperatureLimit.min + (OcProperties.temperatureLimit.step * 5.0);
    Status                  = ctlOverclockTemperatureLimitSetV2(hDAhandle, CurrentTemperatureLimit);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockTemperatureLimit => ctlOverclockTemperatureLimitSetV2 Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nSetting New Temperature Limit: %lf, %s", CurrentTemperatureLimit, DecodeCtlUnits(OcProperties.temperatureLimit.units).c_str());

    // Step 4: Reading back New Temperature Limit
    // Output Temperature Limit units are returned in ctl_oc_properties_t::temperatureLimit::units returned from ctlOverclockGetProperties()
    CurrentTemperatureLimit = 0.0;
    Status                  = ctlOverclockTemperatureLimitGetV2(hDAhandle, &CurrentTemperatureLimit);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockTemperatureLimit => ctlOverclockTemperatureLimitGetV2 after setting, Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nReading New Temperature Limit: %lf, %s", CurrentTemperatureLimit, DecodeCtlUnits(OcProperties.temperatureLimit.units).c_str());

    // Step 5: Resetting Temperature Limit
    // Input Temperature Limit units are given in ctl_oc_properties_t::temperatureLimit::units returned from ctlOverclockGetProperties()
    // Currently ctl_oc_properties_t::temperatureLimit::units for Alchemist are CTL_UNITS_TEMPERATURE_CELSIUS units, for Battlemage are CTL_UNITS_PERCENT units
    CurrentTemperatureLimit = OcProperties.temperatureLimit.Default;
    Status                  = ctlOverclockTemperatureLimitSetV2(hDAhandle, CurrentTemperatureLimit);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockTemperatureLimit => ctlOverclockTemperatureLimitSetV2 after resetting, Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nResetting Temperature Limit to Default Value: %lf, %s", CurrentTemperatureLimit, DecodeCtlUnits(OcProperties.temperatureLimit.units).c_str());
}

/***************************************************************
 * @brief OverclockVramMemSpeedLimit
 * VramMemSpeedLimit Overclocking V2 APIs are only supported for Battlemage, not supported for Alchemist
 * adapter.
 * @param
 * @return
 ***************************************************************/
void OverclockVramMemSpeedLimit(ctl_device_adapter_handle_t hDAhandle)
{
    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    OcProperties.Version             = 1;

    // Step 1: Reading Overclock Properties
    ctl_result_t Status = ctlOverclockGetProperties(hDAhandle, &OcProperties);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVramMemSpeedLimit => ctlOverclockGetProperties Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }

    // Below set of VramMemSpeedLimit Overclocking V2 APIs are only supported for Battlemage, not supported for Alchemist
    //
    // Step 2: Reading Current VramMemSpeed Limit
    // Output VramMemSpeed Limit units are returned in ctl_oc_properties_t::vramMemSpeedLimit::units returned from ctlOverclockGetProperties()
    double CurrentVramMemSpeedLimit = 0.0;
    Status                          = ctlOverclockVramMemSpeedLimitGetV2(hDAhandle, &CurrentVramMemSpeedLimit);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVramMemSpeedLimit => ctlOverclockVramMemSpeedLimitGetV2 Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\n\nCurrent VramMemSpeed Limit: %lf, %s", CurrentVramMemSpeedLimit, DecodeCtlUnits(OcProperties.vramMemSpeedLimit.units).c_str());

    // Step 3: Writing New VramMemSpeed Limit by increasing it by (Step * 5.0) Gbps from min value
    // Input VramMemSpeed Limit units are given in ctl_oc_properties_t::vramMemSpeedLimit::units returned from ctlOverclockGetProperties()
    // Currently ctl_oc_properties_t::vramMemSpeedLimit::units for Battlemage are CTL_UNITS_MEM_SPEED_GBPS units
    CurrentVramMemSpeedLimit = OcProperties.vramMemSpeedLimit.min + (OcProperties.vramMemSpeedLimit.step * 5.0);
    Status                   = ctlOverclockVramMemSpeedLimitSetV2(hDAhandle, CurrentVramMemSpeedLimit);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVramMemSpeedLimit => ctlOverclockVramMemSpeedLimitSetV2 Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nSetting New VramMemSpeed Limit: %lf, %s", CurrentVramMemSpeedLimit, DecodeCtlUnits(OcProperties.vramMemSpeedLimit.units).c_str());

    // Step 4: Reading back New VramMemSpeed Limit
    // Output VramMemSpeed Limit units are returned in ctl_oc_properties_t::vramMemSpeedLimit::units returned from ctlOverclockGetProperties()
    CurrentVramMemSpeedLimit = 0.0;
    Status                   = ctlOverclockVramMemSpeedLimitGetV2(hDAhandle, &CurrentVramMemSpeedLimit);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVramMemSpeedLimit => ctlOverclockVramMemSpeedLimitGetV2 after setting, Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nReading New VramMemSpeed Limit: %lf, %s", CurrentVramMemSpeedLimit, DecodeCtlUnits(OcProperties.vramMemSpeedLimit.units).c_str());

    // Step 5: Resetting VramMemSpeed Limit
    // Input VramMemSpeed Limit units are given in ctl_oc_properties_t::vramMemSpeedLimit::units returned from ctlOverclockGetProperties()
    // Currently ctl_oc_properties_t::vramMemSpeedLimit::units for Battlemage are CTL_UNITS_MEM_SPEED_GBPS units
    CurrentVramMemSpeedLimit = OcProperties.vramMemSpeedLimit.Default;
    Status                   = ctlOverclockVramMemSpeedLimitSetV2(hDAhandle, CurrentVramMemSpeedLimit);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVramMemSpeedLimit => ctlOverclockVramMemSpeedLimitSetV2 after resetting, Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nResetting VramMemSpeed Limit to Default Value: %lf, %s", CurrentVramMemSpeedLimit, DecodeCtlUnits(OcProperties.vramMemSpeedLimit.units).c_str());
}

/***************************************************************
 * @brief OverclockVFCurveReadWrite
 * VFCurve Read/Write Overclocking V2 APIs are only supported for Battlemage, not supported for Alchemist
 * adapter.
 * @param
 * @return
 ***************************************************************/
void OverclockVFCurveReadWrite(ctl_device_adapter_handle_t hDAhandle)
{
    ctl_vf_curve_type_t VFCurveType              = CTL_VF_CURVE_TYPE_STOCK;
    ctl_vf_curve_details_t VFCurveDetails        = CTL_VF_CURVE_DETAILS_SIMPLIFIED;
    uint32_t NumVFPoints                         = 0;
    ctl_voltage_frequency_point_t *pVFCurveTable = NULL;
    ctl_result_t Status                          = CTL_RESULT_SUCCESS;

    // Below set of VFCurve Read/Write Overclocking V2 APIs are only supported for Battlemage, not supported for Alchemist

    PRINT_LOGS("\n\n======================== OverclockVFCurveReadWrite ===============================");
    PRINT_LOGS("\n================================== STEP 1 ========================================");
    // Step 1: Read Stock VF Curve Points with VFCurveDetails as Simplified, Medium, Elaborate
    VFCurveType = CTL_VF_CURVE_TYPE_STOCK;
    for (uint32_t VFCurveDetailsIndex = CTL_VF_CURVE_DETAILS_SIMPLIFIED; VFCurveDetailsIndex < CTL_VF_CURVE_DETAILS_MAX; VFCurveDetailsIndex++)
    {
        NumVFPoints    = 0;
        VFCurveDetails = (ctl_vf_curve_details_t)VFCurveDetailsIndex;
        Status         = ctlOverclockReadVFCurve(hDAhandle, VFCurveType, VFCurveDetails, &NumVFPoints, nullptr);
        if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\nOverclockVFCurveReadWrite => CTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 1, Error: %s, ErrorCode: 0x%x", DecodeVFCurveDetails(VFCurveDetails).c_str(),
                       DecodeRetCode(Status).c_str(), Status);
            return;
        }
        else
        {
            PRINT_LOGS("\n\nCTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 1, NumVFPoints: %lu", DecodeVFCurveDetails(VFCurveDetails).c_str(), NumVFPoints);
            pVFCurveTable = (ctl_voltage_frequency_point_t *)malloc(sizeof(ctl_voltage_frequency_point_t) * NumVFPoints);

            if (pVFCurveTable == NULL)
            {
                free(pVFCurveTable);
                goto Exit;
            }

            Status = ctlOverclockReadVFCurve(hDAhandle, VFCurveType, VFCurveDetails, &NumVFPoints, pVFCurveTable);
            if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
            {
                PRINT_LOGS("\nOverclockVFCurveReadWrite => CTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 2, Error: %s, ErrorCode: 0x%x", DecodeVFCurveDetails(VFCurveDetails).c_str(),
                           DecodeRetCode(Status).c_str(), Status);
                free(pVFCurveTable);
                goto Exit;
            }
            for (uint32_t i = 0; i < NumVFPoints; i++)
            {
                PRINT_LOGS("\nCTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 2, VFPoint: %lu, Frequency: %lu, Voltage: %lu", DecodeVFCurveDetails(VFCurveDetails).c_str(), i,
                           pVFCurveTable[i].Frequency, pVFCurveTable[i].Voltage);
            }
            free(pVFCurveTable);
        }
    }

    PRINT_LOGS("\n================================== STEP 2 ========================================");
    // Step 2.1: Calling Waiver Set first before calling ctlOverclockWriteCustomVFCurve
    Status = ctlOverclockWaiverSet(hDAhandle);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVFCurveReadWrite => ctlOverclockWaiverSet Result: Error %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nOverclock Waiver Set Succesfully for ctlOverclockWriteCustomVFCurve API !!");

    // Step 2.2: Modify few points in Stock Simplified VFCurveTable and perform VFCurveWrite
    NumVFPoints    = 0;
    VFCurveType    = CTL_VF_CURVE_TYPE_STOCK;
    VFCurveDetails = CTL_VF_CURVE_DETAILS_SIMPLIFIED;
    Status         = ctlOverclockReadVFCurve(hDAhandle, VFCurveType, VFCurveDetails, &NumVFPoints, nullptr);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVFCurveReadWrite => CTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 1, Error: %s, ErrorCode: 0x%x", DecodeVFCurveDetails(VFCurveDetails).c_str(),
                   DecodeRetCode(Status).c_str(), Status);
        return;
    }
    else
    {
        PRINT_LOGS("\nCTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 1, NumVFPoints: %lu", DecodeVFCurveDetails(VFCurveDetails).c_str(), NumVFPoints);
        pVFCurveTable = (ctl_voltage_frequency_point_t *)malloc(sizeof(ctl_voltage_frequency_point_t) * NumVFPoints);

        if (pVFCurveTable == NULL)
        {
            free(pVFCurveTable);
            goto Exit;
        }

        Status = ctlOverclockReadVFCurve(hDAhandle, VFCurveType, VFCurveDetails, &NumVFPoints, pVFCurveTable);
        if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\nOverclockVFCurveReadWrite => CTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 2, Error: %s, ErrorCode: 0x%x", DecodeVFCurveDetails(VFCurveDetails).c_str(),
                       DecodeRetCode(Status).c_str(), Status);
            free(pVFCurveTable);
            goto Exit;
        }
        PRINT_LOGS("\nCTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 2, Before modifying VFPoint 5, VFPoint 6", DecodeVFCurveDetails(VFCurveDetails).c_str());
        PRINT_LOGS("\nCTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 2, VFPoint: %lu, Frequency: %lu, Voltage: %lu", DecodeVFCurveDetails(VFCurveDetails).c_str(), 5,
                   pVFCurveTable[5].Frequency, pVFCurveTable[5].Voltage);
        PRINT_LOGS("\nCTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 2, VFPoint: %lu, Frequency: %lu, Voltage: %lu", DecodeVFCurveDetails(VFCurveDetails).c_str(), 6,
                   pVFCurveTable[6].Frequency, pVFCurveTable[6].Voltage);

        pVFCurveTable[5].Frequency += 50;
        pVFCurveTable[6].Frequency += 100;
        PRINT_LOGS("\nCTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 2, After modifying VFPoint 5, VFPoint 6", DecodeVFCurveDetails(VFCurveDetails).c_str());
        PRINT_LOGS("\nCTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 2, VFPoint: %lu, Frequency: %lu, Voltage: %lu", DecodeVFCurveDetails(VFCurveDetails).c_str(), 5,
                   pVFCurveTable[5].Frequency, pVFCurveTable[5].Voltage);
        PRINT_LOGS("\nCTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockReadVFCurve Pass 2, VFPoint: %lu, Frequency: %lu, Voltage: %lu", DecodeVFCurveDetails(VFCurveDetails).c_str(), 6,
                   pVFCurveTable[6].Frequency, pVFCurveTable[6].Voltage);

        Status = ctlOverclockWriteCustomVFCurve(hDAhandle, NumVFPoints, pVFCurveTable);
        if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\nOverclockVFCurveReadWrite => CTL_VF_CURVE_TYPE_STOCK, %s, ctlOverclockWriteCustomVFCurve, Error: %s, ErrorCode: 0x%x", DecodeVFCurveDetails(VFCurveDetails).c_str(),
                       DecodeRetCode(Status).c_str(), Status);
            free(pVFCurveTable);
            goto Exit;
        }
        free(pVFCurveTable);
    }

    PRINT_LOGS("\n================================== STEP 3 ========================================");
    // Step 3: Read Live VF Curve Points with VFCurveDetails as Simplified, Medium, Elaborate
    VFCurveType = CTL_VF_CURVE_TYPE_LIVE;
    for (uint32_t VFCurveDetailsIndex = CTL_VF_CURVE_DETAILS_SIMPLIFIED; VFCurveDetailsIndex < CTL_VF_CURVE_DETAILS_MAX; VFCurveDetailsIndex++)
    {
        NumVFPoints    = 0;
        VFCurveDetails = (ctl_vf_curve_details_t)VFCurveDetailsIndex;
        Status         = ctlOverclockReadVFCurve(hDAhandle, VFCurveType, VFCurveDetails, &NumVFPoints, nullptr);
        if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\nOverclockVFCurveReadWrite => CTL_VF_CURVE_TYPE_LIVE, %s, ctlOverclockReadVFCurve Pass 1, Error: %s, ErrorCode: 0x%x", DecodeVFCurveDetails(VFCurveDetails).c_str(),
                       DecodeRetCode(Status).c_str(), Status);
            return;
        }
        else
        {
            PRINT_LOGS("\n\nCTL_VF_CURVE_TYPE_LIVE, %s, ctlOverclockReadVFCurve Pass 1, NumVFPoints: %lu", DecodeVFCurveDetails(VFCurveDetails).c_str(), NumVFPoints);
            pVFCurveTable = (ctl_voltage_frequency_point_t *)malloc(sizeof(ctl_voltage_frequency_point_t) * NumVFPoints);

            if (pVFCurveTable == NULL)
            {
                free(pVFCurveTable);
                goto Exit;
            }

            Status = ctlOverclockReadVFCurve(hDAhandle, VFCurveType, VFCurveDetails, &NumVFPoints, pVFCurveTable);
            if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
            {
                PRINT_LOGS("\nOverclockVFCurveReadWrite => CTL_VF_CURVE_TYPE_LIVE, %s, ctlOverclockReadVFCurve Pass 2, Error: %s, ErrorCode: 0x%x", DecodeVFCurveDetails(VFCurveDetails).c_str(),
                           DecodeRetCode(Status).c_str(), Status);
                free(pVFCurveTable);
                goto Exit;
            }
            for (uint32_t i = 0; i < NumVFPoints; i++)
            {
                PRINT_LOGS("\nCTL_VF_CURVE_TYPE_LIVE, %s, ctlOverclockReadVFCurve Pass 2, VFPoint: %lu, Frequency: %lu, Voltage: %lu", DecodeVFCurveDetails(VFCurveDetails).c_str(), i,
                           pVFCurveTable[i].Frequency, pVFCurveTable[i].Voltage);
            }
            free(pVFCurveTable);
        }
    }

Exit:
    pVFCurveTable = nullptr;

    PRINT_LOGS("\n================================== STEP 4 ========================================");
    // Step 4: Reset all OC controls (FrequencyOffset, VoltageOffset, PowerLimit, TemperatureLimit, VramMemSpeedLimit including VFCurve)
    Status = ctlOverclockResetToDefault(hDAhandle);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nOverclockVFCurveReadWrite => ctlOverclockResetToDefault, Error: %s, ErrorCode: 0x%x", DecodeRetCode(Status).c_str(), Status);
        return;
    }
    PRINT_LOGS("\nctlOverclockResetToDefault to reset all OC controls (FrequencyOffset, VoltageOffset, PowerLimit, TemperatureLimit, VramMemSpeedLimit including VFCurve) is successful !\n");
}

/***************************************************************
 * @brief OverclockPowerTelemetry
 *
 * Overclock Power Temeletry: The function ctlPowerTelemetryGet allows
 * to retrieve all the available metrics from the adapter in one
 * single and efficient call.
 * @param
 * @return
 ***************************************************************/
void OverclockPowerTelemetry(ctl_device_adapter_handle_t hDAhandle)
{
    ctl_power_telemetry_t pPowerTelemetry = {};
    pPowerTelemetry.Size                  = sizeof(ctl_power_telemetry_t);
    pPowerTelemetry.Version               = 1;

    ctl_result_t Status = ctlPowerTelemetryGet(hDAhandle, &pPowerTelemetry);

    if (Status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nTelemetry Success\n \n");

        if (pPowerTelemetry.timeStamp.bSupported)
        {
            PRINT_LOGS("\nTimeStamp: %f (%s) Datatype:(%s)", pPowerTelemetry.timeStamp.value.datadouble, DecodeCtlUnits(pPowerTelemetry.timeStamp.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.timeStamp.type).c_str());
        }

        if (pPowerTelemetry.gpuEnergyCounter.bSupported)
        {
            PRINT_LOGS("\nGpu Energy Counter: %f (%s) Datatype:(%s)", pPowerTelemetry.gpuEnergyCounter.value.datadouble, DecodeCtlUnits(pPowerTelemetry.gpuEnergyCounter.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.gpuEnergyCounter.type).c_str());
        }

        if (pPowerTelemetry.vramEnergyCounter.bSupported)
        {
            PRINT_LOGS("\nVRAM Energy Counter: %f (%s) Datatype:(%s)", pPowerTelemetry.vramEnergyCounter.value.datadouble, DecodeCtlUnits(pPowerTelemetry.vramEnergyCounter.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.vramEnergyCounter.type).c_str());
        }

        if (pPowerTelemetry.totalCardEnergyCounter.bSupported)
        {
            PRINT_LOGS("\nCard Energy Counter: %f (%s) Datatype:(%s)", pPowerTelemetry.totalCardEnergyCounter.value.datadouble, DecodeCtlUnits(pPowerTelemetry.totalCardEnergyCounter.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.totalCardEnergyCounter.type).c_str());
        }

        if (pPowerTelemetry.gpuVoltage.bSupported)
        {
            PRINT_LOGS("\nGpu Voltage: %f (%s) Datatype:(%s)", pPowerTelemetry.gpuVoltage.value.datadouble, DecodeCtlUnits(pPowerTelemetry.gpuVoltage.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.gpuVoltage.type).c_str());
        }

        if (pPowerTelemetry.gpuCurrentClockFrequency.bSupported)
        {
            PRINT_LOGS("\nGpu Current Frequency: %f (%s) Datatype:(%s)", pPowerTelemetry.gpuCurrentClockFrequency.value.datadouble,
                       DecodeCtlUnits(pPowerTelemetry.gpuCurrentClockFrequency.units).c_str(), DecodeCtlDataType(pPowerTelemetry.gpuCurrentClockFrequency.type).c_str());
        }

        if (pPowerTelemetry.gpuCurrentTemperature.bSupported)
        {
            PRINT_LOGS("\nGpu Current Temperature: %f (%s) Datatype:(%s)", pPowerTelemetry.gpuCurrentTemperature.value.datadouble, DecodeCtlUnits(pPowerTelemetry.gpuCurrentTemperature.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.gpuCurrentTemperature.type).c_str());
        }

        if (pPowerTelemetry.globalActivityCounter.bSupported)
        {
            PRINT_LOGS("\nGpu Activity Counter: %f (%s) Datatype:(%s)", pPowerTelemetry.globalActivityCounter.value.datadouble, DecodeCtlUnits(pPowerTelemetry.globalActivityCounter.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.globalActivityCounter.type).c_str());
        }

        if (pPowerTelemetry.renderComputeActivityCounter.bSupported)
        {
            PRINT_LOGS("\nRender Activity Counter: %f (%s) Datatype:(%s)", pPowerTelemetry.renderComputeActivityCounter.value.datadouble,
                       DecodeCtlUnits(pPowerTelemetry.renderComputeActivityCounter.units).c_str(), DecodeCtlDataType(pPowerTelemetry.renderComputeActivityCounter.type).c_str());
        }

        if (pPowerTelemetry.mediaActivityCounter.bSupported)
        {
            PRINT_LOGS("\nMedia Activity Counter: %f (%s) Datatype:(%s)", pPowerTelemetry.mediaActivityCounter.value.datadouble, DecodeCtlUnits(pPowerTelemetry.mediaActivityCounter.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.mediaActivityCounter.type).c_str());
        }

        if (pPowerTelemetry.vramVoltage.bSupported)
        {
            PRINT_LOGS("\nVRAM Voltage: %f (%s) Datatype:(%s)", pPowerTelemetry.vramVoltage.value.datadouble, DecodeCtlUnits(pPowerTelemetry.vramVoltage.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.vramVoltage.type).c_str());
        }

        if (pPowerTelemetry.vramCurrentClockFrequency.bSupported)
        {
            PRINT_LOGS("\nVRAM Frequency: %f (%s) Datatype:(%s)", pPowerTelemetry.vramCurrentClockFrequency.value.datadouble, DecodeCtlUnits(pPowerTelemetry.vramCurrentClockFrequency.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.vramCurrentClockFrequency.type).c_str());
        }

        if (pPowerTelemetry.vramReadBandwidthCounter.bSupported)
        {
            PRINT_LOGS("\nVRAM Read Bandwidth Counter: %llu (%s) Datatype:(%s)", pPowerTelemetry.vramReadBandwidthCounter.value.datau64,
                       DecodeCtlUnits(pPowerTelemetry.vramReadBandwidthCounter.units).c_str(), DecodeCtlDataType(pPowerTelemetry.vramReadBandwidthCounter.type).c_str());
        }

        if (pPowerTelemetry.vramWriteBandwidthCounter.bSupported)
        {
            PRINT_LOGS("\nVRAM Write Bandwidth Counter: %llu (%s) Datatype:(%s)", pPowerTelemetry.vramWriteBandwidthCounter.value.datau64,
                       DecodeCtlUnits(pPowerTelemetry.vramWriteBandwidthCounter.units).c_str(), DecodeCtlDataType(pPowerTelemetry.vramWriteBandwidthCounter.type).c_str());
        }

        if (pPowerTelemetry.vramCurrentEffectiveFrequency.bSupported)
        {
            PRINT_LOGS("\nVRAM Effective Frequency: %f (%s) Datatype:(%s)", pPowerTelemetry.vramCurrentEffectiveFrequency.value.datadouble,
                       DecodeCtlUnits(pPowerTelemetry.vramCurrentEffectiveFrequency.units).c_str(), DecodeCtlDataType(pPowerTelemetry.vramCurrentEffectiveFrequency.type).c_str());
        }

        if (pPowerTelemetry.vramCurrentTemperature.bSupported)
        {
            PRINT_LOGS("\nVRAM Temperature: %f (%s) Datatype:(%s)", pPowerTelemetry.vramCurrentTemperature.value.datadouble, DecodeCtlUnits(pPowerTelemetry.vramCurrentTemperature.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.vramCurrentTemperature.type).c_str());
        }

        if (pPowerTelemetry.vramReadBandwidth.bSupported)
        {
            PRINT_LOGS("\nVRAM Read Bandwidth: %f (%s) Datatype:(%s)", pPowerTelemetry.vramReadBandwidth.value.datadouble, DecodeCtlUnits(pPowerTelemetry.vramReadBandwidth.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.vramReadBandwidth.type).c_str());
        }

        if (pPowerTelemetry.vramWriteBandwidth.bSupported)
        {
            PRINT_LOGS("\nVRAM Write Bandwidth: %f (%s) Datatype:(%s)", pPowerTelemetry.vramWriteBandwidth.value.datadouble, DecodeCtlUnits(pPowerTelemetry.vramWriteBandwidth.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.vramWriteBandwidth.type).c_str());
        }

        if (pPowerTelemetry.gpuVrTemp.bSupported)
        {
            PRINT_LOGS("\nGPU VR Temperature: %f (%s) Datatype:(%s)", pPowerTelemetry.gpuVrTemp.value.datadouble, DecodeCtlUnits(pPowerTelemetry.gpuVrTemp.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.gpuVrTemp.type).c_str());
        }

        if (pPowerTelemetry.vramVrTemp.bSupported)
        {
            PRINT_LOGS("\nVRAM VR Temperature: %f (%s) Datatype:(%s)", pPowerTelemetry.vramVrTemp.value.datadouble, DecodeCtlUnits(pPowerTelemetry.vramVrTemp.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.vramVrTemp.type).c_str());
        }

        if (pPowerTelemetry.saVrTemp.bSupported)
        {
            PRINT_LOGS("\nSA VR Temperature: %f (%s) Datatype:(%s)", pPowerTelemetry.saVrTemp.value.datadouble, DecodeCtlUnits(pPowerTelemetry.saVrTemp.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.saVrTemp.type).c_str());
        }

        if (pPowerTelemetry.gpuEffectiveClock.bSupported)
        {
            PRINT_LOGS("\nEffective frequency of the GPU: %f (%s) Datatype:(%s)", pPowerTelemetry.gpuEffectiveClock.value.datadouble, DecodeCtlUnits(pPowerTelemetry.gpuEffectiveClock.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.gpuEffectiveClock.type).c_str());
        }

        if (pPowerTelemetry.gpuOverVoltagePercent.bSupported)
        {
            PRINT_LOGS("\nGPU Overvoltage Percentage: %f (%s) Datatype:(%s)", pPowerTelemetry.gpuOverVoltagePercent.value.datadouble,
                       DecodeCtlUnits(pPowerTelemetry.gpuOverVoltagePercent.units).c_str(), DecodeCtlDataType(pPowerTelemetry.gpuOverVoltagePercent.type).c_str());
        }

        if (pPowerTelemetry.gpuPowerPercent.bSupported)
        {
            PRINT_LOGS("\nGPU Power Percentage: %f (%s) Datatype:(%s)", pPowerTelemetry.gpuPowerPercent.value.datadouble, DecodeCtlUnits(pPowerTelemetry.gpuPowerPercent.units).c_str(),
                       DecodeCtlDataType(pPowerTelemetry.gpuPowerPercent.type).c_str());
        }

        if (pPowerTelemetry.gpuTemperaturePercent.bSupported)
        {
            PRINT_LOGS("\nGPU Temperature Percentage: %f (%s) Datatype:(%s)", pPowerTelemetry.gpuTemperaturePercent.value.datadouble,
                       DecodeCtlUnits(pPowerTelemetry.gpuTemperaturePercent.units).c_str(), DecodeCtlDataType(pPowerTelemetry.gpuTemperaturePercent.type).c_str());
        }

        for (int i = 0; i < CTL_FAN_COUNT; i++)
        {
            if (pPowerTelemetry.fanSpeed[i].bSupported)
            {
                PRINT_LOGS("\nFan[%d] Speed: %f (%s) Datatype:(%s)", i, pPowerTelemetry.fanSpeed[i].value.datadouble, DecodeCtlUnits(pPowerTelemetry.fanSpeed[i].units).c_str(),
                           DecodeCtlDataType(pPowerTelemetry.fanSpeed[i].type).c_str());
            }
        }

        PRINT_LOGS("\ngpuPowerLimited: %s", DecodeBoolean(pPowerTelemetry.gpuPowerLimited).c_str());
        PRINT_LOGS("\ngpuTemperatureLimited: %s", DecodeBoolean(pPowerTelemetry.gpuTemperatureLimited).c_str());
        PRINT_LOGS("\ngpuCurrentLimited: %s", DecodeBoolean(pPowerTelemetry.gpuCurrentLimited).c_str());
        PRINT_LOGS("\ngpuVoltageLimited: %s", DecodeBoolean(pPowerTelemetry.gpuVoltageLimited).c_str());
        PRINT_LOGS("\ngpuUtilizationLimited: %s", DecodeBoolean(pPowerTelemetry.gpuUtilizationLimited).c_str());
    }
    else
    {
        PRINT_LOGS("\nOverclockPowerTelemetry => ctlPowerTelemetryGet Result Error: %s, ErrorCode: 0x%x \n \n", DecodeRetCode(Status).c_str(), Status);
    }
}

int main()
{
    ctl_result_t Result                                       = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hDevices                     = nullptr;
    ctl_device_adapter_properties_t StDeviceAdapterProperties = { 0 };
    ctl_init_args_t CtlInitArgs;
    ctl_api_handle_t hAPIHandle;
    // Get a handle to the DLL module.
    uint32_t AdapterCount = 0;
    uint32_t Index        = 0;

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    CtlInitArgs.AppVersion = CTL_MAKE_VERSION(CTL_IMPL_MAJOR_VERSION, CTL_IMPL_MINOR_VERSION);
    CtlInitArgs.flags      = CTL_INIT_FLAG_USE_LEVEL_ZERO;
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
            Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);
            LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");
        }
        catch (const std::bad_array_new_length &e)
        {
            printf("%s \n", e.what());
        }

        if (CTL_RESULT_SUCCESS == Result)
        {
            hDevices = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * AdapterCount);

            if (hDevices == NULL)
            {
                goto Exit;
            }

            try
            {
                Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);
                LOG_AND_EXIT_ON_ERROR(Result, "ctlEnumerateDevices");
            }
            catch (const std::bad_array_new_length &e)
            {
                printf("%s \n", e.what());
            }
        }

        if (CTL_RESULT_SUCCESS != Result)
        {
            PRINT_LOGS("ctlEnumerateDevices returned failure code: 0x%X\n", Result);
            goto Exit;
        }

        for (Index = 0; Index < AdapterCount; Index++)
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
                    goto Exit;
                }

                Result = ctlGetDeviceProperties(hDevices[Index], &StDeviceAdapterProperties);

                if (Result != CTL_RESULT_SUCCESS)
                {
                    PRINT_LOGS("ctlGetDeviceProperties returned failure code: 0x%X\n", Result);
                    break;
                }

                if (CTL_DEVICE_TYPE_GRAPHICS != StDeviceAdapterProperties.device_type)
                {
                    PRINT_LOGS("This is not a Graphics device \n");
                    CTL_FREE_MEM(StDeviceAdapterProperties.pDeviceID);
                    continue;
                }

                if (NULL != StDeviceAdapterProperties.pDeviceID)
                {
                    AdapterID = *(reinterpret_cast<LUID *>(StDeviceAdapterProperties.pDeviceID));
                    PRINT_LOGS("\nAdapter ID %lu\n", AdapterID.LowPart);
                }

                if (0x8086 == StDeviceAdapterProperties.pci_vendor_id)
                {
                    PRINT_LOGS("Intel Adapter Name %s\n", StDeviceAdapterProperties.name);
                    PRINT_LOGS("Vendor id  0x%X\n", StDeviceAdapterProperties.pci_vendor_id);
                    PRINT_LOGS("Device id 0x%X\n", StDeviceAdapterProperties.pci_device_id);
                    PRINT_LOGS("SubSys id 0x%X\n", StDeviceAdapterProperties.pci_subsys_id);
                    PRINT_LOGS("SubSys Vendor id 0x%X\n", StDeviceAdapterProperties.pci_subsys_vendor_id);
                    PRINT_LOGS("Rev id 0x%X\n", StDeviceAdapterProperties.rev_id);
                }

                try
                {
                    OverclockProperties(hDevices[Index]);
                    OverclockFrequencyOffset(hDevices[Index]);
                    OverclockVoltageOffset(hDevices[Index]);
                    OverclockLockFrequency(hDevices[Index]);
                    OverclockPowerLimit(hDevices[Index]);
                    OverclockTemperatureLimit(hDevices[Index]);
                    OverclockVramMemSpeedLimit(hDevices[Index]);
                    OverclockVFCurveReadWrite(hDevices[Index]);

                    // Telemetry
                    // Polling for 4 seconds with 200 ms sampling interval
                    for (uint32_t i = 0; i < 20; i++)
                    {
                        OverclockPowerTelemetry(hDevices[Index]);
                        Sleep(200);
                    }
                }
                catch (const std::bad_array_new_length &e)
                {
                    printf("%s \n", e.what());
                }
                CTL_FREE_MEM(StDeviceAdapterProperties.pDeviceID);
            }
        }
    }

Exit:

    ctlClose(hAPIHandle);
    CTL_FREE_MEM(hDevices);
    return 0;
}
