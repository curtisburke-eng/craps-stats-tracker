#pragma once

#include <mysql/mysql.h>
#include <string>
#include <system_error>
// #include <vector>
// #include <optional>

// #include "PlayerManager/Player.h"
#include "utils/GameState.h"

// class PlayerManager;
// class BonusTracker;
// class RollCounter;

struct st_mysql;

using enum GameState;


class CrapsStatsTracker
{
public:
    CrapsStatsTracker();
    ~CrapsStatsTracker();

    std::error_code ConnectToDatabase();
    std::error_code DisconnectFromDatabase();

    bool GetIsConnected() const { return m_isConnected; }

    // void Run(bool &Active);

    // std::error_code SetGameState(GameState State);
    // GameState GetGameState();

    // std::shared_ptr<PlayerManager> GetPlayerManager() { return m_PlayerManager; };
    // std::shared_ptr<BonusTracker>  GetBonusTracker() { return m_BonusTracker; };
    // std::shared_ptr<RollCounter>   GetRollCounter() { return m_RollCounter; };

private:
//     GameState                         m_GameState = GameState::Idle;
//     std::shared_ptr<PlayerManager>    m_PlayerManager;
//     std::shared_ptr<BonusTracker>     m_BonusTracker;
//     std::shared_ptr<RollCounter>      m_RollCounter;

//     int m_Point;
    bool    m_isConnected;
    MYSQL*  m_DbConnection;

};
