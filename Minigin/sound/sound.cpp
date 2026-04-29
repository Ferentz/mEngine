#include <sound/sound.h>
#include <iostream>
namespace  dae
{
	SoundSystem_Logging::SoundSystem_Logging(std::vector<std::string>&& paths)
		:SoundSystem_Pimpled{ std::move(paths) }
	{

	}
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
