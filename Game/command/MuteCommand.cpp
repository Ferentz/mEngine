#include "MuteCommand.h"

#include <glm/glm.hpp>
#include <serviceLocator.h>
#include <sound/sound.h>

void digger::MuteCommand::Execute()
{
	
	dae::servicelocator soundService{};

	soundService.get_sound_system()->Mute();
}