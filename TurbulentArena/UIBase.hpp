//UIBase.hpp

#pragma once

namespace bjoernligan
{
	class UIBase
	{
		friend class UIManager;

	protected:
		UIBase(sf::RenderWindow* p_xWindow, const float &p_fDepth);
		UIBase(const UIBase&);
		UIBase& operator=(const UIBase&);

	public:
		typedef std::unique_ptr<UIBase> Ptr;
		static Ptr Create(sf::RenderWindow* p_xWindow, const float &p_fDepth);

		virtual ~UIBase();

		virtual void Update(const float &p_fDeltaTime);
		virtual void Draw();
		virtual void SetPos(const sf::Vector2f &p_xPos);
		void AddSprite(sf::Sprite* p_xSprite);

	protected:
		sf::RenderWindow* m_xWindow;
		sf::Vector2f m_xPos;

		std::vector<sf::Sprite*> m_axSprites;
		float m_fDepth;
	};
}