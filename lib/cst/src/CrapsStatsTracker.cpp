
#include "CrapsStatsTracker.h"

#include "PlayerManager.h"
#include "BonusTracker.h"
#include "RollCounter.h"

/**
 * @brief Constructor for CrapsStatsTracker. Initializes members.
 */
CrapsStatsTracker::CrapsStatsTracker() :
    m_PlayerManager(std::make_shared<PlayerManager>()),
    m_BonusTracker(std::make_shared<BonusTracker>()),
    m_RollCounter(std::make_shared<RollCounter>())
{

}

