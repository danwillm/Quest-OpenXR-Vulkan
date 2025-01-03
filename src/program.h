#pragma once

#include <array>
#include <unordered_map>
#include <vector>

#include "android_native_app_glue.h"

#include "main.h"

#include "vulkan/vulkan.h"

#include "openxr/openxr.h"
#include "openxr/openxr_platform.h"

struct SwapchainInfo {
    XrSwapchain swapchain = XR_NULL_HANDLE;

    std::vector<XrSwapchainImageVulkan2KHR> v_images;
    std::vector<VkImageView> v_image_views;

    VkFormat vk_format;

    VkExtent2D extent = {};

    std::vector<VkFramebuffer> v_framebuffers{};
};

struct ViewFrameInfo {

};

class Program {
public:
    Program(android_app *p_app, app_state *p_app_state);

    bool BInit();

    void Tick();

    ~Program();

private:
    android_app *mp_android_app;
    app_state *mp_app_state;

    XrInstance mh_xrinstance;
    XrSystemId mh_xrsystem_id;
    XrSession mh_xrsession;
    bool mb_is_session_running = false;
    bool mb_should_run_framecycle = false;

    XrViewConfigurationType me_app_view_type;
    std::vector<XrViewConfigurationView> mv_view_config_views;
    std::unordered_map<XrReferenceSpaceType, XrSpace> mmap_reference_spaces;
    std::vector<XrView> mv_views;

    std::array<SwapchainInfo, 2> mv_sccolor{};
    std::array<SwapchainInfo, 2> mv_scdepth{};

    XrDebugUtilsMessengerEXT mh_xrdebug_utils_messenger;

    VkInstance mh_vkinstance;
    VkPhysicalDevice mh_vkphysical_device;
    VkDevice mh_vkdevice;
    VkQueue mh_vkqueue;
    VkPipelineLayout mh_vkpipeline_layout;
    VkRenderPass mh_vkrender_pass;
    VkPipeline mh_vkgraphics_pipeline;

    VkBuffer mh_vkbuffer_vertex;
    VkDeviceMemory mh_vkmemory_vertex;

    VkDescriptorSetLayout mh_vkdescriptor_set_layout;
    VkDescriptorPool mh_vkdescriptor_pool;
    std::vector<VkDescriptorSet> mv_vkdescriptor_sets;
    std::vector<VkBuffer> mv_vkbuffer_uniforms;
    std::vector<VkDeviceMemory> mv_vkmemory_uniforms;
    std::vector<void*> mv_vpbuffer_mapped_uniforms;

    VkCommandPool mh_command_pool;
    std::vector<VkCommandBuffer> mv_command_buffers;
    VkFence mh_fence_exec;

    std::vector<VkViewport> mv_vkviewports{};

    uint32_t mun_queue_family;
    VkDebugUtilsMessengerEXT mh_vkdebug_utils_messenger;

    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;

    PFN_xrInitializeLoaderKHR xrInitializeLoaderKHR;
    PFN_xrCreateDebugUtilsMessengerEXT xrCreateDebugUtilsMessengerEXT;
    PFN_xrGetVulkanGraphicsRequirements2KHR xrGetVulkanGraphicsRequirements2KHR;
    PFN_xrCreateVulkanInstanceKHR xrCreateVulkanInstanceKHR;
    PFN_xrGetVulkanGraphicsDevice2KHR xrGetVulkanGraphicsDevice2KHR;
    PFN_xrCreateVulkanDeviceKHR xrCreateVulkanDeviceKHR;
};