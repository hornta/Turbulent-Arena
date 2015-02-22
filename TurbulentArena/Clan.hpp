#pragma once

#include "SliderBridge.hpp"

namespace bjoernligan
{
	class Class;
	class Clan : public sf::Drawable
	{
	public:
		enum ClassType
		{
			AXEMAN,
			BOWMAN,
			GATHERER,
			SCOUT
		};

		struct ClanDef
		{
			std::string name;
			sf::Color color;
			int amountScouts;
			int amountAxemen;
			int amountBowmen;
			int amountGatherers;
		};

		Clan(const std::string& name);
		~Clan();

		void createMember();

		void Update(const float &p_fDeltaTime);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Color getColor() const;

		void AddSliderBridge(SliderBridge* p_xBridge);

	private:
		sf::Color m_color;
		std::string m_name;
		
		std::vector<Class*> m_members;
		std::vector<SliderBridge*> m_xBridges;
	};
}