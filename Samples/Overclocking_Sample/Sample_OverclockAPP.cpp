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

#include <iostream>
#include <string>

#include <windows.h>
#include <stdio.h>
#include "igcl_api.h"

std::string DecodeRetCode(ctl_result_t Res);

void OverclockProperties(ctl_device_adapter_handle_t hDAhandle);
void OverclockFrequencyOffset(ctl_device_adapter_handle_t hDAhandle);
void OverclockVoltageOffset(ctl_device_adapter_handle_t hDAhandle);
void OverclockLockFrequency(ctl_device_adapter_handle_t hDAhandle);
void OverclockPowerLimit(ctl_device_adapter_handle_t hDAhandle);
void OverclockTemperatureLimit(ctl_device_adapter_handle_t hDAhandle);
void OverclockPowerTelemetry(ctl_device_adapter_handle_t hDAhandle);

const char *printType(ctl_data_type_t Type)
{
    switch (Type)
    {
        case ctl_data_type_t::CTL_DATA_TYPE_INT8:
        {
            return "CTL_DATA_TYPE_INT8";
        }
        break;
        case ctl_data_type_t::CTL_DATA_TYPE_UINT8:
        {
            return "CTL_DATA_TYPE_UINT8";
        }
        break;
        case ctl_data_type_t::CTL_DATA_TYPE_INT16:
        {
            return "CTL_DATA_TYPE_INT16";
        }
        break;
        case ctl_data_type_t::CTL_DATA_TYPE_UINT16:
        {
            return "CTL_DATA_TYPE_UINT16";
        }
        break;
        case ctl_data_type_t::CTL_DATA_TYPE_INT32:
        {
            return "CTL_DATA_TYPE_INT32";
        }
        break;
        case ctl_data_type_t::CTL_DATA_TYPE_UINT32:
        {
            return "CTL_DATA_TYPE_UINT32";
        }
        break;
        case ctl_data_type_t::CTL_DATA_TYPE_INT64:
        {
            return "CTL_DATA_TYPE_INT64";
        }
        break;
        case ctl_data_type_t::CTL_DATA_TYPE_UINT64:
        {
            return "CTL_DATA_TYPE_UINT64";
        }
        break;
        case ctl_data_type_t::CTL_DATA_TYPE_FLOAT:
        {
            return "CTL_DATA_TYPE_FLOAT";
        }
        break;
        case ctl_data_type_t::CTL_DATA_TYPE_DOUBLE:
        {
            return "CTL_DATA_TYPE_DOUBLE";
        }
        break;
        case ctl_data_type_t::CTL_DATA_TYPE_STRING_ASCII:
        {
            return "CTL_DATA_TYPE_STRING_ASCII";
        }
        break;
        case ctl_data_type_t::CTL_DATA_TYPE_STRING_UTF16:
        {
            return "CTL_DATA_TYPE_STRING_UTF16";
        }
        break;
        case ctl_data_type_t::CTL_DATA_TYPE_STRING_UTF132:
        {
            return "CTL_DATA_TYPE_STRING_UTF132";
        }
        break;
        default:
            return "Unknown units";
    }
    return "Unknown units";
}
const char *printUnits(ctl_units_t Units)
{
    switch (Units)
    {
        case ctl_units_t::CTL_UNITS_FREQUENCY_MHZ:
        {
            return "Frequency in MHz";
        }
        break;
        case ctl_units_t::CTL_UNITS_OPERATIONS_GTS:
        {
            return "GigaOperations per Second";
        }
        break;
        case ctl_units_t::CTL_UNITS_OPERATIONS_MTS:
        {
            return "MegaOperations per Second";
        }
        break;
        case ctl_units_t::CTL_UNITS_VOLTAGE_VOLTS:
        {
            return "Voltage in Volts";
        }
        break;
        case ctl_units_t::CTL_UNITS_POWER_WATTS:
        {
            return "Power in Watts";
        }
        break;
        case ctl_units_t::CTL_UNITS_TEMPERATURE_CELSIUS:
        {
            return "Temperature in Celsius";
        }
        break;
        case ctl_units_t::CTL_UNITS_ENERGY_JOULES:
        {
            return "Energy in Joules";
        }
        break;
        case ctl_units_t::CTL_UNITS_TIME_SECONDS:
        {
            return "Time in Seconds";
        }
        break;
        case ctl_units_t::CTL_UNITS_MEMORY_BYTES:
        {
            return "Memory in Bytes";
        }
        break;
        case ctl_units_t::CTL_UNITS_ANGULAR_SPEED_RPM:
        {
            return "Angular Speed in RPM";
        }
        break;
        default:
            return "Unknown units";
    }
    return "Unknown units";
}

