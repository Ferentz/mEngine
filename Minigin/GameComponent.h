#include <memory>

//#include <string>
//#include "GameObject.h"
//#include "Transform.h"

namespace dae
{
	class GameObject;

	class GameComponent
	{
	public:
		virtual ~GameComponent() noexcept = default;
		template<class T, typename... Args>
			requires std::is_base_of_v<GameComponent, T>
		friend std::unique_ptr<GameComponent> MakeComponent(GameObject* parent, Args&&...);

		GameComponent(const GameComponent& other) = default;
		GameComponent(GameComponent&& other) = default;
		GameComponent& operator=(const GameComponent& other) = default;
		GameComponent& operator=(GameComponent&& other) = default;

	//protected:
		GameObject* const m_pParent;
		GameComponent(GameObject* parent);
	};

	class derivedComponent final : public GameComponent
	{
	public:
		virtual ~derivedComponent() override = default;
		template<class T, typename... Args>
			requires std::is_base_of_v<GameComponent, T>
		friend std::unique_ptr<GameComponent> MakeComponent(GameObject* parent, Args&&...);
		
		derivedComponent(const derivedComponent& other) = default;
		derivedComponent(derivedComponent&& other) = default;
		derivedComponent& operator=(const derivedComponent& other) = default;
		derivedComponent& operator=(derivedComponent&& other) = default;

	//protected: // i tried something, it didnt work. i need fps
		float m_val{};
		derivedComponent(GameObject* parent, float val);
	};

	/*class TextComponent final : public GameComponent
	{
	public:
		TextComponent(GameObject* parent, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		virtual ~TextComponent() override = default;


		void Update(float deltaTime) ;
		void Render() const ;

		void SetText(const std::string& text);
		void SetPosition(float x, float y);
		void SetColor(const SDL_Color& color);

	private:
		bool m_needsUpdate{};
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		Transform m_transform{};
		std::shared_ptr<Font> m_font{};
		std::shared_ptr<Texture2D> m_textTexture{};

	};*/


		
	template<class T, typename... Args>
		requires std::is_base_of_v<GameComponent, T>
	std::unique_ptr<GameComponent> MakeComponent(GameObject* parent, Args&&...);
	
}