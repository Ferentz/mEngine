#include <glm/glm.hpp>
#include <components/GameComponent.h>

namespace dae
{
	class Tile : public dae::GameComponent
	{
	public:
		Tile(GameObject& parent, int x, int y)
			:GameComponent{ parent },
			point{ x,y }
		{

		}

		glm::ivec2 const& GetPoint()
		{
			return point;
		}

		virtual float GetTileSize() const
		{
			return 1.f;
		}
		virtual bool CanMoveToTile() const
		{
			return true;
		}

	private:

		glm::ivec2 point;
	};
}