/***************************************************************
 * @brief
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
    std::cout << "\n::::::::::::::Overclock Properties::::::::::::::\n" << std::endl;

    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    ctl_result_t status              = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(status) << std::endl;
        return;
    }

    // Is Overclocking supported by this adapter?
    std::cout << "Oc Supported? " << ((OcProperties.bSupported) ? "true" : "false") << std::endl << std::endl;

    // Slider for frequency offset
    std::cout << "Gpu Frequency Offset Supported? " << ((OcProperties.gpuFrequencyOffset.bSupported) ? "true" : "false") << std::endl;
    std::cout << "Gpu Frequency Offset Is Relative? " << ((OcProperties.gpuFrequencyOffset.bRelative) ? "true" : "false") << std::endl;
    std::cout << "Gpu Frequency Offset Have Reference? " << ((OcProperties.gpuFrequencyOffset.bReference) ? "true" : "false") << std::endl;
    std::cout << "Gpu Frequency Offset Default: " << OcProperties.gpuFrequencyOffset.Default << std::endl;
    std::cout << "Gpu Frequency Offset Min: " << OcProperties.gpuFrequencyOffset.min << std::endl;
    std::cout << "Gpu Frequency Offset Max: " << OcProperties.gpuFrequencyOffset.max << std::endl;
    std::cout << "Gpu Frequency Offset Reference: " << OcProperties.gpuFrequencyOffset.reference << std::endl;
    std::cout << "Gpu Frequency Offset Step: " << OcProperties.gpuFrequencyOffset.step << std::endl;
    std::cout << "Gpu Frequency Offset Units: " << printUnits(OcProperties.gpuFrequencyOffset.units) << std::endl << std::endl;

    // Slider for voltage offset
    std::cout << "Gpu Voltage Offset Supported? " << ((OcProperties.gpuVoltageOffset.bSupported) ? "true" : "false") << std::endl;
    std::cout << "Gpu Voltage Offset Is Relative? " << ((OcProperties.gpuVoltageOffset.bRelative) ? "true" : "false") << std::endl;
    std::cout << "Gpu Voltage Offset Have Reference? " << ((OcProperties.gpuVoltageOffset.bReference) ? "true" : "false") << std::endl;
    std::cout << "Gpu Voltage Offset Default: " << OcProperties.gpuVoltageOffset.Default << std::endl;
    std::cout << "Gpu Voltage Offset Min: " << OcProperties.gpuVoltageOffset.min << std::endl;
    std::cout << "Gpu Voltage Offset Max: " << OcProperties.gpuVoltageOffset.max << std::endl;
    std::cout << "Gpu Voltage Offset Reference: " << OcProperties.gpuVoltageOffset.reference << std::endl;
    std::cout << "Gpu Voltage Offset Step: " << OcProperties.gpuVoltageOffset.step << std::endl;
    std::cout << "Gpu Voltage Offset Units: " << printUnits(OcProperties.gpuVoltageOffset.units) << std::endl << std::endl;

    // Slider for VRAM frequency offset
    std::cout << "VRAM Frequency Offset Supported? " << ((OcProperties.vramFrequencyOffset.bSupported) ? "true" : "false") << std::endl;
    std::cout << "VRAM Frequency Offset Is Relative? " << ((OcProperties.vramFrequencyOffset.bRelative) ? "true" : "false") << std::endl;
    std::cout << "VRAM Frequency Offset Have Reference? " << ((OcProperties.vramFrequencyOffset.bReference) ? "true" : "false") << std::endl;
    std::cout << "VRAM Frequency Offset Default: " << OcProperties.vramFrequencyOffset.Default << std::endl;
    std::cout << "VRAM Frequency Offset Min: " << OcProperties.vramFrequencyOffset.min << std::endl;
    std::cout << "VRAM Frequency Offset Max: " << OcProperties.vramFrequencyOffset.max << std::endl;
    std::cout << "VRAM Frequency Offset Reference: " << OcProperties.vramFrequencyOffset.reference << std::endl;
    std::cout << "VRAM Frequency Offset Step: " << OcProperties.vramFrequencyOffset.step << std::endl;
    std::cout << "VRAM Frequency Offset Units: " << printUnits(OcProperties.vramFrequencyOffset.units) << std::endl << std::endl;

    // Slider for VRAM voltage offset
    std::cout << "VRAM Voltage Offset Supported? " << ((OcProperties.vramVoltageOffset.bSupported) ? "true" : "false") << std::endl;
    std::cout << "VRAM Voltage Offset Is Relative? " << ((OcProperties.vramVoltageOffset.bRelative) ? "true" : "false") << std::endl;
    std::cout << "VRAM Voltage Offset Have Reference? " << ((OcProperties.vramVoltageOffset.bReference) ? "true" : "false") << std::endl;
    std::cout << "VRAM Voltage Offset Default: " << OcProperties.vramVoltageOffset.Default << std::endl;
    std::cout << "VRAM Voltage Offset Min: " << OcProperties.vramVoltageOffset.min << std::endl;
    std::cout << "VRAM Voltage Offset Max: " << OcProperties.vramVoltageOffset.max << std::endl;
    std::cout << "VRAM Voltage Offset Reference: " << OcProperties.vramVoltageOffset.reference << std::endl;
    std::cout << "VRAM Voltage Offset Step: " << OcProperties.vramVoltageOffset.step << std::endl;
    std::cout << "VRAM Voltage Offset Units: " << printUnits(OcProperties.vramVoltageOffset.units) << std::endl << std::endl;

    // Slider for Power Limit
    std::cout << "Power Limit Supported? " << ((OcProperties.powerLimit.bSupported) ? "true" : "false") << std::endl;
    std::cout << "Power Limit Is Relative? " << ((OcProperties.powerLimit.bRelative) ? "true" : "false") << std::endl;
    std::cout << "Power Limit Have Reference? " << ((OcProperties.powerLimit.bReference) ? "true" : "false") << std::endl;
    std::cout << "Power Limit Default: " << OcProperties.powerLimit.Default << std::endl;
    std::cout << "Power Limit Min: " << OcProperties.powerLimit.min << std::endl;
    std::cout << "Power Limit Max: " << OcProperties.powerLimit.max << std::endl;
    std::cout << "Power Limit Reference: " << OcProperties.powerLimit.reference << std::endl;
    std::cout << "Power Limit Step: " << OcProperties.powerLimit.step << std::endl;
    std::cout << "Power Limit Units: " << printUnits(OcProperties.powerLimit.units) << std::endl << std::endl;

    // Slider for Temperature Limit
    std::cout << "Temperature Limit Supported? " << ((OcProperties.temperatureLimit.bSupported) ? "true" : "false") << std::endl;
    std::cout << "Temperature Limit Is Relative? " << ((OcProperties.temperatureLimit.bRelative) ? "true" : "false") << std::endl;
    std::cout << "Temperature Limit Have Reference? " << ((OcProperties.temperatureLimit.bReference) ? "true" : "false") << std::endl;
    std::cout << "Temperature Limit Default: " << OcProperties.temperatureLimit.Default << std::endl;
    std::cout << "Temperature Limit Min: " << OcProperties.temperatureLimit.min << std::endl;
    std::cout << "Temperature Limit Max: " << OcProperties.temperatureLimit.max << std::endl;
    std::cout << "Temperature Limit Reference: " << OcProperties.temperatureLimit.reference << std::endl;
    std::cout << "Temperature Limit Step: " << OcProperties.temperatureLimit.step << std::endl;
    std::cout << "Temperature Limit Units: " << printUnits(OcProperties.temperatureLimit.units) << std::endl << std::endl;

    std::cout << std::endl << std::endl;
}

/***************************************************************
 * @brief
 * Overclock Frequency Offset: The function ctlOverclockGpuFrequencyOffsetSet
 * allows to set a positive frequency offset.
 * @param
 * @return
 ***************************************************************/
