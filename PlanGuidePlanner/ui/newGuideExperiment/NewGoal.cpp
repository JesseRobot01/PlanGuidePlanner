//
// Created by Jesse on 09-10-2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GoalFrame.h" resolved

#include "NewGoal.h"
#include "ui_GoalFrame.h"
#include "ui_NewGoal.h"
#include "guide/NewGuideData.h"
#include "themes/GuidePalette.h"

NewGoal::NewGoal(QWidget* parent, NewGuideData::Object* goal) : QWidget(parent), ui(new Ui::NewGoal) {
    ui->setupUi(this);

    ui->progressSlider->installEventFilter(this);
    ui->sliderFrame->installEventFilter(this);
    ui->sliderFrame->setAttribute(Qt::WA_Hover);

    if (goal)
        processGoal(*goal);
    else
        qCritical() << "Goal created without an object!";
}

void NewGoal::processGoal(NewGuideData::Object goal) {
    if (goal.type != NewGuideData::Goal)
        qWarning() << "Object inside Goal is not a Goal!";

    baseGoal = goal;
    setProgress(goal.progress);
    ui->progressSlider->hide();

    ui->name->setText(goal.name);
    ui->time->setText(QString::number(goal.time));
    ui->number->setText(goal.number);
    ui->week->setText(QString::number(goal.date.weekNumber()));


    for (auto task: goal.tasks) {
        addTask(task);
    }
}

void NewGoal::addTask(NewGuideData::Task task) {
    QWidget* goal = new QWidget(ui->text);
    QLayout* layout = new QHBoxLayout(goal);

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QFont labelFont;
    labelFont.setPointSize(21);
    labelFont.setBold(true);

    QFont textFont;
    textFont.setPointSize(12);

    QSizePolicy labelSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Minimum);
    labelSizePolicy.setHorizontalStretch(0);
    labelSizePolicy.setVerticalStretch(0);

    QSizePolicy textSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
    textSizePolicy.setHorizontalStretch(0);
    textSizePolicy.setVerticalStretch(0);

    auto* taskLabel = new QLabel(goal);
    labelSizePolicy.setHeightForWidth(taskLabel->sizePolicy().hasHeightForWidth());
    taskLabel->setSizePolicy(labelSizePolicy);
    taskLabel->setMinimumSize(QSize(0, 40));
    taskLabel->setMaximumSize(QSize(90, 16777215));

    taskLabel->setFont(labelFont);
    taskLabel->setFrameShape(QFrame::Shape::Box);
    taskLabel->setLineWidth(3);
    taskLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);


    auto* taskText = new QLabel(goal);
    textSizePolicy.setHeightForWidth(taskText->sizePolicy().hasHeightForWidth());
    taskText->setSizePolicy(textSizePolicy);
    taskText->setMinimumSize(QSize(0, 40));

    taskText->setFont(textFont);
    taskText->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
    taskText->setFrameShape(QFrame::Shape::Box);
    taskText->setLineWidth(3);
    taskText->setAlignment(Qt::AlignmentFlag::AlignCenter);
    taskText->setWordWrap(true);

    taskText->setText(task.text);


    layout->addWidget(taskLabel);
    layout->addWidget(taskText);

    ui->tasks->addWidget(goal);

    taskOrder.append(task.task);
    taskLabels.append(taskLabel);
    taskNames.append(taskText);
}

bool NewGoal::eventFilter(QObject* obj, QEvent* event) {
    if (obj == ui->progressSlider && event->type() == QEvent::Wheel) {
        event->ignore();
        return true;
    }
    if (obj == ui->sliderFrame && event->type() == QEvent::HoverLeave) {
        ui->progressSlider->hide();
        setProgress(ui->progressSlider->value()); // force update
        return true;
    }
    if (obj == ui->sliderFrame && event->type() == QEvent::HoverEnter) {
        ui->progressSlider->show();
        return true;
    }

    return false;
}

