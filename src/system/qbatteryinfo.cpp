#include "qbatteryinfo.h"
#include <QCoreApplication>

Q_GLOBAL_STATIC(QBatteryInfo, batteryInfo)

QBatteryInfo *QBatteryInfo::instance()
{
    return batteryInfo();
}

QBatteryInfo::QBatteryInfo()
    : QObject{qApp}
{}
