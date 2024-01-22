//
// Created by Jesse on 3 okt. 2023.
//

#ifndef STUDYGUIDE_REPORT_H
#define STUDYGUIDE_REPORT_H


#include <QWidget>

#include "guide/GuideData.h"


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

    GuideData::GuideObject getGuideobject();

private:
    Ui::Report *ui;
    QVector<GuideData::ReportTests> tests;

    void updateStyle();
};


#endif //STUDYGUIDE_REPORT_H
