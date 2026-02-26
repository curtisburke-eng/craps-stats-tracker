
#include "CrapsStatsTracker.h"

#include <mysql/mysql.h>
#include <spdlog/spdlog.h>
#include <system_error>

// #include "PlayerManager/PlayerManager.h"
// #include "BonusTracker/BonusTracker.h"
// #include "RollCounter/RollCounter.h"
#include "utils/CstErrorCodes.h"

using enum CstErrorCodes;


/**
 * @brief Constructor for CrapsStatsTracker. Initializes members.
 */
CrapsStatsTracker::CrapsStatsTracker() :
    // m_PlayerManager(std::make_shared<PlayerManager>()),
    // m_BonusTracker(std::make_shared<BonusTracker>()),
    // m_RollCounter(std::make_shared<RollCounter>()),
    m_isConnected(false),
    m_DbConnection(nullptr)
{

}


/**
 * @brief Destructor. Closes the database connection if open.
 */
CrapsStatsTracker::~CrapsStatsTracker()
{
    DisconnectFromDatabase();
}


/**
 * @brief Connect to a MySQL database
 *
 * @return error_code
 */
std::error_code CrapsStatsTracker::ConnectToDatabase()
{
    if (m_isConnected)
    {
        SPDLOG_WARN("Already connected to Database — call DisconnectFromDatabase() first");
        return eDatabaseConnected;
    }

    const std::string host     = "127.0.0.1"; // "127.0.0.1" forces TCP, which is required for Docker-hosted databases.
    const std::string user     = "root";
    const std::string password = "password";
    const std::string database = "cst_test";
    const unsigned int port    = 3306u;


    SPDLOG_DEBUG("Connecting to database {} at {}:{} as '{}'", database, host, port, user);

    // Initialise the MySQL client
    m_DbConnection = mysql_init(nullptr);
    if (!m_DbConnection)
    {
        SPDLOG_ERROR("mysql_init() failed — out of memory");
        m_isConnected = false;
        return eDatabaseDisconnected;
    }

    // Attempt the connection
    MYSQL* conn = mysql_real_connect(
        m_DbConnection,
        host.empty()     ? nullptr : host.c_str(),
        user.empty()     ? nullptr : user.c_str(),
        password.empty() ? nullptr : password.c_str(),
        database.empty() ? nullptr : database.c_str(),
        port,
        nullptr,  // Unix socket path (nullptr = use TCP)
        0         // Client flags
    );

    if (!conn)
    {
        SPDLOG_ERROR("mysql_real_connect() failed: {}", mysql_error(m_DbConnection));
        mysql_close(m_DbConnection);
        m_DbConnection = nullptr;
        m_isConnected = false;
        return eDatabaseDisconnected;
    }

    SPDLOG_INFO("Connected to {} at {}:{}", database, host, port);
    m_isConnected = true;
    return eOk;
}

/**
 * @brief Close the active database connection
 */
std::error_code CrapsStatsTracker::DisconnectFromDatabase()
{
    if (!m_DbConnection)
    {
        SPDLOG_ERROR("Database connection already closed");
        return eDatabaseDisconnected;
    }

    mysql_close(m_DbConnection);
    m_DbConnection = nullptr;
    m_isConnected  = false;

    SPDLOG_INFO("Database connection closed");
    return eOk;

}