void OverclockFrequencyOffset(ctl_device_adapter_handle_t hDAhandle)
{
    std::cout << "\n::::::::::::::Frequency Offset::::::::::::::\n" << std::endl;

    double GPUFrequencyOffset = 0.0;

    ctl_result_t status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
        return;
    }

    std::cout << "Current Frequency Offset: " << GPUFrequencyOffset << " MHz" << std::endl;

    // Setting 50 MHz Offset
    GPUFrequencyOffset = 50.0;

    // Calling the waiver first
    status = ctlOverclockWaiverSet(hDAhandle);
    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
        return;
    }

    // Setting the Offset
    status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
        return;
    }

    // Reseting local var
    GPUFrequencyOffset = 0.0;
    // Read back to confirm the new value
    status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
        return;
    }

    std::cout << "Current New Frequency Offset: " << GPUFrequencyOffset << " MHz" << std::endl;

    // Setting the Offset to 0
    GPUFrequencyOffset = 0.0;
    ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);

    std::cout << std::endl << std::endl;
}

/***************************************************************
 * @brief
 * Overclock Voltage Offset: The function ctlOverclockGpuVoltageOffsetSet
 * allows to set a positive voltage offset.
 * @param
 * @return
 ***************************************************************/
void OverclockVoltageOffset(ctl_device_adapter_handle_t hDAhandle)
{
    std::cout << "\n::::::::::::::Voltage Offset::::::::::::::\n" << std::endl;

    double VoltageOffset = 0.0;
    ctl_result_t status  = ctlOverclockGpuVoltageOffsetGet(hDAhandle, &VoltageOffset);

    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
        return;
    }

    std::cout << "Current Voltage Offset: " << VoltageOffset << " V" << std::endl;

    // Setting 25 mV Voltage Offset
    VoltageOffset = 0.025;
    // Calling the waiver first
    status = ctlOverclockWaiverSet(hDAhandle);
    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
        return;
    }

    // Setting the Offset
    status = ctlOverclockGpuVoltageOffsetSet(hDAhandle, VoltageOffset);
    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
        return;
    }

    // Reseting local var
    VoltageOffset = 0.0;
    // Read back to confirm the new value
    status = ctlOverclockGpuVoltageOffsetGet(hDAhandle, &VoltageOffset);
    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
        return;
    }

    std::cout << "Current New Voltage Offset: " << VoltageOffset << " V" << std::endl;

    // Setting the Offset to 0
    VoltageOffset = 0.0;
    ctlOverclockGpuVoltageOffsetSet(hDAhandle, VoltageOffset);

    std::cout << std::endl << std::endl;
}

