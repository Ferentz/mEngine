#include <vector>
#include <fstream>
#include <sstream>

namespace digger
{
	enum class levelType
	{
		empty,
		hole,
		digger,
		spawner,
		goldbag,
		gem,
		nobbin
	};

	struct LevelData
	{
		int width{};
		int height{};
		std::vector<levelType> tiles{};
	};

	class Level
	{
		std::vector<LevelData> levels;
	public:
		void LoadData(std::vector<levelType>& vector, std::string file)
		{
			std::ifstream reader{};
			reader.open(file);

			if (!reader.is_open())
				return;

			std::string line;

			LevelData* level{};

			while (std::getline(reader, line))
			{
				if (line.empty()) continue;

				//comments
				if (line.find("//") != std::string::npos) continue;

				if (line.find("Level") != std::string::npos)
				{
					levels.push_back();
					level = &levels.back();
					continue;
				}
				int width{};
				for (auto & character : line)
				{
					level->tiles.push_back(character - '0');
					width++;
				}
				level->width = width{}; //yes, it will be set to the width of the last line in the level
				level->height++;
				
			}
		}
	};
}