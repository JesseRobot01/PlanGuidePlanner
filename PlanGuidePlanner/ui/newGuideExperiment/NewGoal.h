//
// Created by Jesse on 09-10-2025.
//

#ifndef PLANGUIDEPLANNER_GOALFRAME_H
#define PLANGUIDEPLANNER_GOALFRAME_H

#include <QFrame>
#include <QLabel>

#include "guide/NewGuideData.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class GoalFrame;
    class NewGoal;
}

QT_END_NAMESPACE

class NewGoal : public QWidget {
    Q_OBJECT

public:
    explicit NewGoal(QWidget* parent = nullptr, NewGuideData::Object* goal = nullptr);

    void processGoal(NewGuideData::Object goal);

    void addTask(NewGuideData::Task task);

    bool eventFilter(QObject* obj, QEvent* event) override;

    void updateStyle();

    void setProgress(int progress, bool changedFile = true);

    void setProgress(NewGuideData::Progress progress, bool changedFile = true);

    void retranslateUi();

    NewGuideData::Object getGoal();

    ~NewGoal() override;

private slots:

void on_progressSlider_sliderMoved(int newValue);

private:
    Ui::NewGoal* ui;

    QVector<NewGuideData::TaskTypes> taskOrder;
    QVector<QLabel*> taskLabels;
    QVector<QLabel*> taskNames;
    NewGuideData::Object baseGoal;

};

class GoalFrame : public QFrame {
    Q_OBJECT

public:
    explicit GoalFrame(QWidget* parent = nullptr, QVector<NewGuideData::Object>* goals = nullptr);

    ~GoalFrame() override;

    void updateStyle();

    void retranslateUi();

    QVector<NewGuideData::Object> getGoals();

private:
    Ui::GoalFrame* ui;
    QVector<NewGoal*> goals;
};


#endif //PLANGUIDEPLANNER_GOALFRAME_H
