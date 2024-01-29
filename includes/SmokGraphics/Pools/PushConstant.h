#pragma once

//defines data for a push constant

#include <BTDSTD_C/Types.h>

#include <vulkan/vulkan.h>
#include <string.h>

	//defines a push constant create info
	typedef struct SMGraphics_Descriptor_PushConstant_CreateInfo
	{
		VkShaderStageFlags shaderStages; //the stages of the shader
		uint32 memStartOffset; //the start offset into a struct for when the push constant starts
		uint32 size; //the whole size of the struct
	} SMGraphics_Descriptor_PushConstant_CreateInfo;

#if defined(__cplusplus)
	extern "C" {
#endif
		//initalizes the default values for the push constant create info
		inline void SMGraphics_Descriptor_PushConstant_CreateInfo_InitalizeDefaultValues(SMGraphics_Descriptor_PushConstant_CreateInfo* info)
		{
			info->shaderStages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT; //the stages of the shader
			info->memStartOffset = 0; //the start offset into a struct for when the push constant starts
			info->size = 0; //the whole size of the struct
		}

#if defined(__cplusplus)
	}
#endif

	//defines a push constant
	typedef struct SMGraphics_Descriptor_PushConstant
	{
		VkShaderStageFlags shaderStages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT; //the stages of the shader
		uint32_t memStartOffset = 0; //the start offset into a struct for when the push constant starts
		uint32_t size = 0; //the whole size of the struct
	} SMGraphics_Descriptor_PushConstant;

#if defined(__cplusplus)
	extern "C" {
#endif
		//initalizes the default values for the push constant create info
		inline void SMGraphics_Descriptor_PushConstant_InitalizeDefaultValues(SMGraphics_Descriptor_PushConstant* pushConstant)
		{
			pushConstant->shaderStages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT; //the stages of the shader
			pushConstant->memStartOffset = 0; //the start offset into a struct for when the push constant starts
			pushConstant->size = 0; //the whole size of the struct
		}

	//creates a push constant from a create info
	inline void SMGraphics_Descriptor_PushConstant_Create(SMGraphics_Descriptor_PushConstant* pushConstant, const SMGraphics_Descriptor_PushConstant_CreateInfo& info)
	{
		//the struct is identical to the desc, so we can just copy it
		memcpy(pushConstant, &info, sizeof(SMGraphics_Descriptor_PushConstant));
	}

	//updates the push constant and "pushes" it to the GPU
	inline void SMGraphics_Descriptor_PushConstant_PushToGPU(VkCommandBuffer& comBuffer, VkPipelineLayout& pipelineLayout,
		SMGraphics_Descriptor_PushConstant* pushConstant, void* data)
	{
		//updates the data on the GPU
		vkCmdPushConstants(comBuffer, pipelineLayout, pushConstant->shaderStages,
			pushConstant->memStartOffset, pushConstant->size, data);
	}

#if defined(__cplusplus)
	}
#endif