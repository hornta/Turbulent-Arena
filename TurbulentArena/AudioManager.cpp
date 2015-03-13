//AudioManager.cpp

#include "stdafx.h"
#include "AudioManager.hpp"
#include <sstream>

namespace bjoernligan
{
	namespace system
	{
		AudioManager::AudioManager(float p_fMasterVolume, int p_iSoundVolume, int p_iMusicVolume)
		{
			m_fMasterVolume = p_fMasterVolume;
			m_iSoundVolume = p_iSoundVolume;
			m_fModifiedSoundVolume = m_iSoundVolume*m_fMasterVolume;
			m_iMusicVolume = p_iMusicVolume;
			m_fModifiedMusicVolume = m_iMusicVolume*m_fMasterVolume;

			m_iSoundCount = 0;
		}

		AudioManager::Ptr AudioManager::Create(float p_fMasterVolume, int p_iSoundVolume, int p_iMusicVolume)
		{
			return Ptr(new AudioManager(p_fMasterVolume, p_iSoundVolume, p_iMusicVolume));
		}

		AudioManager::~AudioManager()
		{
			{
				auto xIt = m_axSounds.begin();
				while (xIt != m_axSounds.end())
				{
					delete xIt->second;
					(*xIt).second = nullptr;
					++xIt;
				}
				m_axSounds.clear();
			}

			{
				auto xIt = m_axSoundbuffers.begin();
				while (xIt != m_axSoundbuffers.end())
				{
					delete xIt->second;
					(*xIt).second = nullptr;
					++xIt;
				}
				m_axSoundbuffers.clear();
			}

			{
				auto xIt = m_axMusics.begin();
				while (xIt != m_axMusics.end())
				{
					delete xIt->second;
					(*xIt).second = nullptr;
					++xIt;
				}
				m_axMusics.clear();
			}
		}

		void AudioManager::CreateSoundBuffer(std::string p_sName, std::string p_sFilename, const std::string &p_sGroup)
		{
			auto xIt = m_axSounds.find(p_sName);
			if (xIt == m_axSounds.end())
			{
				std::pair<std::string, sf::SoundBuffer*> xPair(p_sName, new sf::SoundBuffer);
				if (xPair.second->loadFromFile("../data/audio/sounds/" + p_sFilename))
				{
					if (m_axSoundbuffers.insert(xPair).second && p_sGroup != "")
						AddSoundToGroup(p_sName, p_sGroup);
				}
				else
				{
					delete xPair.second;
					xPair.second = nullptr;
				}
			}
		}

		void AudioManager::CreateMusic(std::string p_sName, std::string p_sFilename)
		{
			auto xIt = m_axMusics.find(p_sName);
			if (xIt == m_axMusics.end())
			{
				std::pair<std::string, sf::Music*> xPair(p_sName, new sf::Music);
				if (xPair.second->openFromFile("../data/audio/music/" + p_sFilename))
				{
					xPair.second->setVolume(m_fModifiedMusicVolume);
					xPair.second->setLoop(true);
					m_axMusics.insert(xPair);
				}
				else
				{
					delete xPair.second;
					xPair.second = nullptr;
				}
			}
		}

		void AudioManager::ChangeMasterVolume(float p_fMasterVolume)
		{
			if (p_fMasterVolume < 0.0f)
				p_fMasterVolume = 0.0f;
			if (p_fMasterVolume > 1.0f)
				p_fMasterVolume = 1.0f;
			
			m_fMasterVolume = p_fMasterVolume;

			ChangeMusicVolume(m_iMusicVolume);
			ChangeSoundVolume(m_iSoundVolume);
		}

		void AudioManager::ChangeSoundVolume(int p_iVolume)
		{
			if (p_iVolume < 0)
				p_iVolume = 0;
			if (p_iVolume > 100)
				p_iVolume = 100;

			m_iSoundVolume = p_iVolume;
			m_fModifiedSoundVolume = m_iSoundVolume*m_fMasterVolume;

			auto xIt = m_axSounds.begin();
			while (xIt != m_axSounds.end())
			{
				xIt->second->setVolume(m_fModifiedSoundVolume);
				xIt++;
			}
		}

