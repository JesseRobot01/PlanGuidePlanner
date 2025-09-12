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
 * @code TaskTypes @endcode are all the differend labels
 * @code GuideGoalTasks @endcode stores the text and the label for each goal task.
 * @code ReportTests @endcode stores the text and the weight for each test on the report
 * @code GuideGoals @endcode  stores the data for each goal
 * @code GuideObject @endcode stores all the data for the differend sections. (Index, test, Report)
 */

class GuideData : public QObject{
public:
    enum ObjectTypes {
        Index,
        Test,
        Report
    };

    enum TaskTypes {
        Work,
        Read,
        Watch,
        Process,
        Info
    };

    struct GuideGoalTasks {
        TaskTypes task;
        QString text;
        QString link;
    };

    struct ReportTests {
        QString name;
        QString weight;
    };

    class GuideGoals {
    public:
        QVector<GuideGoalTasks> tasks;
        QString name;
        QString goalNumber;
        QString time;
        QString week;
        QString progress;

        void addWork(const QString&name, const QString&link = "") {
            GuideGoalTasks work;
            work.task = Work;
            work.text = name;
            work.link = link;
            tasks.append(work);
        }

        void addRead(const QString&name, const QString&link = "") {
            GuideGoalTasks read;
            read.task = Read;
            read.text = name;
            read.link = link;
            tasks.append(read);
        }

        void addWatch(const QString&name, const QString&link = "") {
            GuideGoalTasks watch;
            watch.task = Watch;
            watch.text = name;
            watch.link = link;
            tasks.append(watch);
        }

        void addProcess(const QString&name, const QString&link = "") {
            GuideGoalTasks process;
            process.task = Process;
            process.text = name;
            process.link = link;
            tasks.append(process);
        }
        void addInfo(const QString&name, const QString&link = "") {
            GuideGoalTasks info;
            info.task = Info;
            info.text = name;
            info.link = link;
            tasks.append(info);
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

  static  Data errorGuide(const QString&error);
};


#endif //GUIDEDATA_H