void NewGoal::on_progressSlider_sliderMoved(int newValue) {
    setProgress(newValue);
}

void NewGoal::updateStyle() {
    GuidePalette palette;

    const QString borderStyle = QString::fromLatin1("border-width:3px;border-style:solid;border-color: %1;")
            .arg(palette.color(QPalette::Base).name());

    const QString baseStyle = "background-color:%1; color:%2; " + borderStyle;

    const QString headerStyle = baseStyle
            .arg(palette.getColor(GuidePalette::HeaderBackground).name())
            .arg(palette.getColor(GuidePalette::HeaderText).name());

    const QString workIndicatorStyle = baseStyle
            .arg(palette.getColor(GuidePalette::WorkIndicatorBackground).name())
            .arg(palette.getColor(GuidePalette::WorkIndicatorText).name());

    const QString watchIndicatorStyle = baseStyle
            .arg(palette.getColor(GuidePalette::WatchIndicatorBackground).name())
            .arg(palette.getColor(GuidePalette::WatchIndicatorText).name());

    const QString readIndicatorStyle = baseStyle
            .arg(palette.getColor(GuidePalette::ReadIndicatorBackground).name())
            .arg(palette.getColor(GuidePalette::ReadIndicatorText).name());

    const QString processIndicatorStyle = baseStyle
            .arg(palette.getColor(GuidePalette::ProcessIndicatorBackground).name())
            .arg(palette.getColor(GuidePalette::ProcessIndicatorText).name());

    const QString infoIndicatorStyle = baseStyle
            .arg(palette.getColor(GuidePalette::InfoIndicatorBackground).name())
            .arg(palette.getColor(GuidePalette::InfoIndicatorText).name());

    ui->name->setStyleSheet(headerStyle);
    ui->time->setStyleSheet(headerStyle);
    ui->number->setStyleSheet(headerStyle);
    ui->week->setStyleSheet(headerStyle);

    for (int i = 0; i < taskOrder.size(); i++) {
        switch (taskOrder.at(i)) {
            case NewGuideData::Work:
                taskLabels.at(i)->setStyleSheet(workIndicatorStyle);
                break;
            case NewGuideData::Read:
                taskLabels.at(i)->setStyleSheet(readIndicatorStyle);
                break;
            case NewGuideData::Watch:
                taskLabels.at(i)->setStyleSheet(watchIndicatorStyle);
                break;
            case NewGuideData::Process:
                taskLabels.at(i)->setStyleSheet(processIndicatorStyle);
                break;
            case NewGuideData::Info:
                taskLabels.at(i)->setStyleSheet(infoIndicatorStyle);
                break;
        }
        taskNames.at(i)->setStyleSheet(borderStyle);
    }

    // Set progress Style
    QString ProgressColour;
    GuidePalette::GuideElements progress;

    switch (static_cast<NewGuideData::Progress>(ui->progressSlider->value())) {
        case NewGuideData::Finished:
            progress = GuidePalette::Progress_Finished;
            break;
        case NewGuideData::Working:
            progress = GuidePalette::Progress_Working;
            break;
        case NewGuideData::NotStarted:
            progress = GuidePalette::Progress_NotStarted;
            break;
        default:
            progress = GuidePalette::HeaderBackground;
            break;
    }

    ProgressColour = QString::fromLatin1("background-color:%1;")
            .arg(palette.getColor(progress).name());

    ui->progressSlider->setStyleSheet(ProgressColour + "border-width: 0px;");
    ui->sliderFrame->setStyleSheet(ProgressColour + borderStyle);
}

void NewGoal::setProgress(int progress, bool changedFile) {
    if (progress < 3 && progress >= 0) {
        setProgress(static_cast<NewGuideData::Progress>(progress), changedFile);
    }
}

