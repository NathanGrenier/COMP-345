#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <tuple>
#include <map>

class AudioManager {
public:
	static const int MAX_VOLUME = MIX_MAX_VOLUME;

	static AudioManager& getInstance();
	int init(int volumeLevel = MAX_VOLUME / 2);
	void deallocateAllAudio();

	Mix_Chunk* loadAudio(const std::string& filepath);
	Mix_Music* loadMusic(const std::string& filePath);

	int getChannelVolumeLevel(int channel) const;
	void setChannelVolumeLevel(int channel, int volume);

	int getGlobalVolumeLevel() const;
	void setGlobalVolumeLevel(int volume);

	int getMusicVolumeLevel(Mix_Music* music) const;
	void setMusicVolumeLevel(int volume);

	int getMusicLevelMapped(Mix_Music* music);
	int getChannelVolumeMapped(int channel);

	void setGameVolume(int mappedVolumeGame);

	int volumeLevel;
	int musicVolumeLevel;

	//Channel constants. Starts at 0
	enum eEffectChannel
	{
		eEffectChannelTowerShot,
		eEffectChannelEnemyDeath,
		eEffectChannelTowerPurchase,
		eEffectChannelUI,
		kEffectChannelTotal,
	};
	bool UIChannelPlaying;

	int channelCount;
private:
	AudioManager() : volumeLevel(0), musicVolumeLevel(0), UIChannelPlaying(false), channelCount(0) {}
	~AudioManager() = default;

	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	bool isWAVFormat(const std::string& filePath);

	std::map<std::string, Mix_Chunk*> mAudioCache;
	std::map<std::string, Mix_Music*> mMusicCache;

	static inline const std::string ASSET_PATH = "assets/";
};