		void AudioManager::ChangeMusicVolume(int p_iVolume)
		{
			if (p_iVolume < 0)
				p_iVolume = 0;
			if (p_iVolume > 100)
				p_iVolume = 100;

			m_iMusicVolume = p_iVolume;
			m_fModifiedMusicVolume = m_iMusicVolume*m_fMasterVolume;

			auto xIt = m_axMusics.begin();
			while (xIt != m_axMusics.end())
			{
				xIt->second->setVolume(m_fModifiedMusicVolume);
				xIt++;
			}
		}

		float AudioManager::GetMasterVolume()
		{
			return m_fMasterVolume;
		}

		int AudioManager::GetRealSoundVolume()
		{
			return m_iSoundVolume;
		}

		int AudioManager::GetRealMusicVolume()
		{
			return m_iMusicVolume;
		}

		int AudioManager::GetSoundVolume()
		{
			return (int)m_fModifiedSoundVolume;
		}

		int AudioManager::GetMusicVolume()
		{
			return (int)m_fModifiedMusicVolume;
		}

		void AudioManager::PlaySoundClip(std::string p_sSoundName, float p_fVolAdjPercent)
		{
			auto xBufferIt = m_axSoundbuffers.find(p_sSoundName);
			if (xBufferIt != m_axSoundbuffers.end())
			{
				std::string sSoundName = p_sSoundName + std::to_string(++m_iSoundCount);
				m_axSounds.insert(std::pair<std::string, sf::Sound*>(sSoundName, new sf::Sound));
			
				auto xSoundsIt = m_axSounds.find(sSoundName);
				(*xSoundsIt).second->setBuffer(*(*xBufferIt).second);
				(*xSoundsIt).second->setVolume(m_fModifiedSoundVolume * p_fVolAdjPercent);
				(*xSoundsIt).second->play();
			}

			SoundsCleanUp();
		}

		void AudioManager::PlayMusic(std::string p_sMusicName)
		{
			auto xIt = m_axMusics.find(p_sMusicName);
			if (xIt != m_axMusics.end())
			{
				xIt->second->play();
			}
		}

		sf::Music* AudioManager::GetMusic(const std::string &p_sMusicName)
		{
			auto xIt = m_axMusics.find(p_sMusicName);
			if (xIt != m_axMusics.end())
			{
				return xIt->second;
			}

			return nullptr;
		}

		void AudioManager::StopSoundsFromBuffer(const std::string &p_sSoundName)
		{
			auto xIt = m_axSounds.begin();
			while (xIt != m_axSounds.end())
			{
				auto xBufferIt = m_axSoundbuffers.begin();
				while (xBufferIt != m_axSoundbuffers.end())
				{
					if ((*xIt).second->getBuffer() == xBufferIt->second && xBufferIt->first == p_sSoundName)
					{
						(*xIt).second->stop();
					}
					++xBufferIt;
				}
				++xIt;
			}
		}

		void AudioManager::StopAllMusic()
		{
			auto xIt = m_axMusics.begin();
			while (xIt != m_axMusics.end())
			{
				auto xBufferIt = m_axSoundbuffers.begin();
				while (xBufferIt != m_axSoundbuffers.end())
				{
					(*xIt).second->stop();
					++xBufferIt;
				}
				++xIt;
			}
		}

		void AudioManager::SoundsCleanUp()
		{
			auto xIt = m_axSounds.begin();
			while (xIt != m_axSounds.end())
			{
				if (xIt->second->getStatus() != sf::Music::Playing)
				{
					delete (*xIt).second;
					(*xIt).second = nullptr;
					xIt = m_axSounds.erase(xIt);
				}
				else
				{
					xIt++;
				}
			}
		}

		void AudioManager::AddSoundToGroup(const std::string &p_sSoundName, const std::string &p_sSoundGroup)
		{
			if (m_axSoundbuffers.find(p_sSoundName) == m_axSoundbuffers.end())
				return;

			auto itr = m_axSoundGroups.find(p_sSoundGroup);
			if (itr != m_axSoundGroups.end())
				(*itr).second.push_back(p_sSoundName);
			else
				m_axSoundGroups.insert(std::pair<std::string, std::vector<std::string> >(p_sSoundGroup, { p_sSoundName }));
		}

		void AudioManager::PlaySoundFromGroup(const std::string &p_sSoundGroup)
		{
			auto itr = m_axSoundGroups.find(p_sSoundGroup);
			if (itr != m_axSoundGroups.end() || !(*itr).second.empty())
				PlaySoundClip((*itr).second[random::random(0, (*itr).second.size() - 1)]);
		}
	}
}