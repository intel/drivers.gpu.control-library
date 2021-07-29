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
 * @file igcl_api.h
 * @version v0-r7
 *
 */
#ifndef _CTL_API_H
#define _CTL_API_H
#if defined(__cplusplus)
#pragma once
#endif

// standard headers
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

// Intel 'ctlApi' common types
#if !defined(__GNUC__)
#pragma region common
#endif
///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_MAKE_VERSION
/// @brief Generates generic ::'ctlApi' API versions
#define CTL_MAKE_VERSION( _major, _minor )  (( _major << 16 )|( _minor & 0x0000ffff))
#endif // CTL_MAKE_VERSION

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_MAJOR_VERSION
/// @brief Extracts ::'ctlApi' API major version
#define CTL_MAJOR_VERSION( _ver )  ( _ver >> 16 )
#endif // CTL_MAJOR_VERSION

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_MINOR_VERSION
/// @brief Extracts ::'ctlApi' API minor version
#define CTL_MINOR_VERSION( _ver )  ( _ver & 0x0000ffff )
#endif // CTL_MINOR_VERSION

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_IMPL_MAJOR_VERSION
/// @brief ::'ctlApi' API major version of this implementation
#define CTL_IMPL_MAJOR_VERSION  1
#endif // CTL_IMPL_MAJOR_VERSION

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_IMPL_MINOR_VERSION
/// @brief ::'ctlApi' API minor version of this implementation
#define CTL_IMPL_MINOR_VERSION  0
#endif // CTL_IMPL_MINOR_VERSION

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_IMPL_VERSION
/// @brief ::'ctlApi' API version of this implementation
#define CTL_IMPL_VERSION  CTL_MAKE_VERSION( CTL_IMPL_MAJOR_VERSION, CTL_IMPL_MINOR_VERSION )
#endif // CTL_IMPL_VERSION

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_APICALL
#if defined(_WIN32)
/// @brief Calling convention for all API functions
#define CTL_APICALL  __cdecl
#else
#define CTL_APICALL  
#endif // defined(_WIN32)
#endif // CTL_APICALL

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_APIEXPORT
#if defined(_WIN32)
/// @brief Microsoft-specific dllexport storage-class attribute
#define CTL_APIEXPORT  __declspec(dllexport)
#else
#define CTL_APIEXPORT  
#endif // defined(_WIN32)
#endif // CTL_APIEXPORT

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_DLLEXPORT
#if defined(_WIN32)
/// @brief Microsoft-specific dllexport storage-class attribute
#define CTL_DLLEXPORT  __declspec(dllexport)
#endif // defined(_WIN32)
#endif // CTL_DLLEXPORT

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_DLLEXPORT
#if __GNUC__ >= 4
/// @brief GCC-specific dllexport storage-class attribute
#define CTL_DLLEXPORT  __attribute__ ((visibility ("default")))
#else
#define CTL_DLLEXPORT  
#endif // __GNUC__ >= 4
#endif // CTL_DLLEXPORT

///////////////////////////////////////////////////////////////////////////////
/// @brief Supported initialization flags
typedef uint32_t ctl_init_flags_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Supported initialization flags
typedef uint32_t ctl_version_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Handle of a control API instance
typedef struct _ctl_api_handle_t *ctl_api_handle_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Handle of a device adapter instance
typedef struct _ctl_device_adapter_handle_t *ctl_device_adapter_handle_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Handle of a device temperature sensor
typedef struct _ctl_temp_handle_t *ctl_temp_handle_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Handle for a device frequency domain
typedef struct _ctl_freq_handle_t *ctl_freq_handle_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Handle of a power device.
typedef struct _ctl_pwr_handle_t *ctl_pwr_handle_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Handle of a device fan
typedef struct _ctl_fan_handle_t *ctl_fan_handle_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Handle of a device memory module
typedef struct _ctl_mem_handle_t *ctl_mem_handle_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Handle of a device engine group
typedef struct _ctl_engine_handle_t *ctl_engine_handle_t;

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_BIT
/// @brief Generic macro for enumerator bit masks
#define CTL_BIT( _i )  ( 1 << _i )
#endif // CTL_BIT

///////////////////////////////////////////////////////////////////////////////
/// @brief Base for all properties types
typedef struct _ctl_base_interface_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure

} ctl_base_interface_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Value type
typedef enum _ctl_property_value_type_t
{
    CTL_PROPERTY_VALUE_TYPE_BOOL = 0,               ///< Boolean
    CTL_PROPERTY_VALUE_TYPE_FLOAT = 1,              ///< Float
    CTL_PROPERTY_VALUE_TYPE_INT32 = 2,              ///< Int32
    CTL_PROPERTY_VALUE_TYPE_UINT32 = 3,             ///< Unsigned Int32
    CTL_PROPERTY_VALUE_TYPE_ENUM = 4,               ///< Enum
    CTL_PROPERTY_VALUE_TYPE_CUSTOM = 5,             ///< Custom argument
    CTL_PROPERTY_VALUE_TYPE_MAX

} ctl_property_value_type_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Property range details, a generic struct to hold min/max/step size
///        information of various feature properties
typedef struct _ctl_property_range_info_t
{
    float min_possible_value;                       ///< [out] Minimum possible value
    float max_possible_value;                       ///< [out] Maximum possible value
    float step_size;                                ///< [out] Step size possible
    float default_value;                            ///< [out] Default value

} ctl_property_range_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Property range details of integer type, a generic struct to hold
///        min/max/step size information of various feature properties
typedef struct _ctl_property_range_info_int_t
{
    int32_t min_possible_value;                     ///< [out] Minimum possible value
    int32_t max_possible_value;                     ///< [out] Maximum possible value
    int32_t step_size;                              ///< [out] Step size possible
    int32_t default_value;                          ///< [out] Default value

} ctl_property_range_info_int_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Property range details of unsigned integer type, a generic struct to
///        hold min/max/step size information of various feature properties
typedef struct _ctl_property_range_info_uint_t
{
    uint32_t min_possible_value;                    ///< [out] Minimum possible value
    uint32_t max_possible_value;                    ///< [out] Maximum possible value
    uint32_t step_size;                             ///< [out] Step size possible
    uint32_t default_value;                         ///< [out] Default value

} ctl_property_range_info_uint_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Bool feature details
typedef struct _ctl_property_info_boolean_t
{
    bool DefaultState;                              ///< [out] Default state

} ctl_property_info_boolean_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Bool feature for get/set
typedef struct _ctl_property_boolean_t
{
    bool Enable;                                    ///< [in,out] Enable

} ctl_property_boolean_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Enumeration feature details
typedef struct _ctl_property_info_enum_t
{
    uint64_t SupportedTypes;                        ///< [out] Supported possible values represented as a bitmask
    uint32_t DefaultType;                           ///< [out] Default type

} ctl_property_info_enum_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Enumeration feature for get/set
typedef struct _ctl_property_enum_t
{
    uint32_t EnableType;                            ///< [in,out] Enable with specific type

} ctl_property_enum_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Float feature details
typedef struct _ctl_property_info_float_t
{
    bool DefaultEnable;                             ///< [in,out] DefaultEnable
    ctl_property_range_info_t RangeInfo;            ///< [out] Min/max/default/step details

} ctl_property_info_float_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Float feature for get/set
typedef struct _ctl_property_float_t
{
    bool Enable;                                    ///< [in,out] Enable
    float Value;                                    ///< [in,out] Value

} ctl_property_float_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Int32 feature details
typedef struct _ctl_property_info_int_t
{
    bool DefaultEnable;                             ///< [in,out] DefaultEnable
    ctl_property_range_info_int_t RangeInfo;        ///< [out] Min/max/default/step details

} ctl_property_info_int_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Int32 feature for get/set
typedef struct _ctl_property_int_t
{
    bool Enable;                                    ///< [in,out] Enable
    int32_t Value;                                  ///< [in,out] Value

} ctl_property_int_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Int32 feature details
typedef struct _ctl_property_info_uint_t
{
    bool DefaultEnable;                             ///< [in,out] DefaultEnable
    ctl_property_range_info_uint_t RangeInfo;       ///< [out] Min/max/default/step details

} ctl_property_info_uint_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Int32 feature for get/set
typedef struct _ctl_property_uint_t
{
    bool Enable;                                    ///< [in,out] Enable
    uint32_t Value;                                 ///< [in,out] Value

} ctl_property_uint_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Feature element details, union of bool/float/enum property_info
///        structs. Used for feature specific capability check
typedef union _ctl_property_info_t
{
    ctl_property_info_boolean_t BoolType;           ///< [in,out] Boolean type fields
    ctl_property_info_float_t FloatType;            ///< [in,out] Float type fields
    ctl_property_info_int_t IntType;                ///< [in,out] Int type fields
    ctl_property_info_enum_t EnumType;              ///< [in,out] Enum type fields
    ctl_property_info_uint_t UIntType;              ///< [in,out] Unsigned Int type fields

} ctl_property_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Feature element details, union of bool/float/enum property structs.
///        Used for get/set calls
typedef union _ctl_property_t
{
    ctl_property_boolean_t BoolType;                ///< [in,out] Boolean type fields
    ctl_property_float_t FloatType;                 ///< [in,out] Float type fields
    ctl_property_int_t IntType;                     ///< [in,out] Int type fields
    ctl_property_enum_t EnumType;                   ///< [in,out] Enum type fields
    ctl_property_uint_t UIntType;                   ///< [in,out] Unsigned Int type fields

} ctl_property_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Defines Return/Error codes. Bit30 indicates error.
///        All generic error codes are between 0x40000000-0x4000FFFF.
///        All 3D specific ones are between 0x60000000-0x6000FFFF.
///        All media specific ones are between 0x50000000-0x5000FFFF.
///        All display specific ones are between 0x48000000-0x4800FFFF
typedef enum _ctl_result_t
{
    CTL_RESULT_SUCCESS = 0,                         ///< "success
                                                    ///< "
    CTL_RESULT_ERROR_GENERIC_START = 0x40000000,    ///< Generic error code starting value, not to be used
    CTL_RESULT_ERROR_NOT_INITIALIZED = 0x40000001,  ///< Result not initialized
    CTL_RESULT_ERROR_ALREADY_INITIALIZED = 0x40000002,  ///< Already initialized
    CTL_RESULT_ERROR_DEVICE_LOST = 0x40000003,      ///< Device hung, reset, was removed, or driver update occurred
    CTL_RESULT_ERROR_OUT_OF_HOST_MEMORY = 0x40000004,   ///< Insufficient host memory to satisfy call
    CTL_RESULT_ERROR_OUT_OF_DEVICE_MEMORY = 0x40000005, ///< Insufficient device memory to satisfy call
    CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS = 0x40000006, ///< Access denied due to permission level
    CTL_RESULT_ERROR_NOT_AVAILABLE = 0x40000007,    ///< Resource was removed
    CTL_RESULT_ERROR_UNINITIALIZED = 0x40000008,    ///< Library not initialized
    CTL_RESULT_ERROR_UNSUPPORTED_VERSION = 0x40000009,  ///< Generic error code for unsupported versions
    CTL_RESULT_ERROR_UNSUPPORTED_FEATURE = 0x4000000a,  ///< Generic error code for unsupported features
    CTL_RESULT_ERROR_INVALID_ARGUMENT = 0x4000000b, ///< Generic error code for invalid arguments
    CTL_RESULT_ERROR_INVALID_API_HANDLE = 0x4000000c,   ///< API handle in invalid
    CTL_RESULT_ERROR_INVALID_NULL_HANDLE = 0x4000000d,  ///< Handle argument is not valid
    CTL_RESULT_ERROR_INVALID_NULL_POINTER = 0x4000000e, ///< Pointer argument may not be nullptr
    CTL_RESULT_ERROR_INVALID_SIZE = 0x4000000f,     ///< Size argument is invalid (e.g., must not be zero)
    CTL_RESULT_ERROR_UNSUPPORTED_SIZE = 0x40000010, ///< Size argument is not supported by the device (e.g., too large)
    CTL_RESULT_ERROR_UNSUPPORTED_IMAGE_FORMAT = 0x40000011, ///< Image format is not supported by the device
    CTL_RESULT_ERROR_DATA_READ = 0x40000012,        ///< Data read error
    CTL_RESULT_ERROR_DATA_WRITE = 0x40000013,       ///< Data write error
    CTL_RESULT_ERROR_DATA_NOT_FOUND = 0x40000014,   ///< Data not found error
    CTL_RESULT_ERROR_NOT_IMPLEMENTED = 0x40000015,  ///< Function not implemented
    CTL_RESULT_ERROR_OS_CALL = 0x40000016,          ///< Operating system call failure
    CTL_RESULT_ERROR_KMD_CALL = 0x40000017,         ///< Kernel mode driver call failure
    CTL_RESULT_ERROR_UNLOAD = 0x40000018,           ///< Library unload failure
    CTL_RESULT_ERROR_ZE_LOADER = 0x40000019,        ///< Level0 loader not found
    CTL_RESULT_ERROR_INVALID_OPERATION_TYPE = 0x4000001a,   ///< Invalid operation type
    CTL_RESULT_ERROR_NULL_OS_INTERFACE = 0x4000001b,///< Null OS interface
    CTL_RESULT_ERROR_NULL_OS_ADAPATER_HANDLE = 0x4000001c,  ///< Null OS adapter handle
    CTL_RESULT_ERROR_NULL_OS_DISPLAY_OUTPUT_HANDLE = 0x4000001d,///< Null display output handle
    CTL_RESULT_ERROR_WAIT_TIMEOUT = 0x4000001e,     ///< Timeout in Wait function
    CTL_RESULT_ERROR_PERSISTANCE_NOT_SUPPORTED = 0x4000001f,///< Persistance not supported
    CTL_RESULT_ERROR_PLATFORM_NOT_SUPPORTED = 0x40000020,   ///< Platform not supported
    CTL_RESULT_ERROR_UNKNOWN_APPLICATION_UID = 0x40000021,  ///< Unknown Appplicaion UID in Initialization call 
    CTL_RESULT_ERROR_INVALID_ENUMERATION = 0x40000022,  ///< The enum is not valid
    CTL_RESULT_ERROR_UNKNOWN = 0x4000FFFF,          ///< Unknown or internal error
    CTL_RESULT_ERROR_GENERIC_END = 0x4000FFFF,      ///< "Generic error code end value, not to be used
                                                    ///< "
    CTL_RESULT_ERROR_3D_START = 0x60000000,         ///< 3D error code starting value, not to be used
    CTL_RESULT_ERROR_3D_END = 0x6000FFFF,           ///< "3D error code end value, not to be used
                                                    ///< "
    CTL_RESULT_ERROR_MEDIA_START = 0x50000000,      ///< Media error code starting value, not to be used
    CTL_RESULT_ERROR_MEDIA_END = 0x5000FFFF,        ///< "Media error code end value, not to be used
                                                    ///< "
    CTL_RESULT_ERROR_DISPLAY_START = 0x48000000,    ///< Display error code starting value, not to be used
    CTL_RESULT_ERROR_INVALID_AUX_ACCESS_FLAG = 0x48000001,  ///< Invalid flag for Aux access
    CTL_RESULT_ERROR_INVALID_SHARPNESS_FILTER_FLAG = 0x48000002,///< Invalid flag for Sharpness
    CTL_RESULT_ERROR_DISPLAY_NOT_ATTACHED = 0x48000003, ///< Error for Display not attached
    CTL_RESULT_ERROR_DISPLAY_NOT_ACTIVE = 0x48000004,   ///< Error for display attached but not active
    CTL_RESULT_ERROR_INVALID_POWERFEATURE_OPTIMIZATION_FLAG = 0x48000005,   ///< Error for invalid power optimization flag
    CTL_RESULT_ERROR_INVALID_POWERSOURCE_TYPE_FOR_DPST = 0x48000006,///< DPST is supported only in DC Mode
    CTL_RESULT_ERROR_INVALID_PIXTX_GET_CONFIG_QUERY_TYPE = 0x48000007,  ///< Invalid query type for pixel transformation get configuration
    CTL_RESULT_ERROR_INVALID_PIXTX_SET_CONFIG_OPERATION_TYPE = 0x48000008,  ///< Invalid operation type for pixel transformation set configuration
    CTL_RESULT_ERROR_INVALID_SET_CONFIG_NUMBER_OF_SAMPLES = 0x48000009, ///< Invalid number of samples for pixel transformation set configuration
    CTL_RESULT_ERROR_INVALID_PIXTX_BLOCK_ID = 0x4800000a,   ///< Invalid block id for pixel transformation
    CTL_RESULT_ERROR_INSUFFICIENT_PIXTX_BLOCK_CONFIG_MEMORY = 0x4800000b,   ///< Insufficient memery allocated for BlockConfigs
    CTL_RESULT_ERROR_DISPLAY_END = 0x4800FFFF,      ///< "Display error code end value, not to be used
                                                    ///< "
    CTL_RESULT_MAX

} ctl_result_t;

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_MAX_DEVICE_NAME_LEN
/// @brief Maximum IPC handle size
#define CTL_MAX_DEVICE_NAME_LEN  100
#endif // CTL_MAX_DEVICE_NAME_LEN

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_MAX_RESERVED_SIZE
/// @brief Maximum reserved size for future members.
#define CTL_MAX_RESERVED_SIZE  128
#endif // CTL_MAX_RESERVED_SIZE

