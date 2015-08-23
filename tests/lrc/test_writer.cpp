#include <lrc/lrcwriter.h>

#include <QtTest/QtTest>

class test_Writer : public QObject
{
	Q_OBJECT
private Q_SLOTS:
	void basicTest();
};

void test_Writer::basicTest()
{
	//LrcWriter reader;
}

QTEST_GUILESS_MAIN(test_Writer)
#include "test_writer.moc"
