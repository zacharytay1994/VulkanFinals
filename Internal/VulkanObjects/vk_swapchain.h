#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace VK_OBJECT
{
	using SwapchainImages = std::vector<VkImage>;
	using SwapchainImageViews = std::vector<VkImageView>;

	struct Swapchain
	{
		VkSwapchainKHR		m_swapchain { VK_NULL_HANDLE };
		VkExtent2D			m_extent;
		VkFormat			m_format;
		SwapchainImages		m_images;
		SwapchainImageViews m_image_views;

		Swapchain ( VkPhysicalDevice vkPhysicalDevice , VkDevice vkLogicalDevice , VkSurfaceKHR vkSurface );
		void Destroy ();
		bool Create ( VkPhysicalDevice vkPhysicalDevice , VkDevice vkLogicalDevice , VkSurfaceKHR vkSurface );

	private:
		bool				m_success { false };
		VkDevice			m_logical_device { VK_NULL_HANDLE };

		VkSurfaceFormatKHR	GetSurfaceFormat ( VkPhysicalDevice vkPhysicalDevice , VkSurfaceKHR vkSurface );
		VkPresentModeKHR	GetPresentMode ( VkPhysicalDevice vkPhysicalDevice , VkSurfaceKHR vkSurface );
		VkExtent2D			GetExtent2D ( VkPhysicalDevice vkPhysicalDevice , VkSurfaceKHR vkSurface );
		SwapchainImages		GetImages ( VkDevice vkLogicalDevice );
		SwapchainImageViews CreateImageViews ( VkDevice vkLogicalDevice );
	};
}