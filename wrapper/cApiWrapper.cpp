//===========================================================================
// Control API & Intel Proprietary Patch Software Licenses
// 4.19.21
// Control API Software License
// Use and Redistribution. You may use, modify, and redistribute the software (the
// "Software"), solely for use on Intel platforms, provided the following conditions are met
// Redistributions must reproduce the above copyright notice and the following terms
// of use in the Software and in the documentation and/or other materials provided
// with the distribution.
// Neither the name of Intel nor the names of its suppliers may be used to endorse
// or promote products derived from this Software without specific prior written
// permission.
// Limited patent license. Intel grants you a world-wide, royalty-free, non-exclusive license
// under patents it now or hereafter owns or controls to make, have made, use, import,
// offer to sell and sell ("Utilize") this Software, but solely to the extent that any such patent
// is necessary to Utilize the Software alone. The patent license shall not apply to any
// combinations which include this software. No hardware per se is licensed hereunder.
//
// Third party programs. The Software may contain Third Party Programs. "Third Party
// Programs" are third party software, open source software or other Intel software listed
// in the "third-party-programs.txt" or other similarly named text file that is included with
// the Software. Third Party Programs, even if included with the distribution of the Software,
// may be governed by separate license terms, including without limitation, third party
// license terms, open source software notices and terms, and/or other Intel software
// license terms. These separate license terms may govern your use of the Third Party
// Programs.
// DISCLAIMER. THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT
// ARE DISCLAIMED. THIS SOFTWARE IS NOT INTENDED FOR USE IN SYSTEMS OR
// APPLICATIONS WHERE FAILURE OF THE SOFTWARE MAY CAUSE PERSONAL INJURY OR
// DEATH AND YOU AGREE THAT YOU ARE FULLY RESPONSIBLE FOR ANY CLAIMS, COSTS,
// DAMAGES, EXPENSES, AND ATTORNEY'S FEES ARISING OUT OF ANY SUCH USE, EVEN
// IF ANY CLAIM ALLEGES THAT INTEL WAS NEGLIGENT REGARDING THE DESIGN OR
// MANUFACTURE OF THE MATERIALS.
//
// LIMITATION OF LIABILITY. IN NO EVENT WILL INTEL BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE. YOU AGREE TO INDEMNIFY AND HOLD INTEL HARMLESS AGAINST ANY
// CLAIMS AND EXPENSES RESULTING FROM YOUR USE OR UNAUTHORIZED USE OF THE
// SOFTWARE.
// No support. Intel may make changes to the Software, at any time without notice, and is
// not obligated to support, update or provide training for the Software.
// Termination. Intel may terminate your right to use the Software in the event of your
// breach of this Agreement and you fail to cure the breach within a reasonable period of
// time.
// Feedback. Should you provide Intel with comments, modifications, corrections,
// enhancements or other input ("Feedback") related to the Software Intel will be free to
// use, disclose, reproduce, license or otherwise distribute or exploit the Feedback in its sole
// discretion without any obligations or restrictions of any kind, including without limitation,
// intellectual property rights or licensing obligations.
// Compliance with laws. You agree to comply with all relevant laws and regulations
// governing your use, transfer, import or export (or prohibition thereof) of the Software.
//
// Governing law. All disputes will be governed by the laws of the United States of America
// and the State of Delaware without reference to conflict of law principles and subject to
// the exclusive jurisdiction of the state or federal courts sitting in the State of Delaware,
// and each party agrees that it submits to the personal jurisdiction and venue of those
// courts and waives any objections. The United Nations Convention on Contracts for the
// International Sale of Goods (1980) is specifically excluded and will not apply to the
// Software.
// *Other names and brands may be claimed as the property of others.
//--------------------------------------------------------------------------

/**
 *
 * @file ctl_api.cpp
 * @version v0-r7
 *
 */

#include <windows.h>
#include <strsafe.h>

#define CTL_APIEXPORT

#include "igcl_api.h"

/////////////////////////////////////////////////////////////////////////////////
//
// Implementation of wrapper functions
//
static HINSTANCE hinstLib = NULL;

/**
 * @brief Function to get DLL name based on app version
 *
 */
#if defined(_WIN64)
    #define CTL_DLL_NAME L"ControlLib"
#else
    #define CTL_DLL_NAME L"ControlLib32"
#endif
ctl_result_t GetControlAPIDLLPath(ctl_init_args_t* pInitArgs, wchar_t* pwcDLLPath)
{
    // Load the requested DLL based on major version in init args
    uint16_t majorVersion = CTL_MAJOR_VERSION(pInitArgs->AppVersion);

    // If caller's major version is higher than the DLL's, then simply not support the caller!
    // This is not supposed to happen as wrapper is part of the app itself which includes control_api.h with right major version
    if (majorVersion > CTL_IMPL_MAJOR_VERSION)
        return CTL_RESULT_ERROR_UNSUPPORTED_VERSION;

    if (majorVersion > 1)
        StringCbPrintfW(pwcDLLPath,512,L"%s%d.dll", CTL_DLL_NAME, majorVersion);
    else // just control_api.dll
        StringCbPrintfW(pwcDLLPath,512,L"%s.dll", CTL_DLL_NAME);

    return CTL_RESULT_SUCCESS;
}

