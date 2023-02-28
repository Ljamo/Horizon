#include "hzpch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>
#include <Windows.h>
#include <stdio.h>
#pragma comment(lib, "user32.lib")

namespace Horizon {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	inline void OpenGLContext::Init()
	{
		SYSTEM_INFO siSysInfo;
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		HZ_CORE_INFO("OpenGL Info:");
		HZ_CORE_ERROR("  Vendor:     {0}", vendor);
		HZ_CORE_ERROR("  Renderer:   {0}", renderer);
		HZ_CORE_ERROR("  OpenGL      {0}", version);


	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}