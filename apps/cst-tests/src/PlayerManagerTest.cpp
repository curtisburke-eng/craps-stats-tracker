
#include "PlayerManagerTest.h"

#include <gtest/gtest.h>
#include <system_error>

#include "PlayerManager/PlayerManager.h"
#include "utils/CstErrorCodes.h"


TEST_F(PlayerManagerTest, Constructor_CreatesPlayerManager)
{
    ASSERT_NE(m_Cst->GetPlayerManager(), nullptr);
}

TEST_F(PlayerManagerTest, GetAllAvailablePlayers)
{
    auto playerManager = m_Cst->GetPlayerManager();
    ASSERT_GT(playerManager->GetAllAvailablePlayers().size(),0);
}

TEST_F(PlayerManagerTest, CreateNewPlayer)
{
    auto playerManager = m_Cst->GetPlayerManager();
    std::error_code retVal = playerManager->CreateNewPlayer("TestPlayer");
    ASSERT_EQ(retVal, CstErrorCodes::eOk);
}

TEST_F(PlayerManagerTest, UpdatePlayerWinLoss)
{
    auto playerManager = m_Cst->GetPlayerManager();
    uint32_t playerId = 1;

    std::error_code retVal = playerManager->UpdatePlayerWinLoss(playerId);
    ASSERT_EQ(retVal, CstErrorCodes::eOk);
}

TEST_F(PlayerManagerTest, GetPlayerWinLoss)
{
    auto playerManager = m_Cst->GetPlayerManager();
    uint32_t playerId = 1;

    int winLoss;
    std::error_code retVal = playerManager->GetPlayerWinLoss(playerId, winLoss);
    ASSERT_EQ(retVal, CstErrorCodes::eOk);
}

TEST_F(PlayerManagerTest, GetAllInGamePlayers)
{
    uint32_t gameId = 1;
    auto playerManager = m_Cst->GetPlayerManager();
    ASSERT_GE(playerManager->GetAllInGamePlayers(gameId).size(),0);
}

TEST_F(PlayerManagerTest, AddPlayerToGame)
{
    uint32_t gameId = 1;
    uint32_t playerId = 1;

    auto playerManager = m_Cst->GetPlayerManager();
    std::error_code retVal = playerManager->AddPlayerToGame(playerId, gameId);

    ASSERT_NE(retVal, CstErrorCodes::eQueryFailed);
}

TEST_F(PlayerManagerTest, BuyInTests)
{
    auto playerManager = m_Cst->GetPlayerManager();
    uint32_t gameId = 1;
    uint32_t playerId = 1;
    int addBuyInAmount = 300;

    // Get originalBuyIn
    int originalBuyIn;
    std::error_code retVal = playerManager->GetPlayerBuyIn(playerId, gameId, originalBuyIn);
    ASSERT_EQ(retVal, CstErrorCodes::eOk);

    retVal = playerManager->AddPlayerBuyIn(playerId, gameId, addBuyInAmount);
    EXPECT_EQ(retVal, CstErrorCodes::eOk);

    int getBuyInAmount;
    retVal = playerManager->GetPlayerBuyIn(playerId, gameId, getBuyInAmount);
    EXPECT_EQ(retVal, CstErrorCodes::eOk);
    EXPECT_EQ(getBuyInAmount, originalBuyIn + addBuyInAmount);

    // Restore originalBuyIn
    retVal = playerManager->SetPlayerBuyIn(playerId, gameId, originalBuyIn);
    ASSERT_EQ(retVal, CstErrorCodes::eOk);
}



TEST_F(PlayerManagerTest, CashOutTests)
{
    auto playerManager = m_Cst->GetPlayerManager();
    uint32_t gameId = 1;
    uint32_t playerId = 1;
    int setCashOutAmount = 500;

    // Get original cash out
    int originalCashOut;
    std::error_code retVal = playerManager->GetPlayerCashOut(playerId, gameId, originalCashOut);
    ASSERT_EQ(retVal, CstErrorCodes::eOk);

    retVal = playerManager->SetPlayerCashOut(playerId, gameId, setCashOutAmount);
    EXPECT_EQ(retVal, CstErrorCodes::eOk);

    int getCashOutAmount;
    retVal = playerManager->GetPlayerCashOut(playerId, gameId, getCashOutAmount);
    EXPECT_EQ(retVal, CstErrorCodes::eOk);
    EXPECT_EQ(getCashOutAmount, setCashOutAmount);

    // Restore original cash out
    retVal = playerManager->SetPlayerCashOut(playerId, gameId, originalCashOut);
    ASSERT_EQ(retVal, CstErrorCodes::eOk);
}


TEST_F(PlayerManagerTest, CurrentShooterTests)
{
    auto playerManager = m_Cst->GetPlayerManager();
    uint32_t gameId = 1;
    uint32_t playerId = 1;

    std::error_code retVal = playerManager->SetCurrentShooter(playerId, gameId);
    ASSERT_EQ(retVal, CstErrorCodes::eOk);

    uint32_t shooterId = playerManager->GetCurrentShooter();
    ASSERT_EQ(shooterId, playerId);
}