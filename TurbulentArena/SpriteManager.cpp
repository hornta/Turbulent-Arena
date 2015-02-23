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

		void SpriteManager::setTexturePath(const std::string& path)
		{
			m_texturePath = path;
		}

		sf::Sprite* SpriteManager::LoadSprite(const std::string &p_sFilename, const std::string &p_sSpritename,
			int p_iPosX, int p_iPosY, int p_iWidth, int p_iHeight,
			float p_fXScale, float p_fYScale)
		{
			if (m_axSprites.find(p_sSpritename) != m_axSprites.end())
				return m_axSprites.find(p_sSpritename)->second.get();

			m_axSprites.insert(std::pair<std::string, std::unique_ptr<sf::Sprite>>(p_sSpritename, std::make_unique<sf::Sprite>()));
			auto xIt = m_axSprites.find(p_sSpritename);
			sf::Texture* xTexture = GetTexture(p_sFilename);
			if (xTexture)
				(*xIt).second->setTexture(*GetTexture(p_sFilename));

			if (p_iWidth > 0 && p_iHeight > 0)
				(*xIt).second->setTextureRect(sf::IntRect(p_iPosX, p_iPosY, p_iWidth, p_iHeight));

			(*xIt).second->setScale(p_fXScale, p_fYScale);
			return (*xIt).second.get();
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
			auto it = m_axTextures.find(p_sFilename);
			if (it == m_axTextures.end())
			{
				std::string texturePath = m_texturePath + p_sFilename;
				m_axTextures.insert(std::make_pair(p_sFilename, std::make_unique<sf::Texture>()));
				if (!m_axTextures[p_sFilename]->loadFromFile(texturePath))
				{
					return nullptr;
				}
				else
					return m_axTextures[p_sFilename].get();
			}
			else
			{
				return it->second.get();
			}
		}
	}
}