//
// Created by Jesse on 04-11-2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_StartScreen.h" resolved

#include "StartScreen.h"

#include "Application.h"
#include "ui_StartScreen.h"
#include "themes/GuidePalette.h"


StartScreen::StartScreen(QWidget* parent) : QWidget(parent), ui(new Ui::StartScreen) {
    ui->setupUi(this);
}

StartScreen::~StartScreen() {
    delete ui;
}

void StartScreen::updateStart() {
    qDebug() << "Updating start screen.";

    // gather all guides.
    QVector<GuideData::Data> guides = APPLICATION->getUpToDateGuides();


    //nuke old start screen.                            (A la kaboem?!)
    delete ui->scrollAreaWidgetContents;

    ui->scrollAreaWidgetContents = nullptr;
    ui->scrollAreaWidgetContents = new QWidget();

    ui->horizontalLayout_2 = new QHBoxLayout(ui->scrollAreaWidgetContents);
    ui->startScreenScrollArea->setWidget(ui->scrollAreaWidgetContents);

    //adds lists
    for (GuideData::Data guide: guides) {
        QWidget* startScreenList = new QWidget(ui->scrollAreaWidgetContents);
        QVBoxLayout* startScreenLayout = new QVBoxLayout(startScreenList);
        QLabel* startScreenLabel = new QLabel(startScreenList);
        startScreenLabel->setText(guide.shortName);
        startScreenLabel->setAlignment(Qt::AlignCenter);

        GuidePalette colourPalette;

        QFont startScreenLabelFont;
        startScreenLabelFont.setPointSize(18);
        startScreenLabelFont.setBold(true);

        startScreenLabel->setFont(startScreenLabelFont);
        startScreenLabel->setStyleSheet(
            QString::fromLatin1(
                "background-color: %1; border-width: 2px; border-left-width: 8px; border-right-width: 8px; border-style: solid; border-color: %1; color:%2; margin-bottom:2px")
            .arg(colourPalette.getColor(GuidePalette::HeaderBackground).name())
            .arg(colourPalette.getColor(GuidePalette::HeaderText).name()));

        startScreenLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        startScreenLayout->addWidget(startScreenLabel);

        ui->horizontalLayout_2->addWidget(startScreenList);


        //Styles
        //todo: proper styles ( even though it works on my system setup :) )
        //  this code gives a weirder style on fusion, however, on my setup (without fusion) it works.

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

        // list :)
        for (GuideData::GuideObject object: guide.objects) {
            if (object.objectType != GuideData::Index) {
                continue;
            }

            for (GuideData::GuideGoals goal: object.goals) {
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
            }
            QLabel* spacerLabel = new QLabel();
            startScreenLayout->addWidget(spacerLabel);
        }
        QSpacerItem* UnderSpacer = new
                QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
        startScreenLayout->addItem(UnderSpacer);
    }

    QSpacerItem* EndSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

    ui->horizontalLayout_2->addItem(EndSpacer);

    qDebug() << "Start Screen Update Successful!";
}
