//
// Created by Jesse on 04-10-2025.
//

#include "GuideData.h"
#include "GuideData.h"


GuideData::Data GuideData::errorGuide(const QString&error) {
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
    goal1.number = "1";
    goal1.time = 5;
    goal1.date = QDate::currentDate();;
    goal1.addTask(tr("Check if the file is supported."), Work);
    goal1.addTask(tr("Open the mentioned file in a text editor,"), Info);
    goal1.addTask(tr("Search for '<StudyGuide' near the start of the document."), Info);

    goal2.name = tr("Try this too:");
    goal2.number = "2";
    goal2.time = 5;
    goal2.date = QDate::currentDate();
    goal2.addTask(tr("Check if the file is not open in another program."), Work);
    goal2.addTask(tr("Try making a copy!"), Work);

    goal3.name = tr("This is also an option:");
    goal3.number = "3";
    goal3.time = 5;
    goal3.date = QDate::currentDate();
    goal3.addTask(tr("Report an issue here."), Work, "https://github.com/JesseRobot01/PlanGuidePlanner/issues");
    goal3.addTask(tr("Make sure to attach the document and the logs!"), Info);

    errorGuide.objects.append(goal1);
    errorGuide.objects.append(goal2);
    errorGuide.objects.append(goal3);

    return errorGuide;
}

void GuideData::Object::setDateFromWeek(QString weekNumber) {
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

void GuideData::Object::addTask(const QString&name, const TaskTypes&taskType, const QString&link) {
    Task task;
    task.task = taskType;

    if (!link.isEmpty())
        task.text = QString::fromLatin1("<a href='%1'>%2</a>").arg(link, name);

    else task.text = name;


    tasks.append(task);
}

void GuideData::Object::setProgressFromInt(const int&number) {
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

void GuideData::Object::addTest(const QString&testname, const int&weight, const QString&weightType) {
    ReportTest test;
    test.name = testname;
    test.weight = weight;
    test.weightType = weightType;

    tests.append(test);
}
