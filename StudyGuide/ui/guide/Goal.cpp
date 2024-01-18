//
// Created by Jesse on 3 okt. 2023.
//

#include "Goal.h"
#include "ui_Goal.h"

Goal::Goal(QWidget *parent) : QWidget(parent), ui(new Ui::Goal) {
    ui->setupUi(this);
    ui->progressSlider->setStyleSheet("background-color: rgb(234, 67, 53);");
    ui->progressBackground->setStyleSheet("background-color: rgb(234, 67, 53); border-color:white;color : white;");
    ui->progressSlider->installEventFilter(this);
    ui->progressBackground->installEventFilter(this);
    ui->progressBackground->setAttribute(Qt::WA_Hover);
    ui->progressSlider->hide();
}

// disable scrolling on scrollwheel
bool Goal::eventFilter(QObject *obj, QEvent *event) {
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

void Goal::setName(const QString &name) {
    ui->goalName->setText(name);
}

void Goal::setGoalNumber(const QString &goalNumber) {
    ui->leraningGoal->setText(goalNumber);
}

void Goal::setTime(const QString &time) {
    ui->goalTime->setText(time);
}

void Goal::addWork(const QString &workName) {
    // First save them in the Vector for resaving
    GuideData::GuideGoalPrefixes prefix;
    prefix.prefix = GuideData::Work;
    prefix.prefixText = workName;
    prefixes.append(prefix);

    // work indicator generation
    QLabel *workIndicator = new QLabel(this);
    workIndicator->setGeometry(0, size, 90, 40);
    QFont indicatorFont;
    indicatorFont.setPointSize(22);
    indicatorFont.setBold(true);
    workIndicator->setFont(indicatorFont);
    workIndicator->setStyleSheet("background-color:rgb(147, 196, 125);color : white;border-color : white;");
    workIndicator->setFrameShape(QFrame::Box);
    workIndicator->setLineWidth(3);
    workIndicator->setAlignment(Qt::AlignCenter);
    workIndicator->setText(tr("UI_WORKINDICATOR"));

    // and now the text
    QLabel *work = new QLabel(this);
    work->setGeometry(QRect(90, size, 890, 40));
    QFont workFont;
    workFont.setPointSize(12);
    workFont.setBold(false);
    work->setFont(workFont);
    work->setStyleSheet(QString::fromUtf8("border-color : white;"));
    work->setFrameShape(QFrame::NoFrame);
    work->setLineWidth(3);
    work->setAlignment(Qt::AlignCenter);
    work->setText(workName);

    size += 40;
}

void Goal::addWatch(const QString &watchName) {
    // First save them in the Vector for resaving
    GuideData::GuideGoalPrefixes prefix;
    prefix.prefix = GuideData::Watch;
    prefix.prefixText = watchName;
    prefixes.append(prefix);

    // watch indicator generation
    QLabel *watchIndicator = new QLabel(this);
    watchIndicator->setGeometry(0, size, 90, 40);
    QFont indicatorFont;
    indicatorFont.setPointSize(22);
    indicatorFont.setBold(true);
    watchIndicator->setFont(indicatorFont);
    watchIndicator->setStyleSheet(
            QString::fromUtf8("background-color:rgb(111, 168, 220);color : white;border-color : white;"));
    watchIndicator->setFrameShape(QFrame::Box);
    watchIndicator->setLineWidth(3);
    watchIndicator->setAlignment(Qt::AlignCenter);
    watchIndicator->setText(tr("UI_WATCHINDICATOR"));

    // and now the text
    QLabel *watch = new QLabel(this);
    watch->setGeometry(QRect(90, size, 890, 40));
    QFont workFont;
    workFont.setPointSize(12);
    workFont.setBold(false);
    watch->setFont(workFont);
    watch->setStyleSheet(QString::fromUtf8("border-color : white;"));
    watch->setFrameShape(QFrame::NoFrame);
    watch->setLineWidth(3);
    watch->setAlignment(Qt::AlignCenter);
    watch->setText(watchName);

    size += 40;
}

void Goal::addRead(const QString &readName) {
    // First save them in the Vector for resaving
    GuideData::GuideGoalPrefixes prefix;
    prefix.prefix = GuideData::Read;
    prefix.prefixText = readName;
    prefixes.append(prefix);


    // Read indicator generation
    QLabel *readIndicator = new QLabel(this);
    readIndicator->setGeometry(0, size, 90, 40);
    QFont indicatorFont;
    indicatorFont.setPointSize(22);
    indicatorFont.setBold(true);
    readIndicator->setFont(indicatorFont);
    readIndicator->setStyleSheet(
            QString::fromUtf8("background-color:rgb(255, 217, 102);color : white;border-color : white;"));
    readIndicator->setFrameShape(QFrame::Box);
    readIndicator->setLineWidth(3);
    readIndicator->setAlignment(Qt::AlignCenter);
    readIndicator->setText(tr("UI_READINDICATOR"));

    // and now the text
    QLabel *read = new QLabel(this);
    read->setGeometry(QRect(90, size, 890, 40));
    QFont workFont;
    workFont.setPointSize(12);
    workFont.setBold(false);
    read->setFont(workFont);
    read->setStyleSheet(QString::fromUtf8("border-color : white;"));
    read->setFrameShape(QFrame::NoFrame);
    read->setLineWidth(3);
    read->setAlignment(Qt::AlignCenter);
    read->setText(readName);

    size += 40;
}

void Goal::addProcess(const QString &processName) {
    // First save them in the Vector for resaving
    GuideData::GuideGoalPrefixes prefix;
    prefix.prefix = GuideData::Process;
    prefix.prefixText = processName;
    prefixes.append(prefix);

    // Process indicator generation
    QLabel *processIndicator = new QLabel(this);
    processIndicator->setGeometry(0, size, 90, 40);
    QFont indicatorFont;
    indicatorFont.setPointSize(22);
    indicatorFont.setBold(true);
    processIndicator->setFont(indicatorFont);
    processIndicator->setStyleSheet(
            QString::fromUtf8("background-color: rgb(142, 124, 195);color : white;border-color : white;"));
    processIndicator->setFrameShape(QFrame::Box);
    processIndicator->setLineWidth(3);
    processIndicator->setAlignment(Qt::AlignCenter);
    processIndicator->setText(tr("UI_PROCESSINDICATOR"));

    // and now the text
    QLabel *process = new QLabel(this);
    process->setGeometry(QRect(90, size, 890, 40));
    QFont workFont;
    workFont.setPointSize(12);
    workFont.setBold(false);
    process->setFont(workFont);
    process->setStyleSheet(QString::fromUtf8("border-color : white;"));
    process->setFrameShape(QFrame::NoFrame);
    process->setLineWidth(3);
    process->setAlignment(Qt::AlignCenter);
    process->setText(processName);

    size += 40;
}

void Goal::setWeek(const QString &week) {
    ui->week->setText(week);
}

void Goal::finalise() {
    ui->week->setGeometry(1090, 0, 100, size);
    ui->progressSlider->setGeometry(983, 3, 104, size - 6);
    ui->progressBackground->setGeometry(983, 3, 104, size - 6);
}

void Goal::setProgress(int progress) {
    switch (progress) {
        case 2:
            ui->progressSlider->setStyleSheet("background-color: rgb(52, 168, 83);");
            ui->progressBackground->setStyleSheet(
                    "background-color: rgb(52, 168, 83);border-color: white;color : white;");
            break;
        case 1:
            ui->progressSlider->setStyleSheet("background-color: rgb(255, 153, 0);");
            ui->progressBackground->setStyleSheet(
                    "background-color: rgb(255, 153, 0);border-color: white;color : white;");
            break;
        case 0:
            ui->progressSlider->setStyleSheet("background-color: rgb(234, 67, 53);");
            ui->progressBackground->setStyleSheet(
                    "background-color: rgb(234, 67, 53);border-color: white;color : white;");
            break;
        default:
            ui->progressSlider->setStyleSheet("background-color: rgb(8, 73, 149);");
            ui->progressBackground->setStyleSheet(
                    "background-color: rgb(8, 73, 149);border-color: white;color : white;");
            break;
    }
    ui->progressSlider->setValue(progress);
}

void Goal::on_progressSlider_sliderMoved(int newValue) {
    setProgress(newValue);
}

GuideData::GuideGoals Goal::getGoal() {
    GuideData::GuideGoals finalGoal;

    finalGoal.name = ui->goalName->text();
    finalGoal.goalNumber = ui->leraningGoal->text();
    finalGoal.time = ui->goalTime->text();
    finalGoal.week = ui->week->text();
    finalGoal.progress = QString::number(ui->progressSlider->value());
    finalGoal.prefixes = prefixes;

    return finalGoal;
}
