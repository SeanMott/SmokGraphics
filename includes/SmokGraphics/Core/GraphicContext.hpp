#pragma once

//defines a graphic context for storing info

#include <vulkan/vulkan.h>

namespace Smok::Graphics::Core
{
	//defines a platform
	enum class Platform
	{
		Windows_X64_Ver10 = 0, //Windows 10, X64

		Count
	};

	//returns a string based on the platform
	inline const char* GetPlatformString(const Platform& platform)
	{
		switch (platform)
		{
		case Platform::Windows_X64_Ver10:
			return "Windows_X64_Ver10";

		default:
			return "The given platform is not valid";
		}
	}

	//defines a render API
	enum class RenderAPI
	{
		Vulkan = 0, //vulkan

		Count
	};

	//returns a string based on the render API
	inline const char* GetRenderAPIString(const RenderAPI& API)
	{
		switch (API)
		{
		case RenderAPI::Vulkan:
			return "Vulkan";

		default:
			return "The given render API is not valid";
		}
	}

	//defines a create info for a graphics context
	struct GraphicsContextCreateInfo
	{
		bool isDebug = true;

		Platform platform = Platform::Count;
		RenderAPI renderAPI = RenderAPI::Count;

		char* appName;
	};

	//defines a graphic context
	struct GraphicsContext
	{
		bool isDebug = true;

		Platform platform = Platform::Count;
		RenderAPI renderAPI = RenderAPI::Count;

		VkInstance vulkanInstance = VK_NULL_HANDLE; VkDebugUtilsMessengerEXT logger = VK_NULL_HANDLE;
	
		char* applicationName;
	};

	//initalizes a graphics context
	bool InitalizeGraphicsContext(GraphicsContext& graphicsContext, const GraphicsContextCreateInfo& info);

	//deinitalizes a graphics context
	void DeinitalizeGraphicsContext(GraphicsContext& graphicsContext);
}