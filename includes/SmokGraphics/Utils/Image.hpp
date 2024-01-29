#pragma once

//defines a image and sampler

#include <BTDSTD/Math/Vector.hpp>
#include <BTDSTD/IO/YAML.hpp>
#include <BTDSTD/IO/File.hpp>

#include <SmokGraphics/Core/GPU.h>
#include <SmokGraphics/Pools/CommandPool.h>
#include <SmokGraphics/Utils/Buffer.h>

#include <SmokGraphics/Utils/Image.h>

namespace Smok::Graphics::Util::Image
{
	////defines a image create info
	//struct ImageCreateInfo
	//{
	//	VkImageType imageType = VK_IMAGE_TYPE_2D;
	//	VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
	//	BTD_Math_U32Vec2 size = { 1, 1 };
	//	uint32 depth = 1;
	//	uint32_t mipLevels = 1;
	//	uint32_t arrayLayers = 1;
	//	VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
	//	VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
	//	VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	//	VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//	VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//};

	////creates a image
	//int SMGraphics_Util_Image_Create(VkImage* image, VmaAllocation* memoy, const ImageCreateInfo* createInfo, VmaAllocator allocator)
	//{
	//	//creates the depth images
	//	VkImageCreateInfo imageInfo = {0};
	//	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	//	imageInfo.imageType = createInfo->imageType;
	//	imageInfo.extent.width = createInfo->size.x;
	//	imageInfo.extent.height = createInfo->size.y;
	//	imageInfo.extent.depth = createInfo->depth;
	//	imageInfo.mipLevels = createInfo->mipLevels;
	//	imageInfo.arrayLayers = createInfo->arrayLayers;

	//	imageInfo.format = createInfo->format;

	//	imageInfo.tiling = createInfo->tiling;

	//	imageInfo.initialLayout = createInfo->initialLayout;

	//	imageInfo.usage = createInfo->usage;

	//	imageInfo.sharingMode = createInfo->sharingMode;

	//	imageInfo.samples = createInfo->samples;
	//	imageInfo.flags = 0; // Optional

	//	VmaAllocationCreateInfo allocCreateInfo = { 0 };
	//	allocCreateInfo.flags = 0;
	//	allocCreateInfo.memoryTypeBits = 0;
	//	allocCreateInfo.pool = VK_NULL_HANDLE;
	//	allocCreateInfo.preferredFlags = 0;
	//	allocCreateInfo.pUserData = NULL;
	//	allocCreateInfo.requiredFlags = 0;

	//	allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
	//	allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
	//	allocCreateInfo.priority = 1.0f;

	//	if (vmaCreateImage(allocator, &imageInfo, &allocCreateInfo, image, memoy, NULL) != VK_SUCCESS)
	//	{
	//		BTD_LogError("Smok Graphics", "Image", "SMGraphics_Util_Image_Create", "Failed to create image!");
	//		return 0;
	//	}

	//	return 1;
	//}

	//destroys a image
	/*inline void SMGraphics_Util_Image_Destroy(VkImage image, VmaAllocation memoy, VmaAllocator allocator)
	{
		vmaDestroyImage(allocator, image, memoy);
	}*/

	////defines a image view create info
	//struct ViewCreateInfo
	//{
	//	VkImageViewType type = VK_IMAGE_VIEW_TYPE_2D;
	//	VkFormat format = VK_FORMAT_B8G8R8A8_SRGB;
	//	VkComponentMapping components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
	//	VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	//	VkImageSubresourceRange subresourceRange = {
	//		VK_IMAGE_ASPECT_COLOR_BIT,
	//		0,
	//		1,
	//		0,
	//		1,
	//	};
	//};

	////creates a image view
	//int SMGraphics_Util_View_Create(VkImageView* view, VkImage image,
	//	ViewCreateInfo* info,
	//	VkDevice device)
	//{
	//	VkImageViewCreateInfo createInfo = {
	//	 VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
	//	 NULL,
	//	 0,
	//	 image,
	//	info->type,
	//	info->format,
	//	info->components,
	//	info->subresourceRange
	//	};

	//	if (vkCreateImageView(device, &createInfo, NULL, view) != VK_SUCCESS)
	//	{
	//		BTD_LogError("Smok Graphics", "View", "SMGraphics_Util_View_Create", "Failed to create image view!");
	//		return 0;
	//	}

	//	return 1;
	//}

