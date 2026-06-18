#pragma once
#include <memory>

namespace  dae
{
	class SoundSystem;

	class servicelocator final
	{
		static std::unique_ptr<SoundSystem> _ss_instance;
	public:

		static SoundSystem* get_sound_system();

		static void register_sound_system(std::unique_ptr<SoundSystem>&& ss);
	};

}