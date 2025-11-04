//
// Created by Jesse on 04-10-2025.
//

#ifndef PLANGUIDEPLANNER_GUIDE_H
#define PLANGUIDEPLANNER_GUIDE_H

#include "guide/GuideData.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

class Goal;
class GoalFrame;
class Test;
class Report;

namespace Ui {
    class GuideBase;
}

class Guide : public QWidget {
    Q_OBJECT

public:
    explicit Guide(QWidget* parent = nullptr, const GuideData::Data* data = nullptr);

    ~Guide() override;

    void processGuide(GuideData::Data data);

    void updateStyle();

    void retranslateUi();

    GuideData::Data getGuide();

    QString name;

    bool isInAutoSaveList = false;

private:
    Ui::GuideBase* ui;
    QLayout* mainLayout;

    QSpacerItem* lastSpacer;

    QVector<GoalFrame *> goalFrames;
    QVector<Test *> testWidgets;
    QVector<Report *> reportWidgets;

    QVector<GuideData::ObjectTypes> objectOrder;
    QVector<GuideData::Object> nonGoalObjects;

    GuideData::Data baseData;
};


#endif //PLANGUIDEPLANNER_GUIDE_H
