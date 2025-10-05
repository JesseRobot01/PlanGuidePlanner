//
// Created by Jesse on 04-10-2025.
//

#include "NewGuideData.h"
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
    goal1.date = QDate::currentDate();;
    goal1.addTask(tr("Check if the file is supported."), Work);
    goal1.addTask(tr("Open the mentioned file in a text editor,"), Info);
    goal1.addTask(tr("Search for '<StudyGuide' near the start of the document."), Info);

    goal2.name = tr("Try this too:");
    goal2.shortName = "2";
    goal2.time = 5;
    goal2.date = QDate::currentDate();
    goal2.addTask(tr("Check if the file is not open in another program."), Work);
    goal2.addTask(tr("Try making a copy!"), Work);

    goal3.name = tr("This is also an option:");
    goal3.shortName = "3";
    goal3.time = 5;
    goal3.date = QDate::currentDate();
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
    newData.period = guide.period;
    newData.info = guide.info;

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

                newGoal.setDateFromWeek(goal.week);
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
                    newGoal.addTask(task.text, taskType, task.link);
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
            newTest.setDateFromWeek(object.week);

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

void NewGuideData::Object::setDateFromWeek(QString weekNumber) {
    if (weekNumber.isEmpty())
        return;

    // For the older format we have to calculate the week ourselves.
    int weekInt = weekNumber.toInt();
    QDate jan4(QDate::currentDate().year(), 1, 4);

    int daysToMonday = Qt::Monday - jan4.dayOfWeek();
    QDate firstMonday = jan4.addDays(daysToMonday);

    // Add weeks to get to desired week
    date = firstMonday.addDays((weekInt - 1) * 7);
}

void NewGuideData::Object::addTask(const QString&name, const TaskTypes&taskType, const QString&link) {
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

void NewGuideData::Object::addTest(const QString&testname, const int&weight, const QString&weightType) {
    ReportTest test;
    test.name = testname;
    test.weight = weight;
    test.weightType = weightType;

    tests.append(test);
}

OldGuideData::Data NewGuideData::toOldData(NewGuideData::Data newData) {
    OldGuideData::Data oldData;

    oldData.name = newData.name;
    oldData.shortName = newData.shortName + "- Converted";
    oldData.autoSaveFile = newData.autoSaveFile;
    oldData.originalFile = newData.originalFile;
    oldData.period = newData.period;
    oldData.info = newData.info;

    OldGuideData::GuideObject* index = nullptr;
    for (auto object: newData.objects) {
        if (object.type == Goal) {
            if (index == nullptr) {
                index = new OldGuideData::GuideObject();
                index->objectType = OldGuideData::Index;
            }
            OldGuideData::GuideGoals goal;
            goal.name = object.name;
            goal.goalNumber = object.shortName;
            if (object.time != 0)
                goal.time = QString::number(object.time);
            if (object.date != QDate(0, 0, 0))
                goal.week = QString::number(object.date.weekNumber());
            goal.progress = QString::number(object.progress);

            for (auto task: object.tasks) {
                switch (task.task) {
                    case Work:
                        goal.addWork(task.text);
                        break;
                    case Watch:
                        goal.addWatch(task.text);
                        break;
                    case Read:
                        goal.addRead(task.text);
                        break;
                    case Process:
                        goal.addProcess(task.text);
                        break;
                    case Info:
                        goal.addInfo(task.text);
                        break;
                }
            }
            index->goals.append(goal);
        }
        else {
            if (index != nullptr) {
                oldData.objects.append(*index);
                index = nullptr;
            }
        }
        if (object.type == Test) {
            OldGuideData::GuideObject test;
            test.objectType = OldGuideData::Test;

            test.name = object.name;
            test.shortName = object.shortName;
            test.info = object.info;
            if (object.date != QDate(0, 0, 0))
            test.week = QString::number(object.date.weekNumber());

            oldData.objects.append(test);
        }
        if (object.type == Report) {
            OldGuideData::GuideObject report;
            report.objectType = OldGuideData::Report;

            for (auto test: object.tests) {
                OldGuideData::ReportTests oldTest;
                oldTest.name = test.name;
                oldTest.weight = QString::number(test.weight);

                report.addTest(oldTest);
            }

            oldData.objects.append(report);
        }
    }
    if (index != nullptr) {
        oldData.objects.append(*index);
        index = nullptr;
    }

    return oldData;
}