/**
* @brief Control Api Init
* 
* @details
*     - Control Api Init
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pInitDesc`
*         + `nullptr == phAPIHandle`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlInit(
    ctl_init_args_t* pInitDesc,                     ///< [in][out] App's control API version
    ctl_api_handle_t* phAPIHandle                   ///< [in][out][release] Control API handle
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    
    // special code - only for ctlInit()
    if (hinstLib == NULL)
    {
        wchar_t strDLLPath[512];
        result = GetControlAPIDLLPath(pInitDesc, strDLLPath);
        if (result == CTL_RESULT_SUCCESS)
        {
            hinstLib = LoadLibraryW(strDLLPath);
        }    
    }

    if (hinstLib != NULL)
    {
        ctl_pfnInit_t pfnInit = (ctl_pfnInit_t)GetProcAddress(hinstLib, "ctlInit");
        if (pfnInit)
        {
            result = pfnInit(pInitDesc, phAPIHandle);
        }
    }


    return result;
}

/**
* @brief Control Api Destroy
* 
* @details
*     - Control Api Close
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hAPIHandle`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlClose(
    ctl_api_handle_t hAPIHandle                     ///< [in][release] Control API implementation handle obtained during init
                                                    ///< call
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnClose_t pfnClose = (ctl_pfnClose_t)GetProcAddress(hinstLib, "ctlClose");
        if (pfnClose)
        {
            result = pfnClose(hAPIHandle);
        }
    }

    // special code - only for ctlClose()
    FreeLibrary(hinstLib);
    hinstLib = NULL;

    return result;
}

/**
* @brief Wait for a property change. Note that this is a blocking call
* 
* @details
*     - Wait for a property change in display, 3d, media etc.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDeviceAdapter`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pArgs`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlWaitForPropertyChange(
    ctl_device_adapter_handle_t hDeviceAdapter,     ///< [in][release] handle to control device adapter
    ctl_wait_property_change_args_t* pArgs          ///< [in] Argument containing information about which property changes to
                                                    ///< listen for
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnWaitForPropertyChange_t pfnWaitForPropertyChange = (ctl_pfnWaitForPropertyChange_t)GetProcAddress(hinstLib, "ctlWaitForPropertyChange");
        if (pfnWaitForPropertyChange)
        {
            result = pfnWaitForPropertyChange(hDeviceAdapter, pArgs);
        }
    }


    return result;
}

/**
* @brief Reserved function
* 
* @details
*     - Reserved function
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDeviceAdapter`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pArgs`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlReservedCall(
    ctl_device_adapter_handle_t hDeviceAdapter,     ///< [in][release] handle to control device adapter
    ctl_reserved_args_t* pArgs                      ///< [in] Argument containing information
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnReservedCall_t pfnReservedCall = (ctl_pfnReservedCall_t)GetProcAddress(hinstLib, "ctlReservedCall");
        if (pfnReservedCall)
        {
            result = pfnReservedCall(hDeviceAdapter, pArgs);
        }
    }


    return result;
}

/**
* @brief Get 3D capabilities
* 
* @details
*     - The application gets 3D properties
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pFeatureCaps`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetSupported3DCapabilities(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    ctl_3d_feature_caps_t* pFeatureCaps             ///< [in,out][release] 3D properties
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetSupported3DCapabilities_t pfnGetSupported3DCapabilities = (ctl_pfnGetSupported3DCapabilities_t)GetProcAddress(hinstLib, "ctlGetSupported3DCapabilities");
        if (pfnGetSupported3DCapabilities)
        {
            result = pfnGetSupported3DCapabilities(hDAhandle, pFeatureCaps);
        }
    }


    return result;
}

/**
* @brief Get/Set 3D feature
* 
* @details
*     - 3D feature details
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pFeature`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetSet3DFeature(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    ctl_3d_feature_getset_t* pFeature               ///< [in][release] 3D feature get/set parameter
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetSet3DFeature_t pfnGetSet3DFeature = (ctl_pfnGetSet3DFeature_t)GetProcAddress(hinstLib, "ctlGetSet3DFeature");
        if (pfnGetSet3DFeature)
        {
            result = pfnGetSet3DFeature(hDAhandle, pFeature);
        }
    }


    return result;
}

/**
* @brief Check Driver version
* 
* @details
*     - The application checks driver version
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDeviceAdapter`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlCheckDriverVersion(
    ctl_device_adapter_handle_t hDeviceAdapter,     ///< [in][release] handle to control device adapter
    ctl_version_info_t version_info                 ///< [in][release] Driver version info
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnCheckDriverVersion_t pfnCheckDriverVersion = (ctl_pfnCheckDriverVersion_t)GetProcAddress(hinstLib, "ctlCheckDriverVersion");
        if (pfnCheckDriverVersion)
        {
            result = pfnCheckDriverVersion(hDeviceAdapter, version_info);
        }
    }


    return result;
}

/**
* @brief Enumerate devices
* 
* @details
*     - The application checks driver version
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hAPIHandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pCount`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlEnumerateDevices(
    ctl_api_handle_t hAPIHandle,                    ///< [in][release] Applications should pass the Control API handle returned
                                                    ///< by the CtlInit function 
    uint32_t* pCount,                               ///< [in,out][release] pointer to the number of device instances. If count
                                                    ///< is zero, then the api will update the value with the total
                                                    ///< number of drivers available. If count is non-zero, then the api will
                                                    ///< only retrieve the number of drivers.
                                                    ///< If count is larger than the number of drivers available, then the api
                                                    ///< will update the value with the correct number of drivers available.
    ctl_device_adapter_handle_t* phDevices          ///< [in,out][optional][release][range(0, *pCount)] array of driver
                                                    ///< instance handles
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnEnumerateDevices_t pfnEnumerateDevices = (ctl_pfnEnumerateDevices_t)GetProcAddress(hinstLib, "ctlEnumerateDevices");
        if (pfnEnumerateDevices)
        {
            result = pfnEnumerateDevices(hAPIHandle, pCount, phDevices);
        }
    }


    return result;
}

/**
* @brief Enumerate display outputs
* 
* @details
*     - Enumerates display output capabilities
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDeviceAdapter`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pCount`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlEnumerateDisplayOutputs(
    ctl_device_adapter_handle_t hDeviceAdapter,     ///< [in][release] handle to control device adapter
    uint32_t* pCount,                               ///< [in,out][release] pointer to the number of display output instances.
                                                    ///< If count is zero, then the api will update the value with the total
                                                    ///< number of outputs available. If count is non-zero, then the api will
                                                    ///< only retrieve the number of outputs.
                                                    ///< If count is larger than the number of drivers available, then the api
                                                    ///< will update the value with the correct number of drivers available.
    ctl_display_output_handle_t* phDisplayOutputs   ///< [in,out][optional][release][range(0, *pCount)] array of display output
                                                    ///< instance handles
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnEnumerateDisplayOutputs_t pfnEnumerateDisplayOutputs = (ctl_pfnEnumerateDisplayOutputs_t)GetProcAddress(hinstLib, "ctlEnumerateDisplayOutputs");
        if (pfnEnumerateDisplayOutputs)
        {
            result = pfnEnumerateDisplayOutputs(hDeviceAdapter, pCount, phDisplayOutputs);
        }
    }


    return result;
}

/**
* @brief Get Device Properties
* 
* @details
*     - The application gets device properties
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pProperties`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetDeviceProperties(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to control device adapter
    ctl_device_adapter_properties_t* pProperties    ///< [in,out][release] Query result for device properties
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetDeviceProperties_t pfnGetDeviceProperties = (ctl_pfnGetDeviceProperties_t)GetProcAddress(hinstLib, "ctlGetDeviceProperties");
        if (pfnGetDeviceProperties)
        {
            result = pfnGetDeviceProperties(hDAhandle, pProperties);
        }
    }


    return result;
}

/**
* @brief Get Display  Properties
* 
* @details
*     - The application gets display  properties
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pProperties`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetDisplayProperties(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_display_properties_t* pProperties           ///< [in,out][release] Query result for display  properties
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetDisplayProperties_t pfnGetDisplayProperties = (ctl_pfnGetDisplayProperties_t)GetProcAddress(hinstLib, "ctlGetDisplayProperties");
        if (pfnGetDisplayProperties)
        {
            result = pfnGetDisplayProperties(hDisplayOutput, pProperties);
        }
    }


    return result;
}

/**
* @brief Get Adapter Display encoder  Properties
* 
* @details
*     - The application gets the graphic adapters display encoder properties
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pProperties`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetAdaperDisplayEncoderProperties(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_adapter_display_encoder_properties_t* pProperties   ///< [in,out][release] Query result for adapter display encoder properties
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetAdaperDisplayEncoderProperties_t pfnGetAdaperDisplayEncoderProperties = (ctl_pfnGetAdaperDisplayEncoderProperties_t)GetProcAddress(hinstLib, "ctlGetAdaperDisplayEncoderProperties");
        if (pfnGetAdaperDisplayEncoderProperties)
        {
            result = pfnGetAdaperDisplayEncoderProperties(hDisplayOutput, pProperties);
        }
    }


    return result;
}

/**
* @brief Get Level0 Device handle
* 
* @details
*     - The application gets OneAPI Level0 Device handles
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pZeDevice`
*         + `nullptr == hInstance`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetZeDevice(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    void* pZeDevice,                                ///< [out][release] ze_device handle
    void** hInstance                                ///< [out][release] Module instance which caller can use to get export
                                                    ///< functions directly
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetZeDevice_t pfnGetZeDevice = (ctl_pfnGetZeDevice_t)GetProcAddress(hinstLib, "ctlGetZeDevice");
        if (pfnGetZeDevice)
        {
            result = pfnGetZeDevice(hDAhandle, pZeDevice, hInstance);
        }
    }


    return result;
}

/**
* @brief Get Sharpness capability
* 
* @details
*     - Returns sharpness capability
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pSharpnessCaps`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetSharpnessCaps(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_sharpness_caps_t* pSharpnessCaps            ///< [in,out][release] Query result for sharpness capability
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetSharpnessCaps_t pfnGetSharpnessCaps = (ctl_pfnGetSharpnessCaps_t)GetProcAddress(hinstLib, "ctlGetSharpnessCaps");
        if (pfnGetSharpnessCaps)
        {
            result = pfnGetSharpnessCaps(hDisplayOutput, pSharpnessCaps);
        }
    }


    return result;
}

/**
* @brief Get Sharpness setting
* 
* @details
*     - Returns current sharpness settings
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pSharpnessSettings`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetCurrentSharpness(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_sharpness_settings_t* pSharpnessSettings    ///< [in,out][release] Query result for sharpness current settings
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetCurrentSharpness_t pfnGetCurrentSharpness = (ctl_pfnGetCurrentSharpness_t)GetProcAddress(hinstLib, "ctlGetCurrentSharpness");
        if (pfnGetCurrentSharpness)
        {
            result = pfnGetCurrentSharpness(hDisplayOutput, pSharpnessSettings);
        }
    }


    return result;
}

/**
* @brief Set Sharpness setting
* 
* @details
*     - Set current sharpness settings
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pSharpnessSettings`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlSetCurrentSharpness(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_sharpness_settings_t* pSharpnessSettings    ///< [in][release] Set sharpness current settings
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnSetCurrentSharpness_t pfnSetCurrentSharpness = (ctl_pfnSetCurrentSharpness_t)GetProcAddress(hinstLib, "ctlSetCurrentSharpness");
        if (pfnSetCurrentSharpness)
        {
            result = pfnSetCurrentSharpness(hDisplayOutput, pSharpnessSettings);
        }
    }


    return result;
}

/**
* @brief I2C Access
* 
* @details
*     - The application does I2C aceess
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pI2cAccessArgs`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*     - ::CTL_RESULT_ERROR_INVALID_OPERATION_TYPE - "Invalid operation type"
*     - ::CTL_RESULT_ERROR_INVALID_SIZE - "Invalid I2C data size"
*     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS - "Insufficient permissions"
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER - "Invalid null pointer"
*     - ::CTL_RESULT_ERROR_NULL_OS_DISPLAY_OUTPUT_HANDLE - "Null OS display output handle"
*     - ::CTL_RESULT_ERROR_NULL_OS_INTERFACE - "Null OS interface"
*     - ::CTL_RESULT_ERROR_NULL_OS_ADAPATER_HANDLE - "Null OS adapter handle"
*     - ::CTL_RESULT_ERROR_KMD_CALL - "Kernal mode driver call failure"
*/
ctl_result_t CTL_APICALL
ctlI2CAccess(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in] Handle to display output
    ctl_i2c_access_args_t* pI2cAccessArgs           ///< [in,out] I2c access arguments
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnI2CAccess_t pfnI2CAccess = (ctl_pfnI2CAccess_t)GetProcAddress(hinstLib, "ctlI2CAccess");
        if (pfnI2CAccess)
        {
            result = pfnI2CAccess(hDisplayOutput, pI2cAccessArgs);
        }
    }


    return result;
}

