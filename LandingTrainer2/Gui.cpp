#include "Gui.h"

Gui::Gui(const char* windowName, int windowWidth, int windowHeight, bool resizable)
{
    if (!glfwInit()) throw std::runtime_error("Unable to initialize GLFW");
    glfwSetErrorCallback(GLFWErrorCallback);
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
    window = glfwCreateWindow(windowWidth, windowHeight, windowName, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

void Gui::render(std::function<void()> renderCallback, ImVec4 clearColor, ImGuiWindowFlags flags)
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImVec2 displaySize = ImGui::GetIO().DisplaySize;

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(displaySize, ImGuiCond_Always);

        ImGui::Begin("container", nullptr, flags);

        renderCallback();

        ImGui::End();

        ImGui::Render();
        int displayWidth, displayHeight;
        glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
        glViewport(0, 0, displayWidth, displayHeight);
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

void Gui::GLFWErrorCallback(int error, const char* description)
{
	std::cout << "GLFW Error " << description << " code: " << error << '\n';
}