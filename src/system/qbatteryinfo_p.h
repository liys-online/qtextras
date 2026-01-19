#ifndef QBATTERYINFO_P_H
#define QBATTERYINFO_P_H
#include "qbatteryinfo.h"
#include <QMap>

namespace QBatteryInfoPlatform {
    bool query(bool& available, int& level, bool& charging);

    struct QPowerSourceState
    {
        enum PowerFlow {
            Unknown,
            BatteryOnly,
            ExternalOnly,
            ChargingBattery
        };

        bool valid = false;

        bool externalOnline = false;
        bool charging = false;

        int percent = -1;
        int remainingMinutes = -1;

        int fullCapacity = -1;
        int designCapacity = -1;
        int healthPercent = -1;

        PowerFlow powerFlow = Unknown;

        bool operator==(const QPowerSourceState &o) const
        {
            return memcmp(this, &o, sizeof(QPowerSourceState)) == 0;
        }
        bool operator!=(const QPowerSourceState &o) const
        {
            return !(*this == o);
        }
    };
    QPowerSourceState qQueryPowerSourceState();

}

class QTimer;

class QBatteryInfoPrivate : public QObject
{
    Q_OBJECT

public:
    QBatteryInfoPrivate(QBatteryInfo *parent);
    ~QBatteryInfoPrivate();

private:
    QBatteryInfo * const q_ptr;
    Q_DECLARE_PUBLIC(QBatteryInfo);
    bool isValid;
    int batteryLifePercent;
    bool isCharging;
    void initialize();

private Q_SLOTS:
    void getBatteryStatus();

};

#endif // QBATTERYINFO_P_H