/**
* @brief Aux Access
* 
* @details
*     - The application does Aux aceess
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pAuxAccessArgs`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*     - ::CTL_RESULT_ERROR_INVALID_OPERATION_TYPE - "Invalid operation type"
*     - ::CTL_RESULT_ERROR_INVALID_SIZE - "Invalid AUX data size"
*     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS - "Insufficient permissions"
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER - "Invalid null pointer"
*     - ::CTL_RESULT_ERROR_NULL_OS_DISPLAY_OUTPUT_HANDLE - "Null OS display output handle"
*     - ::CTL_RESULT_ERROR_NULL_OS_INTERFACE - "Null OS interface"
*     - ::CTL_RESULT_ERROR_NULL_OS_ADAPATER_HANDLE - "Null OS adapter handle"
*     - ::CTL_RESULT_ERROR_INVALID_AUX_ACCESS_FLAG - "Invalid flag for AUX access"
*     - ::CTL_RESULT_ERROR_KMD_CALL - "Kernal mode driver call failure"
*/
ctl_result_t CTL_APICALL
ctlAUXAccess(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in] Handle to display output
    ctl_aux_access_args_t* pAuxAccessArgs           ///< [in,out] Aux access arguments
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnAUXAccess_t pfnAUXAccess = (ctl_pfnAUXAccess_t)GetProcAddress(hinstLib, "ctlAUXAccess");
        if (pfnAUXAccess)
        {
            result = pfnAUXAccess(hDisplayOutput, pAuxAccessArgs);
        }
    }


    return result;
}

/**
* @brief Get Power optimization features
* 
* @details
*     - Returns power optimization capabilities
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pPowerOptimizationCaps`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetPowerOptimizationCaps(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_power_optimization_caps_t* pPowerOptimizationCaps   ///< [in,out][release] Query result for power optimization features
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetPowerOptimizationCaps_t pfnGetPowerOptimizationCaps = (ctl_pfnGetPowerOptimizationCaps_t)GetProcAddress(hinstLib, "ctlGetPowerOptimizationCaps");
        if (pfnGetPowerOptimizationCaps)
        {
            result = pfnGetPowerOptimizationCaps(hDisplayOutput, pPowerOptimizationCaps);
        }
    }


    return result;
}

/**
* @brief Get Power optimization setting
* 
* @details
*     - Returns power optimization setting for a specific feature
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pPowerOptimizationSettings`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*     - ::CTL_RESULT_ERROR_INVALID_POWERFEATURE_OPTIMIZATION_FLAG - "Unsupported PowerOptimizationFeature"
*     - ::CTL_RESULT_ERROR_INVALID_POWERSOURCE_TYPE_FOR_DPST - "DPST is supported only in DC Mode"
*/
ctl_result_t CTL_APICALL
ctlGetPowerOptimizationSetting(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_power_optimization_settings_t* pPowerOptimizationSettings   ///< [in,out][release] Power optimization data to be fetched
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetPowerOptimizationSetting_t pfnGetPowerOptimizationSetting = (ctl_pfnGetPowerOptimizationSetting_t)GetProcAddress(hinstLib, "ctlGetPowerOptimizationSetting");
        if (pfnGetPowerOptimizationSetting)
        {
            result = pfnGetPowerOptimizationSetting(hDisplayOutput, pPowerOptimizationSettings);
        }
    }


    return result;
}

/**
* @brief Set Power optimization setting
* 
* @details
*     - Set power optimization setting for a specific feature
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pPowerOptimizationSettings`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*     - ::CTL_RESULT_ERROR_INVALID_POWERFEATURE_OPTIMIZATION_FLAG - "Unsupported PowerOptimizationFeature"
*     - ::CTL_RESULT_ERROR_INVALID_POWERSOURCE_TYPE_FOR_DPST - "DPST is supported only in DC Mode"
*/
ctl_result_t CTL_APICALL
ctlSetPowerOptimizationSetting(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_power_optimization_settings_t* pPowerOptimizationSettings   ///< [in][release] Power optimization data to be applied
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnSetPowerOptimizationSetting_t pfnSetPowerOptimizationSetting = (ctl_pfnSetPowerOptimizationSetting_t)GetProcAddress(hinstLib, "ctlSetPowerOptimizationSetting");
        if (pfnSetPowerOptimizationSetting)
        {
            result = pfnSetPowerOptimizationSetting(hDisplayOutput, pPowerOptimizationSettings);
        }
    }


    return result;
}