///////////////////////////////////////////////////////////////////////////////
/// @brief Base for all properties types
typedef struct _ctl_base_properties_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure

} ctl_base_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Application Unique ID
typedef struct _ctl_application_id_t
{
    uint32_t Data1;                                 ///< [in] Data1
    uint16_t Data2;                                 ///< [in] Data2
    uint16_t Data3;                                 ///< [in] Data3
    uint8_t Data4[8];                               ///< [in] Data4

} ctl_application_id_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Init arguments
typedef struct _ctl_init_args_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_version_info_t AppVersion;                  ///< [in][release] App's IGCL version
    ctl_init_flags_t flags;                         ///< [in][release] Initialization flags
    ctl_version_info_t SupportedVersion;            ///< [out][release] IGCL implementation version
    ctl_application_id_t ApplicationUID;            ///< [in] Application Provided Unique ID.Application can pass all 0's as
                                                    ///< the default ID

} ctl_init_args_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Reserved struct
typedef struct _ctl_reserved_args_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    void* pSpecialArg;                              ///< [in] Reserved struct
    uint32_t ArgSize;                               ///< [in] struct size

} ctl_reserved_args_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Control Api Init
/// 
/// @details
///     - Control Api Init
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pInitDesc`
///         + `nullptr == phAPIHandle`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlInit(
    ctl_init_args_t* pInitDesc,                     ///< [in][out] App's control API version
    ctl_api_handle_t* phAPIHandle                   ///< [in][out][release] Control API handle
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Control Api Destroy
/// 
/// @details
///     - Control Api Close
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hAPIHandle`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlClose(
    ctl_api_handle_t hAPIHandle                     ///< [in][release] Control API implementation handle obtained during init
                                                    ///< call
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Supported Functions
typedef uint32_t ctl_supported_functions_flags_t;
typedef enum _ctl_supported_functions_flag_t
{
    CTL_SUPPORTED_FUNCTIONS_FLAG_DISPLAY = CTL_BIT(0),  ///< [out] Is Display supported
    CTL_SUPPORTED_FUNCTIONS_FLAG_3D = CTL_BIT(1),   ///< [out] Is 3D supported
    CTL_SUPPORTED_FUNCTIONS_FLAG_MEDIA = CTL_BIT(2),///< [out] Is Media supported
    CTL_SUPPORTED_FUNCTIONS_FLAG_MAX = 0x80000000

} ctl_supported_functions_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Firmware version
typedef struct _ctl_firmware_version_t
{
    uint64_t major_version;                         ///< [out] Major version
    uint64_t minor_version;                         ///< [out] Minor version
    uint64_t build_number;                          ///< [out] Build number

} ctl_firmware_version_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief DeviceType
typedef enum _ctl_device_type_t
{
    CTL_DEVICE_TYPE_GRAPHICS = 1,                   ///< Graphics Device type
    CTL_DEVICE_TYPE_SYSTEM = 2,                     ///< System Device type
    CTL_DEVICE_TYPE_MAX

} ctl_device_type_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Device Adapter properties
typedef struct _ctl_device_adapter_properties_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    void* pDeviceID;                                ///< [in,out] OS specific Device ID
    uint32_t device_id_size;                        ///< [in] size of the device ID
    ctl_device_type_t device_type;                  ///< [out] Device Type
    ctl_supported_functions_flags_t supported_subfunction_flags;///< [out] Supported functions
    uint64_t driver_version;                        ///< [out] Driver version
    ctl_firmware_version_t firmware_version;        ///< [out] Firmware version
    uint32_t pci_vendor_id;                         ///< [out] PCI Vendor ID
    uint32_t pci_device_id;                         ///< [out] PCI Device ID
    uint32_t rev_id;                                ///< [out] PCI Revision ID
    uint32_t num_eus_per_sub_slice;                 ///< [out] Number of EUs per sub-slice
    uint32_t num_sub_slices_per_slice;              ///< [out] Number of sub-slices per slice
    uint32_t num_slices;                            ///< [out] Number of slices
    char name[CTL_MAX_DEVICE_NAME_LEN];             ///< [out] Device name
    char reserved[CTL_MAX_RESERVED_SIZE];           ///< [out] Reserved

} ctl_device_adapter_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief OperationType
typedef enum _ctl_operation_type_t
{
    CTL_OPERATION_TYPE_READ = 1,                    ///< Read operation
    CTL_OPERATION_TYPE_WRITE = 2,                   ///< Write operation
    CTL_OPERATION_TYPE_MAX

} ctl_operation_type_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Generic Structure for Void* datatypes
typedef struct _ctl_generic_void_datatype_t
{
    void* pData;                                    ///< [in,out]void pointer to memory
    uint32_t size;                                  ///< [in,out]size of the allocated memory

} ctl_generic_void_datatype_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Generic Structure for Revision datatypes
typedef struct _ctl_revision_datatype_t
{
    uint8_t major_version;                          ///< [in,out]Major Version
    uint8_t minor_version;                          ///< [in,out]Minor Version
    uint8_t revision_version;                       ///< [in,out]Revision Version

} ctl_revision_datatype_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Property Type flags
typedef uint32_t ctl_property_type_flags_t;
typedef enum _ctl_property_type_flag_t
{
    CTL_PROPERTY_TYPE_FLAG_DISPLAY = CTL_BIT(0),    ///< Display type. Supported scenarios: Sharpness/gamma/CSC
    CTL_PROPERTY_TYPE_FLAG_3D = CTL_BIT(1),         ///< 3D type. Supported scenarios: All set calls via IGCL's 3D APIs
    CTL_PROPERTY_TYPE_FLAG_MEDIA = CTL_BIT(2),      ///< Media type. Supported scenarios: All set calls via IGCL's media APIs
    CTL_PROPERTY_TYPE_FLAG_CORE = CTL_BIT(3),       ///< For future: Core graphic event types like clocking, frequency etc.
    CTL_PROPERTY_TYPE_FLAG_MAX = 0x80000000

} ctl_property_type_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Generic Structure for Revision datatypes
typedef struct _ctl_wait_property_change_args_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_property_type_flags_t PropertyType;         ///< [in] Type of the property
    uint32_t TimeOutMilliSec;                       ///< [in][release] Time-out interval in milliseconds. Specify 0xFFFFFFFF if
                                                    ///< time-out is not desired
    uint32_t EventMiscFlags;                        ///< [in][release] Event flags for future use
    void* pReserved;                                ///< [in][release] Reserved for future use
    uint64_t ReservedOutFlags;                      ///< [out] Reserved out argument for future use

} ctl_wait_property_change_args_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Wait for a property change. Note that this is a blocking call
/// 
/// @details
///     - Wait for a property change in display, 3d, media etc.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDeviceAdapter`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pArgs`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlWaitForPropertyChange(
    ctl_device_adapter_handle_t hDeviceAdapter,     ///< [in][release] handle to control device adapter
    ctl_wait_property_change_args_t* pArgs          ///< [in] Argument containing information about which property changes to
                                                    ///< listen for
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Reserved function
/// 
/// @details
///     - Reserved function
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDeviceAdapter`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pArgs`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlReservedCall(
    ctl_device_adapter_handle_t hDeviceAdapter,     ///< [in][release] handle to control device adapter
    ctl_reserved_args_t* pArgs                      ///< [in] Argument containing information
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_base_interface_t
typedef struct _ctl_base_interface_t ctl_base_interface_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_range_info_t
typedef struct _ctl_property_range_info_t ctl_property_range_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_range_info_int_t
typedef struct _ctl_property_range_info_int_t ctl_property_range_info_int_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_range_info_uint_t
typedef struct _ctl_property_range_info_uint_t ctl_property_range_info_uint_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_info_boolean_t
typedef struct _ctl_property_info_boolean_t ctl_property_info_boolean_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_boolean_t
typedef struct _ctl_property_boolean_t ctl_property_boolean_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_info_enum_t
typedef struct _ctl_property_info_enum_t ctl_property_info_enum_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_enum_t
typedef struct _ctl_property_enum_t ctl_property_enum_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_info_float_t
typedef struct _ctl_property_info_float_t ctl_property_info_float_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_float_t
typedef struct _ctl_property_float_t ctl_property_float_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_info_int_t
typedef struct _ctl_property_info_int_t ctl_property_info_int_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_int_t
typedef struct _ctl_property_int_t ctl_property_int_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_info_uint_t
typedef struct _ctl_property_info_uint_t ctl_property_info_uint_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_property_uint_t
typedef struct _ctl_property_uint_t ctl_property_uint_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_base_properties_t
typedef struct _ctl_base_properties_t ctl_base_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_application_id_t
typedef struct _ctl_application_id_t ctl_application_id_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_init_args_t
typedef struct _ctl_init_args_t ctl_init_args_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_reserved_args_t
typedef struct _ctl_reserved_args_t ctl_reserved_args_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_firmware_version_t
typedef struct _ctl_firmware_version_t ctl_firmware_version_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_device_adapter_properties_t
typedef struct _ctl_device_adapter_properties_t ctl_device_adapter_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_generic_void_datatype_t
typedef struct _ctl_generic_void_datatype_t ctl_generic_void_datatype_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_revision_datatype_t
typedef struct _ctl_revision_datatype_t ctl_revision_datatype_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_wait_property_change_args_t
typedef struct _ctl_wait_property_change_args_t ctl_wait_property_change_args_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_adaptivesync_caps_t
typedef struct _ctl_adaptivesync_caps_t ctl_adaptivesync_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_adaptivesync_getset_t
typedef struct _ctl_adaptivesync_getset_t ctl_adaptivesync_getset_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_3d_feature_details_t
typedef struct _ctl_3d_feature_details_t ctl_3d_feature_details_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_3d_feature_caps_t
typedef struct _ctl_3d_feature_caps_t ctl_3d_feature_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_3d_feature_getset_t
typedef struct _ctl_3d_feature_getset_t ctl_3d_feature_getset_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_display_timing_t
typedef struct _ctl_display_timing_t ctl_display_timing_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_display_properties_t
typedef struct _ctl_display_properties_t ctl_display_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_adapter_display_encoder_properties_t
typedef struct _ctl_adapter_display_encoder_properties_t ctl_adapter_display_encoder_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_sharpness_filter_properties_t
typedef struct _ctl_sharpness_filter_properties_t ctl_sharpness_filter_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_sharpness_caps_t
typedef struct _ctl_sharpness_caps_t ctl_sharpness_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_sharpness_settings_t
typedef struct _ctl_sharpness_settings_t ctl_sharpness_settings_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_i2c_access_args_t
typedef struct _ctl_i2c_access_args_t ctl_i2c_access_args_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_aux_access_args_t
typedef struct _ctl_aux_access_args_t ctl_aux_access_args_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_power_optimization_caps_t
typedef struct _ctl_power_optimization_caps_t ctl_power_optimization_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_power_optimization_lrr_t
typedef struct _ctl_power_optimization_lrr_t ctl_power_optimization_lrr_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_power_optimization_psr_t
typedef struct _ctl_power_optimization_psr_t ctl_power_optimization_psr_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_power_optimization_dpst_t
typedef struct _ctl_power_optimization_dpst_t ctl_power_optimization_dpst_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_power_optimization_settings_t
typedef struct _ctl_power_optimization_settings_t ctl_power_optimization_settings_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pixtx_color_primaries_t
typedef struct _ctl_pixtx_color_primaries_t ctl_pixtx_color_primaries_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pixtx_pixel_format_t
typedef struct _ctl_pixtx_pixel_format_t ctl_pixtx_pixel_format_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pixtx_1dlut_config_t
typedef struct _ctl_pixtx_1dlut_config_t ctl_pixtx_1dlut_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pixtx_matrix_config_t
typedef struct _ctl_pixtx_matrix_config_t ctl_pixtx_matrix_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pixtx_3dlut_sample_t
typedef struct _ctl_pixtx_3dlut_sample_t ctl_pixtx_3dlut_sample_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pixtx_3dlut_config_t
typedef struct _ctl_pixtx_3dlut_config_t ctl_pixtx_3dlut_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pixtx_block_config_t
typedef struct _ctl_pixtx_block_config_t ctl_pixtx_block_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pixtx_pipe_get_config_t
typedef struct _ctl_pixtx_pipe_get_config_t ctl_pixtx_pipe_get_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pixtx_pipe_set_config_t
typedef struct _ctl_pixtx_pipe_set_config_t ctl_pixtx_pipe_set_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_panel_descriptor_access_args_t
typedef struct _ctl_panel_descriptor_access_args_t ctl_panel_descriptor_access_args_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_retro_scaling_settings_t
typedef struct _ctl_retro_scaling_settings_t ctl_retro_scaling_settings_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_retro_scaling_caps_t
typedef struct _ctl_retro_scaling_caps_t ctl_retro_scaling_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_scaling_caps_t
typedef struct _ctl_scaling_caps_t ctl_scaling_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_scaling_settings_t
typedef struct _ctl_scaling_settings_t ctl_scaling_settings_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_engine_properties_t
typedef struct _ctl_engine_properties_t ctl_engine_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_engine_stats_t
typedef struct _ctl_engine_stats_t ctl_engine_stats_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_fan_speed_t
typedef struct _ctl_fan_speed_t ctl_fan_speed_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_fan_temp_speed_t
typedef struct _ctl_fan_temp_speed_t ctl_fan_temp_speed_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_fan_speed_table_t
typedef struct _ctl_fan_speed_table_t ctl_fan_speed_table_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_fan_properties_t
typedef struct _ctl_fan_properties_t ctl_fan_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_fan_config_t
typedef struct _ctl_fan_config_t ctl_fan_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_freq_properties_t
typedef struct _ctl_freq_properties_t ctl_freq_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_freq_range_t
typedef struct _ctl_freq_range_t ctl_freq_range_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_freq_state_t
typedef struct _ctl_freq_state_t ctl_freq_state_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_freq_throttle_time_t
typedef struct _ctl_freq_throttle_time_t ctl_freq_throttle_time_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_video_processing_noise_reduction_info_t
typedef struct _ctl_video_processing_noise_reduction_info_t ctl_video_processing_noise_reduction_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_video_processing_noise_reduction_t
typedef struct _ctl_video_processing_noise_reduction_t ctl_video_processing_noise_reduction_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_video_processing_adaptive_contrast_enhancement_info_t
typedef struct _ctl_video_processing_adaptive_contrast_enhancement_info_t ctl_video_processing_adaptive_contrast_enhancement_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_video_processing_adaptive_contrast_enhancement_t
typedef struct _ctl_video_processing_adaptive_contrast_enhancement_t ctl_video_processing_adaptive_contrast_enhancement_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_video_processing_standard_color_correction_info_t
typedef struct _ctl_video_processing_standard_color_correction_info_t ctl_video_processing_standard_color_correction_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_video_processing_standard_color_correction_t
typedef struct _ctl_video_processing_standard_color_correction_t ctl_video_processing_standard_color_correction_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_video_processing_total_color_correction_info_t
typedef struct _ctl_video_processing_total_color_correction_info_t ctl_video_processing_total_color_correction_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_video_processing_total_color_correction_t
typedef struct _ctl_video_processing_total_color_correction_t ctl_video_processing_total_color_correction_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_video_processing_feature_details_t
typedef struct _ctl_video_processing_feature_details_t ctl_video_processing_feature_details_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_video_processing_feature_caps_t
typedef struct _ctl_video_processing_feature_caps_t ctl_video_processing_feature_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_video_processing_feature_getset_t
typedef struct _ctl_video_processing_feature_getset_t ctl_video_processing_feature_getset_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_mem_properties_t
typedef struct _ctl_mem_properties_t ctl_mem_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_mem_state_t
typedef struct _ctl_mem_state_t ctl_mem_state_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_mem_bandwidth_t
typedef struct _ctl_mem_bandwidth_t ctl_mem_bandwidth_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pci_address_t
typedef struct _ctl_pci_address_t ctl_pci_address_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pci_speed_t
typedef struct _ctl_pci_speed_t ctl_pci_speed_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pci_properties_t
typedef struct _ctl_pci_properties_t ctl_pci_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_pci_state_t
typedef struct _ctl_pci_state_t ctl_pci_state_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_power_properties_t
typedef struct _ctl_power_properties_t ctl_power_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_power_energy_counter_t
typedef struct _ctl_power_energy_counter_t ctl_power_energy_counter_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_power_sustained_limit_t
typedef struct _ctl_power_sustained_limit_t ctl_power_sustained_limit_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_power_burst_limit_t
typedef struct _ctl_power_burst_limit_t ctl_power_burst_limit_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_power_peak_limit_t
typedef struct _ctl_power_peak_limit_t ctl_power_peak_limit_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_energy_threshold_t
typedef struct _ctl_energy_threshold_t ctl_energy_threshold_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare ctl_temp_properties_t
typedef struct _ctl_temp_properties_t ctl_temp_properties_t;



#if !defined(__GNUC__)
#pragma endregion // common
#endif
// Intel 'ctlApi' for Device Adapter
#if !defined(__GNUC__)
#pragma region 3D
#endif
///////////////////////////////////////////////////////////////////////////////
/// @brief Feature type
typedef enum _ctl_3d_feature_t
{
    CTL_3D_FEATURE_FRAME_PACING = 0,                ///< Frame pacing. Contains generic enum type fields
    CTL_3D_FEATURE_ENDURANCE_GAMING = 1,            ///< Endurance gaming. Contains generic integer type fields. Value will be
                                                    ///< interpreted as the max FPS to be used when in DC mode globally or per
                                                    ///< application
    CTL_3D_FEATURE_FRAME_LIMIT = 2,                 ///< Frame limit for games. Contains generic integer type fields. Value
                                                    ///< will be interpreted as the max FPS to be used independent of system
                                                    ///< power state
    CTL_3D_FEATURE_ANISOTROPIC = 3,                 ///< ANISOTROPIC. Contains generic enum type fields
    CTL_3D_FEATURE_CMAA = 4,                        ///< CMAA. Contains generic bool type fields
    CTL_3D_FEATURE_TEXTURE_FILTERING_QUALITY = 5,   ///< Texture filtering quality. Contains generic enum type fields
    CTL_3D_FEATURE_ADAPTIVE_TESSELLATION = 6,       ///< Adaptive tessellation quality. Contains generic integer type fields
    CTL_3D_FEATURE_SHARPENING_FILTER = 7,           ///< Sharpening Filter. Contains generic integer type fields
    CTL_3D_FEATURE_MSAA = 8,                        ///< Msaa. Contains generic enum type fields
    CTL_3D_FEATURE_ASYNC_FLIP_MODES = 9,            ///< Various async flips modes like speed frame, smooth sync & force aync
                                                    ///< flip. Contains generic enum type fields
    CTL_3D_FEATURE_ADAPTIVE_SYNC_PLUS = 10,         ///< Adaptive sync plus. Refer custom field ::ctl_adaptivesync_caps_t &
                                                    ///< ::ctl_adaptivesync_getset_t
    CTL_3D_FEATURE_MAX

} ctl_3d_feature_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Anisotropic values possible
typedef enum _ctl_3d_anisotropic_types_t
{
    CTL_3D_ANISOTROPIC_TYPES_APP_CHOICE = 0,        ///< Application choice
    CTL_3D_ANISOTROPIC_TYPES_2X = 2,                ///< 2X
    CTL_3D_ANISOTROPIC_TYPES_4X = 4,                ///< 4X
    CTL_3D_ANISOTROPIC_TYPES_8X = 8,                ///< 8X
    CTL_3D_ANISOTROPIC_TYPES_16X = 16,              ///< 16X
    CTL_3D_ANISOTROPIC_TYPES_MAX

} ctl_3d_anisotropic_types_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Texture filtering values possible
typedef enum _ctl_3d_texture_filtering_quality_types_t
{
    CTL_3D_TEXTURE_FILTERING_QUALITY_TYPES_PERFORMANCE = 0, ///< Performance
    CTL_3D_TEXTURE_FILTERING_QUALITY_TYPES_BALANCED = 1,///< Balanced
    CTL_3D_TEXTURE_FILTERING_QUALITY_TYPES_QUALITY = 2, ///< Quality
    CTL_3D_TEXTURE_FILTERING_QUALITY_TYPES_MAX

} ctl_3d_texture_filtering_quality_types_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Frame pacing values possible
typedef enum _ctl_3d_frame_pacing_types_t
{
    CTL_3D_FRAME_PACING_TYPES_DISABLE = 0,          ///< Disable
    CTL_3D_FRAME_PACING_TYPES_ENABLE_MODE_FRAME_NO_SMOOTHENING = 1, ///< Enable with scheduler without any frame smoothening
    CTL_3D_FRAME_PACING_TYPES_ENABLE_MODE_FRAME_MAX_SMOOTHENING = 2,///< Enable with scheduler with maximum smoothness
    CTL_3D_FRAME_PACING_TYPES_ENABLE_MODE_COMPETITIVE_GAMING = 3,   ///< Enable with scheduler in competitive gaming mode
    CTL_3D_FRAME_PACING_TYPES_MAX

} ctl_3d_frame_pacing_types_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Cmaa values possible
typedef enum _ctl_3d_cmaa_types_t
{
    CTL_3D_CMAA_TYPES_TURN_OFF = 0,                 ///< Turn off
    CTL_3D_CMAA_TYPES_ENHANCE_APPLICATION = 1,      ///< Enhance Application
    CTL_3D_CMAA_TYPES_OVERRIDE_MSAA = 2,            ///< Override MSAA
    CTL_3D_CMAA_TYPES_MAX

} ctl_3d_cmaa_types_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Adaptive Tessellation
typedef enum _ctl_3d_adaptive_tessellation_types_t
{
    CTL_3D_ADAPTIVE_TESSELLATION_TYPES_TURN_OFF = 0,///< Turn off
    CTL_3D_ADAPTIVE_TESSELLATION_TYPES_TURN_ON = 1, ///< Turn on
    CTL_3D_ADAPTIVE_TESSELLATION_TYPES_MAX

} ctl_3d_adaptive_tessellation_types_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Sharpening filter values possible
typedef enum _ctl_3d_sharpening_filter_types_t
{
    CTL_3D_SHARPENING_FILTER_TYPES_TURN_OFF = 0,    ///< Turn off
    CTL_3D_SHARPENING_FILTER_TYPES_TURN_ON = 1,     ///< Turn on
    CTL_3D_SHARPENING_FILTER_TYPES_MAX

} ctl_3d_sharpening_filter_types_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief MSAA values possible
typedef enum _ctl_3d_msaa_types_t
{
    CTL_3D_MSAA_TYPES_APP_CHOICE = 0,               ///< Application choice
    CTL_3D_MSAA_TYPES_DISABLED = 1,                 ///< Disabled. MSAA count 1
    CTL_3D_MSAA_TYPES_2X = 2,                       ///< 2X
    CTL_3D_MSAA_TYPES_4X = 4,                       ///< 4X
    CTL_3D_MSAA_TYPES_8X = 8,                       ///< 8X
    CTL_3D_MSAA_TYPES_16X = 16,                     ///< 16X
    CTL_3D_MSAA_TYPES_MAX

} ctl_3d_msaa_types_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Async flip modes
typedef enum _ctl_3d_frame_async_flip_types_t
{
    CTL_3D_FRAME_ASYNC_FLIP_TYPES_APP_CHOICE = 0,   ///< Application choice
    CTL_3D_FRAME_ASYNC_FLIP_TYPES_ENABLE_FORCE_SYNC = 1,///< Force async to sync flip conversion
    CTL_3D_FRAME_ASYNC_FLIP_TYPES_ENABLE_SPEED_FRAME = 2,   ///< Enable Intel speed frame technology
    CTL_3D_FRAME_ASYNC_FLIP_TYPES_ENABLE_SMOOTH_SYNC = 3,   ///< Enable Intel smooth sync technology
    CTL_3D_FRAME_ASYNC_FLIP_TYPES_MAX

} ctl_3d_frame_async_flip_types_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Adaptive sync plus caps
typedef struct _ctl_adaptivesync_caps_t
{
    bool AdaptiveBalanceSupported;                  ///< [out] Adaptive balance supported
    ctl_property_info_float_t AdaptiveBalanceStrengthCaps;  ///< [out] Strength of adaptive balance algorithm - min/max/steps/default

} ctl_adaptivesync_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Adaptive sync plus
typedef struct _ctl_adaptivesync_getset_t
{
    bool AdaptiveSync;                              ///< [in,out] Adaptive sync. Note that in Windows, OS controls state of
                                                    ///< adaptive sync and which game gets the feature using it's own policies
    bool AdaptiveBalance;                           ///< [in,out] Adaptive balance
    bool AllowAsyncForHighFPS;                      ///< [in,out] Allow async flips when FPS is higher than max refresh rate of
                                                    ///< the panel
    float AdaptiveBalanceStrength;                  ///< [in,out] Adaptive balance strength

} ctl_adaptivesync_getset_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief 3D feature capability details which will have range/supported and
///        default values
typedef struct _ctl_3d_feature_details_t
{
    ctl_3d_feature_t FeatureType;                   ///< [out] 3D feature type
    ctl_property_value_type_t ValueType;            ///< [out] Type of value
    ctl_property_info_t Value;                      ///< [out] Union of various type of values for 3D features. For enum types
                                                    ///< this can be anisotropic/frame pacing etc. This member is valid iff
                                                    ///< ValueType is not CTL_PROPERTY_VALUE_TYPE_CUSTOM
    int32_t CustomValueSize;                        ///< [in] CustomValue buffer size. Typically for a feature requiring custom
                                                    ///< struct, caller will know of it upfront and can provide the right size
                                                    ///< info here
    void* pCustomValue;                             ///< [in,out] Pointer to a custom structure. Caller should allocate this
                                                    ///< buffer with known custom feature structure size. This member is valid
                                                    ///< iff ValueType is CTL_PROPERTY_VALUE_TYPE_CUSTOM
    bool PerAppSupport;                             ///< [out] Flag indicating whether the feature is supported per application
                                                    ///< or not
    int64_t ConflictingFeatures;                    ///< [out] Mask of ::ctl_3d_feature_t values which can't be enabled along
                                                    ///< with the mentioned FeatureType. If this is 0, it meant the feature
                                                    ///< doesn't have any conflicts with other features
    int64_t Reserved;                               ///< [out] Reserved

} ctl_3d_feature_details_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief 3D feature which are controllable
typedef struct _ctl_3d_feature_caps_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint32_t NumSupportedFeatures;                  ///< [in,out] Number of elements in supported features array
    ctl_3d_feature_details_t* pFeatureDetails;      ///< [in,out] Array of feature details

} ctl_3d_feature_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief 3D feature for get/set
typedef struct _ctl_3d_feature_getset_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_3d_feature_t FeatureType;                   ///< [in] Features interested in
    char* ApplicationName;                          ///< [in] Application name for which the property type is applicable. If
                                                    ///< this is an empty string then this will get/set global settings for the
                                                    ///< given adapter. Note that this should contain only the name of the
                                                    ///< application and not the system specific path
    int8_t ApplicationNameLength;                   ///< [in] Length of ApplicationName string
    bool bSet;                                      ///< [in] Set this if it's a set call
    ctl_property_value_type_t ValueType;            ///< [in] Type of value. Caller has to ensure it provides the right value
                                                    ///< type which decides how one read the union structure below
    ctl_property_t Value;                           ///< [in,out] Union of various type of values for 3D features. For enum
                                                    ///< types this can be anisotropic/frame pacing etc. This member is valid
                                                    ///< iff ValueType is not CTL_PROPERTY_VALUE_TYPE_CUSTOM
    int32_t CustomValueSize;                        ///< [in] CustomValue buffer size. Typically for a feature requiring custom
                                                    ///< struct, caller will know of it upfront and cn provide the right size
                                                    ///< info here
    void* pCustomValue;                             ///< [in,out] Pointer to a custom structure. Caller should allocate this
                                                    ///< buffer with known custom feature structure size. This member is valid
                                                    ///< iff ValueType is CTL_PROPERTY_VALUE_TYPE_CUSTOM

} ctl_3d_feature_getset_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get 3D capabilities
/// 
/// @details
///     - The application gets 3D properties
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pFeatureCaps`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetSupported3DCapabilities(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    ctl_3d_feature_caps_t* pFeatureCaps             ///< [in,out][release] 3D properties
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get/Set 3D feature
/// 
/// @details
///     - 3D feature details
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pFeature`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetSet3DFeature(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    ctl_3d_feature_getset_t* pFeature               ///< [in][release] 3D feature get/set parameter
    );


