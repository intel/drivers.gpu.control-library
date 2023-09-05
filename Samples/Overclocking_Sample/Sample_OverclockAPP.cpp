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
#include "GenericIGCLApp.h"

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
        case ctl_units_t::CTL_UNITS_POWER_MILLIWATTS:
        {
            return "Power in Milli Watts";
        }
        break;
        case ctl_units_t::CTL_UNITS_PERCENT:
        {
            return "Units in Percentage";
        }
        break;
        case ctl_units_t::CTL_UNITS_MEM_SPEED_GBPS:
        {
            return "Units in Gigabyte Per Second";
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
    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);

    ctl_result_t Status = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError: %s", DecodeRetCode(Status).c_str());
        return;
    }

    // Is Overclocking supported by this adapter?
    PRINT_LOGS("\nOc Supported? %s", OcProperties.bSupported ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Max: %f\n", OcProperties.gpuFrequencyOffset.max);

    // Slider for frequency offset
    PRINT_LOGS("\nGpu Frequency Offset Supported? %s ", OcProperties.gpuFrequencyOffset.bSupported ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Supported? %s", OcProperties.gpuFrequencyOffset.bSupported ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Is Relative? %s", OcProperties.gpuFrequencyOffset.bRelative ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Default: %f", OcProperties.gpuFrequencyOffset.Default);
    PRINT_LOGS("\nGpu Frequency Offset Min: %f", OcProperties.gpuFrequencyOffset.min);
    PRINT_LOGS("\nGpu Frequency Offset Max: %f", OcProperties.gpuFrequencyOffset.max);
    PRINT_LOGS("\nGpu Frequency Offset Reference: %f", OcProperties.gpuFrequencyOffset.reference);
    PRINT_LOGS("\nGpu Frequency Offset Step: %f", OcProperties.gpuFrequencyOffset.step);
    PRINT_LOGS("\nGpu Frequency Offset Units:: %s\n", printUnits(OcProperties.gpuFrequencyOffset.units));

    // Slider for voltage offset
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nGpu Voltage Offset Default: %f", OcProperties.gpuVoltageOffset.Default);
    PRINT_LOGS("\n Gpu Voltage Offset Min : %f", OcProperties.gpuVoltageOffset.min);
    PRINT_LOGS("\nGpu Voltage Offset Max: %f", OcProperties.gpuVoltageOffset.max);
    PRINT_LOGS("\nGpu Voltage Offset Reference: %f", OcProperties.gpuVoltageOffset.reference);
    PRINT_LOGS("\nGpu Voltage Offset Step: %f", OcProperties.gpuVoltageOffset.step);
    PRINT_LOGS("\nGpu Voltage Offset Units: %s\n", printUnits(OcProperties.gpuVoltageOffset.units));

    // Slider for VRAM frequency offset
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nVRAM Frequency Offset Default: %f", OcProperties.vramFrequencyOffset.Default);
    PRINT_LOGS("\nVRAM Frequency Offset Min : %f", OcProperties.vramFrequencyOffset.min);
    PRINT_LOGS("\nVRAM Frequency Offset Max: %f", OcProperties.vramFrequencyOffset.max);
    PRINT_LOGS("\nVRAM Frequency Offset Reference: %f", OcProperties.vramFrequencyOffset.reference);
    PRINT_LOGS("\nVRAM Frequency Offset Step: %f", OcProperties.vramFrequencyOffset.step);
    PRINT_LOGS("\nVRAM Frequency Offset Units: %s\n", printUnits(OcProperties.vramFrequencyOffset.units));

    // Slider for VRAM voltage offset
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nVRAM Voltage Offset Default: %f", OcProperties.vramVoltageOffset.Default);
    PRINT_LOGS("\nVRAM Voltage Offset Min: %f", OcProperties.vramVoltageOffset.min);
    PRINT_LOGS("\nVRAM Voltage Offset Max: %f", OcProperties.vramVoltageOffset.max);
    PRINT_LOGS("\nVRAM Voltage Offset Reference : %f", OcProperties.vramVoltageOffset.reference);
    PRINT_LOGS("\nVRAM Voltage Offset Step: %f", OcProperties.vramVoltageOffset.step);
    PRINT_LOGS("\nVRAM Voltage Offset Units: %s\n", printUnits(OcProperties.vramVoltageOffset.units));

    // Slider for Power Limit
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("G\npu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nPower Limit Default: %f", OcProperties.powerLimit.Default);
    PRINT_LOGS("\nPower Limit Min: %f", OcProperties.powerLimit.min);
    PRINT_LOGS("\nPower Limit Max: %f", OcProperties.powerLimit.max);
    PRINT_LOGS("\nPower Limit Reference: %f", OcProperties.powerLimit.reference);
    PRINT_LOGS("\nPower Limit Step: %f", OcProperties.powerLimit.step);
    PRINT_LOGS("\nPower Limit Units: %s\n", printUnits(OcProperties.powerLimit.units));

    // Slider for Temperature Limit
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", OcProperties.gpuFrequencyOffset.bReference ? "true" : "false");
    PRINT_LOGS("\nTemperature Limit Default: %f", OcProperties.temperatureLimit.Default);
    PRINT_LOGS("\nTemperature Limit Min: %f", OcProperties.temperatureLimit.min);
    PRINT_LOGS("\nTemperature Limit Max: %f", OcProperties.temperatureLimit.max);
    PRINT_LOGS("\nTemperature Limit Reference: %f", OcProperties.temperatureLimit.reference);
    PRINT_LOGS("\nTemperature Limit Step: %f", OcProperties.temperatureLimit.step);
    PRINT_LOGS("\nTemperature Limit Units: %s\n \n", printUnits(OcProperties.temperatureLimit.units));
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
    double GPUFrequencyOffset = 0.0;

    ctl_result_t Status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        goto Exit;
    }

    PRINT_LOGS("\nCurrent Frequency Offset: %f MHz", GPUFrequencyOffset);

    // Setting 50 MHz Offset
    GPUFrequencyOffset = 50.0;

    // Calling the waiver first
    Status = ctlOverclockWaiverSet(hDAhandle);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        goto Exit;
    }

    // Setting the Offset
    Status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        goto Exit;
    }

    // Reseting local var
    GPUFrequencyOffset = 0.0;

    // Read back to confirm the new value
    Status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(Status).c_str());
        goto Exit;
    }

    PRINT_LOGS("\nCurrent New Frequency Offset: %f MHz\n \n", GPUFrequencyOffset);

    // Setting the Offset to 0
    GPUFrequencyOffset = 0.0;
    ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);

