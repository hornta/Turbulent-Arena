//SpriteManager.hpp

#pragma once

#include <SFML\Graphics.hpp>
#include <map>
#include <vector>
#include <string>
#include <memory>

namespace bjoernligan
{
	namespace system
	{
		class SpriteManager
		{
		private:
			SpriteManager();
			SpriteManager(const SpriteManager&);
			SpriteManager& operator=(const SpriteManager&);

		public:
			typedef std::unique_ptr<SpriteManager> Ptr;
			static Ptr Create();

			~SpriteManager();

			void CleanUpSprites();

			sf::Sprite* LoadSprite(const std::string &p_sFilename, const std::string &p_sSpritename,
				int p_iPosX = 0, int p_iPosY = 0, int p_iWidth = 0, int p_iHeight = 0,
				float p_fXScale = 1.0, float p_fYScale = 1.0);
			void RemoveSprite(const std::string &p_sSpritename);

			sf::Sprite* GetSprite(const std::string &p_sSpritename);

		private:
			sf::Texture* LoadTexture(const std::string &p_sFilename);
			sf::Texture* GetTexture(const std::string &p_sFilename);

			std::map<std::string, sf::Texture*> m_axTextures;
			std::map<std::string, sf::Sprite*> m_axSprites;
		};
	}
}