//
// Created by Jesse on 25-11-2023.
// This file is used for saving Guide Data
//

#ifndef GUIDEDATA_H
#define GUIDEDATA_H
#include <qlist.h>
#include <qstring.h>
#include <QDebug>
#include <qfileinfo.h>

/**
 * \brief This class is used for saving Guides
 * @code GuideData::Data @endcode is the actual Data.
 * @code ObjectTypes @endcode are all the differend possible sections. (Index, Test, Report)
 * @code PrefixType @endcode are all the differend labels
 * @code GuideGoalPrefixes @endcode stores the text and the label for each goal part.
 * @code ReportTests @endcode stores the text and the weight for each test on the report
 * @code GuideGoals @endcode  stores the data for each goal
 * @code GuideObject @endcode stores all the data for the differend sections. (Index, test, Report)
 */

class GuideData {
public:
    enum ObjectTypes {
        Index,
        Test,
        Report
    };

    enum PrefixTypes {
        Work,
        Read,
        Watch,
        Process,
        Info
    };

    struct GuideGoalPrefixes {
        PrefixTypes prefix;
        QString prefixText;
        QString link;
    };

    struct ReportTests {
        QString name;
        QString weight;
    };

    class GuideGoals {
    public:
        QVector<GuideGoalPrefixes> prefixes;
        QString name;
        QString goalNumber;
        QString time;
        QString week;
        QString progress;

        void addWork(const QString&name, const QString&link = "") {
            GuideGoalPrefixes work;
            work.prefix = Work;
            work.prefixText = name;
            work.link = link;
            prefixes.append(work);
        }

        void addRead(const QString&name, const QString&link = "") {
            GuideGoalPrefixes read;
            read.prefix = Read;
            read.prefixText = name;
            read.link = link;
            prefixes.append(read);
        }

        void addWatch(const QString&name, const QString&link = "") {
            GuideGoalPrefixes watch;
            watch.prefix = Watch;
            watch.prefixText = name;
            watch.link = link;
            prefixes.append(watch);
        }

        void addProcess(const QString&name, const QString&link = "") {
            GuideGoalPrefixes process;
            process.prefix = Process;
            process.prefixText = name;
            process.link = link;
            prefixes.append(process);
        }
        void addInfo(const QString&name, const QString&link = "") {
            GuideGoalPrefixes info;
            info.prefix = Info;
            info.prefixText = name;
            info.link = link;
            prefixes.append(info);
        }
    };

    class GuideObject {
    public:
        ObjectTypes objectType;

        QVector<GuideGoals> goals;
        QVector<ReportTests> tests;

        QString name;
        QString shortName;
        QString week;
        QString info;

        void setTestName(QString testName) {
            if (objectType != Test)
                qWarning() << "Trying to set the test name on a non test type, this does nothing";

            name = testName;
        }

        void setTestInfo(QString testInfo) {
            if (objectType != Test)
                qWarning() << "Trying to set the test info on a non test type, this does nothing";

            info = testInfo;
        }

        void setShortTestName(QString shortTestName) {
            if (objectType != Test)
                qWarning() << "Trying to set the short test name on a non test type, this does nothing";

            shortName = shortTestName;
        }

        void setTestWeek(QString testWeek) {
            if (objectType != Test)
                qWarning() << "Trying to set the test week on a non test type, this does nothing";

            week = testWeek;
        }


        void addTest(ReportTests test) {
            if (objectType != Report)
                qWarning() << "Trying to add an test to a non report type, this does nothing";


            tests.append(test);
        }
    };

    class Data {
    public:
        QVector<GuideObject> objects;
        QString name;
        QString info;
        QString period;
        QString shortName;
        QFileInfo originalFile;
        QFileInfo autoSaveFile;
    };
};


#endif //GUIDEDATA_H
