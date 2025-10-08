//
// Created by Jesse on 08-10-2025.
//

#ifndef PLANGUIDEPLANNER_NEWTEST_H
#define PLANGUIDEPLANNER_NEWTEST_H

#include <QWidget>

#include "guide/NewGuideData.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class NewTest;
}

QT_END_NAMESPACE

class NewTest : public QWidget {
    Q_OBJECT

public:
    explicit NewTest(QWidget* parent = nullptr, const NewGuideData::Object* test = nullptr);

    ~NewTest() override;

    void updateStyle();

private:
    Ui::NewTest* ui;
};


#endif //PLANGUIDEPLANNER_NEWTEST_H