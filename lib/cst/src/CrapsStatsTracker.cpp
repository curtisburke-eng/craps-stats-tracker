
#include "CrapsStatsTracker.h"

#include <spdlog/spdlog.h>
#include <system_error>

#include "PlayerManager/PlayerManager.h"
// #include "BonusTracker/BonusTracker.h"
// #include "RollCounter/RollCounter.h"
#include "utils/CstErrorCodes.h"

using enum CstErrorCodes;


/**
 * @brief Constructor for CrapsStatsTracker. Initializes members
 */
CrapsStatsTracker::CrapsStatsTracker() :
    m_isConnected(false)
{

}


/**
 * @brief Destructor. Closes the database connection if open
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

    const std::string host     = "127.0.0.1"; // "127.0.0.1" forces TCP, which is required for Docker-hosted databases
    const std::string user     = "root";
    const std::string password = "password";
    const std::string database = "cst_test";
    const unsigned int port    = 3306u;


    SPDLOG_DEBUG("Connecting to database {} at {}:{} as '{}'", database, host, port, user);

    // Initialise the MySQL client
    MYSQL* rawConnectionPtr = mysql_init(nullptr);
    if (!rawConnectionPtr)
    {
        SPDLOG_ERROR("mysql_init() failed — out of memory");
        m_isConnected = false;
        return eDatabaseDisconnected;
    }

    // Attempt the connection
    MYSQL* conn = mysql_real_connect(
        rawConnectionPtr,
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
        SPDLOG_ERROR("mysql_real_connect() failed: {}", mysql_error(rawConnectionPtr));
        mysql_close(rawConnectionPtr);
        m_isConnected = false;
        return eDatabaseDisconnected;
    }

    // Wrap the rawConnectionPtr so mysql_close() will be called on last release
    m_DbConnection = MysqlConnection(rawConnectionPtr, MysqlDeleter{});

    // Construct child managers with a valid connection
    m_PlayerManager = std::make_shared<PlayerManager>(m_DbConnection);
    // m_BonusTracker = std::make_shared<BonusTracker>(m_DbConnection);
    // m_RollCounter  = std::make_shared<RollCounter>(m_DbConnection);

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

    m_PlayerManager.reset();
    // m_BonusTracker.reset();
    // m_RollCounter.reset();

    m_DbConnection.reset(); // triggers MysqlDeleter -> mysql_close()
    m_isConnected = false;

    SPDLOG_INFO("Database connection closed");
    return eOk;

}
