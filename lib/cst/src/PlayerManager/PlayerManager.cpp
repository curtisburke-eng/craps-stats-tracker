
#include "PlayerManager.h"

#include <cstdint>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>
#include <system_error>

#include "utils/CstErrorCodes.h"

using enum CstErrorCodes;

/**
 * @brief Constructor for PlayerManager. Initializes members.
 *        Receives a shared database connection from CrapsStatsTracker.
 */
PlayerManager::PlayerManager(MysqlConnection ConnectionPtr):
    m_DbConnection(ConnectionPtr)
{

};


/**
 * @brief Retrieves all players registered in the database.
 *
 * @return std::unordered_map<PlayerId, std::string> Map of player id to player name.
 */
std::unordered_map<PlayerId, std::string> PlayerManager::GetAllAvailablePlayers()
{
    SPDLOG_TRACE("PlayerManager::GetAllAvailablePlayers() called");

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
        SPDLOG_DEBUG("Player {}: {}",std::stoul(row[0]), row[1]);
        players.emplace(std::stoul(row[0]), row[1]);
    }
    mysql_free_result(result);
    return players;
}


/**
 * @brief Creates a new player record in the Players table.
 *
 * @param Name The display name of the new player.
 * @return std::error_code eOk on success, eQueryFailed on database error.
 */
std::error_code PlayerManager::CreateNewPlayer(std::string Name)
{
    // INSERT INTO Players name VALUES {}
    std::string query = fmt::format(
        "INSERT INTO Players (name) VALUES ('{}')",
        Name
    );

    // Send Query to database
    if (mysql_query(m_DbConnection.get(), query.c_str()) != 0)
    {
        SPDLOG_ERROR("Query failed: {}", mysql_error(m_DbConnection.get()));
        return eQueryFailed;
    }
    return eOk;
}


/**
 * @brief Recalculates and updates a player's overall win/loss in the Players table.
 *        Computes SUM(cashOut) - SUM(buyIn) across all games for the player.
 *
 * @param PlayerId The id of the player to update.
 * @return std::error_code eOk on success, eQueryFailed on database error.
 */
std::error_code PlayerManager::UpdatePlayerWinLoss(const PlayerId &PlayerId)
{
    // Query with sub-query - MySQL aggregates all the rows for that player in one pass and writes the result directly
    std::string query = fmt::format(
        "UPDATE Players "
        "SET winLoss = (SELECT SUM(cashOut) - SUM(buyIn) FROM PlayerGameStats WHERE PlayerGameStats.playerId = {}) "
        "WHERE Players.id = {}",
        PlayerId, PlayerId
    );

    // Send Query to database
    if (mysql_query(m_DbConnection.get(), query.c_str()) != 0)
    {
        SPDLOG_ERROR("Query failed: {}", mysql_error(m_DbConnection.get()));
        return eQueryFailed;
    }

    return eOk;
}


/**
 * @brief Retrieves the overall win/loss value for a player from the Players table.
 *
 * @param PlayerId The id of the player to query.
 * @param WinLoss Output parameter populated with the player's win/loss value.
 * @return std::error_code eOk on success, eQueryFailed on database error.
 */
std::error_code PlayerManager::GetPlayerWinLoss(const PlayerId &PlayerId, int &WinLoss)
{

    // SELECT winLoss FROM Players WHERE id = {};
    std::string query = fmt::format(
        "SELECT winLoss FROM Players WHERE id = {}",
        PlayerId
    );

    // Send Query to database
    if (mysql_query(m_DbConnection.get(), query.c_str()) != 0)
    {
        SPDLOG_ERROR("Query failed: {}", mysql_error(m_DbConnection.get()));
        return eQueryFailed;
    }

    MYSQL_RES* result = mysql_store_result(m_DbConnection.get());
    MYSQL_ROW row = mysql_fetch_row(result);
    WinLoss = std::stoul(row[0]);
    SPDLOG_DEBUG("Player {} Win/Loss: {}", PlayerId, WinLoss);

    mysql_free_result(result);
    return eOk;
}


