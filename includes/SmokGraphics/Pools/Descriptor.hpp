#pragma once

//defines a descriptor and it's data

#include <SmokGraphics/Core/GPU.h>

#include <SmokGraphics/Utils/Buffer.h>
#include <SmokGraphics/Utils/Uniforms.hpp>
#include <SmokGraphics/Pools/CommandPool.h>

#include <unordered_map>

namespace Smok::Graphics::Descriptor
{
	//defines a descriptor set pool create info
	struct DescriptorSetPoolCreateInfo
	{
		uint32 uniformBufferPoolCount = 0, //the number of indivial uniform buffers created
			uniformStorageBufferPoolCount = 0, //the number of indivial uniform storage buffers created
			uniformSampler2DArrayPoolCount = 0, //the number of indivial uniform sampler 2D Arrays created

			maxSetCount = 0; //the max number of sets possible in this pool
	};

	//defines a descripor set pool
	struct DescriptorPool
	{
		VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

		//the max amount of uniforms for this pool and the current amount allocated
		uint32 uniformBufferMaxCount = 1, uniformBufferCurrentCount = 0,
			uniformStorageBufferMaxCount = 1, uniformStorageBufferCurrentCount = 0,
			uniformSampler2DArrayMaxCount = 1, uniformSampler2DArrayCurrentCount = 0,

			//the max and current amount of sets allocated with this pool
			setMaxCount = 1, setCurrentCount = 0;
	};

	//creates a descriptor pool
	inline bool DescriptorPool_Create(DescriptorPool* pool, const DescriptorSetPoolCreateInfo& info,
		SMGraphics_Core_GPU* GPU)
	{
		//what kinds of uniforms are we going to allocate and how many
		std::vector<VkDescriptorPoolSize> poolSize = {
			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, info.uniformBufferPoolCount},
			{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, info.uniformStorageBufferPoolCount},
			{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, info.uniformStorageBufferPoolCount}
		};

		//defines create info for descriptor pool
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = poolSize.size();
		poolInfo.pPoolSizes = poolSize.data();
		poolInfo.maxSets = info.maxSetCount; //the max number of sets we can allocate
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;

		//creates the pool
		if (vkCreateDescriptorPool(GPU->device, &poolInfo, nullptr, &pool->descriptorPool) != VK_SUCCESS)
		{
			BTD::Logger::LogError("Smok Graphics", "Descriptor Pool", "DescriptorPool_Create", "Failed to create a descriptor pool!");
			return false;
		}

		pool->setCurrentCount = 0; pool->uniformBufferCurrentCount = 0; pool->uniformStorageBufferCurrentCount = 0;

		pool->setMaxCount = info.maxSetCount; pool->uniformBufferMaxCount = info.uniformBufferPoolCount;
		pool->uniformStorageBufferMaxCount = info.uniformStorageBufferPoolCount;
		pool->uniformSampler2DArrayMaxCount = info.uniformSampler2DArrayPoolCount;

