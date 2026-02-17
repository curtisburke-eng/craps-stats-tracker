#pragma once

#include <gtest/gtest.h>
#include <memory>
#include <system_error>

#include "CrapsStatsTracker.h"

class CstTestBase : public ::testing::Test,
                    public ::testing::WithParamInterface<std::error_code> {

protected:
    void SetUp() override;

    std::shared_ptr<CrapsStatsTracker> m_Cst;
};

