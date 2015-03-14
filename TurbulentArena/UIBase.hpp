//UIBase.hpp

#pragma once

namespace bjoernligan
{
	class UIBase : public sf::Drawable
	{
		friend class UIManager;

	protected:
		UIBase(const std::string &p_sLabel, const float &p_fDepth);
		UIBase(const UIBase&);
		UIBase& operator=(const UIBase&);

	public:
		typedef std::unique_ptr<UIBase> Ptr;
		static Ptr Create(const std::string &p_sLabel, const float &p_fDepth);
		//* returns if used
		virtual bool Update(const float &p_fDeltaTime);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void SetPos(const sf::Vector2f &p_xPos);
		void AddSprite(sf::Sprite* p_xSprite);

	protected:
		sf::Vector2f m_xPos;
		std::string m_sLabel;

		std::vector<sf::Sprite*> m_axSprites;
		float m_fDepth;
	};
}