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
    std::cout << "\n::::::::::::::Print Temperature Properties::::::::::::::\n" << std::endl;

    uint32_t TemperatureHandlerCount = 0;
    ctl_result_t res                 = ctlEnumTemperatureSensors(hDAhandle, &TemperatureHandlerCount, nullptr);
    if ((res != CTL_RESULT_SUCCESS) || TemperatureHandlerCount == 0)
    {
        std::cout << "Temperature component not supported. Error: " << DecodeRetCode(res) << std::endl;
        return;
    }
    else
    {
        std::cout << "Number of Temperature Handles [" << TemperatureHandlerCount << "]" << std::endl;
    }

    ctl_temp_handle_t *pTtemperatureHandle = new ctl_temp_handle_t[TemperatureHandlerCount];

    res = ctlEnumTemperatureSensors(hDAhandle, &TemperatureHandlerCount, pTtemperatureHandle);

    if (res != CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(res) << " for Temperature handle." << std::endl;
        goto cleanUp;
    }

    for (uint32_t i = 0; i < TemperatureHandlerCount; i++)
    {
        std::cout << "\nFor Temperature Handle [" << i << "]" << std::endl;

        std::cout << "[Temperature] Get Temperature properties:" << std::endl;

        ctl_temp_properties_t temperatureProperties = { 0 };
        temperatureProperties.Size                  = sizeof(ctl_temp_properties_t);
        res                                         = ctlTemperatureGetProperties(pTtemperatureHandle[i], &temperatureProperties);

        if (res != CTL_RESULT_SUCCESS)
        {
            std::cout << "Error: " << DecodeRetCode(res) << " from Temperature get properties." << std::endl;
        }
        else
        {
            std::cout << "[Temperature] Max temp [" << (uint32_t)temperatureProperties.maxTemperature << "]" << std::endl
                      << "[Temperature] Sensor type ["
                      << ((temperatureProperties.type == CTL_TEMP_SENSORS_GLOBAL) ? "Global" :
                          (temperatureProperties.type == CTL_TEMP_SENSORS_GPU)    ? "Gpu" :
                          (temperatureProperties.type == CTL_TEMP_SENSORS_MEMORY) ? "Memory" :
                                                                                    "Unknown")
                      << "]" << std::endl;
        }

        std::cout << "[Temperature] Get Temperature state:" << std::endl;

        double temperature = 0;
        res                = ctlTemperatureGetState(pTtemperatureHandle[i], &temperature);

        if (res != CTL_RESULT_SUCCESS)
        {
            std::cout << "Error: " << DecodeRetCode(res) << " from Temperature get state." << std::endl;
        }
        else
        {
            std::cout << "[Temperature] Current Temperature [" << temperature << "] C degrees" << std::endl;
        }
    }

    std::cout << std::endl << std::endl;

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
    std::cout << "\n::::::::::::::Print Frequency Properties::::::::::::::\n" << std::endl;

    uint32_t FrequencyHandlerCount = 0;
    ctl_result_t res               = ctlEnumFrequencyDomains(hDAhandle, &FrequencyHandlerCount, nullptr);
    if ((res != CTL_RESULT_SUCCESS) || FrequencyHandlerCount == 0)
    {
        std::cout << "Temperature component not supported. Error: " << DecodeRetCode(res) << std::endl;
        return;
    }
    else
    {
        std::cout << "Number of Frequency Handles [" << FrequencyHandlerCount << "]" << std::endl;
    }

    ctl_freq_handle_t *pFrequencyHandle = new ctl_freq_handle_t[FrequencyHandlerCount];

    res = ctlEnumFrequencyDomains(hDAhandle, &FrequencyHandlerCount, pFrequencyHandle);

    if (res != CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(res) << " for Frequency handle." << std::endl;
        goto cleanUp;
    }

    for (uint32_t i = 0; i < FrequencyHandlerCount; i++)
    {
        std::cout << "\nFor Frequency Handle [" << i << "] :" << std::endl;

        std::cout << "\n[Frequency] Properties:" << std::endl;

        ctl_freq_properties_t freqProperties = { 0 };
        freqProperties.Size                  = sizeof(ctl_freq_properties_t);
        res                                  = ctlFrequencyGetProperties(pFrequencyHandle[i], &freqProperties);
        if (res)
        {
            std::cout << DecodeRetCode(res) << " from Frequency get properties." << std::endl;
        }
        else
        {
            std::cout << "[Frequency] Min [" << freqProperties.min << "] Mhz" << std::endl;
            std::cout << "[Frequency] Max [" << freqProperties.max << "] Mhz" << std::endl;
            std::cout << "[Frequency] Can Control Frequency? [" << (uint32_t)freqProperties.canControl << "]" << std::endl;
            std::cout << "[Frequency] Frequency Domain [" << ((freqProperties.type == CTL_FREQ_DOMAIN_GPU) ? "GPU" : "MEMORY") << "]" << std::endl;
        }

        std::cout << "\n[Frequency] State:" << std::endl;

        ctl_freq_state_t freqState = { 0 };
        freqState.Size             = sizeof(ctl_freq_state_t);
        res                        = ctlFrequencyGetState(pFrequencyHandle[i], &freqState);
        if (res)
        {
            std::cout << DecodeRetCode(res) << " from Frequency get state." << std::endl;
        }
        else
        {
            std::cout << "[Frequency] Actual Frequency [" << freqState.actual << "] Mhz" << std::endl;
            std::cout << "[Frequency] Efficient Frequency [" << freqState.efficient << "] Mhz" << std::endl;
            std::cout << "[Frequency] Requested Frequency [" << freqState.request << "] Mhz" << std::endl;
            std::cout << "[Frequency] Actual TDP [" << freqState.tdp << "] Watts" << std::endl;
            std::cout << "[Frequency] Throttle Reasons [" << freqState.throttleReasons << "]" << std::endl;
            std::cout << "[Frequency] Voltage [" << freqState.currentVoltage << "] Volts" << std::endl;
        }

        std::cout << "\n[Frequency] Get throttle time:" << std::endl;

        ctl_freq_throttle_time_t throttleTime = { 0 };
        throttleTime.Size                     = sizeof(ctl_freq_throttle_time_t);
        res                                   = ctlFrequencyGetThrottleTime(pFrequencyHandle[i], &throttleTime);
        if (res)
        {
            std::cout << DecodeRetCode(res) << " from Frequency get throttle time." << std::endl;
        }
        else
        {
            std::cout << "[Frequency] Throttle Time [" << throttleTime.throttleTime << "] s" << std::endl;
            std::cout << "[Frequency] Timestamp [" << throttleTime.timestamp << "] s" << std::endl;
        }

        std::cout << "\n[Frequency] Available clocks:" << std::endl;

        uint32_t numClocks = 0;
        double *clocks     = nullptr;
        res                = ctlFrequencyGetAvailableClocks(pFrequencyHandle[i], &numClocks, 0);

        if (res || numClocks == 0)
        {
            std::cout << DecodeRetCode(res) << " from Frequency get available clocks." << std::endl;
        }
        else
        {
            std::cout << "[Frequency] Number of Available clocks [" << numClocks << "]" << std::endl;

            clocks = new double[numClocks];

            res = ctlFrequencyGetAvailableClocks(pFrequencyHandle[i], &numClocks, clocks);

            for (uint32_t i = 0; i < numClocks; i++)
            {
                std::cout << "[Frequency] Clock [" << i << "] Freq[" << clocks[i] << "] Mhz" << std::endl;
            }

            delete[] clocks;
            clocks = nullptr;
        }

        std::cout << "\n[Frequency] Frequency range:" << std::endl;

        ctl_freq_range_t freqRange = { 0 };
        freqRange.Size             = sizeof(ctl_freq_range_t);
        res                        = ctlFrequencyGetRange(pFrequencyHandle[i], &freqRange);

        if (res)
        {
            std::cout << DecodeRetCode(res) << " from Frequency get range." << std::endl;
        }
        else
        {
            std::cout << "[Frequency] Range Max [" << freqRange.max << "] Mhz" << std::endl;
            std::cout << "[Frequency] Range Min [" << freqRange.min << "] Mhz" << std::endl;
        }

        std::cout << "\n[Frequency] Set Frequency range:" << std::endl;

        res = ctlFrequencySetRange(pFrequencyHandle[i], &freqRange);

        if (res)
        {
            std::cout << DecodeRetCode(res) << " from Frequency set range." << std::endl;
        }
        else
        {

            std::cout << "\n[Frequency] Success for Set Range" << std::endl;
        }
    }

