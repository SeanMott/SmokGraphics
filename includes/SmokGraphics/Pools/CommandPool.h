#pragma once

//defines the C API for Command Pool

#include <BTDSTD/Types.hpp>
#include <BTDSTD_C/Logger.h>

#include <vulkan/vulkan.h>

//command buffer
typedef struct SMGraphics_Pool_CommandBuffer
{
	bool isRecording;
	VkCommandBuffer comBuffer;
}SMGraphics_Pool_CommandBuffer;

#if defined(__cplusplus)
extern "C" {
#endif

	//starts recording a command buffer
	inline bool SMGraphics_Pool_CommandBuffer_StartRecording(SMGraphics_Pool_CommandBuffer* comBuffer, const bool renderpassContinue, const bool isSimultaneous, const bool singleUse)
	{
		if (comBuffer->isRecording)
		{
			BTD_LogWarning("Smok Graphics", "Command Buffer", "StartRecording", "Command Buffer is already recording, make sure it stops recording using \"StopRecording\" before you start again.");
			return false;
		}

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (singleUse)
			beginInfo.flags != VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		if (isSimultaneous)
			beginInfo.flags != VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		if (renderpassContinue)
			beginInfo.flags != VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

		vkBeginCommandBuffer(comBuffer->comBuffer, &beginInfo);
		comBuffer->isRecording = true;

		return true;
	}

	//ends a recording a command buffer
	inline bool SMGraphics_Pool_CommandBuffer_StopRecording(SMGraphics_Pool_CommandBuffer* comBuffer)
	{
		if (!comBuffer->isRecording)
		{
			BTD_LogWarning("Smok Graphics", "Command Buffer", "StopRecording", "Command Buffer is not recording, make sure one was started with \"StartRecording\" first.");
			return false;
		}

		vkEndCommandBuffer(comBuffer->comBuffer);
		comBuffer->isRecording = false;

		return true;
	}

#if defined(__cplusplus)
}
#endif

//command pool
typedef struct SMGraphics_Pool_CommandPool
{
	VkCommandPool commandPool;
}SMGraphics_Pool_CommandPool;

#if defined(__cplusplus)
extern "C" {
#endif

	//creates a command pool
	inline bool SMGraphics_Pool_CommandPool_CreateCommandPool(SMGraphics_Pool_CommandPool* commandPool, VkDevice& device, const uint32 graphicsQueueIndex)
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = graphicsQueueIndex;
		if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool->commandPool) != VK_SUCCESS)
		{
			BTD_LogWarning("Smok Graphics", "Command Pool", "Create", "Failed to create a command pool!");
			return false;
		}

		return true;
	}

	//destroys a command pool
	inline void SMGraphics_Pool_CommandPool_DestroyCommandPool(SMGraphics_Pool_CommandPool* commandPool, VkDevice& device)
	{
		vkDestroyCommandPool(device, commandPool->commandPool, nullptr);
	}

	//allocates a command buffer
	inline bool SMGraphics_Pool_CommandPool_CreateCommandBuffer(SMGraphics_Pool_CommandBuffer* comBuffer, SMGraphics_Pool_CommandPool* commandPool,
		VkDevice& device, bool isPrimary)
	{
		VkCommandBufferAllocateInfo bInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		bInfo.commandPool = commandPool->commandPool;
		bInfo.level = (isPrimary == true ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY);
		bInfo.commandBufferCount = 1;
		bInfo.pNext = nullptr;

		if (vkAllocateCommandBuffers(device, &bInfo, &comBuffer->comBuffer) != VK_SUCCESS)
		{
			BTD_LogWarning("Smok Graphics", "Command Pool", "CreateCommandBuffer", "Failed to create a command buffer!");
			return false;
		}
		comBuffer->isRecording = false; //initalize to false

		return true;
	}

	//deallocates a command buffer
	inline void SMGraphics_Pool_CommandPool_DestroyCommandBuffer(SMGraphics_Pool_CommandBuffer* comBuffer, SMGraphics_Pool_CommandPool* commandPool,
		VkDevice& device)
	{
		vkFreeCommandBuffers(device, commandPool->commandPool, 1, &comBuffer->comBuffer);
		comBuffer->comBuffer = VK_NULL_HANDLE; comBuffer->isRecording = false;
	}

	//allocates and starts recording a single use command buffer
	inline bool SMGraphics_Pool_CommandPool_StartSingleUseCommandBuffer(SMGraphics_Pool_CommandPool* commandPool,
		SMGraphics_Pool_CommandBuffer* comBuffer, VkDevice& device)
	{
		if (!SMGraphics_Pool_CommandPool_CreateCommandBuffer(comBuffer, commandPool, device, true))
		{
			BTD_LogWarning("Smok Graphics", "Command Pool", "StartSingleUseCommandBuffer", "Failed to create a single use command buffer!");
			return false;
		}

		SMGraphics_Pool_CommandBuffer_StartRecording(comBuffer, false, false, true);
		return true;
	}

	//stops recording, submits a single use command buffer and deallocates it
	inline bool SMGraphics_Pool_CommandPool_EndSingleUseCommandBuffer(SMGraphics_Pool_CommandPool* commandPool, SMGraphics_Pool_CommandBuffer* comBuffer,
		VkDevice& device, VkQueue& graphicsQueue)
	{
		if (!SMGraphics_Pool_CommandBuffer_StopRecording(comBuffer)) //stops recording
		{
			BTD_LogWarning("Smok Graphics", "Command Pool", "EndSingleUseCommandBuffer", "Failed to stop recording the command buffer before submiting it!");
			return false;
		}

		//submits
		VkSubmitInfo sInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
		sInfo.commandBufferCount = 1;
		sInfo.pCommandBuffers = &comBuffer->comBuffer;

		if (vkQueueSubmit(graphicsQueue, 1, &sInfo, nullptr) != VK_SUCCESS)
		{
			BTD_LogWarning("Smok Graphics", "Command Pool", "EndSingleUseCommandBuffer", "Failed to submit a single use command buffer!");
			return false;
		}

		vkQueueWaitIdle(graphicsQueue); //waits to finish

		//destroy the buffer
		SMGraphics_Pool_CommandPool_DestroyCommandBuffer(comBuffer, commandPool, device);

		return true;
	}

#if defined(__cplusplus)
}
#endif