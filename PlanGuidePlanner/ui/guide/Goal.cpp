//
// Created by Jesse on 3 okt. 2023.
//

#include "Goal.h"
#include "ui_Goal.h"
#include "themes/GuidePalette.h"
#include "Application.h"

Goal::Goal(QWidget* parent) : QWidget(parent), ui(new Ui::Goal) {
    ui->setupUi(this);
    GuidePalette palette;
    QString defaultSlider = QString::fromLatin1("background-color:%1;").arg(
        palette.getColor(GuidePalette::Progress_NotStarted).name());

    ui->progressSlider->setStyleSheet(defaultSlider);
    ui->progressBackground->setStyleSheet(defaultSlider);
    ui->progressSlider->installEventFilter(this);
    ui->progressBackground->installEventFilter(this);
    ui->progressBackground->setAttribute(Qt::WA_Hover);
    ui->progressSlider->hide();

    updateStyle();
}

void Goal::updateStyle() {
    GuidePalette palette;
    QString headerStyle =
            QString::fromLatin1(
                "background-color: %1; border-width: 3px; border-style: solid; border-color:%2;color:%3")
            .arg(palette.getColor(GuidePalette::HeaderBackground).name())
            .arg(palette.color(QPalette::Base).name())
            .arg(palette.getColor(GuidePalette::HeaderText).name());

    ui->goalName->setStyleSheet(headerStyle);
    ui->goalTime->setStyleSheet(headerStyle);
    ui->leraningGoal->setStyleSheet(headerStyle);
    ui->week->setStyleSheet(headerStyle);
}

// disable scrolling on scrollwheel
bool Goal::eventFilter(QObject* obj, QEvent* event) {
    if (obj == ui->progressSlider && event->type() == QEvent::Wheel) {
        event->ignore();
        return true;
    }
    if (obj == ui->progressSlider && event->type() == QEvent::HoverLeave) {
        ui->progressSlider->hide();
        setProgress(ui->progressSlider->value()); // force update
        return true;
    }
    if (obj == ui->progressBackground && event->type() == QEvent::HoverEnter) {
        ui->progressSlider->show();
        return true;
    }

    return false;
}

Goal::~Goal() {
    delete ui;
}

void Goal::setName(const QString&name) {
    ui->goalName->setText(name);
}

void Goal::setGoalNumber(const QString&goalNumber) {
    ui->leraningGoal->setText(goalNumber);
}

void Goal::setTime(const QString&time) {
    ui->goalTime->setText(time);
}

void Goal::addTask(const OldGuideData::GuideGoalTasks task) {
    GuidePalette palette;

    QString indicatorText;
    QString indicatorBackgroundColour;
    QString indicatorTextColour;

    QString link = task.link;
    QString name = task.text;

    switch (task.task) {
        case OldGuideData::Work:
            indicatorText = tr("UI_WORKINDICATOR");

            indicatorBackgroundColour = palette.getColor(GuidePalette::WorkIndicatorBackground).name();
            indicatorTextColour = palette.getColor(GuidePalette::WorkIndicatorText).name();
            break;

        case OldGuideData::Watch:
            indicatorText = tr("UI_WATCHINDICATOR");

            indicatorBackgroundColour = palette.getColor(GuidePalette::WatchIndicatorBackground).name();
            indicatorTextColour = palette.getColor(GuidePalette::WatchIndicatorText).name();
            break;

        case OldGuideData::Read:
            indicatorText = tr("UI_READINDICATOR");

            indicatorBackgroundColour = palette.getColor(GuidePalette::ReadIndicatorBackground).name();
            indicatorTextColour = palette.getColor(GuidePalette::InfoIndicatorText).name();
            break;


        case OldGuideData::Process:
            indicatorText = tr("UI_PROCESSINDICATOR");

            indicatorBackgroundColour = palette.getColor(GuidePalette::ProcessIndicatorBackground).name();
            indicatorTextColour = palette.getColor(GuidePalette::InfoIndicatorText).name();
            break;

        case OldGuideData::Info:
            indicatorText = tr("UI_INFOINDICATOR");

            indicatorBackgroundColour = palette.getColor(GuidePalette::InfoIndicatorBackground).name();
            indicatorTextColour = palette.getColor(GuidePalette::InfoIndicatorText).name();
            break;
    }


    QString indicatorStyle = QString::fromLatin1(
                "background-color:%1; border-width:3px; border-style:solid; border-color:%2;color:%3")
            .arg(indicatorBackgroundColour)
            .arg(palette.color(QPalette::Base).name())
            .arg(indicatorTextColour);

    // work indicator generation
    QLabel* indicator = new QLabel(this);
    indicator->setGeometry(0, size, 90, 40);
    QFont indicatorFont;
    indicatorFont.setPointSize(22);
    indicatorFont.setBold(true);
    indicator->setFont(indicatorFont);
    indicator->setStyleSheet(indicatorStyle);
    indicator->setFrameShape(QFrame::Box);
    indicator->setLineWidth(3);
    indicator->setAlignment(Qt::AlignCenter);
    indicator->setText(indicatorText);

    // and now the text
    QLabel* textLabel = new QLabel(this);
    textLabel->setGeometry(QRect(90, size, 890, 40));
    QFont workFont;
    workFont.setPointSize(12);
    workFont.setBold(false);
    textLabel->setFont(workFont);
    textLabel->setFrameShape(QFrame::NoFrame);
    textLabel->setLineWidth(3);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setTextFormat(Qt::MarkdownText);
    textLabel->setOpenExternalLinks(true);

    if (!link.isEmpty()) {
        QString text = QString("<a href=\"%2\">%1</a>")
                .arg(name)
                .arg(link);
        textLabel->setText(text);
    }
    else
        textLabel->setText(name);

    size += 40;
}

