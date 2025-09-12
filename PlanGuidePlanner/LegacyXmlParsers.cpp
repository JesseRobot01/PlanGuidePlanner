//
// Created by Jesse on 27-08-2025.
//

#include "LegacyXmlParsers.h"
#include <QFile>
#include <QXmlStreamReader>
#include "XmlParser.h"
#include "guide/GuideData.h"
#include "Application.h"
#include <QString>


GuideData::Data LegacyXmlParsers::v1Reader(QFile* xmlFileP) {
    QFile&xmlFile = *xmlFileP;
    QFileInfo fileInfo(xmlFile);
    qDebug() << "Reading xml file (using v1 parser)" << fileInfo.fileName();
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
                GuideData::Data guide;
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
                        GuideData::GuideObject index;
                        index.objectType = GuideData::Index;

                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "index")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "goal") {
                                GuideData::GuideGoals goal;

                                elementName = "";
                                while (!(token == QXmlStreamReader::EndElement && elementName == "goal")) {
                                    token = xml.readNext();
                                    elementName = xml.name().toString();
                                    if (elementName == "name") {
                                        goal.name = xml.readElementText();
                                    }
                                    if (elementName == "number") {
                                        goal.goalNumber = xml.readElementText();
                                    }
                                    if (elementName == "time") {
                                        goal.time = xml.readElementText();
                                    }
                                    if (elementName == "work") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();

                                        goal.addWork(xml.readElementText(), link);
                                    }
                                    if (elementName == "watch") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();

                                        goal.addWatch(xml.readElementText(), link);
                                    }
                                    if (elementName == "read") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();

                                        goal.addRead(xml.readElementText(), link);
                                    }
                                    if (elementName == "process") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();


                                        goal.addProcess(xml.readElementText(), link);
                                    }
                                    if (elementName == "info") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();


                                        goal.addInfo(xml.readElementText(), link);
                                    }
                                    if (elementName == "week") {
                                        goal.week = xml.readElementText();
                                    }
                                    if (elementName == "progress") {
                                        goal.progress = xml.readElementText();
                                    }
                                }

                                index.goals.append(goal);
                            }
                        }
                        guide.objects.append(index);
                    }
                    if (elementName == "test") {
                        GuideData::GuideObject test;
                        test.objectType = GuideData::Test;

                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "test")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "shortname") {
                                test.setShortTestName(xml.readElementText());
                            }
                            if (elementName == "name") {
                                test.setTestName(xml.readElementText());
                            }
                            if (elementName == "info") {
                                test.setTestInfo(xml.readElementText());
                            }
                            if (elementName == "week") {
                                test.setTestWeek(xml.readElementText());
                            }
                        }
                        guide.objects.append(test);
                    }
                    if (elementName == "report") {
                        GuideData::GuideObject report;
                        report.objectType = GuideData::Report;

                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "report")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "test") {
                                GuideData::ReportTests test;
                                elementName = "";
                                while (!(token == QXmlStreamReader::EndElement && elementName == "test")) {
                                    token = xml.readNext();
                                    elementName = xml.name().toString();

                                    if (elementName == "name") {
                                        test.name = xml.readElementText();
                                    }
                                    if (elementName == "weight") {
                                        test.weight = xml.readElementText();
                                    }
                                }
                                report.addTest(test);
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