#if !defined(__GNUC__)
#pragma endregion // 3D
#endif
// Intel 'ctlApi' for Device Adapter
#if !defined(__GNUC__)
#pragma region display
#endif
///////////////////////////////////////////////////////////////////////////////
/// @brief Handle of a display output instance
typedef struct _ctl_display_output_handle_t *ctl_display_output_handle_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Check Driver version
/// 
/// @details
///     - The application checks driver version
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDeviceAdapter`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlCheckDriverVersion(
    ctl_device_adapter_handle_t hDeviceAdapter,     ///< [in][release] handle to control device adapter
    ctl_version_info_t version_info                 ///< [in][release] Driver version info
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Enumerate devices
/// 
/// @details
///     - The application checks driver version
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hAPIHandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pCount`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
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
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Enumerate display outputs
/// 
/// @details
///     - Enumerates display output capabilities
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDeviceAdapter`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pCount`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
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
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief OS specific Display identifiers
typedef union _ctl_os_display_encoder_identifier_t
{
    uint32_t WindowsDisplayEncoderID;               ///< [out] Windows OS Display encoder ID
    ctl_generic_void_datatype_t DisplayEncoderID;   ///< [out] Display encoder ID for non-windows OS

} ctl_os_display_encoder_identifier_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Various display types
typedef enum _ctl_display_output_types_t
{
    CTL_DISPLAY_OUTPUT_TYPES_INVALID = 0,           ///< Invalid
    CTL_DISPLAY_OUTPUT_TYPES_DISPLAYPORT = 1,       ///< DisplayPort
    CTL_DISPLAY_OUTPUT_TYPES_HDMI = 2,              ///< HDMI
    CTL_DISPLAY_OUTPUT_TYPES_DVI = 3,               ///< DVI
    CTL_DISPLAY_OUTPUT_TYPES_MIPI = 4,              ///< MIPI
    CTL_DISPLAY_OUTPUT_TYPES_CRT = 5,               ///< CRT
    CTL_DISPLAY_OUTPUT_TYPES_MAX

} ctl_display_output_types_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Supported output bits per color (bpc) bitmasks
typedef uint32_t ctl_output_bpc_flags_t;
typedef enum _ctl_output_bpc_flag_t
{
    CTL_OUTPUT_BPC_FLAG_6BPC = CTL_BIT(0),          ///< [out] Is 6bpc supported
    CTL_OUTPUT_BPC_FLAG_8BPC = CTL_BIT(1),          ///< [out] Is 8bpc supported
    CTL_OUTPUT_BPC_FLAG_10BPC = CTL_BIT(2),         ///< [out] Is 10bpc supported
    CTL_OUTPUT_BPC_FLAG_12BPC = CTL_BIT(3),         ///< [out] Is 12bpc supported
    CTL_OUTPUT_BPC_FLAG_MAX = 0x80000000

} ctl_output_bpc_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Display output features. This will indicate only the high level
///        capabilities
typedef uint32_t ctl_std_display_feature_flags_t;
typedef enum _ctl_std_display_feature_flag_t
{
    CTL_STD_DISPLAY_FEATURE_FLAG_HDCP = CTL_BIT(0), ///< [out] Is HDCP supported
    CTL_STD_DISPLAY_FEATURE_FLAG_HD_AUDIO = CTL_BIT(1), ///< [out] Is HD Audio supported
    CTL_STD_DISPLAY_FEATURE_FLAG_PSR = CTL_BIT(2),  ///< [out] Is VESA PSR supported
    CTL_STD_DISPLAY_FEATURE_FLAG_ADAPTIVESYNC_VRR = CTL_BIT(3), ///< [out] Is VESA Adaptive Sync or HDMI VRR supported
    CTL_STD_DISPLAY_FEATURE_FLAG_VESA_COMPRESSION = CTL_BIT(4), ///< [out] Is display compression (VESA DSC) supported
    CTL_STD_DISPLAY_FEATURE_FLAG_HDR = CTL_BIT(5),  ///< [out] Is HDR supported
    CTL_STD_DISPLAY_FEATURE_FLAG_HDMI_QMS = CTL_BIT(6), ///< [out] Is HDMI QMS supported
    CTL_STD_DISPLAY_FEATURE_FLAG_MAX = 0x80000000

} ctl_std_display_feature_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Advanced Graphics Features provided by Intel Graphics Adapter. This
///        will indicate only the high level capabilities
typedef uint32_t ctl_intel_display_feature_flags_t;
typedef enum _ctl_intel_display_feature_flag_t
{
    CTL_INTEL_DISPLAY_FEATURE_FLAG_DPST = CTL_BIT(0),   ///< [out] Is DPST supported
    CTL_INTEL_DISPLAY_FEATURE_FLAG_LACE = CTL_BIT(1),   ///< [out] Is LACE supported
    CTL_INTEL_DISPLAY_FEATURE_FLAG_DRRS = CTL_BIT(2),   ///< [out] Is DRRS supported
    CTL_INTEL_DISPLAY_FEATURE_FLAG_MAX = 0x80000000

} ctl_intel_display_feature_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Attached Display Mux Type
typedef enum _ctl_attached_display_mux_type_t
{
    CTL_ATTACHED_DISPLAY_MUX_TYPE_NATIVE = 0,       ///< [out] Native DP / HDMI 
    CTL_ATTACHED_DISPLAY_MUX_TYPE_THUNDERBOLT = 1,  ///< [out] Thunderbolt 
    CTL_ATTACHED_DISPLAY_MUX_TYPE_TYPE_C = 2,       ///< [out] USB Type C  
    CTL_ATTACHED_DISPLAY_MUX_TYPE_USB4 = 3,         ///< [out] USB4 
    CTL_ATTACHED_DISPLAY_MUX_TYPE_MAX

} ctl_attached_display_mux_type_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Signal Standard
typedef enum _ctl_signal_standard_type_t
{
    CTL_SIGNAL_STANDARD_TYPE_UNKNOWN = 0,           ///< [out] Unknown Signal Standard 
    CTL_SIGNAL_STANDARD_TYPE_CUSTOM = 1,            ///< [out] Custom added timing 
    CTL_SIGNAL_STANDARD_TYPE_DMT = 2,               ///< [out] DMT timing  
    CTL_SIGNAL_STANDARD_TYPE_GTF = 3,               ///< [out] GTF Timing 
    CTL_SIGNAL_STANDARD_TYPE_CVT = 4,               ///< [out] CVT Timing 
    CTL_SIGNAL_STANDARD_TYPE_CTA = 5,               ///< [out] CTA Timing 
    CTL_SIGNAL_STANDARD_TYPE_MAX

} ctl_signal_standard_type_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Protocol Converter Location
typedef uint32_t ctl_protocol_converter_location_flags_t;
typedef enum _ctl_protocol_converter_location_flag_t
{
    CTL_PROTOCOL_CONVERTER_LOCATION_FLAG_ONBOARD = CTL_BIT(0),  ///< [out] OnBoard Protocol Converter
    CTL_PROTOCOL_CONVERTER_LOCATION_FLAG_EXTERNAL = CTL_BIT(1), ///< [out] External Dongle
    CTL_PROTOCOL_CONVERTER_LOCATION_FLAG_MAX = 0x80000000

} ctl_protocol_converter_location_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief [out] Display Output configuration related flags which indicate how
///        the output pixel stream drive the panel
typedef uint32_t ctl_display_config_flags_t;
typedef enum _ctl_display_config_flag_t
{
    CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ACTIVE = CTL_BIT(0),///< [out] DisplayActive 0: InActive 1: Active 
    CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED = CTL_BIT(1),  ///< [out] DisplayAttached.This Bit indicates if any dongle/display/hub is
                                                    ///< attached to the encoder. 0: Not Attached 1: Attached 
    CTL_DISPLAY_CONFIG_FLAG_IS_DONGLE_CONNECTED_TO_ENCODER = CTL_BIT(2),///< [out] This BIT will be set if a dongle/hub/onboard protocol converter
                                                    ///< , is attached to the encoder
    CTL_DISPLAY_CONFIG_FLAG_DITHERING_ENABLED = CTL_BIT(3), ///< [out] This BIT will be set if dithering is enabled on the encoder
    CTL_DISPLAY_CONFIG_FLAG_MAX = 0x80000000

} ctl_display_config_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief [out] Encoder configuration related flags which indicate how the
///        output pixel stream drive the panel 
typedef uint32_t ctl_encoder_config_flags_t;
typedef enum _ctl_encoder_config_flag_t
{
    CTL_ENCODER_CONFIG_FLAG_INTERNAL_DISPLAY = CTL_BIT(0),  ///< [out] Internal connection or not
    CTL_ENCODER_CONFIG_FLAG_VESA_TILED_DISPLAY = CTL_BIT(1),///< [out] VESA DisplayID based tiled display which is driven by either
                                                    ///< multiple physical connections (DisplayPort SST) or virtual streams
                                                    ///< (DisplayPort MST)
    CTL_ENCODER_CONFIG_FLAG_TYPEC_CAPABLE = CTL_BIT(2), ///< [out] This is set if encoder supports type c display 
    CTL_ENCODER_CONFIG_FLAG_TBT_CAPABLE = CTL_BIT(3),   ///< [out] This is set if encoder supports Thunderbolt display 
    CTL_ENCODER_CONFIG_FLAG_DITHERING_SUPPORTED = CTL_BIT(4),   ///< [out] This BIT will be set if encoder supports dithering
    CTL_ENCODER_CONFIG_FLAG_VIRTUAL_DISPLAY = CTL_BIT(5),   ///< [out] This BIT will be set if this is a virtual display.Hardware based
                                                    ///< features will not be applicable to this display.For collage display
                                                    ///< this will be set for the virtual output created by driver. For split
                                                    ///< display this will be set for the virtual split displays created out of
                                                    ///< one single physical display
    CTL_ENCODER_CONFIG_FLAG_HIDDEN_DISPLAY = CTL_BIT(6),///< [out] This BIT will be set if display is hidden from OS
    CTL_ENCODER_CONFIG_FLAG_COLLAGE_DISPLAY = CTL_BIT(7),   ///< [out] This BIT will be set if this is a collage display
    CTL_ENCODER_CONFIG_FLAG_SPLIT_DISPLAY = CTL_BIT(8), ///< [out] This BIT will be set if this is a split display
    CTL_ENCODER_CONFIG_FLAG_MAX = 0x80000000

} ctl_encoder_config_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Display Timing
typedef struct _ctl_display_timing_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint64_t PixelClock;                            ///< [out] Pixel Clock in Hz
    uint32_t HActive;                               ///< [out] Horizontal Active
    uint32_t VActive;                               ///< [out] Vertical Active
    uint32_t HTotal;                                ///< [out] Horizontal Total
    uint32_t VTotal;                                ///< [out] Vertical Total
    uint32_t HBlank;                                ///< [out] Horizontal Blank
    uint32_t VBlank;                                ///< [out] Vertical Blank
    uint32_t HSync;                                 ///< [out] Horizontal Blank
    uint32_t VSync;                                 ///< [out] Vertical Blank
    float RefreshRate;                              ///< [out] Refresh Rate
    ctl_signal_standard_type_t SignalStandard;      ///< [out] Signal Standard
    uint8_t VicId;                                  ///< [out] VIC ID for CTA timings

} ctl_display_timing_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief This structure will contain the properties of the display currently
///        attached to the encoder.
typedef struct _ctl_display_properties_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_os_display_encoder_identifier_t Os_display_encoder_handle;  ///< [out] OS specific Display ID
    ctl_display_output_types_t Type;                ///< [out] Device Type from display HW stand point. If a DisplayPort
                                                    ///< protocol converter is involved, this will indicate it's DisplayPort.
                                                    ///< The protocol converter's output will be available from
                                                    ///< ProtocolConverterOutput field
    ctl_attached_display_mux_type_t AttachedDisplayMuxType; ///< [out] Attached Display Mux Type
    ctl_display_output_types_t ProtocolConverterOutput; ///< [out] Protocol output type which can be used if config flags indicate
                                                    ///< it's a protocol converter. If it's not a protocol converter this will
                                                    ///< be set to CTL_DISPLAY_OUTPUT_TYPES_INVALID
    ctl_revision_datatype_t SupportedSpec;          ///< [out] Supported industry spec version.
    ctl_output_bpc_flags_t SupportedOutputBPCFlags; ///< [out] Supported output bits per color. Refer ::ctl_output_bpc_flag_t.
                                                    ///< This is independent of RGB or YCbCr output.This is the max BPC
                                                    ///< supported.BPC will vary per mode based on restrictions like bandwidth
                                                    ///< and monitor support
    ctl_protocol_converter_location_flags_t ProtocolConverterType;  ///< [out] Currently Active Protocol Converter
    ctl_display_config_flags_t DisplayConfigFlags;  ///< [out] Output configuration related flags which indicate how the output
                                                    ///< pixel stream drive the panel
    ctl_std_display_feature_flags_t FeatureEnabledFlags;///< [out] Enabled Display features.Refer ::ctl_std_display_feature_flag_t.
    ctl_std_display_feature_flags_t FeatureSupportedFlags;  ///< [out] Display Supported feature.Refer ::ctl_std_display_feature_flag_t
    ctl_intel_display_feature_flags_t AdvancedFeatureEnabledFlags;  ///< [out] Enabled advanced feature.Refer
                                                    ///< ::ctl_intel_display_feature_flag_t.
    ctl_intel_display_feature_flags_t AdvancedFeatureSupportedFlags;///< [out] Supported advanced feature.Refer
                                                    ///< ::ctl_intel_display_feature_flag_t.
    ctl_display_timing_t Display_Timing_Info;       ///< [out] Applied Timing on the Display
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_display_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Adapter's display encoder properties
typedef struct _ctl_adapter_display_encoder_properties_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_os_display_encoder_identifier_t Os_display_encoder_handle;  ///< [out] OS specific Display ID
    ctl_display_output_types_t Type;                ///< [out] Device Type from display HW stand point. If a DisplayPort
                                                    ///< protocol converter is involved, this will indicate it's DisplayPort.
                                                    ///< The protocol converter's output will be available from
                                                    ///< ProtocolConverterOutput field
    bool IsOnBoardProtocolConverterOutputPresent;   ///< [out] Protocol output type which can be used if it's a protocol
                                                    ///< converter. If it's not a protocol converter this will be set to
                                                    ///< CTL_DISPLAY_OUTPUT_TYPES_INVALID
    ctl_revision_datatype_t SupportedSpec;          ///< [out] Supported industry spec version
    ctl_output_bpc_flags_t SupportedOutputBPCFlags; ///< [out] Supported output bits per color. Refer ::ctl_output_bpc_flag_t.
                                                    ///< This is independent of RGB or YCbCr output.This is the max BPC
                                                    ///< supported.BPC will vary per mode based on restrictions like bandwidth
                                                    ///< and monitor support
    ctl_encoder_config_flags_t EncoderConfigFlags;  ///< [out] Output configuration related flags which indicate how the output
                                                    ///< pixel stream drive the panel  
                                                    ///< Note:  
                                                    ///<    Virtual = 1: This indicates that its a software display. Hardware
                                                    ///< based features will not be applicable to this display. 
                                                    ///<    Collage=1,Virtual=1: Indicates the fake display output created by
                                                    ///< driver which has the combined resolution of multiple physical displays
                                                    ///< involved in collage configuration  
                                                    ///<    Collage=1,Virtual=0: Indicates the child physical displays involved
                                                    ///< in a collage configuration. These are real physical outputs  
                                                    ///<    Split=1,Virtual=1  : Indicates the fake display output created by
                                                    ///< driver which occupies a portion of a real physical display  
                                                    ///<    Split=1,Virtual=0  : Indicates the physical display which got split
                                                    ///< to form multiple split displays  
                                                    ///<    Split=1,Collage=1  : Invalid combination                  
    ctl_std_display_feature_flags_t FeatureSupportedFlags;  ///< [out] Adapter Supported feature flags. Refer
                                                    ///< ::ctl_std_display_feature_flag_t
    ctl_intel_display_feature_flags_t AdvancedFeatureSupportedFlags;///< [out] Advanced Features Supported by the Adapter. Refer
                                                    ///< ::ctl_intel_display_feature_flag_t
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_adapter_display_encoder_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get Device Properties
/// 
/// @details
///     - The application gets device properties
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pProperties`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetDeviceProperties(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to control device adapter
    ctl_device_adapter_properties_t* pProperties    ///< [in,out][release] Query result for device properties
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get Display  Properties
/// 
/// @details
///     - The application gets display  properties
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pProperties`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetDisplayProperties(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_display_properties_t* pProperties           ///< [in,out][release] Query result for display  properties
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get Adapter Display encoder  Properties
/// 
/// @details
///     - The application gets the graphic adapters display encoder properties
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pProperties`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetAdaperDisplayEncoderProperties(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_adapter_display_encoder_properties_t* pProperties   ///< [in,out][release] Query result for adapter display encoder properties
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get Level0 Device handle
/// 
/// @details
///     - The application gets OneAPI Level0 Device handles
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pZeDevice`
///         + `nullptr == hInstance`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetZeDevice(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    void* pZeDevice,                                ///< [out][release] ze_device handle
    void** hInstance                                ///< [out][release] Module instance which caller can use to get export
                                                    ///< functions directly
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Various sharpness filter types
typedef uint32_t ctl_sharpness_filter_type_flags_t;
typedef enum _ctl_sharpness_filter_type_flag_t
{
    CTL_SHARPNESS_FILTER_TYPE_FLAG_NON_ADAPTIVE = CTL_BIT(0),   ///< Non-adaptive sharpness
    CTL_SHARPNESS_FILTER_TYPE_FLAG_ADAPTIVE = CTL_BIT(1),   ///< Adaptive sharpness
    CTL_SHARPNESS_FILTER_TYPE_FLAG_MAX = 0x80000000

} ctl_sharpness_filter_type_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Sharpness filter properties
typedef struct _ctl_sharpness_filter_properties_t
{
    ctl_sharpness_filter_type_flags_t FilterType;   ///< [out] Filter type
    ctl_property_range_info_t FilterDetails;        ///< [out] Min, max & step size information

} ctl_sharpness_filter_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Various sharpness filter types
typedef struct _ctl_sharpness_caps_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_sharpness_filter_type_flags_t SupportedFilterFlags; ///< [out] Supported sharpness filters for a given display output
    uint8_t NumFilterTypes;                         ///< [out] Number of elements in filter properties array
    ctl_sharpness_filter_properties_t* pFilterProperty; ///< [in,out] Array of filter properties structure describing supported
                                                    ///< filter capabilities. Caller should provide a pre-allocated memory for
                                                    ///< this.

} ctl_sharpness_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Current sharpness setting
typedef struct _ctl_sharpness_settings_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    bool Enable;                                    ///< [in,out] Current or new state of sharpness setting
    ctl_sharpness_filter_type_flags_t FilterType;   ///< [in,out] Current or new filter to be set
    float Intensity;                                ///< [in,out] Setting intensity to be applied

} ctl_sharpness_settings_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get Sharpness capability
/// 
/// @details
///     - Returns sharpness capability
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pSharpnessCaps`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetSharpnessCaps(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_sharpness_caps_t* pSharpnessCaps            ///< [in,out][release] Query result for sharpness capability
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get Sharpness setting
/// 
/// @details
///     - Returns current sharpness settings
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pSharpnessSettings`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetCurrentSharpness(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_sharpness_settings_t* pSharpnessSettings    ///< [in,out][release] Query result for sharpness current settings
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Set Sharpness setting
/// 
/// @details
///     - Set current sharpness settings
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pSharpnessSettings`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlSetCurrentSharpness(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_sharpness_settings_t* pSharpnessSettings    ///< [in][release] Set sharpness current settings
    );

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_I2C_MAX_DATA_SIZE
/// @brief I2C Maximum data size
#define CTL_I2C_MAX_DATA_SIZE  0x0080
#endif // CTL_I2C_MAX_DATA_SIZE

///////////////////////////////////////////////////////////////////////////////
/// @brief I2CFlags bitmasks
typedef uint32_t ctl_i2c_flags_t;
typedef enum _ctl_i2c_flag_t
{
    CTL_I2C_FLAG_ATOMICI2C = CTL_BIT(0),            ///< Force Atomic I2C
    CTL_I2C_FLAG_MAX = 0x80000000

} ctl_i2c_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief I2C access arguments
typedef struct _ctl_i2c_access_args_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint32_t DataSize;                              ///< [in,out] Valid data size
    uint32_t Address;                               ///< [in] Adreess to read or write
    ctl_operation_type_t OpType;                    ///< [in] Operation type, 1 for Read, 2 for Write, for Write operation, App
                                                    ///< needs to run with admin privileges
    uint32_t Offset;                                ///< [in] Offset
    ctl_i2c_flags_t Flags;                          ///< [in] I2C Flags. Refer ::ctl_i2c_flags_t
    uint64_t RAD;                                   ///< [in] RAD, For Future use, to be used for branch devices, Interface
                                                    ///< will be provided to get RAD
    uint8_t Data[CTL_I2C_MAX_DATA_SIZE];            ///< [in,out] Data array

} ctl_i2c_access_args_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief I2C Access
/// 
/// @details
///     - The application does I2C aceess
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pI2cAccessArgs`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
///     - ::CTL_RESULT_ERROR_INVALID_OPERATION_TYPE - "Invalid operation type"
///     - ::CTL_RESULT_ERROR_INVALID_SIZE - "Invalid I2C data size"
///     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS - "Insufficient permissions"
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER - "Invalid null pointer"
///     - ::CTL_RESULT_ERROR_NULL_OS_DISPLAY_OUTPUT_HANDLE - "Null OS display output handle"
///     - ::CTL_RESULT_ERROR_NULL_OS_INTERFACE - "Null OS interface"
///     - ::CTL_RESULT_ERROR_NULL_OS_ADAPATER_HANDLE - "Null OS adapter handle"
///     - ::CTL_RESULT_ERROR_KMD_CALL - "Kernal mode driver call failure"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlI2CAccess(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in] Handle to display output
    ctl_i2c_access_args_t* pI2cAccessArgs           ///< [in,out] I2c access arguments
    );

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_AUX_MAX_DATA_SIZE
/// @brief Aux Maximum data size
#define CTL_AUX_MAX_DATA_SIZE  132
#endif // CTL_AUX_MAX_DATA_SIZE

