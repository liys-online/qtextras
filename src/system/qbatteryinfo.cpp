#include "qbatteryinfo.h"

#include <QCoreApplication>
#include <QTimer>

#include "qbatteryinfo_platform_p.h"
#if defined(Q_OS_WIN)
#include "qbatteryinfo_platform_win_p.h"
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
    auto timer = new QTimer(this);
    timer->setInterval(1000);

    connect(timer, &QTimer::timeout, this, &QBatteryInfo::update);
    connect(this, &QBatteryInfo::destroyed, timer, &QTimer::stop);
    timer->start();
    update();
}

void QBatteryInfo::update()
{
    bool available;
    int level;
    bool charging;

    if (!QBatteryInfoPlatform::query(available, level, charging))
        return;

    if (available != m_available) {
        m_available = available;
        emit availableChanged(available);
    }

    if (level != m_level) {
        m_level = level;
        emit levelChanged(level);
    }

    if (charging != m_charging) {
        m_charging = charging;
        emit chargingChanged(charging);
    }
}

bool QBatteryInfo::isAvailable() const
{
    return m_available;
}

int QBatteryInfo::level() const
{
    return m_level;
}

bool QBatteryInfo::isCharging() const
{
    return m_charging;
}
