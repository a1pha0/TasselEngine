#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Core/Layer.h"

namespace Tassel
{
	class TASSEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(const std::string& name = "ImGuiLayer");
		~ImGuiLayer() {}

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void OnUpdate(float deltaTime) override;
		void OnEvent(Event& event) override;

		void SetDarkThemeColors();

		inline void HandleEvents(bool handleEvents) { m_HandleEvents = handleEvents; }
	private:
		bool m_HandleEvents = true;
	};
}
