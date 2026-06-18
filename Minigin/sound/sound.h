#pragma once
#include <memory>
#include <vector>
#include <string>



namespace  dae
{
	using sound_id = unsigned short;
	class SoundSystem
	{
	public:
		SoundSystem() = default;
		SoundSystem(const SoundSystem& other) = delete;
		SoundSystem(SoundSystem&& other) = delete;
		SoundSystem& operator=(const SoundSystem& other) = delete;
		SoundSystem& operator=(SoundSystem&& other) = delete;

		virtual ~SoundSystem() = default;
		// why make the base class pure virtual?
		/*
			since we were told to make it an interface, i will. But i dont see the benifit.
			we make a pure virtual base class to make a pimpled "base" class to then make a decorated version of the base class.

			the only way in which i can see this being usefull is if you want to make a  version of the class like the null version,
			so you dont bloat it with an extra function pointer that it doesnt need
		*/
		virtual void Load(sound_id id) = 0;
		virtual void Play(sound_id id, bool loop = false) = 0;
		virtual void Stop(sound_id id) = 0;
		virtual void Mute() = 0;
	};


	class SoundSystem_Pimpled : public SoundSystem
	{
	protected:
		class SoundImpl;

		std::unique_ptr<SoundImpl> m_impl;

		std::vector<std::string> m_audioPaths;
		bool mute{ false };
	public:
		SoundSystem_Pimpled(std::vector<std::string>& paths);

		virtual ~SoundSystem_Pimpled() override;
		virtual void Load(sound_id id) override;
		virtual void Play(sound_id id, bool loop = false) override;
		virtual void Stop(sound_id id);
		virtual void Mute() override;


		void SetSounds(std::vector<std::string>&& paths) { m_audioPaths = paths; }

		
	};

	class SoundSystem_Logging final : public SoundSystem_Pimpled
	{

	public:
		SoundSystem_Logging(std::vector<std::string>& paths);
		virtual ~SoundSystem_Logging() override = default;
		virtual void Load(sound_id id) override;
		virtual void Play(sound_id id, bool loop = false) override;
		virtual void Stop(sound_id id) override;
		virtual void Mute() override;
	};

}