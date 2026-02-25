#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z = 0);
		void SetPosition(const glm::vec3& position);

		float GetRotation() const
		{
			return m_rotation_radians;
		}

		void SetRotation(float x);

		Transform operator+(Transform const &  in)
		{
			Transform out{};
			out.m_position.x = in.GetPosition().x + this->m_position.x * cos(in.m_rotation_radians) - this->m_position.y * sin(in.m_rotation_radians);
			out.m_position.y = in.GetPosition().y + this->m_position.y * cos(in.m_rotation_radians) + this->m_position.x * sin(in.m_rotation_radians);
			out.m_rotation_radians = this->m_rotation_radians;
			return out;
		}

	private:

		glm::vec3 m_position{};
		float m_rotation_radians{};
	};

	class SmartTransform final
	{
	public:
		void SetTransform(Transform& newTransform);
		void SetPosition(float x, float y, float z = 0);
		void SetRotation(float x);

		Transform const * GetLocalTransform() const;

		Transform const * QueryWorldTransform(Transform const* const parentWorldTransform);
		Transform const * GetWorldTransform() const;

		float GetRotation() { return m_local.GetRotation(); }

		void MakeDirty();

	private:
		bool m_dirty{ true };
		Transform m_local{};
		Transform m_global{};
	};
}