	////destroys a image view
	//inline void SMGraphics_Util_View_Destroy(VkImageView view, VkDevice device)
	//{
	//	vkDestroyImageView(device, view, NULL);
	//}

	////defines a counpound image view
	//struct Image_View
	//{
	//	VkImage image = VK_NULL_HANDLE;
	//	VmaAllocation memoy = VK_NULL_HANDLE;
	//	VkImageView view = VK_NULL_HANDLE;

	//	ImageCreateInfo imageSettings;
	//	ViewCreateInfo viewSettings;
	//};
	

	//defines a Sampler 2D create info

	//defines a sampler 2D

	//creates a sampler 2D

	//destroys a sampler 2D

	////defines the decl data of a image
	//struct Image_DeclData
	//{
	//	ImageCreateInfo info;

	//	std::string directory = "", fileName = "", //the output file directory
	//		binaryDataPath = ""; //points to the path of the binary data file for this data 
	//};

	////writes a image decl file
	//inline bool Image_Decl_WriteFile(const std::string& directory, const std::string& fileName,
	//	const std::string binaryDataPath, const VkImageCreateInfo& info)
	//{
	//	//checks if the file exists/can be made
	//	BTD::IO::File file;
	//	if (!file.Open(std::string(directory + "/" + fileName + ".img2Ddecl"), BTD::IO::FileOP::TextWrite_OpenCreateStart))
	//		return false;

	//	//starts the file
	//	YAML::Emitter emitter;
	//	emitter << YAML::BeginMap;

	//	BTD_IO_YAML_WriteVarValue(emitter, info, type);
	//	BTD_IO_YAML_WriteVarValue(emitter, info, format);
	//	//BTD_IO_YAML_WriteVarValue(emitter, task, info.size);
	//	BTD_IO_YAML_WriteVarValue(emitter, info, info.depth);
	//	BTD_IO_YAML_WriteVarValue(emitter, info, info.mipLevels);
	//	BTD_IO_YAML_WriteVarValue(emitter, info, info.arrayLayers);
	//	BTD_IO_YAML_WriteVarValue(emitter, info, info.samples);
	//	BTD_IO_YAML_WriteVarValue(emitter, info, info.tiling);
	//	BTD_IO_YAML_WriteVarValue(emitter, info, info.usage);
	//	BTD_IO_YAML_WriteVarValue(emitter, info, info.initialLayout);

	//	BTD_IO_YAML_WriteStringValue(emitter, task, binaryDataPath);

	//	VkImageType              imageType;
	//	VkFormat                 format;
	//	VkExtent3D               extent;
	//	uint32_t                 mipLevels;
	//	uint32_t                 arrayLayers;
	//	VkSampleCountFlagBits    samples;
	//	VkImageTiling            tiling;
	//	VkImageUsageFlags        usage;
	//	VkSharingMode            sharingMode;
	//	uint32_t                 queueFamilyIndexCount;
	//	const uint32_t* pQueueFamilyIndices;
	//	VkImageLayout            initialLayout;

	//	emitter << YAML::EndMap;

	//	//write decl file
	//	file.Write(emitter.c_str());
	//	file.Close();

	//	return true;
	//}

	////loads a image decl file
	//inline bool Image_Decl_LoadFile(const std::string& declFilePath, Image_DeclData& task)
	//{
	//	//checks if the decl file exists
	//	BTD::IO::File file;
	//	if (!file.Open(declFilePath, BTD::IO::FileOP::TextRead_OpenExisting))
	//	{
	//		BTD::Logger::LogError("Smok Graphics", "Image", "Image_Decl_WriteFile",
	//			std::string("The given decl file path can't be opened at \"" + declFilePath + "\"").c_str());
	//		return false;
	//	}

	//	//loads the YAML data
	//	YAML::Node _data = YAML::Load(file.Read());
	//	file.Close();

	//	if (!_data)
	//	{
	//		BTD::Logger::LogError("Smok Graphics", "Image", "Image_Decl_WriteFile",
	//			"The decl file is invalid!");
	//		return false;
	//	}

	//	//gets the file settings
	//	uint32 dataPtr = 0; BTD::Math::U32Vec2 dataPtr2; BTD_Math_U32Vec2 dataPtr3;

