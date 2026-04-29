#include <serviceLocator.h>
#include <sound/sound.h>

namespace dae
{
	SoundSystem& servicelocator::get_sound_system()
	{
		return *_ss_instance;
	}

	void servicelocator::register_sound_system(std::unique_ptr<SoundSystem>&& ss)
	{
		_ss_instance = std::move(ss);
	}
}