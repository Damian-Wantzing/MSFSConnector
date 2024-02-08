#pragma once

#include <iostream>
#include <functional>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include<imgui_impl_glfw.h>

class Gui
{
public:
	Gui(const char* windowName, int windowWidth, int windowHeight, bool resizable = true);
	~Gui();
	void render(std::function<void()> renderCallback, ImVec4 clearColor, ImGuiWindowFlags flags = 0);

	static void GLFWErrorCallback(int error, const char* description);

private:

	GLFWwindow* window = nullptr;
};

