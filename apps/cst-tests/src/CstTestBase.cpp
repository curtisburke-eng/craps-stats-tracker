
#include "CstTestBase.h"

#include <gtest/gtest.h>
#include <string>
#include <memory>
#include <system_error>

#include "TestEnvManager.h"

void CstTestBase::SetUp() {
    m_Cst = TestEnvManager::GetCstInstance();

    std::error_code retVal = m_Cst->ConnectToDatabase();

}
