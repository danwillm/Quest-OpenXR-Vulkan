#pragma once

#include <unordered_map>
#include <vector>

#include "android_native_app_glue.h"

#include "main.h"

#include "vulkan/vulkan.h"

#include "openxr/openxr.h"
#include "openxr/openxr_platform.h"

struct SwapchainInfo {
    XrSwapchain swapchain = XR_NULL_HANDLE;

    uint32_t un_image_count = 0;
    std::vector<XrSwapchainImageVulkan2KHR> v_images;

    uint32_t un_width = 0;
    uint32_t un_height = 0;
};

class Program
{
public:
    Program( android_app* p_app, app_state* p_app_state );

    bool BInit();

    void Tick();

    ~Program();

private:
    android_app* mp_android_app;
    app_state* mp_app_state;

    XrInstance mxr_instance;
    XrSystemId mxr_system_id;
    XrSession mxr_session;

    XrViewConfigurationType me_app_view_type;
    std::vector<XrViewConfigurationView> mv_view_config_views;
    std::unordered_map<XrReferenceSpaceType, XrSpace> mmap_reference_spaces;

    SwapchainInfo mswapchain_color{};
    SwapchainInfo mswapchain_depth{};

    XrDebugUtilsMessengerEXT mxr_debug_utils_messenger;

    VkInstance mvk_instance;
    VkPhysicalDevice mvk_physical_device;
    VkDevice mvk_device;
    VkQueue mvk_queue;
    uint32_t mvkindex_queue_family;
    VkDebugUtilsMessengerEXT mvk_debug_utils_messenger;

    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT{nullptr};
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT{nullptr};

    PFN_xrCreateDebugUtilsMessengerEXT xrCreateDebugUtilsMessengerEXT;
    PFN_xrGetVulkanGraphicsRequirements2KHR xrGetVulkanGraphicsRequirements2KHR;
    PFN_xrCreateVulkanInstanceKHR xrCreateVulkanInstanceKHR;
    PFN_xrGetVulkanGraphicsDevice2KHR xrGetVulkanGraphicsDevice2KHR;
    PFN_xrCreateVulkanDeviceKHR xrCreateVulkanDeviceKHR;
};