
#include "TestEnvManager.h"
#include "CrapsStatsTracker.h"

/**
 * @brief Static member initialization.
 */
std::shared_ptr<CrapsStatsTracker> TestEnvManager::m_Cst = nullptr;

/**
 * @brief Retrieves the CST instance.
 * @return A shared pointer to the top level CrapsStatsTracker object.
 */
std::shared_ptr<CrapsStatsTracker> TestEnvManager::GetCstInstance()
{
    return m_Cst;
}

/**
 * @brief Sets the CST instance.
 * @param Cst A shared pointer to the CrapsStatsTracker object to set.
 */
void TestEnvManager::SetCstInstance(std::shared_ptr<CrapsStatsTracker> Cst)
{
    m_Cst = Cst;
}