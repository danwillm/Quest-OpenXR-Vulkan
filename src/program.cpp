#include "program.h"

#include <string>
#include <vector>

#include "xr_linear.h"

#include "log.h"

constexpr XrPosef k_xr_pose_identity = {
        .orientation = {
                .w = 1.f,
        },
};
struct Vertex {
    XrVector3f vec3_position;
    XrVector4f vec4_color;
};

static std::vector<Vertex> gv_vertices = {
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f,}},
        {{0.5f,  -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f,}},
        {{0.5f,  0.5f,  -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f,}},
        {{0.5f,  0.5f,  -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f,}},
        {{-0.5f, 0.5f,  -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f,}},
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f,}}, //

        {{0.5f,  -0.5f, 0.5f},  {1.0f, 0.0f, 0.0f, 1.0f,}},
        {{-0.5f, -0.5f, 0.5f},  {1.0f, 0.0f, 0.0f, 1.0f,}},
        {{0.5f,  0.5f,  0.5f},  {1.0f, 0.0f, 0.0f, 1.0f,}},
        {{-0.5f, 0.5f,  0.5f},  {1.0f, 0.0f, 0.0f, 1.0f,}},
        {{0.5f,  0.5f,  0.5f},  {1.0f, 0.0f, 0.0f, 1.0f,}},
        {{-0.5f, -0.5f, 0.5f},  {1.0f, 0.0f, 0.0f, 1.0f,}},//

        {{-0.5f, 0.5f,  -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f,}},
        {{-0.5f, 0.5f,  0.5f},  {1.0f, 1.0f, 0.0f, 1.0f,}},
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f,}},
        {{-0.5f, -0.5f, 0.5f},  {1.0f, 1.0f, 0.0f, 1.0f,}},
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f,}},
        {{-0.5f, 0.5f,  0.5f},  {1.0f, 1.0f, 0.0f, 1.0f,}},//

        {{0.5f,  0.5f,  0.5f},  {0.0f, 1.0f, 0.0f, 1.0f,}},
        {{0.5f,  0.5f,  -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f,}},
        {{0.5f,  -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f,}},
        {{0.5f,  -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f,}},
        {{0.5f,  -0.5f, 0.5f},  {0.0f, 1.0f, 0.0f, 1.0f,}},
        {{0.5f,  0.5f,  0.5f},  {0.0f, 1.0f, 0.0f, 1.0f,}}, //

        {{0.5f,  -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f,}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f,}},
        {{0.5f,  -0.5f, 0.5f},  {0.0f, 0.0f, 1.0f, 1.0f,}},
        {{-0.5f, -0.5f, 0.5f},  {0.0f, 0.0f, 1.0f, 1.0f,}},
        {{0.5f,  -0.5f, 0.5f},  {0.0f, 0.0f, 1.0f, 1.0f,}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f,}}, //

        {{-0.5f, 0.5f,  -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f,}},
        {{0.5f,  0.5f,  -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f,}},
        {{0.5f,  0.5f,  0.5f},  {1.0f, 1.0f, 1.0f, 1.0f,}},
        {{0.5f,  0.5f,  0.5f},  {1.0f, 1.0f, 1.0f, 1.0f,}},
        {{-0.5f, 0.5f,  0.5f},  {1.0f, 1.0f, 1.0f, 1.0f,}},
        {{-0.5f, 0.5f,  -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f,}},
};

#define b_qualify_xr(x) do {                                            \
        XrResult ret = x;                                               \
        if(XR_FAILED(ret)) {                                            \
            Log(LogError, "[QualifyXR] %s failed with: %i", #x, ret);   \
            return false;                                               \
        }                                                               \
    } while(0)                                                          \

#define v_qualify_xr(x) do {                                            \
        XrResult ret = x;                                               \
        if(XR_FAILED(ret)) {                                            \
            Log(LogError, "[QualifyXR] %s failed with: %i", #x, ret);   \
            return;                                                     \
        }                                                               \
    } while(0)                                                          \

#define b_qualify_vk(x) do {                                            \
        VkResult ret = x;                                               \
        if(ret != VK_SUCCESS) {                                         \
            Log(LogError, "[QualifyVK] %s failed with: %i", #x, ret);   \
            return false;                                               \
        }                                                               \
    } while(0)                                                          \

#define v_qualify_vk(x) do {                                            \
        VkResult ret = x;                                               \
        if(ret != VK_SUCCESS) {                                         \
            Log(LogError, "[QualifyVK] %s failed with: %i", #x, ret);   \
            return;                                                     \
        }                                                               \
    } while(0)                                                          \

#define d_qualify_vk(x) do {                                            \
        VkResult ret = x;                                               \
        if(ret != VK_SUCCESS) {                                         \
            Log(LogError, "[DQualifyVK] %s failed with: %i", #x, ret);  \
            return {};                                                  \
        }                                                               \
    } while(0)                                                          \

#define xr_get_proc(instance, name) do {                                                    \
        b_qualify_xr(xrGetInstanceProcAddr(instance, #name, (PFN_xrVoidFunction *) &name)); \
    } while(0)                                                                              \

#define vk_get_proc(instance, name) do {                                                    \
        name = (PFN_##name) vkGetInstanceProcAddr(instance, #name);                         \
    } while(0)

static VKAPI_ATTR VkBool32 VKAPI_CALL VkDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData) {

    ELogLevel logLevel;
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            logLevel = LogError;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            logLevel = LogWarning;
            break;
        default:
            logLevel = LogInfo;
    }

    Log(logLevel, "[VkDebugCallback] %s", pCallbackData->pMessage);

    return VK_FALSE;
}

static XRAPI_ATTR XrBool32 XRAPI_CALL XrDebugCallback(
        XrDebugUtilsMessageSeverityFlagsEXT messageSeverity,
        XrDebugUtilsMessageTypeFlagsEXT messageType,
        const XrDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData) {

    ELogLevel logLevel;
    switch (messageType) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            logLevel = LogWarning;
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            logLevel = LogError;
            break;
        }
        default: {
            logLevel = LogInfo;
            break;
        }
    }

    Log(logLevel, "[VkDebugCallback] %s: %s", pCallbackData->functionName, pCallbackData->message);

    return XR_FALSE;
}

Program::Program(android_app *p_app, app_state *p_app_state) : mp_android_app(p_app),
                                                               mp_app_state(p_app_state) {}

