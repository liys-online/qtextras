#include "qbatteryinfo.h"

#include <QCoreApplication>
#include <QTimer>
#include <qelapsedtimer.h>
#include <qmutex.h>

#include "qbatteryinfo_p.h"
#if defined(Q_OS_WIN)
#include <windows.h>
// namespace QBatteryInfoPlatform {

// bool query(bool& available, int& level, bool& charging)
// {
//     SYSTEM_POWER_STATUS sps;
//     if (!GetSystemPowerStatus(&sps))
//         return false;

//     available = (sps.BatteryFlag != 128); // 128 = no battery
//     level = (sps.BatteryLifePercent == 255) ? -1 : sps.BatteryLifePercent;
//     charging = (sps.ACLineStatus == 1);

//     return true;
// }



// }
static QMutex g_powerMutex;
static QBatteryInfoPlatform::QPowerSourceState g_cachedState;
static QElapsedTimer g_lastQuery;
static bool g_initialized = false;

static constexpr qint64 QueryIntervalMs = 1000;

QBatteryInfoPlatform::QPowerSourceState qPowerSourceState()
{
    QMutexLocker locker(&g_powerMutex);

    if (!g_initialized) {
        g_lastQuery.invalidate();
        g_initialized = true;
    }

    const bool needUpdate =
        !g_lastQuery.isValid() ||
        g_lastQuery.elapsed() > QueryIntervalMs;

    if (!needUpdate)
        return g_cachedState;

    QBatteryInfoPlatform::QPowerSourceState newState = QBatteryInfoPlatform::qQueryPowerSourceState();

    if (!newState.valid) {
        if (g_cachedState.valid)
            return g_cachedState;
    } else {
        g_cachedState = newState;
    }

    g_lastQuery.restart();
    return g_cachedState;
}
#elif defined(Q_OS_MAC)
#include "qbatteryinfo_platform_mac_p.h"
#else
#include "qbatteryinfo_platform_generic_p.h"
#endif


QBatteryInfo *QBatteryInfo::instance()
{
    static QBatteryInfo inst;
    return &inst;
}

QBatteryInfo::~QBatteryInfo()
{
    qDebug("QBatteryInfo::~QBatteryInfo()");
}

QBatteryInfo::QBatteryInfo()
    : QObject{},
    d_ptr{new QBatteryInfoPrivate(this)}
{

}

bool QBatteryInfo::isAvailable() const
{
    Q_D(const QBatteryInfo);
    return d->isValid;
}

int QBatteryInfo::batteryLifePercent() const
{
    Q_D(const QBatteryInfo);
    return d->batteryLifePercent;
}

bool QBatteryInfo::isCharging() const
{
    Q_D(const QBatteryInfo);
    return d->isCharging;
}
