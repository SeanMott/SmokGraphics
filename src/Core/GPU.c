#include <SmokGraphics/Core/GPU.h>

#include <BTDSTD_C/Logger.h>

//#include <stdlib.h>

//#include <vector>

//initalizes the default values for the GPU create info
void SMGraphics_Core_GPU_CreateInfo_InitalizeDefaultValues(SMGraphics_Core_GPU_CreateInfo* info)
{
	info->isRenderingToTheScreen = 1; //is the GPU used for rendering to the screen
	
	//memset(info->mainFeatures, VK_FALSE, sizeof(VkPhysicalDeviceFeatures)); //the main features of the GPU
	info->mainFeatures.robustBufferAccess = VK_FALSE;
	info->mainFeatures.fullDrawIndexUint32 = VK_FALSE;
	info->mainFeatures.imageCubeArray = VK_FALSE;
	info->mainFeatures.independentBlend = VK_FALSE;
	info->mainFeatures.geometryShader = VK_FALSE;
	info->mainFeatures.tessellationShader = VK_FALSE;
	info->mainFeatures.sampleRateShading = VK_FALSE;
	info->mainFeatures.dualSrcBlend = VK_FALSE;
	info->mainFeatures.logicOp = VK_FALSE;
	info->mainFeatures.multiDrawIndirect = VK_FALSE;
	info->mainFeatures.drawIndirectFirstInstance = VK_FALSE;
	info->mainFeatures.depthClamp = VK_FALSE;
	info->mainFeatures.depthBiasClamp = VK_FALSE;
	info->mainFeatures.fillModeNonSolid = VK_FALSE;
	info->mainFeatures.depthBounds = VK_FALSE;
	info->mainFeatures.wideLines = VK_FALSE;
	info->mainFeatures.largePoints = VK_FALSE;
	info->mainFeatures.alphaToOne = VK_FALSE;
	info->mainFeatures.multiViewport = VK_FALSE;
	info->mainFeatures.samplerAnisotropy = VK_FALSE;
	info->mainFeatures.textureCompressionETC2 = VK_FALSE;
	info->mainFeatures.textureCompressionASTC_LDR = VK_FALSE;
	info->mainFeatures.textureCompressionBC = VK_FALSE;
	info->mainFeatures.occlusionQueryPrecise = VK_FALSE;
	info->mainFeatures.pipelineStatisticsQuery = VK_FALSE;
	info->mainFeatures.vertexPipelineStoresAndAtomics = VK_FALSE;
	info->mainFeatures.fragmentStoresAndAtomics = VK_FALSE;
	info->mainFeatures.shaderTessellationAndGeometryPointSize = VK_FALSE;
	info->mainFeatures.shaderImageGatherExtended = VK_FALSE;
	info->mainFeatures.shaderStorageImageExtendedFormats = VK_FALSE;
	info->mainFeatures.shaderStorageImageMultisample = VK_FALSE;
	info->mainFeatures.shaderStorageImageReadWithoutFormat = VK_FALSE;
	info->mainFeatures.shaderStorageImageWriteWithoutFormat = VK_FALSE;
	info->mainFeatures.shaderUniformBufferArrayDynamicIndexing = VK_FALSE;
	info->mainFeatures.shaderSampledImageArrayDynamicIndexing = VK_FALSE;
	info->mainFeatures.shaderStorageBufferArrayDynamicIndexing = VK_FALSE;
	info->mainFeatures.shaderStorageImageArrayDynamicIndexing = VK_FALSE;
	info->mainFeatures.shaderClipDistance = VK_FALSE;
	info->mainFeatures.shaderCullDistance = VK_FALSE;
	info->mainFeatures.shaderFloat64 = VK_FALSE;
	info->mainFeatures.shaderInt64 = VK_FALSE;
	info->mainFeatures.shaderInt16 = VK_FALSE;
	info->mainFeatures.shaderResourceResidency = VK_FALSE;
	info->mainFeatures.shaderResourceMinLod = VK_FALSE;
	info->mainFeatures.sparseBinding = VK_FALSE;
	info->mainFeatures.sparseResidencyBuffer = VK_FALSE;
	info->mainFeatures.sparseResidencyImage2D = VK_FALSE;
	info->mainFeatures.sparseResidencyImage3D = VK_FALSE;
	info->mainFeatures.sparseResidency2Samples = VK_FALSE;
	info->mainFeatures.sparseResidency4Samples = VK_FALSE;
	info->mainFeatures.sparseResidency8Samples = VK_FALSE;
	info->mainFeatures.sparseResidency16Samples = VK_FALSE;
	info->mainFeatures.sparseResidencyAliased = VK_FALSE;
	info->mainFeatures.variableMultisampleRate = VK_FALSE;
	info->mainFeatures.inheritedQueries = VK_FALSE;

	info->vk11Features = NULL;
	info->deviceDescriptorIndexingFeaturesNeeded = NULL;
}

