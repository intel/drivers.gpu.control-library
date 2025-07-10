//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 * @author daniel.enriquez.montanez@intel.com
 * @file  Sample_TelemetryAPP.cpp
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

void CtlTemperatureTest(ctl_device_adapter_handle_t hDAhandle);
void CtlFrequencyTest(ctl_device_adapter_handle_t hDAhandle);
void CtlPowerTest(ctl_device_adapter_handle_t hDAhandle);
void CtlFanTest_FanGetConfig(ctl_fan_handle_t hFanHandle);
void CtlFanTest(ctl_device_adapter_handle_t hDAhandle);
void CtlPciTest(ctl_device_adapter_handle_t hDAhandle);
void CtlMemoryTest(ctl_device_adapter_handle_t hDAhandle);
void CtlEngineTest(ctl_device_adapter_handle_t hDAhandle);
void CtlLedTest(ctl_device_adapter_handle_t hDAhandle);
void CtlEccTest(ctl_device_adapter_handle_t hDAhandle);
void CtlPowerTelemetryTest(ctl_device_adapter_handle_t hDAhandle);

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

std::string DecodeTemperatureSensor(ctl_temp_sensors_t tempSensor)
{
    static const std::map<ctl_temp_sensors_t, std::string> tempSensorStringMap = { { CTL_TEMP_SENSORS_GLOBAL, "GLOBAL" }, { CTL_TEMP_SENSORS_GPU, "GPU" }, { CTL_TEMP_SENSORS_MEMORY, "MEMORY" } };

    auto it = tempSensorStringMap.find(tempSensor);
    if (it != tempSensorStringMap.end())
    {
        return it->second;
    }
    return "Unknown temperature sensor";
}

std::string DecodeFrequencyDomain(ctl_freq_domain_t freqDomain)
{
    static const std::map<ctl_freq_domain_t, std::string> freqDomainStringMap = { { CTL_FREQ_DOMAIN_GPU, "GPU" }, { CTL_FREQ_DOMAIN_MEMORY, "MEMORY" }, { CTL_FREQ_DOMAIN_MEDIA, "MEDIA" } };

    auto it = freqDomainStringMap.find(freqDomain);
    if (it != freqDomainStringMap.end())
    {
        return it->second;
    }
    return "Unknown frequency domain";
}

std::string DecodeFanSpeedUnits(ctl_fan_speed_units_t Units)
{
    static const std::map<ctl_fan_speed_units_t, std::string> unitsStringMap = { { CTL_FAN_SPEED_UNITS_RPM, "RPM" }, { CTL_FAN_SPEED_UNITS_PERCENT, "PERCENT" } };

    auto it = unitsStringMap.find(Units);
    if (it != unitsStringMap.end())
    {
        return it->second;
    }
    return "Unknown fan speed unit";
}

std::string DecodeFanSpeedMode(ctl_fan_speed_mode_t fanSpeedMode)
{
    static const std::map<ctl_fan_speed_mode_t, std::string> fanSpeedModeStringMap = { { CTL_FAN_SPEED_MODE_DEFAULT, "DEFAULT/STOCK FAN TABLE" },
                                                                                       { CTL_FAN_SPEED_MODE_FIXED, "FIXED SPEED" },
                                                                                       { CTL_FAN_SPEED_MODE_TABLE, "USER FAN TABLE" } };

    auto it = fanSpeedModeStringMap.find(fanSpeedMode);
    if (it != fanSpeedModeStringMap.end())
    {
        return it->second;
    }
    return "Unknown fan speed mode";
}

std::string DecodeEngineGroup(ctl_engine_group_t engineGroup)
{
    static const std::map<ctl_engine_group_t, std::string> engineGroupStringMap = { { CTL_ENGINE_GROUP_GT, "GT" }, { CTL_ENGINE_GROUP_RENDER, "RENDER" }, { CTL_ENGINE_GROUP_MEDIA, "MEDIA" } };

    auto it = engineGroupStringMap.find(engineGroup);
    if (it != engineGroupStringMap.end())
    {
        return it->second;
    }
    return "Unknown engine group";
}

std::string DecodeBoolean(bool Value)
{
    return Value ? "true" : "false";
}

std::string DecodeEccState(ctl_ecc_state_t eccState)
{
    static const std::map<ctl_ecc_state_t, std::string> eccStateStringMap = { { CTL_ECC_STATE_ECC_DEFAULT_STATE, "default" },
                                                                              { CTL_ECC_STATE_ECC_ENABLED_STATE, "enabled" },
                                                                              { CTL_ECC_STATE_ECC_DISABLED_STATE, "disabled" } };

    auto it = eccStateStringMap.find(eccState);
    if (it != eccStateStringMap.end())
    {
        return it->second;
    }
    return "Unknown Ecc state";
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
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlTemperatureTest(ctl_device_adapter_handle_t hDAhandle)
{
    PRINT_LOGS("\n::::::::::::::Print Temperature Properties::::::::::::::\n");

    uint32_t TemperatureHandlerCount = 0;
    ctl_result_t res                 = ctlEnumTemperatureSensors(hDAhandle, &TemperatureHandlerCount, nullptr);
    if ((res != CTL_RESULT_SUCCESS) || TemperatureHandlerCount == 0)
    {
        PRINT_LOGS("\nTemperature component not supported. Error: %s", DecodeRetCode(res).c_str());
        return;
    }
    else
    {
        PRINT_LOGS("\nNumber of Temperature Handles [%u]", TemperatureHandlerCount);
    }

    ctl_temp_handle_t *pTtemperatureHandle = new ctl_temp_handle_t[TemperatureHandlerCount];

    res = ctlEnumTemperatureSensors(hDAhandle, &TemperatureHandlerCount, pTtemperatureHandle);

    if (res != CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError: %s for Temperature handle.", DecodeRetCode(res).c_str());
        goto cleanUp;
    }

    for (uint32_t i = 0; i < TemperatureHandlerCount; i++)
    {
        PRINT_LOGS("\n\nFor Temperature Handle [%u]", i);

        PRINT_LOGS("\n[Temperature] Get Temperature properties:");

        ctl_temp_properties_t temperatureProperties = { 0 };
        temperatureProperties.Size                  = sizeof(ctl_temp_properties_t);
        res                                         = ctlTemperatureGetProperties(pTtemperatureHandle[i], &temperatureProperties);

        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\nError: %s from Temperature get properties.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Temperature] Max temp [%u]", (uint32_t)temperatureProperties.maxTemperature);
            PRINT_LOGS("\n[Temperature] Sensor type [%s]", DecodeTemperatureSensor(temperatureProperties.type).c_str());
        }

        PRINT_LOGS("\n[Temperature] Get Temperature state:");

        double temperature = 0;
        res                = ctlTemperatureGetState(pTtemperatureHandle[i], &temperature);

        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\nError: %s  from Temperature get state.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Temperature] Current Temperature [%f] C degrees \n \n", temperature);
        }
    }

