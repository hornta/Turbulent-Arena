//Axeman.cpp

#include "stdafx.h"
#include "Scout.hpp"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "BNodesInclude.hpp"
#include "BFindTargetNode.hpp"
#include "Clan.hpp"

namespace bjoernligan
{
	Scout::EnlightendFriend::EnlightendFriend(bool enlightend) :
		enlightend(enlightend)
	{

	}

	Scout::Scout(ai::Sense* sense, const sf::Color &p_xTeamColor, Clan* p_xClan)
		: ClanMember(sense, p_xTeamColor, p_xClan)
	{
	}

	void Scout::initiate()
	{
		m_eClass = EClass::Scout;

		m_xCombatStats.Initiate(1, 5);
		m_MovementStats.Initiate(sf::Vector2f(550, 550.0f));

		ai::BehaviorTree* xBT = m_xAgent->getBehaviorTree();

		ai::BSelectorNode* xRootSelector = xBT->CreateRoot<ai::BSelectorNode>();
		xRootSelector;

		// level 2
		ai::BSequenceNode* reportSequence = xRootSelector->AddChild<ai::BSequenceNode>();
		xRootSelector->AddChild<ai::DetectEnemies>()->AttachAgent(m_xAgent.get());
		xRootSelector->AddChild<ai::Explore>()->AttachAgent(m_xAgent.get());

		// level 3
		reportSequence->AddChild<ai::HasSomethingToReport>()->AttachAgent(m_xAgent.get());
		reportSequence->AddChild<ai::ReportToFriends>()->AttachAgent(m_xAgent.get());
	}

	void Scout::update(float deltatime)
	{
		ClanMember::update(deltatime);
		
		// Update friends vector
		std::vector<ClanMember*> members = m_clan->getMembers();
		if (m_enlightendFriends.empty())
		{
			for (std::size_t i = 0; i < members.size(); ++i)
			{
				if (members[i] != this)
				{
					m_enlightendFriends.emplace_back(std::make_unique<EnlightendFriend>(false));
					EnlightendFriend* f = m_enlightendFriends.back().get();
					f->agent = std::make_unique<ai::SenseAgentData>(members[i]->getAgent());
				}
			}
		}
		
		for (std::size_t i = 0; i < m_enlightendFriends.size(); ++i)
		{
			m_enlightendFriends[i]->agent.release();
			m_enlightendFriends[i]->agent = std::make_unique<ai::SenseAgentData>(members[i]->getAgent());
		}

		for (std::size_t i = 0; i < m_enlightendFriends.size(); ++i)
		{
			// Check if we are close to friend to report to him =)
			sf::Vector2f p0 = m_enlightendFriends[i]->agent->m_agent->getOwner()->getSprite()->getPosition();
			sf::Vector2f p1 = getSprite()->getPosition();

			if (Vector2f::dist(Vector2f(p0), Vector2f(p1)) <= 40.f)
			{
				m_enlightendFriends[i]->enlightend = true;
			}
		}
	}
}