//checks if the extension for the device is supported
static inline int checkDeviceExtensionSupport(VkPhysicalDevice gpu, const char* extensions[], const uint32_t _extensionCount)
{
	uint32_t avaibleExtensionCount;
	vkEnumerateDeviceExtensionProperties(gpu, NULL, &avaibleExtensionCount, NULL);

	VkExtensionProperties* availableExtensions = (VkExtensionProperties*)calloc(avaibleExtensionCount, sizeof(VkExtensionProperties));
	vkEnumerateDeviceExtensionProperties(gpu, NULL, &avaibleExtensionCount, availableExtensions);

	int isFound = 0;
	for (uint32_t e = 0; e < _extensionCount; ++e)
	{
		isFound = 0;
		for (uint32 ae = 0; ae < avaibleExtensionCount; ++ae)
		{
			//if it's there
			if (!strcmp(extensions[e], availableExtensions[ae].extensionName))
			{
				free(availableExtensions); availableExtensions = NULL;
				isFound = 1;
				break;
			}
		}

		//if it wasn;t found throw error
		if (!isFound)
		{
			free(availableExtensions); availableExtensions = NULL;
			printf("Smok Graphics Error: GPU, Func: \"checkDeviceExtensionSupport\" || GPU does not support %s\n", extensions[e]);
			return 0;
		}
	}

	free(availableExtensions); availableExtensions = NULL;
	return 1;
}

