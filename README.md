# Control API
Source code for Control API

# Introduction
Control API is meant to be a high level API for all control aspects of hardware, primarily graphics. This is a replacement of legacy CUISDK. It will allow global control and tweaking of display, media & 3D capabilities. It know how to talk to UMD, KMD and is expected to be multi-GPU and OS friendly.

# Level0 compatibility
Control API has an instance of Level0 driver internally which allows it to talk to Level0 driver as and when required. It will provide API's which allow an application vendor to know of the Level0 device to talk to which corresponds to the control API device.

Note: To test the sample along with level0, please ensure that the level0 binaries (primarily ze_loader.dll & ze_intel_gpu64.dll) are 
in a path accessible by the sample.
