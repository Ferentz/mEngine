#include <sound/sound.h>
#include <iostream>
namespace  dae
{
	
	void SoundSystem_Logging::Load(sound_id id)
	{
		std::cout << "loading sound : " << m_audioPaths[id];
		SoundSystem_Pimpled::Load(id);
	}

	void SoundSystem_Logging::Play(sound_id id, float volume)
	{
		std::cout << "playing sound : " << m_audioPaths[id];
		SoundSystem_Pimpled::Play(id, volume);
	}
}
