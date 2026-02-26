
#include "CrapsStatsTrackerTest.h"

#include <gtest/gtest.h>
#include <system_error>
#include "CrapsStatsTracker.h"

// --------------------- CONSTRUCTION TESTS ---------------------

TEST_F(CrapsStatsTrackerTest, Constructor_CreatesValidInstance)
{
    ASSERT_NO_THROW(CrapsStatsTracker());
}

TEST_F(CrapsStatsTrackerTest, DatabaseConnected)
{
    ASSERT_TRUE(m_Cst->GetIsConnected());
}