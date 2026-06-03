#include "Spawner.h"

#include "LevelDataContainer.h"
#include <SceneManager.h>

namespace digger
{
	Spawner::Spawner(dae::GameObject& object, dae::Tilegrid& tilegrid, int x, int y, int load)
		:GameComponent{object}
		, spawnPos{x,y}
		, grid{&tilegrid }
		, numToSpawn{load}
	{

	}
	void Spawner::Update(float delta)
	{
		if (numToSpawn == 0) return;

		timer += delta;
		if (timer >= interval )
		{
			timer = 0;
			if (numToSpawn > 0) numToSpawn--;
			auto nob = LevelDataContainer::MakeNobbin_ai(*grid, spawnPos.x, spawnPos.y);

			// could do a set parent and making this nob release. but this is safe even if working in the non-active scene.
			if(auto parent = GetGameObject()->GetParentAsObject()) parent->Add(std::move(nob));
			else GetGameObject()->GetParentAsScene()->Add(std::move(nob));
			// spawn nobbin
		}
	}
}