#include "qbatteryinfo_p.h"
#include <qtimer.h>
#if defined(Q_OS_WIN)
#include <qt_windows.h>
#include <wbemidl.h>
#include <comdef.h>

#pragma comment(lib, "wbemuuid.lib")
namespace QBatteryInfoPlatform {

// ---------------------------------------------------------
// Main query
// ---------------------------------------------------------

QPowerSourceState qQueryPowerSourceState()
{
    QPowerSourceState state;

    // -----------------------------------------------------
    // 1. GetSystemPowerStatus
    // -----------------------------------------------------
    SYSTEM_POWER_STATUS sps;
    if (!GetSystemPowerStatus(&sps))
        return state;

    state.valid = true;
    state.externalOnline = (sps.ACLineStatus == 1);

    if (sps.BatteryLifePercent != 255)
        state.percent = sps.BatteryLifePercent;

    if (sps.BatteryLifeTime != (DWORD)-1)
        state.remainingMinutes = sps.BatteryLifeTime / 60;

    state.charging = (sps.BatteryFlag & 8);

    // PowerFlow
    if (!state.externalOnline)
        state.powerFlow = QPowerSourceState::BatteryOnly;
    else if (state.charging)
        state.powerFlow = QPowerSourceState::ChargingBattery;
    else
        state.powerFlow = QPowerSourceState::ExternalOnly;


    return state;
}
}
QBatteryInfoPlatform::QPowerSourceState qPowerSourceState();

QBatteryInfoPrivate::QBatteryInfoPrivate(QBatteryInfo *parent)
    : QObject(parent),
      q_ptr(parent)
{
    initialize();
}

QBatteryInfoPrivate::~QBatteryInfoPrivate()
{

}

void QBatteryInfoPrivate::getBatteryStatus()
{
    QBatteryInfoPlatform::QPowerSourceState state = qPowerSourceState();

    if (state.valid) {
        if (isValid != state.valid) {
            isValid = state.valid;
            Q_EMIT q_ptr->availableChanged(isValid);
        }
        if (batteryLifePercent != state.percent) {
            batteryLifePercent = state.percent;
            Q_EMIT q_ptr->batteryLifePercentChanged(batteryLifePercent);
        }
        if (isCharging != state.charging) {
            isCharging = state.charging;
            Q_EMIT q_ptr->chargingChanged(isCharging);
        }
    }
}

void QBatteryInfoPrivate::initialize()
{
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(getBatteryStatus()));
    timer->start(1000);
    getBatteryStatus();
}
#endif

