#pragma once

#include <memory>
#include <mysql/mysql.h>

/**
 * @brief Custom deleter for MYSQL*
 *        Ensures mysql_close() is called instead of delete
 */
struct MysqlDeleter
{
    void operator()(MYSQL* conn) const
    {
        if (conn)
            mysql_close(conn);
    }
};

// Shared ownership handle for a MySQL connection
using MysqlConnection = std::shared_ptr<MYSQL>;