bool Program::BInit() {
    {//Initialize loader
        xr_get_proc(XR_NULL_HANDLE, xrInitializeLoaderKHR);

        XrLoaderInitInfoAndroidKHR xr_loader_init_info = {
                .type = XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR,
                .applicationVM = mp_android_app->activity->vm,
                .applicationContext = mp_android_app->activity->clazz,
        };
        b_qualify_xr(xrInitializeLoaderKHR((XrLoaderInitInfoBaseHeaderKHR *) &xr_loader_init_info));
    }

    {//OpenXR Instance
        std::vector<const char *> v_cs_enabled_extensions = {
                XR_EXT_LOCAL_FLOOR_EXTENSION_NAME,
                XR_KHR_ANDROID_CREATE_INSTANCE_EXTENSION_NAME,
                XR_KHR_VULKAN_ENABLE2_EXTENSION_NAME,
                XR_EXT_DEBUG_UTILS_EXTENSION_NAME,
        };
        XrInstanceCreateInfoAndroidKHR xr_instance_create_info_android = {
                .type = XR_TYPE_INSTANCE_CREATE_INFO_ANDROID_KHR,
                .applicationVM = mp_android_app->activity->vm,
                .applicationActivity = mp_android_app->activity->clazz,
        };
        XrInstanceCreateInfo xr_instance_create_info = {
                .type = XR_TYPE_INSTANCE_CREATE_INFO,
                .next = &xr_instance_create_info_android,
                .applicationInfo = {
                        .applicationName = "danwillm's vulkan test",
                        .applicationVersion = 1,
                        .engineName = "danwillm",
                        .engineVersion = 1,
                        .apiVersion = XR_API_VERSION_1_0,
                },
                .enabledExtensionCount = static_cast<uint32_t>(v_cs_enabled_extensions.size()),
                .enabledExtensionNames = v_cs_enabled_extensions.data(),
        };
        b_qualify_xr(xrCreateInstance(&xr_instance_create_info, &mh_xrinstance));

        //OpenXR Function bindings
        xr_get_proc(mh_xrinstance, xrCreateDebugUtilsMessengerEXT);
        xr_get_proc(mh_xrinstance, xrGetVulkanGraphicsRequirements2KHR);
        xr_get_proc(mh_xrinstance, xrCreateVulkanInstanceKHR);
        xr_get_proc(mh_xrinstance, xrGetVulkanGraphicsDevice2KHR);
        xr_get_proc(mh_xrinstance, xrCreateVulkanDeviceKHR);

        XrDebugUtilsMessengerCreateInfoEXT xr_debug_info{
                XR_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
        xr_debug_info.messageSeverities = XR_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                                          XR_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
#if !defined(NDEBUG)
        xr_debug_info.messageSeverities |=
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
#endif
        xr_debug_info.messageTypes = XR_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                     XR_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                     XR_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        xr_debug_info.userCallback = XrDebugCallback;
        b_qualify_xr(xrCreateDebugUtilsMessengerEXT(mh_xrinstance, &xr_debug_info,
                                                    &mh_xrdebug_utils_messenger));
    }

    {//OpenXR System Properties
        XrSystemGetInfo xr_system_get_info = {
                .type = XR_TYPE_SYSTEM_GET_INFO,
                .formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY,
        };
        b_qualify_xr(xrGetSystem(mh_xrinstance, &xr_system_get_info, &mh_xrsystem_id));

        XrSystemProperties xr_system_properties = {XR_TYPE_SYSTEM_PROPERTIES};
        b_qualify_xr(xrGetSystemProperties(mh_xrinstance, mh_xrsystem_id, &xr_system_properties));
    }

    {//Vulkan Initialization
        XrGraphicsRequirementsVulkan2KHR xr_graphics_requirements_vulkan{
                XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN2_KHR};
        b_qualify_xr(xrGetVulkanGraphicsRequirements2KHR(mh_xrinstance, mh_xrsystem_id,
                                                         &xr_graphics_requirements_vulkan));

        std::vector<const char *> v_enabled_layers{};

        {//Vulkan Validation Layers
#if !defined(NDEBUG)
            const char *s_validation_layer_name = []() -> const char * { //Get Vulkan Validation Layer
                uint32_t un_layer_count;
                d_qualify_vk(vkEnumerateInstanceLayerProperties(&un_layer_count, nullptr));

                std::vector<VkLayerProperties> v_available_layers(un_layer_count);
                d_qualify_vk(vkEnumerateInstanceLayerProperties(&un_layer_count,
                                                                v_available_layers.data()));

                std::vector<const char *> v_validation_layer_names = {
                        "VK_LAYER_KHRONOS_validation",
                        "VK_LAYER_LUNARG_standard_validation"
                };

                for (const auto &s_validation_layer_name: v_validation_layer_names) {
                    for (const auto &layer_properties: v_available_layers) {
                        if (strcmp(s_validation_layer_name, layer_properties.layerName) == 0) {
                            return s_validation_layer_name;
                        }
                    }
                }

                Log(LogWarning, "[XrProgram] Could not find a validation layer!");
                return nullptr;
            }();

            if (s_validation_layer_name) {
                v_enabled_layers.push_back(s_validation_layer_name);
            }
#endif
        }

        std::vector<const char *> v_requested_extensions = {};

        {//Vulkan Extensions
            uint32_t un_extension_count = 0;
            b_qualify_vk(
                    vkEnumerateInstanceExtensionProperties(nullptr, &un_extension_count, nullptr));

            std::vector<VkExtensionProperties> v_available_extensions(un_extension_count);
            b_qualify_vk(vkEnumerateInstanceExtensionProperties(nullptr, &un_extension_count,
                                                                v_available_extensions.data()));

            auto BIsExtensionSupported = [&](const char *pc_extension_name) -> bool {
                auto it = std::find_if(v_available_extensions.begin(), v_available_extensions.end(),
                                       [&](const VkExtensionProperties &vk_extension_properties) {
                                           return strcmp(pc_extension_name,
                                                         vk_extension_properties.extensionName) ==
                                                  0;
                                       });

                return it != v_available_extensions.end();
            };

            if (BIsExtensionSupported(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) {
                v_requested_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }
        }

        VkApplicationInfo vk_application_info = {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = "Demo Vulkan",
                .applicationVersion = 1,
                .pEngineName = "danwillm",
                .engineVersion = 1,
                .apiVersion = VK_API_VERSION_1_1,
        };

        VkInstanceCreateInfo vk_instance_info = {
                .pApplicationInfo = &vk_application_info,
                .enabledLayerCount = static_cast<uint32_t>(v_enabled_layers.size()),
                .ppEnabledLayerNames = v_enabled_layers.data(),
                .enabledExtensionCount = static_cast<uint32_t>(v_requested_extensions.size()),
                .ppEnabledExtensionNames = v_requested_extensions.data()
        };

        XrVulkanInstanceCreateInfoKHR xr_vulkan_instance_create_info = {
                .systemId = mh_xrsystem_id,
                .pfnGetInstanceProcAddr = &vkGetInstanceProcAddr,
                .vulkanCreateInfo = &vk_instance_info,
                .vulkanAllocator = nullptr,
        };

        {//Create Vulkan Instance
            VkResult vk_error;
            b_qualify_xr(xrCreateVulkanInstanceKHR(mh_xrinstance, &xr_vulkan_instance_create_info,
                                                   &mh_vkinstance, &vk_error));
            b_qualify_vk(vk_error);

            vk_get_proc(mh_vkinstance, vkCreateDebugUtilsMessengerEXT);
            vk_get_proc(mh_vkinstance, vkDestroyDebugUtilsMessengerEXT);

            VkDebugUtilsMessengerCreateInfoEXT vk_debug_info{
                    VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
            vk_debug_info.messageSeverity =
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
#if !defined(NDEBUG)
            vk_debug_info.messageSeverity |=
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
#endif
            vk_debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            vk_debug_info.pfnUserCallback = VkDebugCallback;
            vk_debug_info.pUserData = this;
            b_qualify_vk(vkCreateDebugUtilsMessengerEXT(mh_vkinstance, &vk_debug_info, nullptr,
                                                        &mh_vkdebug_utils_messenger));
        }
    }

    {//Vulkan device creation
        XrVulkanGraphicsDeviceGetInfoKHR xr_vk_device_info = {
                .type = XR_TYPE_VULKAN_GRAPHICS_DEVICE_GET_INFO_KHR,
                .systemId = mh_xrsystem_id,
                .vulkanInstance = mh_vkinstance,
        };
        b_qualify_xr(xrGetVulkanGraphicsDevice2KHR(mh_xrinstance, &xr_vk_device_info,
                                                   &mh_vkphysical_device));

        uint32_t un_queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(mh_vkphysical_device, &un_queue_family_count,
                                                 nullptr);

        std::vector<VkQueueFamilyProperties> v_queue_family_properties(un_queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(mh_vkphysical_device, &un_queue_family_count,
                                                 v_queue_family_properties.data());

        for (uint32_t i = 0; i < v_queue_family_properties.size(); i++) {
            if (v_queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                mun_queue_family = i;
                break;
            }
        }

        float f_queue_priorities = 1.f;
        VkDeviceQueueCreateInfo vk_queue_info = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = mun_queue_family,
                .queueCount = 1,
                .pQueuePriorities = &f_queue_priorities,
        };

        std::vector<const char *> v_device_extensions;

        VkPhysicalDeviceFeatures vk_physical_device_features{};

        VkDeviceCreateInfo vk_device_create_info = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .queueCreateInfoCount = 1,
                .pQueueCreateInfos = &vk_queue_info,
                .enabledLayerCount = 0,
                .ppEnabledLayerNames = nullptr,
                .enabledExtensionCount = static_cast<uint32_t>(v_device_extensions.size()),
                .ppEnabledExtensionNames = v_device_extensions.data(),
                .pEnabledFeatures = &vk_physical_device_features
        };

        XrVulkanDeviceCreateInfoKHR xr_vulkan_device_create_info = {
                .type = XR_TYPE_VULKAN_DEVICE_CREATE_INFO_KHR,
                .systemId = mh_xrsystem_id,
                .pfnGetInstanceProcAddr = &vkGetInstanceProcAddr,
                .vulkanPhysicalDevice = mh_vkphysical_device,
                .vulkanCreateInfo = &vk_device_create_info,
                .vulkanAllocator = nullptr
        };

        VkResult vk_err;
        b_qualify_xr(
                xrCreateVulkanDeviceKHR(mh_xrinstance, &xr_vulkan_device_create_info, &mh_vkdevice,
                                        &vk_err));
        b_qualify_vk(vk_err);

        vkGetDeviceQueue(mh_vkdevice, mun_queue_family, 0, &mh_vkqueue);
    }

    {//Create OpenXR Session
        XrGraphicsBindingVulkanKHR xr_graphics_binding_vulkan = {
                .type = XR_TYPE_GRAPHICS_BINDING_VULKAN2_KHR,
                .instance = mh_vkinstance,
                .physicalDevice = mh_vkphysical_device,
                .device = mh_vkdevice,
                .queueFamilyIndex = mun_queue_family,
                .queueIndex = 0
        };
        XrSessionCreateInfo xr_session_create_info = {
                .type = XR_TYPE_SESSION_CREATE_INFO,
                .next = &xr_graphics_binding_vulkan,
                .createFlags = 0,
                .systemId = mh_xrsystem_id
        };
        b_qualify_xr(xrCreateSession(mh_xrinstance, &xr_session_create_info, &mh_xrsession));

        for (XrReferenceSpaceType xr_reference_space_type: {XR_REFERENCE_SPACE_TYPE_VIEW,
                                                            XR_REFERENCE_SPACE_TYPE_STAGE,
                                                            XR_REFERENCE_SPACE_TYPE_LOCAL_FLOOR_EXT}) {//Create OpenXR Reference spaces
            XrReferenceSpaceCreateInfo xr_reference_space_create_info = {
                    .type = XR_TYPE_REFERENCE_SPACE_CREATE_INFO,
                    .referenceSpaceType = xr_reference_space_type,
                    .poseInReferenceSpace = k_xr_pose_identity,
            };

            b_qualify_xr(xrCreateReferenceSpace(mh_xrsession, &xr_reference_space_create_info,
                                                &mmap_reference_spaces[xr_reference_space_type]));
        }
    }

    {//OpenXR View configuration
        uint32_t un_view_config_count;
        b_qualify_xr(xrEnumerateViewConfigurations(mh_xrinstance, mh_xrsystem_id, 0,
                                                   &un_view_config_count, nullptr));

        std::vector<XrViewConfigurationType> v_view_config_types(un_view_config_count);
        b_qualify_xr(xrEnumerateViewConfigurations(mh_xrinstance, mh_xrsystem_id,
                                                   v_view_config_types.size(),
                                                   &un_view_config_count,
                                                   v_view_config_types.data()));

        me_app_view_type = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

        if (std::find(v_view_config_types.begin(), v_view_config_types.end(), XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO) ==
            v_view_config_types.end()) {
            throw std::runtime_error("[XrProgram] View configuration STEREO was not supported on runtime");
        }

        uint32_t un_view_config_views_count;
        b_qualify_xr(
                xrEnumerateViewConfigurationViews(mh_xrinstance, mh_xrsystem_id, me_app_view_type,
                                                  0, &un_view_config_views_count, nullptr));

        mv_view_config_views.resize(un_view_config_views_count, {XR_TYPE_VIEW_CONFIGURATION_VIEW});
        b_qualify_xr(xrEnumerateViewConfigurationViews(mh_xrinstance, mh_xrsystem_id, me_app_view_type,
                                                       mv_view_config_views.size(),
                                                       &un_view_config_views_count,
                                                       mv_view_config_views.data()));

        mv_views.resize(un_view_config_views_count, {XR_TYPE_VIEW});
    }

    {//Create swapchains
        uint32_t un_swapchain_formats_count;
        b_qualify_xr(
                xrEnumerateSwapchainFormats(mh_xrsession, 0, &un_swapchain_formats_count, nullptr));

        std::vector<int64_t> v_swapchain_formats(un_swapchain_formats_count);
        b_qualify_xr(xrEnumerateSwapchainFormats(mh_xrsession, v_swapchain_formats.size(),
                                                 &un_swapchain_formats_count,
                                                 v_swapchain_formats.data()));

        auto GetSupportedSwapchainFormat = [](const std::vector<VkFormat> &vVkSupportedFormats,
                                              std::vector<int64_t> &vLAvailableFormats) -> int64_t {
            auto it = std::find_first_of(vLAvailableFormats.begin(), vLAvailableFormats.end(),
                                         std::begin(vVkSupportedFormats),
                                         std::end(vVkSupportedFormats));

            if (it == vLAvailableFormats.end()) {
                return 0;
            }

            return *it;
        };

        const std::vector<VkFormat> v_vkformat_color = {
                VK_FORMAT_B8G8R8A8_SRGB,
                VK_FORMAT_R8G8B8A8_SRGB,
                VK_FORMAT_B8G8R8A8_UNORM,
                VK_FORMAT_R8G8B8A8_UNORM
        };

        int64_t l_supported_color_format = GetSupportedSwapchainFormat(v_vkformat_color, v_swapchain_formats);

        if (l_supported_color_format == 0) {
            throw std::runtime_error("[XrProgram] No supported swapchain format for depth or color was supported!");
        }

        for (uint32_t i = 0; i < mv_views.size(); i++) {//Color swapchain
            XrSwapchainCreateInfo swapchain_color_create_info = {
                    .type = XR_TYPE_SWAPCHAIN_CREATE_INFO,
                    .createFlags = 0,
                    .usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT,
                    .format = l_supported_color_format,
                    .sampleCount = mv_view_config_views[i].recommendedSwapchainSampleCount,
                    .width = mv_view_config_views[i].recommendedImageRectWidth, //assume same
                    .height = mv_view_config_views[i].recommendedImageRectHeight,
                    .faceCount = 1,
                    .arraySize = 1,
                    .mipCount = 1,
            };
            b_qualify_xr(xrCreateSwapchain(mh_xrsession, &swapchain_color_create_info, &mv_sccolor[i].swapchain));

            uint32_t un_swapchain_image_count;
            b_qualify_xr(xrEnumerateSwapchainImages(mv_sccolor[i].swapchain, 0, &un_swapchain_image_count, nullptr));

            auto &swapchain_images = mv_sccolor[i].v_images;
            swapchain_images.resize(un_swapchain_image_count, {XR_TYPE_SWAPCHAIN_IMAGE_VULKAN2_KHR});
            b_qualify_xr(
                    xrEnumerateSwapchainImages(mv_sccolor[i].swapchain, swapchain_images.size(), &un_swapchain_image_count,
                                               reinterpret_cast<XrSwapchainImageBaseHeader *>(swapchain_images.data())));

            mv_sccolor[i].vk_format = static_cast<VkFormat>(l_supported_color_format);

            mv_sccolor[i].extent = {mv_view_config_views[i].recommendedImageRectWidth, mv_view_config_views[i].recommendedImageRectHeight};

            mv_sccolor[i].v_image_views.resize(mv_sccolor[i].v_images.size());
            for (uint32_t k = 0; k < mv_sccolor[i].v_images.size(); k++) {
                VkImageViewCreateInfo vk_image_view_create_info = {
                        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                        .image = mv_sccolor[i].v_images[k].image,
                        .viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
                        .format = mv_sccolor[i].vk_format,
                        .components = {
                                .r = VK_COMPONENT_SWIZZLE_R,
                                .g = VK_COMPONENT_SWIZZLE_G,
                                .b = VK_COMPONENT_SWIZZLE_B,
                                .a = VK_COMPONENT_SWIZZLE_A
                        },
                        .subresourceRange = {
                                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                .baseMipLevel = 0,
                                .levelCount = 1,
                                .baseArrayLayer = 0,
                                .layerCount = static_cast<uint32_t>(mv_view_config_views.size()),
                        }
                };

                b_qualify_vk(vkCreateImageView(mh_vkdevice, &vk_image_view_create_info, nullptr, &mv_sccolor[i].v_image_views[k]));
            }
        }

        const std::vector<VkFormat> v_vkformat_depth = {
                VK_FORMAT_D32_SFLOAT,
                VK_FORMAT_D16_UNORM
        };
        int64_t l_supported_depth_format = GetSupportedSwapchainFormat(v_vkformat_color, v_swapchain_formats);

        if (l_supported_depth_format == 0) {
            throw std::runtime_error("[XrProgram] No supported swapchain format for depth was supported!");
        }

        for (uint32_t i = 0; i < mv_views.size(); i++) {//Depth swapchain
            auto& sc_depth = mv_scdepth[i];

            XrSwapchainCreateInfo swapchain_create_info = {
                    .type = XR_TYPE_SWAPCHAIN_CREATE_INFO,
                    .usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                    .format = l_supported_depth_format,
                    .sampleCount = mv_view_config_views[i].recommendedSwapchainSampleCount,
                    .width = mv_view_config_views[i].recommendedImageRectWidth,
                    .height = mv_view_config_views[i].recommendedImageRectHeight,
                    .faceCount = 1,
                    .arraySize = 1,
                    .mipCount = 1,
            };
            b_qualify_xr(xrCreateSwapchain(mh_xrsession, &swapchain_create_info, &sc_depth.swapchain));

            uint32_t un_swapchain_image_count;
            b_qualify_xr(xrEnumerateSwapchainImages(sc_depth.swapchain, 0, &un_swapchain_image_count, nullptr));

            auto &swapchain_images = sc_depth.v_images;
            swapchain_images.resize(un_swapchain_image_count, {XR_TYPE_SWAPCHAIN_IMAGE_VULKAN2_KHR});
            b_qualify_xr(xrEnumerateSwapchainImages(sc_depth.swapchain, swapchain_images.size(), &un_swapchain_image_count,
                                                    reinterpret_cast<XrSwapchainImageBaseHeader *>(swapchain_images.data())));

            sc_depth.vk_format = static_cast<VkFormat>(l_supported_depth_format);

            sc_depth.v_image_views.resize(sc_depth.v_images.size());
            for (uint32_t k = 0; k < sc_depth.v_images.size(); k++) {
                VkImageViewCreateInfo image_view_create_info = {
                        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                        .image = sc_depth.v_images[k].image,
                        .viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
                        .format = sc_depth.vk_format,
                        .components = {
                                .r = VK_COMPONENT_SWIZZLE_R,
                                .g = VK_COMPONENT_SWIZZLE_G,
                                .b = VK_COMPONENT_SWIZZLE_B,
                                .a = VK_COMPONENT_SWIZZLE_A
                        },
                        .subresourceRange = {
                                .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
                                .baseMipLevel = 0,
                                .levelCount = 1,
                                .baseArrayLayer = 0,
                                .layerCount = static_cast<uint32_t>(mv_view_config_views.size()),
                        }
                };

                b_qualify_vk(vkCreateImageView(mh_vkdevice, &image_view_create_info, nullptr, &sc_depth.v_image_views[k]));
            }
        }
    }

    {//Vulkan pipeline setup
        auto CreateShaderModule = [](VkDevice device, size_t size_buffer,
                                     const uint32_t *pun_buffer,
                                     VkShaderModule &out_vk_shader_module) {
            VkShaderModuleCreateInfo vk_shader_module_create_info = {
                    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                    .codeSize = size_buffer,
                    .pCode = pun_buffer,
            };

            b_qualify_vk(vkCreateShaderModule(device, &vk_shader_module_create_info, nullptr,
                                              &out_vk_shader_module));

            return true;
        };

        VkShaderModule vksm_vertex;
        VkShaderModule vksm_fragment;

        {//Vertex shader
            AAsset *passet_vertex = AAssetManager_open(mp_android_app->activity->assetManager,
                                                       "shaders/shader.vert.spv",
                                                       AASSET_MODE_BUFFER);
            if (!CreateShaderModule(mh_vkdevice, AAsset_getLength(passet_vertex),
                                    static_cast<const uint32_t *>(AAsset_getBuffer(passet_vertex)),
                                    vksm_vertex)) {
                Log(LogError, "[XrProgram] Failed to create vertex shader!");

                AAsset_close(passet_vertex);
                return false;
            }

            AAsset_close(passet_vertex);
        }

        {//Fragment shader
            AAsset *passet_fragment = AAssetManager_open(mp_android_app->activity->assetManager,
                                                         "shaders/shader.frag.spv",
                                                         AASSET_MODE_BUFFER);
            if (!CreateShaderModule(mh_vkdevice, AAsset_getLength(passet_fragment),
                                    static_cast<const uint32_t *>(AAsset_getBuffer(
                                            passet_fragment)), vksm_fragment)) {
                Log(LogError, "[XrProgram] Failed to create fragment shader!");

                AAsset_close(passet_fragment);
                return false;
            }

            AAsset_close(passet_fragment);
        }

        VkCommandPoolCreateInfo command_pool_create_info = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                .queueFamilyIndex = mun_queue_family
        };
        b_qualify_vk(vkCreateCommandPool(mh_vkdevice, &command_pool_create_info, nullptr,
                                         &mh_command_pool));

        mv_command_buffers.resize(mv_views.size());
        VkCommandBufferAllocateInfo command_buffer_allocate_info = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .commandPool = mh_command_pool,
                .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount = (uint32_t) mv_command_buffers.size(),
        };
        b_qualify_vk(vkAllocateCommandBuffers(mh_vkdevice, &command_buffer_allocate_info,
                                              mv_command_buffers.data()));

        VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info[] = {
                {
                        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        .stage = VK_SHADER_STAGE_VERTEX_BIT,
                        .module = vksm_vertex,
                        .pName = "main"
                },
                {
                        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                        .module = vksm_fragment,
                        .pName = "main"
                }
        };

        std::vector<VkDynamicState> vvk_dynamic_states = {
        };

        VkPipelineDynamicStateCreateInfo dynamic_state_create_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .dynamicStateCount = static_cast<uint32_t>(vvk_dynamic_states.size()),
                .pDynamicStates = vvk_dynamic_states.data()
        };

        VkVertexInputBindingDescription vertex_binding_description = {
                .binding = 0,
                .stride = sizeof(Vertex),
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        };

        std::array<VkVertexInputAttributeDescription, 2> v_vertex_attribute_descriptions = {
                VkVertexInputAttributeDescription{
                        .location = 0,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32B32_SFLOAT,
                        .offset = offsetof(Vertex, vec3_position)
                },
                VkVertexInputAttributeDescription{
                        .location = 1,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32B32A32_SFLOAT,
                        .offset = offsetof(Vertex, vec4_color),
                }
        };

        VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                .vertexBindingDescriptionCount = 1,
                .pVertexBindingDescriptions = &vertex_binding_description,
                .vertexAttributeDescriptionCount = (uint32_t) v_vertex_attribute_descriptions.size(),
                .pVertexAttributeDescriptions = v_vertex_attribute_descriptions.data(),
        };

        VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                .primitiveRestartEnable = VK_FALSE,
        };

        VkViewport viewport = {
                .width = (float) mv_sccolor[0].extent.width,
                .height = (float) mv_sccolor[0].extent.height,
                .minDepth = 0.f,
                .maxDepth = 1.f
        };
        VkRect2D scissor = {
                .offset = {0, 0},
                .extent = mv_sccolor[0].extent,
        };
        VkPipelineViewportStateCreateInfo viewport_state_create_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .viewportCount = 1,
                .pViewports = &viewport,
                .scissorCount = 1,
                .pScissors = &scissor
        };

        VkPipelineRasterizationStateCreateInfo rasterization_state_create_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                .depthClampEnable = VK_FALSE,
                .polygonMode = VK_POLYGON_MODE_FILL,
                .cullMode = VK_CULL_MODE_BACK_BIT,
                .frontFace = VK_FRONT_FACE_CLOCKWISE,
                .depthBiasEnable = VK_FALSE,
                .lineWidth = 1.f,
        };

        VkPipelineMultisampleStateCreateInfo multisample_state_create_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        };

        VkPipelineColorBlendAttachmentState color_blend_attachment_state = {
                .blendEnable = VK_FALSE,
                .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                  VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        };
        VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .logicOpEnable = VK_FALSE,
                .logicOp = VK_LOGIC_OP_COPY,
                .attachmentCount = 1,
                .pAttachments = &color_blend_attachment_state,
        };
        VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                .depthTestEnable = VK_TRUE,
                .depthWriteEnable = VK_TRUE,
                .depthCompareOp = VK_COMPARE_OP_LESS,
                .depthBoundsTestEnable = VK_FALSE,
                .stencilTestEnable = VK_FALSE,
                .minDepthBounds = 0.f,
                .maxDepthBounds = 1.f,
        };

        VkDescriptorSetLayoutBinding descriptor_set_layout_binding = {
                .binding = 0,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = 1,
                .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr,
        };

        VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                .bindingCount = 1,
                .pBindings = &descriptor_set_layout_binding,
        };
        b_qualify_vk(vkCreateDescriptorSetLayout(mh_vkdevice, &descriptor_set_layout_create_info, nullptr, &mh_vkdescriptor_set_layout));

        VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                .setLayoutCount = 1,
                .pSetLayouts = &mh_vkdescriptor_set_layout,
                .pushConstantRangeCount = 0,
                .pPushConstantRanges = nullptr,
        };
        b_qualify_vk(vkCreatePipelineLayout(mh_vkdevice, &pipeline_layout_create_info, nullptr, &mh_vkpipeline_layout));

        VkAttachmentDescription color_attachment = {
                .format = mv_sccolor[0].vk_format,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };
        VkAttachmentReference color_attachment_reference = {
                .attachment = 0,
                .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        };

        VkAttachmentDescription depth_attachment = {
                .format = mv_scdepth[0].vk_format,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        };
        VkAttachmentReference depth_attachment_reference = {
                .attachment = 1,
                .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        };

        VkSubpassDescription subpass_description = {
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                .colorAttachmentCount = 1,
                .pColorAttachments = &color_attachment_reference,
                .pDepthStencilAttachment = &depth_attachment_reference,
        };

        std::vector<VkAttachmentDescription> v_vkattachment_descriptions = {color_attachment, depth_attachment};
        VkRenderPassCreateInfo render_pass_create_info = {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                .attachmentCount = (uint32_t) v_vkattachment_descriptions.size(),
                .pAttachments = v_vkattachment_descriptions.data(),
                .subpassCount = 1,
                .pSubpasses = &subpass_description,
        };
        b_qualify_vk(vkCreateRenderPass(mh_vkdevice, &render_pass_create_info, nullptr, &mh_vkrender_pass));

        VkGraphicsPipelineCreateInfo pipeline_create_info = {
                .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                .stageCount = 2,
                .pStages = pipeline_shader_stage_create_info,
                .pVertexInputState = &vertex_input_state_create_info,
                .pInputAssemblyState = &input_assembly_create_info,
                .pViewportState = &viewport_state_create_info,
                .pRasterizationState = &rasterization_state_create_info,
                .pMultisampleState = &multisample_state_create_info,
                .pDepthStencilState = &depth_stencil_state_create_info,
                .pColorBlendState = &color_blend_state_create_info,
                .pDynamicState = &dynamic_state_create_info,
                .layout = mh_vkpipeline_layout,
                .renderPass = mh_vkrender_pass,
                .subpass = 0,
                .basePipelineHandle = VK_NULL_HANDLE,
                .basePipelineIndex = -1,
        };
        b_qualify_vk(vkCreateGraphicsPipelines(mh_vkdevice, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &mh_vkgraphics_pipeline));

        {//Vertex Buffer creation
            VkBufferCreateInfo buffer_create_info = {
                    .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                    .size = sizeof(gv_vertices[0]) * gv_vertices.size(),
                    .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,

            };
            b_qualify_vk(vkCreateBuffer(mh_vkdevice, &buffer_create_info, nullptr, &mh_vkbuffer_vertex));

            VkMemoryRequirements memory_requirements;
            vkGetBufferMemoryRequirements(mh_vkdevice, mh_vkbuffer_vertex, &memory_requirements);

            VkPhysicalDeviceMemoryProperties memory_properties;
            vkGetPhysicalDeviceMemoryProperties(mh_vkphysical_device, &memory_properties);

            VkMemoryPropertyFlags property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            uint32_t un_memory_index = 0;
            for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
                if ((memory_requirements.memoryTypeBits & (1 << i)) &&
                    (memory_properties.memoryTypes[i].propertyFlags & property_flags) == property_flags) {
                    un_memory_index = i;
                    break;
                }
            }

            VkMemoryAllocateInfo memory_allocate_info = {
                    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                    .allocationSize = memory_requirements.size,
                    .memoryTypeIndex = un_memory_index,
            };

            b_qualify_vk(vkAllocateMemory(mh_vkdevice, &memory_allocate_info, nullptr,
                                          &mh_vkmemory_vertex));
            b_qualify_vk(vkBindBufferMemory(mh_vkdevice, mh_vkbuffer_vertex, mh_vkmemory_vertex, 0));

            void *vp_data;
            b_qualify_vk(vkMapMemory(mh_vkdevice, mh_vkmemory_vertex, 0, buffer_create_info.size, 0, &vp_data));
            memcpy(vp_data, gv_vertices.data(), (size_t) buffer_create_info.size);
            vkUnmapMemory(mh_vkdevice, mh_vkmemory_vertex);
        }

        {//Uniform buffer creation
            VkDeviceSize device_buffer_size = sizeof(XrMatrix4x4f);

            mv_vkbuffer_uniforms.resize(mv_views.size());
            mv_vkmemory_uniforms.resize(mv_views.size());
            mv_vpbuffer_mapped_uniforms.resize(mv_views.size());

            for (uint32_t i = 0; i < mv_views.size(); i++) {

                VkBufferCreateInfo buffer_create_info = {
                        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                        .size = sizeof(XrMatrix4x4f),
                        .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,

                };
                b_qualify_vk(vkCreateBuffer(mh_vkdevice, &buffer_create_info, nullptr, &mv_vkbuffer_uniforms[i]));

                VkMemoryRequirements memory_requirements;
                vkGetBufferMemoryRequirements(mh_vkdevice, mv_vkbuffer_uniforms[i], &memory_requirements);

                VkPhysicalDeviceMemoryProperties memory_properties;
                vkGetPhysicalDeviceMemoryProperties(mh_vkphysical_device, &memory_properties);

                VkMemoryPropertyFlags property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
                uint32_t un_memory_index = 0;
                for (uint32_t k = 0; k < memory_properties.memoryTypeCount; k++) {
                    if ((memory_requirements.memoryTypeBits & (1 << k)) &&
                        (memory_properties.memoryTypes[k].propertyFlags & property_flags) == property_flags) {
                        un_memory_index = k;
                        break;
                    }
                }

                VkMemoryAllocateInfo memory_allocate_info = {
                        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                        .allocationSize = memory_requirements.size,
                        .memoryTypeIndex = un_memory_index,
                };

                b_qualify_vk(vkAllocateMemory(mh_vkdevice, &memory_allocate_info, nullptr,
                                              &mv_vkmemory_uniforms[i]));
                b_qualify_vk(vkBindBufferMemory(mh_vkdevice, mv_vkbuffer_uniforms[i], mv_vkmemory_uniforms[i], 0));

                b_qualify_vk(vkMapMemory(mh_vkdevice, mv_vkmemory_uniforms[i], 0, buffer_create_info.size, 0, &mv_vpbuffer_mapped_uniforms[i]));
            }
        }

        {//Descriptor pools
            VkDescriptorPoolSize descriptor_pool_size = {
                    .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                    .descriptorCount = (uint32_t) mv_views.size(),
            };

            VkDescriptorPoolCreateInfo descriptor_pool_create_info = {
                    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                    .maxSets = (uint32_t) mv_views.size(),
                    .poolSizeCount = 1,
                    .pPoolSizes = &descriptor_pool_size,
            };

            b_qualify_vk(vkCreateDescriptorPool(mh_vkdevice, &descriptor_pool_create_info, nullptr, &mh_vkdescriptor_pool));
        }

        {//Descriptor sets
            std::vector<VkDescriptorSetLayout> v_vkdescriptor_set_layouts(mv_views.size(), mh_vkdescriptor_set_layout);
            mv_vkdescriptor_sets.resize(mv_views.size());

            VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {
                    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                    .descriptorPool = mh_vkdescriptor_pool,
                    .descriptorSetCount = (uint32_t) mv_views.size(),
                    .pSetLayouts = v_vkdescriptor_set_layouts.data(),
            };
            b_qualify_vk(vkAllocateDescriptorSets(mh_vkdevice, &descriptor_set_allocate_info, mv_vkdescriptor_sets.data()));

            for (uint32_t i = 0; i < mv_views.size(); i++) {
                VkDescriptorBufferInfo descriptor_buffer_info = {
                        .buffer = mv_vkbuffer_uniforms[i],
                        .offset = 0,
                        .range = sizeof(XrMatrix4x4f), //or VK_WHOLE_SIZE
                };

                VkWriteDescriptorSet write_descriptor_set = {
                        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                        .dstSet = mv_vkdescriptor_sets[i],
                        .dstBinding = 0,
                        .dstArrayElement = 0,
                        .descriptorCount = 1,
                        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                        .pBufferInfo = &descriptor_buffer_info,
                };

                vkUpdateDescriptorSets(mh_vkdevice, 1, &write_descriptor_set, 0, nullptr);
            }
        }

        for (int i = 0; i < mv_views.size(); i++) {
            mv_sccolor[i].v_framebuffers.resize(mv_sccolor[i].v_images.size());
            for (size_t k = 0; k < mv_sccolor[i].v_images.size(); k++) {
                std::vector<VkImageView> v_vkattachments = {mv_sccolor[i].v_image_views[k], mv_scdepth[i].v_image_views[k]};

                VkFramebufferCreateInfo frame_buffer_create_info = {
                        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                        .renderPass = mh_vkrender_pass,
                        .attachmentCount = (uint32_t) v_vkattachments.size(),
                        .pAttachments = v_vkattachments.data(),
                        .width = mv_sccolor[i].extent.width,
                        .height = mv_sccolor[i].extent.height,
                        .layers = 1,
                };
                b_qualify_vk(vkCreateFramebuffer(mh_vkdevice, &frame_buffer_create_info, nullptr, &mv_sccolor[i].v_framebuffers[k]));
            }
        }

        {//Synchronisation
            VkFenceCreateInfo fence_create_info = {
                    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                    .flags = VK_FENCE_CREATE_SIGNALED_BIT,
            };
            b_qualify_vk(vkCreateFence(mh_vkdevice, &fence_create_info, nullptr, &mh_fence_exec));
        }
    }

    return true;
}

