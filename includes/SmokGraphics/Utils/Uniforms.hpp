#pragma once

//defines uniform types

#include <BTDSTD/Reflection/Datatypes.hpp>

#include <vector>

#include <vulkan/vulkan.h>

namespace Smok::Graphics::Util::Uniform
{
	//defines a uniform buffer
	struct UniformBuffer
	{
		//uniform data	
		uint32_t binding = 0;
		VkShaderStageFlags shaderAccessStages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		size_t structMemSize = 0;
		const char* name = "";
	};

	//defines a storage buffer
	struct UniformStorageBuffer
	{
		//uniform data	
		uint32_t binding = 0;
		VkShaderStageFlags shaderAccessStages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		size_t structMemSize = 0;
		const char* name = "";
	};

	//defines a sampler 2D

	//defines a sampler 2D Array
	struct Sampler2DArray
	{
		VkImageView blankView = VK_NULL_HANDLE; VkSampler blankSampler = VK_NULL_HANDLE;
		uint32_t binding = 0;
		VkShaderStageFlags shaderAccessStages = VK_SHADER_STAGE_FRAGMENT_BIT;
		uint32_t arrayLength = 1;
		const char* name = "";
	};

	//defines a int constant
	struct IntConstant
	{
		int32_t value = 0;
		const char* name = "";
	};

	//defines a pack of uniforms
	struct UniformPack
	{
		std::vector<UniformBuffer> uniformBuffers;
		std::vector<UniformStorageBuffer> uniformStorageBuffers;
		std::vector<Sampler2DArray> sampler2DArrays;

		//clears the data
		inline void Clear() { uniformBuffers.clear(); uniformStorageBuffers.clear(); sampler2DArrays.clear(); }
	};

	//converts to the vulkan type
	inline VkFormat CovertToNativeType(BTD::Reflection::Datatype type)
	{
		switch (type)
		{
		case BTD::Reflection::Datatype::Float:
			return VK_FORMAT_R32_SFLOAT;
		case BTD::Reflection::Datatype::FVec2:
			return VK_FORMAT_R32G32_SFLOAT;
		case BTD::Reflection::Datatype::FVec3:
			return VK_FORMAT_R32G32B32_SFLOAT;
		case BTD::Reflection::Datatype::FVec4:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		default:
			VK_FORMAT_UNDEFINED;
		}
	}

	//vertex entry
	struct VertexEntry
	{
		uint32_t shaderLocation = 0; //the shader location
		BTD::Reflection::Datatype datatype; //the data type
		size_t memOffset = 0; //the memory offset in the struct for the layout
		const char* name = ""; //the name
	};

	//vertex layout
	struct VertexLayout
	{
		uint32_t descriptorSet = 0; //the set that owns this layout
		size_t vertexLayoutMemSize = 0; //the memory size of the whole layout struct

		std::vector<VertexEntry> vertexEntries;

		//genarates the vertex binding description
		VkVertexInputBindingDescription GenBindDesc()
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = descriptorSet;
			bindingDescription.stride = vertexLayoutMemSize;
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		//genarates the vertex attribute descriptions
		std::vector<VkVertexInputAttributeDescription> GenAttDesc()
		{
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
			const uint32_t count = vertexEntries.size();
			attributeDescriptions.resize(count);

			for (uint32_t i = 0; i < count; ++i)
			{
				attributeDescriptions[i].binding = 0;
				attributeDescriptions[i].location = vertexEntries[i].shaderLocation;
				attributeDescriptions[i].format = CovertToNativeType(vertexEntries[i].datatype);
				attributeDescriptions[i].offset = vertexEntries[i].memOffset;
			}

			return attributeDescriptions;
		}
	};
}