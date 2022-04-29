#pragma once

#include "VulkanObjects/vk_config.h"
#include "VulkanObjects/vk_vulkan_instance.h"
#include "VulkanObjects/vk_surface.h"
#include "VulkanObjects/vk_devices.h"
#include "VulkanObjects/vk_swapchain.h"
#include "VulkanObjects/vk_renderpass.h"
#include "VulkanObjects/vk_pipeline.h"
#include "VulkanObjects/vk_framebuffers.h"
#include "VulkanObjects/vk_command.h"
#include "VulkanObjects/vk_model.h"
#include "VulkanObjects/vk_image.h"
#include "VulkanObjects/vk_descriptor.h"

#include <vulkan/vulkan.h>
#include <Windows.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace RENDERER
{
	struct Vulkan
	{
		VK_OBJECT::Instance			m_instance;
		VK_OBJECT::Surface			m_surface;
		VK_OBJECT::Devices			m_devices;
		VK_OBJECT::Swapchain		m_swapchain;
		VK_OBJECT::RenderPass		m_renderpass;
		VK_OBJECT::Framebuffers		m_framebuffers;
		VK_OBJECT::Command			m_commands;
		VK_OBJECT::Descriptor		m_descriptor;

		VkQueue m_graphics_queue;
		VkQueue m_present_queue;

		Vulkan (
			HWND const& hwnd ,
			char const* appName ,
			VK_OBJECT::ConfigRef const& config
		);

		~Vulkan ();

		template <typename PushConstant>
		void CreateGraphicsPipeline ( char const* name , char const* vs , char const* fs )
		{
			// assert is pipeline does not already exist
			assert ( m_graphics_pipelines.find ( name ) == m_graphics_pipelines.end () );
			m_graphics_pipelines.insert (
				{
					name ,
					VK_OBJECT::GraphicsPipeline
					(
						VK_OBJECT::GraphicsPipeline::CreatePipelineLayout<PushConstant> ( m_devices.m_logical_device , m_descriptor.m_descriptor_set_layout ),
						m_devices.m_logical_device ,
						m_swapchain.m_extent ,
						m_renderpass.m_render_pass ,
						vs ,
						fs
					)
				}
			);
			std::cout << "[SUCCESS] RENDERER::Vulkan::CreateGraphicsPipeline - " << name << " pipeline created successfully." << std::endl;
		}

		void BindGraphicsPipeline ( std::string const& name );

		void AddModel ( std::string const& name , VK_OBJECT::Vertices const& vertices , VK_OBJECT::Indices const& indices );
		void AddModel ( std::string const& name , std::string const& fbx );

		void AddImage ( std::string const& name , std::string const& png );
		void BindImage ( std::string const& name );

		void Draw ( std::string const& model ,
			float x , float y , float z ,
			float rx = 0.0f , float ry = 0.0f , float rz = 0.0f ,
			float sx = 1.0f , float sy = 1.0f , float sz = 1.0f );

		void StartFrame ();
		void EndFrame ();

		void Wait ();

	private:
		// graphics pipelines
		std::string														m_current_graphics_pipeline { "" };
		std::unordered_map<std::string , VK_OBJECT::GraphicsPipeline>	m_graphics_pipelines;

		// models
		std::unordered_map<std::string , VK_OBJECT::Model>				m_models;
		std::unordered_map<std::string , VK_OBJECT::Image>				m_images;

		// sync objects
		std::vector<VkSemaphore>	m_available_semaphores;
		std::vector<VkSemaphore>	m_finished_semaphores;
		std::vector<VkFence>		m_in_flight_fences;
		std::vector<VkFence>		m_images_in_flight;

		// frame
		size_t		m_current_frame { 0 };
		uint32_t	m_current_image_index { 0 };

		// camera matrix
		glm::mat4	m_transformation_matrix;

		// vulkan innards
		VkQueue GetDeviceQueue ();
		bool InitializeSyncObjects ();
		void RecreateSwapchain ();
	};
}