/**
* @brief Pixel transformation get pipe configuration
* 
* @details
*     - The application does pixel transformation get pipe configuration
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pPixTxGetConfigArgs`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS - "Insufficient permissions"
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER - "Invalid null pointer"
*     - ::CTL_RESULT_ERROR_NULL_OS_DISPLAY_OUTPUT_HANDLE - "Null OS display output handle"
*     - ::CTL_RESULT_ERROR_NULL_OS_INTERFACE - "Null OS interface"
*     - ::CTL_RESULT_ERROR_NULL_OS_ADAPATER_HANDLE - "Null OS adapter handle"
*     - ::CTL_RESULT_ERROR_KMD_CALL - "Kernal mode driver call failure"
*     - ::CTL_RESULT_ERROR_INVALID_PIXTX_GET_CONFIG_QUERY_TYPE - "Invalid query type"
*     - ::CTL_RESULT_ERROR_INVALID_PIXTX_BLOCK_ID - "Invalid block id"
*     - ::CTL_RESULT_ERROR_INSUFFICIENT_PIXTX_BLOCK_CONFIG_MEMORY - "Insufficient memery allocated for BlockConfigs"
*/
ctl_result_t CTL_APICALL
ctlPixelTransformationGetConfig(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in] Handle to display output
    ctl_pixtx_pipe_get_config_t* pPixTxGetConfigArgs///< [in,out] Pixel transformation get pipe configiguration arguments
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnPixelTransformationGetConfig_t pfnPixelTransformationGetConfig = (ctl_pfnPixelTransformationGetConfig_t)GetProcAddress(hinstLib, "ctlPixelTransformationGetConfig");
        if (pfnPixelTransformationGetConfig)
        {
            result = pfnPixelTransformationGetConfig(hDisplayOutput, pPixTxGetConfigArgs);
        }
    }


    return result;
}

/**
* @brief Pixel transformation set pipe configuration
* 
* @details
*     - The application does pixel transformation set pipe configuration
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pPixTxSetConfigArgs`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS - "Insufficient permissions"
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER - "Invalid null pointer"
*     - ::CTL_RESULT_ERROR_NULL_OS_DISPLAY_OUTPUT_HANDLE - "Null OS display output handle"
*     - ::CTL_RESULT_ERROR_NULL_OS_INTERFACE - "Null OS interface"
*     - ::CTL_RESULT_ERROR_NULL_OS_ADAPATER_HANDLE - "Null OS adapter handle"
*     - ::CTL_RESULT_ERROR_KMD_CALL - "Kernal mode driver call failure"
*     - ::CTL_RESULT_ERROR_INVALID_PIXTX_SET_CONFIG_OPERATION_TYPE - "Invalid operation type"
*     - ::CTL_RESULT_ERROR_INVALID_SET_CONFIG_NUMBER_OF_SAMPLES - "Invalid number of samples"
*     - ::CTL_RESULT_ERROR_INVALID_PIXTX_BLOCK_ID - "Invalid block id"
*     - ::CTL_RESULT_ERROR_PERSISTANCE_NOT_SUPPORTED - "Persistance not supported"
*/
ctl_result_t CTL_APICALL
ctlPixelTransformationSetConfig(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in] Handle to display output
    ctl_pixtx_pipe_set_config_t* pPixTxSetConfigArgs///< [in,out] Pixel transformation set pipe configiguration arguments
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnPixelTransformationSetConfig_t pfnPixelTransformationSetConfig = (ctl_pfnPixelTransformationSetConfig_t)GetProcAddress(hinstLib, "ctlPixelTransformationSetConfig");
        if (pfnPixelTransformationSetConfig)
        {
            result = pfnPixelTransformationSetConfig(hDisplayOutput, pPixTxSetConfigArgs);
        }
    }


    return result;
}

/**
* @brief Panel Descriptor Access
* 
* @details
*     - The application does EDID or Display ID access
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pPanelDescriptorAccessArgs`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*     - ::CTL_RESULT_ERROR_INVALID_OPERATION_TYPE - "Invalid operation type"
*     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS - "Insufficient permissions"
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER - "Invalid null pointer"
*     - ::CTL_RESULT_ERROR_NULL_OS_DISPLAY_OUTPUT_HANDLE - "Null OS display output handle"
*     - ::CTL_RESULT_ERROR_NULL_OS_INTERFACE - "Null OS interface"
*     - ::CTL_RESULT_ERROR_NULL_OS_ADAPATER_HANDLE - "Null OS adapter handle"
*     - ::CTL_RESULT_ERROR_KMD_CALL - "Kernal mode driver call failure"
*/
ctl_result_t CTL_APICALL
ctlPanelDescriptorAccess(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in] Handle to display output
    ctl_panel_descriptor_access_args_t* pPanelDescriptorAccessArgs  ///< [in,out] Panel descriptor access arguments
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnPanelDescriptorAccess_t pfnPanelDescriptorAccess = (ctl_pfnPanelDescriptorAccess_t)GetProcAddress(hinstLib, "ctlPanelDescriptorAccess");
        if (pfnPanelDescriptorAccess)
        {
            result = pfnPanelDescriptorAccess(hDisplayOutput, pPanelDescriptorAccessArgs);
        }
    }


    return result;
}

/**
* @brief Get Supported Retro Scaling Types
* 
* @details
*     - Returns supported retro scaling capabilities
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pRetroScalingCaps`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetSupportedRetroScalingCapability(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to adapter
    ctl_retro_scaling_caps_t* pRetroScalingCaps     ///< [in,out][release] Query result for supported retro scaling types
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetSupportedRetroScalingCapability_t pfnGetSupportedRetroScalingCapability = (ctl_pfnGetSupportedRetroScalingCapability_t)GetProcAddress(hinstLib, "ctlGetSupportedRetroScalingCapability");
        if (pfnGetSupportedRetroScalingCapability)
        {
            result = pfnGetSupportedRetroScalingCapability(hDAhandle, pRetroScalingCaps);
        }
    }


    return result;
}

/**
* @brief Get/Set Retro Scaling
* 
* @details
*     - Get or Set the status of retro scaling.This Api will do a physical
*       modeset resulting in flash on the screen
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pGetSetRetroScalingType`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetSetRetroScaling(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to adapter
    ctl_retro_scaling_settings_t* pGetSetRetroScalingType   ///< [in,out][release] Get or Set the retro scaling type
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetSetRetroScaling_t pfnGetSetRetroScaling = (ctl_pfnGetSetRetroScaling_t)GetProcAddress(hinstLib, "ctlGetSetRetroScaling");
        if (pfnGetSetRetroScaling)
        {
            result = pfnGetSetRetroScaling(hDAhandle, pGetSetRetroScalingType);
        }
    }


    return result;
}

/**
* @brief Get Supported Scaling Types
* 
* @details
*     - Returns supported scaling capabilities
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pScalingCaps`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetSupportedScalingCapability(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_scaling_caps_t* pScalingCaps                ///< [in,out][release] Query result for supported scaling types
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetSupportedScalingCapability_t pfnGetSupportedScalingCapability = (ctl_pfnGetSupportedScalingCapability_t)GetProcAddress(hinstLib, "ctlGetSupportedScalingCapability");
        if (pfnGetSupportedScalingCapability)
        {
            result = pfnGetSupportedScalingCapability(hDisplayOutput, pScalingCaps);
        }
    }


    return result;
}

/**
* @brief Get Current Scaling
* 
* @details
*     - Returns current active scaling
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pGetCurrentScalingType`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetCurrentScaling(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_scaling_settings_t* pGetCurrentScalingType  ///< [in,out][release] Query result for active scaling types
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetCurrentScaling_t pfnGetCurrentScaling = (ctl_pfnGetCurrentScaling_t)GetProcAddress(hinstLib, "ctlGetCurrentScaling");
        if (pfnGetCurrentScaling)
        {
            result = pfnGetCurrentScaling(hDisplayOutput, pGetCurrentScalingType);
        }
    }


    return result;
}

/**
* @brief Set Scaling Type
* 
* @details
*     - Returns current active scaling
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDisplayOutput`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pSetScalingType`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlSetCurrentScaling(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_scaling_settings_t* pSetScalingType         ///< [in,out][release] Set scaling types
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnSetCurrentScaling_t pfnSetCurrentScaling = (ctl_pfnSetCurrentScaling_t)GetProcAddress(hinstLib, "ctlSetCurrentScaling");
        if (pfnSetCurrentScaling)
        {
            result = pfnSetCurrentScaling(hDisplayOutput, pSetScalingType);
        }
    }


    return result;
}

/**
* @brief Get handle of engine groups
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pCount`
*/
ctl_result_t CTL_APICALL
ctlEnumEngineGroups(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to adapter
    uint32_t* pCount,                               ///< [in,out] pointer to the number of components of this type.
                                                    ///< if count is zero, then the driver shall update the value with the
                                                    ///< total number of components of this type that are available.
                                                    ///< if count is greater than the number of components of this type that
                                                    ///< are available, then the driver shall update the value with the correct
                                                    ///< number of components.
    ctl_engine_handle_t* phEngine                   ///< [in,out][optional][range(0, *pCount)] array of handle of components of
                                                    ///< this type.
                                                    ///< if count is less than the number of components of this type that are
                                                    ///< available, then the driver shall only retrieve that number of
                                                    ///< component handles.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnEnumEngineGroups_t pfnEnumEngineGroups = (ctl_pfnEnumEngineGroups_t)GetProcAddress(hinstLib, "ctlEnumEngineGroups");
        if (pfnEnumEngineGroups)
        {
            result = pfnEnumEngineGroups(hDAhandle, pCount, phEngine);
        }
    }


    return result;
}

