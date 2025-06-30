//
// Created by Jesse on 30-06-2025.
//

#include <QTest>

#include <QObject>
#include <XmlParser.h>

class XmlParserTest : public QObject {
    Q_OBJECT

private slots:
    void XmlReadWriteTest() {
        auto testDir = QDir(QFINDTESTDATA("testdata"));
        QFile* inputFile = new QFile(testDir.absoluteFilePath("testfile.xml"));
        GuideData::Data testData;


        //benchmarkig read speed
        QBENCHMARK {
            testData = XmlParser::readXml(inputFile);
        }

        //save it
        testDir.mkdir("tmp");
        QDir testTmpDir = testDir.filePath("tmp");

        auto* outputFile = new QFile(testTmpDir.absoluteFilePath("output.xml"));

        //benchmarking write speed;
        QBENCHMARK {
            XmlParser::saveXml(testData, *outputFile);
        }

        inputFile->open(QIODevice::ReadOnly);
        outputFile->open(QIODevice::ReadOnly);

        QCOMPARE(inputFile->readAll(), outputFile->readAll());
    }
};

QTEST_GUILESS_MAIN(XmlParserTest);

#include "XmlParser_test.moc"
