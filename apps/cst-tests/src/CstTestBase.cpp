
#include "CstTestBase.h"

#include <gtest/gtest.h>
#include <memory>

#include "TestEnvManager.h"

void CstTestBase::SetUp() {
    m_Cst = TestEnvManager::GetCstInstance();
}
