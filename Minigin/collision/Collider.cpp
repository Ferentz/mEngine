#include "Collider.h"

namespace dae
{
	std::vector<Collider*> Collider::s_colliders{};

	Collider::Collider(GameObject& object)
		:GameComponent(object)
	{
		s_colliders.push_back(this);
	}

	Collider::~Collider()
	{
		s_colliders.erase(std::remove(s_colliders.begin(), s_colliders.end(), this), s_colliders.end());
	}


	void Collider::Update(float)
	{
		if (!canCollide) return;
		collisions.clear();

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
		if (!other.canCollide) return false;

		if (other.isTrigger) return false;
		auto const& mypos = GetGameObject()->m_transform.GetWorldTransform()->GetPosition();
		auto const& otherPos = other.GetGameObject()->m_transform.GetWorldTransform()->GetPosition();

		if (
			isPointInsideAABB(
				glm::vec2(mypos.x, mypos.y), glm::vec2(otherPos.x, otherPos.y)
			)
			) return true;

		if (
			isPointInsideAABB(
				glm::vec2(mypos.x, mypos.y), glm::vec2(otherPos.x + other.size.x, otherPos.y)
			)
			) return true;
		if (
			isPointInsideAABB(
				glm::vec2(mypos.x, mypos.y), glm::vec2(otherPos.x, otherPos.y + other.size.y)
			)
			) return true;
		if (
			isPointInsideAABB(
				glm::vec2(mypos.x, mypos.y), glm::vec2(otherPos.x + other.size.x, otherPos.y + other.size.y)
			)
			) return true;

		return false;
	}
	bool Collider::isPointInsideAABB(glm::vec2 pos2d, glm::vec2 point) {
		return
			point.x >= pos2d.x &&
			point.x <= pos2d.x + size.x &&
			point.y >= pos2d.y &&
			point.y <= pos2d.y + size.y
			;
	}

}