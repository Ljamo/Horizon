#pragma once

namespace Horizon {

	enum class GraphicsType
	{
		Dimension2 = 0, Dimension3 = 1
	};

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}