#include "window.hh"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

namespace OT {

void Window::windowResizedFun(GLFWwindow* window, int width, int height)
{
	Window* w = static_cast<Window*>(glfwGetWindowUserPointer(window));
	w->windowResized(width, height);
}

void Window::keyEventFun(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* w = static_cast<Window*>(glfwGetWindowUserPointer(window));
	w->keyEvent(key, scancode, action, mods);
}



Window::Window(int w, int h, const std::string& title)
{
	window_ = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
	if (!window_)
	{
		throw std::runtime_error("Failed to create window");
	}
	glfwSetWindowUserPointer(window_, this);
	glfwSetFramebufferSizeCallback(window_, &Window::windowResizedFun);
	glfwSetKeyCallback(window_, &Window::keyEventFun);

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

void Window::windowResized(int w, int h)
{
	if (isContext_)
		glViewport(0, 0, w, h);

	onResized(w, h);
}

void Window::keyEvent(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		close();
	else
		onKey(key, scancode, action, mods);
}

} // ns

