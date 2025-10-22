//
// Created by Jesse on 27-08-2025.
//

#include "LegacyXmlParsers.h"
#include <QFile>
#include <QXmlStreamReader>
#include "XmlParser.h"
#include "guide/NewGuideData.h"
#include "Application.h"
#include <QString>


NewGuideData::Data LegacyXmlParsers::v1Reader(QFile* xmlFileP) {
    QFile&xmlFile = *xmlFileP;
    QFileInfo fileInfo(xmlFile);
    bool isPrevIndex = 0; //Used for the Breaks.
    qDebug() << "Reading xml file (using v1 parser)" << fileInfo.fileName();
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << fileInfo.fileName();
        return NewGuideData::errorGuide("Failed to open file " + fileInfo.fileName());
    }

    try {
        QXmlStreamReader xml(&xmlFile);
        QXmlStreamReader::TokenType token;
        QString elementName;

        while (!xml.atEnd()) {
            token = xml.readNext();
            elementName = xml.name().toString();


            if (elementName == "studyguide" && token != QXmlStreamReader::EndElement) {
                NewGuideData::Data guide;
                guide.originalFile = fileInfo;
                for (QXmlStreamAttribute attribute: xml.attributes()) {
                    if (attribute.name().toString() == "autosavefile" && attribute.value().toString() == "true") {
                        guide.autoSaveFile = fileInfo;
                    }
                    else if (attribute.name().toString() == "originalfile") {
                        guide.originalFile = QFileInfo(attribute.value().toString());
                    }
                }
                elementName = "";

                while (!(token == QXmlStreamReader::EndElement && elementName == "studyguide")) {
                    token = xml.readNext();
                    elementName = xml.name().toString();


                    if (elementName == "name") {
                        guide.name = xml.readElementText();
                    }
                    if (elementName == "shortname") {
                        guide.shortName = xml.readElementText();
                    }
                    if (elementName == "info") {
                        guide.info = xml.readElementText();
                    }
                    if (elementName == "period") {
                        guide.period = xml.readElementText();
                    }
                    if (elementName == "index") {
                        if (isPrevIndex) {
                            NewGuideData::Object goal;
                            goal.type = NewGuideData::Break;
                            guide.objects.append(goal); // yes, I'm lazy to rename it.
                        }
                        isPrevIndex = true;
                        elementName = "";

                        while (!(token == QXmlStreamReader::EndElement && elementName == "index")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "goal") {
                                NewGuideData::Object goal;
                                goal.type = NewGuideData::Goal;

                                elementName = "";
                                while (!(token == QXmlStreamReader::EndElement && elementName == "goal")) {
                                    token = xml.readNext();
                                    elementName = xml.name().toString();
                                    if (elementName == "name") {
                                        goal.name = xml.readElementText();
                                    }
                                    if (elementName == "number") {
                                        goal.number = xml.readElementText();
                                    }
                                    if (elementName == "time") {
                                        goal.time = xml.readElementText().toInt();
                                    }
                                    if (elementName == "work") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();

                                        goal.addTask(xml.readElementText(), NewGuideData::Work, link);
                                    }
                                    if (elementName == "watch") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();

                                        goal.addTask(xml.readElementText(), NewGuideData::Watch, link);
                                    }
                                    if (elementName == "read") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();

                                        goal.addTask(xml.readElementText(), NewGuideData::Read, link);
                                    }
                                    if (elementName == "process") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();


                                        goal.addTask(xml.readElementText(), NewGuideData::Process, link);
                                    }
                                    if (elementName == "info") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();


                                        goal.addTask(xml.readElementText(), NewGuideData::Info, link);
                                    }
                                    if (elementName == "week") {
                                        goal.setDateFromWeek(xml.readElementText());
                                    }
                                    if (elementName == "progress") {
                                        goal.setProgressFromInt(xml.readElementText().toInt());
                                    }
                                }

                                guide.objects.append(goal);
                            }
                        }
                        continue;
                    }
                    isPrevIndex = false;

                    if (elementName == "test") {
                        NewGuideData::Object test;
                        test.type = NewGuideData::Test;

                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "test")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "shortname") {
                                test.number = (xml.readElementText());
                            }
                            if (elementName == "name") {
                                test.name = (xml.readElementText());
                            }
                            if (elementName == "info") {
                                test.info = (xml.readElementText());
                            }
                            if (elementName == "week") {
                                test.setDateFromWeek(xml.readElementText());
                            }
                        }
                        guide.objects.append(test);
                    }
                    if (elementName == "report") {
                        NewGuideData::Object report;
                        report.type = NewGuideData::Report;

                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "report")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "test") {
                                NewGuideData::ReportTest test;
                                elementName = "";
                                while (!(token == QXmlStreamReader::EndElement && elementName == "test")) {
                                    token = xml.readNext();
                                    elementName = xml.name().toString();

                                    if (elementName == "name") {
                                        test.name = xml.readElementText();
                                    }
                                    if (elementName == "weight") {
                                        test.weight = xml.readElementText().toInt();
                                    }
                                }
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

        return NewGuideData::errorGuide("Error while reading XML file " + fileInfo.fileName());
    }
    return NewGuideData::errorGuide("Xml Parser returned nothing on file " + fileInfo.fileName());
}

