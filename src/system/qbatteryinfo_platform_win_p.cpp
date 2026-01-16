#include "qbatteryinfo_platform_p.h"
#ifdef Q_OS_WIN
#include "qbatteryinfo_platform_win_p.h"
#include <windows.h>

#include <QTimer>
namespace QBatteryInfoPlatform {

bool query(bool& available, int& level, bool& charging)
{
    SYSTEM_POWER_STATUS sps;
    if (!GetSystemPowerStatus(&sps))
        return false;

    available = (sps.BatteryFlag != 128); // 128 = no battery
    level = (sps.BatteryLifePercent == 255) ? -1 : sps.BatteryLifePercent;
    charging = (sps.ACLineStatus == 1);

    return true;
}

}

#endif


void QBatteryInfoPrivate::initialize()
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &QBatteryInfoPrivate::getBatteryStatus);
    timer->start(1000);
    getBatteryStatus();
}



void QBatteryInfoPrivate::getBatteryStatus()
{

}

