#pragma once

namespace Horizon {

	enum class GraphicsType
	{
		_2D = 0, _3D = 1
	};

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}