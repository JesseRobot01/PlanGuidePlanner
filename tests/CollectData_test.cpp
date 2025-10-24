//
// Created by Jesse on 24-10-2025.
//

#include <QTest>

#include <QObject>
#include <XmlParser.h>

#include "ui/newGuideExperiment/NewGuide.h"

class CollectDataTest : public QObject {
    Q_OBJECT

private slots:
    void CollectDataFromGuideTest() {
        auto testDir = QDir(QFINDTESTDATA("testdata"));
        QFile* inputFile = new QFile(testDir.absoluteFilePath("testfile.xml"));
        NewGuideData::Data testData;


        testData = XmlParser::readXml(inputFile);


        NewGuide* guide = new NewGuide(nullptr, &testData);

        //save it
        testDir.mkdir("tmp");
        QDir testTmpDir = testDir.filePath("tmp");

        auto* outputFile = new QFile(testTmpDir.absoluteFilePath("output_collectData.xml"));

        XmlParser::saveXml(guide->getGuide(), *outputFile);


        inputFile->open(QIODevice::ReadOnly);
        outputFile->open(QIODevice::ReadOnly);

        QCOMPARE(inputFile->readAll(), outputFile->readAll());
    }
};

QTEST_MAIN(CollectDataTest);

#include "CollectData_test.moc"
