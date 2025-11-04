//
// Created by Jesse on 09-10-2025.
//

#ifndef PLANGUIDEPLANNER_GOAL_H
#define PLANGUIDEPLANNER_GOAL_H

#include <QFrame>
#include <QLabel>

#include "guide/GuideData.h"


class Guide;
class GoalFrame;

QT_BEGIN_NAMESPACE

namespace Ui {
    class GoalFrame;
    class Goal;
}

QT_END_NAMESPACE

class Goal : public QWidget {
    Q_OBJECT

public:
    explicit Goal(GoalFrame* parent, GuideData::Object* goal = nullptr);

    ~Goal() override;

    bool eventFilter(QObject* obj, QEvent* event) override;

    void processGoal(GuideData::Object goal);

    void addTask(GuideData::Task task);

    void setProgress(int progress, bool changedFile = true);

    void setProgress(GuideData::Progress progress, bool changedFile = true);

    void updateStyle();

    void retranslateUi();

    GuideData::Object getGoal();

private slots:
    void on_progressSlider_sliderMoved(int newValue);

private:
    Ui::Goal* ui;

    QVector<GuideData::TaskTypes> taskOrder;
    QVector<QLabel *> taskLabels;
    QVector<QLabel *> taskNames;
    GuideData::Object baseGoal;
    GoalFrame* parent;
};

class GoalFrame : public QFrame {
    Q_OBJECT

public:
    explicit GoalFrame(Guide* parent, QVector<GuideData::Object>* goals = nullptr);

    ~GoalFrame() override;

    void updateStyle();

    void retranslateUi();

    QVector<GuideData::Object> getGoals();

    Guide* parent;

private:
    Ui::GoalFrame* ui;
    QVector<Goal *> goals;
};


#endif //PLANGUIDEPLANNER_GOAL_H
