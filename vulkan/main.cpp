#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "app.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "vulkan window", nullptr, nullptr);
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr,
		&extensionCount, nullptr);

	std::cout << extensionCount << " extensions supported" <<
		std::endl;

	glm::mat4 matrix;
	glm::vec4 vec;
	auto test = matrix * vec;

	{
		App app;
		app.initVulkan();
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}