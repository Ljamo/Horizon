#include "hzpch.h"
#include "RendererAPI.h"

namespace Horizon {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
	RendererAPI::Dimension RendererAPI::s_Dimension = RendererAPI::Dimension::_2D;

}