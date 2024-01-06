//
// Created by Jesse on 3 okt. 2023.
//

#include <QLabel>

#include "ui_Index.h"

#include "Index.h"
#include "Goal.h"


Index::Index(QWidget* parent) : QWidget(parent), ui(new Ui::Index) {
    ui->setupUi(this);
}

Index::~Index() {
    delete ui;
    for (int i = 0; i < goals.size(); i++) {
        delete goals.at(i);
    }
}

void Index::addGoal(Goal* goal) {
    goals.append(goal);
    goal->setParent(ui->Frame);
    goal->setGeometry(20, size, 1240, goal->size);
    size += goal->size;
}

void Index::finalise() {
    ui->Frame->resize(1240, size + 5);
}

GuideData::GuideObject Index::getGuideObject() {
    GuideData::GuideObject object;
    object.objectType = GuideData::Index;
    // get all the goals.
    for (Goal* goal: goals) {
        object.goals.append(goal->getGoal());
    }
    return object;
}