void Goal::addWork(const QString&workName, const QString&link) {
    // First save them in the Vector for resaving
    OldGuideData::GuideGoalTasks task;

    task.task = OldGuideData::Work;
    task.text = workName;
    task.link = link;
    tasks.append(task);

    addTask(task);
}

void Goal::addWatch(const QString&watchName, const QString&link) {
    // First save them in the Vector for resaving
    OldGuideData::GuideGoalTasks task;

    task.task = OldGuideData::Watch;
    task.text = watchName;
    task.link = link;
    tasks.append(task);

    addTask(task);
}

void Goal::addRead(const QString&readName, const QString&link) {
    // First save them in the Vector for resaving
    OldGuideData::GuideGoalTasks task;
    task.task = OldGuideData::Read;
    task.text = readName;
    task.link = link;
    tasks.append(task);

    addTask(task);
}

void Goal::addProcess(const QString&processName, const QString&link) {
    // First save them in the Vector for resaving
    OldGuideData::GuideGoalTasks task;
    task.task = OldGuideData::Process;
    task.text = processName;
    task.link = link;
    tasks.append(task);

    addTask(task);
}

void Goal::addInfo(const QString&infoText, const QString&link) {
    // First save them in the Vector for resaving
    OldGuideData::GuideGoalTasks task;
    task.task = OldGuideData::Info;
    task.text = infoText;
    task.link = link;
    tasks.append(task);

    addTask(task);
}

void Goal::setWeek(const QString&week) {
    ui->week->setText(week);
}

void Goal::finalise() {
    ui->week->setGeometry(1090, 0, 100, size);
    ui->progressSlider->setGeometry(983, 3, 104, size - 6);
    ui->progressBackground->setGeometry(983, 3, 104, size - 6);
}

void Goal::setProgress(int progress, bool changedFile) {
    QString colour;
    GuidePalette palette;
    GuidePalette::GuideElements element;

    switch (progress) {
        case 2:
            element = GuidePalette::Progress_Finished;
            break;
        case 1:
            element = GuidePalette::Progress_Working;
            break;
        case 0:
            element = GuidePalette::Progress_NotStarted;
            break;
        default:
            element = GuidePalette::HeaderBackground;
            break;
    }
    colour = QString::fromLatin1("background-color:%1;")
            .arg(palette.getColor(element).name());

    ui->progressSlider->setStyleSheet(colour);
    ui->progressBackground->setStyleSheet(colour);

    QSettings settings;
    if (changedFile && !parentGuide->isInAutoSaveList) {
        parentGuide->isInAutoSaveList = true;
        APPLICATION->guidesToSave.append(parentGuide);
        APPLICATION->startAutoSaveTimer();
    }
    APPLICATION->isFileChanged = true;

    ui->progressSlider->setValue(progress);
}

void Goal::on_progressSlider_sliderMoved(int newValue) {
    setProgress(newValue);
}

OldGuideData::GuideGoals Goal::getGoal() {
    OldGuideData::GuideGoals finalGoal;

    finalGoal.name = ui->goalName->text();
    finalGoal.goalNumber = ui->leraningGoal->text();
    finalGoal.time = ui->goalTime->text();
    finalGoal.week = ui->week->text();
    finalGoal.progress = QString::number(ui->progressSlider->value());
    finalGoal.tasks = tasks;

    return finalGoal;
}
