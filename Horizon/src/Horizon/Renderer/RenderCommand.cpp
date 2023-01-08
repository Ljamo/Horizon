#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Horizon {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}