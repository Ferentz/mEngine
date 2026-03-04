#include <iostream>


#include "GameComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include <implot.h>

namespace dae
{
	
	GameComponent::GameComponent(GameObject& parent)
		:m_pParent{&parent}
	{
	}

	void GameComponent::SetParent(GameObject& parent)
	{
		m_pParent = &parent;
	}
	

	GameObject* GameComponent::GetParent() const
	{
		return m_pParent;
	}

	derivedComponent::derivedComponent(GameObject& parent, float val)
		:GameComponent{ parent },
		m_val{val}
	{
	}

	RotatorComponent::RotatorComponent(GameObject& parent)
		:GameComponent{ parent }
	{
	}

	void RotatorComponent::Update(float deltaTime)
	{
		SmartTransform* transform{ GetParent()->GetTransform() };
		transform->SetRotation(transform->GetRotation() + m_rotatingSpeed * deltaTime);
		//transform->MakeDirty();
		GetParent()->MakeDirty();
	}

	void RotatorComponent::SetRotationSpeed(float speed)
	{
		m_rotatingSpeed = speed;
	}

	/*void RotatingObject::Update(float deltaTime)
	{
		m_transform.SetRotation(m_transform.GetRotation() + m_rotatingSpeed * deltaTime);
		MakeDirty();
		GameObject::Update(deltaTime);
	}
	void RotatingObject::SetRotationSpeed(float speed)
	{
		m_rotatingSpeed = speed;
	}*/

	void RenderComponent::Update(float)
	{
		/*if (m_dirty)
		{
			QueryWorldTransform();
			m_dirty = false;
		}*/
	}

	void RenderComponent::MakeDirty()
	{
		m_transform.MakeDirty();

	}

	void RenderComponent::SetPosition(float x, float y)
	{
		MakeDirty();
		m_transform.SetPosition(x, y, 0.0f);
	}

	Transform const* RenderComponent::GetLocalTransform() const
	{
		return m_transform.GetLocalTransform();
	}

	Transform const* RenderComponent::GetWorldTransform() const
	{
		return m_transform.GetWorldTransform();
	}

	Transform const * RenderComponent::QueryWorldTransform()
	{
		return m_transform.QueryWorldTransform(GetParent()->QueryWorldTransform());
	}

	

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
			RunTrash(m_timings);
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

	void UIComponent::RunTrash(std::vector<float>& toFill)
	{
		std::vector<int> buffer(m_bufferSize);
		auto start = std::chrono::high_resolution_clock::now();

		toFill.resize(m_timingsAmount);
		std::fill(toFill.begin(), toFill.end(), 0.f);

		for (int trie{}; trie < m_tries; trie++)
		{
			int step{};
			for (size_t stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
			{
				for (size_t i{}; i < buffer.size(); i += stepsize)
				{
					buffer[i] *= 2;
				}
				const auto current_time{ std::chrono::high_resolution_clock::now() };
				float m_deltaTime = std::chrono::duration<float>(current_time - start).count();
				//std::cout << m_deltaTime << "\n";

				toFill[step] += m_deltaTime;
				step++;
				if (step >= toFill.size())
				{
					break;
				}
				start = current_time;
			}
		}

		for (float& val : toFill)
		{
			val /= m_tries;
		}
	}

	UIComponent_2::UIComponent_2(GameObject& parent, char const* name)
		:UIComponent{ parent , name}
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

		float x_data[10]{0.f, 1.f, 2.f, 3.f, 4.f , 5.f , 6.f , 7.f , 8.f , 9.f};



		if (m_drawPlot1 || m_drawPlot2)
		{
			ImPlot::SetNextAxesToFit();
			if (ImPlot::BeginPlot("My Plot")) {
				
				//ImPlot::PlotBars("My Bar Plot", bar_data, 11);
				if(m_drawPlot1)ImPlot::PlotLine("GameObject", x_data, m_timings.data(), static_cast<int>(m_timings.size()));
				if (m_drawPlot2)ImPlot::PlotLine("GameObjectAlt", x_data, m_timings2.data(), static_cast<int>(m_timings2.size()));

			}
			ImPlot::EndPlot();
			
		}

		ImGui::End(); // everything you begin, you must end.
	}

	void TextureComponent::Render()
	{
		if (m_texture != nullptr)
		{
			const auto& pos = QueryWorldTransform()->GetPosition();
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
		}
	}

	TextureComponent::TextureComponent(GameObject& parent)
		:RenderComponent{ parent },
		m_texture(nullptr)
	{
	}

	void TextureComponent::SetTexture(const std::string& filename)
	{
		m_texture = ResourceManager::GetInstance().LoadTexture(filename);
	}

	TextComponent::TextComponent(GameObject& parent,
		const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
		:TextureComponent(parent),
		m_needsUpdate(true),
		m_text(text),
		m_color(color),
		m_font(std::move(font))
	{
	}

	void TextComponent::Update(float)
	{
		RenderComponent::Update({});
		if (m_needsUpdate)
		{
			const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
			if (surf == nullptr)
			{
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}
			auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
			if (texture == nullptr)
			{
				throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
			}
			SDL_DestroySurface(surf);
			m_texture = std::make_shared<Texture2D>(texture);
			m_needsUpdate = false;
		}
	}
	
	void TextComponent::SetText(const std::string& text)
	{
		m_text = text;
		m_needsUpdate = true;
	}

	void TextComponent::SetColor(const SDL_Color& color)
	{
		m_color = color;
		m_needsUpdate = true;
	}
	FPSComponent::FPSComponent(GameObject& parent, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
		:TextComponent{ parent, text, font, color }
	{
	}
	void FPSComponent::Update(float deltaTime)
	{
		m_accTime += deltaTime;
		++m_frameCount;
		if (m_accTime >= 1.f)
		{
			if (const float fps{ m_frameCount / m_accTime };
				m_prevDelta - fps >= 0.01f || m_prevDelta - fps <= -0.01f)
			{
				SetText(std::format("FPS: {:.2f}", fps));
				m_prevDelta = fps;
				TextComponent::Update(deltaTime);
			}
			m_accTime = 0;
			m_frameCount = 0;
		}
	}
	
	

}

