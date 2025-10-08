//
// Created by Jesse on 04-10-2025.
//

#include "NewGuide.h"

#include <QLabel>
#include <QVBoxLayout>

#include "NewTest.h"
#include "ui_NewGuide.h"
#include "themes/GuidePalette.h"

NewGuide::NewGuide(QWidget* parent, const NewGuideData::Data* data) : QWidget(parent), ui(new Ui::GuideBase) {
    ui->setupUi(this);

    ui->period->hide();
    ui->periodNum->hide();

    for (const auto object : data->objects) {
        if (object.type == NewGuideData::Test) {
            NewTest* newTest = new NewTest(this, &object);
            ui->mainLayout->addWidget(newTest);
        }
    }
    lastSpacer = new QSpacerItem(20, 39, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

    ui->mainLayout->addItem(lastSpacer);
    ui->mainName->setText(data->name);
    ui->mainInfoText->setText(data->info);

    updateStyle();
}

NewGuide::~NewGuide() {
    delete ui;
}

void NewGuide::updateStyle() {
    GuidePalette palette;

    QString objectTextStyle = QString::fromLatin1("color: %1;").arg(palette.getColor(GuidePalette::ObjectText).name());

    QString frameStyle = QString::fromLatin1("background-color: %1;").arg(palette.color(QPalette::Base).name());

    QString borderColourString = QString::fromLatin1("border-width:3px;border-style:solid;border-color: %1;")
            .arg(palette.color(QPalette::Base).name());

    QString headerStyle = QString::fromLatin1("background-color: %1; color: %2")
            .arg(palette.getColor(GuidePalette::HeaderBackground).name())
            .arg(palette.getColor(GuidePalette::HeaderText).name());

    QString periodStyle = QString::fromLatin1("background-color: %1; color:%2")
            .arg(palette.getColor(GuidePalette::TestBackground).name())
            .arg(palette.getColor(GuidePalette::HeaderText).name());


    QString workIndicatorStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                 .arg(palette.getColor(GuidePalette::WorkIndicatorBackground).name())
                                 .arg(palette.getColor(GuidePalette::WorkIndicatorText).name())
                                 + borderColourString;

    QString workIndicatorTextStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                     .arg(palette.getColor(GuidePalette::WorkIndicatorExample).name())
                                     .arg(palette.getColor(GuidePalette::WorkIndicatorText).name())
                                     + borderColourString;


    QString watchIndicatorStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                  .arg(palette.getColor(GuidePalette::WatchIndicatorBackground).name())
                                  .arg(palette.getColor(GuidePalette::WatchIndicatorText).name())
                                  + borderColourString;

    QString watchIndicatorTextStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                      .arg(palette.getColor(GuidePalette::WatchIndicatorExample).name())
                                      .arg(palette.getColor(GuidePalette::WatchIndicatorText).name())
                                      + borderColourString;


    QString readIndicatorStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                 .arg(palette.getColor(GuidePalette::ReadIndicatorBackground).name())
                                 .arg(palette.getColor(GuidePalette::ReadIndicatorText).name())
                                 + borderColourString;

    QString readIndicatorTextStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                     .arg(palette.getColor(GuidePalette::ReadIndicatorExample).name())
                                     .arg(palette.getColor(GuidePalette::ReadIndicatorText).name())
                                     + borderColourString;


    QString processIndicatorStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                    .arg(palette.getColor(GuidePalette::ProcessIndicatorBackground).name())
                                    .arg(palette.getColor(GuidePalette::ProcessIndicatorText).name())
                                    + borderColourString;

    QString processIndicatorTextStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                        .arg(palette.getColor(GuidePalette::ProcessIndicatorExample).name())
                                        .arg(palette.getColor(GuidePalette::ProcessIndicatorText).name())
                                        + borderColourString;

    QString infoIndicatorStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                 .arg(palette.getColor(GuidePalette::InfoIndicatorBackground).name())
                                 .arg(palette.getColor(GuidePalette::InfoIndicatorText).name())
                                 + borderColourString;

    QString infoIndicatorTextStyle = QString::fromLatin1("background-color: %1;color:%2;")
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
}