/**
* @brief Get engine group properties
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hEngine`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pProperties`
*/
ctl_result_t CTL_APICALL
ctlEngineGetProperties(
    ctl_engine_handle_t hEngine,                    ///< [in] Handle for the component.
    ctl_engine_properties_t* pProperties            ///< [in,out] The properties for the specified engine group.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnEngineGetProperties_t pfnEngineGetProperties = (ctl_pfnEngineGetProperties_t)GetProcAddress(hinstLib, "ctlEngineGetProperties");
        if (pfnEngineGetProperties)
        {
            result = pfnEngineGetProperties(hEngine, pProperties);
        }
    }


    return result;
}

/**
* @brief Get the activity stats for an engine group
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hEngine`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pStats`
*/
ctl_result_t CTL_APICALL
ctlEngineGetActivity(
    ctl_engine_handle_t hEngine,                    ///< [in] Handle for the component.
    ctl_engine_stats_t* pStats                      ///< [in,out] Will contain a snapshot of the engine group activity
                                                    ///< counters.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnEngineGetActivity_t pfnEngineGetActivity = (ctl_pfnEngineGetActivity_t)GetProcAddress(hinstLib, "ctlEngineGetActivity");
        if (pfnEngineGetActivity)
        {
            result = pfnEngineGetActivity(hEngine, pStats);
        }
    }


    return result;
}

/**
* @brief Get handle of fans
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pCount`
*/
ctl_result_t CTL_APICALL
ctlEnumFans(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to the adapter
    uint32_t* pCount,                               ///< [in,out] pointer to the number of components of this type.
                                                    ///< if count is zero, then the driver shall update the value with the
                                                    ///< total number of components of this type that are available.
                                                    ///< if count is greater than the number of components of this type that
                                                    ///< are available, then the driver shall update the value with the correct
                                                    ///< number of components.
    ctl_fan_handle_t* phFan                         ///< [in,out][optional][range(0, *pCount)] array of handle of components of
                                                    ///< this type.
                                                    ///< if count is less than the number of components of this type that are
                                                    ///< available, then the driver shall only retrieve that number of
                                                    ///< component handles.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnEnumFans_t pfnEnumFans = (ctl_pfnEnumFans_t)GetProcAddress(hinstLib, "ctlEnumFans");
        if (pfnEnumFans)
        {
            result = pfnEnumFans(hDAhandle, pCount, phFan);
        }
    }


    return result;
}

/**
* @brief Get fan properties
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hFan`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pProperties`
*/
ctl_result_t CTL_APICALL
ctlFanGetProperties(
    ctl_fan_handle_t hFan,                          ///< [in] Handle for the component.
    ctl_fan_properties_t* pProperties               ///< [in,out] Will contain the properties of the fan.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnFanGetProperties_t pfnFanGetProperties = (ctl_pfnFanGetProperties_t)GetProcAddress(hinstLib, "ctlFanGetProperties");
        if (pfnFanGetProperties)
        {
            result = pfnFanGetProperties(hFan, pProperties);
        }
    }


    return result;
}

/**
* @brief Get fan configurations and the current fan speed mode (default, fixed,
*        temp-speed table)
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hFan`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pConfig`
*/
ctl_result_t CTL_APICALL
ctlFanGetConfig(
    ctl_fan_handle_t hFan,                          ///< [in] Handle for the component.
    ctl_fan_config_t* pConfig                       ///< [in,out] Will contain the current configuration of the fan.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnFanGetConfig_t pfnFanGetConfig = (ctl_pfnFanGetConfig_t)GetProcAddress(hinstLib, "ctlFanGetConfig");
        if (pfnFanGetConfig)
        {
            result = pfnFanGetConfig(hFan, pConfig);
        }
    }


    return result;
}

/**
* @brief Configure the fan to run with hardware factory settings (set mode to
*        ::CTL_FAN_SPEED_MODE_DEFAULT)
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hFan`
*     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS
*         + User does not have permissions to make these modifications.
*/
ctl_result_t CTL_APICALL
ctlFanSetDefaultMode(
    ctl_fan_handle_t hFan                           ///< [in] Handle for the component.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnFanSetDefaultMode_t pfnFanSetDefaultMode = (ctl_pfnFanSetDefaultMode_t)GetProcAddress(hinstLib, "ctlFanSetDefaultMode");
        if (pfnFanSetDefaultMode)
        {
            result = pfnFanSetDefaultMode(hFan);
        }
    }


    return result;
}

/**
* @brief Configure the fan to rotate at a fixed speed (set mode to
*        ::CTL_FAN_SPEED_MODE_FIXED)
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hFan`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == speed`
*     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS
*         + User does not have permissions to make these modifications.
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_FEATURE
*         + Fixing the fan speed not supported by the hardware or the fan speed units are not supported. See ::ctl_fan_properties_t.supportedModes and ::ctl_fan_properties_t.supportedUnits.
*/
ctl_result_t CTL_APICALL
ctlFanSetFixedSpeedMode(
    ctl_fan_handle_t hFan,                          ///< [in] Handle for the component.
    const ctl_fan_speed_t* speed                    ///< [in] The fixed fan speed setting
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnFanSetFixedSpeedMode_t pfnFanSetFixedSpeedMode = (ctl_pfnFanSetFixedSpeedMode_t)GetProcAddress(hinstLib, "ctlFanSetFixedSpeedMode");
        if (pfnFanSetFixedSpeedMode)
        {
            result = pfnFanSetFixedSpeedMode(hFan, speed);
        }
    }


    return result;
}

