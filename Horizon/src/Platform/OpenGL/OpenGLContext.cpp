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

		// TODO: Fix Errors
		//HZ_CORE_INFO("OpenGL Info:");
		//printf("  OEM ID: %u\n", siSysInfo.dwOemId);
		//HZ_CORE_INFO("  OEM ID: %u\n", siSysInfo.dwOemId));
		//HZ_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		//HZ_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		//GetSystemInfo(&siSysInfo);
		//
		//printf("Hardware information: \n");
		//printf("  OEM ID: %u\n", siSysInfo.dwOemId);
		//printf("  Number of processors: %u\n",
		//	siSysInfo.dwNumberOfProcessors);
		//printf("  Page size: %u\n", siSysInfo.dwPageSize);
		//printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
		//printf("  Minimum application address: %lx\n",
		//	siSysInfo.lpMinimumApplicationAddress);
		//printf("  Maximum application address: %lx\n",
		//	siSysInfo.lpMaximumApplicationAddress);
		//printf("  Active processor mask: %u\n",
		//	siSysInfo.dwActiveProcessorMask);

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}