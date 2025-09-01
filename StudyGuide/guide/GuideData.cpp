//
// Created by Jesse on 19-08-2025.
//

#include "GuideData.h"

GuideData::Data GuideData::errorGuide(const QString&error) {
    qWarning() << "Error thrown via GuideData.";
    qWarning() << "Error:" << error;
    Data errorGuide;

    errorGuide.name = (tr("Error"));
    errorGuide.shortName = (tr("Err"));

    errorGuide.info = (tr("Uh oh, something went wrong. \n"
                           "Error Description: ") + error);

    GuideObject object;
    object.objectType = Index;

    GuideGoals goal1;
    goal1.name = tr("Try this:");
    goal1.goalNumber = "1";
    goal1.time = "5";
    goal1.week = QString::number(QDate::currentDate().weekNumber());
    goal1.addWork(tr("Check if the file is supported."));
    goal1.addInfo(tr("Open the mentioned file in a text editor,"));
    goal1.addInfo(tr("Search for '<StudyGuide' near the start of the document."));

    object.goals.append(goal1);

    GuideGoals goal2;
    goal2.name = tr("Try this too:");
    goal2.goalNumber = "2";
    goal2.time = "5";
    goal2.week = QString::number(QDate::currentDate().weekNumber());
    goal2.addWork(tr("Check if the file is not open in another program."));
    goal2.addWork(tr("Try making a copy!"));
    object.goals.append(goal2);

    GuideGoals goal3;
    goal3.name = tr("This is also an option:");
    goal3.goalNumber = "3";
    goal3.time = "5";
    goal3.week = QString::number(QDate::currentDate().weekNumber());
    goal3.addWork(tr("Report an issue here."), "https://github.com/JesseRobot01/PlanGuidePlanner/issues");
    goal3.addInfo(tr("Make sure to attach the document and the logs!"));
    object.goals.append(goal3);

    errorGuide.objects.append(object);

    return errorGuide;
}
