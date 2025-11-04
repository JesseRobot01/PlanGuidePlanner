//
// Created by Jesse on 08-10-2025.
//

#ifndef PLANGUIDEPLANNER_REPORT_H
#define PLANGUIDEPLANNER_REPORT_H
#include <QFrame>
#include <QLabel>
#include <qtmetamacros.h>
#include <QVBoxLayout>
#include <QWidget>

#include "guide/GuideData.h"


class Guide;

class Report : public QFrame {
    Q_OBJECT

public:
    Report(Guide* parent, const GuideData::Object* reportObject = nullptr);

    void processReport(GuideData::Object reportObject);

    void addTest(GuideData::ReportTest test);

    void updateStyle();

    void retranslateUi();
private:
    QVBoxLayout* mainLayout ;
    QLabel* mainLabel ;

};


#endif //PLANGUIDEPLANNER_REPORT_H
