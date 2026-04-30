#include "sound.h"
#include <sound/sound.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_audio.h>
//#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <vector>
#include <mutex>
#include <queue>
#include <memory>
#include <functional>
#include <string>
#include <utility>
#include <thread>
#include <ResourceManager.h>

namespace  dae
{

    class SoundSystem_Pimpled::SoundImpl
    {
        MIX_Mixer* m_mixer;
        std::mutex loadMtx{};
        std::mutex audioMtx{};
        std::mutex playMtx{};
        std::condition_variable loadVar;
        std::condition_variable playVar;
        std::condition_variable gotLoadedVar;
        bool ready{ false };

        std::queue<int> m_audioLoadQueue;
        std::queue<int> m_audioPlayQueue;

        void LoadThreaded(std::stop_token stopToken)
        {
            char* path = NULL;
            while (!stopToken.stop_requested())
            {
                std::unique_lock<std::mutex> lck(loadMtx);

                // this will now wait untill it's notified,
                // and only if the que isnt empty will it exxecute.
                loadVar.wait(lck, [&] {return !m_audioLoadQueue.empty(); });

                if (stopToken.stop_requested())
                {
                    loadVar.notify_all();
                    continue;
                }

                int audioIdx = m_audioLoadQueue.front();
                m_audioLoadQueue.pop();
                lck.unlock(); // we no longer need to be in the shared stuff, can rlease while teh thread continues.

               

                auto file{ ResourceManager::GetInstance().GetFullDataPath(m_audioFiles[audioIdx].first) };

                /* load a sound file */
                SDL_asprintf(&path, file.c_str(), SDL_GetBasePath());  /* allocate a string of the full file path */

                auto localAudio{ MIX_LoadAudio(m_mixer, path, false) };
                
                std::unique_lock<std::mutex> lck_audio(audioMtx); // new lock to write;
                m_audioFiles[audioIdx].second = true;
                m_audios.push_back(localAudio);
                if (!m_audios.back()) {
                    SDL_Log("Couldn't load %s: %s", path, SDL_GetError());
                    SDL_free(path);
                    m_audios.pop_back();
                    continue;
                }
                SDL_free(path);  /* done with this, the file is loaded. */

                playVar.notify_all();
            }
        }

        void PlayThreaded(std::stop_token stopToken)
        {
            //every thread can have it's own track.
            MIX_Track* track = MIX_CreateTrack(m_mixer);
            if (!track) {
                SDL_Log("Couldn't create a mixer track: %s", SDL_GetError());
            }

            /* we need a track on the mixer to play the audio. Each track has audio assigned to it, and
            all playing tracks are mixed together for the final output. */

            while (!stopToken.stop_requested())
            {
                std::unique_lock<std::mutex> lck(playMtx);

                // this will now wait untill it's notified,
                // and only if the que isnt empty will it exxecute.
                playVar.wait(lck, [&] {
                    bool empty{ m_audioPlayQueue.empty() };
                    if (!empty)
                   {
                       return m_audioFiles[m_audioPlayQueue.front()].second; // dont pass if the sound isnt loaded
                    }
                    return false; });

                if (stopToken.stop_requested())
                {
                    playVar.notify_all();
                    continue;
                }

                int audioIdx = m_audioPlayQueue.front();
                m_audioPlayQueue.pop();
                lck.unlock(); // we no longer need to be in the shared stuff, can rlease while teh thread continues.

                


                MIX_SetTrackAudio(track, m_audios[audioIdx]);
                /* start the audio playing! */
                MIX_PlayTrack(track, 0);  /* no extra options this time, so a zero for the second argument. */
            }
        }
        std::stop_source stopSource;
        

        std::vector<std::jthread> loadThreads;
        std::vector<std::jthread> playThreads;

    public:
        std::vector<std::pair<std::string, bool >> m_audioFiles;
        // needs to be resized in accordance with audio files,
        // on what the bool is doesnt realy matter since both re known at the start of the program
        std::vector<MIX_Audio*> m_audios;
        ~SoundImpl()
        {
            stopSource.request_stop();
            m_audioLoadQueue.push(0);
            loadVar.notify_all();
            m_audioPlayQueue.push(0);
            playVar.notify_all();

            for (auto& thread : loadThreads)
            {
                thread.join();
            }
            for (auto& thread : playThreads)
            {
                thread.join();
            }

        }
        SoundImpl(std::vector<std::string>& paths)
        {
            
            if (!MIX_Init()) {
                SDL_Log("Couldn't init SDL_mixer library: %s", SDL_GetError());
            }

            m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
            if (!m_mixer) {
                SDL_Log("Couldn't create mixer on default device: %s", SDL_GetError());
            }

            for (auto string : paths) // copy of the strings
            {
                m_audioFiles.emplace_back(string, false);
            }


            // creating stop_token object

            std::stop_token stopToken = stopSource.get_token();

            loadThreads.reserve(2);
            loadThreads.emplace_back(std::jthread(&SoundImpl::LoadThreaded, this, stopToken));
            loadThreads.emplace_back(std::jthread(&SoundImpl::LoadThreaded, this, stopToken));

            playThreads.reserve(2);
            playThreads.emplace_back(std::jthread(&SoundImpl::PlayThreaded, this, stopToken)); // this pointer, can use std::bind.
            playThreads.emplace_back(std::jthread(&SoundImpl::PlayThreaded, this, stopToken));


        }

        void Load(sound_id id)
        {
            m_audioLoadQueue.push(id);
            loadVar.notify_one();

            //LoadThreaded();
        }

        void Play(sound_id id, float)
        {
            m_audioPlayQueue.push(id);
            playVar.notify_one();
        }
    };

    SoundSystem_Logging::SoundSystem_Logging(std::vector<std::string>& paths)
        :SoundSystem_Pimpled{ paths }
    {

    }
    SoundSystem_Pimpled::SoundSystem_Pimpled(std::vector<std::string>& paths)
        :m_impl{ std::make_unique<SoundImpl>(paths) },
        m_audioPaths{ paths }
    {
    }

    SoundSystem_Pimpled::~SoundSystem_Pimpled()
    {
        // stop token for jthreads.
        //MIX_Quit();
    }

    void SoundSystem_Pimpled::Load(sound_id id)
    {
        // so it needs to load on a different thread
        m_impl->Load(id);
    }

    void SoundSystem_Pimpled::Play(sound_id id, float volume)
    {
        // and then play on a different thread.
        // ... code that plays sound "id" at given volume ...

        // check if the id has been loaded
        if (!m_impl->m_audioFiles[id].second)
        {
            m_impl->Load(id);
        }
        m_impl->Play(id, volume);
    }
}


