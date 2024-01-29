#include <SmokGraphics/Pipeline/GraphicsPipeline.hpp>

//configures a pipeline
static inline void ConfigurePipeline(Smok::Graphics::Pipeline::PipelineConfigInfo& configInfo)
{
	configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	configInfo.viewportInfo.viewportCount = 1;
	configInfo.viewportInfo.pViewports = nullptr;
	configInfo.viewportInfo.scissorCount = 1;
	configInfo.viewportInfo.pScissors = nullptr;

	configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
	configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	configInfo.rasterizationInfo.lineWidth = 1.0f;
	configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
	configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
	configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
	configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
	configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

	configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
	configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
	configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
	configInfo.multisampleInfo.alphaToCoverageEnable = VK_TRUE;  // Optional
	configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

	configInfo.colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	configInfo.colorBlendAttachment.blendEnable = VK_TRUE;
	configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	//configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	//configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	//configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
	//configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	//configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	//configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

	configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
	configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
	configInfo.colorBlendInfo.attachmentCount = 1;
	configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
	configInfo.colorBlendInfo.blendConstants[0] = 1.0f;  // Optional
	configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
	configInfo.colorBlendInfo.blendConstants[2] = 0.5f;  // Optional
	configInfo.colorBlendInfo.blendConstants[3] = 1.0f;  // Optional

	configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
	configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
	configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
	configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
	configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
	configInfo.depthStencilInfo.front = {};  // Optional
	configInfo.depthStencilInfo.back = {};   // Optional

	configInfo.dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
	configInfo.dynamicStateInfo.dynamicStateCount =
		static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
	configInfo.dynamicStateInfo.flags = 0;
}

void Smok::Graphics::Pipeline::GraphicsPipeline_Create(GraphicsPipeline* pipeline,
	VkDevice& _device, VkPipelineLayout& pipelineLayout,
	GraphicsShader* _shader, VkRenderPass& _renderPass,
	const VkVertexInputBindingDescription& _bindDesc, const std::vector<VkVertexInputAttributeDescription>& _attDesc)
{
	pipeline->device = _device;
	pipeline->config.renderPass = _renderPass;
	pipeline->shader = _shader;
	pipeline->bindDesc = _bindDesc;
	pipeline->attDesc = _attDesc;

	//configure the pipeline
	ConfigurePipeline(pipeline->config);

	VkPipelineShaderStageCreateInfo shaderStages[2];
	shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStages[0].module = pipeline->shader->vMod;
	shaderStages[0].pName = "main";
	shaderStages[0].flags = 0;
	shaderStages[0].pNext = nullptr;
	shaderStages[0].pSpecializationInfo = nullptr;
	shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStages[1].module = pipeline->shader->fMod;
	shaderStages[1].pName = "main";
	shaderStages[1].flags = 0;
	shaderStages[1].pNext = nullptr;
	shaderStages[1].pSpecializationInfo = nullptr;

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	//data bind
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(pipeline->attDesc.size());
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexAttributeDescriptions = pipeline->attDesc.data();
	vertexInputInfo.pVertexBindingDescriptions = &pipeline->bindDesc;

	//if there is data to bind
	//if (bindDesc)
	//{
	//	vertexInputInfo.vertexAttributeDescriptionCount =
	//		static_cast<uint32_t>(attDesc.size());
	//	vertexInputInfo.vertexBindingDescriptionCount = 1;
	//	vertexInputInfo.pVertexAttributeDescriptions = attDesc.data();
	//	vertexInputInfo.pVertexBindingDescriptions = bindDesc;
	//}
	//else
	//{
	//	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	//	vertexInputInfo.vertexBindingDescriptionCount = 0;
	//	vertexInputInfo.pVertexAttributeDescriptions = nullptr;
	//	vertexInputInfo.pVertexBindingDescriptions = nullptr;
	//}

	pipeline->pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline->pipelineInfo.stageCount = 2;
	pipeline->pipelineInfo.pStages = shaderStages;
	pipeline->pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipeline->pipelineInfo.pInputAssemblyState = &pipeline->config.inputAssemblyInfo;
	pipeline->pipelineInfo.pViewportState = &pipeline->config.viewportInfo;
	pipeline->pipelineInfo.pRasterizationState = &pipeline->config.rasterizationInfo;
	pipeline->pipelineInfo.pMultisampleState = &pipeline->config.multisampleInfo;
	pipeline->pipelineInfo.pColorBlendState = &pipeline->config.colorBlendInfo;
	pipeline->pipelineInfo.pDepthStencilState = &pipeline->config.depthStencilInfo;
	pipeline->pipelineInfo.pDynamicState = &pipeline->config.dynamicStateInfo;
	
	pipeline->pipelineInfo.layout = pipelineLayout;
	pipeline->pipelineInfo.renderPass = pipeline->config.renderPass;
	pipeline->pipelineInfo.subpass = pipeline->config.subpass;
	
	pipeline->pipelineInfo.basePipelineIndex = -1;
	pipeline->pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(
		pipeline->device,
		VK_NULL_HANDLE,
		1,
		&pipeline->pipelineInfo,
		nullptr,
		&pipeline->pipeline) != VK_SUCCESS) {
		BTD::Logger::LogError("Smok Graphics", "Graphics Pipeline", "GraphicsPipeline_Create", "Failed to create graphics pipeline");
	}

}

void Smok::Graphics::Pipeline::GraphicsPipeline_Destroy(GraphicsPipeline* pipeline)
{
	if (pipeline->pipeline == VK_NULL_HANDLE)
		return;

	pipeline->shader = nullptr;

	vkDestroyPipeline(pipeline->device, pipeline->pipeline, nullptr);
	pipeline->pipeline = VK_NULL_HANDLE;

	pipeline->device = VK_NULL_HANDLE;
}

void Smok::Graphics::Pipeline::GraphicsPipeline_Recreate(GraphicsPipeline* pipeline, VkRenderPass& pass)
{
	//destroy old pipeline
	vkDestroyPipeline(pipeline->device, pipeline->pipeline, nullptr);

	GraphicsPipeline_Create(pipeline, pipeline->device, pipeline->config.pipelineLayout, pipeline->shader,
		pass, pipeline->bindDesc, pipeline->attDesc);

	//create
	//if (vkCreateGraphicsPipelines(
	//	pipeline->device,
	//	VK_NULL_HANDLE,
	//	1,
	//	&pipeline->pipelineInfo,
	//	nullptr,
	//	&pipeline->pipeline) != VK_SUCCESS)
	//{
	//	BTD::Logger::LogError("Smok Graphics", "Graphics Pipeline", "GraphicsPipeline_Create", "Failed to create graphics pipeline");
	//}
}