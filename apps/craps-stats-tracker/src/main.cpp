
#include "AppSetup.h"
#include "CrapsStatsTracker.h"

#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {

    // Set up Logger
    AppSetup::CreateLogger();

    SPDLOG_INFO("Starting CrapsStatsTracker...");

    CrapsStatsTracker cst;

    return 0;
}

