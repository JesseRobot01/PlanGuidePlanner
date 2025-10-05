//
// Created by Jesse on 04-11-2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_StartScreen.h" resolved

#include "StartScreen.h"

#include "Application.h"
#include "ui_StartScreen.h"
#include "themes/GuidePalette.h"
#include <QLabel>
#include <QHBoxLayout>

StartScreen::StartScreen(QWidget* parent) : QWidget(parent), ui(new Ui::StartScreen) {
    ui->setupUi(this);
    addWeekLabel();
}

StartScreen::~StartScreen() {
    delete ui;
}

void StartScreen::addWeekLabel() {
    // Set up week label
    QSpacerItem* EndSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

    ui->horizontalLayout_2->addItem(EndSpacer);
    //Styles
    GuidePalette colourPalette;

    QString headerStyle = QString::fromLatin1(
                "background-color: %1; border-width: 2px; border-left-width: 8px; border-right-width: 8px; border-style: solid; border-color: %1; color:%2; margin-bottom:2px")
            .arg(colourPalette.getColor(GuidePalette::HeaderBackground).name())
            .arg(colourPalette.getColor(GuidePalette::HeaderText).name());

    QFont startScreenLabelFont;
    startScreenLabelFont.setPointSize(18);
    startScreenLabelFont.setBold(true);


    QWidget* startScreenList = new QWidget(ui->scrollAreaWidgetContents);
    QVBoxLayout* startScreenLayout = new QVBoxLayout(startScreenList);
    QLabel* startScreenLabel = new QLabel(startScreenList);
    startScreenLabel->setText(tr("Week: %1").arg(QDate::currentDate().weekNumber()));
    startScreenLabel->setAlignment(Qt::AlignCenter);

    startScreenLabel->setFont(startScreenLabelFont);
    startScreenLabel->setStyleSheet(headerStyle);

    startScreenLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

    startScreenLayout->addWidget(startScreenLabel);

    ui->horizontalLayout_2->addWidget(startScreenList);

    QSpacerItem* UnderSpacer = new
            QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    startScreenLayout->addItem(UnderSpacer);
}

void StartScreen::updateStart() {
    qDebug() << "Updating start screen.";

    // gather all guides.
    QVector<OldGuideData::Data> guides = APPLICATION->getUpToDateGuides();


    //nuke old start screen.                            (A la kaboem?!)
    delete ui->scrollAreaWidgetContents;

    ui->scrollAreaWidgetContents = nullptr;
    ui->scrollAreaWidgetContents = new QWidget();

    ui->horizontalLayout_2 = new QHBoxLayout(ui->scrollAreaWidgetContents);
    ui->startScreenScrollArea->setWidget(ui->scrollAreaWidgetContents);

    //Styles
    GuidePalette colourPalette;

    QFont startScreenLabelFont;
    startScreenLabelFont.setPointSize(18);
    startScreenLabelFont.setBold(true);

    QString headerStyle = QString::fromLatin1(
                "background-color: %1; border-width: 2px; border-left-width: 8px; border-right-width: 8px; border-style: solid; border-color: %1; color:%2; margin-bottom:2px")
            .arg(colourPalette.getColor(GuidePalette::HeaderBackground).name())
            .arg(colourPalette.getColor(GuidePalette::HeaderText).name());

    QString notStarted =
            QString::fromLatin1(
                "background-color: %1; border-width: 2px; border-style: solid; border-color:#00000000;")
            .arg(colourPalette.getColor(GuidePalette::Progress_NotStarted).name());

    QString progress =
            QString::fromLatin1(
                "background-color: %1; border-width: 2px; border-style: solid; border-color:#00000000;")
            .arg(colourPalette.getColor(GuidePalette::Progress_Working).name());

    QString finished = QString::fromLatin1(
                "background-color: %1; border-width: 2px; border-style: solid; border-color: #00000000;")
            .arg(colourPalette.getColor(GuidePalette::Progress_Finished).name());


    QFont goalFont;
    goalFont.setBold(true);

    //adds lists
    for (OldGuideData::Data guide: guides) {
        QWidget* startScreenList = new QWidget(ui->scrollAreaWidgetContents);
        QVBoxLayout* startScreenLayout = new QVBoxLayout(startScreenList);
        QLabel* startScreenLabel = new QLabel(startScreenList);
        float currentTotalProgress = 0;
        float totalMaxProgress = 0;

        startScreenLabel->setText(guide.shortName);
        startScreenLabel->setAlignment(Qt::AlignCenter);

        startScreenLabel->setFont(startScreenLabelFont);
        startScreenLabel->setStyleSheet(headerStyle);

        startScreenLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        startScreenLayout->addWidget(startScreenLabel);

        ui->horizontalLayout_2->addWidget(startScreenList);

        // list :)
        for (OldGuideData::GuideObject object: guide.objects) {
            if (object.objectType != OldGuideData::Index) {
                continue;
            }

            for (OldGuideData::GuideGoals goal: object.goals) {
                QLabel* goalLabel = new QLabel(goal.goalNumber);
                goalLabel->setFont(goalFont);
                goalLabel->setAlignment(Qt::AlignCenter);
                switch (goal.progress.toInt()) {
                    case 0:
                        goalLabel->setStyleSheet(notStarted);
                        break;
                    case 1:
                        goalLabel->setStyleSheet(progress);
                        break;
                    case 2:
                        goalLabel->setStyleSheet(finished);
                        break;
                    default:
                        goalLabel->setStyleSheet(notStarted);
                        break;
                }
                startScreenLayout->addWidget(goalLabel);

                totalMaxProgress += 2;
                currentTotalProgress += goal.progress.toInt();
            }
            QLabel* spacerLabel = new QLabel();
            startScreenLayout->addWidget(spacerLabel);
        }
        QSpacerItem* UnderSpacer = new
                QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
        startScreenLayout->addItem(UnderSpacer);

        int percentage = floor(currentTotalProgress / totalMaxProgress * 100);
        QLabel* percentageLabel = new QLabel();
        percentageLabel->setFont(goalFont);
        percentageLabel->setAlignment(Qt::AlignCenter);

        percentageLabel->setText(QString::number(percentage) + "%");
        if (percentage == 100)
            percentageLabel->setStyleSheet(finished);
        else if (percentage >= 50)
            percentageLabel->setStyleSheet(progress);
        else
            percentageLabel->setStyleSheet(notStarted);

        startScreenLayout->addWidget(percentageLabel);
    }

    QSpacerItem* EndSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

    ui->horizontalLayout_2->addItem(EndSpacer);

    addWeekLabel();

    qDebug() << "Start Screen Update Successful!";
}
