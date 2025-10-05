//
// Created by Jesse on 04-10-2025.
//

#include "NewGuideData.h"


NewGuideData::Data NewGuideData::errorGuide(const QString&error) {
    qWarning() << "Error thrown via GuideData.";
    qWarning() << "Error:" << error;
    Data errorGuide;

    errorGuide.name = (tr("Error"));
    errorGuide.shortName = (tr("Err"));

    errorGuide.info = (tr("Uh oh, something went wrong. \n"
                           "Error Description: ") + error);

    Object goal1;
    Object goal2;
    Object goal3;

    goal1.type = Goal;
    goal2.type = Goal;
    goal3.type = Goal;

    goal1.name = tr("Try this:");
    goal1.shortName = "1";
    goal1.time = 5;
    goal1.week = QDate::currentDate();;
    goal1.addTask(tr("Check if the file is supported."), Work);
    goal1.addTask(tr("Open the mentioned file in a text editor,"), Info);
    goal1.addTask(tr("Search for '<StudyGuide' near the start of the document."), Info);

    goal2.name = tr("Try this too:");
    goal2.shortName = "2";
    goal2.time = 5;
    goal2.week = QDate::currentDate();
    goal2.addTask(tr("Check if the file is not open in another program."), Work);
    goal2.addTask(tr("Try making a copy!"), Work);

    goal3.name = tr("This is also an option:");
    goal3.shortName = "3";
    goal3.time = 5;
    goal3.week = QDate::currentDate();
    goal3.addTask(tr("Report an issue here."), Work, "https://github.com/JesseRobot01/PlanGuidePlanner/issues");
    goal3.addTask(tr("Make sure to attach the document and the logs!"), Info);

    errorGuide.objects.append(goal1);
    errorGuide.objects.append(goal2);
    errorGuide.objects.append(goal3);

    return errorGuide;
}

NewGuideData::Data NewGuideData::fromOldData(const OldGuideData::Data&guide) {
    NewGuideData::Data newData;

    newData.name = guide.name;
    newData.shortName = guide.shortName;
    newData.autoSaveFile = guide.autoSaveFile;
    newData.originalFile = guide.originalFile;
    newData.period = guide.period.toInt();

    bool wasPrevIndex = false; // Check if a break is needed

    for (auto object: guide.objects) {
        if (object.objectType == OldGuideData::Index) {
            if (wasPrevIndex) {
                //Add a break
                Object br;
                br.type = Break;
                newData.objects.append(br);
            }
            wasPrevIndex = true;

            for (auto goal: object.goals) {
                Object newGoal;
                newGoal.type = Goal;

                newGoal.name = goal.name;
                newGoal.shortName = goal.goalNumber;
                newGoal.time = goal.time.toInt();

                newGoal.setWeekFromWeekNumber(goal.week);
                newGoal.setProgressFromInt(goal.progress.toInt());

                for (auto task: goal.tasks) {
                    TaskTypes taskType;
                    switch (task.task) {
                        case Work:
                            taskType = Work;
                            break;
                        case Watch:
                            taskType = Watch;
                            break;
                        case Read:
                            taskType = Read;
                            break;
                        case Process:
                            taskType = Process;
                            break;
                        case Info:
                            taskType = Info;
                            break;
                    }
                    newGoal.addTask(task.text, taskType, task.link) ;
                }
                newData.objects.append(newGoal);
            }
        }
        if (object.objectType == OldGuideData::Test) {
            wasPrevIndex = false;
            Object newTest;
            newTest.type = Test;

            newTest.name = object.name;
            newTest.shortName = object.shortName;
            newTest.info = object.info;
            newTest.setWeekFromWeekNumber(object.week);

            newData.objects.append(newTest);
        }
        if (object.objectType == OldGuideData::Report) {
            wasPrevIndex = false;
            Object newReport;
            newReport.type = Report;

           for (auto test: object.tests) {
               newReport.addTest(test.name, test.weight.toInt());
           }

            newData.objects.append(newReport);
        }
    }

    return newData;
}

void NewGuideData::Object::setWeekFromWeekNumber(QString weekNumber) {
    // For the older format we have to calculate the week ourselves.
    int weekInt = weekNumber.toInt();
    QDate jan4(QDate::currentDate().year(), 1, 4);

    int daysToMonday = Qt::Monday - jan4.dayOfWeek();
    QDate firstMonday = jan4.addDays(daysToMonday);

    // Add weeks to get to desired week
    week = firstMonday.addDays((weekInt - 1) * 7);
}

void NewGuideData::Object::addTask(const QString&name, const TaskTypes&taskType, const QString&link ) {
    Task task;
    task.task = taskType;

    if (!link.isEmpty())
        task.text = QString::fromLatin1("<a href='%1'>%2</a>").arg(link, name);

    else task.text = name;


    tasks.append(task);
}

void NewGuideData::Object::setProgressFromInt(const int&number) {
    // Progress
    switch (number) {
        case 2:
            progress = Finished;
            break;
        case 1:
            progress = Working;
            break;
        case 0:
        default:
            progress = NotStarted;
            break;
    }
}

void NewGuideData::Object::addTest(const QString&testname, const int&weight, const QString&weightType ) {
    ReportTest test;
    test.name = testname;
    test.weight = weight;
    test.weightType = weightType;

    tests.append(test);
}