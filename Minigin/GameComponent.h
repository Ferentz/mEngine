#include <string>
#include <memory>

namespace dae
{
	class GameObject;
	class Texture2D;

	class GameComponent
	{
	public:
		virtual ~GameComponent() = default;
		explicit GameComponent(GameObject& parent);

		void SetParent(GameObject& parent);
		void Update(float) {};

	protected:
		GameObject const* GetParent() const;

	private:
		GameObject* m_pParent;
		
		GameComponent(const GameComponent& other) = delete;
		GameComponent(GameComponent&& other) = delete;
		GameComponent& operator=(const GameComponent& other) = delete;
		GameComponent& operator=(GameComponent&& other) = delete;
	};


	class derivedComponent final : public GameComponent
	{
	public:
		virtual ~derivedComponent() override = default;
		explicit derivedComponent(GameObject& parent, float val);


	protected: // i tried something, it didnt work. i need fps
		float m_val{};
		
		derivedComponent(const derivedComponent& other) = delete;
		derivedComponent(derivedComponent&& other) = delete;
		derivedComponent& operator=(const derivedComponent& other) = delete;
		derivedComponent& operator=(derivedComponent&& other) = delete;
	};

	class RenderComponent : public GameComponent
	{
	public:
		virtual ~RenderComponent() = default;
		explicit RenderComponent(GameObject& parent)
			:GameComponent(parent)
		{
		}

		virtual void Render() const
		{

		}

	};

	class TextureComponent final : public RenderComponent
	{
	public:
		virtual ~TextureComponent() override = default;
		explicit TextureComponent(GameObject& parent);

		virtual void Render() const override;
		//virtual void Render()const override {}
		void SetTexture(const std::string& filename);

	protected:

	private:
		std::shared_ptr<Texture2D> m_texture{};
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

}