/***************************************************************
 * @brief
 * Overclock Lock Frequency: The function ctlOverclockGpuLockSet
 * allows to fix the frequency and voltage, useful for OC scanners
 * or performance testing.
 * @param
 * @return
 ***************************************************************/
void OverclockLockFrequency(ctl_device_adapter_handle_t hDAhandle)
{
    std::cout << "\n::::::::::::::Lock Frequency::::::::::::::\n" << std::endl;

    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    ctl_result_t status              = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(status) << std::endl;
        return;
    }

    ctl_oc_vf_pair_t VfPair = {};
    status                  = ctlOverclockGpuLockGet(hDAhandle, &VfPair);

    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(status) << std::endl;
        return;
    }

    std::cout << "Current Locked Frequency: " << VfPair.Frequency << " MHz" << std::endl;
    std::cout << "Current Locked Voltage: " << VfPair.Voltage << " mV" << std::endl;

    VfPair = {};
    // Set slighly more frequency and voltage
    // 50 MHz over reference frequency
    VfPair.Frequency = OcProperties.gpuFrequencyOffset.reference + 50.0;
    // 50 mV over reference voltage. Convert to mV first.
    VfPair.Voltage = OcProperties.gpuVoltageOffset.reference * 1000.0 + 50.0;

    status = ctlOverclockGpuLockSet(hDAhandle, VfPair);

    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(status) << std::endl;
        return;
    }

    // Reseting local var
    VfPair = {};

    status = ctlOverclockGpuLockGet(hDAhandle, &VfPair);

    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(status) << std::endl;
        return;
    }

    // Verifying the new locked frequency and voltage
    std::cout << "New Current Locked Frequency: " << VfPair.Frequency << " MHz" << std::endl;
    std::cout << "New Current Locked Voltage: " << VfPair.Voltage << " mV" << std::endl;

    // Reset to default
    VfPair = {};
    ctlOverclockGpuLockSet(hDAhandle, VfPair);

    std::cout << std::endl << std::endl;
}

