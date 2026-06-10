#include "SkipCommand.h"

#include <glm/glm.hpp>

void digger::SkipCommand::Execute()
{
	GetSubject()->NextLevel();
}