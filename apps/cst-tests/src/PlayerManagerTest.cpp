
#include "PlayerManagerTest.h"

#include <gtest/gtest.h>
#include <system_error>
#include "PlayerManager/PlayerManager.h"

// --------------------- CONSTRUCTION TESTS ---------------------


TEST_F(PlayerManagerTest, Constructor_CreatesPlayerManager)
{
    ASSERT_NE(m_Cst->GetPlayerManager(), nullptr);
}

TEST_F(PlayerManagerTest, GetAllAvailablePlayers)
{
    auto playerManager = m_Cst->GetPlayerManager();
    ASSERT_GT(playerManager->GetAllAvailablePlayers().size(),0);
}