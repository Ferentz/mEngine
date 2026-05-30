#pragma once
#include <glm/glm.hpp>
#include <variant>

namespace dae
{
	class GameObject;
	class RenderComponent;

	class Transform final
	{
	public:
		const glm::vec2& GetPosition() const { return m_position; }
		void SetPosition(float x, float y);
		void SetPosition(const glm::vec2& position);
		void Translate(float x, float y);
		float GetRotation() const
		{
			return m_rotation_radians;
		}

		void SetRotation(float x);
		void Rotate(float degrees);
		/// <summary>
		/// A = B + C
		/// ChildGlobal = parentGlobal + childLocal
		/// </summary>
		/// <param name="in"></param>
		/// <returns></returns>
		Transform operator+(Transform const &  in) const
		{
			Transform out{};

			const float this_cos = cos(m_rotation_radians);
			const float this_sin = sin(m_rotation_radians);

			out.m_position.x = this->m_position.x + in.m_position.x * this_cos - in.m_position.y * this_sin;
			out.m_position.y = this->m_position.y + in.m_position.x * this_sin + in.m_position.y * this_cos;

			out.m_rotation_radians = this->m_rotation_radians + in.m_rotation_radians;

			return out;
		}

		/// <summary>
		/// B = A - C
		/// parentGlobal = ChildGlobal - childLocal
		/// </summary>
		/// <param name="in"></param>
		/// <returns></returns>
		Transform operator-(const Transform& in) const
		{
			Transform out{};

			out.m_rotation_radians = m_rotation_radians - in.m_rotation_radians;

			const float out_cos = cos(out.m_rotation_radians);
			const float out_sin = sin(out.m_rotation_radians);

			out.m_position.x = m_position.x - (in.m_position.x * out_cos - in.m_position.y * out_sin);
			out.m_position.y = m_position.y - (in.m_position.x * out_sin + in.m_position.y * out_cos);

			return out;
		}

		/// <summary>
		/// for A = B + C
		///  -> C = A.MissingLinkWith(B)
		/// childLocal = ChildGlobal.MissingLinkWith(parentGlobal)
		/// </summary>
		/// <param name="in"></param>
		/// <returns></returns>
		Transform MissingLinkWith(Transform const& in) const
		{
			Transform out{};

			float dx = this->m_position.x - in.m_position.x;
			float dy = this->m_position.y - in.m_position.y;

			const float in_cos = cos(in.m_rotation_radians);
			const float in_sin = sin(in.m_rotation_radians);


			out.m_position.x = dx * in_cos + dy * in_sin;
			out.m_position.y = -dx * in_sin + dy * in_cos;

			out.m_rotation_radians = this->m_rotation_radians - in.m_rotation_radians;
			
			return out;
		}

	private:

		glm::vec2 m_position{};
		float m_rotation_radians{};
	};

	class SmartTransform final
	{
	public:
		SmartTransform(GameObject* owner);

		void SetLocalTransform(Transform& newTransform);
		void SetLocalPosition(float x, float y);
		void SetLocalRotation(float x);

		/*void SetGlobalTransform(Transform& newTransform);
		void SetGlobalPosition(float x, float y, float z = 0);
		void SetGlobalRotation(float x);*/

		void Translate(float x, float y);

		Transform const * GetLocalTransform() const;
		Transform const * GetWorldTransform();

		float GetRotation() { return m_local.GetRotation(); }

		/// <summary>
		/// changes local transform so that the global stays the same
		/// </summary>
		/// <param name="newBase"> new base of the world transform</param>
		void ReconstructLocal(GameObject * newBase);

		void MakeDirty();
		bool IsDirty() { return m_dirty; }

	private:
		GameObject* m_pGameObject;
		bool m_dirty{ true };
		Transform m_local{};
		Transform m_global{};
	};
}