void NewGoal::setProgress(NewGuideData::Progress progress, bool changedFile) {
    QString colour;
    GuidePalette palette;
    GuidePalette::GuideElements element;

    switch (progress) {
        case NewGuideData::Finished:
            element = GuidePalette::Progress_Finished;
            break;
        case NewGuideData::Working:
            element = GuidePalette::Progress_Working;
            break;
        case NewGuideData::NotStarted:
            element = GuidePalette::Progress_NotStarted;
            break;
        default:
            element = GuidePalette::HeaderBackground;
            break;
    }

    colour = QString::fromLatin1("background-color:%1;border-width:3px;border-style:solid;border-color: %2;")
            .arg(palette.getColor(element).name())
            .arg(palette.color(QPalette::Base).name());

    QString border = QString::fromLatin1("border-width:3px;border-style:solid;border-color: %1;")
            .arg(palette.color(QPalette::Base).name());

    ui->progressSlider->setStyleSheet(colour + "border-width: 0px;");
    ui->sliderFrame->setStyleSheet(colour + border);

    ui->progressSlider->setValue(progress);
}

void NewGoal::retranslateUi() {
    for (int i = 0; i < taskOrder.size(); i++) {
        switch (taskOrder.at(i)) {
            case NewGuideData::Work:
                taskLabels.at(i)->setText("UI_WORKINDICATOR");
                break;
            case NewGuideData::Watch:
                taskLabels.at(i)->setText("UI_WATCHINDICATOR");
                break;
            case NewGuideData::Read:
                taskLabels.at(i)->setText("UI_READINDICATOR");
                break;
            case NewGuideData::Process:
                taskLabels.at(i)->setText("UI_PROCESSINDICATOR");
                break;
            case NewGuideData::Info:
                taskLabels.at(i)->setText("UI_INFOINDICATOR");
                break;
        }
    }
}

NewGuideData::Object NewGoal::getGoal() {
    baseGoal.setProgressFromInt(ui->progressSlider->value());
    return baseGoal;
}

NewGoal::~NewGoal() {
    delete ui;
}

GoalFrame::GoalFrame(QWidget* parent, QVector<NewGuideData::Object>* goalsI) : QFrame(parent), ui(new Ui::GoalFrame) {
    ui->setupUi(this);
    if (goalsI)
        for (auto goal: *goalsI) {
            NewGoal* newGoal = new NewGoal(this, &goal);
            ui->verticalLayout->addWidget(newGoal);
            goals.append(newGoal);
        }
    else
        qCritical() << "No Goals in Goalframe!";
}

GoalFrame::~GoalFrame() {
    delete ui;
}

void GoalFrame::updateStyle() {
    GuidePalette palette;

    const QString frameStyle = QString::fromLatin1("background-color: %1;").arg(palette.color(QPalette::Base).name());
    const QString textStyle = QString::fromLatin1("color: %1;").arg(palette.getColor(GuidePalette::ObjectText).name());
    const QString labelStyle = QString::fromLatin1(
                "background-color: %1; color: %2; border-width:3px;border-style:solid;border-color: %3;")
            .arg(palette.getColor(GuidePalette::HeaderBackground).name())
            .arg(palette.getColor(GuidePalette::HeaderText).name())
            .arg(palette.color(QPalette::Base).name());


    setStyleSheet(frameStyle);
    ui->subjectLabel->setStyleSheet(textStyle);
    ui->time->setStyleSheet(labelStyle);
    ui->done->setStyleSheet(labelStyle);
    ui->week->setStyleSheet(labelStyle);

    for (auto goal: goals)
        goal->updateStyle();
}

void GoalFrame::retranslateUi() {
    ui->retranslateUi(this);

    for (auto goal: goals)
        goal->retranslateUi();
}

QVector<NewGuideData::Object> GoalFrame::getGoals() {
    QVector<NewGuideData::Object> goalData;

    for (auto goal: goals)
        goalData.append(goal->getGoal());

    return goalData;
}
