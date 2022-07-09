#include "app.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <map>
const static uint32_t WIDTH = 800;
const static uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else 
const bool enableValidationLayers = true;
#endif // DEBUG

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
	 void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage <<
		std::endl;

		return VK_FALSE;
}

bool checkValidationLayerSupport() {
	uint32_t layerCount = 0;
	if (vk::Result::eSuccess != vk::enumerateInstanceLayerProperties(&layerCount, nullptr)) {
		throw std::runtime_error("checkValidationLayerSupport check InstanceLayerProperties fail");
	}

	std::vector<vk::LayerProperties> availableLayers(layerCount);
	if (vk::Result::eSuccess != vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data())) {
		throw std::runtime_error("checkValidationLayerSupport check InstanceLayerProperties fail");
	}

	for (const auto& layerName : validationLayers) {
		bool find = false;
		for (const auto& availLayer : availableLayers) {
			if (0 == strcmp(layerName, availLayer.layerName)) {
				find = true;
				break;
			}
		}
		if (find == false) {
			return false;
		}
	}

	return true;
}
void App::DestroyDebugUtilsMessengerEXT(vk::Instance instance,
										VkDebugUtilsMessengerEXT debugMessenger, const
										VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
		"vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

void App::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType =
		VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr; // Optional
}


void App::setupDebugMessenger() {
	if (!enableValidationLayers) return;
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	populateDebugMessengerCreateInfo(createInfo);
	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr,&debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

VkResult App::CreateDebugUtilsMessengerEXT(vk::Instance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator,
			pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

std::vector<const char*> getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return extensions;
}

void App::createInstance() {
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}
	
	vk::ApplicationInfo appInfo;
	appInfo.sType = vk::StructureType::eApplicationInfo;
	appInfo.setPApplicationName("hello Triangle");
	appInfo.setApplicationVersion(VK_MAKE_VERSION(1, 0, 0));
	appInfo.setPEngineName("NO Engine");
	appInfo.setEngineVersion(VK_MAKE_VERSION(1, 0, 0));
	appInfo.setApiVersion(VK_API_VERSION_1_0);
	vk::InstanceCreateInfo createInfo{};
	createInfo.sType = vk::StructureType::eInstanceCreateInfo;
	createInfo.pApplicationInfo = &appInfo;

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers) {
		createInfo.setEnabledLayerCount(static_cast<uint32_t>(validationLayers.size()));
		createInfo.setPpEnabledLayerNames(validationLayers.data());
		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.setPNext((VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo);
	}
	else {
		createInfo.setEnabledLayerCount(0);
		createInfo.setPNext(nullptr);
	}

	//uint32_t glfwExtensionCount = 0;
	//const char** glfwExtensions;
    //glfwExtensions =
	//	glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    //createInfo.enabledExtensionCount = glfwExtensionCount;
	//createInfo.ppEnabledExtensionNames = glfwExtensions;
	//createInfo.enabledLayerCount = 0;

	auto extensions = getRequiredExtensions();
	createInfo.setEnabledExtensionCount(extensions.size());
	createInfo.setPpEnabledExtensionNames(extensions.data());

	vk::Result result = vk::createInstance(&createInfo, nullptr, &instance);
	if (result != vk::Result::eSuccess) {
		throw std::runtime_error("failed to create instance");
	}

	uint32_t extensionCount = 0;
	auto rescode = vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	if (rescode != vk::Result::eSuccess) {
		throw std::runtime_error("fail to get Instance Extension Properties");
	}
	std::vector<vk::ExtensionProperties> extension(extensionCount);
	if (vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, extension.data()) != vk::Result::eSuccess) {
		throw std::runtime_error("fail to get Instance Extension Properties");
	}


	std::cout << "avaiable extension: \n";
	for (const auto& e : extension) {
		std::cout << "\t " << e.extensionName << " \n";
	}
}

void App::initVulkan() {
	createInstance();
	setupDebugMessenger();
	pickPhysicalDevice();
	createLogicalDevice();
}


void App::cleanup() {
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	device.destroy();
	instance.destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void App::run() {
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void App::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void App::initWindow() {
	glfwInit();
	
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr,nullptr);

}

bool isDeviceSuitable(VkPhysicalDevice device) {
	VkPhysicalDeviceFeatures deviceFeatures;
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && 
		deviceFeatures.geometryShader;
}

int App::rateDeviceSuitability(VkPhysicalDevice device) {
	VkPhysicalDeviceFeatures deviceFeatures;
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	int score = 0;
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 100;
	}
	else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
		score += 50;
	}
	
	score += deviceProperties.limits.maxImageDimension2D;

	if (!deviceFeatures.geometryShader) {
		return 0;
	}
	if (!isDeviceSuitable(device)) {
		return 0;
	}
	std::cout << "device : " << deviceProperties.deviceName << " score : " << score << std::endl;

	return score;
}

void App::pickPhysicalDevice() {
	uint32_t physicalCount = 0;
	vkEnumeratePhysicalDevices(instance, &physicalCount, nullptr);
	if (physicalCount == 0) {
		throw std::runtime_error("not find any physical Device");
		return;
	}
	std::multimap<int, VkPhysicalDevice> scoreToDevices;
	std::vector<VkPhysicalDevice> physicalDevices(physicalCount);
	vkEnumeratePhysicalDevices(instance, &physicalCount, physicalDevices.data());

	std::cout << "find physical device num : " << physicalCount << std::endl;
	for (const auto& device : physicalDevices) {
		int score = rateDeviceSuitability(device);
		scoreToDevices.insert(std::make_pair(score, device));
		
	}

	if (scoreToDevices.rbegin()->first > 0) {
		this->physicalDevice = scoreToDevices.rbegin()->second;
	}
	else {
		throw std::runtime_error("all physical device are not suitable");
	}
}

QueueFamilyIndices App::findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices{};
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFailmy = i;
			break;
		}
		i++;
	}
	return indices;
}

bool App::isDeviceSuitable(VkPhysicalDevice device) {
	auto indices = findQueueFamilies(device);
	return indices.graphicsFailmy.has_value();
}

void App::createLogicalDevice() {
	auto indices = findQueueFamilies(physicalDevice);
	vk::DeviceQueueCreateInfo createInfo{};
	float queuePriority = 1.0f;
	createInfo.sType = vk::StructureType::eDeviceQueueCreateInfo;
	createInfo.setQueueFamilyIndex(indices.graphicsFailmy.value());
	createInfo.setQueueCount(1);
	createInfo.setQueuePriorities(queuePriority);
	vk::PhysicalDeviceFeatures deviceFeatures{};
	vk::DeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.sType = vk::StructureType::eDeviceCreateInfo;
	deviceCreateInfo.setPQueueCreateInfos(&createInfo);
	deviceCreateInfo.setQueueCreateInfoCount(1);
	deviceCreateInfo.setPEnabledFeatures(&deviceFeatures);
	deviceCreateInfo.enabledExtensionCount = 0;
	if (enableValidationLayers) {
		deviceCreateInfo.setEnabledLayerCount(validationLayers.size());
		deviceCreateInfo.setPpEnabledLayerNames(validationLayers.data());
	}
	else {
		deviceCreateInfo.setEnabledLayerCount(0);
	}
	if (physicalDevice.createDevice(&deviceCreateInfo, nullptr, &device) != vk::Result::eSuccess) {
		throw std::runtime_error("create device failed");
	}
}