/**
 * @brief Retrieves all players currently associated with a given game.
 *
 * @param GameId The id of the game to query.
 * @return std::unordered_map<PlayerId, std::string> Map of player id to player name for all players in the game.
 */
std::unordered_map<PlayerId, std::string> PlayerManager::GetAllInGamePlayers(const uint32_t& GameId)
{
    SPDLOG_TRACE("PlayerManager::GetAllInGamePlayers() called with argument GameId = {}",GameId);

    std::string query = fmt::format(
        "SELECT Players.id, Players.name FROM Players "
        "JOIN PlayerGameStats ON Players.id = PlayerGameStats.playerId "
        "WHERE PlayerGameStats.gameId = {}",
        GameId
    );

    // Query the database for all players
    if (mysql_query(m_DbConnection.get(), query.c_str()) != 0)
    {
        SPDLOG_ERROR("Query failed: {}", mysql_error(m_DbConnection.get()));
        return {};
    }

    MYSQL_RES* result = mysql_store_result(m_DbConnection.get());
    MYSQL_ROW row;
    std::unordered_map<uint32_t, std::string> players;
    while ((row = mysql_fetch_row(result)) != nullptr)
    {
        SPDLOG_DEBUG("Player {}: {}",std::stoul(row[0]), row[1]);
        players.emplace(std::stoul(row[0]), row[1]);
    }
    mysql_free_result(result);
    return players;
}


/**
 * @brief Associate a player to a game.
 *
 * @return std::error_code if a player with that id is already assigned to the game or query fails.
 */
std::error_code PlayerManager::AddPlayerToGame(const PlayerId& PlayerId, const uint32_t& GameId)
{
    SPDLOG_TRACE("PlayerManager::AddPlayerToGame() called with argument PlayerId = {} and GameId = {}", PlayerId, GameId);

    auto inGamePlayers = GetAllInGamePlayers(GameId);
    if (inGamePlayers.contains(PlayerId)) {
        SPDLOG_ERROR("Player already associatied with this game.");
        return ePlayerActive;
    }

    // INSERT INTO PlayerGameStats (gameId, playerId, buyIn, cashOut, averageRollsPerShot) VALUES ({},{}, 0.00, 0.00, 0.00);
    std::string query = fmt::format(
        "INSERT INTO PlayerGameStats (gameId, playerId, buyIn, cashOut, averageRollsPerShot)"
        "VALUES ({}, {}, 0.00, 0.00, 0.00)",
        GameId, PlayerId
    );

    // Send Query to database
    if (mysql_query(m_DbConnection.get(), query.c_str()) != 0)
    {
        SPDLOG_ERROR("Query failed: {}", mysql_error(m_DbConnection.get()));
        return eQueryFailed;
    }

    return eOk;
}


/**
 * @brief Sets (replaces) the buy-in amount for a player in a game.
 *
 * @param PlayerId The id of the player.
 * @param GameId The id of the game.
 * @param Amount The buy-in amount to set. Must be positive.
 * @return std::error_code eOk on success, eInvalidArgument if Amount <= 0,
 *         ePlayerNotFound if player is not in the game, eQueryFailed on database error.
 */
std::error_code PlayerManager::SetPlayerBuyIn(const PlayerId &PlayerId, const uint32_t &GameId, const int &Amount)
{
    if (Amount <= 0.0)
    {
        SPDLOG_ERROR("Buy-in amount must be positive");
        return eInvalidArgument;
    }

    auto inGamePlayers = GetAllInGamePlayers(GameId);
    if (!inGamePlayers.contains(PlayerId)) {
        SPDLOG_ERROR("Player is not associatied with this game.");
        return ePlayerNotFound;
    }

    // UPDATE PlayerGameStats SET buyIn = {} WHERE playerId = {} AND gameId = {};
    std::string query = fmt::format(
        "UPDATE PlayerGameStats SET buyIn = {} WHERE playerId = {} AND gameId = {}",
        Amount, PlayerId, GameId
    );

    // Send Query to database
    if (mysql_query(m_DbConnection.get(), query.c_str()) != 0)
    {
        SPDLOG_ERROR("Query failed: {}", mysql_error(m_DbConnection.get()));
        return eQueryFailed;
    }

    return eOk;
}


