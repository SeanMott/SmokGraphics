#pragma once

//defines a common buffer util

#include <BTDSTD_C/Logger.h>

#include <vma/vk_mem_alloc.h>

	//defines a buffer create info
	typedef struct SMGraphics_Util_Buffer_CreateInfo
	{
		VkBufferCreateInfo bufferInfo;
		VmaAllocationCreateInfo allocInfo;
	} SMGraphics_Util_Buffer_CreateInfo;

#if defined(__cplusplus)
	extern "C" {
#endif
		//initalizes the default values for the buffer create info
		inline void SMGraphics_Util_Buffer_CreateInfo_InitalizeDefaultValues(SMGraphics_Util_Buffer_CreateInfo* info)
		{
			info->bufferInfo = {0};
			info->bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info->bufferInfo.flags = 0;
			
			info->allocInfo = {0};
			info->allocInfo.flags = 0;
			info->allocInfo.memoryTypeBits = 0;
			info->allocInfo.pool = VK_NULL_HANDLE;
			info->allocInfo.preferredFlags = 0;
			info->allocInfo.pUserData = NULL;
			info->allocInfo.requiredFlags = 0;
		}

#if defined(__cplusplus)
	}
#endif

	//defines a buffer
	typedef struct SMGraphics_Util_Buffer
	{
		size_t size; //the memory size
		VkBuffer buffer; //the buffer
		VmaAllocation memory; //the allocated memory
		VmaAllocationInfo allocationInfo; //the info about the allocation
		void* mappedData; //the mapped memory
	} SMGraphics_Util_Buffer;

#if defined(__cplusplus)
	extern "C" {
#endif

		//initalizes the default values for the buffer
		inline void SMGraphics_Util_Buffer_InitalizeDefaultValues(SMGraphics_Util_Buffer* buffer)
		{
			buffer->size = 0; //the memory size
			buffer->buffer = VK_NULL_HANDLE; //the buffer
			buffer->memory; //the allocated memory
			buffer->allocationInfo; //the info about the allocation
			buffer->mappedData = NULL; //the mapped memory
		}

	//creates a buffer and allocates memory
	inline int SMGraphics_Util_Buffer_Create(SMGraphics_Util_Buffer* buffer, SMGraphics_Util_Buffer_CreateInfo* info, VmaAllocator allocator)
	{
		if (vmaCreateBuffer(allocator, &info->bufferInfo, &info->allocInfo, &buffer->buffer, &buffer->memory,&buffer->allocationInfo) != VK_SUCCESS)
		{
			BTD_LogError("Smok Graphics", "Buffer", "SMGraphics_Util_Buffer_Create", "Failed to create a buffer!");
			return 0;
		}

		buffer->size = info->bufferInfo.size;

		return 1;
	}

	//destroys a buffer
	inline void SMGraphics_Util_Buffer_Destroy(SMGraphics_Util_Buffer* buffer, VmaAllocator allocator)
	{
		vmaDestroyBuffer(allocator, buffer->buffer, buffer->memory);
	}

	//creates a staging buffer
	inline bool SMGraphics_Util_Buffer_CreateStagingBuffer(SMGraphics_Util_Buffer* buffer, const size_t memBufferSize, VmaAllocator allocator)
	{
		SMGraphics_Util_Buffer_CreateInfo info;
		SMGraphics_Util_Buffer_CreateInfo_InitalizeDefaultValues(&info);
		info.bufferInfo.size = memBufferSize;
		info.bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		info.allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
		info.allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
			VMA_ALLOCATION_CREATE_MAPPED_BIT;

		SMGraphics_Util_Buffer_InitalizeDefaultValues(buffer);
		if(!SMGraphics_Util_Buffer_Create(buffer, &info, allocator))
		{
			BTD_LogError("Smok Graphics", "Buffer", "SMGraphics_Util_Buffer_CreateStagingBuffer", "Failed to create a staging buffer!");
			return 0;
		}

		return 1;
	}

	//copies data from one buffer to another source
	inline void SMGraphics_Util_Buffer_CopyBufferToBuffer(SMGraphics_Util_Buffer* targetBuffer, SMGraphics_Util_Buffer* sourceBuffer,
		VkCommandBuffer& comBuffer)
	{
		VkBufferCopy copyRegion{};
		copyRegion.size = targetBuffer->size;
		vkCmdCopyBuffer(comBuffer, sourceBuffer->buffer, targetBuffer->buffer, 1, &copyRegion);
	}

#if defined(__cplusplus)
	}
#endif