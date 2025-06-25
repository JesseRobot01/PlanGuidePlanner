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

void Goal::addWork(const QString&workName, const QString&link) {
    // First save them in the Vector for resaving
    GuideData::GuideGoalPrefixes prefix;
    GuidePalette palette;
    prefix.prefix = GuideData::Work;
    prefix.prefixText = workName;
    prefix.link = link;
    prefixes.append(prefix);

    QString indicatorStyle = QString::fromLatin1(
                "background-color:%1; border-width:3px; border-style:solid; border-color:%2;color:%3")
            .arg(palette.getColor(GuidePalette::WorkIndicatorBackground).name())
            .arg(palette.color(QPalette::Base).name())
            .arg(palette.getColor(GuidePalette::WorkIndicatorText).name());

    // work indicator generation
    QLabel* workIndicator = new QLabel(this);
    workIndicator->setGeometry(0, size, 90, 40);
    QFont indicatorFont;
    indicatorFont.setPointSize(22);
    indicatorFont.setBold(true);
    workIndicator->setFont(indicatorFont);
    workIndicator->setStyleSheet(indicatorStyle);
    workIndicator->setFrameShape(QFrame::Box);
    workIndicator->setLineWidth(3);
    workIndicator->setAlignment(Qt::AlignCenter);
    workIndicator->setText(tr("UI_WORKINDICATOR"));

    // and now the text
    QLabel* work = new QLabel(this);
    work->setGeometry(QRect(90, size, 890, 40));
    QFont workFont;
    workFont.setPointSize(12);
    workFont.setBold(false);
    work->setFont(workFont);
    work->setFrameShape(QFrame::NoFrame);
    work->setLineWidth(3);
    work->setAlignment(Qt::AlignCenter);

    if (!link.isEmpty()) {
        QString workText = QString("<a href=\"%2\">%1</a>")
                .arg(workName)
                .arg(link);
        work->setOpenExternalLinks(true);
        work->setText(workText);
    }
    else
        work->setText(workName);

    size += 40;
}

void Goal::addWatch(const QString&watchName, const QString&link) {
    // First save them in the Vector for resaving
    GuideData::GuideGoalPrefixes prefix;
    GuidePalette palette;

    QString indicatorStyle = QString::fromLatin1(
                "background-color:%1; border-width:3px; border-style:solid; border-color:%2;color:%3")
            .arg(palette.getColor(GuidePalette::WatchIndicatorBackground).name())
            .arg(palette.color(QPalette::Base).name())
            .arg(palette.getColor(GuidePalette::WatchIndicatorText).name());

    prefix.prefix = GuideData::Watch;
    prefix.prefixText = watchName;
    prefix.link = link;
    prefixes.append(prefix);

    // watch indicator generation
    QLabel* watchIndicator = new QLabel(this);
    watchIndicator->setGeometry(0, size, 90, 40);
    QFont indicatorFont;
    indicatorFont.setPointSize(22);
    indicatorFont.setBold(true);
    watchIndicator->setFont(indicatorFont);
    watchIndicator->setStyleSheet(indicatorStyle);
    watchIndicator->setFrameShape(QFrame::Box);
    watchIndicator->setLineWidth(3);
    watchIndicator->setAlignment(Qt::AlignCenter);
    watchIndicator->setText(tr("UI_WATCHINDICATOR"));

    // and now the text
    QLabel* watch = new QLabel(this);
    watch->setGeometry(QRect(90, size, 890, 40));
    QFont workFont;
    workFont.setPointSize(12);
    workFont.setBold(false);
    watch->setFont(workFont);
    watch->setFrameShape(QFrame::NoFrame);
    watch->setLineWidth(3);
    watch->setAlignment(Qt::AlignCenter);

    if (!link.isEmpty()) {
        QString watchText = QString("<a href=\"%2\">%1</a>")
                .arg(watchName)
                .arg(link);

        watch->setOpenExternalLinks(true);
        watch->setText(watchText);
    }
    else
        watch->setText(watchName);

    size += 40;
}

void Goal::addRead(const QString&readName, const QString&link) {
    // First save them in the Vector for resaving
    GuideData::GuideGoalPrefixes prefix;
    GuidePalette palette;
    prefix.prefix = GuideData::Read;
    prefix.prefixText = readName;
    prefix.link = link;
    prefixes.append(prefix);

    QString indicatorStyle = QString::fromLatin1(
                "background-color:%1; border-width:3px; border-style:solid; border-color:%2; color:%3;")
            .arg(palette.getColor(GuidePalette::ReadIndicatorBackground).name())
            .arg(palette.color(QPalette::Base).name())
            .arg(palette.getColor(GuidePalette::ReadIndicatorText).name());


    // Read indicator generation
    QLabel* readIndicator = new QLabel(this);
    readIndicator->setGeometry(0, size, 90, 40);
    QFont indicatorFont;
    indicatorFont.setPointSize(22);
    indicatorFont.setBold(true);
    readIndicator->setFont(indicatorFont);
    readIndicator->setStyleSheet(indicatorStyle);
    readIndicator->setFrameShape(QFrame::Box);
    readIndicator->setLineWidth(3);
    readIndicator->setAlignment(Qt::AlignCenter);
    readIndicator->setText(tr("UI_READINDICATOR"));

    // and now the text
    QLabel* read = new QLabel(this);
    read->setGeometry(QRect(90, size, 890, 40));
    QFont workFont;
    workFont.setPointSize(12);
    workFont.setBold(false);
    read->setFont(workFont);
    read->setFrameShape(QFrame::NoFrame);
    read->setLineWidth(3);
    read->setAlignment(Qt::AlignCenter);

    if (!link.isEmpty()) {
        QString readText = QString("<a href=\"%2\">%1</a>")
                .arg(readName)
                .arg(link);

        read->setOpenExternalLinks(true);
        read->setText(readText);
    }
    else
        read->setText(readName);

    size += 40;
}