/**
* @brief Configure the fan to adjust speed based on a temperature/speed table
*        (set mode to ::CTL_FAN_SPEED_MODE_TABLE)
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hFan`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == speedTable`
*     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS
*         + User does not have permissions to make these modifications.
*     - ::CTL_RESULT_ERROR_INVALID_ARGUMENT
*         + The temperature/speed pairs in the array are not sorted on temperature from lowest to highest.
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_FEATURE
*         + Fan speed table not supported by the hardware or the fan speed units are not supported. See ::ctl_fan_properties_t.supportedModes and ::ctl_fan_properties_t.supportedUnits.
*/
ctl_result_t CTL_APICALL
ctlFanSetSpeedTableMode(
    ctl_fan_handle_t hFan,                          ///< [in] Handle for the component.
    const ctl_fan_speed_table_t* speedTable         ///< [in] A table containing temperature/speed pairs.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnFanSetSpeedTableMode_t pfnFanSetSpeedTableMode = (ctl_pfnFanSetSpeedTableMode_t)GetProcAddress(hinstLib, "ctlFanSetSpeedTableMode");
        if (pfnFanSetSpeedTableMode)
        {
            result = pfnFanSetSpeedTableMode(hFan, speedTable);
        }
    }


    return result;
}

/**
* @brief Get current state of a fan - current mode and speed
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hFan`
*     - ::CTL_RESULT_ERROR_INVALID_ENUMERATION
*         + `::CTL_FAN_SPEED_UNITS_PERCENT < units`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pSpeed`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_FEATURE
*         + The requested fan speed units are not supported. See ::ctl_fan_properties_t.supportedUnits.
*/
ctl_result_t CTL_APICALL
ctlFanGetState(
    ctl_fan_handle_t hFan,                          ///< [in] Handle for the component.
    ctl_fan_speed_units_t units,                    ///< [in] The units in which the fan speed should be returned.
    int32_t* pSpeed                                 ///< [in,out] Will contain the current speed of the fan in the units
                                                    ///< requested. A value of -1 indicates that the fan speed cannot be
                                                    ///< measured.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnFanGetState_t pfnFanGetState = (ctl_pfnFanGetState_t)GetProcAddress(hinstLib, "ctlFanGetState");
        if (pfnFanGetState)
        {
            result = pfnFanGetState(hFan, units, pSpeed);
        }
    }


    return result;
}

/**
* @brief Get handle of frequency domains
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pCount`
*/
ctl_result_t CTL_APICALL
ctlEnumFrequencyDomains(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    uint32_t* pCount,                               ///< [in,out] pointer to the number of components of this type.
                                                    ///< if count is zero, then the driver shall update the value with the
                                                    ///< total number of components of this type that are available.
                                                    ///< if count is greater than the number of components of this type that
                                                    ///< are available, then the driver shall update the value with the correct
                                                    ///< number of components.
    ctl_freq_handle_t* phFrequency                  ///< [in,out][optional][range(0, *pCount)] array of handle of components of
                                                    ///< this type.
                                                    ///< if count is less than the number of components of this type that are
                                                    ///< available, then the driver shall only retrieve that number of
                                                    ///< component handles.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnEnumFrequencyDomains_t pfnEnumFrequencyDomains = (ctl_pfnEnumFrequencyDomains_t)GetProcAddress(hinstLib, "ctlEnumFrequencyDomains");
        if (pfnEnumFrequencyDomains)
        {
            result = pfnEnumFrequencyDomains(hDAhandle, pCount, phFrequency);
        }
    }


    return result;
}

/**
* @brief Get frequency properties - available frequencies
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hFrequency`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pProperties`
*/
ctl_result_t CTL_APICALL
ctlFrequencyGetProperties(
    ctl_freq_handle_t hFrequency,                   ///< [in] Handle for the component.
    ctl_freq_properties_t* pProperties              ///< [in,out] The frequency properties for the specified domain.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnFrequencyGetProperties_t pfnFrequencyGetProperties = (ctl_pfnFrequencyGetProperties_t)GetProcAddress(hinstLib, "ctlFrequencyGetProperties");
        if (pfnFrequencyGetProperties)
        {
            result = pfnFrequencyGetProperties(hFrequency, pProperties);
        }
    }


    return result;
}

/**
* @brief Get available non-overclocked hardware clock frequencies for the
*        frequency domain
* 
* @details
*     - The list of available frequencies is returned in order of slowest to
*       fastest.
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hFrequency`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pCount`
*/
ctl_result_t CTL_APICALL
ctlFrequencyGetAvailableClocks(
    ctl_freq_handle_t hFrequency,                   ///< [in] Device handle of the device.
    uint32_t* pCount,                               ///< [in,out] pointer to the number of frequencies.
                                                    ///< if count is zero, then the driver shall update the value with the
                                                    ///< total number of frequencies that are available.
                                                    ///< if count is greater than the number of frequencies that are available,
                                                    ///< then the driver shall update the value with the correct number of frequencies.
    double* phFrequency                             ///< [in,out][optional][range(0, *pCount)] array of frequencies in units of
                                                    ///< MHz and sorted from slowest to fastest.
                                                    ///< if count is less than the number of frequencies that are available,
                                                    ///< then the driver shall only retrieve that number of frequencies.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnFrequencyGetAvailableClocks_t pfnFrequencyGetAvailableClocks = (ctl_pfnFrequencyGetAvailableClocks_t)GetProcAddress(hinstLib, "ctlFrequencyGetAvailableClocks");
        if (pfnFrequencyGetAvailableClocks)
        {
            result = pfnFrequencyGetAvailableClocks(hFrequency, pCount, phFrequency);
        }
    }


    return result;
}

/**
* @brief Get current frequency limits
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hFrequency`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pLimits`
*/
ctl_result_t CTL_APICALL
ctlFrequencyGetRange(
    ctl_freq_handle_t hFrequency,                   ///< [in] Handle for the component.
    ctl_freq_range_t* pLimits                       ///< [in,out] The range between which the hardware can operate for the
                                                    ///< specified domain.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnFrequencyGetRange_t pfnFrequencyGetRange = (ctl_pfnFrequencyGetRange_t)GetProcAddress(hinstLib, "ctlFrequencyGetRange");
        if (pfnFrequencyGetRange)
        {
            result = pfnFrequencyGetRange(hFrequency, pLimits);
        }
    }


    return result;
}

/**
* @brief Set frequency range between which the hardware can operate.
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hFrequency`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pLimits`
*     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS
*         + User does not have permissions to make these modifications.
*/
ctl_result_t CTL_APICALL
ctlFrequencySetRange(
    ctl_freq_handle_t hFrequency,                   ///< [in] Handle for the component.
    const ctl_freq_range_t* pLimits                 ///< [in] The limits between which the hardware can operate for the
                                                    ///< specified domain.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnFrequencySetRange_t pfnFrequencySetRange = (ctl_pfnFrequencySetRange_t)GetProcAddress(hinstLib, "ctlFrequencySetRange");
        if (pfnFrequencySetRange)
        {
            result = pfnFrequencySetRange(hFrequency, pLimits);
        }
    }


    return result;
}

