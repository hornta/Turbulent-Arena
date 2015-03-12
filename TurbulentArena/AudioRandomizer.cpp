//AudioRandomizer.cpp

#include "stdafx.h"
#include "AudioRandomizer.hpp"
#include "AudioManager.hpp"

namespace bjoernligan
{
	AudioRandomizer::AudioRandomizer()
	{
		m_xAudioManager = ServiceLocator<system::AudioManager>::GetService();
	}

	void AudioRandomizer::AddSoundName(const std::string &p_sSoundName)
	{
		m_asSoundNames.push_back(p_sSoundName);
	}

	void AudioRandomizer::PlayRandomSound(const float &p_fVolume)
	{
		if (!m_xAudioManager || m_asSoundNames.empty())
			return;

		m_xAudioManager->PlaySoundClip(m_asSoundNames[random::random(0, m_asSoundNames.size() - 1)], p_fVolume);
	}
}