cleanUp:
    delete[] pTtemperatureHandle;
    pTtemperatureHandle = nullptr;
}

/***************************************************************
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlFrequencyTest(ctl_device_adapter_handle_t hDAhandle)
{

    uint32_t FrequencyHandlerCount = 0;
    ctl_result_t res               = ctlEnumFrequencyDomains(hDAhandle, &FrequencyHandlerCount, nullptr);
    if ((res != CTL_RESULT_SUCCESS) || FrequencyHandlerCount == 0)
    {
        PRINT_LOGS("\nTemperature component not supported. Error: %s", DecodeRetCode(res).c_str());
        return;
    }
    else
    {
        PRINT_LOGS("\nNumber of Frequency Handles [%u]", FrequencyHandlerCount);
    }

    ctl_freq_handle_t *pFrequencyHandle = new ctl_freq_handle_t[FrequencyHandlerCount];

    res = ctlEnumFrequencyDomains(hDAhandle, &FrequencyHandlerCount, pFrequencyHandle);

    if (res != CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError: %s for Frequency handle.", DecodeRetCode(res).c_str());
        goto cleanUp;
    }

    for (uint32_t i = 0; i < FrequencyHandlerCount; i++)
    {
        PRINT_LOGS("\n\nFor Frequency Handle: [%u]", i);

        PRINT_LOGS("\n\n[Frequency] Properties:");

        ctl_freq_properties_t freqProperties = { 0 };
        freqProperties.Size                  = sizeof(ctl_freq_properties_t);
        res                                  = ctlFrequencyGetProperties(pFrequencyHandle[i], &freqProperties);
        if (res)
        {
            PRINT_LOGS("\n from Frequency get properties. %s", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Frequency] Min [%f]] MHz", freqProperties.min);
            PRINT_LOGS("\n[Frequency] Max [%f]] MHz", freqProperties.max);
            PRINT_LOGS("\n[Frequency] Can Control Frequency? [%u]]", (uint32_t)freqProperties.canControl);
            PRINT_LOGS("\n[Frequency] Frequency Domain [%s]]", DecodeFrequencyDomain(freqProperties.type).c_str());
        }

        PRINT_LOGS("\n\n[Frequency] State:");

        ctl_freq_state_t freqState = { 0 };
        freqState.Size             = sizeof(ctl_freq_state_t);
        res                        = ctlFrequencyGetState(pFrequencyHandle[i], &freqState);
        if (res)
        {
            PRINT_LOGS("\n %s from Frequency get state.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Frequency] Actual Frequency [%f] Mhz", freqState.actual);
            PRINT_LOGS("\n[Frequency] Efficient Frequency [%f] Mhz", freqState.efficient);
            PRINT_LOGS("\n[Frequency] Requested Frequency [%f] Mhz", freqState.request);
            PRINT_LOGS("\n[Frequency] Max clock Frequency for a target TDP [%f] Mhz", freqState.tdp);
            PRINT_LOGS("\n[Frequency] Throttle Reasons [%u]", freqState.throttleReasons);
            PRINT_LOGS("\n[Frequency] Voltage [%f] Volts", freqState.currentVoltage);
        }

        PRINT_LOGS("\n\n[Frequency] Get throttle time:");

        ctl_freq_throttle_time_t throttleTime = { 0 };
        throttleTime.Size                     = sizeof(ctl_freq_throttle_time_t);
        res                                   = ctlFrequencyGetThrottleTime(pFrequencyHandle[i], &throttleTime);
        if (res)
        {
            PRINT_LOGS("\n %s from Frequency get throttle time.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Frequency] Throttle Time [%llu ] s", throttleTime.throttleTime);
            PRINT_LOGS("\n[Frequency] Timestamp [%llu] s", throttleTime.timestamp);
        }

        PRINT_LOGS("\n\n[Frequency] Available clocks:");

        uint32_t numClocks = 0;
        double *clocks     = nullptr;
        res                = ctlFrequencyGetAvailableClocks(pFrequencyHandle[i], &numClocks, 0);

        if (res || numClocks == 0)
        {
            PRINT_LOGS("\n %s from Frequency get available clocks.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Frequency] Number of Available clocks [%u]", numClocks);

            clocks = new double[numClocks];

            res = ctlFrequencyGetAvailableClocks(pFrequencyHandle[i], &numClocks, clocks);
            if (CTL_RESULT_SUCCESS == res)
            {
                for (uint32_t i = 0; i < numClocks; i++)
                {
                    PRINT_LOGS("\n[Frequency] Clock [%u]  Freq[%f] MHz", i, clocks[i]);
                }
            }

            delete[] clocks;
            clocks = nullptr;
        }

        PRINT_LOGS("\n\n[Frequency] Frequency range:");

        ctl_freq_range_t freqRange = { 0 };
        freqRange.Size             = sizeof(ctl_freq_range_t);
        res                        = ctlFrequencyGetRange(pFrequencyHandle[i], &freqRange);

        if (res)
        {
            PRINT_LOGS("\n %s from Frequency get range.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Frequency] Range Max [%f] MHz", freqRange.max);
            PRINT_LOGS("\n[Frequency] Range Min [%f] MHz", freqRange.min);
        }

        PRINT_LOGS("\n\n[Frequency] Set Frequency range:");

        res = ctlFrequencySetRange(pFrequencyHandle[i], &freqRange);

        if (res)
        {
            PRINT_LOGS("\n %s from Frequency set range.", DecodeRetCode(res).c_str());
        }
        else
        {

            PRINT_LOGS("\n\n[Frequency] Success for Set Range");
        }
    }

cleanUp:
    delete[] pFrequencyHandle;
    pFrequencyHandle = nullptr;
}

void CtlPowerTest(ctl_device_adapter_handle_t hDAhandle)
{
    uint32_t PowerHandlerCount = 0;
    ctl_result_t res           = ctlEnumPowerDomains(hDAhandle, &PowerHandlerCount, nullptr);
    if ((res != CTL_RESULT_SUCCESS) || PowerHandlerCount == 0)
    {
        PRINT_LOGS("\nPower component not supported. Error: %s", DecodeRetCode(res).c_str());
        return;
    }
    else
    {
        PRINT_LOGS("\nNumber of Power Handles [%u]", PowerHandlerCount);
    }

    ctl_pwr_handle_t *pPowerHandle = new ctl_pwr_handle_t[PowerHandlerCount];

    res = ctlEnumPowerDomains(hDAhandle, &PowerHandlerCount, pPowerHandle);

    if (res != CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError: %s for Power handle.", DecodeRetCode(res).c_str());
        goto cleanUp;
    }

    for (uint32_t i = 0; i < PowerHandlerCount; i++)
    {
        PRINT_LOGS("\n\nFor Power Handle [%u]", i);

        ctl_power_properties_t properties = { 0 };
        properties.Size                   = sizeof(ctl_power_properties_t);
        res                               = ctlPowerGetProperties(pPowerHandle[i], &properties);

        PRINT_LOGS("\n\n[Power] Properties:");

        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\n %s from Power get properties.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Power] Can Control [%s]", DecodeBoolean(properties.canControl).c_str());
            PRINT_LOGS("\n[Power] Default Power Limit [%u]", (uint32_t)properties.defaultLimit);
            PRINT_LOGS("\n[Power] Max Power Limit [%i] mW", properties.maxLimit);
            PRINT_LOGS("\n[Power] Min Power Limit [%i] mW", properties.minLimit);
        }

        PRINT_LOGS("\n\n[Power] Energy counter:");

        ctl_power_energy_counter_t energyCounter = { 0 };
        energyCounter.Size                       = sizeof(ctl_power_energy_counter_t);
        res                                      = ctlPowerGetEnergyCounter(pPowerHandle[i], &energyCounter);

        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\n %s from Power get energy counter.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Power] Energy Counter [%llu] micro J", energyCounter.energy);
            PRINT_LOGS("\n[Power] Time Stamp [%llu] time stamp", energyCounter.timestamp);
        }

        PRINT_LOGS("\n\n[Power] Get Limits:");

        ctl_power_limits_t powerLimits = {};
        powerLimits.Size               = sizeof(ctl_power_limits_t);
        res                            = ctlPowerGetLimits(pPowerHandle[i], &powerLimits);

        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\n %s from Power get limits.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Power] Sustained Power Limit Enabled [%s]", DecodeBoolean(powerLimits.sustainedPowerLimit.enabled).c_str());
            PRINT_LOGS("\n[Power] Sustained Power (PL1) Value [%i] mW", powerLimits.sustainedPowerLimit.power);
            PRINT_LOGS("\n[Power] Sustained Power (PL1 Tau) Time Window [%i] ms", powerLimits.sustainedPowerLimit.interval);
            PRINT_LOGS("\n[Power] Burst Power Limit Enabled [%s]", DecodeBoolean(powerLimits.burstPowerLimit.enabled).c_str());
            PRINT_LOGS("\n[Power] Burst Power (PL2) Value [%i] mW", powerLimits.burstPowerLimit.power);
            PRINT_LOGS("\n[Power] Peak Power (PL4) AC Value [%i] mW", powerLimits.peakPowerLimits.powerAC);
            PRINT_LOGS("\n[Power] Peak Power (PL4) DC Value [%i] mW", powerLimits.peakPowerLimits.powerDC);
        }

        PRINT_LOGS("\n\n[Power] Set Limits:");

        res = ctlPowerSetLimits(pPowerHandle[i], &powerLimits);

        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\n %s from Power set limits.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n\n[Power] Set Limits Success!");
        }
    }

cleanUp:
    delete[] pPowerHandle;
    pPowerHandle = nullptr;
}

/***************************************************************
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlFanTest_FanGetConfig(ctl_fan_handle_t hFanHandle)
{
    PRINT_LOGS("\n[Fan] Fan get Config:");

    ctl_fan_config_t FanConfig = {};
    FanConfig.Size             = sizeof(ctl_fan_config_t);
    ctl_result_t res           = ctlFanGetConfig(hFanHandle, &FanConfig);

    if (res != CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\n %s from Fan get Config.", DecodeRetCode(res).c_str());
    }
    else
    {
        PRINT_LOGS("\n[Fan] Fan Config Mode [%s]", DecodeFanSpeedMode(FanConfig.mode).c_str());
        if (CTL_FAN_SPEED_MODE_DEFAULT == FanConfig.mode)
        {
            PRINT_LOGS("\n[Fan] Fan Config Mode: Default/Stock");
        }
        else if (CTL_FAN_SPEED_MODE_FIXED == FanConfig.mode)
        {
            PRINT_LOGS("\n[Fan] Fan Config Fixed Speed [%u]", FanConfig.speedFixed.speed);
            PRINT_LOGS("\n[Fan] Fan Config Fixed Speed Unit [%s]", DecodeFanSpeedUnits(FanConfig.speedFixed.units).c_str());
        }
        else if (CTL_FAN_SPEED_MODE_TABLE == FanConfig.mode)
        {
            PRINT_LOGS("\n[Fan] Fan Config Fan Table NumPoints [%u]", FanConfig.speedTable.numPoints);
            for (int32_t numPoints = 0; numPoints < FanConfig.speedTable.numPoints; numPoints++)
            {
                PRINT_LOGS("\n[Fan] Fan Config Fan Table Point Speed Unit [%s]", DecodeFanSpeedUnits(FanConfig.speedTable.table[numPoints].speed.units).c_str());
                PRINT_LOGS("\n[Fan] Fan Config Fan Table Point Speed [%u]", FanConfig.speedTable.table[numPoints].speed.speed);
                PRINT_LOGS("\n[Fan] Fan Config Fan Table Point Temperature [%u]", FanConfig.speedTable.table[numPoints].temperature);
            }
        }
    }
}

/***************************************************************
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlFanTest(ctl_device_adapter_handle_t hDAhandle)
{
    PRINT_LOGS("\n::::::::::::::Print Fan Properties::::::::::::::\n");

    uint32_t FanHandlerCount = 0;
    ctl_result_t res         = ctlEnumFans(hDAhandle, &FanHandlerCount, nullptr);
    if ((res != CTL_RESULT_SUCCESS) || FanHandlerCount == 0)
    {
        PRINT_LOGS("\nFan component not supported. Error: %s", DecodeRetCode(res).c_str());
        return;
    }
    else
    {
        PRINT_LOGS("\nNumber of Fan Handles [%u]", FanHandlerCount);
    }

    ctl_fan_handle_t *pFanHandle = new ctl_fan_handle_t[FanHandlerCount];

    res = ctlEnumFans(hDAhandle, &FanHandlerCount, pFanHandle);

    if (res != CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError: %s  for Fan handle.", DecodeRetCode(res).c_str());
        goto cleanUp;
    }

    for (uint32_t i = 0; i < FanHandlerCount; i++)
    {
        PRINT_LOGS("\n\nFor Fan Handle [%u]", i);

        PRINT_LOGS("\n[Fan] Fan get properties:");
        ctl_fan_properties_t Fan_properties = {};
        Fan_properties.Size                 = sizeof(ctl_fan_properties_t);
        res                                 = ctlFanGetProperties(pFanHandle[i], &Fan_properties);

        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\n %s from Fan get state.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Fan] Can Control [%s]", DecodeBoolean(Fan_properties.canControl).c_str());
            PRINT_LOGS("\n[Fan] Max Points [%i]", Fan_properties.maxPoints);
            PRINT_LOGS("\n[Fan] Max RPM [%i]", Fan_properties.maxRPM);
            PRINT_LOGS("\n[Fan] Supported Modes [%u]", Fan_properties.supportedModes);
            PRINT_LOGS("\n[Fan] Supported Units [%u]", Fan_properties.supportedUnits);
        }

        CtlFanTest_FanGetConfig(pFanHandle[i]);

        PRINT_LOGS("\n[Fan] Fan get state:");

        for (uint32_t index = 0; index < 10; index++)
        {
            ctl_fan_speed_units_t units = CTL_FAN_SPEED_UNITS_RPM;
            int32_t speed               = 0;
            res                         = ctlFanGetState(pFanHandle[i], units, &speed);

            if (res != CTL_RESULT_SUCCESS)
            {
                PRINT_LOGS("\n %s   from Fan get state.", DecodeRetCode(res).c_str());
            }
            else
            {
                PRINT_LOGS("\n[Fan] Speed [%i] RPM", speed);
            }

            Sleep(100);
        }

        ctl_fan_speed_table_t FanTable = {};
        FanTable.Size                  = sizeof(ctl_fan_speed_table_t);
        FanTable.numPoints             = 10;
        FanTable.Version               = 0;

        for (uint32_t index = 0; index < 10; index++)
        {
            FanTable.table[index]               = { 0 };
            FanTable.table[index].Version       = 0;
            FanTable.table[index].Size          = sizeof(ctl_fan_temp_speed_t);
            FanTable.table[index].temperature   = (index + 1) * 10;
            FanTable.table[index].speed.Size    = sizeof(ctl_fan_speed_t);
            FanTable.table[index].speed.Version = 0;
            FanTable.table[index].speed.units   = CTL_FAN_SPEED_UNITS_PERCENT;
            FanTable.table[index].speed.speed   = index * 10;
        }

        res = ctlFanSetSpeedTableMode(pFanHandle[i], &FanTable);
        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\n %s   from Fan Set Speed Table Mode.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Fan] Set Speed Table Mode Success");
        }

        CtlFanTest_FanGetConfig(pFanHandle[i]);

        res = ctlFanSetDefaultMode(pFanHandle[i]);
        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\n %s   from Fan Set DEFAULT Speed Table Mode.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Fan] Set DEFAULT Speed Table Mode Success");
        }

        CtlFanTest_FanGetConfig(pFanHandle[i]);
    }

cleanUp:
    delete[] pFanHandle;
    pFanHandle = nullptr;
}

/***************************************************************
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlPciTest(ctl_device_adapter_handle_t hDAhandle)
{

    PRINT_LOGS("\n[PCI] Get PCI properties:");
    ctl_pci_properties_t Pci_properties = { 0 };
    Pci_properties.Size                 = sizeof(ctl_pci_properties_t);
    ctl_result_t res                    = ctlPciGetProperties(hDAhandle, &Pci_properties);

    if (res != CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError: %s from PCI get properties.", DecodeRetCode(res).c_str());
    }
    else
    {
        PRINT_LOGS("\n[Pci] Domain [%u]", Pci_properties.address.domain);
        PRINT_LOGS("\n[Pci] Bus [%u]", Pci_properties.address.bus);
        PRINT_LOGS("\n[Pci] Device [%u]", Pci_properties.address.device);
        PRINT_LOGS("\n[Pci] Function [%u]", Pci_properties.address.function);
        PRINT_LOGS("\n[Pci] Gen [%i]", Pci_properties.maxSpeed.gen);
        PRINT_LOGS("\n[Pci] Width [%i]", Pci_properties.maxSpeed.width);
        PRINT_LOGS("\n[Pci] Max Bandwidth [%lli] bytes per second", Pci_properties.maxSpeed.maxBandwidth);
        PRINT_LOGS("\n[Pci] Resizable Bar Supported [%u]", Pci_properties.resizable_bar_supported);
        PRINT_LOGS("\n[Pci] Resizable Bar Enabled [%u]", Pci_properties.resizable_bar_enabled);
    }

    ctl_pci_state_t Pci_state = { 0 };
    Pci_state.Size            = sizeof(ctl_pci_state_t);
    res                       = ctlPciGetState(hDAhandle, &Pci_state);
    if (res != CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError: %s from PCI get state.", DecodeRetCode(res).c_str());
    }
    else
    {
        PRINT_LOGS("\n[Pci] Current Gen Speed [%i]", Pci_state.speed.gen);
        PRINT_LOGS("\n[Pci] Current Width [%i]", Pci_state.speed.width);
        PRINT_LOGS("\n[Pci] Current Max Bandwidth [%lli] bytes per second", Pci_state.speed.maxBandwidth);
    }

    PRINT_LOGS("\n \n");
}

/***************************************************************
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlMemoryTest(ctl_device_adapter_handle_t hDAhandle)
{

    uint32_t MemoryHandlerCount = 0;
    ctl_result_t res            = ctlEnumMemoryModules(hDAhandle, &MemoryHandlerCount, nullptr);
    if ((res != CTL_RESULT_SUCCESS) || MemoryHandlerCount == 0)
    {
        PRINT_LOGS("\nMemory component not supported. Error: %s", DecodeRetCode(res).c_str());
        return;
    }
    else
    {
        PRINT_LOGS("\nNumber of Memory Handles [%u]", MemoryHandlerCount);
    }

    ctl_mem_handle_t *pMemoryHandle = new ctl_mem_handle_t[MemoryHandlerCount];

    res = ctlEnumMemoryModules(hDAhandle, &MemoryHandlerCount, pMemoryHandle);

    if (res != CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError: %s for Memory handle.", DecodeRetCode(res).c_str());
        goto cleanUp;
    }

    for (uint32_t i = 0; i < MemoryHandlerCount; i++)
    {
        PRINT_LOGS("\n\nFor Memory Handle [%u]", i);

        PRINT_LOGS("\n[Memory] Get Memory properties:");

        ctl_mem_properties_t memoryProperties = { 0 };
        memoryProperties.Size                 = sizeof(ctl_mem_properties_t);
        res                                   = ctlMemoryGetProperties(pMemoryHandle[i], &memoryProperties);

        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\nError: %s from Memory get properties.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Memory] Bus Width [%i]", memoryProperties.busWidth);
            PRINT_LOGS("\n[Memory] Location [%u]", (uint32_t)memoryProperties.location);
            PRINT_LOGS("\n[Memory] Number of Channels [%i]", memoryProperties.numChannels);
            PRINT_LOGS("\n[Memory] Physical Size [%llu]", memoryProperties.physicalSize);
            PRINT_LOGS("\n[Memory] Memory Type [%u]", (uint32_t)memoryProperties.type);
        }

        PRINT_LOGS("\n[Memory] Get Memory State:");

        ctl_mem_state_t state = { 0 };
        state.Size            = sizeof(ctl_mem_state_t);
        res                   = ctlMemoryGetState(pMemoryHandle[i], &state);

        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\nError: %s from Memory get State.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Memory] Memory Size [%llu]", state.size);
            PRINT_LOGS("\n[Memory] Memory Free [%llu]", state.free);
        }

        PRINT_LOGS("\n\n[Memory] Get Memory Bandwidth Version 1:");

        ctl_mem_bandwidth_t bandwidth = { 0 };
        bandwidth.Version             = 1;
        bandwidth.Size                = sizeof(ctl_mem_bandwidth_t);
        res                           = ctlMemoryGetBandwidth(pMemoryHandle[i], &bandwidth);

        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\nError: %s from Memory get Bandwidth.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Memory] Read Counter (in bytes) [%llu]", bandwidth.readCounter);
            PRINT_LOGS("\n[Memory] Write Counter (in bytes) [%llu]", bandwidth.writeCounter);
            PRINT_LOGS("\n[Memory] Max Bandwidth [%llu]", bandwidth.maxBandwidth);
            PRINT_LOGS("\n[Memory] Time Stamp [%llu] \n \n", bandwidth.timestamp);
        }
    }

cleanUp:
    delete[] pMemoryHandle;
    pMemoryHandle = nullptr;
}

/***************************************************************
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlEngineTest(ctl_device_adapter_handle_t hDAhandle)
{
    PRINT_LOGS("\n::::::::::::::Print Engine Properties::::::::::::::\n");

    uint32_t EngineHandlerCount = 0;
    ctl_result_t res            = ctlEnumEngineGroups(hDAhandle, &EngineHandlerCount, nullptr);
    if ((res != CTL_RESULT_SUCCESS) || EngineHandlerCount == 0)
    {
        PRINT_LOGS("\nEngine component not supported. Error: %s", DecodeRetCode(res).c_str());
        return;
    }
    else
    {
        PRINT_LOGS("\nNumber of Engine Handles [%u]", EngineHandlerCount);
    }

    ctl_engine_handle_t *pEngineHandle = new ctl_engine_handle_t[EngineHandlerCount];

    res = ctlEnumEngineGroups(hDAhandle, &EngineHandlerCount, pEngineHandle);

    if (res != CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError: %s for Engine handle.", DecodeRetCode(res).c_str());
        goto cleanUp;
    }

    for (uint32_t i = 0; i < EngineHandlerCount; i++)
    {
        PRINT_LOGS("\n\nFor Engine Handle [%u]", i);
        ctl_engine_properties_t engineProperties = { 0 };
        engineProperties.Size                    = sizeof(ctl_engine_properties_t);
        res                                      = ctlEngineGetProperties(pEngineHandle[i], &engineProperties);
        if (res)
        {
            PRINT_LOGS("\nError: %s from Engine get properties.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Engine] Engine type [%s]", DecodeEngineGroup(engineProperties.type).c_str());
        }

        ctl_engine_stats_t engineStats = { 0 };
        engineStats.Size               = sizeof(ctl_engine_stats_t);
        int32_t iterations             = 3;
        uint64_t prevActiveCounter = 0, prevTimeStamp = 0;
        do
        {
            res = ctlEngineGetActivity(pEngineHandle[i], &engineStats);

            if (res != CTL_RESULT_SUCCESS)
            {
                PRINT_LOGS("\nError: %s  from Engine get activity.", DecodeRetCode(res).c_str());
            }
            else
            {
                uint64_t activeDiff = engineStats.activeTime - prevActiveCounter;
                uint64_t timeWindow = engineStats.timestamp - prevTimeStamp;

                double percentActivity = static_cast<double>(activeDiff) / static_cast<double>(timeWindow);
                percentActivity *= 100.0;

                PRINT_LOGS("\n[Engine] Active Time [%llu]\n", activeDiff);
                PRINT_LOGS("[Engine] Time Stamp [%llu]\n", timeWindow);
                PRINT_LOGS("[Engine] Usage [%f] \n \n \n", percentActivity);

                prevActiveCounter = engineStats.activeTime;
                prevTimeStamp     = engineStats.timestamp;
            }

            iterations--;
            Sleep(200);
        } while (iterations > 0);
    }

cleanUp:
    delete[] pEngineHandle;
    pEngineHandle = nullptr;
}

/***************************************************************
 * @brief Main Function
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlLedTest(ctl_device_adapter_handle_t hDAhandle)
{
    PRINT_LOGS("\n::::::::::::::Print Led Properties::::::::::::::\n");

    ctl_result_t Result          = CTL_RESULT_SUCCESS;
    uint32_t LedCount            = 0;
    ctl_led_handle_t *pLedHandle = nullptr;
    uint32_t Index               = 0;

    Result = ctlEnumLeds(hDAhandle, &LedCount, pLedHandle);
    if ((Result != CTL_RESULT_SUCCESS) || LedCount == 0)
    {
        PRINT_LOGS("\nLed component not supported. Result: %s, LedCount %d", DecodeRetCode(Result).c_str(), LedCount);
        return;
    }
    else
    {
        PRINT_LOGS("\nNumber of Led Handles [%u]", LedCount);
    }

    pLedHandle = (ctl_led_handle_t *)malloc(sizeof(ctl_led_handle_t) * LedCount);
    if (pLedHandle == nullptr)
    {
        PRINT_LOGS("\nNull pointer after malloc\n");
        return;
    }

    Result = ctlEnumLeds(hDAhandle, &LedCount, pLedHandle);
    if (Result != CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError: %s for Led handle.", DecodeRetCode(Result).c_str());
        goto cleanUp;
    }

    for (Index = 0; Index < LedCount; Index++)
    {
        if (NULL != pLedHandle[Index])
        {
            PRINT_LOGS("\n\n[Led] For pLedHandle[%u] = 0x%p\n", Index, pLedHandle[Index]);

            PRINT_LOGS("\n[Led] Get Led properties:");

            ctl_led_properties_t LedProperties = { 0 };
            LedProperties.Size                 = sizeof(ctl_led_properties_t);
            Result                             = ctlLedGetProperties(pLedHandle[Index], &LedProperties);

            if (Result != CTL_RESULT_SUCCESS)
            {
                PRINT_LOGS("\nError: %s from Led get properties.\n", DecodeRetCode(Result).c_str());
            }
            else
            {
                PRINT_LOGS("\n[Led] Can Control [%s]", DecodeBoolean(LedProperties.canControl).c_str());
                PRINT_LOGS("\n[Led] Is I2C [%s]", DecodeBoolean(LedProperties.isI2C).c_str());
                PRINT_LOGS("\n[Led] Is PWM [%s]", DecodeBoolean(LedProperties.isPWM).c_str());
                PRINT_LOGS("\n[Led] Have RGB [%s]", DecodeBoolean(LedProperties.haveRGB).c_str());
            }

            PRINT_LOGS("\n\n[Led] Get Led state:");

            ctl_led_state_t ledState = { 0 };
            ledState.Size            = sizeof(ctl_led_state_t);
            Result                   = ctlLedGetState(pLedHandle[Index], &ledState);

            if (Result != CTL_RESULT_SUCCESS)
            {
                PRINT_LOGS("\nError: %s  from Led get state.\n", DecodeRetCode(Result).c_str());
            }
            else
            {
                PRINT_LOGS("\n[Led] Get Led State successful");
                PRINT_LOGS("\n[Led] IsOn [%s]", DecodeBoolean(ledState.isOn).c_str());
                PRINT_LOGS("\n[Led] PWM: Led On/Off Ratio [%f]", ledState.pwm);
                PRINT_LOGS("\n[Led] Red color of Led [%f]", ledState.color.red);
                PRINT_LOGS("\n[Led] Green color of Led [%f]", ledState.color.green);
                PRINT_LOGS("\n[Led] Blue color of Led [%f]", ledState.color.blue);
            }

            PRINT_LOGS("\n\n[Led] Set Led state:");

            void *pBuffer       = &ledState;
            uint32_t bufferSize = sizeof(ctl_led_state_t);
            Result              = ctlLedSetState(pLedHandle[Index], pBuffer, bufferSize);

            if (Result != CTL_RESULT_SUCCESS)
            {
                PRINT_LOGS("\nError: %s from Led Set State\n", DecodeRetCode(Result).c_str());
            }
            else
            {
                PRINT_LOGS("\n[Led] Successfully Set Led State\n");
            }
        }
    }

cleanUp:
    if (pLedHandle != nullptr)
    {
        free(pLedHandle);
        pLedHandle = nullptr;
    }

    return;
}

/***************************************************************
 * @brief Main Function
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlEccTest(ctl_device_adapter_handle_t hDAhandle)
{
    PRINT_LOGS("\n::::::::::::::Print Ecc Properties::::::::::::::\n");

    // Get ECC Properites
    ctl_result_t Result                = CTL_RESULT_SUCCESS;
    ctl_ecc_properties_t EccProperties = { 0 };
    EccProperties.Size                 = sizeof(ctl_ecc_properties_t);
    Result                             = ctlEccGetProperties(hDAhandle, &EccProperties);

    PRINT_LOGS("\n[Ecc] Get Ecc properties:");
    if (Result != CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nEcc component not supported. Result: %s", DecodeRetCode(Result).c_str());
        return;
    }
    else
    {
        PRINT_LOGS("\n[Ecc] Is Supported [%s]", DecodeBoolean(EccProperties.isSupported).c_str());
        PRINT_LOGS("\n[Ecc] Can Control [%s]", DecodeBoolean(EccProperties.canControl).c_str());
    }

    if (EccProperties.isSupported)
    {
        // Get ECC State
        ctl_ecc_state_desc_t eccState = { 0 };
        eccState.Size                 = sizeof(ctl_ecc_state_t);
        Result                        = ctlEccGetState(hDAhandle, &eccState);
        PRINT_LOGS("\n\n[Ecc] Get Ecc state:");
        if (Result != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\nError: %s  from Ecc Get state.\n", DecodeRetCode(Result).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Ecc] Ecc Get State successful");
            PRINT_LOGS("\n[Ecc] Current Ecc State [%s]", DecodeEccState(eccState.currentEccState).c_str());
            PRINT_LOGS("\n[Ecc] Pending Ecc State [%s]", DecodeEccState(eccState.pendingEccState).c_str());
        }

        if (eccState.currentEccState != eccState.pendingEccState)
        {
            PRINT_LOGS("\n[Ecc] Pending Ecc State[%s] is not applied yet from previous Ecc Set Call. Please reboot the system for it to take affect.",
                       DecodeEccState(eccState.pendingEccState).c_str());
            return;
        }
        else
        {
            PRINT_LOGS("\n[Ecc] Pending Ecc State [%s] from previous Ecc Set Call applied and ECC state toggled successfully.", DecodeEccState(eccState.pendingEccState).c_str());
        }

        if (EccProperties.canControl)
        {
            // Disabling ECC State if current ECC State is enabled and there is nothing to apply (currentEccState == pendingEccState)
            if (eccState.currentEccState == CTL_ECC_STATE_ECC_ENABLED_STATE && eccState.currentEccState == eccState.pendingEccState)
            {
                eccState.currentEccState = CTL_ECC_STATE_ECC_DISABLED_STATE;
                Result                   = ctlEccSetState(hDAhandle, &eccState);
                PRINT_LOGS("\n\n[Ecc] Set Ecc state:");
                PRINT_LOGS("\n\n[Ecc] Disabling Ecc state [%s]", DecodeEccState(eccState.currentEccState).c_str());
                if (Result != CTL_RESULT_SUCCESS)
                {
                    PRINT_LOGS("\nError: %s from Ecc Set State\n", DecodeRetCode(Result).c_str());
                }
                else
                {
                    PRINT_LOGS("\n[Ecc] Ecc Set State call successful\n");
                    PRINT_LOGS("\n[Ecc] Pending Ecc State to be applied from Ecc Set State call [%s]", DecodeEccState(eccState.pendingEccState).c_str());
                }
            }
            // Enabling ECC State if current ECC State if disabled and there is nothing to apply (currentEccState == pendingEccState)
            else if (eccState.currentEccState == CTL_ECC_STATE_ECC_DISABLED_STATE && eccState.currentEccState == eccState.pendingEccState)
            {
                eccState.currentEccState = CTL_ECC_STATE_ECC_ENABLED_STATE;
                Result                   = ctlEccSetState(hDAhandle, &eccState);
                PRINT_LOGS("\n\n[Ecc] Set Ecc state:");
                PRINT_LOGS("\n\n[Ecc] Enabling Ecc state [%s]", DecodeEccState(eccState.currentEccState).c_str());
                if (Result != CTL_RESULT_SUCCESS)
                {
                    PRINT_LOGS("\nError: %s from Ecc Set State\n", DecodeRetCode(Result).c_str());
                }
                else
                {
                    PRINT_LOGS("\n[Ecc] Ecc Set State call successful\n");
                    PRINT_LOGS("\n[Ecc] Pending Ecc State to be applied from Ecc Set State call [%s]", DecodeEccState(eccState.pendingEccState).c_str());
                }
            }

            // Retrieving back ECC State after Ecc Set State call to determine if currentEccState == pendingEccState
            Result = ctlEccGetState(hDAhandle, &eccState);
            PRINT_LOGS("\n\n[Ecc] Get Ecc state again:");
            if (Result != CTL_RESULT_SUCCESS)
            {
                PRINT_LOGS("\nError: %s  from Ecc Get state.\n", DecodeRetCode(Result).c_str());
            }
            else
            {
                PRINT_LOGS("\n[Ecc] Ecc Get State successful");
                PRINT_LOGS("\n[Ecc] Current Ecc State [%s]", DecodeEccState(eccState.currentEccState).c_str());
                PRINT_LOGS("\n[Ecc] Pending Ecc State [%s]", DecodeEccState(eccState.pendingEccState).c_str());
            }

            // System reboot is needed always if currentEccState != pendingEccState for the pendingEccState to be applied
            if (eccState.currentEccState != eccState.pendingEccState)
            {
                PRINT_LOGS("\n[Ecc] Pending Ecc State [%s] is not applied yet after the Ecc Set State call. Please reboot the system for it to take affect.",
                           DecodeEccState(eccState.pendingEccState).c_str());
            }
        }
        else
        {
            PRINT_LOGS("\n[Ecc] Ecc state cannot be changed on this system.");
        }
    }
    else
    {
        PRINT_LOGS("\n[Ecc] ECC is not supported on this system.");
    }

    return;
}

void CtlPowerTelemetryTest(ctl_device_adapter_handle_t hDAhandle)
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
        PRINT_LOGS("\nCtlPowerTelemetryTest => ctlPowerTelemetryGet Result Error: %s, ErrorCode: 0x%x \n \n", DecodeRetCode(Status).c_str(), Status);
    }
}

void PerComponentTest(ctl_device_adapter_handle_t hDAhandle)
{
    try
    {
        CtlPowerTest(hDAhandle);
    }
    catch (const std::bad_array_new_length &e)
    {
        printf("%s \n", e.what());
    }
    try
    {
        CtlFrequencyTest(hDAhandle);
    }
    catch (const std::bad_array_new_length &e)
    {
        printf("%s \n", e.what());
    }
    try
    {
        CtlEngineTest(hDAhandle);
    }
    catch (const std::bad_array_new_length &e)
    {
        printf("%s \n", e.what());
    }
    try
    {
        CtlTemperatureTest(hDAhandle);
    }
    catch (const std::bad_array_new_length &e)
    {
        printf("%s \n", e.what());
    }
    try
    {
        CtlMemoryTest(hDAhandle);
    }
    catch (const std::bad_array_new_length &e)
    {
        printf("%s \n", e.what());
    }
    try
    {
        CtlPciTest(hDAhandle);
    }
    catch (const std::bad_array_new_length &e)
    {
        printf("%s \n", e.what());
    }
    try
    {
        CtlFanTest(hDAhandle);
    }
    catch (const std::bad_array_new_length &e)
    {
        printf("%s \n", e.what());
    }
    try
    {
        CtlLedTest(hDAhandle);
    }
    catch (const std::bad_array_new_length &e)
    {
        printf("%s \n", e.what());
    }
    try
    {
        CtlEccTest(hDAhandle);
    }
    catch (const std::bad_array_new_length &e)
    {
        printf("%s \n", e.what());
    }
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
                    CTL_FREE_MEM(StDeviceAdapterProperties.pDeviceID);
                    continue;
                }

                if (NULL != StDeviceAdapterProperties.pDeviceID)
                {
                    AdapterID = *(reinterpret_cast<LUID *>(StDeviceAdapterProperties.pDeviceID));
                    PRINT_LOGS("\nAdapter ID %lu \n", AdapterID.LowPart);
                }

                if (0x8086 == StDeviceAdapterProperties.pci_vendor_id)
                {
                    PRINT_LOGS("\nIntel Adapter Name %s", StDeviceAdapterProperties.name);
                    PRINT_LOGS("\nVendor id  0x%X", StDeviceAdapterProperties.pci_vendor_id);
                    PRINT_LOGS("\nDevice id 0x%X", StDeviceAdapterProperties.pci_device_id);
                    PRINT_LOGS("\nSubSys id 0x%X", StDeviceAdapterProperties.pci_subsys_id);
                    PRINT_LOGS("\nSubSys Vendor id 0x%X", StDeviceAdapterProperties.pci_subsys_vendor_id);
                    PRINT_LOGS("\nRev id 0x%X", StDeviceAdapterProperties.rev_id);
                }

                // Per Component Tests
                PerComponentTest(hDevices[Index]);

                // Telemetry Test
                // Polling during 1 second at 20 ms
                for (uint32_t i = 0; i < 50; i++)
                {
                    try
                    {
                        CtlPowerTelemetryTest(hDevices[Index]);
                    }
                    catch (const std::bad_array_new_length &e)
                    {
                        printf("%s \n", e.what());
                    }
                    Sleep(20);
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