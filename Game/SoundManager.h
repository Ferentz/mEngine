#pragma once

#include <components/GameComponent.h>
#include <eventSystem/Listener.h>

#include <GameObject.h>
#include <serviceLocator.h>
#include <sound/sound.h>
#include <eventSystem/EventStack.h>
#include <eventSystem/EventHash.h>


#include <vector>
#include <string>
//namespace dae
//{
//	class InputMethod;
//	class TextureComponent;
//}

namespace digger
{
	class SoundManager : public dae::GameComponent, public dae::Listener
	{

		enum class soundType
		{
			select,
			start,
			music,
			dead,
			collect
		};
		std::vector<std::string> paths{ "select.mp3", "start.mp3", "music.mp3", "dead.mp3", "collect.mp3" };

		bool started{false};

		public:
			SoundManager(dae::GameObject  & parent)
				:dae::GameComponent{parent}
			{

				dae::EventStack::GetEventStack().Register(*this);
				dae::servicelocator soundService{};
				if(!soundService.get_sound_system())
				soundService.register_sound_system(std::make_unique<dae::SoundSystem_Pimpled>(paths));
				//soundService.get_sound_system().Play(2, true);
			}

			~SoundManager()
			{
				dae::EventStack::GetEventStack().Unregister(this);
			}

			void Start() override
			{
				started = true;
			}

			void TuneIn(dae::EventId id, dae::GameObject* ) override
			{
				if(!started) return;
				dae::servicelocator soundService{};
				switch (id)
				{
				case dae::make_sdbm_hash("selected button"):
				{
					soundService.get_sound_system()->Play(int(soundType::select));
					break;
				}
				case dae::make_sdbm_hash("start level"):
				{
					soundService.get_sound_system()->Play(int(soundType::start));
					break;
				}
				case dae::make_sdbm_hash("died player"):
				{
					soundService.get_sound_system()->Play(int(soundType::dead));
					break;
				}
				case dae::make_sdbm_hash("treshhold collected"):
				{
					soundService.get_sound_system()->Play(int(soundType::start));
					break;
				}
				case dae::make_sdbm_hash("gained points"):
				{
					soundService.get_sound_system()->Play(int(soundType::collect));
					break;
				}
				case dae::make_sdbm_hash("death enemy"):
				{
					soundService.get_sound_system()->Play(int(soundType::select));
					break;
				}
				case dae::make_sdbm_hash("name entered"):
				{
					soundService.get_sound_system()->Play(int(soundType::dead));
					break;
				}
				case dae::make_sdbm_hash("start game"):
				{
					soundService.get_sound_system()->Play(int(soundType::music),true);
					break;
				}
				case dae::make_sdbm_hash("end game"):
				{
					soundService.get_sound_system()->Stop(int(soundType::music));
					break;
				}
				}
			}
	};
}