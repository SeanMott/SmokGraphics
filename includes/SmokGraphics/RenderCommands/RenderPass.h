#pragma once

//defines the C API implementation of render pass

#include <BTDSTD_C/Math/Vectors.h>

#include <vulkan/vulkan.h>

typedef struct SMGraphics_RenderCommands_RenderPass
{
	BTD_Math_Color clearColor; //the clear color
	char* name; //the name of the render pass
} SMGraphics_RenderCommands_RenderPass;

#if defined(__cplusplus)
extern "C" {
#endif

	//initalizes the render pass
	inline void SMGraphics_RenderCommands_RenderPass_Create(SMGraphics_RenderCommands_RenderPass* renderPass,
		BTD_Math_Color* clearColor, char* name)
	{
		renderPass->name = name;
		BTD_Math_Color_SetColor(&renderPass->clearColor, clearColor);
	}

	////destroys the render pass
	//inline void SMGraphics_RenderCommands_RenderPass_Destroy(SMGraphics_RenderCommands_RenderPass* renderPass)
	//{
	//	//BTD_String_DestroyString(&renderPass->name);
	//}

	//starts a render pass
	inline void SMGraphics_RenderCommands_RenderPass_StartRenderPass(SMGraphics_RenderCommands_RenderPass* renderPass,
		VkCommandBuffer& comBuffer,
		VkFramebuffer& targetFrameBuffer, VkRenderPass& renderPassAttachs,
		const BTD_Math_U32Vec2 renderSize, const BTD_Math_I32Vec2 renderOffset)
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

		renderPassInfo.renderPass = renderPassAttachs;
		renderPassInfo.framebuffer = targetFrameBuffer;

		renderPassInfo.renderArea.offset = { renderOffset.x, renderOffset.y };
		renderPassInfo.renderArea.extent = { renderSize.x, renderSize.y };

		renderPassInfo.clearValueCount = 2;
		VkClearValue clearValues[2];
		memcpy(clearValues[0].color.float32, ((float*)&renderPass->clearColor), sizeof(clearValues[0].color.float32)); //copies data for clear color
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.pClearValues = clearValues;

		vkCmdBeginRenderPass(comBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	//ends a render pass
	inline void SMGraphics_RenderCommands_RenderPass_EndRenderPass(SMGraphics_RenderCommands_RenderPass* renderPass,
		VkCommandBuffer& comBuffer)
	{
		vkCmdEndRenderPass(comBuffer);
	}

#if defined(__cplusplus)
}
#endif