#include "vk_render_pass.hpp"

namespace wabby::render::vulkan
{

vk::raii::RenderPass build_render_pass(const vk::raii::Device &device, const vk::SurfaceFormatKHR &surface_format)
{
    vk::AttachmentDescription color_attachment_description
    {
        .format = surface_format.format,
        .samples = vk::SampleCountFlagBits::e1,
        .loadOp = vk::AttachmentLoadOp::eClear,
        .storeOp = vk::AttachmentStoreOp::eStore,
        .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
        .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
        .initialLayout = vk::ImageLayout::eUndefined,
        .finalLayout = vk::ImageLayout::ePresentSrcKHR
    };

    vk::ArrayProxy<vk::AttachmentDescription> color_attachment_descriptions(color_attachment_description);

    vk::AttachmentReference color_attachment_reference
    {
        .attachment = 0,
        .layout = vk::ImageLayout::eAttachmentOptimal
    };

    vk::ArrayProxy<vk::AttachmentReference> color_attachment_references(color_attachment_reference);

    vk::SubpassDescription subpass_description
    {
        .pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
        .colorAttachmentCount = color_attachment_references.size(),
        .pColorAttachments = color_attachment_references.data()
    };

    vk::ArrayProxy<vk::SubpassDescription> subpass_descriptions(subpass_description);

    vk::SubpassDependency subpass_dependency
    {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .srcAccessMask = {},
        .dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite,
    };

    vk::RenderPassCreateInfo render_pass_create_info
    {
        .attachmentCount = color_attachment_descriptions.size(),
        .pAttachments = color_attachment_descriptions.data(),
        .subpassCount = subpass_descriptions.size(),
        .pSubpasses = subpass_descriptions.data(),
        .dependencyCount = 1,
        .pDependencies = &subpass_dependency
    };

    return vk::raii::RenderPass(device, render_pass_create_info);
}

vk_render_pass::vk_render_pass(const vk::raii::Device &device, const vk::SurfaceFormatKHR &surface_format) :
    render_pass_(build_render_pass(device, surface_format))
{}


void vk_render_pass::begin(const vk::raii::CommandBuffer &buffer, const vk::raii::Framebuffer &framebuffer, vk::Rect2D render_area, vk::ArrayProxy<vk::ClearValue> clear_values)
{
    // begin renderpass
    vk::RenderPassBeginInfo render_pass_begin_info
    {
        .renderPass = *render_pass_,
        .framebuffer = *framebuffer,
        .renderArea = render_area,
        .clearValueCount = clear_values.size(),
        .pClearValues = clear_values.data()
    };
    buffer.beginRenderPass(render_pass_begin_info);
}

void vk_render_pass::end(const vk::raii::CommandBuffer &buffer)
{
    buffer.endRenderPass();
}

}