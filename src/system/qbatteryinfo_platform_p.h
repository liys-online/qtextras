#ifndef QBATTERYINFO_PLATFORM_P_H
#define QBATTERYINFO_PLATFORM_P_H
#include <QtCore/qglobal.h>
namespace QBatteryInfoPlatform {
    bool query(bool& available, int& level, bool& charging);
}


#endif // QBATTERYINFO_PLATFORM_P_H
