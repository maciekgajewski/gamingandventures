#include "window.hh"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

namespace OT {

void Window::windowResized(GLFWwindow* window, int width, int height)
{
	Window* w = static_cast<Window*>(glfwGetWindowUserPointer(window));
	w->onResized(width, height);
}

void Window::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* w = static_cast<Window*>(glfwGetWindowUserPointer(window));
	w->onKey(key, scancode, action, mods);
}



Window::Window(int w, int h, const std::string& title)
{
	window_ = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
	if (!window_)
	{
		throw std::runtime_error("Failed to create window");
	}
	glfwSetWindowUserPointer(window_, this);
	glfwSetFramebufferSizeCallback(window_, &Window::windowResized);
	glfwSetKeyCallback(window_, &Window::keyEvent);

}

Window::~Window()
{
	if (window_)
		glfwDestroyWindow(window_);
}

void Window::makeContextCurrent()
{
	glfwMakeContextCurrent(window_);
	isContext_ = true;
}

void Window::close()
{
	glfwSetWindowShouldClose(window_, true);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(window_);
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(window_);
}

void Window::onResized(int w, int h)
{
	std::cout << "window resized to " << w << "x" << h << std::endl;
	if (isContext_)
		glViewport(0, 0, w, h);
}

void Window::onKey(int key, int, int action, int)
{
	std::cout << "key action. key: " << key << ", action: " << action << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		close();
}

} // ns

