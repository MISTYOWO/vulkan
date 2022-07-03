#include "app.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else 
const bool enableValidationLayers = true;
#endif // DEBUG

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
	 void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage <<
		std::endl;

		return VK_FALSE;
}

bool checkValidationLayerSupport() {
	uint32_t layerCount = 0;
	vk::enumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<vk::LayerProperties> availableLayers(layerCount);

	vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

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

void App::setupDebugMessenger() {
	if (!enableValidationLayers) return;
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
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
	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr,&debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

VkResult App::CreateDebugUtilsMessengerEXT(vk::Instance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		//std::cout << "CreateDebugUtilsMessengerEXT find" << std::endl;
		return func(instance, pCreateInfo, pAllocator,
			pDebugMessenger);
	}
	else {
		//std::cout << "CreateDebugUtilsMessengerEXT not find" << std::endl;
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
	if (enableValidationLayers) {
		createInfo.setEnabledLayerCount(static_cast<uint32_t>(validationLayers.size()));
		createInfo.setPpEnabledLayerNames(validationLayers.data());
	}
	else {
		createInfo.setEnabledLayerCount(0);
	}
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	
    glfwExtensions =
		glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;

	auto extensions = getRequiredExtensions();
	createInfo.setEnabledExtensionCount(extensions.size());
	createInfo.setPpEnabledExtensionNames(extensions.data());

	vk::Result result = vk::createInstance(&createInfo, nullptr, &instance);
	if (result != vk::Result::eSuccess) {
		throw std::runtime_error("failed to create instance");
	}

	uint32_t extensionCount = 0;
	vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<vk::ExtensionProperties> extension(extensionCount);
	vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, extension.data());


	std::cout << "avaiable extension: \n";
	for (const auto& e : extension) {
		std::cout << "\t " << e.extensionName << " \n";
	}
}

void App::initVulkan() {
	createInstance();
	setupDebugMessenger();
}


void App::cleanup() {
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	instance.destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}
