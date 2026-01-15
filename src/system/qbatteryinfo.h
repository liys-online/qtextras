#ifndef QBATTERYINFO_H
#define QBATTERYINFO_H

#include <QObject>

class QBatteryInfo : public QObject
{
    Q_OBJECT
public:
    static QBatteryInfo* instance();
    QBatteryInfo(const QBatteryInfo &) = delete;
    QBatteryInfo &operator=(const QBatteryInfo &) = delete;
    explicit QBatteryInfo();

signals:
};

#endif // QBATTERYINFO_H
