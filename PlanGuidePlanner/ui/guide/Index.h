//
// Created by Jesse on 3 okt. 2023.
//

#ifndef STUDYGUIDE_INDEX_H
#define STUDYGUIDE_INDEX_H


#include <QWidget>

#include "guide/GuideData.h"

#include "Goal.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class Index;
}

QT_END_NAMESPACE

class Index : public QWidget {
    Q_OBJECT

public:
    explicit Index(QWidget* parent = nullptr);

    ~Index() override;

    void addGoal(Goal* goal);

    void finalise();

    int size = 100;

    GuideData::GuideObject getGuideObject();

private:
    Ui::Index* ui;

    QVector<Goal *> goals;

    void updateStyle();
};


#endif //STUDYGUIDE_INDEX_H
