#pragma once

// #include "Player.h"
#include "utils/DbConnection.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <system_error>

using PlayerId = uint32_t;

class PlayerManager
{
public:
    explicit PlayerManager(MysqlConnection conn);
    ~PlayerManager() = default;

    std::unordered_map<PlayerId, std::string> GetAllAvailablePlayers();
    std::unordered_map<PlayerId, std::string> GetAllInGamePlayers();

    std::error_code     AddPlayerToGame(const PlayerId &PlayerId, const uint32_t &GameId);
    std::error_code     GetPlayerByName(std::string Name);
    std::error_code     GetPlayerById(PlayerId);
    std::error_code     CreateNewPlayer(std::string Name);

    std::error_code     SetPlayerBuyIn(PlayerId Id);
    int                 GetPlayerBuyIn(PlayerId Id);
    std::error_code     SetPlayerCashOut(PlayerId Id);
    int                 GetPlayerCashOut(PlayerId Id);

    std::error_code     SetPlayerNickname(PlayerId Id, std::string Nickname);
    std::string         GetPlayerNickname(PlayerId Id);

    std::error_code     SetCurrentShooter(PlayerId Id);
    PlayerId            GetCurrentShooter();

    void ResetInGamePlayers();

private:
    MysqlConnection                         m_DbConnection;
    std::unordered_map<PlayerId, std::string> m_Players;
    // PlayerId                                m_NextId;

    std::error_code     RecordBuyIn(PlayerId Id, int Amount);
    std::error_code     RecordCashOut(PlayerId Id, int Amount);
    std::size_t         GetPlayerCount() const;

    Player* FindPlayer(PlayerId Id);
};
