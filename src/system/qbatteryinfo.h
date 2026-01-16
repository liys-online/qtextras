#ifndef QBATTERYINFO_H
#define QBATTERYINFO_H

#include "system_global.h"
#include <QObject>

class QBatteryInfoPrivate;
class QTEXTRAS_SYSTEM_EXPORT QBatteryInfo final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool available READ isAvailable NOTIFY availableChanged)
    Q_PROPERTY(int level READ level NOTIFY levelChanged)
    Q_PROPERTY(bool charging READ isCharging NOTIFY chargingChanged)
public:

    enum ChargerType {
        UnknownCharger = 0,
        WallCharger,
        USBCharger,
        VariableCurrentCharger
    };

    enum ChargingState {
        UnknownChargingState = 0,
        Charging,
        IdleChargingState,
        Discharging
    };

    enum LevelStatus {
        LevelUnknown = 0,
        LevelEmpty,
        LevelLow,
        LevelOk,
        LevelFull
    };

    enum Health {
        HealthUnknown = 0,
        HealthOk,
        HealthBad
    };
    static QBatteryInfo* instance();
    ~QBatteryInfo() override;

    bool isAvailable() const;

    int level() const;

    bool isCharging() const;

Q_SIGNALS:
    void availableChanged(bool available);
    void levelChanged(int level);
    void chargingChanged(bool charging);

private:
    explicit QBatteryInfo();
    QBatteryInfo(const QBatteryInfo &) = delete;
    QBatteryInfo &operator=(const QBatteryInfo &) = delete;


    void update();

    bool m_available;
    int m_level;
    bool m_charging;
    QBatteryInfoPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QBatteryInfo)
};

#endif // QBATTERYINFO_H
