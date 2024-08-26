#include "program.h"

#include <string>
#include <vector>

#include "log.h"

constexpr XrPosef k_xr_pose_identity = {
        .orientation = {
                .w= 1.f,
        },
};

#define qualify_xr(x) do {                                              \
        XrResult ret = x;                                               \
        if(XR_FAILED(ret)) {                                            \
            Log(LogError, "[QualifyXR] %s failed with: %i", #x, ret);   \
            return false;                                               \
        }                                                               \
    } while(0)                                                          \

#define qualify_vk(x) do {                                              \
        VkResult ret = x;                                               \
        if(ret != VK_SUCCESS) {                                         \
            Log(LogError, "[QualifyVK] %s failed with: %i", #x, ret);   \
            return false;                                               \
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
        qualify_xr(xrGetInstanceProcAddr(instance, #name, (PFN_xrVoidFunction *) &name));   \
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
                                                               mp_app_state(p_app_state) {
}

bool Program::BInit() {
    //OpenXR Instance
    {
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
                        .apiVersion = XR_CURRENT_API_VERSION,
                },
                .enabledExtensionCount = static_cast<uint32_t>(v_cs_enabled_extensions.size()),
                .enabledExtensionNames = v_cs_enabled_extensions.data(),
        };
        qualify_xr(xrCreateInstance(&xr_instance_create_info, &mxr_instance));

        XrDebugUtilsMessengerCreateInfoEXT xr_debug_info{XR_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
        xr_debug_info.messageSeverities = XR_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
#if !defined(NDEBUG)
        xr_debug_info.messageSeverities |=
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
#endif
        xr_debug_info.messageTypes = XR_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                     XR_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        xr_debug_info.userCallback = XrDebugCallback;
        qualify_xr(xrCreateDebugUtilsMessengerEXT(mxr_instance, &xr_debug_info, &mxr_debug_utils_messenger));

        //OpenXR Function bindings
        xr_get_proc(mxr_instance, xrGetVulkanGraphicsRequirements2KHR);
        xr_get_proc(mxr_instance, xrCreateVulkanInstanceKHR);
    }

    //OpenXR System Properties
    {
        XrSystemGetInfo xr_system_get_info = {
                .type = XR_TYPE_SYSTEM_GET_INFO,
                .formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY,
        };
        qualify_xr(xrGetSystem(mxr_instance, &xr_system_get_info, &mxr_system_id));

        XrSystemProperties xr_system_properties = {XR_TYPE_SYSTEM_PROPERTIES};
        qualify_xr(xrGetSystemProperties(mxr_instance, mxr_system_id, &xr_system_properties));
    }

    { //Vulkan Initialization
        XrGraphicsRequirementsVulkan2KHR xr_graphics_requirements_vulkan{XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN2_KHR};
        qualify_xr(xrGetVulkanGraphicsRequirements2KHR(mxr_instance, mxr_system_id, &xr_graphics_requirements_vulkan));

        std::vector<const char *> v_enabled_layers{};

        { //Vulkan Layers
#if !defined(NDEBUG)
            const char *s_validation_layer_name = []() -> const char * { //Get Vulkan Validation Layer
                uint32_t un_layer_count;
                d_qualify_vk(vkEnumerateInstanceLayerProperties(&un_layer_count, nullptr));

                std::vector<VkLayerProperties> v_available_layers(un_layer_count);
                d_qualify_vk(vkEnumerateInstanceLayerProperties(&un_layer_count, v_available_layers.data()));

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

                Log(LogWarning, "Could not find a validation layer!");
                return nullptr;
            }();

            if (!s_validation_layer_name) {
                v_enabled_layers.push_back(s_validation_layer_name);
            }
#endif
        }

        std::vector<const char *> v_requested_extensions = {};

        {//Vulkan Extensions
            uint32_t un_extension_count = 0;
            qualify_vk(vkEnumerateInstanceExtensionProperties(nullptr, &un_extension_count, nullptr));

            std::vector<VkExtensionProperties> v_available_extensions(un_extension_count);
            qualify_vk(vkEnumerateInstanceExtensionProperties(nullptr, &un_extension_count, v_available_extensions.data()));

            auto BIsExtensionSupported = [&](const char *pc_extension_name) -> bool {
                auto it = std::find_if(v_available_extensions.begin(), v_available_extensions.end(), [&](const VkExtensionProperties &vk_extension_properties) {
                    return strcmp(pc_extension_name, vk_extension_properties.extensionName) == 0;
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
                .apiVersion = VK_API_VERSION_1_0,
        };

        VkInstanceCreateInfo vk_instance_info = {
                .pApplicationInfo = &vk_application_info,
                .enabledLayerCount = static_cast<uint32_t>(v_enabled_layers.size()),
                .ppEnabledLayerNames = v_enabled_layers.data(),
                .enabledExtensionCount = static_cast<uint32_t>(v_requested_extensions.size()),
                .ppEnabledExtensionNames = v_requested_extensions.data()
        };

        XrVulkanInstanceCreateInfoKHR xr_vulkan_instance_create_info = {
                .systemId = mxr_system_id,
                .pfnGetInstanceProcAddr = &vkGetInstanceProcAddr,
                .vulkanCreateInfo = &vk_instance_info,
                .vulkanAllocator = nullptr,
        };

        {//Create Vulkan Instance
            VkResult vk_error;
            qualify_xr(xrCreateVulkanInstanceKHR(mxr_instance, &xr_vulkan_instance_create_info, &mvk_instance, &vk_error));
            qualify_vk(vk_error);

            vk_get_proc(mvk_instance, vkCreateDebugUtilsMessengerEXT);
            vk_get_proc(mvk_instance, vkDestroyDebugUtilsMessengerEXT);

            VkDebugUtilsMessengerCreateInfoEXT vk_debug_info{VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
            vk_debug_info.messageSeverity =
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
#if !defined(NDEBUG)
            vk_debug_info.messageSeverity |=
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
#endif
            vk_debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            vk_debug_info.pfnUserCallback = VkDebugCallback;
            vk_debug_info.pUserData = this;
            qualify_vk(vkCreateDebugUtilsMessengerEXT(mvk_instance, &vk_debug_info, nullptr, &mvk_debug_utils_messenger));
        }
    }

    {//Vulkan device creation

        XrVulkanGraphicsDeviceGetInfoKHR xr_vk_device_info = {
                .type = XR_TYPE_VULKAN_GRAPHICS_DEVICE_GET_INFO_KHR,
                .systemId = mxr_system_id,
                .vulkanInstance = mvk_instance,
        };
        qualify_xr(xrGetVulkanGraphicsDevice2KHR(mxr_instance, &xr_vk_device_info, &mvk_physical_device));

        uint32_t un_queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(mvk_physical_device, &un_queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> v_queue_family_properties(un_queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(mvk_physical_device, &un_queue_family_count, v_queue_family_properties.data());

        for (uint32_t i = 0; i < v_queue_family_properties.size(); i++) {
            if (v_queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                mvkindex_queue_family = i;
                break;
            }
        }

        float f_queue_priorities = 1.f;
        VkDeviceQueueCreateInfo queueInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = mvkindex_queue_family,
                .queueCount = 1,
                .pQueuePriorities = &f_queue_priorities,
        };

        std::vector<const char *> v_device_extensions;

        VkPhysicalDeviceFeatures vk_physical_device_features{};

        VkDeviceCreateInfo vk_device_create_info = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .queueCreateInfoCount = 1,
                .pQueueCreateInfos = &queueInfo,
                .enabledLayerCount = 0,
                .ppEnabledLayerNames = nullptr,
                .enabledExtensionCount = static_cast<uint32_t>(v_device_extensions.size()),
                .ppEnabledExtensionNames = v_device_extensions.data(),
                .pEnabledFeatures = &vk_physical_device_features
        };

        XrVulkanDeviceCreateInfoKHR xr_vulkan_device_create_info = {
                .type = XR_TYPE_VULKAN_DEVICE_CREATE_INFO_KHR,
                .systemId = mxr_system_id,
                .pfnGetInstanceProcAddr = &vkGetInstanceProcAddr,
                .vulkanPhysicalDevice = mvk_physical_device,
                .vulkanCreateInfo = &vk_device_create_info,
                .vulkanAllocator = nullptr
        };

        VkResult vk_err;
        qualify_xr(xrCreateVulkanDeviceKHR(mxr_instance, &xr_vulkan_device_create_info, &mvk_device, &vk_err));
        qualify_vk(vk_err);
    }

    {//Create OpenXR Session
        XrGraphicsBindingVulkanKHR xr_graphics_binding_vulkan = {
                .type = XR_TYPE_GRAPHICS_BINDING_VULKAN2_KHR,
                .instance = mvk_instance,
                .physicalDevice = mvk_physical_device,
                .device = mvk_device,
                .queueFamilyIndex = mvkindex_queue_family,
                .queueIndex = 0
        };
        XrSessionCreateInfo xr_session_create_info = {
                .type = XR_TYPE_SESSION_CREATE_INFO,
                .next = &xr_graphics_binding_vulkan,
                .createFlags = 0,
                .systemId = mxr_system_id
        };
        qualify_xr(xrCreateSession(mxr_instance, &xr_session_create_info, &mxr_session));

        for (XrReferenceSpaceType xr_reference_space_type: {XR_REFERENCE_SPACE_TYPE_VIEW,
                                                            XR_REFERENCE_SPACE_TYPE_STAGE,
                                                            XR_REFERENCE_SPACE_TYPE_LOCAL_FLOOR_EXT}) {//Create OpenXR Reference spaces
            XrReferenceSpaceCreateInfo xr_reference_space_create_info = {
                    .type = XR_TYPE_REFERENCE_SPACE_CREATE_INFO,
                    .referenceSpaceType = xr_reference_space_type,
                    .poseInReferenceSpace = k_xr_pose_identity,
            };

            qualify_xr(xrCreateReferenceSpace(mxr_session, &xr_reference_space_create_info, &mmap_reference_spaces[xr_reference_space_type]));
        }
    }

    {//OpenXR View configuration
        uint32_t un_view_config_count;
        qualify_xr(xrEnumerateViewConfigurations(mxr_instance, mxr_system_id, 0, &un_view_config_count, nullptr));

        std::vector<XrViewConfigurationType> v_view_config_types(un_view_config_count);
        qualify_xr(xrEnumerateViewConfigurations(mxr_instance, mxr_system_id, v_view_config_types.size(), &un_view_config_count, v_view_config_types.data()));

        me_app_view_type = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

        if (std::find(v_view_config_types.begin(), v_view_config_types.end(), XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO) == v_view_config_types.end()) {
            throw std::runtime_error("[Program] View configuration STEREO was not supported on runtime");
        }

        uint32_t un_view_config_views_count;
        qualify_xr(xrEnumerateViewConfigurationViews(mxr_instance, mxr_system_id, me_app_view_type, 0, &un_view_config_views_count, nullptr));

        mv_view_config_views.resize(un_view_config_views_count);
        qualify_xr(xrEnumerateViewConfigurationViews(mxr_instance, mxr_system_id, me_app_view_type, mv_view_config_views.size(), &un_view_config_views_count,
                                                     mv_view_config_views.data()));
    }

    {//Create swapchains
        uint32_t un_swapchain_formats_count;
        qualify_xr(xrEnumerateSwapchainFormats(mxr_session, 0, &un_swapchain_formats_count, nullptr));

        std::vector<int64_t> v_swapchain_formats(un_swapchain_formats_count);
        qualify_xr(xrEnumerateSwapchainFormats(mxr_session, v_swapchain_formats.size(), &un_swapchain_formats_count, v_swapchain_formats.data()));

        auto GetSupportedSwapchainFormat = [](const std::vector<VkFormat> &vVkSupportedFormats, std::vector<int64_t> &vLAvailableFormats) -> uint64_t {
            auto it = std::find_first_of(vLAvailableFormats.begin(), vLAvailableFormats.end(), std::begin(vVkSupportedFormats), std::end(vVkSupportedFormats));

            if (it == vLAvailableFormats.end()) {
                return 0;
            }

            return *it;
        };

        const std::vector<VkFormat> vvk_color_formats = {
                VK_FORMAT_B8G8R8A8_SRGB,
                VK_FORMAT_R8G8B8A8_SRGB,
                VK_FORMAT_B8G8R8A8_UNORM,
                VK_FORMAT_R8G8B8A8_UNORM
        };
        const std::vector<VkFormat> vvk_depth_formats = {
                VK_FORMAT_D32_SFLOAT,
                VK_FORMAT_D16_UNORM
        };

        int64_t l_supported_color_format = GetSupportedSwapchainFormat(vvk_color_formats, v_swapchain_formats);
        int64_t l_supported_depth_format = GetSupportedSwapchainFormat(vvk_color_formats, v_swapchain_formats);

        if (l_supported_color_format == 0 || l_supported_depth_format == 0) {
            throw std::runtime_error("[Program] No supported swapchain format for depth or color was supported!");
        }

        for (size_t i = 0; i < mv_view_config_views.size(); i++) {
            {//Color swapchain
                XrSwapchainCreateInfo xr_swapchain_color_create_info = {
                        .type = XR_TYPE_SWAPCHAIN_CREATE_INFO,
                        .createFlags = 0,
                        .usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT,
                        .format = l_supported_color_format,
                        .sampleCount = mv_view_config_views[i].recommendedSwapchainSampleCount,
                        .width = mv_view_config_views[i].recommendedImageRectWidth,
                        .height = mv_view_config_views[i].recommendedImageRectHeight,
                        .faceCount = 1,
                        .arraySize = 1,
                        .mipCount = 1,
                };
                qualify_xr(xrCreateSwapchain(mxr_session, &xr_swapchain_color_create_info, &mswapchain_color.swapchain));


                uint32_t un_swapchain_image_count;
                qualify_xr(xrEnumerateSwapchainImages(mswapchain_color.swapchain, 0, &un_swapchain_image_count, nullptr));

                auto &swapchain_images = mswapchain_color.v_images;
                swapchain_images.resize(un_swapchain_image_count, {XR_TYPE_SWAPCHAIN_IMAGE_VULKAN2_KHR});
                qualify_xr(xrEnumerateSwapchainImages(mswapchain_color.swapchain, swapchain_images.size(), &un_swapchain_image_count,
                                                      reinterpret_cast<XrSwapchainImageBaseHeader *>(swapchain_images.data())));
            }

            {//Depth swapchain
                XrSwapchainCreateInfo xr_swapchain_depth_create_info = {
                        .type = XR_TYPE_SWAPCHAIN_CREATE_INFO,
                        .createFlags = 0,
                        .usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                        .format = l_supported_color_format,
                        .sampleCount = mv_view_config_views[i].recommendedSwapchainSampleCount,
                        .width = mv_view_config_views[i].recommendedImageRectWidth,
                        .height = mv_view_config_views[i].recommendedImageRectHeight,
                        .faceCount = 1,
                        .arraySize = 1,
                        .mipCount = 1,
                };
                qualify_xr(xrCreateSwapchain(mxr_session, &xr_swapchain_depth_create_info, &mswapchain_depth.swapchain));


                uint32_t un_swapchain_image_count;
                qualify_xr(xrEnumerateSwapchainImages(mswapchain_depth.swapchain, 0, &un_swapchain_image_count, nullptr));

                auto &swapchain_images = mswapchain_depth.v_images;
                swapchain_images.resize(un_swapchain_image_count, {XR_TYPE_SWAPCHAIN_IMAGE_VULKAN2_KHR});
                qualify_xr(xrEnumerateSwapchainImages(mswapchain_depth.swapchain, swapchain_images.size(), &un_swapchain_image_count,
                                                      reinterpret_cast<XrSwapchainImageBaseHeader *>(swapchain_images.data())));
            }
        }

    }

    return true;
}

void Program::Tick() {
    Log("Hi");
}

Program::~Program() {

}