	//	BTD_IO_YAML_LoadVarValue(task, Image_DeclData, _data, dataPtr, uint32, info.imageType);
	//	BTD_IO_YAML_LoadVarValue(task, Image_DeclData, _data, dataPtr, uint32, info.format);
	//	//BTD_IO_YAML_LoadVarValue(task, Image_DeclData, _data, dataPtr3, BTD_Math_U32Vec2, info.size);
	//	BTD_IO_YAML_LoadVarValue(task, Image_DeclData, _data, dataPtr, uint32, info.depth);
	//	BTD_IO_YAML_LoadVarValue(task, Image_DeclData, _data, dataPtr, uint32, info.mipLevels);
	//	BTD_IO_YAML_LoadVarValue(task, Image_DeclData, _data, dataPtr, uint32, info.arrayLayers);
	//	BTD_IO_YAML_LoadVarValue(task, Image_DeclData, _data, dataPtr, uint32, info.samples);
	//	BTD_IO_YAML_LoadVarValue(task, Image_DeclData, _data, dataPtr, uint32, info.tiling);
	//	BTD_IO_YAML_LoadVarValue(task, Image_DeclData, _data, dataPtr, uint32, info.usage);
	//	BTD_IO_YAML_LoadVarValue(task, Image_DeclData, _data, dataPtr, uint32, info.initialLayout);

	//	BTD_IO_YAML_LoadStringValue(task, _data, binaryDataPath);

	//	return true;
	//}

	////creates a image from a buffer of data
	//inline bool Image_CreateWithData(Image_View* image,
	//	SMGraphics_Util_Buffer* imageDataStagingBuffer,
	//	VmaAllocator& allocator, SMGraphics_Core_GPU* GPU, SMGraphics_Pool_CommandPool* commandPool)
	//{
	//	//creates a image
	//	if (!SMGraphics_Util_Image_Create(&image->image, &image->memoy, &image->imageSettings, allocator))
	//		return false;

	//	//starts a single use buffer so we can transfer to a useable image
	//	SMGraphics_Pool_CommandBuffer comBuffer;
	//	SMGraphics_Pool_CommandPool_StartSingleUseCommandBuffer(commandPool, &comBuffer, GPU->device);

	//	//transition image to a format that can have data written into it
	//	VkImageSubresourceRange range;
	//	range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	//	range.baseMipLevel = 0;
	//	range.levelCount = 1;
	//	range.baseArrayLayer = 0;
	//	range.layerCount = 1;

	//	VkImageMemoryBarrier imageBarrier_toTransfer = {};
	//	imageBarrier_toTransfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

	//	imageBarrier_toTransfer.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//	imageBarrier_toTransfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	//	imageBarrier_toTransfer.image = image->image;
	//	imageBarrier_toTransfer.subresourceRange = range;

	//	imageBarrier_toTransfer.srcAccessMask = 0;
	//	imageBarrier_toTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	//	//barrier the image into the transfer-receive layout
	//	vkCmdPipelineBarrier(comBuffer.comBuffer,
	//		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
	//		0, 0, nullptr, 0, nullptr, 1, &imageBarrier_toTransfer);

	//	//copies data into the image
	//	VkBufferImageCopy copyRegion = {};
	//	copyRegion.bufferOffset = 0;
	//	copyRegion.bufferRowLength = 0;
	//	copyRegion.bufferImageHeight = 0;

	//	copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	//	copyRegion.imageSubresource.mipLevel = 0;
	//	copyRegion.imageSubresource.baseArrayLayer = 0;
	//	copyRegion.imageSubresource.layerCount = 1;
	//	copyRegion.imageExtent = { image->imageSettings.size.x, image->imageSettings.size.y, 1 };

	//	//copy the buffer into the image
	//	vkCmdCopyBufferToImage(comBuffer.comBuffer,
	//		imageDataStagingBuffer->buffer, image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

	//	//transitions the image into one that can be read by the shader
	//	VkImageMemoryBarrier imageBarrier_toReadable = imageBarrier_toTransfer;

	//	imageBarrier_toReadable.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	//	imageBarrier_toReadable.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	//	imageBarrier_toReadable.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	//	imageBarrier_toReadable.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	//	//barrier the image into the shader readable layout
	//	vkCmdPipelineBarrier(comBuffer.comBuffer,
	//		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
	//		0, 0, nullptr, 0, nullptr, 1, &imageBarrier_toReadable);

	//	SMGraphics_Pool_CommandPool_EndSingleUseCommandBuffer(commandPool, &comBuffer, GPU->device, GPU->graphicsQueue);

