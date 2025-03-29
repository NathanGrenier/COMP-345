#include <util/AudioManager.h>
#include <Global.h>

AudioManager& AudioManager::getInstance() {
	static AudioManager instance;
	return instance;
}

int AudioManager::init(int volumeLevel) {
	// Allocate Channels
	channelCount = Mix_AllocateChannels(eEffectChannel::kEffectChannelTotal);
	if (channelCount != eEffectChannel::kEffectChannelTotal)
	{
		SDL_Log("Unable to allocate channels! SDL_mixer error: %s\n", SDL_GetError());
	}

	// Set the volume level
	setGlobalVolumeLevel(volumeLevel);

	return channelCount;
}

void AudioManager::deallocateAllAudio() {
	for (auto& audio : mAudioCache) {
		Mix_FreeChunk(audio.second);
	}
	mAudioCache.clear();

	for (auto& music : mMusicCache) {
		Mix_FreeMusic(music.second);
	}
	mMusicCache.clear();
}

Mix_Chunk* AudioManager::loadAudio(const std::string& filePath) {
	if (!isWAVFormat(filePath)) {
		SDL_Log("Audio file must be in WAV format!");
		return nullptr;
	}

	std::string fullPath = ASSET_PATH + filePath;

	auto it = mAudioCache.find(fullPath);
	if (it != mAudioCache.end()) {
		return it->second; // Return the cached value
	}

	Mix_Chunk* audio = nullptr;
	audio = Mix_LoadWAV(fullPath.c_str());
	if (audio == nullptr) {
		SDL_Log("Unable to load audio! SDL_mixer error: %s\n", SDL_GetError());
		return nullptr;
	}

	mAudioCache[fullPath] = audio;
	return audio;
}

Mix_Music* AudioManager::loadMusic(const std::string& filePath) {
	if (!isWAVFormat(filePath)) {
		SDL_Log("Audio file must be in WAV format!");
		return nullptr;
	}

	std::string fullPath = ASSET_PATH + filePath;

	auto it = mMusicCache.find(fullPath);
	if (it != mMusicCache.end()) {
		return it->second; // Return the cached value
	}

	Mix_Music* music = nullptr;
	music = Mix_LoadMUS(fullPath.c_str());
	if (music == nullptr) {
		SDL_Log("Unable to load music! SDL_mixer error: %s\n", SDL_GetError());
		return nullptr;
	}

	mMusicCache[fullPath] = music;
	return music;
}

bool AudioManager::isWAVFormat(const std::string& filePath) {
	std::string extension = filePath.substr(filePath.find_last_of('.') + 1);
	return extension == "wav";
}

int AudioManager::getChannelVolumeLevel(int channel) const {
	return Mix_Volume(channel, -1);
}

void AudioManager::setChannelVolumeLevel(int channel, int volume) {
	Mix_Volume(channel, volume);
}

int AudioManager::getGlobalVolumeLevel() const {
	return volumeLevel;
}

void AudioManager::setGlobalVolumeLevel(int volume) {
	volumeLevel = volume;
	Mix_Volume(-1, volumeLevel);
	setMusicVolumeLevel(volumeLevel);
}

int AudioManager::getMusicVolumeLevel(Mix_Music* music) const {
	return Mix_GetMusicVolume(music);
}

void AudioManager::setMusicVolumeLevel(int volume) {
	musicVolumeLevel = volume;
	Mix_VolumeMusic(volume);
}