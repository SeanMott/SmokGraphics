#pragma once

//image C API for images

#include <BTDSTD_C/Types.h>

#include <vma/vk_mem_alloc.h>

#if defined(__cplusplus)
extern "C" {
#endif

	//-------------------------IMAGE FUNCTIONS--------------------------------//

	//creates image
	inline int SMGraphics_Util_Image_Create(VkImage* image, VmaAllocation* memory, VkImageCreateInfo* info, VmaAllocator allocator)
	{
		VmaAllocationCreateInfo allocCreateInfo;
		allocCreateInfo.flags = 0;
		allocCreateInfo.memoryTypeBits = 0;
		allocCreateInfo.pool = VK_NULL_HANDLE;
		allocCreateInfo.preferredFlags = 0;
		allocCreateInfo.pUserData = NULL;
		allocCreateInfo.requiredFlags = 0;

		allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
		allocCreateInfo.priority = 1.0f;

		if (vmaCreateImage(allocator, info, &allocCreateInfo, image, memory, NULL) != VK_SUCCESS)
		{
			BTD_LogError("Smok Graphics", "Image", "SMGraphics_Util_Image_Create", "Failed to create image!");
			return 0;
		}

		return 1;
	}

	//destroys image
	inline void SMGraphics_Util_Image_Destroy(VkImage image, VmaAllocation memory, VmaAllocator allocator)
	{
		vmaDestroyImage(allocator, image, memory);
	}

	//generates a image create info
	inline VkImageCreateInfo SMGraphics_Util_Image_CreateInfo_Default()
	{
		VkImageCreateInfo imageInfo;
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.flags = 0;
		imageInfo.pNext = NULL;

		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		
		imageInfo.extent.width = 0;
		imageInfo.extent.height = 0;
		
		imageInfo.extent.depth = 1;
		
		imageInfo.mipLevels = 1;

		imageInfo.arrayLayers = 1;
		
		imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

		return imageInfo;
	}

	//sets the image create info type
	inline void SMGraphics_Util_Image_CreateInfo_SetImageType(VkImageCreateInfo* imageInfo, VkImageType type)
	{
		imageInfo->imageType = type;
	}

	//sets the image create info size
	inline void SMGraphics_Util_Image_CreateInfo_SetSize(VkImageCreateInfo* imageInfo, const uint32 width, const uint32 height)
	{
		imageInfo->extent.width = width;
		imageInfo->extent.height = height;
	}

	//sets the image create info depth
	inline void SMGraphics_Util_Image_CreateInfo_SetDepth(VkImageCreateInfo* imageInfo, const uint32 depth)
	{
		imageInfo->extent.depth = depth;
	}

	//sets the image create info mip levels
	inline void SMGraphics_Util_Image_CreateInfo_SetMipLevels(VkImageCreateInfo* imageInfo, const uint32 mipLevels)
	{
		imageInfo->mipLevels = mipLevels;
	}

	//sets the image create info array layers
	inline void SMGraphics_Util_Image_CreateInfo_SetArrayLevels(VkImageCreateInfo* imageInfo, const uint32 arrayLayers)
	{
		imageInfo->arrayLayers = arrayLayers;
	}

	//sets the image create info format
	inline void SMGraphics_Util_Image_CreateInfo_SetFormat(VkImageCreateInfo* imageInfo, const VkFormat format)
	{
		imageInfo->format = format;
	}

	//sets the image create info tiling
	inline void SMGraphics_Util_Image_CreateInfo_SetTiling(VkImageCreateInfo* imageInfo, const VkImageTiling tiling)
	{
		imageInfo->tiling = VK_IMAGE_TILING_OPTIMAL;
	}

	//sets the image create info initial layout
	inline void SMGraphics_Util_Image_CreateInfo_SetInitalLayout(VkImageCreateInfo* imageInfo, const VkImageLayout initalLayout)
	{
		imageInfo->initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	}

	//sets the image create info usage
	inline void SMGraphics_Util_Image_CreateInfo_SetUsage(VkImageCreateInfo* imageInfo, const VkImageUsageFlags usage)
	{
		imageInfo->usage = usage;
	}

	//sets the image create info sharing mode
	inline void SMGraphics_Util_Image_CreateInfo_SetSharingMode(VkImageCreateInfo* imageInfo, const VkSharingMode sharingMode)
	{
		imageInfo->sharingMode = sharingMode;
	}

	//sets the image create info samples
	inline void SMGraphics_Util_Image_CreateInfo_SetSamplingCount(VkImageCreateInfo* imageInfo, const VkSampleCountFlagBits samplingCount)
	{
		imageInfo->samples = samplingCount;
	}

	//-------------------------IMAGE VIEW FUNCTIONS--------------------------------//

	//creates a image view
	inline int SMGraphics_Util_ImageView_Create(VkImageView* view, const VkImageViewCreateInfo* info, VkDevice device)
	{
		if (vkCreateImageView(device, info, NULL, view) != VK_SUCCESS)
		{
			BTD_LogError("Smok Graphics", "Image", "SMGraphics_Util_ImageView_Create", "Failed to create image image!");
			return 0;
		}

		return 1;
	}

	//destroys a image view
	inline void SMGraphics_Util_ImageView_Destroy(VkImageView view, VkDevice device)
	{
		vkDestroyImageView(device, view, NULL);
	}

	//generates a image view create info
	inline VkImageViewCreateInfo SMGraphics_Util_ImageView_CreateInfo_Default()
	{
		VkImageViewCreateInfo imageViewCreateInfo;
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.flags = 0;
		imageViewCreateInfo.pNext = NULL;

		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

		imageViewCreateInfo.image = NULL;

		imageViewCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;

		imageViewCreateInfo.subresourceRange.layerCount = 1;

		return imageViewCreateInfo;
	}

	//sets the image view create info type
	inline void SMGraphics_Util_ImageView_CreateInfo_SetViewType(VkImageViewCreateInfo* info, const VkImageViewType type)
	{
		info->viewType = type;
	}

	//sets the image view create info image
	inline void SMGraphics_Util_ImageView_CreateInfo_SetImage(VkImageViewCreateInfo* info, VkImage image)
	{
		info->image = image;
	}

	//sets the image view create info format
	inline void SMGraphics_Util_ImageView_CreateInfo_SetFormat(VkImageViewCreateInfo* info, const VkFormat format)
	{
		info->format = format;
	}

	
	//sets the image view create info component swizzle
	inline void SMGraphics_Util_ImageView_CreateInfo_SetComponentSwizzle(VkImageViewCreateInfo* info,
		const VkComponentSwizzle r, const VkComponentSwizzle g, const VkComponentSwizzle b, const VkComponentSwizzle a)
	{
		info->components.r = r;
		info->components.g = g;
		info->components.b = b;
		info->components.a = a;
	}

	//sets the image view create info aspect mask
	inline void SMGraphics_Util_ImageView_CreateInfo_SetAspectMask(VkImageViewCreateInfo* info, const VkImageAspectFlagBits aspectMask)
	{
		info->subresourceRange.aspectMask = aspectMask;
	}

	//sets the image view create info base mip level
	inline void SMGraphics_Util_ImageView_CreateInfo_SetBaseMipLevel(VkImageViewCreateInfo* info, const uint32 baseMipLevel)
	{
		info->subresourceRange.baseMipLevel = baseMipLevel;
	}

	//sets the image view create info level count
	inline void SMGraphics_Util_ImageView_CreateInfo_SetLevelCount(VkImageViewCreateInfo* info, const uint32 levelCount)
	{
		info->subresourceRange.levelCount = levelCount;
	}

	//sets the image view create info base array layer
	inline void SMGraphics_Util_ImageView_CreateInfo_SetBaseArrayLayer(VkImageViewCreateInfo* info, const uint32 baseArrayLayer)
	{
		info->subresourceRange.baseArrayLayer = baseArrayLayer;
	}

	//sets the image view create info layer count
	inline void SMGraphics_Util_ImageView_CreateInfo_SetLayerCount(VkImageViewCreateInfo* info, const uint32 layerCount)
	{
		info->subresourceRange.layerCount = layerCount;
	}

#if defined(__cplusplus)
}
#endif