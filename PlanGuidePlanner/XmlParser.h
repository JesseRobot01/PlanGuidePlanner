//
// Created by Jesse on 6 okt. 2023.
//

#ifndef PLANGUIDEPLANNER_XMLPARSER_H
#define PLANGUIDEPLANNER_XMLPARSER_H

#include <QFile>
#include <QtConcurrent/QtConcurrent>

#include "guide/OldGuideData.h"


class XmlParser {
public:
    static OldGuideData::Data readXml(QFile *xmlFile);

    static OldGuideData::Data readXml(const QString &xmlFile) {
        QFile *file = new QFile(xmlFile);
        return readXml(file);
    }

    static QVector<OldGuideData::Data> readXml(const QStringList &xmlFiles) {
        QVector<OldGuideData::Data> guides;
        QVector<QFuture<OldGuideData::Data>> futures;

        for (const QString &xmlFile: xmlFiles) {
            futures.append(QtConcurrent::run([xmlFile]() {
                return readXml(xmlFile);
            }));
        }

        // Wait for all threads to finish
        for (QFuture<OldGuideData::Data> future: futures) {
            future.waitForFinished();
        }

        // Retrieve the results
        for (const QFuture<OldGuideData::Data> &future: futures) {
            guides.append(future.result());
        }

        return guides;
    }

    static void saveXml(const OldGuideData::Data &guide, QFile &fileToSaveTo, bool isAutoSave = false, bool useAutoFormatting = true);

    static void autoSaveXml(QVector<OldGuideData::Data> GuidesToSave);
};

#endif //PLANGUIDEPLANNER_XMLPARSER_H
