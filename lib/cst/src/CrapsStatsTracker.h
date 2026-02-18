#pragma once

#include <memory>

class PlayerManager;
class BonusTracker;
class RollCounter;

class CrapsStatsTracker
{

public:
    CrapsStatsTracker();
    ~CrapsStatsTracker() = default;

private:
    std::shared_ptr<PlayerManager>  m_PlayerManager;
    std::shared_ptr<BonusTracker>   m_BonusTracker;
    std::shared_ptr<RollCounter>    m_RollCounter;
};
