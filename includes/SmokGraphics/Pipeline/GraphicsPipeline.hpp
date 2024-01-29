#pragma once

//defines a functions for graphics shaders, pipeline, and layout for Vulkan

#include <BTDSTD/IO/File.hpp>
#include <BTDSTD_C/Math/Vectors.h>

#include <SmokGraphics/Core/GPU.h>

#include <SmokGraphics/Utils/Uniforms.hpp>

#include <vulkan/vulkan.h>
#include <yaml-cpp/yaml.h>

namespace Smok::Graphics::Pipeline
{
	//defines a graphics shader
	struct GraphicsShader
	{
		uint64 assetID = 0; //the asset ID for this asset

		VkShaderModule vMod = VK_NULL_HANDLE, fMod = VK_NULL_HANDLE;
		VkDevice device = VK_NULL_HANDLE;

		Util::Uniform::UniformPack uniforms; std::vector<Util::Uniform::IntConstant> intConsts;
		
		std::string vertPath = "", fragPath = "", declPath = "";
	};

	//creates a graphics shader from a path
	inline bool GraphicsShader_Create(GraphicsShader* shader, SMGraphics_Core_GPU* _GPU,
		const char* vPath, const char* fPath)
	{
		shader->device = _GPU->device;

		//loads from file
		BTD::IO::File file; file.Open(vPath, BTD::IO::FileOP::BinaryRead_OpenExisting);
		if (!file.IsOpen())
		{
			printf("Failed to open the vertex shader at %s!\n", vPath);
			return false;
		}
		std::vector<char> data = file.ReadVec(); //reads file

		VkShaderModuleCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = data.size();
		info.pCode = reinterpret_cast<const uint32_t*>(data.data());

		//creates the module
		if (vkCreateShaderModule(shader->device, &info, nullptr, &shader->vMod) != VK_SUCCESS)
		{
			printf("Vulfix Error: Shader Mod Creation || Failed to create a Shader module, check if the code was brought in properly!\n");
			return false;
		}
		file.Close();

		file.Open(fPath, BTD::IO::FileOP::BinaryRead_OpenExisting);
		if (!file.IsOpen())
		{
			printf("Failed to open the fragment shader at %s!\n", fPath);
			return false;
		}
		data = file.ReadVec(); //reads file

		info = {};
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = data.size();
		info.pCode = reinterpret_cast<const uint32_t*>(data.data());

		//creates the module
		if (vkCreateShaderModule(shader->device, &info, nullptr, &shader->fMod) != VK_SUCCESS)
		{
			printf("Vulfix Error: Shader Mod Creation || Failed to create a Shader module, check if the code was brought in properly!\n");
			return false;
		}
		file.Close();

		//generates reflected uniforms and consts

		//stores paths
		shader->vertPath = std::string(vPath); shader->fragPath = std::string(fPath);

		return true;
	}

	//destroys a graphics shader
	inline void GraphicsShader_Destroy(GraphicsShader* shader)
	{
		if (shader->vMod == VK_NULL_HANDLE)
			return;

		vkDestroyShaderModule(shader->device, shader->fMod, nullptr);
		vkDestroyShaderModule(shader->device, shader->vMod, nullptr);

		shader->uniforms.Clear(); shader->intConsts.clear();
	}

	//defines the decl file extension for graphics shaders
#define SMOK_GRAPHICS_GRAPHICS_SHADER_DECL_FILE_EXTENSION ".shaderDecl"

	//writes a graphics shader decl file
	inline void GraphicsShader_WriteDeclFile(const std::string& dir, const std::string& assetName,
		const std::string& vertexPath, const std::string& fragmentPath)
	{
		YAML::Emitter emitter;

		emitter << YAML::BeginMap;

		emitter << YAML::Key << "assetName" << YAML::DoubleQuoted << assetName;

		emitter << YAML::Key << "vertexPath" << YAML::DoubleQuoted << vertexPath;
		emitter << YAML::Key << "fragmentPath" << YAML::DoubleQuoted << fragmentPath;

		emitter << YAML::EndMap;

		BTD::IO::File file; file.Open(dir + "/" + assetName + SMOK_GRAPHICS_GRAPHICS_SHADER_DECL_FILE_EXTENSION,
			BTD::IO::FileOP::TextWrite_OpenCreateStart);
		file.Write(emitter.c_str());
		file.Close();
	}

