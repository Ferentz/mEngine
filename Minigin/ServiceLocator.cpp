#include <serviceLocator.h>
#include <sound/sound.h>

#include <utility>

namespace dae
{
	std::unique_ptr<SoundSystem> servicelocator::_ss_instance{nullptr};

	SoundSystem* servicelocator::get_sound_system()
	{
		return _ss_instance.get();
	}

	void servicelocator::register_sound_system(std::unique_ptr<SoundSystem>&& ss)
	{
		_ss_instance = std::move(ss);
	}
}