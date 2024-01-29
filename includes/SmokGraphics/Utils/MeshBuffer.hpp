#pragma once

//defines mesh buffers for vertex and index buffers

#include <BTDSTD_C/Types.h>
#include <BTDSTD_C/Logger.h>

#include <SmokGraphics/Core/GPU.h>
#include <SmokGraphics/Utils/Buffer.h>
#include <SmokGraphics/Pools/CommandPool.h>

//namespace Smok::Graphics::Util
//{
	//defines a create info for a vertex buffer
	typedef struct SMGraphics_Util_VertexBuffer_CreateInfo
	{
		size_t vertexBufferElementMemSize; //the size of the element in the vertex array
		size_t verticesCount; //the number of vertices in the array
		void* vertices; //the vertex data

	} SMGraphics_Util_VertexBuffer_CreateInfo;

	//defines a vertex buffer
	typedef struct SMGraphics_Util_VertexBuffer
	{
		SMGraphics_Util_Buffer buffer; //the internal buffer
		uint32 vertexCount; //the number of vertices
		void* mappedData; //the buffer's mapped data

		SMGraphics_Util_VertexBuffer()
		{
			vertexCount = 0; //the number of vertices
			mappedData = NULL; //the buffer's mapped data
		}

	} SMGraphics_Util_VertexBuffer;

#if defined(__cplusplus)
	extern "C" {
#endif
		//initalizes vertex buffer create info to default values
		inline void SMGraphics_Util_VertexBuffer_CreateInfo_InitalizeDefaultValues(SMGraphics_Util_VertexBuffer_CreateInfo* info)
		{
			info->vertexBufferElementMemSize = 0; //the size of the element in the vertex array
			info->verticesCount = 0; //the number of vertices in the array
			info->vertices = NULL; //the vertex data
		}

		//initalizes vertex buffer to default values
		inline void SMGraphics_Util_VertexBuffer_InitalizeDefaultValues(SMGraphics_Util_VertexBuffer* vertexBuffer)
		{
			SMGraphics_Util_Buffer_InitalizeDefaultValues(&vertexBuffer->buffer); //the internal buffer
			vertexBuffer->vertexCount = 0; //the number of vertices
			vertexBuffer->mappedData = NULL; //the buffer's mapped data
		}

		//creates a vertex buffer
		inline int SMGraphics_Util_VertexBuffer_Create(SMGraphics_Util_VertexBuffer* vertexBuffer, const SMGraphics_Util_VertexBuffer_CreateInfo* info,
			VmaAllocator allocator, SMGraphics_Pool_CommandPool* commandPool, SMGraphics_Core_GPU* GPU)
		{
			//creates staging buffer
			const size_t bufferMemSize = info->verticesCount * info->vertexBufferElementMemSize;
			SMGraphics_Util_Buffer stagingBuffer;
			SMGraphics_Util_Buffer_InitalizeDefaultValues(&stagingBuffer);
			if (!SMGraphics_Util_Buffer_CreateStagingBuffer(&stagingBuffer, bufferMemSize, allocator))
			{
				BTD_LogError("Smok Graphics", "Mesh Buffer", "SMGraphics_Util_VertexBuffer_Create", "Failed to create a vertex buffer staging buffer!");
				return 0;
			}

			//copies vertices into staging buffer
			vmaMapMemory(allocator, stagingBuffer.memory, &stagingBuffer.mappedData); //maps buffer

			memcpy(stagingBuffer.mappedData, info->vertices, bufferMemSize); //copies data

			vmaUnmapMemory(allocator, stagingBuffer.memory); //unmaps buffer

			//creates vertex buffer
			SMGraphics_Util_Buffer_CreateInfo vertexBufferCreateInfo;
			SMGraphics_Util_Buffer_CreateInfo_InitalizeDefaultValues(&vertexBufferCreateInfo);
			vertexBufferCreateInfo.bufferInfo.size = bufferMemSize;
			vertexBufferCreateInfo.bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			vertexBufferCreateInfo.allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

			//initalize vertex buffer
			SMGraphics_Util_VertexBuffer_InitalizeDefaultValues(vertexBuffer);
			vertexBuffer->vertexCount = 0;
			
			//creates
			if (!SMGraphics_Util_Buffer_Create(&vertexBuffer->buffer, &vertexBufferCreateInfo, allocator))
			{
				SMGraphics_Util_Buffer_Destroy(&stagingBuffer, allocator);
				BTD_LogError("Smok Graphics", "Mesh Buffer", "SMGraphics_Util_VertexBuffer_Create", "Failed to create a vertex buffer!");
				return 0;
			}

			//copies data to vertex buffer
			SMGraphics_Pool_CommandBuffer comBuffer;
			SMGraphics_Pool_CommandPool_StartSingleUseCommandBuffer(commandPool, &comBuffer, GPU->device);
			SMGraphics_Util_Buffer_CopyBufferToBuffer(&vertexBuffer->buffer, &stagingBuffer, comBuffer.comBuffer);
			SMGraphics_Pool_CommandPool_EndSingleUseCommandBuffer(commandPool, &comBuffer, GPU->device, GPU->graphicsQueue);

			//clean up staging buffer
			SMGraphics_Util_Buffer_Destroy(&stagingBuffer, allocator);

			vertexBuffer->vertexCount = info->verticesCount;
			return 1;
		}

		//destroys a vertex buffer
		inline void SMGraphics_Util_VertexBuffer_Destroy(SMGraphics_Util_VertexBuffer* vertexBuffer, VmaAllocator allocator)
		{
			if (!vertexBuffer || !vertexBuffer->vertexCount)
				return;

			vertexBuffer->vertexCount = 0;
			SMGraphics_Util_Buffer_Destroy(&vertexBuffer->buffer, allocator);
		}

		//binds a vertex buffer
		inline void SMGraphics_Util_VertexBuffer_Bind(VkCommandBuffer comBuffer, SMGraphics_Util_VertexBuffer* vertexBuffer, const VkDeviceSize offset)
		{
			vkCmdBindVertexBuffers(comBuffer, 0, 1, &vertexBuffer->buffer.buffer, &offset);
		}

		//draws a vertex buffer
		inline void SMGraphics_Util_VertexBuffer_Draw(VkCommandBuffer comBuffer, SMGraphics_Util_VertexBuffer* vertexBuffer,
			const uint32 instanceCount, const uint32 instanceStart, const uint32 vertexStartIndex)
		{
			vkCmdDraw(comBuffer, vertexBuffer->vertexCount, instanceCount, vertexStartIndex, instanceStart);
		}

#if defined(__cplusplus)
	}
