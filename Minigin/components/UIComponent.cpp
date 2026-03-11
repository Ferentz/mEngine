#include "UIComponent.h"
#include "GameObject.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include <implot.h>

namespace dae
{
	struct Transform_Exercise
	{
		float matrix[16] =
		{ 1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	};

	class GameObject3D_Exercise
	{
	public:
		Transform_Exercise transform{};
		int id{};
	};

	class GameObject3DAlt_Exercise
	{
	public:
		std::unique_ptr<Transform_Exercise> local{ std::make_unique<Transform_Exercise>() };
		int id{};
	};


	UIComponent::UIComponent(GameObject& parent, char const* name)
		:RenderComponent{ parent },
		m_name{ name }
	{
	}

	void UIComponent::Update(float)
	{
		if (m_run_trash1)
		{
			RunTrash<int>(m_timings);
			//RunTrash1();
			m_run_trash1 = false;
		}
	}

	void UIComponent::Render()
	{
		bool open{ true };

		//ImGui::SetNextWindowSize(ImVec2{ 300,200 });

		ImGui::Begin(m_name, &open);

		//ImGui::Text("welcome to mgui app");
		//ImGui::DragInt("buffer size", &bufferSize);
		ImGui::InputInt("# samples", &m_bufferSize);
		ImGui::InputInt("# tries to average", &m_tries);

		if (ImGui::Button("Trash the cache with Transform"))
		{
			m_run_trash1 = true;
			m_drawPlot1 = true;
		}

		float x_data[10]{ 0.f, 1.f, 2.f, 3.f, 4.f , 5.f , 6.f , 7.f , 8.f , 9.f };



		if (m_drawPlot1)
		{
			ImPlot::SetNextAxesToFit();
			if (ImPlot::BeginPlot("My Plot")) {

				//ImPlot::PlotBars("My Bar Plot", bar_data, 11);
				if (m_drawPlot1)ImPlot::PlotLine("GameObject", x_data, m_timings.data(), static_cast<int>(m_timings.size()));
			}
			ImPlot::EndPlot();

		}
		ImGui::End(); // everything you begin, you must end.
	}

	UIComponent_2::UIComponent_2(GameObject& parent, char const* name)
		:UIComponent{ parent , name }
	{
	}

	void UIComponent_2::Update(float)
	{
		if (m_run_trash1)
		{
			RunTrash<GameObject3D_Exercise>(m_timings);
			//RunTrash1();
			m_run_trash1 = false;
		}
		if (m_run_trash2)
		{
			RunTrash<GameObject3DAlt_Exercise>(m_timings2);
			//RunTrash2();
			m_run_trash2 = false;
		}
	}

	void UIComponent_2::Render()
	{
		bool open{ true };

		//ImGui::SetNextWindowSize(ImVec2{ 300,200 });

		ImGui::Begin(m_name, &open);

		//ImGui::Text("welcome to mgui app");
		//ImGui::DragInt("buffer size", &bufferSize);
		ImGui::InputInt("# samples", &m_bufferSize);
		ImGui::InputInt("# tries to average", &m_tries);

		if (ImGui::Button("Trash the cache with GameObject3D"))
		{
			m_run_trash1 = true;
			m_drawPlot1 = true;
		}
		if (ImGui::Button("Trash the cache with GameObject3DAlt"))
		{
			m_run_trash2 = true;
			m_drawPlot2 = true;
		}

		float x_data[10]{ 0.f, 1.f, 2.f, 3.f, 4.f , 5.f , 6.f , 7.f , 8.f , 9.f };



		if (m_drawPlot1 || m_drawPlot2)
		{
			ImPlot::SetNextAxesToFit();
			if (ImPlot::BeginPlot("My Plot")) {

				//ImPlot::PlotBars("My Bar Plot", bar_data, 11);
				if (m_drawPlot1)ImPlot::PlotLine("GameObject", x_data, m_timings.data(), static_cast<int>(m_timings.size()));
				if (m_drawPlot2)ImPlot::PlotLine("GameObjectAlt", x_data, m_timings2.data(), static_cast<int>(m_timings2.size()));

			}
			ImPlot::EndPlot();

		}

		ImGui::End(); // everything you begin, you must end.
	}
}