/**
* @brief Get current frequency state - frequency request, actual frequency, TDP
*        limits
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hFrequency`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pState`
*/
ctl_result_t CTL_APICALL
ctlFrequencyGetState(
    ctl_freq_handle_t hFrequency,                   ///< [in] Handle for the component.
    ctl_freq_state_t* pState                        ///< [in,out] Frequency state for the specified domain.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnFrequencyGetState_t pfnFrequencyGetState = (ctl_pfnFrequencyGetState_t)GetProcAddress(hinstLib, "ctlFrequencyGetState");
        if (pfnFrequencyGetState)
        {
            result = pfnFrequencyGetState(hFrequency, pState);
        }
    }


    return result;
}

/**
* @brief Get frequency throttle time
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hFrequency`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pThrottleTime`
*/
ctl_result_t CTL_APICALL
ctlFrequencyGetThrottleTime(
    ctl_freq_handle_t hFrequency,                   ///< [in] Handle for the component.
    ctl_freq_throttle_time_t* pThrottleTime         ///< [in,out] Will contain a snapshot of the throttle time counters for the
                                                    ///< specified domain.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnFrequencyGetThrottleTime_t pfnFrequencyGetThrottleTime = (ctl_pfnFrequencyGetThrottleTime_t)GetProcAddress(hinstLib, "ctlFrequencyGetThrottleTime");
        if (pfnFrequencyGetThrottleTime)
        {
            result = pfnFrequencyGetThrottleTime(hFrequency, pThrottleTime);
        }
    }


    return result;
}

/**
* @brief Get Video Processing capabilities
* 
* @details
*     - The application gets Video Processing properties
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pFeatureCaps`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetSupportedVideoProcessingCapabilities(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    ctl_video_processing_feature_caps_t* pFeatureCaps   ///< [in,out][release] Video Processing properties
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetSupportedVideoProcessingCapabilities_t pfnGetSupportedVideoProcessingCapabilities = (ctl_pfnGetSupportedVideoProcessingCapabilities_t)GetProcAddress(hinstLib, "ctlGetSupportedVideoProcessingCapabilities");
        if (pfnGetSupportedVideoProcessingCapabilities)
        {
            result = pfnGetSupportedVideoProcessingCapabilities(hDAhandle, pFeatureCaps);
        }
    }


    return result;
}

/**
* @brief Get/Set Video Processing feature details
* 
* @details
*     - Video Processing feature details
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pFeature`
*     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
*/
ctl_result_t CTL_APICALL
ctlGetSetVideoProcessingFeature(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    ctl_video_processing_feature_getset_t* pFeature ///< [in][release] Video Processing feature get/set parameter
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnGetSetVideoProcessingFeature_t pfnGetSetVideoProcessingFeature = (ctl_pfnGetSetVideoProcessingFeature_t)GetProcAddress(hinstLib, "ctlGetSetVideoProcessingFeature");
        if (pfnGetSetVideoProcessingFeature)
        {
            result = pfnGetSetVideoProcessingFeature(hDAhandle, pFeature);
        }
    }


    return result;
}

/**
* @brief Get handle of memory modules
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pCount`
*/
ctl_result_t CTL_APICALL
ctlEnumMemoryModules(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    uint32_t* pCount,                               ///< [in,out] pointer to the number of components of this type.
                                                    ///< if count is zero, then the driver shall update the value with the
                                                    ///< total number of components of this type that are available.
                                                    ///< if count is greater than the number of components of this type that
                                                    ///< are available, then the driver shall update the value with the correct
                                                    ///< number of components.
    ctl_mem_handle_t* phMemory                      ///< [in,out][optional][range(0, *pCount)] array of handle of components of
                                                    ///< this type.
                                                    ///< if count is less than the number of components of this type that are
                                                    ///< available, then the driver shall only retrieve that number of
                                                    ///< component handles.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnEnumMemoryModules_t pfnEnumMemoryModules = (ctl_pfnEnumMemoryModules_t)GetProcAddress(hinstLib, "ctlEnumMemoryModules");
        if (pfnEnumMemoryModules)
        {
            result = pfnEnumMemoryModules(hDAhandle, pCount, phMemory);
        }
    }


    return result;
}

/**
* @brief Get memory properties
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hMemory`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pProperties`
*/
ctl_result_t CTL_APICALL
ctlMemoryGetProperties(
    ctl_mem_handle_t hMemory,                       ///< [in] Handle for the component.
    ctl_mem_properties_t* pProperties               ///< [in,out] Will contain memory properties.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnMemoryGetProperties_t pfnMemoryGetProperties = (ctl_pfnMemoryGetProperties_t)GetProcAddress(hinstLib, "ctlMemoryGetProperties");
        if (pfnMemoryGetProperties)
        {
            result = pfnMemoryGetProperties(hMemory, pProperties);
        }
    }


    return result;
}

/**
* @brief Get memory state - health, allocated
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hMemory`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pState`
*/
ctl_result_t CTL_APICALL
ctlMemoryGetState(
    ctl_mem_handle_t hMemory,                       ///< [in] Handle for the component.
    ctl_mem_state_t* pState                         ///< [in,out] Will contain the current health and allocated memory.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnMemoryGetState_t pfnMemoryGetState = (ctl_pfnMemoryGetState_t)GetProcAddress(hinstLib, "ctlMemoryGetState");
        if (pfnMemoryGetState)
        {
            result = pfnMemoryGetState(hMemory, pState);
        }
    }


    return result;
}

/**
* @brief Get memory bandwidth
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hMemory`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pBandwidth`
*     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS
*         + User does not have permissions to query this telemetry.
*/
ctl_result_t CTL_APICALL
ctlMemoryGetBandwidth(
    ctl_mem_handle_t hMemory,                       ///< [in] Handle for the component.
    ctl_mem_bandwidth_t* pBandwidth                 ///< [in,out] Will contain the current health, free memory, total memory
                                                    ///< size.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnMemoryGetBandwidth_t pfnMemoryGetBandwidth = (ctl_pfnMemoryGetBandwidth_t)GetProcAddress(hinstLib, "ctlMemoryGetBandwidth");
        if (pfnMemoryGetBandwidth)
        {
            result = pfnMemoryGetBandwidth(hMemory, pBandwidth);
        }
    }


    return result;
}

/**
* @brief Get PCI properties - address, max speed
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pProperties`
*/
ctl_result_t CTL_APICALL
ctlPciGetProperties(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    ctl_pci_properties_t* pProperties               ///< [in,out] Will contain the PCI properties.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnPciGetProperties_t pfnPciGetProperties = (ctl_pfnPciGetProperties_t)GetProcAddress(hinstLib, "ctlPciGetProperties");
        if (pfnPciGetProperties)
        {
            result = pfnPciGetProperties(hDAhandle, pProperties);
        }
    }


    return result;
}

/**
* @brief Get current PCI state - current speed
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pState`
*/
ctl_result_t CTL_APICALL
ctlPciGetState(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    ctl_pci_state_t* pState                         ///< [in,out] Will contain the PCI properties.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnPciGetState_t pfnPciGetState = (ctl_pfnPciGetState_t)GetProcAddress(hinstLib, "ctlPciGetState");
        if (pfnPciGetState)
        {
            result = pfnPciGetState(hDAhandle, pState);
        }
    }


    return result;
}

/**
* @brief Get handle of power domains
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pCount`
*/
ctl_result_t CTL_APICALL
ctlEnumPowerDomains(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    uint32_t* pCount,                               ///< [in,out] pointer to the number of components of this type.
                                                    ///< if count is zero, then the driver shall update the value with the
                                                    ///< total number of components of this type that are available.
                                                    ///< if count is greater than the number of components of this type that
                                                    ///< are available, then the driver shall update the value with the correct
                                                    ///< number of components.
    ctl_pwr_handle_t* phPower                       ///< [in,out][optional][range(0, *pCount)] array of handle of components of
                                                    ///< this type.
                                                    ///< if count is less than the number of components of this type that are
                                                    ///< available, then the driver shall only retrieve that number of
                                                    ///< component handles.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnEnumPowerDomains_t pfnEnumPowerDomains = (ctl_pfnEnumPowerDomains_t)GetProcAddress(hinstLib, "ctlEnumPowerDomains");
        if (pfnEnumPowerDomains)
        {
            result = pfnEnumPowerDomains(hDAhandle, pCount, phPower);
        }
    }


    return result;
}

