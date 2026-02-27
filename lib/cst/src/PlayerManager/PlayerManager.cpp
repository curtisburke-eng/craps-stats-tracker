
#include "PlayerManager.h"
// #include "PlayerManager/Player.h"
#include "utils/CstErrorCodes.h"

#include <cstdint>
#include <memory>
#include <spdlog/spdlog.h>
#include <system_error>

using enum CstErrorCodes;

/**
 * @brief Constructor for PlayerManager. Initializes members.
 *        Receives a shared database connection from CrapsStatsTracker.
 */
PlayerManager::PlayerManager(MysqlConnection ConnectionPtr):
    m_DbConnection(ConnectionPtr)
{

};


std::unordered_map<PlayerId, std::string> PlayerManager::GetAllAvailablePlayers()
{
    // Query the database for all players
    if (mysql_query(m_DbConnection.get(), "SELECT id, name FROM Players") != 0)
    {
        SPDLOG_ERROR("Query failed: {}", mysql_error(m_DbConnection.get()));
        return {};
    }

    MYSQL_RES* result = mysql_store_result(m_DbConnection.get());
    MYSQL_ROW row;
    std::unordered_map<uint32_t, std::string> players;
    while ((row = mysql_fetch_row(result)) != nullptr)
    {
        players.emplace(std::stoul(row[0]), row[1]);
    }
    mysql_free_result(result);
    return players;
}


std::unordered_map<PlayerId, std::string> PlayerManager::GetAllInGamePlayers()
{
    // TODO: Update to use a db query
    return m_Players;
}


/**
 * @brief Add a new player by name.
 *
 * @return std::error_code if a player with that id is already assigned to the game.
 */
std::error_code PlayerManager::AddPlayerToGame(const PlayerId& PlayerId, const uint32_t& GameId)
{
    // INSERT INTO PlayerGameStats (gameId, playerId, buyIn, cashOut, averageRollsPerShot) VALUES (1, 1, 100.00, 0.00, 0.00);

    return eOk;
}
