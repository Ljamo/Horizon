#pragma once

#include "Horizon/Core/Core.h"
#include "Horizon/Events/Event.h"

#include "Horizon/Core/Timestep.h"

namespace Horizon {

	class HORIZON_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}