//Axeman.cpp

#include "stdafx.h"
#include "Scout.hpp"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "BNodesInclude.hpp"
#include "BFindTargetNode.hpp"
#include "Clan.hpp"
#include "Axeman.hpp"
#include "AudioManager.hpp"

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

	Scout::~Scout()
	{
	}

	void Scout::RemoveEnlightenedFriend(ai::Agent* p_xAgent)
	{
		if (p_xAgent->getOwner()->GetClass() == ClanMember::EClass::EClassScout)
			return;

		auto enl_itr = m_enlightendFriends.begin();
		while (enl_itr != m_enlightendFriends.end())
		{
			if ((*enl_itr)->agent->m_agent == p_xAgent)
				enl_itr = m_enlightendFriends.erase(enl_itr);
			else
				++enl_itr;
		}
	}

	void Scout::initiate()
	{
		m_eClass = EClass::EClassScout;

		m_xCombatStats.Initiate(1, 5);
		m_MovementStats.Initiate(0.0f);

		ai::BehaviorTree* xBT = m_xAgent->getBehaviorTree();

		ai::BSelectorNode* xRootSelector = xBT->CreateRoot<ai::BSelectorNode>();
		xRootSelector;

		// level 2
		//ai::BSequenceNode* attackSequence = xRootSelector->AddChild<ai::BSequenceNode>();
		ai::BSequenceNode* reportSequence = xRootSelector->AddChild<ai::BSequenceNode>();
		xRootSelector->AddChild<ai::DetectEnemies>()->AttachAgent(m_xAgent.get());
		xRootSelector->AddChild<ai::Explore>()->AttachAgent(m_xAgent.get());

		// level 3
		//attackSequence->AddChild<ai::BIsScared>()->AttachAgent(m_xAgent.get());
		//attackSequence->AddChild<
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
				if (members[i] != this && members[i]->GetClass() == EClass::EClassAxeman)
				{
					m_enlightendFriends.emplace_back(std::make_unique<EnlightendFriend>(true));
					EnlightendFriend* f = m_enlightendFriends.back().get();

					if (members[i]->GetCombat()->Alive())
					{
						f->agent.reset();
						f->agent = std::make_unique<ai::SenseAgentData>(members[i]->getAgent());
					}
				}
			}
		}

		for (std::size_t i = 0; i < m_enlightendFriends.size(); ++i)
		{
			m_enlightendFriends[i]->agent.reset();
			m_enlightendFriends[i]->agent = std::make_unique<ai::SenseAgentData>(members[i]->getAgent());
		}

		for (std::size_t i = 0; i < m_enlightendFriends.size(); ++i)
		{
			if (m_enlightendFriends[i]->agent.get())
			{
				if (!m_enlightendFriends[i]->enlightend)
				{
					// Check if we are close to friend to report to him =)
					sf::Vector2f p0 = m_enlightendFriends[i]->agent->m_agent->getOwner()->getSprite()->getPosition();
					sf::Vector2f p1 = getSprite()->getPosition();

					if (Vector2f::dist(Vector2f(p0), Vector2f(p1)) <= 40.f)
					{
						m_enlightendFriends[i]->enlightend = true;

						bjoernligan::Axeman* axeman = static_cast<bjoernligan::Axeman*>(m_enlightendFriends[i]->agent->m_agent->getOwner());

						for (std::size_t k = 0; k < m_enlightendFriends[i]->toReport.size(); ++k)
						{
							axeman->m_incomingReports.push_back(std::move(m_enlightendFriends[i]->toReport[k]));
						}
						m_enlightendFriends[i]->toReport.clear();
					}
				}
			}
		}
	}
}