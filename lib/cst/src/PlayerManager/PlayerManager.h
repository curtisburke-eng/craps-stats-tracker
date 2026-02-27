
#pragma once

#include <string>
#include <unordered_map>
#include <system_error>

#include "utils/DbConnection.h"

using PlayerId = uint32_t;

class PlayerManager
{
public:
    explicit PlayerManager(MysqlConnection conn);
    ~PlayerManager() = default;

    // Players (without a game)
    std::unordered_map<PlayerId, std::string>   GetAllAvailablePlayers();
    std::error_code                             CreateNewPlayer(std::string Name);
    std::error_code                             UpdatePlayerWinLoss(const PlayerId &PlayerId);
    std::error_code                             GetPlayerWinLoss(const PlayerId &PlayerId, int &WinLoss);


    // Players (with a game)
    std::unordered_map<PlayerId, std::string>   GetAllInGamePlayers(const uint32_t& GameId);
    std::error_code                             AddPlayerToGame(const PlayerId &PlayerId, const uint32_t &GameId);

    // Player Stats
    std::error_code     SetPlayerBuyIn(const PlayerId &PlayerId, const uint32_t &GameId, const int &Amount);
    std::error_code     AddPlayerBuyIn(const PlayerId &PlayerId, const uint32_t &GameId, const int &Amount);
    std::error_code     GetPlayerBuyIn(const PlayerId &PlayerId, const uint32_t &GameId, int &Amount);

    std::error_code     SetPlayerCashOut(const PlayerId &PlayerId, const uint32_t &GameId, const int &Amount);
    std::error_code     GetPlayerCashOut(const PlayerId &PlayerId, const uint32_t &GameId, int &Amount);

    std::error_code     UpdatePlayerAvgRollsPerShot(const PlayerId &PlayerId);
    std::error_code     GetPlayerAvgRollsPerShot(const PlayerId &PlayerId, int &AverageRollsPerShot);

    // Local
    std::error_code     SetCurrentShooter(const PlayerId &PlayerId, const uint32_t &GameId);
    PlayerId            GetCurrentShooter() { return m_shooterId; };

private:
    MysqlConnection     m_DbConnection;
    PlayerId            m_shooterId;
};
