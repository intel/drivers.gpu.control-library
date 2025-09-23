# Intel Graphics Control Library (IGCL)
Header, wrapper library and samples of IGCL version 1.0
 

# Introduction
IGCL is meant to be a collection of high level APIs for all control aspects of hardware, primarily graphics. This is replacement of legacy Intel CUISDK which used to be released only to OEM's and selected customers. IGCL allows global control and tweaking of display, media & 3D capabilities.

# Notes
* IGCL binaries are distributed as part of Intel Graphics driver package.
* Header & library wrapper code are provided here to help developers with their application development.
* For API/spec questions or issues, for now, use the "Issues" tab under Github. For issues related to an already shipped binary of this spec, contact standard Intel customer support for Graphics.
* Core API's related to Engine/Fan/Frequency/Memory/Overclock/PCI/Power/Temperature are limited to 64-bit applications. This is a Level0 limitation which is limited to 64-bit. For these API's, IGCL uses Level0 library.

  Device Frequency APIs
  - ctlEnumFrequencyDomains
  - ctlFrequencyGetProperties
  - ctlFrequencyGetAvailableClocks
  - ctlFrequencyGetRange
  - ctlFrequencySetRange
  - ctlFrequencyGetState
  - ctlFrequencyGetThrottleTime

  Temperature APIs
  - ctlEnumTemperatureSensors
  - ctlTemperatureGetProperties
  - ctlTemperatureGetState

  Power APIs
  - ctlEnumPowerDomains
  - ctlPowerGetProperties
  - ctlPowerGetEnergyCounter
  - ctlPowerGetLimits
  - ctlPowerSetLimits

  Fan APIs
  - ctlEnumFans
  - ctlFanGetProperties
  - ctlFanGetConfig
  - ctlFanGetState
  - ctlFanSetDefaultMode
  - ctlFanSetFixedSpeedMode
  - ctlFanSetSpeedTableMode
  
  PCI APIs
  - ctlPciGetProperties
  - ctlPciGetState

  Memory APIs
  - ctlEnumMemoryModules
  - ctlMemoryGetProperties
  - ctlMemoryGetState
  - ctlMemoryGetBandwidth
  
  Engine APIs
  - ctlEnumEngineGroups
  - ctlEngineGetProperties
  - ctlEngineGetActivity

  More details on Level0 can be found at https://oneapi-src.github.io/level-zero-spec/level-zero/latest/index.html

# Usage 
cmake.exe -B <output_folder> -S <cmake_source_folder> -G "Visual Studio 17 2022" -A x64