	//loads a graphics shader decl file
	inline void GraphicsShader_LoadDeclFile(const std::string& declPath, 
		std::string& assetName, std::string& vertexPath, std::string& fragmentPath)
	{
		BTD::IO::File file;
		if (!file.Open(declPath, BTD::IO::FileOP::TextRead_OpenExisting))
		{
			BTD_LogError("Smok Graphics", "Graphics Shader", "GraphicsShader_LoadDeclFile",
				std::string("failed to find decl data at \"" + declPath + "\"").c_str());
			return;
		}

		YAML::Node data = YAML::Load(file.Read());

		assetName = data["assetName"].as<std::string>();

		vertexPath = data["vertexPath"].as<std::string>();
		fragmentPath = data["fragmentPath"].as<std::string>();
	}

	//defines a create info for a graphics pipeline layout
	struct GraphicsPipelineLayoutCreateInfo
	{
		std::vector<VkDescriptorSetLayout> descriptorLayouts;
		std::vector<VkPushConstantRange> pushConstants;
	};

	//defines a graphics pipeline layout
	struct GraphicsPipelineLayout
	{
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkDevice device = VK_NULL_HANDLE;
	};

	//creates the pipeline layout
	inline bool GraphicsPipelineLayout_Create(GraphicsPipelineLayout* layout, SMGraphics_Core_GPU* GPU, const GraphicsPipelineLayoutCreateInfo& info)
	{
		layout->device = GPU->device;

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		//uniforms
		pipelineLayoutCreateInfo.setLayoutCount = info.descriptorLayouts.size();
		pipelineLayoutCreateInfo.pSetLayouts = info.descriptorLayouts.data();

		//push constants
		pipelineLayoutCreateInfo.pushConstantRangeCount = info.pushConstants.size();
		pipelineLayoutCreateInfo.pPushConstantRanges = info.pushConstants.data();

		//create layout
		if (vkCreatePipelineLayout(layout->device, &pipelineLayoutCreateInfo, nullptr, &layout->pipelineLayout) != VK_SUCCESS)
		{
			BTD::Logger::LogError("Smok Graphics", "Graphics Pipeline Layout", "Create", "Failed to create graphics pipeline layout");
			return false;
		}

		return true;
	}

	//destroys the pipeline layout
	inline void GraphicsPipelineLayout_Destroy(GraphicsPipelineLayout* layout)
	{
		if (layout->pipelineLayout == VK_NULL_HANDLE)
			return;

		vkDestroyPipelineLayout(layout->device, layout->pipelineLayout, nullptr);
	}

	//defines the config
	struct PipelineConfigInfo
	{
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	//defines a graphics pipeline create info

	//defines a graphics pipeline
	struct GraphicsPipeline
	{
		uint64 assetID = 0; //the assetID for this asset
		VkPipeline pipeline = VK_NULL_HANDLE; //the handle for this object

		uint64 graphicsShaderAssetID = 0; //the asset ID for the graphics shader
		GraphicsShader* shader = nullptr; //the pointer handle to the graphics shader

		std::vector<VkVertexInputAttributeDescription> attDesc;
		VkVertexInputBindingDescription bindDesc;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		PipelineConfigInfo config{};
		
		VkDevice device = VK_NULL_HANDLE;
	};

	//creates the pipeline
	void GraphicsPipeline_Create(GraphicsPipeline* pipeline,
		VkDevice& _device, VkPipelineLayout& pipelineLayout, GraphicsShader* _shader, VkRenderPass& _renderPass,
		const VkVertexInputBindingDescription& _bindDesc, const std::vector<VkVertexInputAttributeDescription>& _attDesc);
	//remakes the pipeline
	void GraphicsPipeline_Recreate(GraphicsPipeline* pipeline, VkRenderPass& renderPass);
	//destroys the pipeline
	void GraphicsPipeline_Destroy(GraphicsPipeline* pipeline);

	//binds the pipeline
	inline void GraphicsPipeline_Bind(GraphicsPipeline* pipeline, VkCommandBuffer& comBuffer)
	{
		vkCmdBindPipeline(comBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);
	}

	//sets the viewport and scissor
	inline void GraphicsPipeline_SetViewportAndScissor(VkCommandBuffer& comBuffer,
		const BTD_Math_U32Vec2& size, const BTD_Math_U32Vec2& offset)
	{
		//sets viewport
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)size.x;
		viewport.height = (float)size.y;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		//sets scissor
		VkRect2D scissor{};
		scissor.offset = { Smok_Util_Typepun(offset.x, int32), Smok_Util_Typepun(offset.y, int32) };
		scissor.extent = { size.x, size.y };

		vkCmdSetViewport(comBuffer, 0, 1, &viewport);
		vkCmdSetScissor(comBuffer, 0, 1, &scissor);
	}
}