//
// Created by Jesse on 08-10-2025.
//

#ifndef PLANGUIDEPLANNER_TEST_H
#define PLANGUIDEPLANNER_TEST_H

#include <QWidget>

#include "guide/GuideData.h"


class Guide;
QT_BEGIN_NAMESPACE

namespace Ui {
    class Test;
}

QT_END_NAMESPACE

class Test : public QWidget {
    Q_OBJECT

public:
    explicit Test(Guide* parent, const GuideData::Object* test = nullptr);

    void setTest(GuideData::Object testObject);

    ~Test() override;

    void updateStyle();

private:
    Ui::Test* ui;
};


#endif //PLANGUIDEPLANNER_TEST_H
