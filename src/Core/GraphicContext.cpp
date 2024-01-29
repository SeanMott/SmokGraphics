#include <SmokGraphics/Core/GraphicContext.hpp>

#include <BTDSTD/Types.hpp>
#include <BTDSTD/Logger.hpp>

#include <GLFW/glfw3.h>

//default logging entries
static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDefaultLayerLogger(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	printf("validation layer: %s\n", pCallbackData->pMessage);

	return VK_FALSE;
}

//checks for validation layer
static inline bool CheckValLayers(const const char* valLayers[], const uint32& _layerCount)
{
	//gets layers
	uint32_t aviableLayerCount;
	vkEnumerateInstanceLayerProperties(&aviableLayerCount, nullptr);

	VkLayerProperties* availableLayers = (VkLayerProperties*)calloc(aviableLayerCount, sizeof(VkLayerProperties));
	vkEnumerateInstanceLayerProperties(&aviableLayerCount, availableLayers);

	//goes through them and checks agiesnt the requested ones
	for (uint32 l = 0; l < _layerCount; ++l)
	{
		bool layerFound = false;

		for (uint32 al = 0; al < aviableLayerCount; ++al)
		{
			if (!strcmp(valLayers[l], availableLayers[al].layerName))
			{
				free(availableLayers); availableLayers = nullptr;
				layerFound = true;
				break;
			}
		}

		//if the layer was not found
		if (!layerFound)
		{
			free(availableLayers); availableLayers = nullptr;

			//throw error
			printf("No Layers\n");
			return false;
		}
	}

	free(availableLayers); availableLayers = nullptr;
	return true;
}

bool Smok::Graphics::Core::InitalizeGraphicsContext(Smok::Graphics::Core::GraphicsContext& graphicsContext, const Smok::Graphics::Core::GraphicsContextCreateInfo& info)
{
	graphicsContext.isDebug = info.isDebug;
	graphicsContext.applicationName = info.appName;

	//inits GLFW
	if (!glfwInit())
	{
		BTD::Logger::LogError("Smok Graphics", "GPU", "CreateVulkanInstance", "Failed to init GLFW!");
		return false;
	}

	//application data
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = graphicsContext.applicationName;// .data;
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pEngineName = "None";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	//vulkanInstance data
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//get extensions
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	if (!glfwExtensions)
	{
		BTD::Logger::LogError("Smok Graphics", "Graphic Context", "CreateVulkanInstance", "Failed to get GLFW extensions!");
		return false;
	}

	//const char** extentions = (const char**)calloc((info.isDebug == true ? glfwExtensionCount + 1 : glfwExtensionCount), sizeof(char*));

	std::vector<const char*> extentions; extentions.resize((info.isDebug == true ? glfwExtensionCount + 1 : glfwExtensionCount));
	for (uint32 i = 0; i < glfwExtensionCount; ++i)
		extentions[i] = glfwExtensions[i];

	if (info.isDebug)
		extentions[glfwExtensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
	
	createInfo.enabledExtensionCount = (info.isDebug == true ? glfwExtensionCount + 1 : glfwExtensionCount);
	createInfo.ppEnabledExtensionNames = extentions.data();
	
	//get layers
	const char* valLayers[] = {
			"VK_LAYER_KHRONOS_validation"
	};
	if (info.isDebug && CheckValLayers(valLayers, 1))
	{
		createInfo.enabledLayerCount = 1;
		createInfo.ppEnabledLayerNames = valLayers;
	}
	else
		createInfo.enabledLayerCount = 0;

	//debug logger
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (info.isDebug)
	{
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;

		debugCreateInfo.pfnUserCallback = VulkanDefaultLayerLogger;

		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}

	//create vulkanInstance
	if (vkCreateInstance(&createInfo, nullptr, &graphicsContext.vulkanInstance) != VK_SUCCESS)
	{
		printf("Vulfix Creation Error: vulkanInstance || Failed to create the vulkanInstance!\n");
		return false;
	}

	//creates debug logger
	if (info.isDebug)
		((PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(graphicsContext.vulkanInstance,
			"vkCreateDebugUtilsMessengerEXT"))(graphicsContext.vulkanInstance, &debugCreateInfo, nullptr, &graphicsContext.logger);

	////clean up extension strings
	//for (uint32 i = 0; i < glfwExtensionCount; ++i)
	//	free((void*)extentions[i]);
	//free(extentions);

	return true;
}

void Smok::Graphics::Core::DeinitalizeGraphicsContext(Smok::Graphics::Core::GraphicsContext& graphicsContext)
{
	if (graphicsContext.vulkanInstance == VK_NULL_HANDLE)
		return;

	if (graphicsContext.logger != VK_NULL_HANDLE)
		((PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(graphicsContext.vulkanInstance,
			"vkDestroyDebugUtilsMessengerEXT"))(graphicsContext.vulkanInstance, graphicsContext.logger, nullptr);

	vkDestroyInstance(graphicsContext.vulkanInstance, nullptr);

	glfwTerminate();
}