/***************************************************************
 * @brief Main Function
 *
 * Overclock Power Limit: The function ctlOverclockPowerLimitSet
 * allows to increase or decrease the power (TPD) budget for the
 * adapter.
 * @param
 * @return
 ***************************************************************/
void OverclockPowerLimit(ctl_device_adapter_handle_t hDAhandle)
{
    std::cout << "\n::::::::::::::Power Limit::::::::::::::\n" << std::endl;

    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    ctl_result_t status              = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Error Getting Properties" << DecodeRetCode(status) << std::endl;
        return;
    }

    std::cout << "Getting current Power Limit" << std::endl << std::endl;
    double CurrentPowerLimit = 0.0;

    status = ctlOverclockPowerLimitGet(hDAhandle, &CurrentPowerLimit);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Current Sustained Power Limit:" << CurrentPowerLimit << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\nSetting current Power Limit inside limits:" << std::endl << std::endl;

    // Convert to mW and get min + 15 W.
    CurrentPowerLimit = OcProperties.powerLimit.Default * 1000.0 + 15000.0;

    status = ctlOverclockPowerLimitSet(hDAhandle, CurrentPowerLimit);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Power Limit set correctly." << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\nGetting the now current Power Limit:" << std::endl << std::endl;

    double NewCurrentPowerLimit = 0.0;

    status = ctlOverclockPowerLimitGet(hDAhandle, &NewCurrentPowerLimit);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Current Sustained Power Limit:" << NewCurrentPowerLimit << std::endl;
        std::cout << "Requested Sustained Power Limit:" << CurrentPowerLimit << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    // Reset to default
    CurrentPowerLimit = OcProperties.powerLimit.Default * 1000.0;
    ctlOverclockPowerLimitSet(hDAhandle, CurrentPowerLimit);

    std::cout << std::endl << std::endl;
}

/***************************************************************
 * @brief Main Function
 *
 * Overclock Temperature Limit: The function ctlOverclockTemperatureLimitSet
 * allows to increase or decrease the temperature limit. If the temperature reaches
 * the limit, frequency will be throttled to be within the temperature limit.
 * adapter.
 * @param
 * @return
 ***************************************************************/
void OverclockTemperatureLimit(ctl_device_adapter_handle_t hDAhandle)
{
    std::cout << "\n::::::::::::::Temperature Limit::::::::::::::\n" << std::endl;

    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    ctl_result_t status              = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Error Getting Properties" << DecodeRetCode(status) << std::endl;
        return;
    }

    std::cout << "Getting current Temperature Limit" << std::endl << std::endl;
    double CurrentTemperatureLimit = 0.0;

    status = ctlOverclockTemperatureLimitGet(hDAhandle, &CurrentTemperatureLimit);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Current Temperature Limit:" << CurrentTemperatureLimit << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\nSetting current Temperature Limit" << std::endl << std::endl;

    CurrentTemperatureLimit = OcProperties.temperatureLimit.Default + 5.0;

    status = ctlOverclockTemperatureLimitSet(hDAhandle, CurrentTemperatureLimit);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Temperature Limit set correctly." << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\nGetting the now current Temperature Limit:" << std::endl << std::endl;

    double NewCurrentTemperatureLimit = 0.0;

    status = ctlOverclockTemperatureLimitGet(hDAhandle, &NewCurrentTemperatureLimit);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Current Temperature Limit:" << NewCurrentTemperatureLimit << std::endl;
        std::cout << "Requested Temperature Limit:" << CurrentTemperatureLimit << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    // Resetting to default
    CurrentTemperatureLimit = OcProperties.temperatureLimit.Default;
    ctlOverclockTemperatureLimitSet(hDAhandle, CurrentTemperatureLimit);

    std::cout << std::endl << std::endl;
}

/***************************************************************
 * @brief Main Function
 *
 * Overclock Power Temeletry: The function ctlPowerTelemetryGet allows
 * to retrieve all the available metrics from the adapter in one
 * single and efficient call.
 * @param
 * @return
 ***************************************************************/
