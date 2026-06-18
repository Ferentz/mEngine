#include "sound.h"
#include <sound/sound.h>
#include <iostream>
namespace  dae
{
	
	void SoundSystem_Logging::Load(sound_id id)
	{
		std::cout << "loading sound : " << m_audioPaths[id];
		SoundSystem_Pimpled::Load(id);
	}

	void SoundSystem_Logging::Play(sound_id id, bool loop)
	{
		std::cout << "playing sound : " << m_audioPaths[id];
		SoundSystem_Pimpled::Play(id, loop);
	}

	void SoundSystem_Logging::Stop(sound_id id)
	{
		std::cout << "stopping sound : " << m_audioPaths[id];
		SoundSystem_Pimpled::Stop(id);
	}
	void dae::SoundSystem_Logging::Mute()
	{
		std::cout << "muting sound ";
		SoundSystem_Pimpled::Mute();
	}
}