#endif

	//defines a create info for a index buffer
	typedef struct SMGraphics_Util_IndexBuffer_CreateInfo
	{
		size_t indicesCount; //the number of indices in the array
		uint32* indices; //the index data

		SMGraphics_Util_IndexBuffer_CreateInfo()
		{
			indicesCount = 0; //the number of indices in the array
			indices = nullptr; //the index data
		}
	} SMGraphics_Util_IndexBuffer_CreateInfo;

	////defines a create info for a index buffer
	//struct IndexBufferCreateInfo
	//{
	//	size_t indicesCount = 0; //the number of indices in the array
	//	uint32* indices = nullptr; //the index data
	//};

	//defines a index buffer
	typedef struct SMGraphics_Util_IndexBuffer
	{
		SMGraphics_Util_Buffer buffer; //the internal buffer
		size_t indexCount = 0; //the number of indices
		void* mappedData = nullptr; //the buffer's mapped data

		SMGraphics_Util_IndexBuffer()
		{
			SMGraphics_Util_Buffer_InitalizeDefaultValues(&buffer); //the internal buffer
			indexCount = 0; //the number of indices
			mappedData = nullptr; //the buffer's mapped data
		}

	} SMGraphics_Util_IndexBuffer;

#if defined(__cplusplus)
	extern "C" {
#endif

		//creates a index buffer
		inline int SMGraphics_Util_IndexBuffer_Create(SMGraphics_Util_IndexBuffer* indexBuffer, const SMGraphics_Util_IndexBuffer_CreateInfo* info,
			VmaAllocator allocator, SMGraphics_Pool_CommandPool* commandPool, SMGraphics_Core_GPU* GPU)
		{
			//creates staging buffer
			const size_t bufferMemSize = info->indicesCount * sizeof(uint32);
			SMGraphics_Util_Buffer stagingBuffer;
			if (!SMGraphics_Util_Buffer_CreateStagingBuffer(&stagingBuffer, bufferMemSize, allocator))
			{
				BTD_LogError("Smok Graphics", "Mesh Buffer", "SMGraphics_Util_IndexBuffer_Create", "Failed to create a index buffer staging buffer!");
				return 0;
			}

			//copies indices into staging buffer
			vmaMapMemory(allocator, stagingBuffer.memory, &stagingBuffer.mappedData); //maps buffer

			memcpy(stagingBuffer.mappedData, info->indices, bufferMemSize); //copies data

			vmaUnmapMemory(allocator, stagingBuffer.memory); //unmaps buffer

			//creates index buffer
			SMGraphics_Util_Buffer_CreateInfo indexBufferCreateInfo;
			SMGraphics_Util_Buffer_CreateInfo_InitalizeDefaultValues(&indexBufferCreateInfo);
			indexBufferCreateInfo.bufferInfo.size = bufferMemSize;
			indexBufferCreateInfo.bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

			indexBufferCreateInfo.allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

			if (!SMGraphics_Util_Buffer_Create(&indexBuffer->buffer, &indexBufferCreateInfo, allocator))
			{
				SMGraphics_Util_Buffer_Destroy(&stagingBuffer, allocator);
				BTD_LogError("Smok Graphics", "Mesh Buffer", "SMGraphics_Util_IndexBuffer_Create", "Failed to create a index buffer!");
				return 0;
			}

			//copies data to index buffer
			SMGraphics_Pool_CommandBuffer comBuffer;
			SMGraphics_Pool_CommandPool_StartSingleUseCommandBuffer(commandPool, &comBuffer, GPU->device);
			SMGraphics_Util_Buffer_CopyBufferToBuffer(&indexBuffer->buffer, &stagingBuffer, comBuffer.comBuffer);
			SMGraphics_Pool_CommandPool_EndSingleUseCommandBuffer(commandPool, &comBuffer, GPU->device, GPU->graphicsQueue);

			//clean up staging buffer
			SMGraphics_Util_Buffer_Destroy(&stagingBuffer, allocator);

			indexBuffer->indexCount = info->indicesCount;
			return 1;
		}

		//destroys a index buffer
		inline void SMGraphics_Util_IndexBuffer_Destroy(SMGraphics_Util_IndexBuffer* indexBuffer, VmaAllocator allocator)
		{
			if (!indexBuffer || !indexBuffer->indexCount)
				return;

			indexBuffer->indexCount = 0;
			SMGraphics_Util_Buffer_Destroy(&indexBuffer->buffer, allocator);
		}

		//binds a index buffer
		inline void SMGraphics_Util_IndexBuffer_Bind(VkCommandBuffer comBuffer, SMGraphics_Util_IndexBuffer* indexBuffer, const VkDeviceSize offset)
		{
			vkCmdBindIndexBuffer(comBuffer, indexBuffer->buffer.buffer, offset, VK_INDEX_TYPE_UINT32);
		}

		//draws a index buffer
		inline void SMGraphics_Util_IndexBuffer_Draw(VkCommandBuffer comBuffer, SMGraphics_Util_IndexBuffer* indexBuffer,
			const uint32 indiceStartIndex, const uint32 indexCount,
			const uint32 instanceCount, const uint32 instanceStart)
		{
			vkCmdDrawIndexed(comBuffer, indexCount, instanceCount, indiceStartIndex, 0, instanceStart);
		}
#if defined(__cplusplus)
	}
#endif