//
// Created by Jesse on 6 okt. 2023.
//

#ifndef PLANGUIDEPLANNER_XMLPARSER_H
#define PLANGUIDEPLANNER_XMLPARSER_H

#include <QFile>
#include <QtConcurrent/QtConcurrent>

#include "guide/GuideData.h"


class XmlParser {
public:
    static GuideData::Data readXml(QFile *xmlFile);

    static GuideData::Data readXml(const QString &xmlFile) {
        QFile *file = new QFile(xmlFile);
        return readXml(file);
    }

    static QVector<GuideData::Data> readXml(const QStringList &xmlFiles) {
        QVector<GuideData::Data> guides;
        QVector<QFuture<GuideData::Data>> futures;

        for (const QString &xmlFile: xmlFiles) {
            futures.append(QtConcurrent::run([xmlFile]() {
                return readXml(xmlFile);
            }));
        }

        // Wait for all threads to finish
        for (QFuture<GuideData::Data> future: futures) {
            future.waitForFinished();
        }

        // Retrieve the results
        for (const QFuture<GuideData::Data> &future: futures) {
            guides.append(future.result());
        }

        return guides;
    }

    static void saveXml(const GuideData::Data &guide, QFile &fileToSaveTo, bool isAutoSave = false, bool useAutoFormatting = true);

    static void autoSaveXml(QVector<GuideData::Data> GuidesToSave);
};

#endif //PLANGUIDEPLANNER_XMLPARSER_H