//defines a main feature array
#define GetMemOffsetOfDeviceMainFeatures(s) offsetof(VkPhysicalDeviceFeatures, s)
#define DEVICE_MAIN_FEATURE_COUNT 54
static const size_t MAIN_DEVICE_FEATURE_OFFSETS[DEVICE_MAIN_FEATURE_COUNT] = {
	GetMemOffsetOfDeviceMainFeatures(robustBufferAccess),
	GetMemOffsetOfDeviceMainFeatures(fullDrawIndexUint32),
	GetMemOffsetOfDeviceMainFeatures(imageCubeArray),
	GetMemOffsetOfDeviceMainFeatures(independentBlend),
	GetMemOffsetOfDeviceMainFeatures(geometryShader),
	GetMemOffsetOfDeviceMainFeatures(tessellationShader),
	GetMemOffsetOfDeviceMainFeatures(sampleRateShading),
	GetMemOffsetOfDeviceMainFeatures(dualSrcBlend),
	GetMemOffsetOfDeviceMainFeatures(logicOp),
	GetMemOffsetOfDeviceMainFeatures(multiDrawIndirect),
	GetMemOffsetOfDeviceMainFeatures(drawIndirectFirstInstance),
	GetMemOffsetOfDeviceMainFeatures(depthClamp),
	GetMemOffsetOfDeviceMainFeatures(depthBiasClamp),
	GetMemOffsetOfDeviceMainFeatures(fillModeNonSolid),
	GetMemOffsetOfDeviceMainFeatures(depthBounds),
	GetMemOffsetOfDeviceMainFeatures(wideLines),
	GetMemOffsetOfDeviceMainFeatures(largePoints),
	GetMemOffsetOfDeviceMainFeatures(alphaToOne),
	GetMemOffsetOfDeviceMainFeatures(multiViewport),
	GetMemOffsetOfDeviceMainFeatures(samplerAnisotropy),
	GetMemOffsetOfDeviceMainFeatures(textureCompressionETC2),
	GetMemOffsetOfDeviceMainFeatures(textureCompressionASTC_LDR),
	GetMemOffsetOfDeviceMainFeatures(textureCompressionBC),
	GetMemOffsetOfDeviceMainFeatures(occlusionQueryPrecise),
	GetMemOffsetOfDeviceMainFeatures(pipelineStatisticsQuery),
	GetMemOffsetOfDeviceMainFeatures(vertexPipelineStoresAndAtomics),
	GetMemOffsetOfDeviceMainFeatures(fragmentStoresAndAtomics),
	GetMemOffsetOfDeviceMainFeatures(shaderTessellationAndGeometryPointSize),
	GetMemOffsetOfDeviceMainFeatures(shaderImageGatherExtended),
	GetMemOffsetOfDeviceMainFeatures(shaderStorageImageExtendedFormats),
	GetMemOffsetOfDeviceMainFeatures(shaderStorageImageMultisample),
	GetMemOffsetOfDeviceMainFeatures(shaderStorageImageReadWithoutFormat),
	GetMemOffsetOfDeviceMainFeatures(shaderStorageImageWriteWithoutFormat),
	GetMemOffsetOfDeviceMainFeatures(shaderUniformBufferArrayDynamicIndexing),
	GetMemOffsetOfDeviceMainFeatures(shaderSampledImageArrayDynamicIndexing),
	GetMemOffsetOfDeviceMainFeatures(shaderStorageImageArrayDynamicIndexing),
	GetMemOffsetOfDeviceMainFeatures(shaderClipDistance),
	GetMemOffsetOfDeviceMainFeatures(shaderCullDistance),
	GetMemOffsetOfDeviceMainFeatures(shaderFloat64),
	GetMemOffsetOfDeviceMainFeatures(shaderInt64),
	GetMemOffsetOfDeviceMainFeatures(shaderInt16),
	GetMemOffsetOfDeviceMainFeatures(shaderResourceResidency),
	GetMemOffsetOfDeviceMainFeatures(shaderResourceMinLod),
	GetMemOffsetOfDeviceMainFeatures(sparseBinding),
	GetMemOffsetOfDeviceMainFeatures(sparseResidencyBuffer),
	GetMemOffsetOfDeviceMainFeatures(sparseResidencyImage2D),
	GetMemOffsetOfDeviceMainFeatures(sparseResidencyImage3D),
	GetMemOffsetOfDeviceMainFeatures(sparseResidency2Samples),
	GetMemOffsetOfDeviceMainFeatures(sparseResidency4Samples),
	GetMemOffsetOfDeviceMainFeatures(sparseResidency8Samples),
	GetMemOffsetOfDeviceMainFeatures(sparseResidency16Samples),
	GetMemOffsetOfDeviceMainFeatures(sparseResidencyAliased),
	GetMemOffsetOfDeviceMainFeatures(variableMultisampleRate),
	GetMemOffsetOfDeviceMainFeatures(inheritedQueries)
};

//checks for main GPU features
static inline int CheckForMainGPUFeatures(VkPhysicalDevice device, const VkPhysicalDeviceFeatures deviceFeatureNeed)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	return (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		deviceFeatures.samplerAnisotropy == VK_TRUE);

	//VkPhysicalDeviceFeatures deviceFeatures;
	//vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	//
	////checks the features
	//for (uint32 i = 0; i < DEVICE_MAIN_FEATURE_COUNT; ++i)
	//{
	//	//if it doesn't have the needed feature
	//	if ((*((VkBool32*)((uint8*)((&deviceFeatureNeed)) + MAIN_DEVICE_FEATURE_OFFSETS[i]))) && !(*((VkBool32*)((uint8*)((&deviceFeatures)) + MAIN_DEVICE_FEATURE_OFFSETS[i]))))
	//		return 0;
	//}

	//return 1;
}

