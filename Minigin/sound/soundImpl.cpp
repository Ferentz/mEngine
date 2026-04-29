#include <sound/sound.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <vector>
#include <mutex>
#include <queue>
#include <functional>
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

        void LoadThreaded()
        {
            char* path = NULL;
            while (true)
            {
                std::unique_lock<std::mutex> lck(loadMtx);

                // this will now wait untill it's notified,
                // and only if the que isnt empty will it exxecute.
                loadVar.wait(lck, [&] {return !m_audioLoadQueue.empty(); });
                int audioIdx = m_audioLoadQueue.front();
                m_audioLoadQueue.pop();
                lck.unlock(); // we no longer need to be in the shared stuff, can rlease while teh thread continues.

                /* load a sound file */
                SDL_asprintf(&path, m_audioFiles[audioIdx].first.c_str(), SDL_GetBasePath());  /* allocate a string of the full file path */

                auto localAudio{ MIX_LoadAudio(m_mixer, path, false) };

                std::unique_lock<std::mutex> lck_audio(audioMtx); // new lock to write;
                m_audios.push_back(localAudio);
                if (!m_audios.back()) {
                    SDL_Log("Couldn't load %s: %s", path, SDL_GetError());
                    SDL_free(path);
                    m_audios.pop_back();
                }
                SDL_free(path);  /* done with this, the file is loaded. */
            }
        }

        void PlayThreaded()
        {
            //every thread can have it's own track.
            MIX_Track* track = MIX_CreateTrack(m_mixer);
            if (!track) {
                SDL_Log("Couldn't create a mixer track: %s", SDL_GetError());
            }

            /* we need a track on the mixer to play the audio. Each track has audio assigned to it, and
            all playing tracks are mixed together for the final output. */

            while (true)
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

                int audioIdx = m_audioPlayQueue.front();
                m_audioPlayQueue.pop();
                lck.unlock(); // we no longer need to be in the shared stuff, can rlease while teh thread continues.




                MIX_SetTrackAudio(track, m_audios[audioIdx]);
                /* start the audio playing! */
                MIX_PlayTrack(track, 0);  /* no extra options this time, so a zero for the second argument. */
                playVar.notify_all();
            }
        }
        std::vector<std::jthread> loadThreads;
        std::vector<std::jthread> playThreads;

    public:
        std::vector<std::pair<std::string, bool >> m_audioFiles;
        // needs to be resized in accordance with audio files,
        // on what the bool is doesnt realy matter since both re known at the start of the program
        std::vector<MIX_Audio*> m_audios;

        SoundImpl()
        {
            loadThreads.reserve(2);
            loadThreads.emplace_back(std::jthread(&SoundImpl::LoadThreaded, this));
            loadThreads.emplace_back(std::jthread(&SoundImpl::LoadThreaded, this));

            playThreads.reserve(2);
            playThreads.emplace_back(std::jthread(&SoundImpl::PlayThreaded, this)); // this pointer, can use std::bind.
            playThreads.emplace_back(std::jthread(&SoundImpl::PlayThreaded, this));

            if (!MIX_Init()) {
                SDL_Log("Couldn't init SDL_mixer library: %s", SDL_GetError());
            }

            m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
            if (!m_mixer) {
                SDL_Log("Couldn't create mixer on default device: %s", SDL_GetError());
            }
        }

        void Load(sound_id id)
        {
            m_audioLoadQueue.push(id);
            loadVar.notify_one();
        }

        void Play(sound_id id, float)
        {
            m_audioPlayQueue.push(id);
            playVar.notify_one();
        }
    };

    SoundSystem_Pimpled::SoundSystem_Pimpled(std::vector<std::string>&& paths)
        :m_impl{ std::make_unique<SoundImpl>() },
        m_audioPaths{ paths }
    {
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
        if (m_impl->m_audioFiles[id].second)
        {
            m_impl->Load(id);
            return;
        }
        m_impl->Play(id, volume);
    }
}


//sdl mixer example
/*

SDL_mixer Examples
SDL3_mixer
basics
01 - load - and -play
This example code creates a mixer, loads a single sound, and plays it once.

(Remember that web browsers won't make sound until you click on the page!)
    Other examples :
basics / 01 - load - and -play
basics / 02 - play - with - options
basics / 03 - play - multiple - sounds
basics / 04 - metadata
basics / 05 - sinewave
basics / 06 - seeking
// load-and-play.c ... 

//This example code creates a mixer, loads a single sound, and plays it once.

// This code is public domain. Feel free to use it for any purpose!


#define SDL_MAIN_USE_CALLBACKS 1  // use the callbacks instead of main() 
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>

 // We will use this renderer to draw into this window every frame.
    static SDL_Window * window = NULL;
static SDL_Renderer* renderer = NULL;
static MIX_Mixer* mixer = NULL;
static MIX_Track* track = NULL;

// This function runs once at startup. 
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    char* path = NULL;
    MIX_Audio* audio = NULL;

    SDL_SetAppMetadata("Example Load And Play", "1.0", "com.example.load-and-play");

    // this doesn't have to run very much, so give up tons of CPU time between iterations. Optional! 
    SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "5");

   // we don't need video, but we'll make a window for smooth operation.
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/basic/load-and-play", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!MIX_Init()) {
        SDL_Log("Couldn't init SDL_mixer library: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create a mixer on the default audio device. Don't care about the specific audio format. 
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer) {
        SDL_Log("Couldn't create mixer on default device: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //load a sound file 
    SDL_asprintf(&path, "%smusic.mp3", SDL_GetBasePath());  // allocate a string of the full file path 
    audio = MIX_LoadAudio(mixer, path, false);
    if (!audio) {
        SDL_Log("Couldn't load %s: %s", path, SDL_GetError());
        SDL_free(path);
        return SDL_APP_FAILURE;
    }

    SDL_free(path);  // done with this, the file is loaded.

    // we need a track on the mixer to play the audio. Each track has audio assigned to it, and
     //  all playing tracks are mixed together for the final output.

    track = MIX_CreateTrack(mixer);
    if (!track) {
        SDL_Log("Couldn't create a mixer track: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    MIX_SetTrackAudio(track, audio);

    // start the audio playing! 
    MIX_PlayTrack(track, 0);  //no extra options this time, so a zero for the second argument. 

    // we don't save `audio`; SDL_mixer will clean it up for us during MIX_Quit().

    return SDL_APP_CONTINUE;  // carry on with the program! 
}

//This function runs when a new event (mouse input, keypresses, etc) occurs. 
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; // end the program, reporting success to the OS. 
    }
    return SDL_APP_CONTINUE;  // carry on with the program! 
}

//This function runs once per frame, and is the heart of the program. 
SDL_AppResult SDL_AppIterate(void* appstate)
{
    //draw a blank video frame to keep the OS happy 
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    // when the track has finished playing, end the program. 
    if (!MIX_TrackPlaying(track)) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;  // carry on with the program! 
}

//This function runs once at shutdown. 
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    // SDL will clean up the window/renderer for us, MIX_Quit() destroys any mixer objects we made. 
    MIX_Quit();
}

*/

