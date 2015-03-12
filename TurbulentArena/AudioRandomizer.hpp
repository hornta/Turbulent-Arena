//AudioRandomizer.hpp

#pragma once

namespace bjoernligan
{
	namespace system
	{
		class AudioManager;
	}

	class AudioRandomizer
	{
	public:
		AudioRandomizer();

		void AddSoundName(const std::string &p_sSoundName);
		void PlayRandomSound(const float &p_fVolume = 1.0f);

	private:
		system::AudioManager* m_xAudioManager;
		std::vector<std::string> m_asSoundNames;
	};
}