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
        int time = 0;

        QDate date = {0,0,0};
        Progress progress = NotStarted;

        void setDateFromWeek(QString weekNumber);

        void addTask(const QString&name, const TaskTypes&taskType, const QString&link = "");

        void setProgressFromInt(const int&number);

        void addTest(const QString&testname, const int&weight, const QString&weightType = "x");
    };

    class Data {
    public:
        QVector<Object> objects;
        QString name;
        QString info;
        QString period;
        QString shortName;
        QFileInfo originalFile;
        QFileInfo autoSaveFile;
    };

   static Data errorGuide(const QString&error);

    static Data fromOldData(const OldGuideData::Data&guide);
    static OldGuideData::Data toOldData(NewGuideData::Data newDataFormat);
};


#endif //PLANGUIDEPLANNER_NEWGUIDEDATA_H