void Program::Tick() {
    XrEventDataBuffer xr_event_buffer{XR_TYPE_EVENT_DATA_BUFFER};
    XrResult result = xrPollEvent(mh_xrinstance, &xr_event_buffer);
    while (result == XR_SUCCESS) {
        Log("[XrProgram] Event!");
        switch (xr_event_buffer.type) {
            case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
                XrEventDataSessionStateChanged *pxr_session_state_changed = reinterpret_cast<XrEventDataSessionStateChanged *>(&xr_event_buffer);
                if (pxr_session_state_changed->session != mh_xrsession) {
                    Log("[XrProgram] Received session state changed for unknown session?!");
                    break;
                }

                switch (pxr_session_state_changed->state) {
                    case XR_SESSION_STATE_IDLE:
                    case XR_SESSION_STATE_UNKNOWN: {
                        mb_should_run_framecycle = false;

                        break;
                    }

                    case XR_SESSION_STATE_FOCUSED:
                    case XR_SESSION_STATE_SYNCHRONIZED:
                    case XR_SESSION_STATE_VISIBLE: {
                        mb_should_run_framecycle = true;

                        break;
                    }

                    case XR_SESSION_STATE_READY: {
                        if (!mb_is_session_running) {
                            Log("[XrProgram] Session began");
                            XrSessionBeginInfo xr_session_begin_info = {
                                    .type = XR_TYPE_SESSION_BEGIN_INFO,
                                    .primaryViewConfigurationType = me_app_view_type
                            };
                            v_qualify_xr(xrBeginSession(mh_xrsession, &xr_session_begin_info));

                            mb_is_session_running = true;
                        }

                        mb_should_run_framecycle = true;
                        break;
                    }

                    case XR_SESSION_STATE_STOPPING: {
                        if (mb_is_session_running) {
                            v_qualify_xr(xrEndSession(mh_xrsession));
                            mb_is_session_running = false;
                        }

                        mb_should_run_framecycle = true;
                        break;
                    }

                    case XR_SESSION_STATE_LOSS_PENDING:
                    case XR_SESSION_STATE_EXITING: {
                        Log("[XrProgram] Destroying session");

                        v_qualify_xr(xrDestroySession(mh_xrsession));

                        mb_is_session_running = false;
                        mb_should_run_framecycle = false;
                        mp_app_state->b_app_running = false;

                        break;
                    }

                    default: {
                        Log(LogWarning, "[XrProgram] SESSION_STATE_CHANGED: Unhandled event: %i",
                            pxr_session_state_changed->state);
                        break;
                    }
                }

                break;
            }

            case XR_TYPE_EVENT_DATA_EVENTS_LOST: {
                auto *p_events_lost = reinterpret_cast<XrEventDataEventsLost *>(&xr_event_buffer);
                Log(LogWarning, "[XrProgram] EVENTS_LOST: Lost events: %i",
                    p_events_lost->lostEventCount);
                break;
            }

            case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
                auto *pxr_instance_loss_pending = reinterpret_cast<XrEventDataInstanceLossPending *>(&xr_event_buffer);

                mp_app_state->b_app_running = false;
                mb_is_session_running = false;
                mb_should_run_framecycle = false;
                break;
            }

            default: {
                break;
            }
        }

        result = xrPollEvent(mh_xrinstance, &xr_event_buffer);
    }

    if (!mb_should_run_framecycle) {
        return;
    }

    XrFrameState frame_state{XR_TYPE_FRAME_STATE};
    {//Wait frame
        XrFrameWaitInfo frame_wait_info = {
                .type = XR_TYPE_FRAME_WAIT_INFO,
        };
        v_qualify_xr(xrWaitFrame(mh_xrsession, &frame_wait_info, &frame_state));
    }

    if (!frame_state.shouldRender) {
        return;
    }

    {//Get views and update uniforms
        for (uint32_t i = 0; i < mv_views.size(); i++) {
            XrViewState view_state = {
                    .type = XR_TYPE_VIEW_STATE,
            };
            XrViewLocateInfo view_locate_info = {
                    .type = XR_TYPE_VIEW_LOCATE_INFO,
                    .viewConfigurationType = me_app_view_type,
                    .displayTime = frame_state.predictedDisplayTime,
                    .space = mmap_reference_spaces[XR_REFERENCE_SPACE_TYPE_STAGE],
            };

            uint32_t un_out_view_count;
            v_qualify_xr(
                    xrLocateViews(mh_xrsession, &view_locate_info, &view_state, mv_views.size(),
                                  &un_out_view_count, mv_views.data()));

            XrMatrix4x4f mat44_projection;
            XrMatrix4x4f_CreateProjectionFov(&mat44_projection, GRAPHICS_VULKAN, &mv_views[i].fov, 0, 1.f);

            XrMatrix4x4f mat44_view;
            XrMatrix4x4f_CreateFromRigidTransform(&mat44_view, &mv_views[i].pose);

            XrMatrix4x4f mat44_inv_view;
            XrMatrix4x4f_Invert(&mat44_inv_view, &mat44_view);

            XrMatrix4x4f mat44_mvp;
            XrMatrix4x4f_Multiply(&mat44_mvp, &mat44_projection, &mat44_inv_view);

            memcpy(mv_vpbuffer_mapped_uniforms[i], &mat44_mvp, sizeof(mat44_mvp));
        }
    }

    {//graphics
        XrFrameBeginInfo frame_begin_info = {
                .type = XR_TYPE_FRAME_BEGIN_INFO,
        };
        v_qualify_xr(xrBeginFrame(mh_xrsession, &frame_begin_info));

        std::vector<XrCompositionLayerDepthInfoKHR> v_composition_layer_depth_info(mv_views.size());
        std::vector<XrCompositionLayerProjectionView> v_composition_layer_projection_views(
                mv_views.size());

        v_qualify_vk(vkWaitForFences(mh_vkdevice, 1, &mh_fence_exec, VK_TRUE, UINT64_MAX));
        v_qualify_vk(vkResetFences(mh_vkdevice, 1, &mh_fence_exec));

        v_qualify_vk(vkResetCommandPool(mh_vkdevice, mh_command_pool, 0));

        for (uint32_t i = 0; i < mv_views.size(); i++) {
            uint32_t un_index = 0;
            XrSwapchainImageAcquireInfo swapchain_image_acquire_info = {
                    .type = XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO,
            };
            v_qualify_xr(
                    xrAcquireSwapchainImage(mv_sccolor[i].swapchain, &swapchain_image_acquire_info,
                                            &un_index));

            XrSwapchainImageWaitInfo swapchain_image_wait_info = {
                    .type = XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO,
                    .timeout = XR_INFINITE_DURATION,
            };
            v_qualify_xr(xrWaitSwapchainImage(mv_sccolor[i].swapchain, &swapchain_image_wait_info));

            VkCommandBufferBeginInfo command_buffer_begin_info = {
                    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
                    .pInheritanceInfo = nullptr,
            };
            v_qualify_vk(vkBeginCommandBuffer(mv_command_buffers[i], &command_buffer_begin_info));

            static std::vector<VkClearValue> v_vkclear_values = {
                    {
                            .color = {{0.f, 0.f, 0.f, 1.f}}
                    },
                    {
                            .depthStencil = {1.f, 0},
                    }
            };

            VkRenderPassBeginInfo render_pass_begin_info = {
                    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                    .renderPass = mh_vkrender_pass,
                    .framebuffer = mv_sccolor[i].v_framebuffers[un_index],
                    .renderArea = {
                            .offset = {0, 0},
                            .extent = mv_sccolor[i].extent
                    },
                    .clearValueCount = (uint32_t) v_vkclear_values.size(),
                    .pClearValues = v_vkclear_values.data()
            };
            vkCmdBeginRenderPass(mv_command_buffers[i], &render_pass_begin_info,
                                 VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(mv_command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                              mh_vkgraphics_pipeline);

            VkBuffer v_vertex_buffers[] = {mh_vkbuffer_vertex};
            VkDeviceSize v_offsets[] = {0};
            vkCmdBindVertexBuffers(mv_command_buffers[i], 0, 1, v_vertex_buffers, v_offsets);

            vkCmdBindDescriptorSets(
                    mv_command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mh_vkpipeline_layout, 0, 1, &mv_vkdescriptor_sets[i], 0, nullptr);

            vkCmdDraw(mv_command_buffers[i], gv_vertices.size(), 1, 0, 0);

            vkCmdEndRenderPass(mv_command_buffers[i]);
            v_qualify_vk(vkEndCommandBuffer(mv_command_buffers[i]));

            XrSwapchainImageReleaseInfo swapchain_image_release_info = {
                    .type = XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO,
            };
            v_qualify_xr(xrReleaseSwapchainImage(mv_sccolor[i].swapchain,
                                                 &swapchain_image_release_info));

            v_composition_layer_depth_info[i] = {
                    .type = XR_TYPE_COMPOSITION_LAYER_DEPTH_INFO_KHR,
                    .subImage = {
                            .swapchain = mv_scdepth[i].swapchain,
                            .imageRect = {
                                    .offset = {0, 0},
                                    .extent = {(int32_t) mv_scdepth[i].extent.width,
                                               (int32_t ) mv_scdepth[i].extent.height}
                            }
                    },
                    .minDepth = 0.f,
                    .maxDepth = 1.f,
                    .nearZ = 0.01f,
                    .farZ = 100.f,
            };
            v_composition_layer_projection_views[i] = {
                    .type = XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW,
                    .next = &v_composition_layer_depth_info[i],
                    .pose = mv_views[i].pose,
                    .fov = mv_views[i].fov,
                    .subImage = {
                            .swapchain = mv_sccolor[i].swapchain,
                            .imageRect = {
                                    .offset = {0, 0},
                                    .extent = {(int32_t) mv_sccolor[i].extent.width,
                                               (int32_t) mv_sccolor[i].extent.height}
                            },
                    }
            };
        }

        VkSubmitInfo submit_info = {
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .commandBufferCount = (uint32_t) mv_command_buffers.size(),
                .pCommandBuffers = mv_command_buffers.data(),
        };
        v_qualify_vk(vkQueueSubmit(mh_vkqueue, 1, &submit_info, mh_fence_exec));

        XrCompositionLayerProjection composition_layer_projection = {
                .type = XR_TYPE_COMPOSITION_LAYER_PROJECTION,
                .space = mmap_reference_spaces[XR_REFERENCE_SPACE_TYPE_STAGE],
                .viewCount = (uint32_t) mv_views.size(),
                .views = v_composition_layer_projection_views.data(),
        };

        std::vector<XrCompositionLayerBaseHeader *> v_layers_base{};
        v_layers_base.push_back(
                reinterpret_cast<XrCompositionLayerBaseHeader *>(&composition_layer_projection));

        XrFrameEndInfo frame_end_info = {
                .type = XR_TYPE_FRAME_END_INFO,
                .displayTime = frame_state.predictedDisplayTime,
                .environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE,
                .layerCount = (uint32_t) v_layers_base.size(),
                .layers = v_layers_base.data()
        };
        v_qualify_xr(xrEndFrame(mh_xrsession, &frame_end_info));
    }
}

Program::~Program() {

}