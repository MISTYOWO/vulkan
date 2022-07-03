#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

class App {
public:
	void initVulkan();
	void createInstance();
	void cleanup();
	void setupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(vk::Instance instance, 
										  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
										  const VkAllocationCallbacks* pAllocator, 
										  VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(vk::Instance instance,
									   VkDebugUtilsMessengerEXT debugMessenger, const
									   VkAllocationCallbacks* pAllocator);
private:
	vk::Instance instance;
	GLFWwindow* window;
	VkDebugUtilsMessengerEXT debugMessenger;
};