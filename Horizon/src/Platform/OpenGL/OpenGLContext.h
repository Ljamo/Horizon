#pragma once

#include "Horizon/Renderer/GraphicsContext.h"
#include <glad/glad.h>

struct GLFWwindow;

namespace Horizon {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}