	//	//creates view
	//	VkImageViewCreateInfo viewInfo{};
	//	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	//	viewInfo.image = image->image;
	//	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	//	viewInfo.format = image->imageSettings.format;
	//	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	//	viewInfo.subresourceRange.baseMipLevel = 0;
	//	viewInfo.subresourceRange.levelCount = 1;
	//	viewInfo.subresourceRange.baseArrayLayer = 0;
	//	viewInfo.subresourceRange.layerCount = 1;

	//	vkCreateImageView(GPU->device, &viewInfo, nullptr, &image->view);

	//	return true;
	//}

	//defines a sampler 2D decl data
	struct Sampler2D_DeclData
	{
		VkFilter filters = VK_FILTER_NEAREST;
		VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;

		std::string directory = "", fileName = ""; //the directory and file name for decl
	};

	//writes a sampler 2D decl file
	inline bool Sampler2D_Decl_WriteFile(const Sampler2D_DeclData& task)
	{
		//checks if the file exists/can be made
		BTD::IO::File file;
		if (!file.Open(std::string(task.directory + "/" + task.fileName + ".sap2Ddecl"), BTD::IO::FileOP::TextWrite_OpenCreateStart))
			return false;

		//starts the file
		YAML::Emitter emitter;
		emitter << YAML::BeginMap;

		emitter << YAML::Key << "filter" << YAML::Value << task.filters;
		//BTD_IO_YAML_WriteVarValue(emitter, task, filters);
		emitter << YAML::Key << "addressMode" << YAML::Value << task.addressMode;
		//BTD_IO_YAML_WriteVarValue(emitter, task, addressMode);

		emitter << YAML::EndMap;

		//write decl file
		file.Write(emitter.c_str());
		file.Close();

		return true;
	}

	//loads a sampler 2D decl file
	bool Sampler2D_Decl_LoadFile(const std::string& declFilePath, Sampler2D_DeclData& data)
	{
		//checks if the decl file exists
		BTD::IO::File file;
		if (!file.Open(declFilePath, BTD::IO::FileOP::TextRead_OpenExisting))
		{
			BTD::Logger::LogError("Smok Graphics", "Sampler", "Sampler2D_Decl_LoadFile",
				std::string("The given decl file path can't be opened at \"" + declFilePath + "\"").c_str());
			return false;
		}

		//loads the YAML data
		YAML::Node _data = YAML::Load(file.Read());
		file.Close();

		if (!_data)
		{
			BTD::Logger::LogError("Smok Graphics", "Sampler", "Sampler2D_Decl_LoadFile",
				"The decl file is invalid!");
			return false;
		}

		//gets the file settings
		uint32 dataPtr = 0;
		data.filters = (VkFilter)_data["filter"].as<unsigned int>();
		data.addressMode = (VkSamplerAddressMode)_data["addressMode"].as<unsigned int>();
		//BTD_IO_YAML_LoadVarValue(data, Sampler2D_DeclData, _data, dataPtr, uint32, addressMode);
		//BTD_IO_YAML_LoadVarValue(data, Sampler2D_DeclData, _data, dataPtr, uint32, filters);

		return true;
	}

	//defines a sampler 2D
	struct Sampler2D
	{
		uint64 assetID = 0; //asset ID for the asset manager

		VkSampler sampler = VK_NULL_HANDLE;
		VkFilter filters = VK_FILTER_NEAREST;
		VkSamplerAddressMode samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;

		std::string declPath;

		//~Sampler2D() { BTD_String_DestroyString(&declPath); }
	};

	//creates a sampler 2D
	inline bool Sampler2D_Create(Sampler2D* sampler, SMGraphics_Core_GPU* GPU, const Sampler2D_DeclData& createInfo)
	{
		VkSamplerCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		info.pNext = nullptr;

		info.magFilter = createInfo.filters;
		info.minFilter = createInfo.filters;
		info.addressModeU = createInfo.addressMode;
		info.addressModeV = createInfo.addressMode;
		info.addressModeW = createInfo.addressMode;

		vkCreateSampler(GPU->device, &info, nullptr, &sampler->sampler);

		return true;
	}

	//destroys a sampler 2D
	inline void Sampler2D_Destroy(Sampler2D* sampler, SMGraphics_Core_GPU* GPU)
	{
		vkDestroySampler(GPU->device, sampler->sampler, nullptr);
	}
}