#define GetMemOffsetOfDeviceExtraIndexingFeatures(s) offsetof(VkPhysicalDeviceDescriptorIndexingFeaturesEXT, s)
#define DEVICE_EXTRA_INDEXING_FEATURE_COUNT 20
static const size_t EXTRA_DEVICE_FEATURE_INDEXING_OFFSETS[DEVICE_EXTRA_INDEXING_FEATURE_COUNT] = {
	GetMemOffsetOfDeviceExtraIndexingFeatures(shaderInputAttachmentArrayDynamicIndexing),
	GetMemOffsetOfDeviceExtraIndexingFeatures(shaderUniformTexelBufferArrayDynamicIndexing),
	GetMemOffsetOfDeviceExtraIndexingFeatures(shaderStorageTexelBufferArrayDynamicIndexing),
	GetMemOffsetOfDeviceExtraIndexingFeatures(shaderUniformBufferArrayNonUniformIndexing),
	GetMemOffsetOfDeviceExtraIndexingFeatures(shaderSampledImageArrayNonUniformIndexing),
	GetMemOffsetOfDeviceExtraIndexingFeatures(shaderStorageBufferArrayNonUniformIndexing),
	GetMemOffsetOfDeviceExtraIndexingFeatures(shaderStorageImageArrayNonUniformIndexing),
	GetMemOffsetOfDeviceExtraIndexingFeatures(shaderInputAttachmentArrayNonUniformIndexing),
	GetMemOffsetOfDeviceExtraIndexingFeatures(shaderUniformTexelBufferArrayNonUniformIndexing),
	GetMemOffsetOfDeviceExtraIndexingFeatures(shaderStorageTexelBufferArrayNonUniformIndexing),
	GetMemOffsetOfDeviceExtraIndexingFeatures(descriptorBindingUniformBufferUpdateAfterBind),
	GetMemOffsetOfDeviceExtraIndexingFeatures(descriptorBindingSampledImageUpdateAfterBind),
	GetMemOffsetOfDeviceExtraIndexingFeatures(descriptorBindingStorageImageUpdateAfterBind),
	GetMemOffsetOfDeviceExtraIndexingFeatures(descriptorBindingStorageBufferUpdateAfterBind),
	GetMemOffsetOfDeviceExtraIndexingFeatures(descriptorBindingUniformTexelBufferUpdateAfterBind),
	GetMemOffsetOfDeviceExtraIndexingFeatures(descriptorBindingStorageTexelBufferUpdateAfterBind),
	GetMemOffsetOfDeviceExtraIndexingFeatures(descriptorBindingUpdateUnusedWhilePending),
	GetMemOffsetOfDeviceExtraIndexingFeatures(descriptorBindingPartiallyBound),
	GetMemOffsetOfDeviceExtraIndexingFeatures(descriptorBindingVariableDescriptorCount),
	GetMemOffsetOfDeviceExtraIndexingFeatures(runtimeDescriptorArray)
};

//checks for extra indexing features of a GPU
static inline int CheckForExtraGPUFeatures_IndexingFeatures(VkPhysicalDevice device, const VkPhysicalDeviceDescriptorIndexingFeatures* deviceFeatureNeed)
{
	VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures;// = { 0 };
	indexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
	indexingFeatures.pNext = NULL;

	VkPhysicalDeviceFeatures2 deviceFeatures;// = { 0 };
	deviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	deviceFeatures.pNext = &indexingFeatures;
	vkGetPhysicalDeviceFeatures2(device, &deviceFeatures);

	//checks the features
	for (uint32 i = 0; i < DEVICE_EXTRA_INDEXING_FEATURE_COUNT; ++i)
	{
		//if it doesn't have the needed feature
		if ((*((VkBool32*)((uint8*)((&deviceFeatureNeed)) + EXTRA_DEVICE_FEATURE_INDEXING_OFFSETS[i]))) &&
			!(*((VkBool32*)((uint8*)((&deviceFeatures)) + EXTRA_DEVICE_FEATURE_INDEXING_OFFSETS[i]))))
			return 0;
	}

	return 1;
}

//defines the queue familys
typedef struct QueueFamilyIndices
{
	uint32_t graphicsFamily;
	uint32_t presentFamily;
	int graphicsFamilyHasValue;
	int presentFamilyHasValue;
} QueueFamilyIndices;

//checks if the queue indices is complete and has both present and present queue
static inline int QueueFamilyIndices_IsComplete(QueueFamilyIndices* indices)
{
	return (indices->graphicsFamilyHasValue == 1 && indices->presentFamilyHasValue == 1);
}

