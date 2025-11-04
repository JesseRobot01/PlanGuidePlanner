//
// Created by Jesse on 6 okt. 2023.
//

#include <QFile>
#include <QXmlStreamReader>
#include "XmlParser.h"
#include "guide/GuideData.h"
#include "Application.h"
#include <QString>

#include "LegacyXmlParsers.h"

const float currentReadWriteVersion = 2.0;

GuideData::Data XmlParser::readXml(QFile* xmlFileP) {
    QFile&xmlFile = *xmlFileP;
    QFileInfo fileInfo(xmlFile);
    qDebug() << "Reading xml file" << fileInfo.fileName();
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << fileInfo.fileName();
        return GuideData::errorGuide("Failed to open file " + fileInfo.fileName());
    }

    try {
        QXmlStreamReader xml(&xmlFile);
        QXmlStreamReader::TokenType token;
        QString elementName;

        while (!xml.atEnd()) {
            token = xml.readNext();
            elementName = xml.name().toString();

            if (elementName == "studyguide" && token != QXmlStreamReader::EndElement) {
                qWarning() << "Legacy file found! starting legacy parser...";
                xmlFile.close();
                return LegacyXmlParsers::v1Reader(xmlFileP);
            }

            if (elementName == "pgp" && token != QXmlStreamReader::EndElement) {
                GuideData::Data guide;

                guide.originalFile = fileInfo;
                for (QXmlStreamAttribute attribute: xml.attributes()) {
                    if (attribute.name().toString() == "format-version") {
                        // Check if version is supported
                        if (attribute.value().toFloat() == 1.0) {
                            qWarning() << "PGP file V1 found! Starting V1 parser...";
                            xmlFile.close();
                            return LegacyXmlParsers::v1PGPReader(xmlFileP);
                        }

                        if (attribute.value().toFloat() > currentReadWriteVersion)
                            qWarning() << "This file is newer than the supported reader, continue with caution!";
                    }

                    if (attribute.name().toString() == "autosavefile" && attribute.value().toString() == "true") {
                        guide.autoSaveFile = fileInfo;
                    }
                    else if (attribute.name().toString() == "originalfile") {
                        guide.originalFile = QFileInfo(attribute.value().toString());
                    }
                }
                elementName = "";

                while (!(token == QXmlStreamReader::EndElement && elementName == "pgp")) {
                    token = xml.readNext();
                    elementName = xml.name().toString();

                    if (elementName == "guide") {
                        for (QXmlStreamAttribute attribute: xml.attributes()) {
                            if (attribute.name().toString() == "period") {
                                guide.period = attribute.value().toString();
                            }
                        }
                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "guide")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "name") {
                                for (QXmlStreamAttribute attribute: xml.attributes()) {
                                    if (attribute.name().toString() == "short") {
                                        guide.shortName = attribute.value().toString();
                                    }
                                }
                                guide.name = xml.readElementText();
                            }
                            if (elementName == "info") {
                                guide.info = xml.readElementText();
                            }
                        }
                    }

                    if (elementName == "goal") {
                        GuideData::Object goal;
                        goal.type = GuideData::Goal;

                        elementName = "";

                        for (QXmlStreamAttribute attribute: xml.attributes()) {
                            if (attribute.name().toString() == "number") {
                                goal.number = attribute.value().toString();
                            }
                            if (attribute.name().toString() == "duration") {
                                goal.time = attribute.value().toInt();
                            }
                            if (attribute.name().toString() == "date") {
                                goal.date = QDate::fromString(attribute.value().toString(), Qt::ISODate);
                            }
                            if (attribute.name().toString() == "progress") {
                                goal.setProgressFromInt(attribute.value().toInt());
                            }
                        }

                        while (!(token == QXmlStreamReader::EndElement && elementName == "goal")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();
                            if (elementName == "name") {
                                goal.name = xml.readElementText();
                            }
                            if (elementName == "task") {
                                GuideData::TaskTypes taskType = GuideData::Work;
                                for (QXmlStreamAttribute attribute: xml.attributes()) {
                                    if (attribute.name().toString() == "type") {
                                        if (attribute.value().toString() == "work")
                                            taskType = GuideData::Work;
                                        if (attribute.value().toString() == "watch")
                                            taskType = GuideData::Watch;
                                        if (attribute.value().toString() == "read")
                                            taskType = GuideData::Read;
                                        if (attribute.value().toString() == "process")
                                            taskType = GuideData::Process;
                                        if (attribute.value().toString() == "info")
                                            taskType = GuideData::Info;
                                    }
                                }

                                goal.addTask(xml.readElementText(), taskType);
                            }
                        }
                        guide.objects.append(goal);
                    }
                    if (elementName == "br") {
                        GuideData::Object br;
                        br.type = GuideData::Break;
                        guide.objects.append(br);
                    }

                    if (elementName == "test") {
                        GuideData::Object test;
                        test.type = GuideData::Test;

                        elementName = "";
                        for (QXmlStreamAttribute attribute: xml.attributes()) {
                            if (attribute.name().toString() == "number") {
                                test.number = attribute.value().toString();
                            }
                            if (attribute.name().toString() == "date") {
                                test.date = QDate::fromString(attribute.value().toString(), Qt::ISODate);
                            }
                        }
                        while (!(token == QXmlStreamReader::EndElement && elementName == "test")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "name") {
                                test.name = (xml.readElementText());
                            }
                            if (elementName == "info") {
                                test.info = (xml.readElementText());
                            }
                        }
                        guide.objects.append(test);
                    }
                    if (elementName == "report") {
                        GuideData::Object report;
                        report.type = GuideData::Report;

                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "report")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "test") {
                                GuideData::ReportTest test;
                                elementName = "";
                                for (QXmlStreamAttribute attribute: xml.attributes()) {
                                    if (attribute.name().toString() == "weight") {
                                        test.weight = attribute.value().toInt();
                                    }
                                    if (attribute.name().toString() == "type") {
                                        test.weightType = attribute.value().toString();
                                    }
                                }
                                test.name = xml.readElementText();
                                report.tests.append(test);
                            }
                        }
                        guide.objects.append(report);
                    }
                }


                qDebug() << "Finished reading xml file" << fileInfo.fileName();
                xml.clear();
                xmlFile.close();
                return guide;
            }
        }
    }

    catch (...) {
        qCritical() << "Error while reading XML file" << fileInfo.fileName();
        xmlFile.close();

        return GuideData::errorGuide("Error while reading XML file " + fileInfo.fileName());
    }
    return GuideData::errorGuide("Xml Parser returned nothing on file " + fileInfo.fileName());
}

