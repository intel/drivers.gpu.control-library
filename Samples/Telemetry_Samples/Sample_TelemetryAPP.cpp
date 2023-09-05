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

#include <iostream>
#include <string>

#include <windows.h>
#include <stdio.h>
#include "igcl_api.h"
#include "GenericIGCLApp.h"

std::string DecodeRetCode(ctl_result_t Res);
void CtlTemperatureTest(ctl_device_adapter_handle_t hDAhandle);
void CtlFrequencyTest(ctl_device_adapter_handle_t hDAhandle);
void CtlPowerTest(ctl_device_adapter_handle_t hDAhandle);
void CtlFanTest(ctl_device_adapter_handle_t hDAhandle);
void CtlPciTest(ctl_device_adapter_handle_t hDAhandle);
void CtlMemoryTest(ctl_device_adapter_handle_t hDAhandle);
void CtlEngineTest(ctl_device_adapter_handle_t hDAhandle);
void CtlOverclockPropertiesTest(ctl_device_adapter_handle_t hDAhandle);
void CtlOverclockFrequencyOffsetTest(ctl_device_adapter_handle_t hDAhandle, double FreqOffset);
void CtlOverclockFrequencyAndVoltageOffsetTest(ctl_device_adapter_handle_t hDAhandle);
void CtlOverclockPowerTest(ctl_device_adapter_handle_t hDAhandle);
void CtlOverclockTemperatureTest(ctl_device_adapter_handle_t hDAhandle);
void CtlPowerTelemetryTest(ctl_device_adapter_handle_t hDAhandle);

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
            PRINT_LOGS("\n[Temperature] Sensor type [%s]", ((temperatureProperties.type == CTL_TEMP_SENSORS_GLOBAL) ? "Global" :
                                                            (temperatureProperties.type == CTL_TEMP_SENSORS_GPU)    ? "Gpu" :
                                                            (temperatureProperties.type == CTL_TEMP_SENSORS_MEMORY) ? "Memory" :
                                                                                                                      "Unknown"));
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
            PRINT_LOGS("\n[Frequency] Frequency Domain [%s]]", ((freqProperties.type == CTL_FREQ_DOMAIN_GPU) ? "GPU" : "MEMORY"));
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

            for (uint32_t i = 0; i < numClocks; i++)
            {
                PRINT_LOGS("\n[Frequency] Clock [%u]  Freq[%f] MHz", i, clocks[i]);
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
            PRINT_LOGS("\n[Power] Can Control [%u]", (uint32_t)properties.canControl);
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
            PRINT_LOGS("\n[Power] Sustained Power Limit Enabled [%u]", (uint32_t)powerLimits.sustainedPowerLimit.enabled);
            PRINT_LOGS("\n[Power] Sustained Power (PL1) Value [%i] mW", powerLimits.sustainedPowerLimit.power);
            PRINT_LOGS("\n[Power] Sustained Power (PL1 Tau) Time Window [%i] ms", powerLimits.sustainedPowerLimit.interval);
            PRINT_LOGS("\n[Power] Burst Power Limit Enabled [%u]", (uint32_t)powerLimits.burstPowerLimit.enabled);
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
            PRINT_LOGS("\n[Fan] Can Control [%u]", (uint32_t)Fan_properties.canControl);
            PRINT_LOGS("\n[Fan] Max Points [%i]", Fan_properties.maxPoints);
            PRINT_LOGS("\n[Fan] Max RPM [%i]", Fan_properties.maxRPM);
            PRINT_LOGS("\n[Fan] Supported Modes [%u]", Fan_properties.supportedModes);
            PRINT_LOGS("\n[Fan] Supported Units [%u]", Fan_properties.supportedUnits);
        }

        PRINT_LOGS("\n[Fan] Fan get Config:");

        ctl_fan_config_t FanConfig = {};
        FanConfig.Size             = sizeof(ctl_fan_config_t);
        res                        = ctlFanGetConfig(pFanHandle[i], &FanConfig);

        if (res != CTL_RESULT_SUCCESS)
        {
            PRINT_LOGS("\n %s from Fan get Config.", DecodeRetCode(res).c_str());
        }
        else
        {
            PRINT_LOGS("\n[Fan] Fan Config Mode [%u]", FanConfig.mode);
            if (CTL_FAN_SPEED_MODE_FIXED == FanConfig.mode)
            {
                PRINT_LOGS("\n[Fan] Fan Config Fixed Speed [%u]", FanConfig.speedFixed.speed);
                PRINT_LOGS("\n[Fan] Fan Config Fixed Speed Unit [%u]", FanConfig.speedFixed.units);
            }
            else if (CTL_FAN_SPEED_MODE_TABLE == FanConfig.mode)
            {
                PRINT_LOGS("\n[Fan] Fan Config Fan Table NumPoints [%u]", FanConfig.speedTable.numPoints);
                for (int32_t numPoints = 0; numPoints < FanConfig.speedTable.numPoints; numPoints++)
                {
                    PRINT_LOGS("\n[Fan] Fan Config Fan Table Point Speed Unit [%u]", FanConfig.speedTable.table[numPoints].speed.units);
                    PRINT_LOGS("\n[Fan] Fan Config Fan Table Point Speed [%u]", FanConfig.speedTable.table[numPoints].speed.speed);
                    PRINT_LOGS("\n[Fan] Fan Config Fan Table Point Temperature [%u]", FanConfig.speedTable.table[numPoints].temperature);
                }
            }
        }

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
            PRINT_LOGS("\n[Engine] Engine type [%s]", ((engineProperties.type == CTL_ENGINE_GROUP_GT)     ? "Gt" :
                                                       (engineProperties.type == CTL_ENGINE_GROUP_RENDER) ? "Render" :
                                                       (engineProperties.type == CTL_ENGINE_GROUP_MEDIA)  ? "Media" :
                                                                                                            "Unknown"));
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
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlOverclockPropertiesTest(ctl_device_adapter_handle_t hDAhandle)
{

    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    ctl_result_t status              = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {

        PRINT_LOGS("\nOc Supported? %s", ((OcProperties.bSupported) ? "true" : "false"));

        PRINT_LOGS("\nGpu Frequency Offset Supported? %s", ((OcProperties.gpuFrequencyOffset.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nGpu Frequency Offset Is Relative? %s", ((OcProperties.gpuFrequencyOffset.bRelative) ? "true" : "false"));
        PRINT_LOGS("\nGpu Frequency Offset Have Reference? %s", ((OcProperties.gpuFrequencyOffset.bReference) ? "true" : "false"));
        PRINT_LOGS("\nGpu Frequency Offset Default: %f", OcProperties.gpuFrequencyOffset.Default);
        PRINT_LOGS("\nGpu Frequency Offset Min: %f", OcProperties.gpuFrequencyOffset.min);
        PRINT_LOGS("\nGpu Frequency Offset Max: %f", OcProperties.gpuFrequencyOffset.max);
        PRINT_LOGS("\nGpu Frequency Offset Reference: %f", OcProperties.gpuFrequencyOffset.reference);
        PRINT_LOGS("\nGpu Frequency Offset Step: %f", OcProperties.gpuFrequencyOffset.step);
        PRINT_LOGS("\nGpu Frequency Offset Units: %s", printUnits(OcProperties.gpuFrequencyOffset.units));

        PRINT_LOGS("\nGpu Voltage Offset Supported? %s", ((OcProperties.gpuVoltageOffset.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nGpu Voltage Offset Is Relative? %s", ((OcProperties.gpuVoltageOffset.bRelative) ? "true" : "false"));
        PRINT_LOGS("\nGpu Voltage Offset Have Reference? %s", ((OcProperties.gpuVoltageOffset.bReference) ? "true" : "false"));
        PRINT_LOGS("\nGpu Voltage Offset Default: %f", OcProperties.gpuVoltageOffset.Default);
        PRINT_LOGS("\nGpu Voltage Offset Min: %f", OcProperties.gpuVoltageOffset.min);
        PRINT_LOGS("\nGpu Voltage Offset Max: %f", OcProperties.gpuVoltageOffset.max);
        PRINT_LOGS("\nGpu Voltage Offset Reference: %f", OcProperties.gpuVoltageOffset.reference);
        PRINT_LOGS("\nGpu Voltage Offset Step: %f", OcProperties.gpuVoltageOffset.step);
        PRINT_LOGS("\nGpu Voltage Offset Units: %s", printUnits(OcProperties.gpuVoltageOffset.units));

        PRINT_LOGS("\nPower Limit Supported? %s", ((OcProperties.powerLimit.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nPower Limit Is Relative? %s", ((OcProperties.powerLimit.bRelative) ? "true" : "false"));
        PRINT_LOGS("\nPower Limit Have Reference? %s", ((OcProperties.powerLimit.bReference) ? "true" : "false"));
        PRINT_LOGS("\nPower Limit Default: %f", OcProperties.powerLimit.Default);
        PRINT_LOGS("\nPower Limit Min: %f", OcProperties.powerLimit.min);
        PRINT_LOGS("\nPower Limit Max: %f", OcProperties.powerLimit.max);
        PRINT_LOGS("\nPower Limit Reference: %f", OcProperties.powerLimit.reference);
        PRINT_LOGS("\nPower Limit Step: %f", OcProperties.powerLimit.step);
        PRINT_LOGS("\nPower Limit Units: %s", printUnits(OcProperties.powerLimit.units));

        PRINT_LOGS("\nTemperature Limit Supported? %s", ((OcProperties.temperatureLimit.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nTemperature Limit Is Relative? %s", ((OcProperties.temperatureLimit.bRelative) ? "true" : "false"));
        PRINT_LOGS("\nTemperature Limit Have Reference? %s", ((OcProperties.temperatureLimit.bReference) ? "true" : "false"));
        PRINT_LOGS("\nTemperature Limit Default: %f", OcProperties.temperatureLimit.Default);
        PRINT_LOGS("\nTemperature Limit Min: %f", OcProperties.temperatureLimit.min);
        PRINT_LOGS("\nTemperature Limit Max: %f", OcProperties.temperatureLimit.max);
        PRINT_LOGS("\nTemperature Limit Reference: %f", OcProperties.temperatureLimit.reference);
        PRINT_LOGS("\nTemperature Limit Step: %f", OcProperties.temperatureLimit.step);
        PRINT_LOGS("\nTemperature Limit Units: %s", printUnits(OcProperties.temperatureLimit.units));
    }
    else
    {
        PRINT_LOGS("\nError: %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\n \n");
}

/***************************************************************
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlOverclockFrequencyOffsetTest(ctl_device_adapter_handle_t hDAhandle, double FreqOffset)
{
    double GPUFrequencyOffset = 0.0;
    double VrelOffset         = 0.0;
    PRINT_LOGS("\n::::::::::::::Overclocking Tests::::::::::::::\n");
    PRINT_LOGS("\n::::::::::::::1.0 Frequency Offset::::::::::::::\n");
    PRINT_LOGS("\n::::::::::::::1.1 Get Frequency Offset::::::::::::::\n");
    PRINT_LOGS("\n1.1.1 Get Frequency Offset:::::::::::::::\n");
    ctl_result_t status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        if (GPUFrequencyOffset == 0.0)
        {
            PRINT_LOGS("\nResult: Overclocking disabled, Frequency Offset is 0");
        }
        else
        {
            PRINT_LOGS("\nResult: Overclocking enabled, Frequency Offset is:%f", GPUFrequencyOffset);
        }
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    GPUFrequencyOffset = FreqOffset; //  Setting Offset to FreqOffset MHz

    PRINT_LOGS("\nSet Frequency Offset with: %f MHz", GPUFrequencyOffset);
    status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_ERROR_CORE_OVERCLOCK_WAIVER_NOT_SET)
    {
        PRINT_LOGS("\nResult: Correctly returned: %s", DecodeRetCode(status).c_str());
    }
    else
    {
        PRINT_LOGS("\nResult: Incorrectly returned: %s", DecodeRetCode(status).c_str());
    }

    status = ctlOverclockWaiverSet(hDAhandle);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Waiver Called correctly");
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\nSet Frequency Offset with: %f MHz", GPUFrequencyOffset);
    status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Frequency Offset Set Correctly ");
    }
    else
    {
        PRINT_LOGS("\nResult: Incorrectly returned: %s", DecodeRetCode(status).c_str());
    }

    status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Overclocking enabled, Frequency Offset is:%f", GPUFrequencyOffset);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    status = ctlOverclockGpuVoltageOffsetGet(hDAhandle, &VrelOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Overclocking enabled, Voltage Offset is:%f", VrelOffset);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    GPUFrequencyOffset = 0.0; //  Setting Offset to 0 MHz
    VrelOffset         = 0.0;

    status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Frequency Offset successfully Set to 0");
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    status = ctlOverclockGpuVoltageOffsetSet(hDAhandle, VrelOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Voltage Offset successfully Set to 0");
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }
    status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Overclocking disabled, Frequency Offset is:%f", GPUFrequencyOffset);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    status = ctlOverclockGpuVoltageOffsetGet(hDAhandle, &VrelOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Overclocking disabled, Voltage Offset is: %f", VrelOffset);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    GPUFrequencyOffset = -FreqOffset; //  Setting Offset to -FreqOffset MHz
    status             = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Overclocking enabled, Frequency Offset is:%f", GPUFrequencyOffset);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Overclocking enabled, Frequency Offset is: %f", GPUFrequencyOffset);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    GPUFrequencyOffset = 0.0; //  Setting Offset to 0 MHz

    status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Offset successfully Set to 0");
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Overclocking disabled, Frequency Offset is:%f", GPUFrequencyOffset);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\n \n");
}

/***************************************************************
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlOverclockFrequencyAndVoltageOffsetTest(ctl_device_adapter_handle_t hDAhandle)
{
    double GPUFrequencyOffset = 0.0;
    double VrelOffset         = 0.0;

    GPUFrequencyOffset = 100.0;
    VrelOffset         = 50; // in mV

    PRINT_LOGS("\nSet Frequency Offset with: %f  MHz Vrel Offset: %f", GPUFrequencyOffset, VrelOffset);

    ctl_result_t status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Frequency Offset Set Correctly ");
    }
    else
    {
        PRINT_LOGS("\nResult: Incorrectly returned: %s", DecodeRetCode(status).c_str());
    }

    status = ctlOverclockGpuVoltageOffsetSet(hDAhandle, VrelOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Vrel Offset Set Correctly ");
    }
    else
    {
        PRINT_LOGS("\nResult: Incorrectly returned: %s", DecodeRetCode(status).c_str());
    }
    PRINT_LOGS("\n2.1.4 Get Modified Frequency and Vrel Offset:::::::::::::::\n");
    if (ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset) == ctl_result_t::CTL_RESULT_SUCCESS &&
        ctlOverclockGpuVoltageOffsetGet(hDAhandle, &VrelOffset) == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Overclocking enabled, Frequency Offset is: %f Vrel Offset is: %f", GPUFrequencyOffset, VrelOffset);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\n2.1.5 Set Frequency Offset to 0 MHz and Vrel Offset to 0 v to disable OC:::::::::::::::\n");
    GPUFrequencyOffset = 0.0; //  Setting Offset to 0 MHz
    VrelOffset         = 0.0;

    status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Frequency Offset successfully Set to 0");
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    status = ctlOverclockGpuVoltageOffsetSet(hDAhandle, VrelOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Voltage Offset successfully Set to 0");
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\n2.1.6 Reading Back Frequency Offset to confirm OC is disabled:::::::::::::::\n");
    status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Overclocking disabled, Frequency Offset is:%f", GPUFrequencyOffset);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    status = ctlOverclockGpuVoltageOffsetGet(hDAhandle, &VrelOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nResult: Overclocking disabled, Voltage Offset is: %f", VrelOffset);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\n \n");
}

/***************************************************************
 * @brief Main Function
 *
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlOverclockPowerTest(ctl_device_adapter_handle_t hDAhandle)
{
    PRINT_LOGS("\n::::::::::::::Print Power Limit::::::::::::::\n");

    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    ctl_result_t status              = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError Getting Properties%s", DecodeRetCode(status).c_str());
        return;
    }

    PRINT_LOGS("\nGetting current Power Limit\n");
    double CurrentPowerLimit = 0.0;

    status = ctlOverclockPowerLimitGet(hDAhandle, &CurrentPowerLimit);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nCurrent Sustained Power Limit: %f", CurrentPowerLimit);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\n\nSetting current Power Limit outside limits:");

    // Convert to mW and get min -1 W to be out of bounds
    CurrentPowerLimit = OcProperties.powerLimit.min * 1000.0 - 1000.0;

    status = ctlOverclockPowerLimitSet(hDAhandle, CurrentPowerLimit);
    if (status == ctl_result_t::CTL_RESULT_ERROR_CORE_OVERCLOCK_POWER_OUTSIDE_RANGE)
    {
        PRINT_LOGS("\nCorrectly returned out of bounds.");
    }

    PRINT_LOGS("\n\nSetting current Power Limit inside limits:\n");
    // Convert to mW and get min + 1 W to be in bounds.
    CurrentPowerLimit = OcProperties.powerLimit.min * 1000.0 + 1000.0;

    status = ctlOverclockPowerLimitSet(hDAhandle, CurrentPowerLimit);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nPower Limit set correctly.");
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\n\nGetting the now current Power Limit:\n");

    double NewCurrentPowerLimit = 0.0;

    status = ctlOverclockPowerLimitGet(hDAhandle, &NewCurrentPowerLimit);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nCurrent Sustained Power Limit: %f", NewCurrentPowerLimit);
        PRINT_LOGS("\nRequested Sustained Power Limit: %f", CurrentPowerLimit);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\n \n");
}

/***************************************************************
 * @brief Main Function
 *
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlOverclockTemperatureTest(ctl_device_adapter_handle_t hDAhandle)
{
    PRINT_LOGS("\n::::::::::::::Print Temperature Limit::::::::::::::\n");

    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    ctl_result_t status              = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (status != ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nError Getting Properties %s", DecodeRetCode(status).c_str());
        return;
    }

    PRINT_LOGS("\nGetting current Temperature Limit  \n");
    double CurrentTemperatureLimit = 0.0;

    status = ctlOverclockTemperatureLimitGet(hDAhandle, &CurrentTemperatureLimit);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nCurrent Temperature Limit: %f", CurrentTemperatureLimit);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\n\nSetting current Temperature Limit outside limits: \n");

    CurrentTemperatureLimit = OcProperties.temperatureLimit.min - 1.0;

    status = ctlOverclockTemperatureLimitSet(hDAhandle, CurrentTemperatureLimit);
    if (status == ctl_result_t::CTL_RESULT_ERROR_CORE_OVERCLOCK_TEMPERATURE_OUTSIDE_RANGE)
    {
        PRINT_LOGS("\nCorrectly returned out of bounds.");
    }

    PRINT_LOGS("\n\nSetting current Temperature Limit inside limits: \n");

    CurrentTemperatureLimit = OcProperties.temperatureLimit.min + 1.0;

    status = ctlOverclockTemperatureLimitSet(hDAhandle, CurrentTemperatureLimit);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nTemperature Limit set correctly.");
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\n\nGetting the now current Temperature Limit: \n");

    double NewCurrentTemperatureLimit = 0.0;

    status = ctlOverclockTemperatureLimitGet(hDAhandle, &NewCurrentTemperatureLimit);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nCurrent Temperature Limit: %f", NewCurrentTemperatureLimit);
        PRINT_LOGS("\nRequested Temperature Limit: %f", CurrentTemperatureLimit);
    }
    else
    {
        PRINT_LOGS("\nResult: Error %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\n \n");
}

/***************************************************************
 * @brief Main Function
 *
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlPowerTelemetryTest(ctl_device_adapter_handle_t hDAhandle)
{
    PRINT_LOGS("\n:: :: :: :: :: :: ::Print Telemetry:: :: :: :: :: :: ::\n");

    ctl_power_telemetry_t pPowerTelemetry = {};
    pPowerTelemetry.Size                  = sizeof(ctl_power_telemetry_t);
    ctl_result_t status                   = ctlPowerTelemetryGet(hDAhandle, &pPowerTelemetry);

    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        PRINT_LOGS("\nTelemetry Success \n");

        PRINT_LOGS("\nTimeStamp:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.timeStamp.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.timeStamp.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.timeStamp.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.timeStamp.value.datadouble);

        PRINT_LOGS("\nGpu Energy Counter:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.gpuEnergyCounter.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.gpuEnergyCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.gpuEnergyCounter.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.gpuEnergyCounter.value.datadouble);

        PRINT_LOGS("\nGpu Voltage:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.gpuVoltage.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.gpuVoltage.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.gpuVoltage.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.gpuVoltage.value.datadouble);

        PRINT_LOGS("\nGpu Current Frequency:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.gpuCurrentClockFrequency.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.gpuCurrentClockFrequency.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.gpuCurrentClockFrequency.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.gpuCurrentClockFrequency.value.datadouble);

        PRINT_LOGS("\nGpu Current Temperature:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.gpuCurrentTemperature.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.gpuCurrentTemperature.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.gpuCurrentTemperature.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.gpuCurrentTemperature.value.datadouble);

        PRINT_LOGS("\nGpu Activity Counter:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.globalActivityCounter.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.globalActivityCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.globalActivityCounter.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.globalActivityCounter.value.datadouble);

        PRINT_LOGS("\nRender Activity Counter:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.renderComputeActivityCounter.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.renderComputeActivityCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.renderComputeActivityCounter.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.renderComputeActivityCounter.value.datadouble);

        PRINT_LOGS("\nMedia Activity Counter:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.mediaActivityCounter.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.mediaActivityCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.mediaActivityCounter.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.mediaActivityCounter.value.datadouble);

        PRINT_LOGS("\nVRAM Energy Counter:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.vramEnergyCounter.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramEnergyCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramEnergyCounter.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.vramEnergyCounter.value.datadouble);

        PRINT_LOGS("\nVRAM Voltage:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.vramVoltage.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramVoltage.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramVoltage.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.vramVoltage.value.datadouble);

        PRINT_LOGS("\nVRAM Frequency:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.vramCurrentClockFrequency.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramCurrentClockFrequency.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramCurrentClockFrequency.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.vramCurrentClockFrequency.value.datadouble);

        PRINT_LOGS("\nVRAM Effective Frequency:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.vramCurrentEffectiveFrequency.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramCurrentEffectiveFrequency.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramCurrentEffectiveFrequency.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.vramCurrentEffectiveFrequency.value.datadouble);

        PRINT_LOGS("\nVRAM Read Bandwidth:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.vramReadBandwidthCounter.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramReadBandwidthCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramReadBandwidthCounter.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.vramReadBandwidthCounter.value.datadouble);

        PRINT_LOGS("\nVRAM Write Bandwidth:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.vramWriteBandwidthCounter.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramWriteBandwidthCounter.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramWriteBandwidthCounter.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.vramWriteBandwidthCounter.value.datadouble);

        PRINT_LOGS("\nVRAM Temperature:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.vramCurrentTemperature.bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.vramCurrentTemperature.units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.vramCurrentTemperature.type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.vramCurrentTemperature.value.datadouble);

        PRINT_LOGS("\nFan Speed:");
        PRINT_LOGS("\nSupported: %s", ((pPowerTelemetry.fanSpeed[0].bSupported) ? "true" : "false"));
        PRINT_LOGS("\nUnits: %s", printUnits(pPowerTelemetry.fanSpeed[0].units));
        PRINT_LOGS("\nType: %s", printType(pPowerTelemetry.fanSpeed[0].type));
        PRINT_LOGS("\nValue: %f", pPowerTelemetry.fanSpeed[0].value.datadouble);
    }
    else
    {
        PRINT_LOGS("\nError: %s", DecodeRetCode(status).c_str());
    }

    PRINT_LOGS("\n \n");
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

                    if (NULL != StDeviceAdapterProperties.pDeviceID)
                    {
                        free(StDeviceAdapterProperties.pDeviceID);
                    }
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
                CtlPowerTest(hDevices[Index]);
                CtlFrequencyTest(hDevices[Index]);
                CtlEngineTest(hDevices[Index]);
                CtlTemperatureTest(hDevices[Index]);
                CtlMemoryTest(hDevices[Index]);
                CtlPciTest(hDevices[Index]);
                CtlFanTest(hDevices[Index]);

                // Overclocking Test
                CtlOverclockPropertiesTest(hDevices[Index]);

                std::vector<double> vFrequencies;
                vFrequencies.push_back(10.0);
                vFrequencies.push_back(30.0);
                vFrequencies.push_back(50.0);
                vFrequencies.push_back(75.0);
                vFrequencies.push_back(100.0);
                vFrequencies.push_back(150.0);
                vFrequencies.push_back(200.0);
                vFrequencies.push_back(250.0);

                for (uint32_t ci = 0; ci < vFrequencies.size(); ci++)
                {
                    CtlOverclockFrequencyOffsetTest(hDevices[Index], vFrequencies[ci]);
                }

                CtlOverclockFrequencyAndVoltageOffsetTest(hDevices[Index]);

                CtlOverclockPowerTest(hDevices[Index]);
                CtlOverclockTemperatureTest(hDevices[Index]);

                // Telemetry Test
                // Polling during 1 second at 20 ms
                for (uint32_t i = 0; i < 50; i++)
                {
                    CtlPowerTelemetryTest(hDevices[Index]);
                    Sleep(20);
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
