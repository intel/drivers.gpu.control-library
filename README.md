# Intel Graphics Control Library (IGCL)
Header, wrapper library and samples of IGCL version 1.0
 

# Introduction
IGCL is meant to be a collection of high level APIs for all control aspects of hardware, primarily graphics. This is replacement of legacy Intel CUISDK which used to be released only to OEM's and selected customers. IGCL allows global control and tweaking of display, media & 3D capabilities.

# Notes
* IGCL binaries are distributed as part of Intel Graphics driver package.
* Header & library wrapper code are provided here to help developers with their application development.
* For API/spec questions or issues, for now, use the "Issues" tab under Github. For issues related to an already shipped binary of this spec, contact standard Intel customer support for Graphics.
* Not all API's are supported with 32-bit as L0 is limited to 64-bit. Please refer following table.

| Domains	| 32/64b	| GPU support| 
| --------|--------|------------|
| 3D/gaming	| 32b+64b	| iGPU + dGPU| 
| Media	| 32b+64b	| iGPU + dGPU| 
| Display	| 32b+64b	| iGPU + dGPU| 
| Over clocking (OC)	| 32b+64b	| dGPU only| 
| Telemetry (ctlPowerTelemetryGet)	| 32b+64b	| iGPU + dGPU| 
| Fan/engine/frequency/memory/pci/power/temperature API's (L0 wrapper)	| 64b only	| iGPU + dGPU| 

# Usage 
cmake.exe -B <output_folder> -S <cmake_source_folder> -G "Visual Studio 17 2022" -A x64
