#pragma once

//defines getting a chosen GPU

#include <BTDSTD_C/Types.h>

#include <vulkan/vulkan.h>

//defines a list of extensions to check for

//defines a create info for GPU
typedef struct SMGraphics_Core_GPU_CreateInfo
{
	int isRenderingToTheScreen; //is the GPU used for rendering to the screen
	VkPhysicalDeviceFeatures mainFeatures; //the main features of the GPU

	VkPhysicalDeviceVulkan11Features* vk11Features;
	VkPhysicalDeviceDescriptorIndexingFeaturesEXT* deviceDescriptorIndexingFeaturesNeeded;
} SMGraphics_Core_GPU_CreateInfo;

#if defined(__cplusplus)
extern "C" {
#endif

//initalizes the default values for the GPU create info
void SMGraphics_Core_GPU_CreateInfo_InitalizeDefaultValues(SMGraphics_Core_GPU_CreateInfo* info);

#if defined(__cplusplus)
}
#endif

//defines a GPU
typedef struct SMGraphics_Core_GPU
{
	VkPhysicalDevice gpu; VkDevice device;
	VkQueue graphicsQueue; VkQueue presentQueue;
	uint32 graphicsQueueIndex; uint32 presentQueueIndex;

	VkPhysicalDeviceProperties GPUProperties; //the properties of the GPU
	VkPhysicalDeviceFeatures mainFeatures; //the main features of the GPU
} SMGraphics_Core_GPU;

#if defined(__cplusplus)
extern "C" {
#endif

//creates a GPU
int SMGraphics_Core_GPU_Create(SMGraphics_Core_GPU* gpu, const SMGraphics_Core_GPU_CreateInfo info,
	VkSurfaceKHR surface, VkInstance vulkanInstance);

//destroys a GPU
void SMGraphics_Core_GPU_Destroy(SMGraphics_Core_GPU* gpu);

#if defined(__cplusplus)
}
#endif