//
// Created by Jesse on 04-10-2025.
//

#include "NewGuide.h"

#include <QLabel>
#include <QVBoxLayout>


#include "ui_NewGuide.h"
#include "themes/GuidePalette.h"

NewGuide::NewGuide(QWidget* parent, const NewGuideData::Data* data) : QWidget(parent), ui(new Ui::GuideBase) {
    ui->setupUi(this);

    if (data)
        processGuide(*data);
    else
        qCritical() << "No data in guide!";

    updateStyle();
    retranslateUi();
}

NewGuide::~NewGuide() {
    delete ui;
    delete mainLayout;
    delete lastSpacer;

    goalFrames.clear();
    testWidgets.clear();
    reportWidgets.clear();
}

void NewGuide::updateStyle() {
    GuidePalette palette;

    const QString objectTextStyle = QString::fromLatin1("color: %1;").arg(
        palette.getColor(GuidePalette::ObjectText).name());

    const QString frameStyle = QString::fromLatin1("background-color: %1;").arg(palette.color(QPalette::Base).name());

    const QString borderColourString = QString::fromLatin1("border-width:3px;border-style:solid;border-color: %1;")
            .arg(palette.color(QPalette::Base).name());

    const QString baseStyle = QString::fromLatin1("background-color: %1; color:%2; ") + borderColourString;

    const QString headerStyle = QString::fromLatin1("background-color: %1; color: %2")
            .arg(palette.getColor(GuidePalette::HeaderBackground).name())
            .arg(palette.getColor(GuidePalette::HeaderText).name());

    const QString periodStyle = QString::fromLatin1("background-color: %1; color: %2")
            .arg(palette.getColor(GuidePalette::TestBackground).name())
            .arg(palette.getColor(GuidePalette::HeaderText).name());

    const QString workIndicatorStyle = baseStyle
                                       .arg(palette.getColor(GuidePalette::WorkIndicatorBackground).name())
                                       .arg(palette.getColor(GuidePalette::WorkIndicatorText).name())
                                       + borderColourString;

    const QString workIndicatorTextStyle = baseStyle
                                           .arg(palette.getColor(GuidePalette::WorkIndicatorExample).name())
                                           .arg(palette.getColor(GuidePalette::WorkIndicatorText).name())
                                           + borderColourString;

    const QString watchIndicatorStyle = baseStyle
                                        .arg(palette.getColor(GuidePalette::WatchIndicatorBackground).name())
                                        .arg(palette.getColor(GuidePalette::WatchIndicatorText).name())
                                        + borderColourString;

    const QString watchIndicatorTextStyle = baseStyle
                                            .arg(palette.getColor(GuidePalette::WatchIndicatorExample).name())
                                            .arg(palette.getColor(GuidePalette::WatchIndicatorText).name())
                                            + borderColourString;

    const QString readIndicatorStyle = baseStyle
                                       .arg(palette.getColor(GuidePalette::ReadIndicatorBackground).name())
                                       .arg(palette.getColor(GuidePalette::ReadIndicatorText).name())
                                       + borderColourString;

    const QString readIndicatorTextStyle = baseStyle
                                           .arg(palette.getColor(GuidePalette::ReadIndicatorExample).name())
                                           .arg(palette.getColor(GuidePalette::ReadIndicatorText).name())
                                           + borderColourString;

    const QString processIndicatorStyle = baseStyle
                                          .arg(palette.getColor(GuidePalette::ProcessIndicatorBackground).name())
                                          .arg(palette.getColor(GuidePalette::ProcessIndicatorText).name())
                                          + borderColourString;

    const QString processIndicatorTextStyle = baseStyle
                                              .arg(palette.getColor(GuidePalette::ProcessIndicatorExample).name())
                                              .arg(palette.getColor(GuidePalette::ProcessIndicatorText).name())
                                              + borderColourString;

    const QString infoIndicatorStyle = baseStyle
                                       .arg(palette.getColor(GuidePalette::InfoIndicatorBackground).name())
                                       .arg(palette.getColor(GuidePalette::InfoIndicatorText).name())
                                       + borderColourString;

    const QString infoIndicatorTextStyle = baseStyle
                                           .arg(palette.getColor(GuidePalette::InfoIndicatorExample).name())
                                           .arg(palette.getColor(GuidePalette::InfoIndicatorText).name())
                                           + borderColourString;


    ui->infoFrame->setStyleSheet(frameStyle);
    ui->header->setStyleSheet(headerStyle);
    ui->periodNum->setStyleSheet(periodStyle);

    ui->mainInfoHead->setStyleSheet(objectTextStyle);
    ui->period->setStyleSheet(headerStyle);

    ui->workLabelText->setStyleSheet(workIndicatorTextStyle);
    ui->workLabel->setStyleSheet(workIndicatorStyle);

    ui->watchLabelText->setStyleSheet(watchIndicatorTextStyle);
    ui->watchLabel->setStyleSheet(watchIndicatorStyle);

    ui->readLabelText->setStyleSheet(readIndicatorTextStyle);
    ui->readLabel->setStyleSheet(readIndicatorStyle);

    ui->processLabelText->setStyleSheet(processIndicatorTextStyle);
    ui->processLabel->setStyleSheet(processIndicatorStyle);

    ui->infoLabelText->setStyleSheet(infoIndicatorTextStyle);
    ui->infoLabel->setStyleSheet(infoIndicatorStyle);

    for (auto frame: goalFrames) {
        frame->updateStyle();
    }
    for (auto test: testWidgets) {
        test->updateStyle();
    }
    for (auto report: reportWidgets) {
        report->updateStyle();
    }
}

