#pragma once

#include "CrapsStatsTracker.h"
#include <memory>

class TestEnvManager
{
public:
    static std::shared_ptr<CrapsStatsTracker> GetCstInstance();
    static void SetCstInstance(std::shared_ptr<CrapsStatsTracker> Cst);

private:
    static std::shared_ptr<CrapsStatsTracker>  m_Cst;
};
