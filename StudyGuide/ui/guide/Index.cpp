//
// Created by Jesse on 3 okt. 2023.
//

#include <QLabel>

#include "ui_Index.h"

#include "Index.h"
#include "Goal.h"
#include "themes/GuidePalette.h"


Index::Index(QWidget *parent) : QWidget(parent), ui(new Ui::Index) {
    ui->setupUi(this);
    updateStyle();
}

void Index::updateStyle() {
    GuidePalette palette;

    QString frameStyle = QString::fromLatin1("background-color: %1;").arg(palette.color(QPalette::Base).name());
    QString textStyle = QString::fromLatin1("color: %1;").arg(palette.getColor(GuidePalette::ObjectText).name());
    QString nameStyle = QString::fromLatin1("background-color: %1;color:%2;")
            .arg(palette.getColor(GuidePalette::HeaderBackground).name())
            .arg(palette.getColor(GuidePalette::HeaderText).name());

    QString borderColourString = QString::fromLatin1("border-width:3px;border-style:solid;border-color: %1;").arg(
            palette.color(QPalette::Base).name());

    ui->frame->setStyleSheet(frameStyle);
    ui->subjectIndex->setStyleSheet(textStyle);
    ui->time->setStyleSheet(nameStyle + borderColourString);
    ui->done->setStyleSheet(nameStyle + borderColourString);
    ui->week->setStyleSheet(nameStyle + borderColourString);

}

Index::~Index() {
    delete ui;
    for (int i = 0; i < goals.size(); i++) {
        delete goals.at(i);
    }
}

void Index::addGoal(Goal *goal) {
    goals.append(goal);
    goal->setParent(ui->frame);
    goal->setGeometry(20, size, 1240, goal->size);
    size += goal->size;
}

void Index::finalise() {
    ui->frame->resize(1240, size + 5);
}

GuideData::GuideObject Index::getGuideObject() {
    GuideData::GuideObject object;
    object.objectType = GuideData::Index;
    // get all the goals.
    for (Goal *goal: goals) {
        object.goals.append(goal->getGoal());
    }
    return object;
}
