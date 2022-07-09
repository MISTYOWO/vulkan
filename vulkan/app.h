#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <optional>
struct QueueFamilyIndices;
class App {
public:

	App() : instance(nullptr), physicalDevice(nullptr), window(nullptr),debugMessenger(nullptr) {}
	void run();
	void initVulkan();
	void initWindow();
	void mainLoop();

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
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	void pickPhysicalDevice();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	void createLogicalDevice();
	int rateDeviceSuitability(VkPhysicalDevice device);

private:
	vk::Instance instance;
	vk::PhysicalDevice physicalDevice;
	GLFWwindow* window;
	VkDebugUtilsMessengerEXT debugMessenger;
	vk::Device device;
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFailmy;
};