#pragma once
#include <glm/glm.hpp>
#include <components/GameComponent.h>

namespace dae
{
	class Tilegrid;
}

namespace digger
{
	class Spawner : public dae::GameComponent
	{
		glm::ivec2 spawnPos;
		dae::Tilegrid* grid;
		int numToSpawn;
		float timer{3.f};
		float interval{5.f};
	public:
		Spawner(dae::GameObject& object, dae::Tilegrid& grid, int x, int y, int load = -1);

		void Update(float) override;
	};
}