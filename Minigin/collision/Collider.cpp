#include "Collider.h"
#include "Renderer.h"

#include <SDL3/SDL.h>
namespace dae
{
	std::vector<Collider*> Collider::s_colliders{};

	Collider::Collider(GameObject& object)
		:RenderComponent(object)
	{
		s_colliders.push_back(this);
	}

	Collider::~Collider()
	{
		s_colliders.erase(std::remove(s_colliders.begin(), s_colliders.end(), this), s_colliders.end());
	}


	void Collider::Update(float)
	{
		collisions.clear();
		if (!canCollide) return;
		for (auto collider : s_colliders)
		{
			if (this == collider) continue;
			if (Overlap(*collider))
			{
				collisions.push_back(collider);
			}
		}
		if (!collisions.empty()) m_signal.BroadCast(make_sdbm_hash("collide"), GetGameObject());
	}

	bool Collider::Overlap(Collider& other)
	{
		if (!other.canCollide)
			return false;

		if (other.isTrigger)
			return false;

		const auto& myPos =GetGameObject()->m_transform.GetWorldTransform()->GetPosition();

		const auto& otherPos =other.GetGameObject()->m_transform.GetWorldTransform()->GetPosition();

		return
			myPos.x < otherPos.x + other.size.x &&
			myPos.x + size.x > otherPos.x &&
			myPos.y < otherPos.y + other.size.y &&
			myPos.y + size.y > otherPos.y;
	}
	bool Collider::isPointInsideAABB(glm::vec2 pos2d, glm::vec2 point) {
		return
			point.x >= pos2d.x &&
			point.x <= pos2d.x + size.x &&
			point.y >= pos2d.y &&
			point.y <= pos2d.y + size.y
			;
	}

	void dae::Collider::Render()
	{
		if (!debug) return;
		SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();

		if (!renderer)
			return;

		const auto& pos = GetGameObject()->m_transform.GetWorldTransform()->GetPosition();

		SDL_FRect rect
		{
			pos.x,
			pos.y,
			size.x,
			size.y
		};

		// Choose color
		if (isTrigger)
		{
			SDL_SetRenderDrawColor(
				renderer,
				0, 255, 255, 255); // cyan
		}
		else if (!collisions.empty())
		{
			SDL_SetRenderDrawColor(
				renderer,
				255, 0, 0, 255); // red = colliding
		}
		else
		{
			SDL_SetRenderDrawColor(
				renderer,
				0, 255, 0, 255); // green = normal
		}



		SDL_RenderRect(renderer, &rect);
	}

}