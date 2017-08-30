#pragma once

#include <string>

class GLFWwindow;

namespace OT {

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

protected:

	virtual void onResized(int w, int h);
	virtual void onKey(int key, int /*scancode*/, int action, int /*mods*/);

private:

	static void windowResized(GLFWwindow* window, int width, int height);
	static void keyEvent(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/);

	GLFWwindow* window_ = nullptr;
	bool isContext_ = false;};


}
