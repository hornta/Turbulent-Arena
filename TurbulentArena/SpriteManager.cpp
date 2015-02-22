//SpriteManager.cpp

#include "stdafx.h"
#include "SpriteManager.hpp"
#include "Settings.hpp"
#include <fstream>

namespace bjoernligan
{
	namespace system
	{
		SpriteManager::SpriteManager()
		{

		}

		SpriteManager::Ptr SpriteManager::Create()
		{
			return Ptr(new SpriteManager);
		}

		sf::Sprite* SpriteManager::LoadSprite(const std::string &p_sFilename, const std::string &p_sSpritename,
			int p_iPosX, int p_iPosY, int p_iWidth, int p_iHeight,
			float p_fXScale, float p_fYScale)
		{
			if (m_axSprites.find(p_sSpritename) != m_axSprites.end())
				return m_axSprites.find(p_sSpritename)->second.get();

			m_axSprites.insert(std::pair<std::string, std::unique_ptr<sf::Sprite>>(p_sSpritename, std::make_unique<sf::Sprite>()));
			auto xIt = m_axSprites.find(p_sSpritename);
			sf::Texture* xTexture = LoadTexture(p_sFilename);
			if (xTexture)
				(*xIt).second->setTexture(*LoadTexture(p_sFilename));

			if (p_iWidth > 0 && p_iHeight > 0)
				(*xIt).second->setTextureRect(sf::IntRect(p_iPosX, p_iPosY, p_iWidth, p_iHeight));

			(*xIt).second->setScale(p_fXScale, p_fYScale);
			return (*xIt).second.get();
		}

		sf::Texture* SpriteManager::LoadTexture(const std::string &p_sFilename)
		{
			std::string sFilePath = Settings::m_sSpritePath + p_sFilename;

			auto it = m_axTextures.find(sFilePath);
			if (it == m_axTextures.end())
			{
				m_axTextures.insert(std::pair<const std::string, std::unique_ptr<sf::Texture>>(sFilePath, std::make_unique<sf::Texture>()));
				it = m_axTextures.find(sFilePath);
				if (!it->second->loadFromFile(sFilePath))
				{
					m_axTextures.erase(it);
					return nullptr;
				}
			}
			return it->second.get();
		}

		void SpriteManager::RemoveSprite(const std::string &p_sSpritename)
		{
			auto it = m_axSprites.find(p_sSpritename);
			if (it == m_axSprites.end())
			{
				return;
			}
			else
				m_axSprites.erase(it);
		}

		sf::Sprite* SpriteManager::GetSprite(const std::string &p_sSpritename)
		{
			auto it = m_axSprites.find(p_sSpritename);
			if (it == m_axSprites.end())
				return nullptr;
			else
				return it->second.get();
		}

		sf::Texture* SpriteManager::GetTexture(const std::string &p_sFilename)
		{
			std::string sFullFileName = Settings::m_sSpritePath + p_sFilename;
			auto it = m_axTextures.find(sFullFileName);
			if (it == m_axTextures.end())
				return nullptr;
			else
				return it->second.get();
		}
	}
}