void XmlParser::saveXml(const GuideData::Data&guide, QFile&fileToSaveTo, bool isAutoSave, bool useAutoFormatting) {
    // QFile&fileToSaveTo = *fileToSaveToP;
    QFileInfo fileInfo(fileToSaveTo);
    if (!fileToSaveTo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file " << fileInfo.fileName() << "for saving.";
    }
    try {
        QXmlStreamWriter xml(&fileToSaveTo);
        xml.setAutoFormatting(useAutoFormatting);

        xml.writeStartDocument();
        xml.writeStartElement("pgp");
        xml.writeAttribute("format-version", QString::number(currentReadWriteVersion));
        if (isAutoSave) {
            xml.writeAttribute("autosavefile", "true");
            xml.writeAttribute("originalfile", guide.originalFile.filePath());
        }
        xml.writeStartElement("guide");
        xml.writeAttribute("period", guide.period); {
            xml.writeStartElement("name");
            xml.writeAttribute("short", guide.shortName);
            xml.writeCharacters(guide.name);
            xml.writeEndElement();
            xml.writeTextElement("info", guide.info);
        }
        xml.writeEndElement(); // Guide

        // start element loop
        for (GuideData::Object object: guide.objects) {
            switch (object.type) {
                case GuideData::Goal:
                    xml.writeStartElement("goal");

                    xml.writeAttribute("number", object.number);
                    xml.writeAttribute("duration", QString::number(object.time));
                    xml.writeAttribute("date", object.date.toString(Qt::ISODate));
                    xml.writeAttribute("progress", QString::number(object.progress));

                    xml.writeTextElement("name", object.name);


                    for (GuideData::Task task: object.tasks) {
                        xml.writeStartElement("task");
                        switch (task.task) {
                            case GuideData::Work:
                                xml.writeAttribute("type", "work");
                                break;
                            case GuideData::Read:
                                xml.writeAttribute("type", "read");
                                break;
                            case GuideData::Watch:
                                xml.writeAttribute("type", "watch");
                                break;
                            case GuideData::Process:
                                xml.writeAttribute("type", "process");
                                break;
                            case GuideData::Info:
                                xml.writeAttribute("type", "info");
                                break;
                        }
                        xml.writeCharacters(task.text);
                        xml.writeEndElement(); // end of task
                    }
                    xml.writeEndElement(); // goal
                    break;
                case GuideData::Break:
                    xml.writeEmptyElement("br");
                    break;
                case GuideData::Test:
                    xml.writeStartElement("test");
                    xml.writeAttribute("number", object.number);
                    xml.writeAttribute("date", object.date.toString(Qt::ISODate));
                    xml.writeTextElement("name", object.name);
                    xml.writeTextElement("info", object.info);
                    xml.writeEndElement(); // test
                    break;
                case GuideData::Report:
                    xml.writeStartElement("report");
                    for (GuideData::ReportTest test: object.tests) {
                        xml.writeStartElement("test");
                        xml.writeAttribute("weight", QString::number(test.weight));
                        xml.writeAttribute("type", test.weightType);
                        xml.writeCharacters(test.name);
                        xml.writeEndElement(); // test
                    }
                    xml.writeEndElement(); // report
            }
        }

        xml.writeEndElement(); // pgp
        xml.writeEndDocument();
        qDebug() << "Finished saving file" << fileInfo.fileName();;
    }
    catch (...) {
        qCritical() << "Failed to save Xml file" << fileInfo.fileName();;
    }
    fileToSaveTo.close();
}

void XmlParser::autoSaveXml(QVector<GuideData::Data> GuidesToSave) {
    QVector<QFuture<void>> futures;

    for (auto guide: GuidesToSave) {
        if (!guide.autoSaveFile.exists()) {
            QFileInfo autoSaveFile(APPLICATION->getAutoSaveLocation(), guide.originalFile.fileName());
            guide.autoSaveFile = autoSaveFile;
        }

        futures.append(QtConcurrent::run([guide]() {
            QFile autoSaveFile(guide.autoSaveFile.filePath());
            return saveXml(guide, autoSaveFile, true, false);
        }));
    }
}
