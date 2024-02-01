//
// Created by Jesse on 3 okt. 2023.
//

#ifndef STUDYGUIDE_GOAL_H
#define STUDYGUIDE_GOAL_H


#include <QWidget>
#include <QFile>

#include "guide/GuideData.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class Goal;
}

QT_END_NAMESPACE

class Goal : public QWidget {
Q_OBJECT

public:
    explicit Goal(QWidget *parent = nullptr);

    ~Goal() override;

    bool eventFilter(QObject *obj, QEvent *event);

    void addWork(const QString &workName, const QString&link = "");

    void setName(const QString &name);

    void setGoalNumber(const QString &goalNumber);

    void setTime(const QString &time);

    void addWatch(const QString &watchName, const QString&link = "");

    void addRead(const QString &readName, const QString&link = "");

    void addProcess(const QString &processName, const QString&link = "");

    void setProgress(int progress);

    void setProgress(const QString &progress) {
        setProgress(progress.toInt());
    }

    void finalise();

    void setWeek(const QString &week);

    GuideData::GuideGoals getGoal();

    int size = 40;

private slots:

    void on_progressSlider_sliderMoved(int newValue);

private:
    Ui::Goal *ui;
    QVector<GuideData::GuideGoalPrefixes> prefixes;

    void updateStyle();
};


#endif //STUDYGUIDE_GOAL_H
