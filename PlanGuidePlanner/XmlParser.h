//
// Created by Jesse on 6 okt. 2023.
//

#ifndef PLANGUIDEPLANNER_XMLPARSER_H
#define PLANGUIDEPLANNER_XMLPARSER_H

#include <QFile>
#include <QtConcurrent/QtConcurrent>

#include "guide/NewGuideData.h"


class XmlParser {
public:
    static NewGuideData::Data readXml(QFile *xmlFile);

    static NewGuideData::Data readXml(const QString &xmlFile) {
        QFile *file = new QFile(xmlFile);
        return readXml(file);
    }

    static QVector<NewGuideData::Data> readXml(const QStringList &xmlFiles) {
        QVector<NewGuideData::Data> guides;
        QVector<QFuture<NewGuideData::Data>> futures;

        for (const QString &xmlFile: xmlFiles) {
            futures.append(QtConcurrent::run([xmlFile]() {
                return readXml(xmlFile);
            }));
        }

        // Wait for all threads to finish
        for (QFuture<NewGuideData::Data> future: futures) {
            future.waitForFinished();
        }

        // Retrieve the results
        for (const QFuture<NewGuideData::Data> &future: futures) {
            guides.append(future.result());
        }

        return guides;
    }

    static void saveXml(const NewGuideData::Data &guide, QFile &fileToSaveTo, bool isAutoSave = false, bool useAutoFormatting = true);

    static void autoSaveXml(QVector<NewGuideData::Data> GuidesToSave);
};

#endif //PLANGUIDEPLANNER_XMLPARSER_H
