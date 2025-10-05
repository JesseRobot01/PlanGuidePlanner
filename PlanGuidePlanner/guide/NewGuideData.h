//
// Created by Jesse on 04-10-2025.
//

#ifndef PLANGUIDEPLANNER_NEWGUIDEDATA_H
#define PLANGUIDEPLANNER_NEWGUIDEDATA_H
#include <QObject>
#include <QDate>
#include <QFileInfo>

#include "OldGuideData.h"

class NewGuideData : public QObject {
public:
    enum ObjectTypes {
        Goal,
        Test,
        Report,
        Break
    };

    enum TaskTypes {
        Work,
        Read,
        Watch,
        Process,
        Info
    };

    enum Progress {
        NotStarted,
        Working,
        Finished
    };

    struct Task {
        TaskTypes task;
        QString text;
    };

    struct ReportTest {
        QString name;
        int weight;
        QString weightType;
    };

    class Object {
    public:
        ObjectTypes type;

        QVector<Task> tasks;
        QVector<ReportTest> tests;

        QString name;
        QString info;
        QString shortName;
        int time;

        QDate week;
        Progress progress = NotStarted;

        void setWeekFromWeekNumber(QString weekNumber);

        void addTask(const QString&name, const TaskTypes&taskType, const QString&link = "");

        void setProgressFromInt(const int&number);

        void addTest(const QString&testname, const int&weight, const QString&weightType = "x");
    };

    class Data {
    public:
        QVector<Object> objects;
        QString name;
        QString info;
        int period;
        QString shortName;
        QFileInfo originalFile;
        QFileInfo autoSaveFile;
    };

    Data errorGuide(const QString&error);

    Data fromOldData(const OldGuideData::Data&guide);
};


#endif //PLANGUIDEPLANNER_NEWGUIDEDATA_H