void OverclockPowerTelemetry(ctl_device_adapter_handle_t hDAhandle)
{
    std::cout << "\n::::::::::::::Print Telemetry::::::::::::::\n" << std::endl;

    ctl_power_telemetry_t pPowerTelemetry = {};
    pPowerTelemetry.Size                  = sizeof(ctl_power_telemetry_t);
    ctl_result_t status                   = ctlPowerTelemetryGet(hDAhandle, &pPowerTelemetry);

    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Telemetry Success" << std::endl << std::endl;

        std::cout << "TimeStamp:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.timeStamp.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.timeStamp.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.timeStamp.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.timeStamp.value.datadouble << std::endl << std::endl;

        std::cout << "Gpu Energy Counter:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.gpuEnergyCounter.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.gpuEnergyCounter.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.gpuEnergyCounter.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.gpuEnergyCounter.value.datadouble << std::endl << std::endl;

        std::cout << "Gpu Voltage:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.gpuVoltage.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.gpuVoltage.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.gpuVoltage.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.gpuVoltage.value.datadouble << std::endl << std::endl;

        std::cout << "Gpu Current Frequency:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.gpuCurrentClockFrequency.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.gpuCurrentClockFrequency.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.gpuCurrentClockFrequency.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.gpuCurrentClockFrequency.value.datadouble << std::endl << std::endl;

        std::cout << "Gpu Current Temperature:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.gpuCurrentTemperature.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.gpuCurrentTemperature.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.gpuCurrentTemperature.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.gpuCurrentTemperature.value.datadouble << std::endl << std::endl;

        std::cout << "Gpu Activity Counter:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.globalActivityCounter.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.globalActivityCounter.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.globalActivityCounter.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.globalActivityCounter.value.datadouble << std::endl << std::endl;

        std::cout << "Render Activity Counter:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.renderComputeActivityCounter.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.renderComputeActivityCounter.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.renderComputeActivityCounter.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.renderComputeActivityCounter.value.datadouble << std::endl << std::endl;

        std::cout << "Media Activity Counter:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.mediaActivityCounter.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.mediaActivityCounter.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.mediaActivityCounter.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.mediaActivityCounter.value.datadouble << std::endl << std::endl;

        std::cout << "VRAM Energy Counter:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.vramEnergyCounter.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.vramEnergyCounter.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.vramEnergyCounter.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.vramEnergyCounter.value.datadouble << std::endl << std::endl;

        std::cout << "VRAM Voltage:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.vramVoltage.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.vramVoltage.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.vramVoltage.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.vramVoltage.value.datadouble << std::endl << std::endl;

        std::cout << "VRAM Frequency:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.vramCurrentClockFrequency.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.vramCurrentClockFrequency.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.vramCurrentClockFrequency.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.vramCurrentClockFrequency.value.datadouble << std::endl << std::endl;

        std::cout << "VRAM Effective Frequency:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.vramCurrentEffectiveFrequency.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.vramCurrentEffectiveFrequency.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.vramCurrentEffectiveFrequency.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.vramCurrentEffectiveFrequency.value.datadouble << std::endl << std::endl;

        std::cout << "VRAM Read Bandwidth:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.vramReadBandwidthCounter.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.vramReadBandwidthCounter.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.vramReadBandwidthCounter.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.vramReadBandwidthCounter.value.datadouble << std::endl << std::endl;

        std::cout << "VRAM Write Bandwidth:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.vramWriteBandwidthCounter.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.vramWriteBandwidthCounter.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.vramWriteBandwidthCounter.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.vramWriteBandwidthCounter.value.datadouble << std::endl << std::endl;

        std::cout << "VRAM Temperature:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.vramCurrentTemperature.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.vramCurrentTemperature.units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.vramCurrentTemperature.type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.vramCurrentTemperature.value.datadouble << std::endl << std::endl;

        std::cout << "Fan Speed:" << std::endl;
        std::cout << "Supported: " << ((pPowerTelemetry.fanSpeed[0].bSupported) ? "true" : "false") << std::endl;
        std::cout << "Units: " << printUnits(pPowerTelemetry.fanSpeed[0].units) << std::endl;
        std::cout << "Type: " << printType(pPowerTelemetry.fanSpeed[0].type) << std::endl;
        std::cout << "Value: " << pPowerTelemetry.fanSpeed[0].value.datadouble << std::endl << std::endl;
    }
    else
    {
        std::cout << "Error: " << DecodeRetCode(status) << std::endl;
    }

    std::cout << std::endl << std::endl;
}

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
    CtlInitArgs.flags      = CTL_INIT_FLAG_USE_LEVEL_ZERO;
    CtlInitArgs.Size       = sizeof(CtlInitArgs);
    CtlInitArgs.Version    = 0;
    ZeroMemory(&CtlInitArgs.ApplicationUID, sizeof(ctl_application_id_t));
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
                return ERROR;
            }
            Result = ctlEnumerateDevices(hAPIHandle, &Adapter_count, hDevices);
        }
        if (CTL_RESULT_SUCCESS != Result)
        {
            printf("ctlEnumerateDevices returned failure code: 0x%X\n", Result);
            goto free_exit;
        }

        for (Index = 0; Index < Adapter_count; Index++)
        {
            if (NULL != hDevices[Index])
            {
                LUID AdapterID;
                StDeviceAdapterProperties.Size           = sizeof(ctl_device_adapter_properties_t);
                StDeviceAdapterProperties.pDeviceID      = malloc(sizeof(LUID));
                StDeviceAdapterProperties.device_id_size = sizeof(LUID);

                if (NULL == StDeviceAdapterProperties.pDeviceID)
                {
                    return ERROR;
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
                    std::cout << "Adapter ID " << AdapterID.LowPart << "\n";
                }

                if (0x8086 == StDeviceAdapterProperties.pci_vendor_id)
                {
                    std::cout << "Intel Adapter Name " << StDeviceAdapterProperties.name << "\n";
                    std::cout << "Vendor id  " << StDeviceAdapterProperties.pci_vendor_id << "\n";
                    std::cout << "Device id " << StDeviceAdapterProperties.pci_device_id << "\n";
                    std::cout << "Rev id " << StDeviceAdapterProperties.rev_id << "\n";
                }

                OverclockProperties(hDevices[Index]);
                OverclockFrequencyOffset(hDevices[Index]);
                OverclockVoltageOffset(hDevices[Index]);
                OverclockLockFrequency(hDevices[Index]);
                OverclockPowerLimit(hDevices[Index]);
                OverclockTemperatureLimit(hDevices[Index]);

                // Telemetry
                // Polling during 1 second at 100 ms
                for (uint32_t i = 0; i < 10; i++)
                {
                    OverclockPowerTelemetry(hDevices[Index]);
                    Sleep(100);
                }

                if (NULL != StDeviceAdapterProperties.pDeviceID)
                {
                    free(StDeviceAdapterProperties.pDeviceID);
                }
            }
        }
    }

