//
// Created by Jesse on 3 okt. 2023.
//

#ifndef STUDYGUIDE_GOAL_H
#define STUDYGUIDE_GOAL_H


#include <QWidget>
#include <QFile>

#include "guide/GuideData.h"
#include "Guide.h"


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

    bool eventFilter(QObject *obj, QEvent *event) override;

    void addWork(const QString &workName, const QString&link = "");

    void setName(const QString &name);

    void setGoalNumber(const QString &goalNumber);

    void setTime(const QString &time);

    void addWatch(const QString &watchName, const QString&link = "");

    void addRead(const QString &readName, const QString&link = "");

    void addProcess(const QString &processName, const QString&link = "");

    void addInfo(const QString &info, const QString&link = "");

    void addTask(const GuideData::GuideGoalPrefixes task);

    void setProgress(int progress, bool changedFile = true);

    void setProgress(const QString &progress , bool changedFile = true) {
        setProgress(progress.toInt(), changedFile);
    }

    void finalise();

    void setWeek(const QString &week);

    GuideData::GuideGoals getGoal();

    int size = 40;
    Guide* parentGuide; // for knowing which guide to save for auto save.



private slots:

    void on_progressSlider_sliderMoved(int newValue);

private:
    Ui::Goal *ui;
    QVector<GuideData::GuideGoalPrefixes> prefixes;

    void updateStyle();
};


#endif //STUDYGUIDE_GOAL_H
