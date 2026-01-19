#include <QTest>
#include "qbatteryinfo.h"

// add necessary includes here

class tst_QBatteryInfo : public QObject
{
    Q_OBJECT

public:
    tst_QBatteryInfo();
    ~tst_QBatteryInfo();

private slots:
    void test_case1();

};

tst_QBatteryInfo::tst_QBatteryInfo()
{
    if (!QBatteryInfo::instance()->isAvailable()) {
        QSKIP("No battery available on this system.");
    }
}

tst_QBatteryInfo::~tst_QBatteryInfo()
{

}

void tst_QBatteryInfo::test_case1()
{
    qDebug() << QBatteryInfo::instance()->isAvailable();
    qDebug() << QBatteryInfo::instance()->batteryLifePercent();
    qDebug() << QBatteryInfo::instance()->isCharging();
}

QTEST_MAIN(tst_QBatteryInfo)

#include "tst_qbatteryinfo.moc"
