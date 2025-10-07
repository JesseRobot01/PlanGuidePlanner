//
// Created by Jesse on 30-06-2025.
//

#include <QTest>

#include <QObject>
#include <XmlParser.h>

class LegacyXmlParserTest : public QObject {
    Q_OBJECT

private slots:
    void LegacyXmlsReadTest() {
        auto testDir = QDir(QFINDTESTDATA("Legacy Parser Test"));
        auto newFileDir = QDir(QFINDTESTDATA("XML Parser Test"));
        // We use this file for comparison so that there are less files to update each version


        QStringList testFiles = {
            testDir.absoluteFilePath("legacyFormat.xml"),
            testDir.absoluteFilePath("v1.xml")
        };

        QVector<GuideData::Data> testData;


        testData = XmlParser::readXml(testFiles);

        //save it
        testDir.mkdir("tmp");
        QDir testTmpDir = testDir.filePath("tmp");

        QVector<QFile *> outputFiles;

        for (auto data: testData) {
            auto* outputFile = new QFile(testTmpDir.absoluteFilePath(data.originalFile.baseName()));

            XmlParser::saveXml(data, *outputFile);
            outputFiles.append(outputFile);
        }

        QFile* newestFile = new QFile(newFileDir.absoluteFilePath("testfile.xml"));

        for (auto outputFile: outputFiles) {
            newestFile->open(QIODevice::ReadOnly);
            outputFile->open(QIODevice::ReadOnly);
            QCOMPARE(newestFile->readAll(), outputFile->readAll());
            newestFile->close();
        }
    }
};

QTEST_GUILESS_MAIN(LegacyXmlParserTest);

#include "LegacyXmlParsers_test.moc"