NewGuideData::Data LegacyXmlParsers::v1PGPReader(QFile* xmlFileP) {
    QFile&xmlFile = *xmlFileP;
    QFileInfo fileInfo(xmlFile);
    qDebug() << "Reading xml file" << fileInfo.fileName();
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << fileInfo.fileName();
        return NewGuideData::errorGuide("Failed to open file " + fileInfo.fileName());
    }

    try {
        QXmlStreamReader xml(&xmlFile);
        QXmlStreamReader::TokenType token;
        QString elementName;

        while (!xml.atEnd()) {
            token = xml.readNext();
            elementName = xml.name().toString();

            if (elementName == "pgp" && token != QXmlStreamReader::EndElement) {
                NewGuideData::Data guide;

                guide.originalFile = fileInfo;
                for (QXmlStreamAttribute attribute: xml.attributes()) {
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


                    if (elementName == "name") {
                        guide.name = xml.readElementText();
                    }
                    if (elementName == "shortname") {
                        guide.shortName = xml.readElementText();
                    }
                    if (elementName == "info") {
                        guide.info = xml.readElementText();
                    }
                    if (elementName == "period") {
                        guide.period = xml.readElementText();
                    }

                    if (elementName == "goal") {
                        NewGuideData::Object goal;
                        goal.type = NewGuideData::Goal;

                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "goal")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();
                            if (elementName == "name") {
                                goal.name = xml.readElementText();
                            }
                            if (elementName == "number") {
                                goal.number = xml.readElementText();
                            }
                            if (elementName == "time") {
                                goal.time = xml.readElementText().toInt();
                            }
                            if (elementName == "work") {
                                QString link = "";
                                for (QXmlStreamAttribute attribute: xml.attributes())
                                    if (attribute.name().toString() == "href")
                                        link = attribute.value().toString();

                                goal.addTask(xml.readElementText(), NewGuideData::Work, link);
                            }
                            if (elementName == "watch") {
                                QString link = "";
                                for (QXmlStreamAttribute attribute: xml.attributes())
                                    if (attribute.name().toString() == "href")
                                        link = attribute.value().toString();

                                goal.addTask(xml.readElementText(), NewGuideData::Watch, link);
                            }
                            if (elementName == "read") {
                                QString link = "";
                                for (QXmlStreamAttribute attribute: xml.attributes())
                                    if (attribute.name().toString() == "href")
                                        link = attribute.value().toString();

                                goal.addTask(xml.readElementText(), NewGuideData::Read, link);
                            }
                            if (elementName == "process") {
                                QString link = "";
                                for (QXmlStreamAttribute attribute: xml.attributes())
                                    if (attribute.name().toString() == "href")
                                        link = attribute.value().toString();


                                goal.addTask(xml.readElementText(), NewGuideData::Process, link);
                            }
                            if (elementName == "info") {
                                QString link = "";
                                for (QXmlStreamAttribute attribute: xml.attributes())
                                    if (attribute.name().toString() == "href")
                                        link = attribute.value().toString();


                                goal.addTask(xml.readElementText(), NewGuideData::Info, link);
                            }
                            if (elementName == "week") {
                                goal.setDateFromWeek(xml.readElementText());
                            }
                            if (elementName == "progress") {
                                goal.setProgressFromInt(xml.readElementText().toInt());
                            }
                        }
                        guide.objects.append(goal);
                    }

                    if (elementName == "test") {
                        NewGuideData::Object test;
                        test.type = NewGuideData::Test;

                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "test")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "shortname") {
                                test.number = (xml.readElementText());
                            }
                            if (elementName == "name") {
                                test.name = (xml.readElementText());
                            }
                            if (elementName == "info") {
                                test.info = (xml.readElementText());
                            }
                            if (elementName == "week") {
                                test.setDateFromWeek(xml.readElementText());
                            }
                        }
                        guide.objects.append(test);
                    }
                    if (elementName == "report") {
                        NewGuideData::Object report;
                        report.type = NewGuideData::Report;

                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "report")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "test") {
                                NewGuideData::ReportTest test;
                                elementName = "";
                                while (!(token == QXmlStreamReader::EndElement && elementName == "test")) {
                                    token = xml.readNext();
                                    elementName = xml.name().toString();

                                    if (elementName == "name") {
                                        test.name = xml.readElementText();
                                    }
                                    if (elementName == "weight") {
                                        test.weight = xml.readElementText().toInt();
                                    }
                                }
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

        return NewGuideData::errorGuide("Error while reading XML file " + fileInfo.fileName());
    }
    return NewGuideData::errorGuide("Xml Parser returned nothing on file " + fileInfo.fileName());
}
