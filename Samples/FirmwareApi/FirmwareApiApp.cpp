//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  FirmwareApiApp.cpp
 * @brief This file contains the I2C AUX Sample App & the 'main' function. Program execution begins and ends there.
 *
 */

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#define CTL_APIEXPORT // caller of control API DLL shall define this before
                      // including igcl_api.h
#include "igcl_api.h"
#include "GenericIGCLApp.h"

ctl_result_t IsDiscreteGfxAdapter(ctl_device_adapter_properties_t *pDeviceAdapterProperties)
{
    if (CTL_DEVICE_TYPE_GRAPHICS != pDeviceAdapterProperties->device_type)
    {
        printf("This is not a Graphics device \n");
        return CTL_RESULT_ERROR_NOT_INITIALIZED;
    }
    if (0 != pDeviceAdapterProperties->graphics_adapter_properties)
    {
        printf("This is not a Discrete Graphics adapter, its either integrated/LDA adapter \n");
        return CTL_RESULT_ERROR_INVALID_NULL_HANDLE;
    }

    return CTL_RESULT_SUCCESS;
}

int main()
{
    ctl_result_t Result                                        = CTL_RESULT_SUCCESS;
    uint32_t AdapterCount                                      = 0;
    ctl_device_adapter_handle_t *hDevices                      = nullptr;
    ctl_device_adapter_properties_t DeviceAdapterProperties[4] = { 0 };
    LUID DeviceID[4]                                           = { 0 };

    // Get a handle to the DLL module.
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

    if (CTL_RESULT_SUCCESS != Result)
    {
        goto Exit;
    }

    AdapterCount = 0;

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

    if (CTL_RESULT_SUCCESS == Result)
    {
        hDevices = (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * AdapterCount);
        if (nullptr == hDevices)
        {
            Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
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
        printf("ctlEnumerateDevices returned failure code: 0x%X\n", Result);
        goto Exit;
    }

    try
    {
        for (uint32_t i = 0; i < AdapterCount; i++)
        {
            DeviceAdapterProperties[i].Size           = sizeof(ctl_device_adapter_properties_t);
            DeviceAdapterProperties[i].pDeviceID      = &(DeviceID[i]);
            DeviceAdapterProperties[i].device_id_size = sizeof(LUID);
            if (NULL == DeviceAdapterProperties[i].pDeviceID)
            {
                Result = CTL_RESULT_ERROR_INVALID_NULL_POINTER;
                goto Exit;
            }

            Result = ctlGetDeviceProperties(hDevices[i], &DeviceAdapterProperties[i]);
            if (Result != CTL_RESULT_SUCCESS)
            {
                printf("ctlGetDeviceProperties returned failure code: 0x%X\n", Result);
                continue;
            }

            Result = IsDiscreteGfxAdapter(&DeviceAdapterProperties[i]);
            if (Result != CTL_RESULT_SUCCESS)
            {
                continue;
            }

            // Get PCI paramters to check link speed
            ctl_pci_properties_t PciProperties = { 0 };
            PciProperties.Size                 = sizeof(PciProperties);
            ctl_result_t res                   = ctlPciGetProperties(hDevices[i], &PciProperties);
            if (res != CTL_RESULT_SUCCESS)
            {
                printf("Error: %d from PCI get properties.", res);
                continue;
            }

            // Get PCI paramters to check current link speed
            ctl_pci_state_t PciState = { 0 };
            PciState.Size            = sizeof(PciState);

            res = ctlPciGetState(hDevices[i], &PciState);
            if (res != CTL_RESULT_SUCCESS)
            {
                printf("Error: %d from PCI get properties.", res);
                continue;
            }

            printf("PCI Max link speed: %d :: PCI Current link speed: %d \n", PciProperties.maxSpeed.gen, PciState.speed.gen);

            if (PciState.speed.gen == 5)
            {
                printf("Current PCI link speed is already Gen5, so nothing to do\n");
                continue;
            }

            // Check if current FW supports Gen5 to Gen4 downgrade capability
            ctl_firmware_properties_t BaseFwProperties = { 0 };
            BaseFwProperties.Size                      = sizeof(BaseFwProperties);

            res = ctlGetFirmwareProperties(hDevices[i], &BaseFwProperties);
            if (res == CTL_RESULT_SUCCESS)
            {
                if (BaseFwProperties.FirmwareConfig & CTL_FIRMWARE_CONFIG_FLAG_IS_DEVICE_LINK_SPEED_DOWNGRADE_CAPABLE)
                {
                    printf("FW supports Gen5 to Gen4 downgrade capability\n");
                }
                else
                {
                    printf("FW does not support Gen5 to Gen4 downgrade capability\n");
                    continue;
                }

                if (BaseFwProperties.FirmwareConfig & CTL_FIRMWARE_CONFIG_FLAG_IS_DEVICE_LINK_SPEED_DOWNGRADE_ACTIVE)
                {
                    printf("We downgraded from Gen5 to Gen4 successfully or we already attempted to go from Gen4 to Gen5 and it probably failed\n");
                    continue;
                }
            }
            else
            {
                printf("Error: %d from getfirmwareproperties.", res);
            }

            // Enumerate firmware components
            uint32_t Count                                        = 0;
            ctl_firmware_component_handle_t *phFirmwareComponents = nullptr;

            res = ctlEnumerateFirmwareComponents(hDevices[i], &Count, nullptr);
            if (res != CTL_RESULT_SUCCESS)
            {
                printf("Error: %d from enumeratefirmwarecomponents.", res);
                continue;
            }

            phFirmwareComponents = (ctl_firmware_component_handle_t *)malloc(sizeof(ctl_firmware_component_handle_t) * Count);
            if (phFirmwareComponents == nullptr)
            {
                printf("Error: %d from malloc.", res);
                continue;
            }
            res = ctlEnumerateFirmwareComponents(hDevices[i], &Count, phFirmwareComponents);
            if (res != CTL_RESULT_SUCCESS)
            {
                if (phFirmwareComponents != nullptr)
                    free(phFirmwareComponents);

                printf("Error: %d from enumeratefirmwarecomponents.", res);
                continue;
            }

            for (uint32_t j = 0; j < Count; j++)
            {
                ctl_firmware_component_properties_t FwComponentProperties = { 0 };
                FwComponentProperties.Size                                = sizeof(FwComponentProperties);

                res = ctlGetFirmwareComponentProperties(phFirmwareComponents[j], &FwComponentProperties);
                if (res != CTL_RESULT_SUCCESS)
                {
                    printf("Error: %d from getfirmwarecomponentproperties.", res);
                    continue;
                }

                printf("Fw componenent (%d) : Name: %s and Version : %s \n", j, FwComponentProperties.name, FwComponentProperties.version);
            }

            if (phFirmwareComponents != nullptr)
                free(phFirmwareComponents);
        }
    }
    catch (const std::bad_array_new_length &e)
    {
        printf("%s \n", e.what());
    }
Exit:

    if (hDevices != nullptr)
        free(hDevices);

    ctlClose(hAPIHandle);

    printf("Overrall test result is 0x%X\n", CTL_RESULT_SUCCESS);

    return CTL_RESULT_SUCCESS;
}