///////////////////////////////////////////////////////////////////////////////
/// @brief AUX Flags bitmasks
typedef uint32_t ctl_aux_flags_t;
typedef enum _ctl_aux_flag_t
{
    CTL_AUX_FLAG_NATIVE_AUX = CTL_BIT(0),           ///< For Native AUX operation
    CTL_AUX_FLAG_I2C_AUX = CTL_BIT(1),              ///< For I2C AUX operation
    CTL_AUX_FLAG_I2C_AUX_MOT = CTL_BIT(2),          ///< For I2C AUX MOT operation
    CTL_AUX_FLAG_MAX = 0x80000000

} ctl_aux_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief AUX access arguments
typedef struct _ctl_aux_access_args_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_operation_type_t OpType;                    ///< [in] Operation type, 1 for Read, 2 for Write, for Write operation, App
                                                    ///< needs to run with admin privileges
    ctl_aux_flags_t Flags;                          ///< [in] Aux Flags. Refer ::ctl_aux_flags_t
    uint32_t Address;                               ///< [in] Adreess to read or write
    uint64_t RAD;                                   ///< [in] RAD, For Future use, to be used for branch devices, Interface
                                                    ///< will be provided to get RAD
    uint32_t PortID;                                ///< [in] Port ID, For Future use, to be used for SST tiled devices
    uint32_t DataSize;                              ///< [in,out] Valid data size
    uint8_t Data[CTL_AUX_MAX_DATA_SIZE];            ///< [in,out] Data array

} ctl_aux_access_args_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Aux Access
/// 
/// @details
///     - The application does Aux aceess
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pAuxAccessArgs`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
///     - ::CTL_RESULT_ERROR_INVALID_OPERATION_TYPE - "Invalid operation type"
///     - ::CTL_RESULT_ERROR_INVALID_SIZE - "Invalid AUX data size"
///     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS - "Insufficient permissions"
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER - "Invalid null pointer"
///     - ::CTL_RESULT_ERROR_NULL_OS_DISPLAY_OUTPUT_HANDLE - "Null OS display output handle"
///     - ::CTL_RESULT_ERROR_NULL_OS_INTERFACE - "Null OS interface"
///     - ::CTL_RESULT_ERROR_NULL_OS_ADAPATER_HANDLE - "Null OS adapter handle"
///     - ::CTL_RESULT_ERROR_INVALID_AUX_ACCESS_FLAG - "Invalid flag for AUX access"
///     - ::CTL_RESULT_ERROR_KMD_CALL - "Kernal mode driver call failure"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlAUXAccess(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in] Handle to display output
    ctl_aux_access_args_t* pAuxAccessArgs           ///< [in,out] Aux access arguments
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Power saving features (Each individual feature's set & get call can be
///        called only once at a time)
typedef uint32_t ctl_power_optimization_flags_t;
typedef enum _ctl_power_optimization_flag_t
{
    CTL_POWER_OPTIMIZATION_FLAG_FBC = CTL_BIT(0),   ///< Frame buffer compression
    CTL_POWER_OPTIMIZATION_FLAG_PSR = CTL_BIT(1),   ///< Panel self refresh
    CTL_POWER_OPTIMIZATION_FLAG_DPST = CTL_BIT(2),  ///< Display back-light power saving technology
    CTL_POWER_OPTIMIZATION_FLAG_LRR = CTL_BIT(3),   ///< Low refresh rate (LRR/ALRR/UBRR)
    CTL_POWER_OPTIMIZATION_FLAG_MAX = 0x80000000

} ctl_power_optimization_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Power Source
typedef enum _ctl_power_source_t
{
    CTL_POWER_SOURCE_AC = 0,                        ///< Power Source AC
    CTL_POWER_SOURCE_DC = 1,                        ///< Power Source DC
    CTL_POWER_SOURCE_MAX

} ctl_power_source_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Power Optimization Plan
typedef enum _ctl_power_optimization_plan_t
{
    CTL_POWER_OPTIMIZATION_PLAN_BALANCED = 0,       ///< Balanced mode
    CTL_POWER_OPTIMIZATION_PLAN_HIGH_PERFORMANCE = 1,   ///< High Performance Mode
    CTL_POWER_OPTIMIZATION_PLAN_POWER_SAVER = 2,    ///< Power Saver Mode
    CTL_POWER_OPTIMIZATION_PLAN_MAX

} ctl_power_optimization_plan_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Type of low refresh rate feature
typedef uint32_t ctl_power_optimization_lrr_flags_t;
typedef enum _ctl_power_optimization_lrr_flag_t
{
    CTL_POWER_OPTIMIZATION_LRR_FLAG_LRR10 = CTL_BIT(0), ///< LRR 1.0
    CTL_POWER_OPTIMIZATION_LRR_FLAG_LRR20 = CTL_BIT(1), ///< LRR 2.0
    CTL_POWER_OPTIMIZATION_LRR_FLAG_LRR25 = CTL_BIT(2), ///< LRR 2.5
    CTL_POWER_OPTIMIZATION_LRR_FLAG_ALRR = CTL_BIT(3),  ///< Autonomous LRR
    CTL_POWER_OPTIMIZATION_LRR_FLAG_UBLRR = CTL_BIT(4), ///< User based low refresh rate
    CTL_POWER_OPTIMIZATION_LRR_FLAG_UBZRR = CTL_BIT(5), ///< User based zero refresh rate
    CTL_POWER_OPTIMIZATION_LRR_FLAG_MAX = 0x80000000

} ctl_power_optimization_lrr_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Power optimization caps
typedef struct _ctl_power_optimization_caps_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_power_optimization_flags_t SupportedFeatures;   ///< [out] Supported power optimization features

} ctl_power_optimization_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get Power optimization features
/// 
/// @details
///     - Returns power optimization capabilities
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pPowerOptimizationCaps`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetPowerOptimizationCaps(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_power_optimization_caps_t* pPowerOptimizationCaps   ///< [in,out][release] Query result for power optimization features
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief LRR detailed settings
typedef struct _ctl_power_optimization_lrr_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_power_optimization_lrr_flags_t SupportedLRRTypes;   ///< [out] LRR type(s)
    ctl_power_optimization_lrr_flags_t CurrentLRRTypes; ///< [in,out] Current enabled LRR type(s) or the LRR type(s) to set to
    bool bRequirePSRDisable;                        ///< [out] Require PSR disable for any change in the selected LRR feature.
                                                    ///< Caller can re-enable PSR once the respective LRR feature is
                                                    ///< enable/disabled. E.g. for UBRR based on platform this flag may not be
                                                    ///< set in which case caller doesn't need to do an explicit PSR disable
    uint16_t LowRR;                                 ///< [out] Lowest RR used for LRR functionality if known to source

} ctl_power_optimization_lrr_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief PSR detailed settings
typedef struct _ctl_power_optimization_psr_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint8_t PSRVersion;                             ///< [in,out] A value of 1 means PSR1, 2 means PSR2
    bool FullFetchUpdate;                           ///< [in,out] Full fetch and update

} ctl_power_optimization_psr_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief DPST detailed settings
typedef struct _ctl_power_optimization_dpst_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    int8_t MaxLevels;                               ///< [out] Maximum supported levels starting from 0
    int8_t Level;                                   ///< [in,out] Current level or level to be set
    bool SoCIP;                                     ///< [out] SoC IP based or not
    bool IsEPSMEnabled;                             ///< [in,out] EPSM enabled or no
    bool IsEPSMSupported;                           ///< [out] EPSM supported or no

} ctl_power_optimization_dpst_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Feature specific power optimization data
typedef union _ctl_power_optimization_feature_specific_info_t
{
    ctl_power_optimization_lrr_t LRRInfo;           ///< [out] LRR info
    ctl_power_optimization_psr_t PSRInfo;           ///< [in,out] PSR info
    ctl_power_optimization_dpst_t DPSTInfo;         ///< [in,out] DPST info

} ctl_power_optimization_feature_specific_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Power optimization settings
typedef struct _ctl_power_optimization_settings_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_power_optimization_plan_t PowerOptimizationPlan;///< [in] Power optimization power plan (max power/max perf/balanced)
    ctl_power_optimization_flags_t PowerOptimizationFeature;///< [in] Power optimization feature interested in
    bool Enable;                                    ///< [in,out] Enable state
    ctl_power_optimization_feature_specific_info_t FeatureSpecificData; ///< [in,out] Data specific to the feature caller is interested in
    ctl_power_source_t PowerSource;                 ///< [in] AC/DC

} ctl_power_optimization_settings_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get Power optimization setting
/// 
/// @details
///     - Returns power optimization setting for a specific feature
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pPowerOptimizationSettings`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
///     - ::CTL_RESULT_ERROR_INVALID_POWERFEATURE_OPTIMIZATION_FLAG - "Unsupported PowerOptimizationFeature"
///     - ::CTL_RESULT_ERROR_INVALID_POWERSOURCE_TYPE_FOR_DPST - "DPST is supported only in DC Mode"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetPowerOptimizationSetting(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_power_optimization_settings_t* pPowerOptimizationSettings   ///< [in,out][release] Power optimization data to be fetched
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Set Power optimization setting
/// 
/// @details
///     - Set power optimization setting for a specific feature
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pPowerOptimizationSettings`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
///     - ::CTL_RESULT_ERROR_INVALID_POWERFEATURE_OPTIMIZATION_FLAG - "Unsupported PowerOptimizationFeature"
///     - ::CTL_RESULT_ERROR_INVALID_POWERSOURCE_TYPE_FOR_DPST - "DPST is supported only in DC Mode"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlSetPowerOptimizationSetting(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_power_optimization_settings_t* pPowerOptimizationSettings   ///< [in][release] Power optimization data to be applied
    );

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_MAX_NUM_SAMPLES_PER_CHANNEL_1D_LUT
/// @brief Maximum number of samples per channel 1D LUT
#define CTL_MAX_NUM_SAMPLES_PER_CHANNEL_1D_LUT  8192
#endif // CTL_MAX_NUM_SAMPLES_PER_CHANNEL_1D_LUT

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixtx pipe set configuration flags bitmasks
typedef uint32_t ctl_pixtx_pipe_set_config_flags_t;
typedef enum _ctl_pixtx_pipe_set_config_flag_t
{
    CTL_PIXTX_PIPE_SET_CONFIG_FLAG_PERSIST_ACROSS_POWER_EVENTS = CTL_BIT(0),///< For maintaining persistance across power events
    CTL_PIXTX_PIPE_SET_CONFIG_FLAG_MAX = 0x80000000

} ctl_pixtx_pipe_set_config_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation block types
typedef enum _ctl_pixtx_block_type_t
{
    CTL_PIXTX_BLOCK_TYPE_1D_LUT = 1,                ///< Block type 1D LUT
    CTL_PIXTX_BLOCK_TYPE_3D_LUT = 2,                ///< Block type 3D LUT
    CTL_PIXTX_BLOCK_TYPE_3X3_MATRIX = 3,            ///< Block type 3x3 matrix
    CTL_PIXTX_BLOCK_TYPE_3X3_MATRIX_AND_OFFSETS = 4,///< Block type 3x3 matrix and offsets
    CTL_PIXTX_BLOCK_TYPE_MAX

} ctl_pixtx_block_type_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation LUT sampling types
typedef enum _ctl_pixtx_lut_sampling_type_t
{
    CTL_PIXTX_LUT_SAMPLING_TYPE_UNIFORM = 0,        ///< Uniform LUT sampling
    CTL_PIXTX_LUT_SAMPLING_TYPE_NONUNIFORM = 1,     ///< Non uniform LUT sampling, Required mainly in HDR mode
    CTL_PIXTX_LUT_SAMPLING_TYPE_MAX

} ctl_pixtx_lut_sampling_type_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Configuration query types
typedef enum _ctl_pixtx_config_query_type_t
{
    CTL_PIXTX_CONFIG_QUERY_TYPE_CAPABILITY = 0,     ///< Get complete pixel processing pipeline capability
    CTL_PIXTX_CONFIG_QUERY_TYPE_CURRENT = 1,        ///< Get the configuration set through last set call
    CTL_PIXTX_CONFIG_QUERY_TYPE_MAX

} ctl_pixtx_config_query_type_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Configuration operation types
typedef enum _ctl_pixtx_config_opertaion_type_t
{
    CTL_PIXTX_CONFIG_OPERTAION_TYPE_RESTORE_DEFAULT = 1,///< Restore block by block or entire pipe line. Use NumBlocks = 0 to
                                                    ///< restore all.
    CTL_PIXTX_CONFIG_OPERTAION_TYPE_SET_CUSTOM = 2, ///< Custom LUT or matrix can be set thorugh this option.
    CTL_PIXTX_CONFIG_OPERTAION_TYPE_MAX

} ctl_pixtx_config_opertaion_type_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation gamma encoding types
typedef enum _ctl_pixtx_gamma_encoding_type_t
{
    CTL_PIXTX_GAMMA_ENCODING_TYPE_SRGB = 0,         ///< Gamma encoding SRGB
    CTL_PIXTX_GAMMA_ENCODING_TYPE_REC709 = 1,       ///< Gamma encoding REC709, Applicable for REC2020 as well
    CTL_PIXTX_GAMMA_ENCODING_TYPE_ST2084 = 2,       ///< Gamma encoding ST2084
    CTL_PIXTX_GAMMA_ENCODING_TYPE_HLG = 3,          ///< Gamma encoding HLG
    CTL_PIXTX_GAMMA_ENCODING_TYPE_LINEAR = 4,       ///< Gamma encoding linear
    CTL_PIXTX_GAMMA_ENCODING_TYPE_MAX

} ctl_pixtx_gamma_encoding_type_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation color space types
typedef enum _ctl_pixtx_color_space_t
{
    CTL_PIXTX_COLOR_SPACE_REC709 = 0,               ///< Color space REC709
    CTL_PIXTX_COLOR_SPACE_REC2020 = 1,              ///< Color space REC2020
    CTL_PIXTX_COLOR_SPACE_ADOBE_RGB = 2,            ///< Color space AdobeRGB
    CTL_PIXTX_COLOR_SPACE_P3_D65 = 3,               ///< Color space P3_D65
    CTL_PIXTX_COLOR_SPACE_P3_DCI = 4,               ///< Color space P3_DCI
    CTL_PIXTX_COLOR_SPACE_P3_D60 = 5,               ///< Color space P3_D60
    CTL_PIXTX_COLOR_SPACE_CUSTOM = 0xFFFF,          ///< Color space custom, Refer ::ctl_pixtx_color_primaries_t for color
                                                    ///< primary details
    CTL_PIXTX_COLOR_SPACE_MAX

} ctl_pixtx_color_space_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation color model types
typedef enum _ctl_pixtx_color_model_t
{
    CTL_PIXTX_COLOR_MODEL_RGB_FR = 0,               ///< Color model RGB full range
    CTL_PIXTX_COLOR_MODEL_RGB_LR = 1,               ///< Color model RGB limited range
    CTL_PIXTX_COLOR_MODEL_YCBCR_422_FR = 2,         ///< Color model YCBCR 422 full range
    CTL_PIXTX_COLOR_MODEL_YCBCR_422_LR = 3,         ///< Color model YCBCR 422 limited range
    CTL_PIXTX_COLOR_MODEL_YCBCR_420_FR = 4,         ///< Color model YCBCR 420 full range
    CTL_PIXTX_COLOR_MODEL_YCBCR_420_LR = 5,         ///< Color model YCBCR 420 limited range
    CTL_PIXTX_COLOR_MODEL_MAX

} ctl_pixtx_color_model_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation color primaries
typedef struct _ctl_pixtx_color_primaries_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    double xR;                                      ///< [out] CIE1931 x value with maximum red pixel value
    double yR;                                      ///< [out] CIE1931 y value with maximum red pixel value
    double xG;                                      ///< [out] CIE1931 x value with maximum green pixel value
    double yG;                                      ///< [out] CIE1931 y value with maximum green pixel value
    double xB;                                      ///< [out] CIE1931 x value with maximum blue pixel value
    double yB;                                      ///< [out] CIE1931 y value with maximum blue pixel value
    double xW;                                      ///< [out] CIE1931 x value with maximum white pixel value
    double yW;                                      ///< [out] CIE1931 y value with maximum white pixel value

} ctl_pixtx_color_primaries_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation pixel format
typedef struct _ctl_pixtx_pixel_format_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint32_t BitsPerColor;                          ///< [out] Bits per color, It Will be 16 for FP16 case
    bool IsFloat;                                   ///< [out] Will be set for FP16 or other floating point encoding schemes
    ctl_pixtx_gamma_encoding_type_t EncodingType;   ///< [out] Encoding type
    ctl_pixtx_color_space_t ColorSpace;             ///< [out] Color space
    ctl_pixtx_color_model_t ColorModel;             ///< [out] Color model
    ctl_pixtx_color_primaries_t ColorPrimaries;     ///< [out] Color primaries, Used mainly for custom color space
    double MaxBrightness;                           ///< [out] Maximum brightness of pixel values. If no input is given,
                                                    ///< default will be set to sRGB during set call. If panel capability is
                                                    ///< not known get call will default to sRGB.
    double MinBrightness;                           ///< [out] Minimum brightness of pixel values

} ctl_pixtx_pixel_format_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation 1D LUT configuration
typedef struct _ctl_pixtx_1dlut_config_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_pixtx_lut_sampling_type_t SamplingType;     ///< [in,out] Blocks with non-uniform sampling capability support unifrom
                                                    ///< sampling also but not vice versa.
    uint32_t NumSamplesPerChannel;                  ///< [in,out] Number of samples per channel. Resampled internally based on
                                                    ///< HW capability for uniformly sampled LUT.Maximum supported value is
                                                    ///< MAX_NUM_SAMPLES_PER_CHANNEL_1D_LUT Caller needs to use exact sampling
                                                    ///< position given in pSamplePositions for non-uniformly sampled LUTs.
    uint32_t NumChannels;                           ///< [in,out] Number of channels, 1 for Grey scale LUT, 3 for RGB LUT
    double* pSampleValues;                          ///< [in,out] Pointer to sample values, R array followed by G and B arrays
                                                    ///< in case of multi-channel LUT. Allocation size for pSampleValues should
                                                    ///< be NumSamplesPerChannel * NumChannels * sizeof(double)
    double* pSamplePositions;                       ///< [out] LUT (same for all channels) to represent sampling positions for
                                                    ///< non-uniformly sampled LUTs.Can be NULL in case uniformly sampled LUTs

} ctl_pixtx_1dlut_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation matrix configuration
typedef struct _ctl_pixtx_matrix_config_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    double PreOffsets[3];                           ///< [in,out] Pre offsets
    double PostOffsets[3];                          ///< [in,out] Post offsets
    double Matrix[3][3];                            ///< [in,out] 3x3 Matrix

} ctl_pixtx_matrix_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation 3D LUT sample
typedef struct _ctl_pixtx_3dlut_sample_t
{
    double Red;                                     ///< [in,out] Red output value
    double Green;                                   ///< [in,out] Green output value
    double Blue;                                    ///< [in,out] Blue output value

} ctl_pixtx_3dlut_sample_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation 3D LUT configuration
typedef struct _ctl_pixtx_3dlut_config_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint32_t NumSamplesPerChannel;                  ///< [in,out] Number of samples per channel
    ctl_pixtx_3dlut_sample_t* pSampleValues;        ///< [in,out] Pointer to sample values, R in outer most loop followed by G
                                                    ///< and B

} ctl_pixtx_3dlut_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation configuration
typedef union _ctl_pixtx_config_t
{
    ctl_pixtx_1dlut_config_t OneDLutConfig;         ///< [in,out] 1D LUT configuration
    ctl_pixtx_3dlut_config_t ThreeDLutConfig;       ///< [in,out] 3D LUT configuration
    ctl_pixtx_matrix_config_t MatrixConfig;         ///< [in,out] Matrix configuration

} ctl_pixtx_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation block configuration
typedef struct _ctl_pixtx_block_config_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint32_t BlockId;                               ///< [in,out] Unique ID for each pixel processing block. Id for a block is
                                                    ///< fixed for a platform.
    ctl_pixtx_block_type_t BlockType;               ///< [in,out] Block type
    ctl_pixtx_config_t Config;                      ///< [in,out] Configuration

} ctl_pixtx_block_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation pipe get configuration
typedef struct _ctl_pixtx_pipe_get_config_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_pixtx_config_query_type_t QueryType;        ///< [in] Query operation type
    ctl_pixtx_pixel_format_t InputPixelFormat;      ///< [out] Input pixel format
    ctl_pixtx_pixel_format_t OutputPixelFormat;     ///< [out] Output pixel format
    uint32_t NumBlocks;                             ///< [out] Number of blocks
    ctl_pixtx_block_config_t* pBlockConfigs;        ///< [out] Pointer to specific configs

} ctl_pixtx_pipe_get_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation pipe set configuration
typedef struct _ctl_pixtx_pipe_set_config_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_pixtx_config_opertaion_type_t OpertaionType;///< [in] Set operation type
    ctl_pixtx_pipe_set_config_flags_t Flags;        ///< [in] Flags
    uint32_t NumBlocks;                             ///< [in] Number of blocks
    ctl_pixtx_block_config_t* pBlockConfigs;        ///< [in,out] Array of block specific configs

} ctl_pixtx_pipe_set_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation get pipe configuration
/// 
/// @details
///     - The application does pixel transformation get pipe configuration
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pPixTxGetConfigArgs`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
///     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS - "Insufficient permissions"
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER - "Invalid null pointer"
///     - ::CTL_RESULT_ERROR_NULL_OS_DISPLAY_OUTPUT_HANDLE - "Null OS display output handle"
///     - ::CTL_RESULT_ERROR_NULL_OS_INTERFACE - "Null OS interface"
///     - ::CTL_RESULT_ERROR_NULL_OS_ADAPATER_HANDLE - "Null OS adapter handle"
///     - ::CTL_RESULT_ERROR_KMD_CALL - "Kernal mode driver call failure"
///     - ::CTL_RESULT_ERROR_INVALID_PIXTX_GET_CONFIG_QUERY_TYPE - "Invalid query type"
///     - ::CTL_RESULT_ERROR_INVALID_PIXTX_BLOCK_ID - "Invalid block id"
///     - ::CTL_RESULT_ERROR_INSUFFICIENT_PIXTX_BLOCK_CONFIG_MEMORY - "Insufficient memery allocated for BlockConfigs"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlPixelTransformationGetConfig(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in] Handle to display output
    ctl_pixtx_pipe_get_config_t* pPixTxGetConfigArgs///< [in,out] Pixel transformation get pipe configiguration arguments
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Pixel transformation set pipe configuration
/// 
/// @details
///     - The application does pixel transformation set pipe configuration
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pPixTxSetConfigArgs`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
///     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS - "Insufficient permissions"
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER - "Invalid null pointer"
///     - ::CTL_RESULT_ERROR_NULL_OS_DISPLAY_OUTPUT_HANDLE - "Null OS display output handle"
///     - ::CTL_RESULT_ERROR_NULL_OS_INTERFACE - "Null OS interface"
///     - ::CTL_RESULT_ERROR_NULL_OS_ADAPATER_HANDLE - "Null OS adapter handle"
///     - ::CTL_RESULT_ERROR_KMD_CALL - "Kernal mode driver call failure"
///     - ::CTL_RESULT_ERROR_INVALID_PIXTX_SET_CONFIG_OPERATION_TYPE - "Invalid operation type"
///     - ::CTL_RESULT_ERROR_INVALID_SET_CONFIG_NUMBER_OF_SAMPLES - "Invalid number of samples"
///     - ::CTL_RESULT_ERROR_INVALID_PIXTX_BLOCK_ID - "Invalid block id"
///     - ::CTL_RESULT_ERROR_PERSISTANCE_NOT_SUPPORTED - "Persistance not supported"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlPixelTransformationSetConfig(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in] Handle to display output
    ctl_pixtx_pipe_set_config_t* pPixTxSetConfigArgs///< [in,out] Pixel transformation set pipe configiguration arguments
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Panel descriptor access arguments
typedef struct _ctl_panel_descriptor_access_args_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_operation_type_t OpType;                    ///< [in] Operation type, 1 for Read, 2 for Write, for Write operation, App
                                                    ///< needs to run with admin privileges, Currently only Read operation is
                                                    ///< supported
    uint32_t BlockNumber;                           ///< [in] Block number, Need to provide only if acccessing EDID
    uint32_t DescriptorDataSize;                    ///< [in] Descriptor data size, Should be 0 for querying the size and
                                                    ///< should be DescriptorDataSize derived from query call otherwise
    uint8_t* pDescriptorData;                       ///< [in,out] Panel descriptor data

} ctl_panel_descriptor_access_args_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Panel Descriptor Access
/// 
/// @details
///     - The application does EDID or Display ID access
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pPanelDescriptorAccessArgs`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
///     - ::CTL_RESULT_ERROR_INVALID_OPERATION_TYPE - "Invalid operation type"
///     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS - "Insufficient permissions"
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER - "Invalid null pointer"
///     - ::CTL_RESULT_ERROR_NULL_OS_DISPLAY_OUTPUT_HANDLE - "Null OS display output handle"
///     - ::CTL_RESULT_ERROR_NULL_OS_INTERFACE - "Null OS interface"
///     - ::CTL_RESULT_ERROR_NULL_OS_ADAPATER_HANDLE - "Null OS adapter handle"
///     - ::CTL_RESULT_ERROR_KMD_CALL - "Kernal mode driver call failure"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlPanelDescriptorAccess(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in] Handle to display output
    ctl_panel_descriptor_access_args_t* pPanelDescriptorAccessArgs  ///< [in,out] Panel descriptor access arguments
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Retro Scaling Types
typedef uint32_t ctl_retro_scaling_type_flags_t;
typedef enum _ctl_retro_scaling_type_flag_t
{
    CTL_RETRO_SCALING_TYPE_FLAG_INTEGER = CTL_BIT(0),   ///< Integer Scaling
    CTL_RETRO_SCALING_TYPE_FLAG_NEAREST_NEIGHBOUR = CTL_BIT(1), ///< Nearest Neighbour Scaling
    CTL_RETRO_SCALING_TYPE_FLAG_MAX = 0x80000000

} ctl_retro_scaling_type_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Set/Get Retro Scaling Type
typedef struct _ctl_retro_scaling_settings_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    bool Get;                                       ///< [in][release] Set to true to get current scaling . Set to False to Set
                                                    ///< the scaling
    bool Enable;                                    ///< [in,out] State of the scaler
    ctl_retro_scaling_type_flags_t RetroScalingType;///< [out] Requested retro scaling types

} ctl_retro_scaling_settings_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Retro Scaling caps
typedef struct _ctl_retro_scaling_caps_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_retro_scaling_type_flags_t SupportedRetroScaling;   ///< [out] Supported retro scaling types

} ctl_retro_scaling_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get Supported Retro Scaling Types
/// 
/// @details
///     - Returns supported retro scaling capabilities
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pRetroScalingCaps`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetSupportedRetroScalingCapability(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to adapter
    ctl_retro_scaling_caps_t* pRetroScalingCaps     ///< [in,out][release] Query result for supported retro scaling types
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get/Set Retro Scaling
/// 
/// @details
///     - Get or Set the status of retro scaling.This Api will do a physical
///       modeset resulting in flash on the screen
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pGetSetRetroScalingType`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetSetRetroScaling(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to adapter
    ctl_retro_scaling_settings_t* pGetSetRetroScalingType   ///< [in,out][release] Get or Set the retro scaling type
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Scaling Types
typedef uint32_t ctl_scaling_type_flags_t;
typedef enum _ctl_scaling_type_flag_t
{
    CTL_SCALING_TYPE_FLAG_IDENTITY = CTL_BIT(0),    ///< No scaling is applied and display manages scaling itself when possible
    CTL_SCALING_TYPE_FLAG_CENTERED = CTL_BIT(1),    ///< Source is not scaled but place in the center of the target display
    CTL_SCALING_TYPE_FLAG_STRETCHED = CTL_BIT(2),   ///< Source is stretched to fit the target size
    CTL_SCALING_TYPE_FLAG_ASPECT_RATIO_CENTERED_MAX = CTL_BIT(3),   ///< The aspect ratio is maintained with the source centered
    CTL_SCALING_TYPE_FLAG_CUSTOM = CTL_BIT(4),      ///< None of the standard types match this .Additional parameters are
                                                    ///< required which should be set via a private driver interface
    CTL_SCALING_TYPE_FLAG_MAX = 0x80000000

} ctl_scaling_type_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Scaling caps
typedef struct _ctl_scaling_caps_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_scaling_type_flags_t SupportedScaling;      ///< [out] Supported scaling types

} ctl_scaling_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Set/Get Scaling type
typedef struct _ctl_scaling_settings_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    bool Enable;                                    ///< [in,out] State of the scaler
    ctl_scaling_type_flags_t ScalingType;           ///< [in,out] Requested scaling types
    uint32_t CustomScalingX;                        ///< [in,out] Custom Scaling X resolution
    uint32_t CustomScalingY;                        ///< [in,out] Custom Scaling Y resolution
    bool HardwareModeSet;                           ///< [in] Flag to indicate hardware modeset should be done to apply the
                                                    ///< scaling.Setting this to true would result in a flash on the screen. If
                                                    ///< this flag is set to false , API will request the OS to do a virtual
                                                    ///< modeset , but the OS can ignore this request and do a hardware modeset
                                                    ///< in some instances

} ctl_scaling_settings_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get Supported Scaling Types
/// 
/// @details
///     - Returns supported scaling capabilities
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pScalingCaps`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetSupportedScalingCapability(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_scaling_caps_t* pScalingCaps                ///< [in,out][release] Query result for supported scaling types
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get Current Scaling
/// 
/// @details
///     - Returns current active scaling
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pGetCurrentScalingType`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetCurrentScaling(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_scaling_settings_t* pGetCurrentScalingType  ///< [in,out][release] Query result for active scaling types
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Set Scaling Type
/// 
/// @details
///     - Returns current active scaling
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDisplayOutput`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pSetScalingType`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlSetCurrentScaling(
    ctl_display_output_handle_t hDisplayOutput,     ///< [in][release] Handle to display output
    ctl_scaling_settings_t* pSetScalingType         ///< [in,out][release] Set scaling types
    );


#if !defined(__GNUC__)
#pragma endregion // display
#endif
// Intel 'ctlApi' for Device Adapter - Engine groups
#if !defined(__GNUC__)
#pragma region engine
#endif
///////////////////////////////////////////////////////////////////////////////
/// @brief Accelerator engine groups
typedef enum _ctl_engine_group_t
{
    CTL_ENGINE_GROUP_GT = 0,                        ///< Access information about all engines combined.
    CTL_ENGINE_GROUP_RENDER = 1,                    ///< Access information about all render and compute engines combined.
    CTL_ENGINE_GROUP_MEDIA = 2,                     ///< Access information about all media engines combined.
    CTL_ENGINE_GROUP_MAX

} ctl_engine_group_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Engine group properties
typedef struct _ctl_engine_properties_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_engine_group_t type;                        ///< [out] The engine group

} ctl_engine_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Engine activity counters
/// 
/// @details
///     - Percent utilization is calculated by taking two snapshots (s1, s2) and
///       using the equation: %util = (s2.activeTime - s1.activeTime) /
///       (s2.timestamp - s1.timestamp)
typedef struct _ctl_engine_stats_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint64_t activeTime;                            ///< [out] Monotonic counter for time in microseconds that this resource is
                                                    ///< actively running workloads.
    uint64_t timestamp;                             ///< [out] Monotonic timestamp counter in microseconds when activeTime
                                                    ///< counter was sampled.
                                                    ///< This timestamp should only be used to calculate delta time between
                                                    ///< snapshots of this structure.
                                                    ///< Never take the delta of this timestamp with the timestamp from a
                                                    ///< different structure since they are not guaranteed to have the same base.
                                                    ///< The absolute value of the timestamp is only valid during within the
                                                    ///< application and may be different on the next execution.

} ctl_engine_stats_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get handle of engine groups
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pCount`
CTL_APIEXPORT ctl_result_t CTL_APICALL
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
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get engine group properties
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hEngine`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pProperties`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlEngineGetProperties(
    ctl_engine_handle_t hEngine,                    ///< [in] Handle for the component.
    ctl_engine_properties_t* pProperties            ///< [in,out] The properties for the specified engine group.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get the activity stats for an engine group
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hEngine`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pStats`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlEngineGetActivity(
    ctl_engine_handle_t hEngine,                    ///< [in] Handle for the component.
    ctl_engine_stats_t* pStats                      ///< [in,out] Will contain a snapshot of the engine group activity
                                                    ///< counters.
    );


#if !defined(__GNUC__)
#pragma endregion // engine
#endif
// Intel 'ctlApi' for Device Adapter- Fan management
#if !defined(__GNUC__)
#pragma region fan
#endif
///////////////////////////////////////////////////////////////////////////////
/// @brief Fan resource speed mode
typedef enum _ctl_fan_speed_mode_t
{
    CTL_FAN_SPEED_MODE_DEFAULT = 0,                 ///< The fan speed is operating using the hardware default settings
    CTL_FAN_SPEED_MODE_FIXED = 1,                   ///< The fan speed is currently set to a fixed value
    CTL_FAN_SPEED_MODE_TABLE = 2,                   ///< The fan speed is currently controlled dynamically by hardware based on
                                                    ///< a temp/speed table
    CTL_FAN_SPEED_MODE_MAX

} ctl_fan_speed_mode_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Fan speed units
typedef enum _ctl_fan_speed_units_t
{
    CTL_FAN_SPEED_UNITS_RPM = 0,                    ///< The fan speed is in units of revolutions per minute (rpm)
    CTL_FAN_SPEED_UNITS_PERCENT = 1,                ///< The fan speed is a percentage of the maximum speed of the fan
    CTL_FAN_SPEED_UNITS_MAX

} ctl_fan_speed_units_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Fan speed
typedef struct _ctl_fan_speed_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    int32_t speed;                                  ///< [in,out] The speed of the fan. On output, a value of -1 indicates that
                                                    ///< there is no fixed fan speed setting.
    ctl_fan_speed_units_t units;                    ///< [in,out] The units that the fan speed is expressed in. On output, if
                                                    ///< fan speed is -1 then units should be ignored.

} ctl_fan_speed_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Fan temperature/speed pair
typedef struct _ctl_fan_temp_speed_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint32_t temperature;                           ///< [in,out] Temperature in degrees Celsius.
    ctl_fan_speed_t speed;                          ///< [in,out] The speed of the fan

} ctl_fan_temp_speed_t;

///////////////////////////////////////////////////////////////////////////////
#ifndef CTL_FAN_TEMP_SPEED_PAIR_COUNT
/// @brief Maximum number of fan temperature/speed pairs in the fan speed table.
#define CTL_FAN_TEMP_SPEED_PAIR_COUNT  32
#endif // CTL_FAN_TEMP_SPEED_PAIR_COUNT

///////////////////////////////////////////////////////////////////////////////
/// @brief Fan speed table
typedef struct _ctl_fan_speed_table_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    int32_t numPoints;                              ///< [in,out] The number of valid points in the fan speed table. 0 means
                                                    ///< that there is no fan speed table configured. -1 means that a fan speed
                                                    ///< table is not supported by the hardware.
    ctl_fan_temp_speed_t table[CTL_FAN_TEMP_SPEED_PAIR_COUNT];  ///< [in,out] Array of temperature/fan speed pairs. The table is ordered
                                                    ///< based on temperature from lowest to highest.

} ctl_fan_speed_table_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Fan properties
typedef struct _ctl_fan_properties_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    bool canControl;                                ///< [out] Indicates if software can control the fan speed assuming the
                                                    ///< user has permissions
    uint32_t supportedModes;                        ///< [out] Bitfield of supported fan configuration modes
                                                    ///< (1<<::ctl_fan_speed_mode_t)
    uint32_t supportedUnits;                        ///< [out] Bitfield of supported fan speed units
                                                    ///< (1<<::ctl_fan_speed_units_t)
    int32_t maxRPM;                                 ///< [out] The maximum RPM of the fan. A value of -1 means that this
                                                    ///< property is unknown. 
    int32_t maxPoints;                              ///< [out] The maximum number of points in the fan temp/speed table. A
                                                    ///< value of -1 means that this fan doesn't support providing a temp/speed
                                                    ///< table.

} ctl_fan_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Fan configuration
typedef struct _ctl_fan_config_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_fan_speed_mode_t mode;                      ///< [in,out] The fan speed mode (fixed, temp-speed table)
    ctl_fan_speed_t speedFixed;                     ///< [in,out] The current fixed fan speed setting
    ctl_fan_speed_table_t speedTable;               ///< [out] A table containing temperature/speed pairs

} ctl_fan_config_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get handle of fans
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pCount`
CTL_APIEXPORT ctl_result_t CTL_APICALL
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
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get fan properties
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hFan`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pProperties`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlFanGetProperties(
    ctl_fan_handle_t hFan,                          ///< [in] Handle for the component.
    ctl_fan_properties_t* pProperties               ///< [in,out] Will contain the properties of the fan.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get fan configurations and the current fan speed mode (default, fixed,
///        temp-speed table)
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hFan`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pConfig`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlFanGetConfig(
    ctl_fan_handle_t hFan,                          ///< [in] Handle for the component.
    ctl_fan_config_t* pConfig                       ///< [in,out] Will contain the current configuration of the fan.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Configure the fan to run with hardware factory settings (set mode to
///        ::CTL_FAN_SPEED_MODE_DEFAULT)
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hFan`
///     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS
///         + User does not have permissions to make these modifications.
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlFanSetDefaultMode(
    ctl_fan_handle_t hFan                           ///< [in] Handle for the component.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Configure the fan to rotate at a fixed speed (set mode to
///        ::CTL_FAN_SPEED_MODE_FIXED)
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hFan`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == speed`
///     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS
///         + User does not have permissions to make these modifications.
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_FEATURE
///         + Fixing the fan speed not supported by the hardware or the fan speed units are not supported. See ::ctl_fan_properties_t.supportedModes and ::ctl_fan_properties_t.supportedUnits.
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlFanSetFixedSpeedMode(
    ctl_fan_handle_t hFan,                          ///< [in] Handle for the component.
    const ctl_fan_speed_t* speed                    ///< [in] The fixed fan speed setting
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Configure the fan to adjust speed based on a temperature/speed table
///        (set mode to ::CTL_FAN_SPEED_MODE_TABLE)
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hFan`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == speedTable`
///     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS
///         + User does not have permissions to make these modifications.
///     - ::CTL_RESULT_ERROR_INVALID_ARGUMENT
///         + The temperature/speed pairs in the array are not sorted on temperature from lowest to highest.
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_FEATURE
///         + Fan speed table not supported by the hardware or the fan speed units are not supported. See ::ctl_fan_properties_t.supportedModes and ::ctl_fan_properties_t.supportedUnits.
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlFanSetSpeedTableMode(
    ctl_fan_handle_t hFan,                          ///< [in] Handle for the component.
    const ctl_fan_speed_table_t* speedTable         ///< [in] A table containing temperature/speed pairs.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get current state of a fan - current mode and speed
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hFan`
///     - ::CTL_RESULT_ERROR_INVALID_ENUMERATION
///         + `::CTL_FAN_SPEED_UNITS_PERCENT < units`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pSpeed`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_FEATURE
///         + The requested fan speed units are not supported. See ::ctl_fan_properties_t.supportedUnits.
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlFanGetState(
    ctl_fan_handle_t hFan,                          ///< [in] Handle for the component.
    ctl_fan_speed_units_t units,                    ///< [in] The units in which the fan speed should be returned.
    int32_t* pSpeed                                 ///< [in,out] Will contain the current speed of the fan in the units
                                                    ///< requested. A value of -1 indicates that the fan speed cannot be
                                                    ///< measured.
    );


#if !defined(__GNUC__)
#pragma endregion // fan
#endif
// Intel 'ctlApi' for Device Adapter - Frequency domains
#if !defined(__GNUC__)
#pragma region frequency
#endif
///////////////////////////////////////////////////////////////////////////////
/// @brief Frequency domains.
typedef enum _ctl_freq_domain_t
{
    CTL_FREQ_DOMAIN_GPU = 0,                        ///< GPU Core Domain.
    CTL_FREQ_DOMAIN_MEMORY = 1,                     ///< Local Memory Domain.
    CTL_FREQ_DOMAIN_MAX

} ctl_freq_domain_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Frequency properties
typedef struct _ctl_freq_properties_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_freq_domain_t type;                         ///< [out] The hardware block that this frequency domain controls (GPU,
                                                    ///< memory, ...)
    bool canControl;                                ///< [out] Indicates if software can control the frequency of this domain
                                                    ///< assuming the user has permissions
    double min;                                     ///< [out] The minimum hardware clock frequency in units of MHz.
    double max;                                     ///< [out] The maximum non-overclock hardware clock frequency in units of
                                                    ///< MHz.

} ctl_freq_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Frequency range between which the hardware can operate. The limits can
///        be above or below the hardware limits - the hardware will clamp
///        appropriately.
typedef struct _ctl_freq_range_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    double min;                                     ///< [in,out] The min frequency in MHz below which hardware frequency
                                                    ///< management will not request frequencies. On input, setting to 0 will
                                                    ///< permit the frequency to go down to the hardware minimum. On output, a
                                                    ///< negative value indicates that no external minimum frequency limit is
                                                    ///< in effect.
    double max;                                     ///< [in,out] The max frequency in MHz above which hardware frequency
                                                    ///< management will not request frequencies. On input, setting to 0 or a
                                                    ///< very big number will permit the frequency to go all the way up to the
                                                    ///< hardware maximum. On output, a negative number indicates that no
                                                    ///< external maximum frequency limit is in effect.

} ctl_freq_range_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Frequency throttle reasons
typedef uint32_t ctl_freq_throttle_reason_flags_t;
typedef enum _ctl_freq_throttle_reason_flag_t
{
    CTL_FREQ_THROTTLE_REASON_FLAG_AVE_PWR_CAP = CTL_BIT(0), ///< frequency throttled due to average power excursion (PL1)
    CTL_FREQ_THROTTLE_REASON_FLAG_BURST_PWR_CAP = CTL_BIT(1),   ///< frequency throttled due to burst power excursion (PL2)
    CTL_FREQ_THROTTLE_REASON_FLAG_CURRENT_LIMIT = CTL_BIT(2),   ///< frequency throttled due to current excursion (PL4)
    CTL_FREQ_THROTTLE_REASON_FLAG_THERMAL_LIMIT = CTL_BIT(3),   ///< frequency throttled due to thermal excursion (T > TjMax)
    CTL_FREQ_THROTTLE_REASON_FLAG_PSU_ALERT = CTL_BIT(4),   ///< frequency throttled due to power supply assertion
    CTL_FREQ_THROTTLE_REASON_FLAG_SW_RANGE = CTL_BIT(5),///< frequency throttled due to software supplied frequency range
    CTL_FREQ_THROTTLE_REASON_FLAG_HW_RANGE = CTL_BIT(6),///< frequency throttled due to a sub block that has a lower frequency
                                                    ///< range when it receives clocks
    CTL_FREQ_THROTTLE_REASON_FLAG_MAX = 0x80000000

} ctl_freq_throttle_reason_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Frequency state
typedef struct _ctl_freq_state_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    double currentVoltage;                          ///< [out] Current voltage in Volts. A negative value indicates that this
                                                    ///< property is not known.
    double request;                                 ///< [out] The current frequency request in MHz. A negative value indicates
                                                    ///< that this property is not known.
    double tdp;                                     ///< [out] The maximum frequency in MHz supported under the current TDP
                                                    ///< conditions. This fluctuates dynamically based on the power and thermal
                                                    ///< limits of the part. A negative value indicates that this property is
                                                    ///< not known.
    double efficient;                               ///< [out] The efficient minimum frequency in MHz. A negative value
                                                    ///< indicates that this property is not known.
    double actual;                                  ///< [out] The resolved frequency in MHz. A negative value indicates that
                                                    ///< this property is not known.
    ctl_freq_throttle_reason_flags_t throttleReasons;   ///< [out] The reasons that the frequency is being limited by the hardware.
                                                    ///< Returns 0 (frequency not throttled) or a combination of ::ctl_freq_throttle_reason_flag_t.

} ctl_freq_state_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Frequency throttle time snapshot
/// 
/// @details
///     - Percent time throttled is calculated by taking two snapshots (s1, s2)
///       and using the equation: %throttled = (s2.throttleTime -
///       s1.throttleTime) / (s2.timestamp - s1.timestamp)
typedef struct _ctl_freq_throttle_time_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint64_t throttleTime;                          ///< [out] The monotonic counter of time in microseconds that the frequency
                                                    ///< has been limited by the hardware.
    uint64_t timestamp;                             ///< [out] Microsecond timestamp when throttleTime was captured.
                                                    ///< This timestamp should only be used to calculate delta time between
                                                    ///< snapshots of this structure.
                                                    ///< Never take the delta of this timestamp with the timestamp from a
                                                    ///< different structure since they are not guaranteed to have the same base.
                                                    ///< The absolute value of the timestamp is only valid during within the
                                                    ///< application and may be different on the next execution.

} ctl_freq_throttle_time_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get handle of frequency domains
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pCount`
CTL_APIEXPORT ctl_result_t CTL_APICALL
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
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get frequency properties - available frequencies
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hFrequency`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pProperties`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlFrequencyGetProperties(
    ctl_freq_handle_t hFrequency,                   ///< [in] Handle for the component.
    ctl_freq_properties_t* pProperties              ///< [in,out] The frequency properties for the specified domain.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get available non-overclocked hardware clock frequencies for the
///        frequency domain
/// 
/// @details
///     - The list of available frequencies is returned in order of slowest to
///       fastest.
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hFrequency`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pCount`
CTL_APIEXPORT ctl_result_t CTL_APICALL
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
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get current frequency limits
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hFrequency`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pLimits`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlFrequencyGetRange(
    ctl_freq_handle_t hFrequency,                   ///< [in] Handle for the component.
    ctl_freq_range_t* pLimits                       ///< [in,out] The range between which the hardware can operate for the
                                                    ///< specified domain.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Set frequency range between which the hardware can operate.
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hFrequency`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pLimits`
///     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS
///         + User does not have permissions to make these modifications.
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlFrequencySetRange(
    ctl_freq_handle_t hFrequency,                   ///< [in] Handle for the component.
    const ctl_freq_range_t* pLimits                 ///< [in] The limits between which the hardware can operate for the
                                                    ///< specified domain.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get current frequency state - frequency request, actual frequency, TDP
///        limits
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hFrequency`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pState`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlFrequencyGetState(
    ctl_freq_handle_t hFrequency,                   ///< [in] Handle for the component.
    ctl_freq_state_t* pState                        ///< [in,out] Frequency state for the specified domain.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get frequency throttle time
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hFrequency`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pThrottleTime`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlFrequencyGetThrottleTime(
    ctl_freq_handle_t hFrequency,                   ///< [in] Handle for the component.
    ctl_freq_throttle_time_t* pThrottleTime         ///< [in,out] Will contain a snapshot of the throttle time counters for the
                                                    ///< specified domain.
    );


#if !defined(__GNUC__)
#pragma endregion // frequency
#endif
// Intel 'ctlApi' for Device Adapter
#if !defined(__GNUC__)
#pragma region media
#endif
///////////////////////////////////////////////////////////////////////////////
/// @brief Feature type
typedef enum _ctl_video_processing_feature_t
{
    CTL_VIDEO_PROCESSING_FEATURE_FILM_MODE_DETECTION = 0,   ///< Film mode detection.  Contains CTL_PROPERTY_VALUE_TYPE_BOOL ValueType.
    CTL_VIDEO_PROCESSING_FEATURE_NOISE_REDUCTION = 1,   ///< Noise reduction.  Contains CTL_PROPERTY_VALUE_TYPE_CUSTOM type field
                                                    ///< using struct ::ctl_video_processing_noise_reduction_t.
    CTL_VIDEO_PROCESSING_FEATURE_SHARPNESS = 2,     ///< Sharpness.  Contains CTL_PROPERTY_VALUE_TYPE_UINT32 ValueType.
    CTL_VIDEO_PROCESSING_FEATURE_ADAPTIVE_CONTRAST_ENHANCEMENT = 3, ///< Adaptive contrast enhancement.  Contains
                                                    ///< CTL_PROPERTY_VALUE_TYPE_CUSTOM type field using struct
                                                    ///< ::ctl_video_processing_adaptive_contrast_enhancement_t.
    CTL_VIDEO_PROCESSING_FEATURE_SUPER_RESOLUTION = 4,  ///< Super resolution.  Contains CTL_PROPERTY_VALUE_TYPE_ENUM ValueType
                                                    ///< using ::ctl_video_processing_super_resolution_flags_t.
    CTL_VIDEO_PROCESSING_FEATURE_STANDARD_COLOR_CORRECTION = 5, ///< Standard color correction.  Controls Hue, Saturation, Contrast,
                                                    ///< Brightness.  Contains CTL_PROPERTY_VALUE_TYPE_CUSTOM type field using
                                                    ///< struct ::ctl_video_processing_standard_color_correction_t.
    CTL_VIDEO_PROCESSING_FEATURE_TOTAL_COLOR_CORRECTION = 6,///< Total color correction.  Controls Red, Green, Blue, Yellow, Cyan,
                                                    ///< Magenta.  Contains CTL_PROPERTY_VALUE_TYPE_CUSTOM type field using
                                                    ///< struct ::ctl_video_processing_total_color_correction_t.
    CTL_VIDEO_PROCESSING_FEATURE_SKIN_TONE_ENHANCEMENT = 7, ///< Skin tone enhancement.  Contains CTL_PROPERTY_VALUE_TYPE_UINT32
                                                    ///< ValueType.
    CTL_VIDEO_PROCESSING_FEATURE_MAX

} ctl_video_processing_feature_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Super resolution values possible
typedef uint32_t ctl_video_processing_super_resolution_flags_t;
typedef enum _ctl_video_processing_super_resolution_flag_t
{
    CTL_VIDEO_PROCESSING_SUPER_RESOLUTION_FLAG_DISABLE = CTL_BIT(0),///< Disable
    CTL_VIDEO_PROCESSING_SUPER_RESOLUTION_FLAG_ENABLE_DEFAULT_SCENARIO_MODE = CTL_BIT(1),   ///< Enable with default super resolution mode
    CTL_VIDEO_PROCESSING_SUPER_RESOLUTION_FLAG_ENABLE_CONFERENCE_SCENARIO_MODE = CTL_BIT(2),///< Super resolution mode targeted at video conference content
    CTL_VIDEO_PROCESSING_SUPER_RESOLUTION_FLAG_ENABLE_CAMERA_SCENARIO_MODE = CTL_BIT(3),///< Super resolution mode targeted at camera capture content (e.g.
                                                    ///< security camera)
    CTL_VIDEO_PROCESSING_SUPER_RESOLUTION_FLAG_MAX = 0x80000000

} ctl_video_processing_super_resolution_flag_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Noise Reduction feature details structure to be used with
///        NOISE_REDUCTION
typedef struct _ctl_video_processing_noise_reduction_info_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_property_info_uint_t noise_reduction;       ///< [in,out] Noise reduction min, max, default and step size information
    bool noise_reduction_auto_detect_supported;     ///< [in,out] Noise reduction Auto Detect is supported; only valid if
                                                    ///< NOISE_REDUCTION is enabled.  If enabled, noise reduction level is
                                                    ///< automatically determined and set value is not used.
    ctl_property_info_boolean_t noise_reduction_auto_detect;///< [in,out] Noise reduction auto detect default information
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_video_processing_noise_reduction_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Noise Reduction Get/Set structure to be used with NOISE_REDUCTION
typedef struct _ctl_video_processing_noise_reduction_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_property_uint_t noise_reduction;            ///< [in,out] Noise reduction enable and value setting
    ctl_property_boolean_t noise_reduction_auto_detect; ///< [in,out] Noise reduction auto detect setting
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_video_processing_noise_reduction_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Adaptive Contrast Enhancement feature details structure to be used
///        with ADAPTIVE_CONTRAST_ENHANCEMENT
typedef struct _ctl_video_processing_adaptive_contrast_enhancement_info_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_property_info_uint_t adaptive_contrast_enhancement; ///< [in,out] Adaptive Contrast Enhancement min, max, default and step size
                                                    ///< information
    bool adaptive_contrast_enhancement_coexistence_supported;   ///< [in,out] Adaptive contrast enhancement coexistance is supported; only
                                                    ///< valid if ADAPTIVE_CONTRAST_ENHANCEMENT is enabled.  If enabled, Video
                                                    ///< adaptive contrast ehancement will be allowed to be enabled and coexist
                                                    ///< with Display adaptive contrast ehancement feature.
    ctl_property_info_boolean_t adaptive_contrast_enhancement_coexistence;  ///< [in,out] Adaptive contrast enhancement coexistence default information
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_video_processing_adaptive_contrast_enhancement_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Adaptive Contrast Enhancement Get/Set structure to be used with
///        ADAPTIVE_CONTRAST_ENHANCEMENT
typedef struct _ctl_video_processing_adaptive_contrast_enhancement_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_property_uint_t adaptive_contrast_enhancement;  ///< [in,out] Adaptive Contrast Enhancement enable and value setting
    ctl_property_boolean_t adaptive_contrast_enhancement_coexistence;   ///< [in,out] Adaptive contrast enhancement coexistance setting
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_video_processing_adaptive_contrast_enhancement_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Standard Color Correction feature details structure to be used with
///        STANDARD_COLOR_CORRECTION
typedef struct _ctl_video_processing_standard_color_correction_info_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    bool standard_color_correction_default_enable;  ///< [in,out] STANDARD_COLOR_CORRECTION default enable setting.  This
                                                    ///< global settings controls all of Hue, Saturation, Contrast, Brightness
                                                    ///< enabling.  Individual Enable controls shall be ignored.
    ctl_property_info_float_t brightness;           ///< [in,out] Brightness min, max, default and step size information
    ctl_property_info_float_t contrast;             ///< [in,out] Contrast min, max, default and step size information
    ctl_property_info_float_t hue;                  ///< [in,out] Hue min, max, default and step size information
    ctl_property_info_float_t saturation;           ///< [in,out] Saturation min, max, default and step size information
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_video_processing_standard_color_correction_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Standard Color Correction Get/Set structure to be used with
///        STANDARD_COLOR_CORRECTION
typedef struct _ctl_video_processing_standard_color_correction_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    bool standard_color_correction_enable;          ///< [in,out] STANDARD_COLOR_CORRECTION enable setting.  This global
                                                    ///< setting controls all of Hue, Saturation, Contrast, Brightness
                                                    ///< enabling.
    float brightness;                               ///< [in,out] Brightness value
    float contrast;                                 ///< [in,out] Contrast value
    float hue;                                      ///< [in,out] Hue value
    float saturation;                               ///< [in,out] Saturation value
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_video_processing_standard_color_correction_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Total Color Correction Get/Set structure to be used with
///        TOTAL_COLOR_CORRECTION
typedef struct _ctl_video_processing_total_color_correction_info_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    bool total_color_correction_default_enable;     ///< [in,out] TOTAL_COLOR_CORRECTION enable setting.  This global setting
                                                    ///< controls all of Red, Green, Blue, Yellow, Cyan, Magenta enabling. 
                                                    ///< Individual Enable controls shall be ignored.
    ctl_property_info_uint_t red;                   ///< [in,out] Red min, max, default and step size information
    ctl_property_info_uint_t green;                 ///< [in,out] Green min, max, default and step size information
    ctl_property_info_uint_t blue;                  ///< [in,out] Blue min, max, default and step size information
    ctl_property_info_uint_t yellow;                ///< [in,out] Yellow min, max, default and step size information
    ctl_property_info_uint_t cyan;                  ///< [in,out] Cyan min, max, default and step size information
    ctl_property_info_uint_t magenta;               ///< [in,out] Magenta min, max, default and step size information
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_video_processing_total_color_correction_info_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Total Color Correction Get/Set structure to be used with
///        TOTAL_COLOR_CORRECTION
typedef struct _ctl_video_processing_total_color_correction_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    bool total_color_correction_enable;             ///< [in,out] TOTAL_COLOR_CORRECTION enable setting.  This global setting
                                                    ///< controls all of Red, Green, Blue, Yellow, Cyan, Magenta enabling.
    uint32_t red;                                   ///< [in,out] Red value
    uint32_t green;                                 ///< [in,out] Green value
    uint32_t blue;                                  ///< [in,out] Blue value
    uint32_t yellow;                                ///< [in,out] Yellow value
    uint32_t cyan;                                  ///< [in,out] Cyan value
    uint32_t magenta;                               ///< [in,out] Magenta value
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_video_processing_total_color_correction_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Video Processing feature details which will have range supported and
///        default values
typedef struct _ctl_video_processing_feature_details_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_video_processing_feature_t FeatureType;     ///< [out] Video processing feature type
    ctl_property_value_type_t ValueType;            ///< [out] Type of value
    ctl_property_info_t Value;                      ///< [out] Union of various type of values for Video Processing features.
                                                    ///< For enum types this can be noise reduction, color control etc. This
                                                    ///< member is valid iff ValueType is not CTL_PROPERTY_VALUE_TYPE_CUSTOM
    int32_t CustomValueSize;                        ///< [in] CustomValue buffer size
    void* pCustomValue;                             ///< [in,out] Pointer to a custom structure. Features that use CustomType,
                                                    ///< after the first query for all of the supported features the user needs
                                                    ///< to allocate this buffer and then query again just this specific
                                                    ///< feature for the structure to be filled in. Caller should allocate this
                                                    ///< buffer with known custom feature structure size. This member is valid
                                                    ///< iff ValueType is CTL_PROPERTY_VALUE_TYPE_CUSTOM.
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_video_processing_feature_details_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Video Processing features which are controllable
typedef struct _ctl_video_processing_feature_caps_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint32_t NumSupportedFeatures;                  ///< [in,out] Number of elements in supported features array
    ctl_video_processing_feature_details_t* pFeatureDetails;///< [in,out] Array of supported features and their details
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_video_processing_feature_caps_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Video Processing feature for get/set
typedef struct _ctl_video_processing_feature_getset_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_video_processing_feature_t FeatureType;     ///< [in] Features interested in
    char* ApplicationName;                          ///< [in] Application name for which the property type is applicable. If
                                                    ///< this is an empty string then this will get/set global settings for the
                                                    ///< given adapter. Note that this should contain only the name of the
                                                    ///< application and not the system specific path.  [This is not currently
                                                    ///< supported and should be an empty string.]
    int8_t ApplicationNameLength;                   ///< [in] Length of ApplicationName string
    bool bSet;                                      ///< [in] Set this if it's a set call
    ctl_property_value_type_t ValueType;            ///< [in] Type of value. Caller has to ensure it provides the right value
                                                    ///< type which decides how one read the union structure below
    ctl_property_t Value;                           ///< [in,out] Union of various type of values for Video Processing
                                                    ///< features. For enum types this can be noise reduction, color control
                                                    ///< etc. This member is valid iff ValueType is not
                                                    ///< CTL_PROPERTY_VALUE_TYPE_CUSTOM
    int32_t CustomValueSize;                        ///< [in] CustomValue buffer size.  For a feature requiring custom struct,
                                                    ///< caller will know of it upfront the struct to use based on the feautre
                                                    ///< and can provide the right size info here
    void* pCustomValue;                             ///< [in,out] Pointer to a custom structure. Caller should allocate this
                                                    ///< buffer with known custom feature structure size. This member is valid
                                                    ///< iff ValueType is CTL_PROPERTY_VALUE_TYPE_CUSTOM
    uint32_t ReservedFields[16];                    ///< [out] Reserved field of 64 bytes

} ctl_video_processing_feature_getset_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get Video Processing capabilities
/// 
/// @details
///     - The application gets Video Processing properties
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pFeatureCaps`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetSupportedVideoProcessingCapabilities(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    ctl_video_processing_feature_caps_t* pFeatureCaps   ///< [in,out][release] Video Processing properties
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get/Set Video Processing feature details
/// 
/// @details
///     - Video Processing feature details
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pFeature`
///     - ::CTL_RESULT_ERROR_UNSUPPORTED_VERSION - "Unsupported version"
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlGetSetVideoProcessingFeature(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    ctl_video_processing_feature_getset_t* pFeature ///< [in][release] Video Processing feature get/set parameter
    );


#if !defined(__GNUC__)
#pragma endregion // media
#endif
// Intel 'ctlApi' for Device Adapter - Memory management
#if !defined(__GNUC__)
#pragma region memory
#endif
///////////////////////////////////////////////////////////////////////////////
/// @brief Memory module types
typedef enum _ctl_mem_type_t
{
    CTL_MEM_TYPE_HBM = 0,                           ///< HBM memory
    CTL_MEM_TYPE_DDR = 1,                           ///< DDR memory
    CTL_MEM_TYPE_DDR3 = 2,                          ///< DDR3 memory
    CTL_MEM_TYPE_DDR4 = 3,                          ///< DDR4 memory
    CTL_MEM_TYPE_DDR5 = 4,                          ///< DDR5 memory
    CTL_MEM_TYPE_LPDDR = 5,                         ///< LPDDR memory
    CTL_MEM_TYPE_LPDDR3 = 6,                        ///< LPDDR3 memory
    CTL_MEM_TYPE_LPDDR4 = 7,                        ///< LPDDR4 memory
    CTL_MEM_TYPE_LPDDR5 = 8,                        ///< LPDDR5 memory
    CTL_MEM_TYPE_MAX

} ctl_mem_type_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Memory module location
typedef enum _ctl_mem_loc_t
{
    CTL_MEM_LOC_SYSTEM = 0,                         ///< System memory
    CTL_MEM_LOC_DEVICE = 1,                         ///< On board local device memory
    CTL_MEM_LOC_MAX

} ctl_mem_loc_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Memory properties
typedef struct _ctl_mem_properties_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_mem_type_t type;                            ///< [out] The memory type
    ctl_mem_loc_t location;                         ///< [out] Location of this memory (system, device)
    uint64_t physicalSize;                          ///< [out] Physical memory size in bytes. A value of 0 indicates that this
                                                    ///< property is not known. However, a call to ::ctlMemoryGetState() will
                                                    ///< correctly return the total size of usable memory.
    int32_t busWidth;                               ///< [out] Width of the memory bus. A value of -1 means that this property
                                                    ///< is unknown.
    int32_t numChannels;                            ///< [out] The number of memory channels. A value of -1 means that this
                                                    ///< property is unknown.

} ctl_mem_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Memory state - health, allocated
/// 
/// @details
///     - Percent allocation is given by 100 * (size - free / size.
///     - Percent free is given by 100 * free / size.
typedef struct _ctl_mem_state_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint64_t free;                                  ///< [out] The free memory in bytes
    uint64_t size;                                  ///< [out] The total allocatable memory in bytes (can be less than
                                                    ///< ::ctl_mem_properties_t.physicalSize)

} ctl_mem_state_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Memory bandwidth
/// 
/// @details
///     - Percent bandwidth is calculated by taking two snapshots (s1, s2) and
///       using the equation: %bw = 10^6 * ((s2.readCounter - s1.readCounter) +
///       (s2.writeCounter - s1.writeCounter)) / (s2.maxBandwidth *
///       (s2.timestamp - s1.timestamp))
typedef struct _ctl_mem_bandwidth_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint64_t maxBandwidth;                          ///< [out] Current maximum bandwidth in units of bytes/sec
    uint64_t timestamp;                             ///< [out] The timestamp when these measurements were sampled.
                                                    ///< This timestamp should only be used to calculate delta time between
                                                    ///< snapshots of this structure.
                                                    ///< Never take the delta of this timestamp with the timestamp from a
                                                    ///< different structure since they are not guaranteed to have the same base.
                                                    ///< The absolute value of the timestamp is only valid during within the
                                                    ///< application and may be different on the next execution.

} ctl_mem_bandwidth_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get handle of memory modules
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pCount`
CTL_APIEXPORT ctl_result_t CTL_APICALL
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
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get memory properties
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hMemory`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pProperties`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlMemoryGetProperties(
    ctl_mem_handle_t hMemory,                       ///< [in] Handle for the component.
    ctl_mem_properties_t* pProperties               ///< [in,out] Will contain memory properties.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get memory state - health, allocated
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hMemory`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pState`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlMemoryGetState(
    ctl_mem_handle_t hMemory,                       ///< [in] Handle for the component.
    ctl_mem_state_t* pState                         ///< [in,out] Will contain the current health and allocated memory.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get memory bandwidth
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hMemory`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pBandwidth`
///     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS
///         + User does not have permissions to query this telemetry.
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlMemoryGetBandwidth(
    ctl_mem_handle_t hMemory,                       ///< [in] Handle for the component.
    ctl_mem_bandwidth_t* pBandwidth                 ///< [in,out] Will contain the current health, free memory, total memory
                                                    ///< size.
    );


#if !defined(__GNUC__)
#pragma endregion // memory
#endif
// Intel 'ctlApi' for Device Adapter - PCI Information
#if !defined(__GNUC__)
#pragma region pci
#endif
///////////////////////////////////////////////////////////////////////////////
/// @brief PCI address
typedef struct _ctl_pci_address_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint32_t domain;                                ///< [out] BDF domain
    uint32_t bus;                                   ///< [out] BDF bus
    uint32_t device;                                ///< [out] BDF device
    uint32_t function;                              ///< [out] BDF function

} ctl_pci_address_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief PCI speed
typedef struct _ctl_pci_speed_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    int32_t gen;                                    ///< [out] The link generation. A value of -1 means that this property is
                                                    ///< unknown.
    int32_t width;                                  ///< [out] The number of lanes. A value of -1 means that this property is
                                                    ///< unknown.
    int64_t maxBandwidth;                           ///< [out] The maximum bandwidth in bytes/sec (sum of all lanes). A value
                                                    ///< of -1 means that this property is unknown.

} ctl_pci_speed_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Static PCI properties
typedef struct _ctl_pci_properties_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_pci_address_t address;                      ///< [out] The BDF address
    ctl_pci_speed_t maxSpeed;                       ///< [out] Fastest port configuration supported by the device (sum of all
                                                    ///< lanes)
    bool resizable_bar_supported;                   ///< [out] Support for Resizable Bar on this device.
    bool resizable_bar_enabled;                     ///< [out] Resizable Bar enabled on this device

} ctl_pci_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Dynamic PCI state
typedef struct _ctl_pci_state_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_pci_speed_t speed;                          ///< [out] The current port configure speed

} ctl_pci_state_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get PCI properties - address, max speed
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pProperties`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlPciGetProperties(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    ctl_pci_properties_t* pProperties               ///< [in,out] Will contain the PCI properties.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get current PCI state - current speed
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pState`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlPciGetState(
    ctl_device_adapter_handle_t hDAhandle,          ///< [in][release] Handle to display adapter
    ctl_pci_state_t* pState                         ///< [in,out] Will contain the PCI properties.
    );