void Goal::addProcess(const QString&processName, const QString&link) {
    // First save them in the Vector for resaving
    GuideData::GuideGoalPrefixes prefix;
    GuidePalette palette;
    prefix.prefix = GuideData::Process;
    prefix.prefixText = processName;
    prefix.link = link;
    prefixes.append(prefix);

    QString indicatorStyle = QString::fromLatin1(
                "background-color:%1; border-width:3px; border-style:solid; border-color:%2;color:%3")
            .arg(palette.getColor(GuidePalette::ProcessIndicatorBackground).name())
            .arg(palette.color(QPalette::Base).name())
            .arg(palette.getColor(GuidePalette::ProcessIndicatorText).name());

    // Process indicator generation
    QLabel* processIndicator = new QLabel(this);
    processIndicator->setGeometry(0, size, 90, 40);
    QFont indicatorFont;
    indicatorFont.setPointSize(22);
    indicatorFont.setBold(true);
    processIndicator->setFont(indicatorFont);
    processIndicator->setStyleSheet(indicatorStyle);
    processIndicator->setAlignment(Qt::AlignCenter);
    processIndicator->setText(tr("UI_PROCESSINDICATOR"));

    // and now the text
    QLabel* process = new QLabel(this);
    process->setGeometry(QRect(90, size, 890, 40));
    QFont workFont;
    workFont.setPointSize(12);
    workFont.setBold(false);
    process->setFont(workFont);
    process->setFrameShape(QFrame::NoFrame);
    process->setLineWidth(3);
    process->setAlignment(Qt::AlignCenter);

    if (!link.isEmpty()) {
        QString processText = QString("<a href=\"%2\">%1</a>")
                .arg(processName)
                .arg(link);

        process->setOpenExternalLinks(true);
        process->setText(processText);
    }
    else
        process->setText(processName);

    size += 40;
}

void Goal::addInfo(const QString&infoText, const QString&link) {
    // First save them in the Vector for resaving
    GuideData::GuideGoalPrefixes prefix;
    GuidePalette palette;
    prefix.prefix = GuideData::Info;
    prefix.prefixText = infoText;
    prefix.link = link;
    prefixes.append(prefix);

    QString indicatorStyle = QString::fromLatin1(
                "background-color:%1; border-width:3px; border-style:solid; border-color:%2;color:%3")
            .arg(palette.getColor(GuidePalette::InfoIndicatorBackground).name())
            .arg(palette.color(QPalette::Base).name())
            .arg(palette.getColor(GuidePalette::InfoIndicatorText).name());

    // Info indicator generation
    QLabel* InfoIndicator = new QLabel(this);
    InfoIndicator->setGeometry(0, size, 90, 40);
    QFont indicatorFont;
    indicatorFont.setPointSize(22);
    indicatorFont.setBold(true);
    InfoIndicator->setFont(indicatorFont);
    InfoIndicator->setStyleSheet(indicatorStyle);
    InfoIndicator->setAlignment(Qt::AlignCenter);
    InfoIndicator->setText(tr("UI_INFOINDICATOR"));

    // and now the text
    QLabel* info = new QLabel(this);
    info->setGeometry(QRect(90, size, 890, 40));
    QFont workFont;
    workFont.setPointSize(12);
    workFont.setBold(false);
    info->setFont(workFont);
    info->setFrameShape(QFrame::NoFrame);
    info->setLineWidth(3);
    info->setAlignment(Qt::AlignCenter);

    if (!link.isEmpty()) {
        QString infoTextLink = QString("<a href=\"%2\">%1</a>")
                .arg(infoText)
                .arg(link);

        info->setOpenExternalLinks(true);
        info->setText(infoTextLink);
    }
    else
        info->setText(infoText);

    size += 40;
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

    switch (progress) {
        case 2:
            colour = QString::fromLatin1("background-color:%1;").arg(
                palette.getColor(GuidePalette::Progress_Finished).name());

            ui->progressSlider->setStyleSheet(colour);
            ui->progressBackground->setStyleSheet(colour);
            break;
        case 1:
            colour = QString::fromLatin1("background-color:%1;").arg(
                palette.getColor(GuidePalette::Progress_Working).name());

            ui->progressSlider->setStyleSheet(colour);
            ui->progressBackground->setStyleSheet(colour);
            break;
        case 0:
            colour = QString::fromLatin1("background-color:%1;").arg(
                palette.getColor(GuidePalette::Progress_NotStarted).name());

            ui->progressSlider->setStyleSheet(colour);
            ui->progressBackground->setStyleSheet(colour);
            break;
        default:
            colour = QString::fromLatin1("background-color:%1;").arg(
                palette.getColor(GuidePalette::HeaderBackground).name());

            ui->progressSlider->setStyleSheet(colour);
            ui->progressBackground->setStyleSheet(colour);
            break;
    }
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
