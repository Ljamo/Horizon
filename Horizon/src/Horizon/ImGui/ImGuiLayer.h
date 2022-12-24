#pragma once

#include "Horizon/Layer.h"

#include "Horizon/Events/ApplicationEvent.h"
#include "Horizon/Events/KeyEvent.h"
#include "Horizon/Events/MouseEvent.h"

namespace Horizon {

	class HORIZON_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}