static inline int FindQueueFamilies(QueueFamilyIndices* indices, VkPhysicalDevice gpu, VkSurfaceKHR _surface)
{
	int wasFound = 0;

	uint32_t queueFamilyCount = 0; 
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, NULL);
	if (!queueFamilyCount)
		return wasFound;

	VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*)calloc(queueFamilyCount, sizeof(VkQueueFamilyProperties));
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, queueFamilies);

	int i = 0;
	//gets the graphics bits
	for (uint32 q = 0; q < queueFamilyCount; ++q) {
		if ((queueFamilies[q].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
			indices->graphicsFamily = i;
			indices->graphicsFamilyHasValue = 1;
		}

		//gets the present mode
		VkBool32 presentSupport = 0;
		vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, _surface, &presentSupport);
		if (presentSupport) {
			indices->presentFamily = i;
			indices->presentFamilyHasValue = 1;
		}

		//if it has everything return it
		if (QueueFamilyIndices_IsComplete(indices)) {
			{
				wasFound = 1;
				break;
			}
		}

		i++;
	}

	free(queueFamilies); queueFamilies = NULL;
	return wasFound;
}

//is the device sutiable
static inline int IsDeviceSuitable(VkPhysicalDevice gpu, VkSurfaceKHR surface, QueueFamilyIndices* indices,
	const char* deviceExtensions[], const uint32 deviceExtensionCount,
	VkPhysicalDeviceFeatures mainFeatures,
	VkPhysicalDeviceVulkan11Features* vk11Features,
	VkPhysicalDeviceDescriptorIndexingFeaturesEXT* deviceDescriptorIndexingFeaturesNeeded)
{
	int state = 0;

	//if it has the basic features desired
	state = CheckForMainGPUFeatures(gpu, mainFeatures);
	if (!state)
	{
		BTD_LogError("Smok Graphics", "GPU", "IsDeviceSuitable", "Failed to find basic GPU features!");
		return 0;
	}

	//checks for device extensions
	if (deviceExtensionCount > 0)
	{
		state = checkDeviceExtensionSupport(gpu, deviceExtensions, deviceExtensionCount);
		if (!state)
		{
			BTD_LogError("Smok Graphics", "GPU", "IsDeviceSuitable", "GPU does not support device extensions!");
			return 0;
		}
	}

	//if extra features are wanted
	if (deviceDescriptorIndexingFeaturesNeeded != NULL)
	{
		state = CheckForExtraGPUFeatures_IndexingFeatures(gpu, deviceDescriptorIndexingFeaturesNeeded);
		if (!state)
		{
			BTD_LogError("Smok Graphics", "GPU", "IsDeviceSuitable", "GPU does not support Descriptor Indexing feature!");
			return 0;
		}
	}

	//checks if it has the graphics familes and present modes
	state = FindQueueFamilies(indices, gpu, surface);
	if (!state)
	{
		BTD_LogError("Smok Graphics", "GPU", "IsDeviceSuitable", "GPU does not have a present and graphics queue!");
		return 0;
	}

	return 1;
}