		return true;
	}

	//destroys a descriptor pool
	inline void DescriptorPool_Destroy(DescriptorPool* pool, SMGraphics_Core_GPU* GPU)
	{
		vkDestroyDescriptorPool(GPU->device, pool->descriptorPool, nullptr);
	}

	//defines a descripor set layout create info
	struct DescriptorSetLayoutCreateInfo
	{
		Util::Uniform::UniformPack uniforms;
	};

	//defines a descripor set layout
	struct DescriptorSetLayout
	{
		VkDescriptorSetLayout descriptorSetLayout;
		Util::Uniform::UniformPack uniforms;
	};

	//creates a descriptor set layout
	inline bool DescriptorSetLayout_Create(DescriptorSetLayout* layout, const DescriptorSetLayoutCreateInfo& info,
		SMGraphics_Core_GPU* GPU)
	{
		const size_t uniformBufferCount = info.uniforms.uniformBuffers.size(),
			uniformStorageBufferCount = info.uniforms.uniformStorageBuffers.size(),
			uniformSampler2DArrayCount = info.uniforms.sampler2DArrays.size();
		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.reserve(uniformBufferCount + uniformStorageBufferCount + uniformSampler2DArrayCount);

		//goes through the uniforms
		for (uint32 i = 0; i < uniformBufferCount; ++i)
		{
			VkDescriptorSetLayoutBinding* binding = &bindings.emplace_back(VkDescriptorSetLayoutBinding());
			binding->binding = info.uniforms.uniformBuffers[i].binding;
			binding->descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			binding->descriptorCount = 1;
			binding->stageFlags = info.uniforms.uniformBuffers[i].shaderAccessStages;
			binding->pImmutableSamplers = nullptr;
		}

		for (uint32 i = 0; i < uniformStorageBufferCount; ++i)
		{
			VkDescriptorSetLayoutBinding* binding = &bindings.emplace_back(VkDescriptorSetLayoutBinding());
			binding->binding = info.uniforms.uniformStorageBuffers[i].binding;
			binding->descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			binding->descriptorCount = 1;
			binding->stageFlags = info.uniforms.uniformStorageBuffers[i].shaderAccessStages;
			binding->pImmutableSamplers = nullptr;
		}

		for (uint32 i = 0; i < uniformSampler2DArrayCount; ++i)
		{
			VkDescriptorSetLayoutBinding* binding = &bindings.emplace_back(VkDescriptorSetLayoutBinding());
			binding->binding = info.uniforms.sampler2DArrays[i].binding;
			binding->descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			binding->descriptorCount = 1;
			binding->stageFlags = info.uniforms.sampler2DArrays[i].shaderAccessStages;
			binding->pImmutableSamplers = &info.uniforms.sampler2DArrays[i].blankSampler;
		}

		//creates the layout
		VkDescriptorSetLayoutCreateInfo descLayoutInfo{};
		descLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descLayoutInfo.bindingCount = bindings.size();
		descLayoutInfo.pBindings = bindings.data();
		descLayoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT_EXT;

		if (vkCreateDescriptorSetLayout(GPU->device, &descLayoutInfo, nullptr, &layout->descriptorSetLayout) != VK_SUCCESS)
		{
			BTD::Logger::LogError("Smok Graphics", "Descriptor Set Layout", "DescriptorSetLayout_Create", "Failed to create descriptor set layout!");
			return false;
		}

		layout->uniforms = info.uniforms;

		return true;
	}

	//destroys a descriptor set layout
	inline void DescriptorSetLayout_Destroy(DescriptorSetLayout* layout, SMGraphics_Core_GPU* GPU)
	{
		vkDestroyDescriptorSetLayout(GPU->device, layout->descriptorSetLayout, nullptr);
	}

	//defines a uniform buffer stored in the descriptor set
	struct DescriptorSet_UniformBuffer
	{
		std::vector<SMGraphics_Util_Buffer> buffers; //the buffers
		std::vector<bool> isSafeCopy; //is the buffer safe to directly copy into or transfer
	};

	//defines a uniform buffer stored in the descriptor set
	struct DescriptorSet_UniformStorageBuffer
	{
		std::vector<SMGraphics_Util_Buffer> buffers; //the buffers
		std::vector<bool> isSafeCopy; //is the buffer safe to directly copy into or transfer
	};

	//defines a uniform sampler 2D array stored in the descriptor set
	struct DescriptorSet_UniformSampler2DArray
	{
		std::vector<std::vector<VkDescriptorImageInfo>> imageData; //stores the image data
	};

	//defines a descriptor set
	struct DescriptorSet
	{
		DescriptorSetLayout* layout = nullptr;
		std::vector<VkDescriptorSet> descriptorSets;

		//the uniform buffers
		std::unordered_map<const const char*, DescriptorSet_UniformBuffer> uniformBuffers;
		std::unordered_map<const const char*, DescriptorSet_UniformStorageBuffer> uniformStorageBuffers;
		std::unordered_map<const const char*, DescriptorSet_UniformSampler2DArray> uniformSampler2DArrays;
	};

	//recreates a uniform buffer's buffer
	inline bool DescriptorSet_UniformBuffer_RecreateBuffer(VkDescriptorSet& set,
		SMGraphics_Util_Buffer* buffer, bool& isSafeCopy,
		const size_t& bufferMemSize, 
		VkWriteDescriptorSet* descriptorWrite, VkDescriptorBufferInfo* bufferInfo,
		VmaAllocator& allocator)
	{
		//if buffer is not NULL, we destroy the old one
		if (buffer->buffer != VK_NULL_HANDLE)
			SMGraphics_Util_Buffer_Destroy(buffer, allocator);

		//defines a create info for the buffers
		SMGraphics_Util_Buffer_CreateInfo uniformBufferCreateInfo;
		SMGraphics_Util_Buffer_CreateInfo_InitalizeDefaultValues(&uniformBufferCreateInfo);
		uniformBufferCreateInfo.bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		uniformBufferCreateInfo.bufferInfo.size = bufferMemSize;

		uniformBufferCreateInfo.allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
		uniformBufferCreateInfo.allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
			VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
			VMA_ALLOCATION_CREATE_MAPPED_BIT;

		//allocates a buffer
		SMGraphics_Util_Buffer_Create(buffer, &uniformBufferCreateInfo, allocator);

		//checks and stores if the memory of the uniform buffer is in a mapable space, so we can just copy
		//if not we can do a transfer
		VkMemoryPropertyFlags memPropFlags;
		vmaGetAllocationMemoryProperties(allocator, buffer->memory, &memPropFlags);

		if (memPropFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			isSafeCopy = true; //memset(isSafeCopy, 1, sizeof(bool));
		else
			isSafeCopy = false; //memset(isSafeCopy, 0, sizeof(bool));

		//defines the buffer info and binds it with the descriptor write info
		bufferInfo->buffer = buffer->buffer;
		bufferInfo->offset = 0;
		bufferInfo->range = bufferMemSize;

		descriptorWrite->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite->dstSet = set;
		descriptorWrite->dstBinding = 0;
		descriptorWrite->dstArrayElement = 0;

		descriptorWrite->descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite->descriptorCount = 1;

		descriptorWrite->pBufferInfo = bufferInfo;
		descriptorWrite->pImageInfo = nullptr;
		descriptorWrite->pTexelBufferView = nullptr;

		return true;
	}

	//recreates a uniform storage buffer's buffer
	inline bool DescriptorSet_UniformStorageBuffer_RecreateBuffer(VkDescriptorSet& set,
		SMGraphics_Util_Buffer* buffer, bool& isSafeCopy,
		const size_t& bufferMemSize,
		VkWriteDescriptorSet* descriptorWrite, VkDescriptorBufferInfo* bufferInfo,
		VmaAllocator& allocator)
	{
		//if buffer is not NULL, we destroy the old one
		if (buffer->buffer != VK_NULL_HANDLE)
			SMGraphics_Util_Buffer_Destroy(buffer, allocator);

		//defines a create info for the buffers
		SMGraphics_Util_Buffer_CreateInfo uniformBufferCreateInfo;
		SMGraphics_Util_Buffer_CreateInfo_InitalizeDefaultValues(&uniformBufferCreateInfo);
		uniformBufferCreateInfo.bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		uniformBufferCreateInfo.bufferInfo.size = bufferMemSize;

		uniformBufferCreateInfo.allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
		uniformBufferCreateInfo.allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
			VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
			VMA_ALLOCATION_CREATE_MAPPED_BIT;

		//allocates a buffer
		SMGraphics_Util_Buffer_Create(buffer, &uniformBufferCreateInfo, allocator);

		//checks and stores if the memory of the uniform buffer is in a mapable space, so we can just copy
		//if not we can do a transfer
		VkMemoryPropertyFlags memPropFlags;
		vmaGetAllocationMemoryProperties(allocator, buffer->memory, &memPropFlags);

		if (memPropFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			isSafeCopy = true; //memset(isSafeCopy, 1, sizeof(bool));
		else
			isSafeCopy = false; //memset(isSafeCopy, 0, sizeof(bool));

		//defines the buffer info and binds it with the descriptor write info
		bufferInfo->buffer = buffer->buffer;
		bufferInfo->offset = 0;
		bufferInfo->range = bufferMemSize;

		descriptorWrite->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite->dstSet = set;
		descriptorWrite->dstBinding = 0;
		descriptorWrite->dstArrayElement = 0;

		descriptorWrite->descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorWrite->descriptorCount = 1;

		descriptorWrite->pBufferInfo = bufferInfo;
		descriptorWrite->pImageInfo = nullptr;
		descriptorWrite->pTexelBufferView = nullptr;

		return true;
	}

	//creates a descriptor set
	inline bool DescriptorSet_Create(DescriptorSet* set, DescriptorSetLayout* layout, DescriptorPool* pool,
		VmaAllocator& allocator, SMGraphics_Core_GPU* GPU, const uint32& framesInFlight)
	{
		//checks if there is enough space in the pool
		const size_t uniformBufferCount = layout->uniforms.uniformBuffers.size();
		const size_t uniformStorageBufferCount = layout->uniforms.uniformStorageBuffers.size();
		const size_t uniformSampler2DArrayCount = layout->uniforms.sampler2DArrays.size();
		/*if (pool->setCurrentCount >= pool->setMaxCount ||
			pool->uniformBufferCurrentCount + uniformBufferCount >= pool->uniformBufferMaxCount ||
			pool->uniformStorageBufferCurrentCount + uniformStorageBufferCount >= pool->uniformStorageBufferMaxCount ||
			pool->uniformSampler2DArrayCurrentCount + uniformSampler2DArrayCount >= pool->uniformSampler2DArrayMaxCount)
		{
			BTD::Logger::LogWarning("Smok Graphics", "Descriptor Set", "DescriptorSet_Create", "Descriptor Pool passed in does not have enough space to fit the descriptor set and it's desired uniforms!");
			return false;
		}*/

		std::vector<VkDescriptorSetLayout> descLayouts; descLayouts.resize(framesInFlight, layout->descriptorSetLayout);

		VkDescriptorSetAllocateInfo descSetAllocInfo{};
		descSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descSetAllocInfo.descriptorPool = pool->descriptorPool;
		descSetAllocInfo.descriptorSetCount = descLayouts.size();
		descSetAllocInfo.pSetLayouts = descLayouts.data();
		set->descriptorSets.resize(framesInFlight);
		if (vkAllocateDescriptorSets(GPU->device, &descSetAllocInfo, set->descriptorSets.data()) != VK_SUCCESS)
		{
			BTD::Logger::LogError("Smok Graphics", "Descriptor Set", "DescriptorSet_Create", "Failed to allocate descriptor sets!");
			return false;
		}

		set->layout = layout;

		//buffer infos and write info
		std::vector<VkDescriptorBufferInfo> descSetBufferInfos;
		descSetBufferInfos.reserve((uniformBufferCount * framesInFlight) + (uniformStorageBufferCount * framesInFlight));
		
		std::vector<VkWriteDescriptorSet> descSetWrite;
		descSetWrite.reserve((uniformBufferCount * framesInFlight) 
			+ (uniformStorageBufferCount * framesInFlight) +
		(uniformSampler2DArrayCount * framesInFlight));

		//goes through the uniform buffers
		for (uint32 i = 0; i < uniformBufferCount; ++i)
		{
			//preps the uniform buffer for having data entered
			set->uniformBuffers[layout->uniforms.uniformBuffers[i].name];
			DescriptorSet_UniformBuffer* UB = &set->uniformBuffers[layout->uniforms.uniformBuffers[i].name];
			Util::Uniform::UniformBuffer* UBuffer = &layout->uniforms.uniformBuffers[i];
			UB->buffers.resize(framesInFlight); UB->isSafeCopy.resize(framesInFlight);

			//goes through the buffers
			for (size_t f = 0; f < framesInFlight; f++)
			{
				bool state = false;
				DescriptorSet_UniformBuffer_RecreateBuffer(set->descriptorSets[f],
					&UB->buffers[f], state,
					UBuffer->structMemSize,
					&descSetWrite.emplace_back(VkWriteDescriptorSet()), &descSetBufferInfos.emplace_back(VkDescriptorBufferInfo()), 
					allocator);
					UB->isSafeCopy[f] = state; //it wouldn't let it get passed directly to the func, so we move from state
			}
		}

		//goes through the uniform storage buffers
		for (uint32 i = 0; i < uniformStorageBufferCount; ++i)
		{
			//preps the uniform buffer for having data entered
			set->uniformBuffers[layout->uniforms.uniformStorageBuffers[i].name];
			DescriptorSet_UniformStorageBuffer* UB = &set->uniformStorageBuffers[layout->uniforms.uniformStorageBuffers[i].name];
			Util::Uniform::UniformStorageBuffer* UBuffer = &layout->uniforms.uniformStorageBuffers[i];
			UB->buffers.resize(framesInFlight); UB->isSafeCopy.resize(framesInFlight);

			//goes through the buffers
			for (size_t f = 0; f < framesInFlight; f++)
			{

			bool state = false;
			DescriptorSet_UniformStorageBuffer_RecreateBuffer(set->descriptorSets[f],
				&UB->buffers[f], state,
				UBuffer->structMemSize,
				&descSetWrite.emplace_back(VkWriteDescriptorSet()), &descSetBufferInfos.emplace_back(VkDescriptorBufferInfo()),
				allocator);
			UB->isSafeCopy[f] = state; //it wouldn't let it get passed directly to the func, so we move from state
			}
		}

		//goes through the uniform sampler 2D arrays
		for (uint32 i = 0; i < uniformSampler2DArrayCount; ++i)
		{
			//preps the uniform buffer for having data entered
			set->uniformSampler2DArrays[layout->uniforms.sampler2DArrays[i].name];
			DescriptorSet_UniformSampler2DArray* UB = &set->uniformSampler2DArrays[layout->uniforms.sampler2DArrays[i].name];
			Util::Uniform::Sampler2DArray* UBuffer = &layout->uniforms.sampler2DArrays[i];
			UB->imageData.resize(framesInFlight);

			//goes through the buffers
			for (size_t f = 0; f < framesInFlight; f++)
			{
				UB->imageData[f].resize(UBuffer->arrayLength);
				for (uint32 t = 0; t < UBuffer->arrayLength; ++t) //goes through the individual texture infos
				{
					UB->imageData[f][t].sampler = UBuffer->blankSampler;
					UB->imageData[f][t].imageView = UBuffer->blankView;
					UB->imageData[f][t].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

					VkWriteDescriptorSet* descriptorWrite = &descSetWrite.emplace_back(VkWriteDescriptorSet());
					descriptorWrite->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrite->dstSet = set->descriptorSets[f];
					descriptorWrite->dstBinding = 0;
					descriptorWrite->dstArrayElement = 0;

					descriptorWrite->descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					descriptorWrite->descriptorCount = 1;

					descriptorWrite->pBufferInfo = nullptr;
					descriptorWrite->pImageInfo = &UB->imageData[f][t];
					descriptorWrite->pTexelBufferView = nullptr;
				}
			}
		}

		//updates the write commands for the descriptor sets
		vkUpdateDescriptorSets(GPU->device, descSetWrite.size(), descSetWrite.data(), 0, nullptr);

		return true;
	}

	//destroys a descriptor set
	inline void DescriptorSet_Destroy(DescriptorSet* set, DescriptorPool* pool, VmaAllocator& allocator, SMGraphics_Core_GPU* GPU)
	{
		//cleans up uniforms
		for (auto& UB : set->uniformBuffers)
		{
			for (uint32 i = 0; i < UB.second.buffers.size(); ++i)
				SMGraphics_Util_Buffer_Destroy(&UB.second.buffers[i], allocator);
		}
		set->uniformBuffers.clear();

		for (auto& UB : set->uniformStorageBuffers)
		{
			for (uint32 i = 0; i < UB.second.buffers.size(); ++i)
				SMGraphics_Util_Buffer_Destroy(&UB.second.buffers[i], allocator);
		}
		set->uniformStorageBuffers.clear();

		set->uniformSampler2DArrays.clear();

		set->layout = nullptr;
		set->descriptorSets.clear();
	}

	//uploads data to a uniform buffer, since a staging buffer IS NOT provided, it makes one
	//it also copies data to every buffer
	inline void DescriptorSet_UniformBuffer_UploadDataToGPU_AllBuffers(DescriptorSet* set,
		const char* uniformName,
		VmaAllocator& allocator, SMGraphics_Core_GPU* GPU, void* data,
		SMGraphics_Pool_CommandPool* commandPool)
	{
		//gets uniform buffer
		DescriptorSet_UniformBuffer* UB = &set->uniformBuffers[uniformName];

		//goes through the buffers		
		for (uint32 i = 0; i < UB->buffers.size(); ++i)
		{
			// Allocation ended up in a mappable memory and is already mapped - write to it directly.
			if (UB->isSafeCopy[i])
				memcpy(UB->buffers[i].allocationInfo.pMappedData, data, UB->buffers[i].size);

			// Allocation ended up in a non-mappable memory - need to transfer.
			else
			{
				SMGraphics_Util_Buffer stagingBuffer;
				SMGraphics_Util_Buffer_CreateStagingBuffer(&stagingBuffer, UB->buffers[i].size, allocator);
					
				//creates a single use command buffer
				SMGraphics_Pool_CommandBuffer comBuffer;
				SMGraphics_Pool_CommandPool_StartSingleUseCommandBuffer(commandPool, &comBuffer, GPU->device);

				//copy into buffer
				memcpy(UB->buffers[i].allocationInfo.pMappedData, data, UB->buffers[i].size);
				vmaFlushAllocation(allocator, stagingBuffer.memory, 0, VK_WHOLE_SIZE);
				//vkCmdPipelineBarrier: VK_ACCESS_HOST_WRITE_BIT --> VK_ACCESS_TRANSFER_READ_BIT
				VkBufferCopy bufCopy = {
					0, // srcOffset
					0, // dstOffset,
					UB->buffers[i].size }; // size
				vkCmdCopyBuffer(comBuffer.comBuffer, stagingBuffer.buffer, UB->buffers[i].buffer, 1, &bufCopy);

				//submits the single use command buffer
				SMGraphics_Pool_CommandPool_EndSingleUseCommandBuffer(commandPool, &comBuffer, GPU->device, GPU->graphicsQueue);
			
				SMGraphics_Util_Buffer_Destroy(&stagingBuffer, allocator);
			}
		}
	}

	//uploads data to a uniform storage buffer, since a staging buffer IS NOT provided, it makes one
	//it also copies data to every buffer
	inline void DescriptorSet_UniformStorageBuffer_UploadDataToGPU_AllBuffers(DescriptorSet* set,
		const char* uniformName,
		VmaAllocator& allocator, SMGraphics_Core_GPU* GPU, void* data,
		SMGraphics_Pool_CommandPool* commandPool)
	{
		//gets uniform buffer
		DescriptorSet_UniformStorageBuffer* UB = &set->uniformStorageBuffers[uniformName];

		//goes through the buffers		
		for (uint32 i = 0; i < UB->buffers.size(); ++i)
		{
			// Allocation ended up in a mappable memory and is already mapped - write to it directly.
			if (UB->isSafeCopy[i])
				memcpy(UB->buffers[i].allocationInfo.pMappedData, data, UB->buffers[i].size);

			// Allocation ended up in a non-mappable memory - need to transfer.
			else
			{
				SMGraphics_Util_Buffer stagingBuffer;
				SMGraphics_Util_Buffer_CreateStagingBuffer(&stagingBuffer, UB->buffers[i].size, allocator);

				//creates a single use command buffer
				SMGraphics_Pool_CommandBuffer comBuffer;
				SMGraphics_Pool_CommandPool_StartSingleUseCommandBuffer(commandPool, &comBuffer, GPU->device);

				//copy into buffer
				memcpy(UB->buffers[i].allocationInfo.pMappedData, data, UB->buffers[i].size);
				vmaFlushAllocation(allocator, stagingBuffer.memory, 0, VK_WHOLE_SIZE);
				//vkCmdPipelineBarrier: VK_ACCESS_HOST_WRITE_BIT --> VK_ACCESS_TRANSFER_READ_BIT
				VkBufferCopy bufCopy = {
					0, // srcOffset
					0, // dstOffset,
					UB->buffers[i].size }; // size
				vkCmdCopyBuffer(comBuffer.comBuffer, stagingBuffer.buffer, UB->buffers[i].buffer, 1, &bufCopy);

				//submits the single use command buffer
				SMGraphics_Pool_CommandPool_EndSingleUseCommandBuffer(commandPool, &comBuffer, GPU->device, GPU->graphicsQueue);
			
				SMGraphics_Util_Buffer_Destroy(&stagingBuffer, allocator);
			}
		}
	}

	//updates the texture data in the sampler array
	inline void DescriptorSet_UniformSampler2DArray_UploadDataToGPU_AllBuffers(DescriptorSet* set,
		const char* uniformName, SMGraphics_Core_GPU* GPU,
		VkImageView* views, VkSampler* samplers, const size_t& count)
	{
		//gets sampler array
		DescriptorSet_UniformSampler2DArray* SA = &set->uniformSampler2DArrays[uniformName];
		
		std::vector<VkWriteDescriptorSet> descSetWrite;
		
		for (uint32 f = 0; f < SA->imageData.size(); ++f)
		{
			SA->imageData[f].resize(count);

			for (uint32 i = 0; i < count; ++i)
			{
				SA->imageData[f][i].sampler = samplers[i];// blankSampler;
				SA->imageData[f][i].imageView = views[i];// blankTexture;
				SA->imageData[f][i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

				VkWriteDescriptorSet* descriptorWrite = &descSetWrite.emplace_back(VkWriteDescriptorSet());
				descriptorWrite->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite->dstSet = set->descriptorSets[f];
				descriptorWrite->dstBinding = 0;
				descriptorWrite->dstArrayElement = 0;

				descriptorWrite->descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				descriptorWrite->descriptorCount = 1;

				descriptorWrite->pBufferInfo = nullptr;
				descriptorWrite->pImageInfo = &SA->imageData[f][i];
				descriptorWrite->pTexelBufferView = nullptr;
			}
		}
		
		//updates the write commands for the descriptor sets
		vkUpdateDescriptorSets(GPU->device, descSetWrite.size(), descSetWrite.data(), 0, nullptr);
	}

	//updates the texture data in the sampler array to a specific index
	inline void DescriptorSet_UniformSampler2DArray_UploadDataToGPU_SpecificBuffer(DescriptorSet* set, const uint32 bufferIndex,
		const char* uniformName, SMGraphics_Core_GPU* GPU,
		VkImageView* views, VkSampler* samplers, const size_t& count)
	{
		//gets sampler array
		DescriptorSet_UniformSampler2DArray* SA = &set->uniformSampler2DArrays[uniformName];

		std::vector<VkWriteDescriptorSet> descSetWrite;

		SA->imageData[bufferIndex].resize(count);

		for (uint32 i = 0; i < count; ++i)
		{
			SA->imageData[bufferIndex][i].sampler = samplers[i];// blankSampler;
			SA->imageData[bufferIndex][i].imageView = views[i];// blankTexture;
			SA->imageData[bufferIndex][i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			VkWriteDescriptorSet* descriptorWrite = &descSetWrite.emplace_back(VkWriteDescriptorSet());
			descriptorWrite->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite->dstSet = set->descriptorSets[bufferIndex];
			descriptorWrite->dstBinding = 0;
			descriptorWrite->dstArrayElement = 0;

			descriptorWrite->descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite->descriptorCount = 1;

			descriptorWrite->pBufferInfo = nullptr;
			descriptorWrite->pImageInfo = &SA->imageData[bufferIndex][i];
			descriptorWrite->pTexelBufferView = nullptr;
		}

		//updates the write commands for the descriptor sets
		vkUpdateDescriptorSets(GPU->device, descSetWrite.size(), descSetWrite.data(), 0, nullptr);
	}
}