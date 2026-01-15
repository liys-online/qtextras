#include <QTest>

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

}

tst_QBatteryInfo::~tst_QBatteryInfo()
{

}

void tst_QBatteryInfo::test_case1()
{

}

QTEST_APPLESS_MAIN(tst_QBatteryInfo)

#include "tst_qbatteryinfo.moc"
