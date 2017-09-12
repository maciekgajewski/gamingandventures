#pragma once

#include <string>

class GLFWwindow;

namespace Utils {

class Window
{
public:

	Window(int w, int h, const std::string& title);
	~Window();
	Window(const Window&) = delete;

	void makeContextCurrent();
	void close();
	void swapBuffers();

	bool shouldClose() const;

	GLFWwindow* win() { return window_; }

protected:

	virtual void onResized(int w, int h) {}
	virtual void onKey(int key, int scancode, int action, int mods) {}

private:

	void windowResized(int w, int h);
	void keyEvent(int key, int scancode, int action, int mods);

	static void windowResizedFun(GLFWwindow* window, int width, int height);
	static void keyEventFun(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/);

	GLFWwindow* window_ = nullptr;
	bool isContext_ = false;};


}
