//AudioManager.h

#pragma once

namespace bjoernligan
{
	namespace system
	{
		class AudioManager
		{
		private:
			AudioManager(float p_fMasterVolume = 0.50f, int p_iSoundVolume = 100, int p_iMusicVolume = 100);
			AudioManager(const AudioManager&);
			AudioManager& operator=(const AudioManager&);

		public:
			typedef std::unique_ptr<AudioManager> Ptr;
			static Ptr Create(float p_fMasterVolume = 0.50f, int p_iSoundVolume = 100, int p_iMusicVolume = 100);

			~AudioManager();

			void CreateSoundBuffer(std::string p_sName, std::string p_sFilename, const std::string &p_sSoundGroup = "");
			void CreateMusic(std::string p_sName, std::string p_sFilename);

			void ChangeMasterVolume(float p_fMasterVolume);
			void ChangeSoundVolume(int p_iVolume);
			void ChangeMusicVolume(int p_iVolume);
			float GetMasterVolume();
			int GetRealSoundVolume();
			int GetRealMusicVolume();
			int GetSoundVolume();
			int GetMusicVolume();

			void PlaySoundClip(std::string p_sSoundName, float p_fVolAdjPercent = 1.0);
			void PlayMusic(std::string p_sMusicName);
			sf::Music* GetMusic(const std::string &p_sMusicName);
			void StopSoundsFromBuffer(const std::string &p_sSoundName);
			void StopAllMusic();

			void SoundsCleanUp();

			//sound-groups
			void AddSoundToGroup(const std::string &p_sSoundName, const std::string &p_sSoundGroup);
			void PlaySoundFromGroup(const std::string &p_sSoundGroup);

		private:
			float m_fMasterVolume;
			int m_iSoundVolume;
			int m_iMusicVolume;
			float m_fModifiedSoundVolume;
			float m_fModifiedMusicVolume;

			int m_iSoundCount;

			std::map<std::string, sf::SoundBuffer*> m_axSoundbuffers;
			std::map<std::string, sf::Sound*> m_axSounds;
			std::map<std::string, sf::Music*> m_axMusics;
			std::map<std::string, std::vector<std::string> > m_axSoundGroups;
		};
	}
}