Exit:
    return;
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
    double VoltageOffset = 0.0;
    ctl_result_t Status  = ctlOverclockGpuVoltageOffsetGet(hDAhandle, &VoltageOffset);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        goto Exit;
    }

    PRINT_LOGS("\n Current Voltage Offset: %f V", VoltageOffset);

    // Setting 25 mV Voltage Offset
    VoltageOffset = 25;

    // Calling the waiver first
    Status = ctlOverclockWaiverSet(hDAhandle);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        goto Exit;
    }

    // Setting the Offset
    Status = ctlOverclockGpuVoltageOffsetSet(hDAhandle, VoltageOffset);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        goto Exit;
    }

    // Reseting local var
    VoltageOffset = 0.0;

    // Read back to confirm the new value
    Status = ctlOverclockGpuVoltageOffsetGet(hDAhandle, &VoltageOffset);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        goto Exit;
    }

    PRINT_LOGS("\nCurrent New Voltage Offset: %f\n \n", VoltageOffset);

    // Setting the Offset to 0
    VoltageOffset = 0.0;
    ctlOverclockGpuVoltageOffsetSet(hDAhandle, VoltageOffset);

Exit:
    return;
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
    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    ctl_oc_vf_pair_t VfPair          = {};

    ctl_result_t Status = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        goto Exit;
    }

    Status = ctlOverclockGpuLockGet(hDAhandle, &VfPair);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        goto Exit;
    }

    PRINT_LOGS("\nCurrent Locked Frequency: %f MHz", VfPair.Frequency);
    PRINT_LOGS("\nCurrent Locked Voltage: %f mV", VfPair.Voltage);

    VfPair = {};
    // Set slighly more frequency and voltage
    // 50 MHz over reference frequency
    VfPair.Frequency = OcProperties.gpuFrequencyOffset.reference + 50.0;
    // 50 mV over reference voltage. Convert to mV first.
    VfPair.Voltage = OcProperties.gpuVoltageOffset.reference * 1000.0 + 50.0;

    Status = ctlOverclockGpuLockSet(hDAhandle, VfPair);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        goto Exit;
    }

    // Reseting local var
    VfPair = {};
    Status = ctlOverclockGpuLockGet(hDAhandle, &VfPair);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        goto Exit;
    }

    // Verifying the new locked frequency and voltage
    PRINT_LOGS("\nNew Current Locked Frequency: %f MHz", VfPair.Frequency);
    PRINT_LOGS("\nNew Current Locked Voltage: %f mV \n \n", VfPair.Voltage);

    // Reset to default
    VfPair = {};
    ctlOverclockGpuLockSet(hDAhandle, VfPair);