/**
* @brief Get properties related to a power domain
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hPower`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pProperties`
*/
ctl_result_t CTL_APICALL
ctlPowerGetProperties(
    ctl_pwr_handle_t hPower,                        ///< [in] Handle for the component.
    ctl_power_properties_t* pProperties             ///< [in,out] Structure that will contain property data.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnPowerGetProperties_t pfnPowerGetProperties = (ctl_pfnPowerGetProperties_t)GetProcAddress(hinstLib, "ctlPowerGetProperties");
        if (pfnPowerGetProperties)
        {
            result = pfnPowerGetProperties(hPower, pProperties);
        }
    }


    return result;
}

/**
* @brief Get energy counter
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hPower`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pEnergy`
*/
ctl_result_t CTL_APICALL
ctlPowerGetEnergyCounter(
    ctl_pwr_handle_t hPower,                        ///< [in] Handle for the component.
    ctl_power_energy_counter_t* pEnergy             ///< [in,out] Will contain the latest snapshot of the energy counter and
                                                    ///< timestamp when the last counter value was measured.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnPowerGetEnergyCounter_t pfnPowerGetEnergyCounter = (ctl_pfnPowerGetEnergyCounter_t)GetProcAddress(hinstLib, "ctlPowerGetEnergyCounter");
        if (pfnPowerGetEnergyCounter)
        {
            result = pfnPowerGetEnergyCounter(hPower, pEnergy);
        }
    }


    return result;
}

/**
* @brief Get power limits
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hPower`
*/
ctl_result_t CTL_APICALL
ctlPowerGetLimits(
    ctl_pwr_handle_t hPower,                        ///< [in] Handle for the component.
    ctl_power_sustained_limit_t* pSustained,        ///< [in,out][optional] The sustained power limit. If this is null, the
                                                    ///< current sustained power limits will not be returned.
    ctl_power_burst_limit_t* pBurst,                ///< [in,out][optional] The burst power limit. If this is null, the current
                                                    ///< peak power limits will not be returned.
    ctl_power_peak_limit_t* pPeak                   ///< [in,out][optional] The peak power limit. If this is null, the peak
                                                    ///< power limits will not be returned.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnPowerGetLimits_t pfnPowerGetLimits = (ctl_pfnPowerGetLimits_t)GetProcAddress(hinstLib, "ctlPowerGetLimits");
        if (pfnPowerGetLimits)
        {
            result = pfnPowerGetLimits(hPower, pSustained, pBurst, pPeak);
        }
    }


    return result;
}

/**
* @brief Set power limits
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hPower`
*     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS
*         + User does not have permissions to make these modifications.
*     - ::CTL_RESULT_ERROR_NOT_AVAILABLE
*         + The device is in use, meaning that the GPU is under Over clocking, applying power limits under overclocking is not supported.
*/
ctl_result_t CTL_APICALL
ctlPowerSetLimits(
    ctl_pwr_handle_t hPower,                        ///< [in] Handle for the component.
    const ctl_power_sustained_limit_t* pSustained,  ///< [in][optional] The sustained power limit. If this is null, no changes
                                                    ///< will be made to the sustained power limits.
    const ctl_power_burst_limit_t* pBurst,          ///< [in][optional] The burst power limit. If this is null, no changes will
                                                    ///< be made to the burst power limits.
    const ctl_power_peak_limit_t* pPeak             ///< [in][optional] The peak power limit. If this is null, no changes will
                                                    ///< be made to the peak power limits.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnPowerSetLimits_t pfnPowerSetLimits = (ctl_pfnPowerSetLimits_t)GetProcAddress(hinstLib, "ctlPowerSetLimits");
        if (pfnPowerSetLimits)
        {
            result = pfnPowerSetLimits(hPower, pSustained, pBurst, pPeak);
        }
    }


    return result;
}

/**
* @brief Get handle of temperature sensors
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hDAhandle`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pCount`
*/
ctl_result_t CTL_APICALL
ctlEnumTemperatureSensors(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    uint32_t* pCount,                               ///< [in,out] pointer to the number of components of this type.
                                                    ///< if count is zero, then the driver shall update the value with the
                                                    ///< total number of components of this type that are available.
                                                    ///< if count is greater than the number of components of this type that
                                                    ///< are available, then the driver shall update the value with the correct
                                                    ///< number of components.
    ctl_temp_handle_t* phTemperature                ///< [in,out][optional][range(0, *pCount)] array of handle of components of
                                                    ///< this type.
                                                    ///< if count is less than the number of components of this type that are
                                                    ///< available, then the driver shall only retrieve that number of
                                                    ///< component handles.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnEnumTemperatureSensors_t pfnEnumTemperatureSensors = (ctl_pfnEnumTemperatureSensors_t)GetProcAddress(hinstLib, "ctlEnumTemperatureSensors");
        if (pfnEnumTemperatureSensors)
        {
            result = pfnEnumTemperatureSensors(hDAhandle, pCount, phTemperature);
        }
    }


    return result;
}

/**
* @brief Get temperature sensor properties
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hTemperature`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pProperties`
*/
ctl_result_t CTL_APICALL
ctlTemperatureGetProperties(
    ctl_temp_handle_t hTemperature,                 ///< [in] Handle for the component.
    ctl_temp_properties_t* pProperties              ///< [in,out] Will contain the temperature sensor properties.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnTemperatureGetProperties_t pfnTemperatureGetProperties = (ctl_pfnTemperatureGetProperties_t)GetProcAddress(hinstLib, "ctlTemperatureGetProperties");
        if (pfnTemperatureGetProperties)
        {
            result = pfnTemperatureGetProperties(hTemperature, pProperties);
        }
    }


    return result;
}

/**
* @brief Get the temperature from a specified sensor
* 
* @details
*     - The application may call this function from simultaneous threads.
*     - The implementation of this function should be lock-free.
* 
* @returns
*     - ::CTL_RESULT_SUCCESS
*     - ::CTL_RESULT_ERROR_UNINITIALIZED
*     - ::CTL_RESULT_ERROR_DEVICE_LOST
*     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
*         + `nullptr == hTemperature`
*     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
*         + `nullptr == pTemperature`
*/
ctl_result_t CTL_APICALL
ctlTemperatureGetState(
    ctl_temp_handle_t hTemperature,                 ///< [in] Handle for the component.
    double* pTemperature                            ///< [in,out] Will contain the temperature read from the specified sensor
                                                    ///< in degrees Celsius.
    )
{
    ctl_result_t result = CTL_RESULT_ERROR_NOT_INITIALIZED;
    

    if (hinstLib != NULL)
    {
        ctl_pfnTemperatureGetState_t pfnTemperatureGetState = (ctl_pfnTemperatureGetState_t)GetProcAddress(hinstLib, "ctlTemperatureGetState");
        if (pfnTemperatureGetState)
        {
            result = pfnTemperatureGetState(hTemperature, pTemperature);
        }
    }


    return result;
}


//
// End of wrapper function implementation
//
/////////////////////////////////////////////////////////////////////////////////
