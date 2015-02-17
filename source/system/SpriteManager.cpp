//SpriteManager.cpp

#include "stdafx.h"
#include <system\SpriteManager.hpp>
#include <Settings.hpp>
#include <fstream>

namespace bjoernligan
{
	namespace system
	{
		SpriteManager::SpriteManager()
		{

		}

		SpriteManager::~SpriteManager()
		{
			CleanUpSprites();
			{
				auto it = m_axTextures.begin();
				while (it != m_axTextures.end())
				{
					delete it->second;
					it->second = nullptr;
					it++;
				}
				m_axTextures.clear();
			}
		}

		SpriteManager::Ptr SpriteManager::Create()
		{
			return Ptr(new SpriteManager);
		}

		void SpriteManager::CleanUpSprites()
		{
			if (!m_axSprites.empty())
			{
				auto it = m_axSprites.begin();
				while (it != m_axSprites.end())
				{
					delete it->second;
					it->second = nullptr;
					it++;
				}
				m_axSprites.clear();
			}
		}

		sf::Sprite* SpriteManager::LoadSprite(const std::string &p_sFilename, const std::string &p_sSpritename,
			int p_iPosX, int p_iPosY, int p_iWidth, int p_iHeight,
			float p_fXScale, float p_fYScale)
		{
			if (m_axSprites.find(p_sSpritename) != m_axSprites.end())
				return m_axSprites.find(p_sSpritename)->second;

			m_axSprites.insert(std::pair<std::string, sf::Sprite*>(p_sSpritename, new sf::Sprite));
			auto xIt = m_axSprites.find(p_sSpritename);
			sf::Texture* xTexture = LoadTexture(p_sFilename);
			if (xTexture)
				(*xIt).second->setTexture(*LoadTexture(p_sFilename));

			if (p_iWidth > 0 && p_iHeight > 0)
				(*xIt).second->setTextureRect(sf::IntRect(p_iPosX, p_iPosY, p_iWidth, p_iHeight));

			(*xIt).second->setScale(p_fXScale, p_fYScale);
			return (*xIt).second;
		}

		sf::Texture* SpriteManager::LoadTexture(const std::string &p_sFilename)
		{
			std::string sFilePath = Settings::m_sSpritePath + p_sFilename;

			auto it = m_axTextures.find(sFilePath);
			if (it == m_axTextures.end())
			{
				m_axTextures.insert(std::pair<const std::string, sf::Texture*>(sFilePath, new sf::Texture));
				it = m_axTextures.find(sFilePath);
				if (!it->second->loadFromFile(sFilePath))
				{
					delete it->second;
					it->second = nullptr;
					m_axTextures.erase(it);
					return nullptr;
				}
			}
			return it->second;
		}

		void SpriteManager::RemoveSprite(const std::string &p_sSpritename)
		{
			auto it = m_axSprites.find(p_sSpritename);
			if (it == m_axSprites.end())
			{
				return;
			}
			else
			{
				delete it->second;
				it->second = nullptr;
				m_axSprites.erase(it);
			}
		}

		sf::Sprite* SpriteManager::GetSprite(const std::string &p_sSpritename)
		{
			auto it = m_axSprites.find(p_sSpritename);
			if (it == m_axSprites.end())
				return nullptr;
			else
				return it->second;
		}

		sf::Texture* SpriteManager::GetTexture(const std::string &p_sFilename)
		{
			std::string sFullFileName = Settings::m_sSpritePath + p_sFilename;
			auto it = m_axTextures.find(sFullFileName);
			if (it == m_axTextures.end())
				return nullptr;
			else
				return it->second;
		}
	}
}