#if !defined(__GNUC__)
#pragma endregion // pci
#endif
// Intel 'ctlApi' for Device Adapter - Power management
#if !defined(__GNUC__)
#pragma region power
#endif
///////////////////////////////////////////////////////////////////////////////
/// @brief Properties related to device power settings
typedef struct _ctl_power_properties_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    bool canControl;                                ///< [out] Software can change the power limits of this domain assuming the
                                                    ///< user has permissions.
    int32_t defaultLimit;                           ///< [out] The factory default TDP power limit of the part in milliwatts. A
                                                    ///< value of -1 means that this is not known.
    int32_t minLimit;                               ///< [out] The minimum power limit in milliwatts that can be requested.
    int32_t maxLimit;                               ///< [out] The maximum power limit in milliwatts that can be requested.

} ctl_power_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Energy counter snapshot
/// 
/// @details
///     - Average power is calculated by taking two snapshots (s1, s2) and using
///       the equation: PowerWatts = (s2.energy - s1.energy) / (s2.timestamp -
///       s1.timestamp)
typedef struct _ctl_power_energy_counter_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    uint64_t energy;                                ///< [out] The monotonic energy counter in microjoules.
    uint64_t timestamp;                             ///< [out] Microsecond timestamp when energy was captured.
                                                    ///< This timestamp should only be used to calculate delta time between
                                                    ///< snapshots of this structure.
                                                    ///< Never take the delta of this timestamp with the timestamp from a
                                                    ///< different structure since they are not guaranteed to have the same base.
                                                    ///< The absolute value of the timestamp is only valid during within the
                                                    ///< application and may be different on the next execution.

} ctl_power_energy_counter_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Sustained power limits
/// 
/// @details
///     - The power controller (Punit) will throttle the operating frequency if
///       the power averaged over a window (typically seconds) exceeds this
///       limit.
typedef struct _ctl_power_sustained_limit_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    bool enabled;                                   ///< [in,out] indicates if the limit is enabled (true) or ignored (false)
    int32_t power;                                  ///< [in,out] power limit in milliwatts
    int32_t interval;                               ///< [in,out] power averaging window (Tau) in milliseconds

} ctl_power_sustained_limit_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Burst power limit
/// 
/// @details
///     - The power controller (Punit) will throttle the operating frequency of
///       the device if the power averaged over a few milliseconds exceeds a
///       limit known as PL2. Typically PL2 > PL1 so that it permits the
///       frequency to burst higher for short periods than would be otherwise
///       permitted by PL1.
typedef struct _ctl_power_burst_limit_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    bool enabled;                                   ///< [in,out] indicates if the limit is enabled (true) or ignored (false)
    int32_t power;                                  ///< [in,out] power limit in milliwatts

} ctl_power_burst_limit_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Peak power limit
/// 
/// @details
///     - The power controller (Punit) will preemptively throttle the operating
///       frequency of the device when the instantaneous power exceeds this
///       limit. The limit is known as PL4. It expresses the maximum power that
///       can be drawn from the power supply.
///     - If this power limit is removed or set too high, the power supply will
///       generate an interrupt when it detects an overcurrent condition and the
///       power controller will throttle the device frequencies down to min. It
///       is thus better to tune the PL4 value in order to avoid such
///       excursions.
typedef struct _ctl_power_peak_limit_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    int32_t powerAC;                                ///< [in,out] power limit in milliwatts for the AC power source.
    int32_t powerDC;                                ///< [in,out] power limit in milliwatts for the DC power source. On input,
                                                    ///< this is ignored if the product does not have a battery. On output,
                                                    ///< this will be -1 if the product does not have a battery.

} ctl_power_peak_limit_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Energy threshold
/// 
/// @details
///     - .
typedef struct _ctl_energy_threshold_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    bool enable;                                    ///< [in,out] Indicates if the energy threshold is enabled.
    double threshold;                               ///< [in,out] The energy threshold in Joules. Will be 0.0 if no threshold
                                                    ///< has been set.
    uint32_t processId;                             ///< [in,out] The host process ID that set the energy threshold. Will be
                                                    ///< 0xFFFFFFFF if no threshold has been set.

} ctl_energy_threshold_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get handle of power domains
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pCount`
CTL_APIEXPORT ctl_result_t CTL_APICALL
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
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get properties related to a power domain
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hPower`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pProperties`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlPowerGetProperties(
    ctl_pwr_handle_t hPower,                        ///< [in] Handle for the component.
    ctl_power_properties_t* pProperties             ///< [in,out] Structure that will contain property data.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get energy counter
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hPower`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pEnergy`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlPowerGetEnergyCounter(
    ctl_pwr_handle_t hPower,                        ///< [in] Handle for the component.
    ctl_power_energy_counter_t* pEnergy             ///< [in,out] Will contain the latest snapshot of the energy counter and
                                                    ///< timestamp when the last counter value was measured.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get power limits
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hPower`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlPowerGetLimits(
    ctl_pwr_handle_t hPower,                        ///< [in] Handle for the component.
    ctl_power_sustained_limit_t* pSustained,        ///< [in,out][optional] The sustained power limit. If this is null, the
                                                    ///< current sustained power limits will not be returned.
    ctl_power_burst_limit_t* pBurst,                ///< [in,out][optional] The burst power limit. If this is null, the current
                                                    ///< peak power limits will not be returned.
    ctl_power_peak_limit_t* pPeak                   ///< [in,out][optional] The peak power limit. If this is null, the peak
                                                    ///< power limits will not be returned.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Set power limits
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hPower`
///     - ::CTL_RESULT_ERROR_INSUFFICIENT_PERMISSIONS
///         + User does not have permissions to make these modifications.
///     - ::CTL_RESULT_ERROR_NOT_AVAILABLE
///         + The device is in use, meaning that the GPU is under Over clocking, applying power limits under overclocking is not supported.
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlPowerSetLimits(
    ctl_pwr_handle_t hPower,                        ///< [in] Handle for the component.
    const ctl_power_sustained_limit_t* pSustained,  ///< [in][optional] The sustained power limit. If this is null, no changes
                                                    ///< will be made to the sustained power limits.
    const ctl_power_burst_limit_t* pBurst,          ///< [in][optional] The burst power limit. If this is null, no changes will
                                                    ///< be made to the burst power limits.
    const ctl_power_peak_limit_t* pPeak             ///< [in][optional] The peak power limit. If this is null, no changes will
                                                    ///< be made to the peak power limits.
    );


#if !defined(__GNUC__)
#pragma endregion // power
#endif
// Intel 'ctlApi' for Device Adapter - Temperature Sensors
#if !defined(__GNUC__)
#pragma region temperature
#endif
///////////////////////////////////////////////////////////////////////////////
/// @brief Temperature sensors
typedef enum _ctl_temp_sensors_t
{
    CTL_TEMP_SENSORS_GLOBAL = 0,                    ///< The maximum temperature across all device sensors
    CTL_TEMP_SENSORS_GPU = 1,                       ///< The maximum temperature across all sensors in the GPU
    CTL_TEMP_SENSORS_MEMORY = 2,                    ///< The maximum temperature across all sensors in the local memory
    CTL_TEMP_SENSORS_GLOBAL_MIN = 3,                ///< The minimum temperature across all device sensors
    CTL_TEMP_SENSORS_GPU_MIN = 4,                   ///< The minimum temperature across all sensors in the GPU
    CTL_TEMP_SENSORS_MEMORY_MIN = 5,                ///< The minimum temperature across all sensors in the local device memory
    CTL_TEMP_SENSORS_MAX

} ctl_temp_sensors_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Temperature sensor properties
typedef struct _ctl_temp_properties_t
{
    uint32_t Size;                                  ///< [in] size of this structure
    uint8_t Version;                                ///< [in] version of this structure
    ctl_temp_sensors_t type;                        ///< [out] Which part of the device the temperature sensor measures
    double maxTemperature;                          ///< [out] Will contain the maximum temperature for the specific device in
                                                    ///< degrees Celsius.

} ctl_temp_properties_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Get handle of temperature sensors
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hDAhandle`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pCount`
CTL_APIEXPORT ctl_result_t CTL_APICALL
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
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get temperature sensor properties
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hTemperature`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pProperties`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlTemperatureGetProperties(
    ctl_temp_handle_t hTemperature,                 ///< [in] Handle for the component.
    ctl_temp_properties_t* pProperties              ///< [in,out] Will contain the temperature sensor properties.
    );

///////////////////////////////////////////////////////////////////////////////
/// @brief Get the temperature from a specified sensor
/// 
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
/// 
/// @returns
///     - ::CTL_RESULT_SUCCESS
///     - ::CTL_RESULT_ERROR_UNINITIALIZED
///     - ::CTL_RESULT_ERROR_DEVICE_LOST
///     - ::CTL_RESULT_ERROR_INVALID_NULL_HANDLE
///         + `nullptr == hTemperature`
///     - ::CTL_RESULT_ERROR_INVALID_NULL_POINTER
///         + `nullptr == pTemperature`
CTL_APIEXPORT ctl_result_t CTL_APICALL
ctlTemperatureGetState(
    ctl_temp_handle_t hTemperature,                 ///< [in] Handle for the component.
    double* pTemperature                            ///< [in,out] Will contain the temperature read from the specified sensor
                                                    ///< in degrees Celsius.
    );


#if !defined(__GNUC__)
#pragma endregion // temperature
#endif


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlInit 
typedef ctl_result_t (CTL_APICALL *ctl_pfnInit_t)(
    ctl_init_args_t*,
    ctl_api_handle_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlClose 
typedef ctl_result_t (CTL_APICALL *ctl_pfnClose_t)(
    ctl_api_handle_t
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlWaitForPropertyChange 
typedef ctl_result_t (CTL_APICALL *ctl_pfnWaitForPropertyChange_t)(
    ctl_device_adapter_handle_t,
    ctl_wait_property_change_args_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlReservedCall 
typedef ctl_result_t (CTL_APICALL *ctl_pfnReservedCall_t)(
    ctl_device_adapter_handle_t,
    ctl_reserved_args_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetSupported3DCapabilities 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetSupported3DCapabilities_t)(
    ctl_device_adapter_handle_t,
    ctl_3d_feature_caps_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetSet3DFeature 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetSet3DFeature_t)(
    ctl_device_adapter_handle_t,
    ctl_3d_feature_getset_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlCheckDriverVersion 
typedef ctl_result_t (CTL_APICALL *ctl_pfnCheckDriverVersion_t)(
    ctl_device_adapter_handle_t,
    ctl_version_info_t
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlEnumerateDevices 
typedef ctl_result_t (CTL_APICALL *ctl_pfnEnumerateDevices_t)(
    ctl_api_handle_t,
    uint32_t*,
    ctl_device_adapter_handle_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlEnumerateDisplayOutputs 
typedef ctl_result_t (CTL_APICALL *ctl_pfnEnumerateDisplayOutputs_t)(
    ctl_device_adapter_handle_t,
    uint32_t*,
    ctl_display_output_handle_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetDeviceProperties 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetDeviceProperties_t)(
    ctl_device_adapter_handle_t,
    ctl_device_adapter_properties_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetDisplayProperties 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetDisplayProperties_t)(
    ctl_display_output_handle_t,
    ctl_display_properties_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetAdaperDisplayEncoderProperties 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetAdaperDisplayEncoderProperties_t)(
    ctl_display_output_handle_t,
    ctl_adapter_display_encoder_properties_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetZeDevice 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetZeDevice_t)(
    ctl_device_adapter_handle_t,
    void*,
    void**
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetSharpnessCaps 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetSharpnessCaps_t)(
    ctl_display_output_handle_t,
    ctl_sharpness_caps_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetCurrentSharpness 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetCurrentSharpness_t)(
    ctl_display_output_handle_t,
    ctl_sharpness_settings_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlSetCurrentSharpness 
typedef ctl_result_t (CTL_APICALL *ctl_pfnSetCurrentSharpness_t)(
    ctl_display_output_handle_t,
    ctl_sharpness_settings_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlI2CAccess 
typedef ctl_result_t (CTL_APICALL *ctl_pfnI2CAccess_t)(
    ctl_display_output_handle_t,
    ctl_i2c_access_args_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlAUXAccess 
typedef ctl_result_t (CTL_APICALL *ctl_pfnAUXAccess_t)(
    ctl_display_output_handle_t,
    ctl_aux_access_args_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetPowerOptimizationCaps 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetPowerOptimizationCaps_t)(
    ctl_display_output_handle_t,
    ctl_power_optimization_caps_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetPowerOptimizationSetting 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetPowerOptimizationSetting_t)(
    ctl_display_output_handle_t,
    ctl_power_optimization_settings_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlSetPowerOptimizationSetting 
typedef ctl_result_t (CTL_APICALL *ctl_pfnSetPowerOptimizationSetting_t)(
    ctl_display_output_handle_t,
    ctl_power_optimization_settings_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlPixelTransformationGetConfig 
typedef ctl_result_t (CTL_APICALL *ctl_pfnPixelTransformationGetConfig_t)(
    ctl_display_output_handle_t,
    ctl_pixtx_pipe_get_config_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlPixelTransformationSetConfig 
typedef ctl_result_t (CTL_APICALL *ctl_pfnPixelTransformationSetConfig_t)(
    ctl_display_output_handle_t,
    ctl_pixtx_pipe_set_config_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlPanelDescriptorAccess 
typedef ctl_result_t (CTL_APICALL *ctl_pfnPanelDescriptorAccess_t)(
    ctl_display_output_handle_t,
    ctl_panel_descriptor_access_args_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetSupportedRetroScalingCapability 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetSupportedRetroScalingCapability_t)(
    ctl_device_adapter_handle_t,
    ctl_retro_scaling_caps_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetSetRetroScaling 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetSetRetroScaling_t)(
    ctl_device_adapter_handle_t,
    ctl_retro_scaling_settings_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetSupportedScalingCapability 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetSupportedScalingCapability_t)(
    ctl_display_output_handle_t,
    ctl_scaling_caps_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetCurrentScaling 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetCurrentScaling_t)(
    ctl_display_output_handle_t,
    ctl_scaling_settings_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlSetCurrentScaling 
typedef ctl_result_t (CTL_APICALL *ctl_pfnSetCurrentScaling_t)(
    ctl_display_output_handle_t,
    ctl_scaling_settings_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlEnumEngineGroups 
typedef ctl_result_t (CTL_APICALL *ctl_pfnEnumEngineGroups_t)(
    ctl_device_adapter_handle_t,
    uint32_t*,
    ctl_engine_handle_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlEngineGetProperties 
typedef ctl_result_t (CTL_APICALL *ctl_pfnEngineGetProperties_t)(
    ctl_engine_handle_t,
    ctl_engine_properties_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlEngineGetActivity 
typedef ctl_result_t (CTL_APICALL *ctl_pfnEngineGetActivity_t)(
    ctl_engine_handle_t,
    ctl_engine_stats_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlEnumFans 
typedef ctl_result_t (CTL_APICALL *ctl_pfnEnumFans_t)(
    ctl_device_adapter_handle_t,
    uint32_t*,
    ctl_fan_handle_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlFanGetProperties 
typedef ctl_result_t (CTL_APICALL *ctl_pfnFanGetProperties_t)(
    ctl_fan_handle_t,
    ctl_fan_properties_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlFanGetConfig 
typedef ctl_result_t (CTL_APICALL *ctl_pfnFanGetConfig_t)(
    ctl_fan_handle_t,
    ctl_fan_config_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlFanSetDefaultMode 
typedef ctl_result_t (CTL_APICALL *ctl_pfnFanSetDefaultMode_t)(
    ctl_fan_handle_t
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlFanSetFixedSpeedMode 
typedef ctl_result_t (CTL_APICALL *ctl_pfnFanSetFixedSpeedMode_t)(
    ctl_fan_handle_t,
    const ctl_fan_speed_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlFanSetSpeedTableMode 
typedef ctl_result_t (CTL_APICALL *ctl_pfnFanSetSpeedTableMode_t)(
    ctl_fan_handle_t,
    const ctl_fan_speed_table_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlFanGetState 
typedef ctl_result_t (CTL_APICALL *ctl_pfnFanGetState_t)(
    ctl_fan_handle_t,
    ctl_fan_speed_units_t,
    int32_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlEnumFrequencyDomains 
typedef ctl_result_t (CTL_APICALL *ctl_pfnEnumFrequencyDomains_t)(
    ctl_device_adapter_handle_t,
    uint32_t*,
    ctl_freq_handle_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlFrequencyGetProperties 
typedef ctl_result_t (CTL_APICALL *ctl_pfnFrequencyGetProperties_t)(
    ctl_freq_handle_t,
    ctl_freq_properties_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlFrequencyGetAvailableClocks 
typedef ctl_result_t (CTL_APICALL *ctl_pfnFrequencyGetAvailableClocks_t)(
    ctl_freq_handle_t,
    uint32_t*,
    double*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlFrequencyGetRange 
typedef ctl_result_t (CTL_APICALL *ctl_pfnFrequencyGetRange_t)(
    ctl_freq_handle_t,
    ctl_freq_range_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlFrequencySetRange 
typedef ctl_result_t (CTL_APICALL *ctl_pfnFrequencySetRange_t)(
    ctl_freq_handle_t,
    const ctl_freq_range_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlFrequencyGetState 
typedef ctl_result_t (CTL_APICALL *ctl_pfnFrequencyGetState_t)(
    ctl_freq_handle_t,
    ctl_freq_state_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlFrequencyGetThrottleTime 
typedef ctl_result_t (CTL_APICALL *ctl_pfnFrequencyGetThrottleTime_t)(
    ctl_freq_handle_t,
    ctl_freq_throttle_time_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetSupportedVideoProcessingCapabilities 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetSupportedVideoProcessingCapabilities_t)(
    ctl_device_adapter_handle_t,
    ctl_video_processing_feature_caps_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlGetSetVideoProcessingFeature 
typedef ctl_result_t (CTL_APICALL *ctl_pfnGetSetVideoProcessingFeature_t)(
    ctl_device_adapter_handle_t,
    ctl_video_processing_feature_getset_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlEnumMemoryModules 
typedef ctl_result_t (CTL_APICALL *ctl_pfnEnumMemoryModules_t)(
    ctl_device_adapter_handle_t,
    uint32_t*,
    ctl_mem_handle_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlMemoryGetProperties 
typedef ctl_result_t (CTL_APICALL *ctl_pfnMemoryGetProperties_t)(
    ctl_mem_handle_t,
    ctl_mem_properties_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlMemoryGetState 
typedef ctl_result_t (CTL_APICALL *ctl_pfnMemoryGetState_t)(
    ctl_mem_handle_t,
    ctl_mem_state_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlMemoryGetBandwidth 
typedef ctl_result_t (CTL_APICALL *ctl_pfnMemoryGetBandwidth_t)(
    ctl_mem_handle_t,
    ctl_mem_bandwidth_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlPciGetProperties 
typedef ctl_result_t (CTL_APICALL *ctl_pfnPciGetProperties_t)(
    ctl_device_adapter_handle_t,
    ctl_pci_properties_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlPciGetState 
typedef ctl_result_t (CTL_APICALL *ctl_pfnPciGetState_t)(
    ctl_device_adapter_handle_t,
    ctl_pci_state_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlEnumPowerDomains 
typedef ctl_result_t (CTL_APICALL *ctl_pfnEnumPowerDomains_t)(
    ctl_device_adapter_handle_t,
    uint32_t*,
    ctl_pwr_handle_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlPowerGetProperties 
typedef ctl_result_t (CTL_APICALL *ctl_pfnPowerGetProperties_t)(
    ctl_pwr_handle_t,
    ctl_power_properties_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlPowerGetEnergyCounter 
typedef ctl_result_t (CTL_APICALL *ctl_pfnPowerGetEnergyCounter_t)(
    ctl_pwr_handle_t,
    ctl_power_energy_counter_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlPowerGetLimits 
typedef ctl_result_t (CTL_APICALL *ctl_pfnPowerGetLimits_t)(
    ctl_pwr_handle_t,
    ctl_power_sustained_limit_t*,
    ctl_power_burst_limit_t*,
    ctl_power_peak_limit_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlPowerSetLimits 
typedef ctl_result_t (CTL_APICALL *ctl_pfnPowerSetLimits_t)(
    ctl_pwr_handle_t,
    const ctl_power_sustained_limit_t*,
    const ctl_power_burst_limit_t*,
    const ctl_power_peak_limit_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlEnumTemperatureSensors 
typedef ctl_result_t (CTL_APICALL *ctl_pfnEnumTemperatureSensors_t)(
    ctl_device_adapter_handle_t,
    uint32_t*,
    ctl_temp_handle_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlTemperatureGetProperties 
typedef ctl_result_t (CTL_APICALL *ctl_pfnTemperatureGetProperties_t)(
    ctl_temp_handle_t,
    ctl_temp_properties_t*
    );


///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for ctlTemperatureGetState 
typedef ctl_result_t (CTL_APICALL *ctl_pfnTemperatureGetState_t)(
    ctl_temp_handle_t,
    double*
    );


#if defined(__cplusplus)
} // extern "C"
#endif

#endif // _CTL_API_H