cleanUp:
    delete[] pFrequencyHandle;
    pFrequencyHandle = nullptr;
}

void CtlPowerTest(ctl_device_adapter_handle_t hDAhandle)
{
    std::cout << "\n::::::::::::::Print Power Properties::::::::::::::\n" << std::endl;

    uint32_t PowerHandlerCount = 0;
    ctl_result_t res           = ctlEnumPowerDomains(hDAhandle, &PowerHandlerCount, nullptr);
    if ((res != CTL_RESULT_SUCCESS) || PowerHandlerCount == 0)
    {
        std::cout << "Power component not supported. Error: " << DecodeRetCode(res) << std::endl;
        return;
    }
    else
    {
        std::cout << "Number of Power Handles [" << PowerHandlerCount << "]" << std::endl;
    }

    ctl_pwr_handle_t *pPowerHandle = new ctl_pwr_handle_t[PowerHandlerCount];

    res = ctlEnumPowerDomains(hDAhandle, &PowerHandlerCount, pPowerHandle);

    if (res != CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(res) << " for Power handle." << std::endl;
        goto cleanUp;
    }

    for (uint32_t i = 0; i < PowerHandlerCount; i++)
    {
        std::cout << "\nFor Power Handle [" << i << "]." << std::endl;

        ctl_power_properties_t properties = { 0 };
        properties.Size                   = sizeof(ctl_power_properties_t);
        res                               = ctlPowerGetProperties(pPowerHandle[i], &properties);

        std::cout << "\n[Power] Properties:" << std::endl;

        if (res != CTL_RESULT_SUCCESS)
        {
            std::cout << DecodeRetCode(res) << " from Power get properties." << std::endl;
        }
        else
        {
            std::cout << "[Power] Can Control [" << (uint32_t)properties.canControl << "]" << std::endl;
            std::cout << "[Power] Max Power Limit [" << properties.maxLimit << "] mW" << std::endl;
            std::cout << "[Power] Min Power Limit [" << properties.minLimit << "] mW" << std::endl;
        }

        std::cout << "\n[Power] Energy counter:" << std::endl;

        ctl_power_energy_counter_t energyCounter = { 0 };
        energyCounter.Size                       = sizeof(ctl_power_energy_counter_t);
        res                                      = ctlPowerGetEnergyCounter(pPowerHandle[i], &energyCounter);

        if (res != CTL_RESULT_SUCCESS)
        {
            std::cout << DecodeRetCode(res) << " from Power get energy counter." << std::endl;
        }
        else
        {
            std::cout << "[Power] Energy Counter [" << energyCounter.energy << "] micro J" << std::endl;
            std::cout << "[Power] Time Stamp [" << energyCounter.timestamp << "] time stamp" << std::endl;
        }

        std::cout << "\n[Power] Get Limits:" << std::endl;

        ctl_power_limits_t powerLimits = {};
        powerLimits.Size               = sizeof(ctl_power_limits_t);
        res                            = ctlPowerGetLimits(pPowerHandle[i], &powerLimits);

        if (res != CTL_RESULT_SUCCESS)
        {
            std::cout << DecodeRetCode(res) << " from Power get limits." << std::endl;
        }
        else
        {
            std::cout << "[Power] Sustained Power Limit Enabled [" << (uint32_t)powerLimits.sustainedPowerLimit.enabled << "]" << std::endl
                      << "[Power] Sustained Power (PL1) Value [" << powerLimits.sustainedPowerLimit.power << "] mW" << std::endl
                      << "[Power] Sustained Power (PL1 Tau) Time Window [" << powerLimits.sustainedPowerLimit.interval << "] ms" << std::endl
                      << "[Power] Burst Power Limit Enabled [" << (uint32_t)powerLimits.burstPowerLimit.enabled << "]" << std::endl
                      << "[Power] Burst Power (PL2) Value [" << powerLimits.burstPowerLimit.power << "] mW" << std::endl
                      << "[Power] Peak Power (PL4) AC Value [" << powerLimits.peakPowerLimits.powerAC << "] mW" << std::endl
                      << "[Power] Peak Power (PL4) DC Value [" << powerLimits.peakPowerLimits.powerDC << "] mW" << std::endl;
        }

        std::cout << "\n[Power] Set Limits:" << std::endl;

        res = ctlPowerSetLimits(pPowerHandle[i], &powerLimits);

        if (res != CTL_RESULT_SUCCESS)
        {
            std::cout << DecodeRetCode(res) << " from Power set limits." << std::endl;
        }
        else
        {
            std::cout << "\n[Power] Set Limits Success!" << std::endl;
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
    std::cout << "\n::::::::::::::Print Fan Properties::::::::::::::\n" << std::endl;

    uint32_t FanHandlerCount = 0;
    ctl_result_t res         = ctlEnumFans(hDAhandle, &FanHandlerCount, nullptr);
    if ((res != CTL_RESULT_SUCCESS) || FanHandlerCount == 0)
    {
        std::cout << "Fan component not supported. Error: " << DecodeRetCode(res) << std::endl;
        return;
    }
    else
    {
        std::cout << "Number of Fan Handles [" << FanHandlerCount << "]" << std::endl;
    }

    ctl_fan_handle_t *pFanHandle = new ctl_fan_handle_t[FanHandlerCount];

    res = ctlEnumFans(hDAhandle, &FanHandlerCount, pFanHandle);

    if (res != CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(res) << " for Fan handle." << std::endl;
        goto cleanUp;
    }

    for (uint32_t i = 0; i < FanHandlerCount; i++)
    {
        std::cout << "\nFor Fan Handle [" << i << "]" << std::endl;

        std::cout << "[Fan] Fan get properties:" << std::endl;
        ctl_fan_properties_t Fan_properties = {};
        Fan_properties.Size                 = sizeof(ctl_fan_properties_t);
        res                                 = ctlFanGetProperties(pFanHandle[i], &Fan_properties);

        if (res != CTL_RESULT_SUCCESS)
        {
            std::cout << DecodeRetCode(res) << " from Fan get state." << std::endl;
        }
        else
        {
            std::cout << "[Fan] Can Control [" << (uint32_t)Fan_properties.canControl << "]" << std::endl;
            std::cout << "[Fan] Max Points [" << (uint32_t)Fan_properties.maxPoints << "]" << std::endl;
            std::cout << "[Fan] Max RPM [" << (uint32_t)Fan_properties.maxRPM << "]" << std::endl;
            std::cout << "[Fan] Supported Modes [" << (uint32_t)Fan_properties.supportedModes << "]" << std::endl;
            std::cout << "[Fan] Supported Units [" << (uint32_t)Fan_properties.supportedUnits << "]" << std::endl;
        }

        std::cout << "[Fan] Fan get state:" << std::endl;

        for (uint32_t index = 0; index < 10; index++)
        {
            ctl_fan_speed_units_t units = CTL_FAN_SPEED_UNITS_RPM;
            int32_t speed               = 0;
            res                         = ctlFanGetState(pFanHandle[i], units, &speed);

            if (res != CTL_RESULT_SUCCESS)
            {
                std::cout << DecodeRetCode(res) << " from Fan get state." << std::endl;
            }
            else
            {
                std::cout << "[Fan] Speed [" << speed << "] RPM" << std::endl;
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
    std::cout << "\n::::::::::::::Print PCI Properties::::::::::::::\n" << std::endl;

    std::cout << "[Temperature] Get Temperature properties:" << std::endl;
    ctl_pci_properties_t Pci_properties = { 0 };
    Pci_properties.Size                 = sizeof(ctl_pci_properties_t);
    ctl_result_t res                    = ctlPciGetProperties(hDAhandle, &Pci_properties);

    if (res != CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(res) << " from PCI get properties." << std::endl;
    }
    else
    {
        std::cout << "[Pci] Domain [" << Pci_properties.address.domain << "]" << std::endl;
        std::cout << "[Pci] Bus [" << Pci_properties.address.bus << "]" << std::endl;
        std::cout << "[Pci] Device [" << Pci_properties.address.device << "]" << std::endl;
        std::cout << "[Pci] Function [" << Pci_properties.address.function << "]" << std::endl;
        std::cout << "[Pci] Gen [" << Pci_properties.maxSpeed.gen << "]" << std::endl;
        std::cout << "[Pci] Width [" << Pci_properties.maxSpeed.width << "]" << std::endl;
        std::cout << "[Pci] Max Bandwidth [" << Pci_properties.maxSpeed.maxBandwidth << "] bytes per second" << std::endl;
    }

    ctl_pci_state_t Pci_state = { 0 };
    Pci_state.Size            = sizeof(ctl_pci_state_t);
    res                       = ctlPciGetState(hDAhandle, &Pci_state);
    if (res != CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(res) << " from PCI get state." << std::endl;
    }
    else
    {
        std::cout << "[Pci] Current Gen Speed [" << Pci_state.speed.gen << "]" << std::endl;
        std::cout << "[Pci] Current Width [" << Pci_state.speed.width << "]" << std::endl;
        std::cout << "[Pci] Current Max Bandwidth [" << Pci_state.speed.maxBandwidth << "] bytes per second" << std::endl;
    }

    std::cout << std::endl << std::endl;
}

/***************************************************************
 * @brief
 * place_holder_for_Detailed_desc
 * @param
 * @return
 ***************************************************************/
void CtlMemoryTest(ctl_device_adapter_handle_t hDAhandle)
{
    std::cout << "\n::::::::::::::Print Memory Properties::::::::::::::\n" << std::endl;

    uint32_t MemoryHandlerCount = 0;
    ctl_result_t res            = ctlEnumMemoryModules(hDAhandle, &MemoryHandlerCount, nullptr);
    if ((res != CTL_RESULT_SUCCESS) || MemoryHandlerCount == 0)
    {
        std::cout << "Memory component not supported. Error: " << DecodeRetCode(res) << std::endl;
        return;
    }
    else
    {
        std::cout << "Number of Memory Handles [" << MemoryHandlerCount << "]" << std::endl;
    }

    ctl_mem_handle_t *pMemoryHandle = new ctl_mem_handle_t[MemoryHandlerCount];

    res = ctlEnumMemoryModules(hDAhandle, &MemoryHandlerCount, pMemoryHandle);

    if (res != CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(res) << " for Memory handle." << std::endl;
        goto cleanUp;
    }

    for (uint32_t i = 0; i < MemoryHandlerCount; i++)
    {
        std::cout << "\nFor Memory Handle [" << i << "]" << std::endl;

        std::cout << "[Memory] Get Memory properties:" << std::endl;

        ctl_mem_properties_t memoryProperties = { 0 };
        memoryProperties.Size                 = sizeof(ctl_mem_properties_t);
        res                                   = ctlMemoryGetProperties(pMemoryHandle[i], &memoryProperties);

        if (res != CTL_RESULT_SUCCESS)
        {
            std::cout << "Error: " << DecodeRetCode(res) << " from Memory get properties." << std::endl;
        }
        else
        {
            std::cout << "[Memory] Bus Width [" << memoryProperties.busWidth << "]" << std::endl
                      << "[Memory] Location [" << (uint32_t)memoryProperties.location << "]" << std::endl
                      << "[Memory] Number of Channels [" << memoryProperties.numChannels << "]" << std::endl
                      << "[Memory] Physical Size [" << memoryProperties.physicalSize << "]" << std::endl
                      << "[Memory] Memory Type [" << memoryProperties.type << "]" << std::endl;
        }

        std::cout << "[Memory] Get Memory State:" << std::endl;

        ctl_mem_state_t state = { 0 };
        state.Size            = sizeof(ctl_mem_state_t);
        res                   = ctlMemoryGetState(pMemoryHandle[i], &state);

        if (res != CTL_RESULT_SUCCESS)
        {
            std::cout << "Error: " << DecodeRetCode(res) << " from Memory get State." << std::endl;
        }
        else
        {
            std::cout << "[Memory] Memory Size [" << state.size << "]" << std::endl;
            std::cout << "[Memory] Memory Free [" << state.free << "]" << std::endl;
        }

        std::cout << "[Memory] Get Memory Bandwidth:" << std::endl;

        ctl_mem_bandwidth_t bandwidth = { 0 };
        bandwidth.Size                = sizeof(ctl_mem_bandwidth_t);
        res                           = ctlMemoryGetBandwidth(pMemoryHandle[i], &bandwidth);

        if (res != CTL_RESULT_SUCCESS)
        {
            std::cout << "Error: " << DecodeRetCode(res) << " from Memory get Bandwidth." << std::endl;
        }
        else
        {
            std::cout << "[Memory] Max Bandwidth [" << bandwidth.maxBandwidth << "]" << std::endl;
            std::cout << "[Memory] Time Stamp [" << bandwidth.timestamp << "]" << std::endl;
        }
    }

    std::cout << std::endl << std::endl;

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
    std::cout << "\n::::::::::::::Print Engine Properties::::::::::::::\n" << std::endl;

    uint32_t EngineHandlerCount = 0;
    ctl_result_t res            = ctlEnumEngineGroups(hDAhandle, &EngineHandlerCount, nullptr);
    if ((res != CTL_RESULT_SUCCESS) || EngineHandlerCount == 0)
    {
        std::cout << "Engine component not supported. Error: " << DecodeRetCode(res) << std::endl;
        return;
    }
    else
    {
        std::cout << "Number of Engine Handles [" << EngineHandlerCount << "]" << std::endl;
    }

    ctl_engine_handle_t *pEngineHandle = new ctl_engine_handle_t[EngineHandlerCount];

    res = ctlEnumEngineGroups(hDAhandle, &EngineHandlerCount, pEngineHandle);

    if (res != CTL_RESULT_SUCCESS)
    {
        std::cout << "Error: " << DecodeRetCode(res) << " for Engine handle." << std::endl;
        goto cleanUp;
    }

    for (uint32_t i = 0; i < EngineHandlerCount; i++)
    {
        std::cout << "\nFor Engine Handle [" << i << "]" << std::endl;
        ctl_engine_properties_t engineProperties = { 0 };
        engineProperties.Size                    = sizeof(ctl_engine_properties_t);
        res                                      = ctlEngineGetProperties(pEngineHandle[i], &engineProperties);
        if (res)
        {
            std::cout << "Error: " << DecodeRetCode(res) << " from Engine get properties." << std::endl;
        }
        else
        {
            std::cout << "[Engine] Engine type ["
                      << ((engineProperties.type == CTL_ENGINE_GROUP_GT)     ? "Gt" :
                          (engineProperties.type == CTL_ENGINE_GROUP_RENDER) ? "Render" :
                          (engineProperties.type == CTL_ENGINE_GROUP_MEDIA)  ? "Media" :
                                                                               "Unknown")
                      << "]" << std::endl;
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
                std::cout << "Error: " << DecodeRetCode(res) << " from Engine get activity." << std::endl;
            }
            else
            {
                uint64_t activeDiff = engineStats.activeTime - prevActiveCounter;
                uint64_t timeWindow = engineStats.timestamp - prevTimeStamp;

                double percentActivity = static_cast<double>(activeDiff) / static_cast<double>(timeWindow);
                percentActivity *= 100.0;

                std::cout << "[Engine] Active Time [" << activeDiff << "] time stamp" << std::endl
                          << "[Engine] Time Stamp [" << timeWindow << "] time stamp" << std::endl
                          << "[Engine] Usage [" << percentActivity << "] %" << std::endl;

                prevActiveCounter = engineStats.activeTime;
                prevTimeStamp     = engineStats.timestamp;
            }

            iterations--;
            Sleep(200);
        } while (iterations > 0);
    }

    std::cout << std::endl << std::endl;

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
    std::cout << "\n::::::::::::::Print Overclock Properties::::::::::::::\n" << std::endl;

    ctl_oc_properties_t OcProperties = {};
    OcProperties.Size                = sizeof(ctl_oc_properties_t);
    ctl_result_t status              = ctlOverclockGetProperties(hDAhandle, &OcProperties);

    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {

        std::cout << "Oc Supported? " << ((OcProperties.bSupported) ? "true" : "false") << std::endl << std::endl;

        std::cout << "Gpu Frequency Offset Supported? " << ((OcProperties.gpuFrequencyOffset.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Gpu Frequency Offset Is Relative? " << ((OcProperties.gpuFrequencyOffset.bRelative) ? "true" : "false") << std::endl;
        std::cout << "Gpu Frequency Offset Have Reference? " << ((OcProperties.gpuFrequencyOffset.bReference) ? "true" : "false") << std::endl;
        std::cout << "Gpu Frequency Offset Default: " << OcProperties.gpuFrequencyOffset.Default << std::endl;
        std::cout << "Gpu Frequency Offset Min: " << OcProperties.gpuFrequencyOffset.min << std::endl;
        std::cout << "Gpu Frequency Offset Max: " << OcProperties.gpuFrequencyOffset.max << std::endl;
        std::cout << "Gpu Frequency Offset Reference: " << OcProperties.gpuFrequencyOffset.reference << std::endl;
        std::cout << "Gpu Frequency Offset Step: " << OcProperties.gpuFrequencyOffset.step << std::endl;
        std::cout << "Gpu Frequency Offset Units: " << printUnits(OcProperties.gpuFrequencyOffset.units) << std::endl << std::endl;

        std::cout << "Gpu Voltage Offset Supported? " << ((OcProperties.gpuVoltageOffset.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Gpu Voltage Offset Is Relative? " << ((OcProperties.gpuVoltageOffset.bRelative) ? "true" : "false") << std::endl;
        std::cout << "Gpu Voltage Offset Have Reference? " << ((OcProperties.gpuVoltageOffset.bReference) ? "true" : "false") << std::endl;
        std::cout << "Gpu Voltage Offset Default: " << OcProperties.gpuVoltageOffset.Default << std::endl;
        std::cout << "Gpu Voltage Offset Min: " << OcProperties.gpuVoltageOffset.min << std::endl;
        std::cout << "Gpu Voltage Offset Max: " << OcProperties.gpuVoltageOffset.max << std::endl;
        std::cout << "Gpu Voltage Offset Reference: " << OcProperties.gpuVoltageOffset.reference << std::endl;
        std::cout << "Gpu Voltage Offset Step: " << OcProperties.gpuVoltageOffset.step << std::endl;
        std::cout << "Gpu Voltage Offset Units: " << printUnits(OcProperties.gpuVoltageOffset.units) << std::endl << std::endl;

        std::cout << "Power Limit Supported? " << ((OcProperties.powerLimit.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Power Limit Is Relative? " << ((OcProperties.powerLimit.bRelative) ? "true" : "false") << std::endl;
        std::cout << "Power Limit Have Reference? " << ((OcProperties.powerLimit.bReference) ? "true" : "false") << std::endl;
        std::cout << "Power Limit Default: " << OcProperties.powerLimit.Default << std::endl;
        std::cout << "Power Limit Min: " << OcProperties.powerLimit.min << std::endl;
        std::cout << "Power Limit Max: " << OcProperties.powerLimit.max << std::endl;
        std::cout << "Power Limit Reference: " << OcProperties.powerLimit.reference << std::endl;
        std::cout << "Power Limit Step: " << OcProperties.powerLimit.step << std::endl;
        std::cout << "Power Limit Units: " << printUnits(OcProperties.powerLimit.units) << std::endl << std::endl;

        std::cout << "Temperature Limit Supported? " << ((OcProperties.temperatureLimit.bSupported) ? "true" : "false") << std::endl;
        std::cout << "Temperature Limit Is Relative? " << ((OcProperties.temperatureLimit.bRelative) ? "true" : "false") << std::endl;
        std::cout << "Temperature Limit Have Reference? " << ((OcProperties.temperatureLimit.bReference) ? "true" : "false") << std::endl;
        std::cout << "Temperature Limit Default: " << OcProperties.temperatureLimit.Default << std::endl;
        std::cout << "Temperature Limit Min: " << OcProperties.temperatureLimit.min << std::endl;
        std::cout << "Temperature Limit Max: " << OcProperties.temperatureLimit.max << std::endl;
        std::cout << "Temperature Limit Reference: " << OcProperties.temperatureLimit.reference << std::endl;
        std::cout << "Temperature Limit Step: " << OcProperties.temperatureLimit.step << std::endl;
        std::cout << "Temperature Limit Units: " << printUnits(OcProperties.temperatureLimit.units) << std::endl << std::endl;
    }
    else
    {
        std::cout << "Error: " << DecodeRetCode(status) << std::endl;
    }

    std::cout << std::endl << std::endl;
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
    std::cout << "\n::::::::::::::Overclocking Tests::::::::::::::\n" << std::endl;
    std::cout << "\n::::::::::::::1.0 Frequency Offset::::::::::::::\n" << std::endl;
    std::cout << "\n::::::::::::::1.1 Get Frequency Offset::::::::::::::\n" << std::endl;
    std::cout << "\n1.1.1 Get Frequency Offset:::::::::::::::\n" << std::endl;
    ctl_result_t status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        if (GPUFrequencyOffset == 0.0)
        {
            std::cout << "Result: Overclocking disabled, Frequency Offset is 0" << std::endl;
        }
        else
        {
            std::cout << "Result: Overclocking enabled, Frequency Offset is:" << GPUFrequencyOffset << std::endl;
        }
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\n1.1.2 Set Frequency Offset without calling waiver::::::::::::::\n" << std::endl;

    GPUFrequencyOffset = FreqOffset; //  Setting Offset to FreqOffset MHz

    std::cout << "Set Frequency Offset with: " << GPUFrequencyOffset << " MHz" << std::endl;
    status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_ERROR_CORE_OVERCLOCK_WAIVER_NOT_SET)
    {
        std::cout << "Result: Correctly returned: " << DecodeRetCode(status) << std::endl;
    }
    else
    {
        std::cout << "Result: Incorrectly returned: " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\n1.1.3 Calling waiver::::::::::::::\n" << std::endl;
    status = ctlOverclockWaiverSet(hDAhandle);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Waiver Called correctly" << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\n1.2.0 Set Frequency Offset with waiver::::::::::::::\n" << std::endl;
    std::cout << "Set Frequency Offset with: " << GPUFrequencyOffset << " MHz" << std::endl;
    status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Frequency Offset Set Correctly " << std::endl;
    }
    else
    {
        std::cout << "Result: Incorrectly returned: " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\n1.2.1 Get Modified Frequency Offset:::::::::::::::\n" << std::endl;
    status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Overclocking enabled, Frequency Offset is:" << GPUFrequencyOffset << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    status = ctlOverclockGpuVoltageOffsetGet(hDAhandle, &VrelOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Overclocking enabled, Voltage Offset is:" << VrelOffset << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }
    std::cout << "\n1.2.2 Set Frequency Offset to 0 MHz and Vrel Offset to 0 v to disable OC:::::::::::::::\n" << std::endl;

    GPUFrequencyOffset = 0.0; //  Setting Offset to 0 MHz
    VrelOffset         = 0.0;

    status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Frequency Offset successfully Set to 0" << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    status = ctlOverclockGpuVoltageOffsetSet(hDAhandle, VrelOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Voltage Offset successfully Set to 0" << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\n1.2.3 Reading Back Frequency Offset to confirm OC is disabled:::::::::::::::\n" << std::endl;
    status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Overclocking disabled, Frequency Offset is:" << GPUFrequencyOffset << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    status = ctlOverclockGpuVoltageOffsetGet(hDAhandle, &VrelOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Overclocking disabled, Voltage Offset is:" << VrelOffset << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\n1.3.0 Set Frequency Offset to -100 MHz to test negative offset OC:::::::::::::::\n" << std::endl;

    GPUFrequencyOffset = -FreqOffset; //  Setting Offset to -FreqOffset MHz
    status             = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Overclocking enabled, Frequency Offset is:" << GPUFrequencyOffset << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\n1.3.1 Get Modified Frequency Offset:::::::::::::::\n" << std::endl;

    status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Overclocking enabled, Frequency Offset is: " << GPUFrequencyOffset << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\n1.3.2 Set Frequency Offset to 0 MHz to disable OC:::::::::::::::\n" << std::endl;

    GPUFrequencyOffset = 0.0; //  Setting Offset to 0 MHz

    status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Offset successfully Set to 0" << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\n1.3.3 Reading Back Frequency Offset to confirm OC is disabled:::::::::::::::\n" << std::endl;

    status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Overclocking disabled, Frequency Offset is:" << GPUFrequencyOffset << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << std::endl << std::endl;
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
    std::cout << "\n::::::::::::::Overclocking Tests::::::::::::::\n" << std::endl;
    std::cout << "\n::::::::::::::2.0 Frequency Offset::::::::::::::\n" << std::endl;
    std::cout << "\n::::::::::::::2.1 Get Frequency Offset::::::::::::::\n" << std::endl;
    std::cout << "\n2.1.2 Get Frequency Offset:::::::::::::::\n" << std::endl;

    std::cout << "\n2.1.3 Set Frequency Offset and Vrel Offset::::::::::::::\n" << std::endl;

    GPUFrequencyOffset = 100.0;
    VrelOffset         = 0.05;

    std::cout << "Set Frequency Offset with: " << GPUFrequencyOffset << " MHz Vrel Offset: " << VrelOffset << std::endl;

    ctl_result_t status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Frequency Offset Set Correctly " << std::endl;
    }
    else
    {
        std::cout << "Result: Incorrectly returned: " << DecodeRetCode(status) << std::endl;
    }

    status = ctlOverclockGpuVoltageOffsetSet(hDAhandle, VrelOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Vrel Offset Set Correctly " << std::endl;
    }
    else
    {
        std::cout << "Result: Incorrectly returned: " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\n2.1.4 Get Modified Frequency and Vrel Offset:::::::::::::::\n" << std::endl;
    if (ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset) == ctl_result_t::CTL_RESULT_SUCCESS &&
        ctlOverclockGpuVoltageOffsetGet(hDAhandle, &VrelOffset) == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Overclocking enabled, Frequency Offset is: " << GPUFrequencyOffset << " Vrel Offset is: " << VrelOffset << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\n2.1.5 Set Frequency Offset to 0 MHz and Vrel Offset to 0 v to disable OC:::::::::::::::\n" << std::endl;

    GPUFrequencyOffset = 0.0; //  Setting Offset to 0 MHz
    VrelOffset         = 0.0;

    status = ctlOverclockGpuFrequencyOffsetSet(hDAhandle, GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Frequency Offset successfully Set to 0" << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    status = ctlOverclockGpuVoltageOffsetSet(hDAhandle, VrelOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Voltage Offset successfully Set to 0" << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << "\n2.1.6 Reading Back Frequency Offset to confirm OC is disabled:::::::::::::::\n" << std::endl;

    status = ctlOverclockGpuFrequencyOffsetGet(hDAhandle, &GPUFrequencyOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Overclocking disabled, Frequency Offset is:" << GPUFrequencyOffset << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    status = ctlOverclockGpuVoltageOffsetGet(hDAhandle, &VrelOffset);
    if (status == ctl_result_t::CTL_RESULT_SUCCESS)
    {
        std::cout << "Result: Overclocking disabled, Voltage Offset is:" << VrelOffset << std::endl;
    }
    else
    {
        std::cout << "Result: Error " << DecodeRetCode(status) << std::endl;
    }

    std::cout << std::endl << std::endl;
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
    std::cout << "\n::::::::::::::Print Power Limit::::::::::::::\n" << std::endl;

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

    std::cout << "\nSetting current Power Limit outside limits:" << std::endl << std::endl;

    // Convert to mW and get min -1 W to be out of bounds
    CurrentPowerLimit = OcProperties.powerLimit.min * 1000.0 - 1000.0;

    status = ctlOverclockPowerLimitSet(hDAhandle, CurrentPowerLimit);
    if (status == ctl_result_t::CTL_RESULT_ERROR_CORE_OVERCLOCK_POWER_OUTSIDE_RANGE)
    {
        std::cout << "Correctly returned out of bounds." << std::endl;
    }

    std::cout << "\nSetting current Power Limit inside limits:" << std::endl << std::endl;
    // Convert to mW and get min + 1 W to be in bounds.
    CurrentPowerLimit = OcProperties.powerLimit.min * 1000.0 + 1000.0;

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

    std::cout << std::endl << std::endl;
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
    std::cout << "\n::::::::::::::Print Temperature Limit::::::::::::::\n" << std::endl;

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

    std::cout << "\nSetting current Temperature Limit outside limits:" << std::endl << std::endl;

    CurrentTemperatureLimit = OcProperties.temperatureLimit.min - 1.0;

    status = ctlOverclockTemperatureLimitSet(hDAhandle, CurrentTemperatureLimit);
    if (status == ctl_result_t::CTL_RESULT_ERROR_CORE_OVERCLOCK_TEMPERATURE_OUTSIDE_RANGE)
    {
        std::cout << "Correctly returned out of bounds." << std::endl;
    }

    std::cout << "\nSetting current Temperature Limit inside limits:" << std::endl << std::endl;

    CurrentTemperatureLimit = OcProperties.temperatureLimit.min + 1.0;

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

    std::cout << std::endl << std::endl;
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