#pragma once

#include <memory>
#include <system_error>

#include "utils/DbConnection.h"

class PlayerManager;
// class BonusTracker;
// class RollCounter;


class CrapsStatsTracker
{
public:
    CrapsStatsTracker();
    ~CrapsStatsTracker();

    std::error_code ConnectToDatabase();
    std::error_code DisconnectFromDatabase();

    bool GetIsConnected() const { return m_isConnected; }

    std::shared_ptr<PlayerManager> GetPlayerManager() { return m_PlayerManager; };
    // std::shared_ptr<BonusTracker>  GetBonusTracker() { return m_BonusTracker; };
    // std::shared_ptr<RollCounter>   GetRollCounter() { return m_RollCounter; };

private:
    std::shared_ptr<PlayerManager>    m_PlayerManager;
    // std::shared_ptr<BonusTracker>  m_BonusTracker;
    // std::shared_ptr<RollCounter>   m_RollCounter;

    bool             m_isConnected;
    MysqlConnection  m_DbConnection;

};