int SMGraphics_Core_GPU_Create(SMGraphics_Core_GPU* gpu, const SMGraphics_Core_GPU_CreateInfo info,
	VkSurfaceKHR surface, VkInstance vulkanInstance)
{
	uint32_t deviceCount;
	vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, NULL);
	if (!deviceCount)
	{
		BTD_LogError("Smok Graphics", "GPU", "SMGraphics_Core_GPU_Create", "No GPUs were found, can not get GPU! Please check you support Vulkan.");
		return 0;
	}

	QueueFamilyIndices indices;

	//if the swapchain and rendering to the screen is needed
	const char* deviceExtensions[] = {
						 VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	
	if (info.isRenderingToTheScreen)
	{
		//gets the GPU you want if it can
		VkPhysicalDevice* devices = (VkPhysicalDevice*)calloc(deviceCount, sizeof(VkPhysicalDevice));
		vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, devices);

		for (uint32 i = 0; i < deviceCount; ++i)
		{
			if (IsDeviceSuitable(devices[i], surface, &indices,
				deviceExtensions, 1,
				info.mainFeatures, info.vk11Features, info.deviceDescriptorIndexingFeaturesNeeded))
			{
				gpu->gpu = devices[i];
				break;
			}
		}
		free(devices);
	}

	//if it's only for compute and that's all we need a GPU for
	else
	{
		BTD_LogError("Smok Graphics", "GPU", "SMGraphics_Core_GPU_Create", "Smok Graphics does not currently support only getting the GPU for compute stuff!");
	}

	//if it wasn't found
	if (gpu->gpu == VK_NULL_HANDLE)
	{
		BTD_LogError("Smok Graphics", "GPU", "SMGraphics_Core_GPU_Create", "No GPUs were found, can not get GPU! Please check you support Vulkan.");
		return 0;
	}

	//creates a device
	uint32 queueFamilyCount = 0; VkDeviceQueueCreateInfo queueCreateInfos[3];
	const float queuePriorities[1] = { 1.0f };
	
	//initalizes the default zero out values for the queue create info
	queueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfos[0].pNext = NULL;
	queueCreateInfos[0].flags = 0;
	queueCreateInfos[0].queueFamilyIndex = 0;
	queueCreateInfos[0].queueCount = 1;
	queueCreateInfos[0].pQueuePriorities = queuePriorities;
	VkDeviceQueueCreateInfo* dstQueueCreateInfo = &queueCreateInfos[1];
	dstQueueCreateInfo = (VkDeviceQueueCreateInfo*)memcpy(dstQueueCreateInfo, &queueCreateInfos[0], sizeof(VkDeviceQueueCreateInfo));
	dstQueueCreateInfo = &queueCreateInfos[2];
	dstQueueCreateInfo = (VkDeviceQueueCreateInfo*)memcpy(dstQueueCreateInfo, &queueCreateInfos[0], sizeof(VkDeviceQueueCreateInfo));

	//if they're the same
	if (indices.presentFamily == indices.graphicsFamily)
	{
		queueFamilyCount = 1;
		queueCreateInfos[0].queueFamilyIndex = indices.presentFamily;
	}

	//if they are not
	else
	{
		queueFamilyCount = 2;
		queueCreateInfos[0].queueFamilyIndex = indices.graphicsFamily;
		queueCreateInfos[1].queueFamilyIndex = indices.presentFamily;
	}

	//create the logical device
	VkDeviceCreateInfo deviceCreateInfo;// = { 0 };
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.queueCreateInfoCount = queueFamilyCount;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos;

	deviceCreateInfo.pEnabledFeatures = &info.mainFeatures;

	//sets layers (ignored for implementations)
	deviceCreateInfo.enabledLayerCount = 0;
	deviceCreateInfo.ppEnabledLayerNames = NULL;

	//sets the desired extensions
	if (info.isRenderingToTheScreen)
	{
		deviceCreateInfo.enabledExtensionCount = 1;
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;
	}
	else
	{
		deviceCreateInfo.enabledExtensionCount = 0;
		deviceCreateInfo.ppEnabledExtensionNames = NULL;
	}

	deviceCreateInfo.pNext = info.vk11Features;
	deviceCreateInfo.flags = 0;

	//creates the device
	if (vkCreateDevice(gpu->gpu, &deviceCreateInfo, NULL, &gpu->device) != VK_SUCCESS)
	{
		BTD_LogError("Smok Graphics", "GPU", "CreateGPU", "Failed to create a Logical Device, check that the GPU is valid and this system supports Vulkan!");
		return 0;
	}

	//copies the features
	gpu->mainFeatures = info.mainFeatures;
	gpu->graphicsQueueIndex = indices.graphicsFamily;
	gpu->presentQueueIndex = indices.presentFamily;

	//gets the queue
	vkGetDeviceQueue(gpu->device, gpu->graphicsQueueIndex, 0, &gpu->graphicsQueue);
	vkGetDeviceQueue(gpu->device, gpu->presentQueueIndex, 0, &gpu->presentQueue);

	//gets stats
	vkGetPhysicalDeviceProperties(gpu->gpu, &gpu->GPUProperties);

	return 1;
}

void SMGraphics_Core_GPU_Destroy(SMGraphics_Core_GPU* gpu)
{
	vkDestroyDevice(gpu->device, NULL); //destroys gpu
}