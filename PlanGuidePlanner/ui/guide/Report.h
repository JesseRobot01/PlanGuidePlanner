//
// Created by Jesse on 3 okt. 2023.
//

#ifndef PLANGUIDEPLANNER_REPORT_H
#define PLANGUIDEPLANNER_REPORT_H


#include <QWidget>

#include "guide/OldGuideData.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class Report;
}

QT_END_NAMESPACE

class Report : public QWidget {
Q_OBJECT

public:
    explicit Report(QWidget *parent = nullptr);

    ~Report() override;

    void addTest(const QString &name, const QString &weight);

    void finalise();

    int size = 60;

    OldGuideData::GuideObject getGuideobject();

private:
    Ui::Report *ui;
    QVector<OldGuideData::ReportTests> tests;

    void updateStyle();
};


#endif //PLANGUIDEPLANNER_REPORT_H