void NewGuide::retranslateUi() {
    ui->retranslateUi(this);

    for (auto frame: goalFrames) {
        frame->retranslateUi();
    }
    for (auto report: reportWidgets) {
        report->retranslateUi();
    }
}

void NewGuide::processGuide(NewGuideData::Data data) {
    baseData = data;

    ui->mainName->setText(data.name);
    ui->mainInfoText->setText(data.info);

    if (data.period.isEmpty()) {
        ui->period->hide();
        ui->periodNum->hide();
    }
    else ui->periodNum->setText(data.period);

    QVector<NewGuideData::Object> goals;

    for (auto object: data.objects) {
        objectOrder.append(object.type);
        if (object.type == NewGuideData::Goal) {
            goals.append(object);
            continue;
        }
        nonGoalObjects.append(object);

        if (!goals.isEmpty()) {
            //This also works for breaks.
            GoalFrame* frame = new GoalFrame(this, &goals);
            goals.clear();
            ui->mainLayout->addWidget(frame);
            goalFrames.append(frame);
        }
        if (object.type == NewGuideData::Test) {
            NewTest* newTest = new NewTest(this, &object);
            ui->mainLayout->addWidget(newTest);
            testWidgets.append(newTest);
        }
        if (object.type == NewGuideData::Report) {
            NewReport* newReport = new NewReport(this, &object);
            ui->mainLayout->addWidget(newReport);
            reportWidgets.append(newReport);
        }
    }

    if (!goals.isEmpty()) {
        GoalFrame* frame = new GoalFrame(this, &goals);
        goals.clear();
        ui->mainLayout->addWidget(frame);
        goalFrames.append(frame);
    }

    lastSpacer = new QSpacerItem(20, 39, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    ui->mainLayout->addItem(lastSpacer);
}

NewGuideData::Data NewGuide::getGuide() {
    NewGuideData::Data data;
    data = baseData; // Fill up main data
    data.objects.clear(); // Reset

    int goalCounter = 0;
    int restCounter = 0;

    QVector<NewGuideData::Object> goals;
    for (auto goalFrame: goalFrames) {
        goals.append(goalFrame->getGoals());
    }
    for (auto object: objectOrder) {
        switch (object) {
            case NewGuideData::Goal:
                data.objects.append(goals.at(goalCounter));
                goalCounter++;
                break;
            default: //The rest
                data.objects.append(nonGoalObjects.at(restCounter));
                restCounter++;
                break;
        }
    }
    baseData = data;
    return baseData;
}
