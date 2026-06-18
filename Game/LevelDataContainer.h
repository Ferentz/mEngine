#pragma once


#include <memory>


#include <GameObject.h>
#include <Singleton.h>



namespace dae
{
	class InputMethod;
	class Tilegrid;
	class Scene;
}



namespace digger
{
	enum class gameMode;
	class Spawner;

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

	struct HighScore
	{
		std::string name{};
		int score{};
	};
	
	class LevelDataContainer final: public  dae::Singleton<LevelDataContainer>
	{
		std::vector<LevelData> levels;
		std::vector<HighScore> highscores{};

		friend class Singleton<LevelDataContainer>;
		LevelDataContainer() = default;
	public:

		std::vector<HighScore> & GetHighScores() { return highscores;  }
		void LoadData(std::string file);

		void BuildStartScreen(dae::Scene& scene);

		void CreateLeaderBoard(dae::Scene* scene = nullptr);

		void BuildEndScreen(dae::GameObject& scene);

		std::vector<HighScore> loadHighScores();

		bool SaveHighScores( std::vector<HighScore> const& scores);

		
		std::vector<LevelData> const& GetLeveles() { return levels; }
		
		
		void BuildLevel(int level, dae::GameObject& scene, gameMode mode, dae::InputMethod* player1, dae::InputMethod* player2);

		static std::unique_ptr<dae::GameObject> MakeDigger(dae::InputMethod* input, dae::Tilegrid& in_grid, int x, int y);

		static std::unique_ptr<dae::GameObject> MakeNobbin(dae::Tilegrid& in_grid, Spawner* home, int x, int y);

		static std::unique_ptr<dae::GameObject> MakeNobbin_ai(dae::Tilegrid& in_grid, Spawner* home,int x, int y);

		static std::unique_ptr<dae::GameObject> MakeNobbin_player(dae::InputMethod* input, dae::Tilegrid& in_grid, int x, int y);

		static std::unique_ptr<dae::GameObject> MakeGem(dae::Tilegrid& in_grid, int x, int y);

		static std::unique_ptr<dae::GameObject> MakeBag(dae::Tilegrid& in_grid, int x, int y);

		static std::unique_ptr<dae::GameObject> MakeSpawner(dae::Tilegrid& in_grid, int x, int y, int amount = -1);

		std::unique_ptr<dae::GameObject> MakeGameEssentials(dae::GameObject* root, gameMode m, dae::InputMethod* player1, dae::InputMethod* player2);
	};

	
}