free_exit:

    ctlClose(hAPIHandle);

    if (hDisplayOutput != nullptr)
    {
        free(hDisplayOutput);
        hDisplayOutput = nullptr;
    }
    hDisplayOutput = nullptr;

    if (hDevices != nullptr)
    {
        free(hDevices);
        hDevices = nullptr;
    }

    return 0;
}

// Decoding the return code for the most common error codes.
std::string DecodeRetCode(ctl_result_t Res)
{
    switch (Res)
    {
        case CTL_RESULT_SUCCESS:
        {
            return std::string("[CTL_RESULT_SUCCESS]");
        }
        case CTL_RESULT_ERROR_CORE_OVERCLOCK_NOT_SUPPORTED:
        {
            return std::string("[CTL_RESULT_ERROR_CORE_OVERCLOCK_NOT_SUPPORTED]");
        }
        case CTL_RESULT_ERROR_CORE_OVERCLOCK_VOLTAGE_OUTSIDE_RANGE:
        {
            return std::string("[CTL_RESULT_ERROR_CORE_OVERCLOCK_VOLTAGE_OUTSIDE_RANGE]");
        }
        case CTL_RESULT_ERROR_CORE_OVERCLOCK_FREQUENCY_OUTSIDE_RANGE:
        {
            return std::string("[CTL_RESULT_ERROR_CORE_OVERCLOCK_FREQUENCY_OUTSIDE_RANGE]");
        }
        case CTL_RESULT_ERROR_CORE_OVERCLOCK_POWER_OUTSIDE_RANGE:
        {
            return std::string("[CTL_RESULT_ERROR_CORE_OVERCLOCK_POWER_OUTSIDE_RANGE]");
        }
        case CTL_RESULT_ERROR_CORE_OVERCLOCK_TEMPERATURE_OUTSIDE_RANGE:
        {
            return std::string("[CTL_RESULT_ERROR_CORE_OVERCLOCK_TEMPERATURE_OUTSIDE_RANGE]");
        }
        case CTL_RESULT_ERROR_GENERIC_START:
        {
            return std::string("[CTL_RESULT_ERROR_GENERIC_START]");
        }
        case CTL_RESULT_ERROR_CORE_OVERCLOCK_RESET_REQUIRED:
        {
            return std::string("[CTL_RESULT_ERROR_CORE_OVERCLOCK_RESET_REQUIRED]");
        }
        case CTL_RESULT_ERROR_CORE_OVERCLOCK_IN_VOLTAGE_LOCKED_MODE:
        {
            return std::string("[CTL_RESULT_ERROR_CORE_OVERCLOCK_IN_VOLTAGE_LOCKED_MODE");
        }
        case CTL_RESULT_ERROR_CORE_OVERCLOCK_WAIVER_NOT_SET:
        {
            return std::string("[CTL_RESULT_ERROR_CORE_OVERCLOCK_WAIVER_NOT_SET]");
        }
        case CTL_RESULT_ERROR_NOT_INITIALIZED:
        {
            return std::string("[CTL_RESULT_ERROR_NOT_INITIALIZED]");
        }
        case CTL_RESULT_ERROR_ALREADY_INITIALIZED:
        {
            return std::string("[CTL_RESULT_ERROR_ALREADY_INITIALIZED]");
        }
        case CTL_RESULT_ERROR_DEVICE_LOST:
        {
            return std::string("[CTL_RESULT_ERROR_DEVICE_LOST]");
        }
        case CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS:
        {
            return std::string("[CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS]");
        }
        case CTL_RESULT_ERROR_NOT_AVAILABLE:
        {
            return std::string("[CTL_RESULT_ERROR_NOT_AVAILABLE]");
        }
        case CTL_RESULT_ERROR_UNINITIALIZED:
        {
            return std::string("[CTL_RESULT_ERROR_UNINITIALIZED]");
        }
        case CTL_RESULT_ERROR_UNSUPPORTED_VERSION:
        {
            return std::string("[CTL_RESULT_ERROR_UNSUPPORTED_VERSION]");
        }
        case CTL_RESULT_ERROR_UNSUPPORTED_FEATURE:
        {
            return std::string("[CTL_RESULT_ERROR_UNSUPPORTED_FEATURE]");
        }
        case CTL_RESULT_ERROR_INVALID_ARGUMENT:
        {
            return std::string("[CTL_RESULT_ERROR_INVALID_ARGUMENT]");
        }
        case CTL_RESULT_ERROR_INVALID_NULL_HANDLE:
        {
            return std::string("[CTL_RESULT_ERROR_INVALID_NULL_HANDLE]");
        }
        case CTL_RESULT_ERROR_INVALID_NULL_POINTER:
        {
            return std::string("[CTL_RESULT_ERROR_INVALID_NULL_POINTER]");
        }
        case CTL_RESULT_ERROR_INVALID_SIZE:
        {
            return std::string("[CTL_RESULT_ERROR_INVALID_SIZE]");
        }
        case CTL_RESULT_ERROR_UNSUPPORTED_SIZE:
        {
            return std::string("[CTL_RESULT_ERROR_UNSUPPORTED_SIZE]");
        }
        case CTL_RESULT_ERROR_NOT_IMPLEMENTED:
        {
            return std::string("[CTL_RESULT_ERROR_NOT_IMPLEMENTED]");
        }
        case CTL_RESULT_ERROR_ZE_LOADER:
        {
            return std::string("[CTL_RESULT_ERROR_ZE_LOADER]");
        }
        case CTL_RESULT_ERROR_INVALID_OPERATION_TYPE:
        {
            return std::string("[CTL_RESULT_ERROR_INVALID_OPERATION_TYPE]");
        }
        case CTL_RESULT_ERROR_UNKNOWN:
        {
            return std::string("[CTL_RESULT_ERROR_UNKNOWN]");
        }
        default:
            return std::string("[Unknown Error]");
    }
}