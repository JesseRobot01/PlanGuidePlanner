//
// Created by Jesse on 3 okt. 2023.
//

#ifndef STUDYGUIDE_TEST_H
#define STUDYGUIDE_TEST_H


#include <QWidget>

#include "guide/GuideData.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class Test;
}

QT_END_NAMESPACE

class Test : public QWidget {
    Q_OBJECT

public:
    explicit Test(QWidget* parent = nullptr);

    ~Test() override;

    void setName(const QString&name);

    void setShortName(const QString&shortName);

    void setInfo(const QString&info);

    void setWeek(const QString&week);

    void finalise();

    GuideData::GuideObject getGuideobject();

    int size = 80;

private:
    Ui::Test* ui;
};


#endif //STUDYGUIDE_TEST_H