/**
 * @brief Adds to the buy-in amount for a player in a game. Accumulates across multiple buy-ins.
 *
 * @param PlayerId The id of the player.
 * @param GameId The id of the game.
 * @param Amount The buy-in amount to add. Must be positive.
 * @return std::error_code eOk on success, eInvalidArgument if Amount <= 0,
 *         ePlayerNotFound if player is not in the game, eQueryFailed on database error.
 */
std::error_code PlayerManager::AddPlayerBuyIn(const PlayerId &PlayerId, const uint32_t &GameId, const int &Amount)
{
    if (Amount <= 0.0)
    {
        SPDLOG_ERROR("Buy-in amount must be positive");
        return eInvalidArgument;
    }

    auto inGamePlayers = GetAllInGamePlayers(GameId);
    if (!inGamePlayers.contains(PlayerId)) {
        SPDLOG_ERROR("Player is not associatied with this game.");
        return ePlayerNotFound;
    }

    // UPDATE PlayerGameStats SET buyIn = buyIn + {} WHERE playerId = {} AND gameId = {};
    std::string query = fmt::format(
        "UPDATE PlayerGameStats SET buyIn = buyIn + {} WHERE playerId = {} AND gameId = {}",
        Amount, PlayerId, GameId
    );

    // Send Query to database
    if (mysql_query(m_DbConnection.get(), query.c_str()) != 0)
    {
        SPDLOG_ERROR("Query failed: {}", mysql_error(m_DbConnection.get()));
        return eQueryFailed;
    }

    return eOk;
}

/**
 * @brief Retrieves the buy-in amount for a player in a game.
 *
 * @param PlayerId The id of the player.
 * @param GameId The id of the game.
 * @param Amount Output parameter populated with the player's buy-in amount.
 * @return std::error_code eOk on success, ePlayerNotFound if player is not in the game,
 *         eQueryFailed on database error.
 */
std::error_code PlayerManager::GetPlayerBuyIn(const PlayerId &PlayerId, const uint32_t &GameId, int& Amount)
{

    auto inGamePlayers = GetAllInGamePlayers(GameId);
    if (!inGamePlayers.contains(PlayerId)) {
        SPDLOG_ERROR("Player is not associatied with this game.");
        return ePlayerNotFound;
    }

    // SELECT buyIn FROM PlayerGameStats WHERE playerId = {} AND gameId = {};
    std::string query = fmt::format(
        "SELECT buyIn FROM PlayerGameStats WHERE playerId = {} AND gameId = {}",
        PlayerId, GameId
    );

    // Send Query to database
    if (mysql_query(m_DbConnection.get(), query.c_str()) != 0)
    {
        SPDLOG_ERROR("Query failed: {}", mysql_error(m_DbConnection.get()));
        return eQueryFailed;
    }

    MYSQL_RES* result = mysql_store_result(m_DbConnection.get());
    MYSQL_ROW row = mysql_fetch_row(result);
    Amount = std::stoul(row[0]);
    SPDLOG_DEBUG("Player {} Buy In: {}", PlayerId, Amount);

    mysql_free_result(result);
    return eOk;
}


/**
 * @brief Sets the cash-out amount for a player in a game.
 *
 * @param PlayerId The id of the player.
 * @param GameId The id of the game.
 * @param Amount The cash-out amount to set. Must be positive.
 * @return std::error_code eOk on success, eInvalidArgument if Amount <= 0,
 *         ePlayerNotFound if player is not in the game, eQueryFailed on database error.
 */
