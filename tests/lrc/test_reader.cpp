/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include <lrc/lrcreader.h>

#include <QtTest/QtTest>

// copy from lrcreader
static int _convertTimeStr(const char *str)
{
	// hh, mm, ss
	double times[3] = { 0.f, 0.f, 0.f };
	int length = strlen(str);
	for (int i = length - 1,  idx = 2; i >= 0 && idx >= 0; --idx, i -= 2)
	{
		while (i > 0 && str[i - 1] != ':')
			--i;
		sscanf(str + i, "%lf", &times[idx]);
	}

	return int(((times[0] * 60 + times[1]) * 60 + times[2]) * 1000 + 0.5);
}

class test_Reader : public QObject
{
	Q_OBJECT
private Q_SLOTS:
	void basicTest();
	void readFromMemoryTest();
};

void test_Reader::basicTest()
{
	LrcReader reader(QFINDTESTDATA("Jewel-Stand.lrc").toStdString());
	QVERIFY(reader.parse());
}

void test_Reader::readFromMemoryTest()
{
	const char *lyrics =
	{
		"[title:test]\n"
		"[ar:]\r\n"
		"[offset:300]\n"
		"[00:00.00]Test\n"
		"[03:12.93]\n"
		"[00:03.56][00:43.41]Mutiple time tags\n\r"
		"[00:10.43]End\n"
	};

	LrcReader reader;
	reader.setBuffer(lyrics);
	QVERIFY(reader.parse());

	const LRC &lrc = reader.getLrc();
	QVERIFY(lrc.count() == 5);

	// testing lyrics
	const LRC::Line *firstLine = lrc.getLine(0);
	QVERIFY(firstLine->index == 0 && firstLine->lyrics == "Test");

	int nnLine = lrc.getLine(0)->index + 2;
	QVERIFY(nnLine == 2);
	QVERIFY(lrc.getLine(nnLine)->lyrics == "End");

	const LRC::Line *line = lrc.getLineByTime(_convertTimeStr("00:43.41"));
	QVERIFY(line != nullptr && line->lyrics == "Mutiple time tags");
	line = lrc.getLineByTime(_convertTimeStr("00:10.41"));
	QVERIFY(line == nullptr);

	// testing attributes
	QVERIFY(lrc.attrCount() == 3);
	QVERIFY(lrc.attribute("title") == "test");
	QVERIFY(lrc.attribute("by") == std::string());
	QVERIFY(lrc.getOffset() == 300);
}

QTEST_GUILESS_MAIN(test_Reader)
#include "test_reader.moc"