Exit:
    return;
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
    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);

    ctl_result_t Status = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        return;
    }

    PRINT_LOGS("\nGetting current Power Limit\n \n");

    double CurrentPowerLimit = 0.0;

    Status = ctlOverclockPowerLimitGet(hDAhandle, &CurrentPowerLimit);

    if (Status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nCurrent Sustained Power Limit: %f", CurrentPowerLimit);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
    }

    PRINT_LOGS("\nSetting current Power Limit inside limits: \n \n");

    // Convert to mW and get min + 15 W.
    CurrentPowerLimit = OcProperties.powerLimit.Default * 1000.0 + 15000.0;

    Status = ctlOverclockPowerLimitSet(hDAhandle, CurrentPowerLimit);

    if (Status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nPower Limit set correctly.\n \n");
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
    }

    PRINT_LOGS("\nGetting the now current Power Limit:\n \n");

    double NewCurrentPowerLimit = 0.0;

    Status = ctlOverclockPowerLimitGet(hDAhandle, &NewCurrentPowerLimit);

    if (Status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nCurrent Sustained Power Limit: %f", NewCurrentPowerLimit);
        PRINT_LOGS("\nRequested Sustained Power Limit: %f", CurrentPowerLimit);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s\n \n", DecodeRetCode(Status).c_str());
    }

    // Reset to default
    CurrentPowerLimit = OcProperties.powerLimit.Default * 1000.0;
    ctlOverclockPowerLimitSet(hDAhandle, CurrentPowerLimit);
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
    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    ctl_result_t Result              = CTL_RESULT_SUCCESS;

    ctl_result_t Status = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (Status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
        return;
    }

    PRINT_LOGS("\nGetting current Temperature Limit\n");
    double CurrentTemperatureLimit = 0.0;

    Status = ctlOverclockTemperatureLimitGet(hDAhandle, &CurrentTemperatureLimit);

    if (Status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nCurrent Temperature Limit: %f\n", CurrentTemperatureLimit);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
    }

    PRINT_LOGS("\nSetting current Temperature Limit \n \n");

    CurrentTemperatureLimit = OcProperties.temperatureLimit.Default + 5.0;

    Status = ctlOverclockTemperatureLimitSet(hDAhandle, CurrentTemperatureLimit);

    if (Status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nTemperature Limit set correctly.\n");
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s\n", DecodeRetCode(Status).c_str());
    }

    PRINT_LOGS("\nGetting the now current Temperature Limit:\n \n");

    double NewCurrentTemperatureLimit = 0.0;

    Status = ctlOverclockTemperatureLimitGet(hDAhandle, &NewCurrentTemperatureLimit);

    if (Status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nCurrent Temperature Limit: %f", NewCurrentTemperatureLimit);
        PRINT_LOGS("\nRequested Temperature Limit: %f", CurrentTemperatureLimit);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s\n \n", DecodeRetCode(Status).c_str());
    }

    // Resetting to default
    CurrentTemperatureLimit = OcProperties.temperatureLimit.Default;
    ctlOverclockTemperatureLimitSet(hDAhandle, CurrentTemperatureLimit);
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
    ctl_power_telemetry_t pPowerTelemetry = {};
    pPowerTelemetry.Size                  = sizeof(ctl_power_telemetry_t);

    ctl_result_t Status = ctlPowerTelemetryGet(hDAhandle, &pPowerTelemetry);

    if (Status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nTelemetry Success\n \n");

        PRINT_LOGS("\nTimeStamp");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.timeStamp.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits %s", printUnits(pPowerTelemetry.timeStamp.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.timeStamp.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.timeStamp.value.datadouble);

        PRINT_LOGS("\nGpu Energy Counter:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.gpuEnergyCounter.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits:%s", printUnits(pPowerTelemetry.gpuEnergyCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.gpuEnergyCounter.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.gpuEnergyCounter.value.datadouble);

        PRINT_LOGS("\nGpu Voltage:");
        PRINT_LOGS("\n Supported : %s", ((pPowerTelemetry.gpuVoltage.bSupported) ? " true " : " false "));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.gpuVoltage.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.gpuVoltage.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.gpuVoltage.value.datadouble);

        PRINT_LOGS("\nGpu Current Frequency:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.gpuCurrentClockFrequency.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.gpuCurrentClockFrequency.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.gpuCurrentClockFrequency.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.gpuCurrentClockFrequency.value.datadouble);

        PRINT_LOGS("\nGpu Current Temperature:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.gpuCurrentTemperature.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.gpuCurrentTemperature.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.gpuCurrentTemperature.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.gpuCurrentTemperature.value.datadouble);

        PRINT_LOGS("\nGpu Activity Counter:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.globalActivityCounter.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.globalActivityCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.globalActivityCounter.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.globalActivityCounter.value.datadouble);

        PRINT_LOGS("\nRender Activity Counter:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.renderComputeActivityCounter.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.renderComputeActivityCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.renderComputeActivityCounter.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.renderComputeActivityCounter.value.datadouble);

        PRINT_LOGS("\nMedia Activity Counter:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.mediaActivityCounter.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.mediaActivityCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.mediaActivityCounter.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.mediaActivityCounter.value.datadouble);

        PRINT_LOGS("\nVRAM Energy Counter:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.vramEnergyCounter.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramEnergyCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramEnergyCounter.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.vramEnergyCounter.value.datadouble);

        PRINT_LOGS("\nVRAM Voltage:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.vramVoltage.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramVoltage.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramVoltage.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.vramVoltage.value.datadouble);

        PRINT_LOGS("\nVRAM Frequency:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.vramCurrentClockFrequency.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramCurrentClockFrequency.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramCurrentClockFrequency.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.vramCurrentClockFrequency.value.datadouble);

        PRINT_LOGS("\nVRAM Effective Frequency:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.vramCurrentEffectiveFrequency.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramCurrentEffectiveFrequency.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramCurrentEffectiveFrequency.type));
        PRINT_LOGS("\nValue: %f \n", pPowerTelemetry.vramCurrentEffectiveFrequency.value.datadouble);

        PRINT_LOGS("\nVRAM Read Bandwidth:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.vramReadBandwidthCounter.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramReadBandwidthCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramReadBandwidthCounter.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.vramReadBandwidthCounter.value.datadouble);

        PRINT_LOGS("\nVRAM Write Bandwidth:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.vramWriteBandwidthCounter.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramWriteBandwidthCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramWriteBandwidthCounter.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.vramWriteBandwidthCounter.value.datadouble);

        PRINT_LOGS("\nVRAM Temperature:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.vramCurrentTemperature.bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramCurrentTemperature.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramCurrentTemperature.type));
        PRINT_LOGS("\nValue: %f\n", pPowerTelemetry.vramCurrentTemperature.value.datadouble);

        PRINT_LOGS("\nFan Speed:");
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", (pPowerTelemetry.fanSpeed[0].bSupported) ? "true" : "false");
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.fanSpeed[0].units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.fanSpeed[0].type));
        PRINT_LOGS("\nValue: %f \n \n", pPowerTelemetry.fanSpeed[0].value.datadouble);
    }
    else
    {
        PRINT_LOGS("\nError: %s \n \n", DecodeRetCode(Status).c_str());
    }
}

int main()
{
    ctl_result_t Result                                       = CTL_RESULT_SUCCESS;
    ctl_device_adapter_handle_t *hDevices                     = nullptr;
    ctl_display_output_handle_t *hDisplayOutput               = nullptr;
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

    Result = ctlInit(&CtlInitArgs, &hAPIHandle);

    if (CTL_RESULT_SUCCESS == Result)
    {
        // Initialization successful
        // Get the list of Intel Adapters
        Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);

        if (CTL_RESULT_SUCCESS == Result)
        {
            hDevices = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * AdapterCount);

            if (hDevices == NULL)
            {
                goto Exit;
            }

            Result = ctlEnumerateDevices(hAPIHandle, &AdapterCount, hDevices);
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

                    if (NULL != StDeviceAdapterProperties.pDeviceID)
                    {
                        free(StDeviceAdapterProperties.pDeviceID);
                    }
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

                CTL_FREE_MEM(StDeviceAdapterProperties.pDeviceID);
            }
        }
    }

Exit:

    if (ctlClose(hAPIHandle) != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError: %s", DecodeRetCode(ctlClose(hAPIHandle)).c_str());
        return 0;
    }

    CTL_FREE_MEM(hDisplayOutput);
    CTL_FREE_MEM(hDevices);
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