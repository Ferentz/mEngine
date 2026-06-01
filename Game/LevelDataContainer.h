#pragma once


#include <memory>


#include <GameObject.h>



namespace dae
{
	class InputMethod;
	class Tilegrid;
	class Scene;
}



namespace digger
{
	enum class gameMode
	{
		normal,
		coop,
		vs
	};

	enum class levelType
	{
		empty,
		hole,
		digger,
		spawner,
		goldbag,
		gem,
		digger2,
		nobbin
	};

	struct LevelData
	{
		int width{};
		int height{};
		std::vector<levelType> tiles{};
	};


	
	class LevelDataContainer
	{
		std::vector<LevelData> levels;
	public:
		void LoadData(std::string file);

		void BuildStartScreen();
		
		std::vector<LevelData> const& GetLeveles() { return levels; }
		
		void BuildScene(int level, dae::Scene& scene, gameMode mode, dae::InputMethod* player1, dae::InputMethod* player2);

		static std::unique_ptr<dae::GameObject> MakeDigger(dae::InputMethod* input, dae::Tilegrid& in_grid, int x, int y);

		static std::unique_ptr<dae::GameObject> MakeNobbin(dae::Tilegrid& in_grid, int x, int y);

		static std::unique_ptr<dae::GameObject> MakeNobbin_ai(dae::Tilegrid& in_grid, int x, int y);

		static std::unique_ptr<dae::GameObject> MakeNobbin_player(dae::InputMethod* input, dae::Tilegrid& in_grid, int x, int y);

		static std::unique_ptr<dae::GameObject> MakeGem(dae::Tilegrid& in_grid, int x, int y);

		static std::unique_ptr<dae::GameObject> MakeBag(dae::Tilegrid& in_grid, int x, int y);

		static std::unique_ptr<dae::GameObject> MakeSpawner(dae::Tilegrid& in_grid, int x, int y, int amount = -1);
	};

	
}