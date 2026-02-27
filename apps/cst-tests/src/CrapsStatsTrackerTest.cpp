
#include "CrapsStatsTrackerTest.h"

#include <gtest/gtest.h>
#include <system_error>
#include "CrapsStatsTracker.h"

// --------------------- CONSTRUCTION TESTS ---------------------

TEST_F(CrapsStatsTrackerTest, Constructor_CreatesValidInstance)
{
    ASSERT_NO_THROW(CrapsStatsTracker());
}

TEST_F(CrapsStatsTrackerTest, Constructor_CreatesPlayerManager)
{
    ASSERT_NE(m_Cst->GetPlayerManager(), nullptr);
}

// TEST_F(CrapsStatsTrackerTest, Constructor_CreatesRollCounter)
// {
//     ASSERT_NE(m_Cst->GetRollCounter(), nullptr);
// }

// TEST_F(CrapsStatsTrackerTest, Constructor_CreatesBonusTracker)
// {
//     ASSERT_NE(m_Cst->GetBonusTracker(), nullptr);
// }

// --------------------- DATABASE TESTS ---------------------

TEST_F(CrapsStatsTrackerTest, DatabaseConnected)
{
    // Connect is called from the SetUp function
    ASSERT_TRUE(m_Cst->GetIsConnected());
}

TEST_F(CrapsStatsTrackerTest, DisconnectFromDatabase)
{
    ASSERT_TRUE(m_Cst->GetIsConnected());

    m_Cst->DisconnectFromDatabase();

    ASSERT_FALSE(m_Cst->GetIsConnected());
}