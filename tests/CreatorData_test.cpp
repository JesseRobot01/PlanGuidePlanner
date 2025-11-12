//
// Created by Jesse on 01-11-2025.
//

#include <QTest>

#include <QObject>
#include <XmlParser.h>

#include "creator/Creator.h"

class CreatorDataTest : public QObject {
    Q_OBJECT

private slots:
    void CollectDataFromCreatorTest() {
        auto testDir = QDir(QFINDTESTDATA("testData")); // Can use this file.
        QFile* inputFile = new QFile(testDir.absoluteFilePath("TestFile.xml"));
        GuideData::Data testData;


        testData = XmlParser::readXml(inputFile);


        auto creator = new Creator(nullptr);

        creator->open(testData);

        //save it
        testDir.mkdir("tmp");
        QDir testTmpDir = testDir.filePath("tmp");

        auto* outputFile = new QFile(testTmpDir.absoluteFilePath("output_creatorData.xml"));

        XmlParser::saveXml(creator->getCurrentGuide(), *outputFile);


        inputFile->open(QIODevice::ReadOnly);
        outputFile->open(QIODevice::ReadOnly);

        QCOMPARE(inputFile->readAll(), outputFile->readAll());
    }
};

QTEST_MAIN(CreatorDataTest);

#include "CreatorData_test.moc"
