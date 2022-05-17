#include "vk_render_pass.hpp"

namespace wabby::render::vulkan
{

vk::raii::RenderPass build_render_pass(const vk::raii::Device &device, const vk::SurfaceFormatKHR &surface_format)
{
    // TODO make configurable
    std::vector<vk::AttachmentDescription> attachment_descriptions;
    attachment_descriptions.reserve(2);

    // color attachment
    attachment_descriptions.emplace_back(vk::AttachmentDescription {
        .format = surface_format.format,
        .samples = vk::SampleCountFlagBits::e1,
        .loadOp = vk::AttachmentLoadOp::eClear,
        .storeOp = vk::AttachmentStoreOp::eStore,
        .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
        .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
        .initialLayout = vk::ImageLayout::eUndefined,
        .finalLayout = vk::ImageLayout::ePresentSrcKHR
    });

    std::vector<vk::AttachmentReference> attachment_references;
    attachment_references.emplace_back(vk::AttachmentReference {
        .attachment = 0,
        .layout = vk::ImageLayout::eAttachmentOptimal
    });

    std::vector<vk::SubpassDescription> subpass_descriptions;
    subpass_descriptions.emplace_back(vk::SubpassDescription {
        .pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
        .colorAttachmentCount = static_cast<uint32_t>(attachment_references.size()),
        .pColorAttachments = attachment_references.data()
    });

    std::vector<vk::SubpassDependency> subpass_dependencies;
    subpass_dependencies.emplace_back(vk::SubpassDependency{
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .srcAccessMask = {},
        .dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite,
    });

    vk::RenderPassCreateInfo render_pass_create_info
    {
        .attachmentCount = static_cast<uint32_t>(attachment_descriptions.size()),
        .pAttachments = attachment_descriptions.data(),

        .subpassCount = static_cast<uint32_t>(subpass_descriptions.size()),
        .pSubpasses = subpass_descriptions.data(),

        .dependencyCount = static_cast<uint32_t>(subpass_dependencies.size()),
        .pDependencies = subpass_dependencies.data()
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