std::error_code PlayerManager::SetPlayerCashOut(const PlayerId &PlayerId, const uint32_t &GameId, const int &Amount)
{
    if (Amount <= 0.0)
    {
        SPDLOG_ERROR("Cash-out amount must be positive");
        return eInvalidArgument;
    }

    auto inGamePlayers = GetAllInGamePlayers(GameId);
    if (!inGamePlayers.contains(PlayerId)) {
        SPDLOG_ERROR("Player is not associatied with this game.");
        return ePlayerNotFound;
    }

    // UPDATE PlayerGameStats SET cashOut = {} WHERE playerId = {} AND gameId = {};
    std::string query = fmt::format(
        "UPDATE PlayerGameStats SET cashOut = {} WHERE playerId = {} AND gameId = {}",
        Amount, PlayerId, GameId
    );

    // Send Query to database
    if (mysql_query(m_DbConnection.get(), query.c_str()) != 0)
    {
        SPDLOG_ERROR("Query failed: {}", mysql_error(m_DbConnection.get()));
        return eQueryFailed;
    }

    return eOk;
}


/**
 * @brief Retrieves the cash-out amount for a player in a game.
 *
 * @param PlayerId The id of the player.
 * @param GameId The id of the game.
 * @param Amount Output parameter populated with the player's cash-out amount.
 * @return std::error_code eOk on success, ePlayerNotFound if player is not in the game,
 *         eQueryFailed on database error.
 */
std::error_code PlayerManager::GetPlayerCashOut(const PlayerId &PlayerId, const uint32_t &GameId, int &Amount)
{
    auto inGamePlayers = GetAllInGamePlayers(GameId);
    if (!inGamePlayers.contains(PlayerId)) {
        SPDLOG_ERROR("Player is not associatied with this game.");
        return ePlayerNotFound;
    }

    // SELECT cashOut FROM PlayerGameStats WHERE playerId = {} AND gameId = {};
    std::string query = fmt::format(
        "SELECT cashOut FROM PlayerGameStats WHERE playerId = {} AND gameId = {}",
        PlayerId, GameId
    );

    // Send Query to database
    if (mysql_query(m_DbConnection.get(), query.c_str()) != 0)
    {
        SPDLOG_ERROR("Query failed: {}", mysql_error(m_DbConnection.get()));
        return eQueryFailed;
    }

    MYSQL_RES* result = mysql_store_result(m_DbConnection.get());
    MYSQL_ROW row = mysql_fetch_row(result);
    Amount = std::stoul(row[0]);
    SPDLOG_DEBUG("Player {} Cash Out: {}", PlayerId, Amount);

    mysql_free_result(result);
    return eOk;
}


/**
 * @brief
 *
 * @param PlayerId
 * @return std::error_code
 */
std::error_code PlayerManager::UpdatePlayerAvgRollsPerShot(const PlayerId &PlayerId)
{
    //TODO: Implement
    return eOk;
}


/**
 * @brief
 *
 * @param PlayerId
 * @param AverageRollsPerShot
 * @return std::error_code
 */
std::error_code PlayerManager::GetPlayerAvgRollsPerShot(const PlayerId &PlayerId, int &AverageRollsPerShot)
{
    //TODO: Implement
    return eOk;
}


/**
 * @brief Sets the current shooter for the active game.
 *
 * @param PlayerId The id of the player who is now shooting.
 * @param GameId The id of the game being played.
 * @return std::error_code eOk on success, ePlayerNotFound if player is not in the game.
 */
std::error_code PlayerManager::SetCurrentShooter(const PlayerId &PlayerId, const uint32_t &GameId)
{
    auto inGamePlayers = GetAllInGamePlayers(GameId);
    if (!inGamePlayers.contains(PlayerId)) {
        SPDLOG_ERROR("Player is not associatied with this game.");
        return ePlayerNotFound;
    }

    m_shooterId = PlayerId;
    return eOk;
}