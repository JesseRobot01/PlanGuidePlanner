//
// Created by Jesse on 08-10-2025.
//

#ifndef PLANGUIDEPLANNER_NEWREPORT_H
#define PLANGUIDEPLANNER_NEWREPORT_H
#include <QFrame>
#include <QLabel>
#include <qtmetamacros.h>
#include <QVBoxLayout>
#include <QWidget>

#include "guide/NewGuideData.h"


class NewReport : public QFrame {
    Q_OBJECT

public:
    NewReport(QWidget* parent = nullptr, const NewGuideData::Object* reportObject = nullptr);

    void addTest(NewGuideData::ReportTest test);

    void updateStyle();

    void retranslateUi();
private:
    QVBoxLayout* mainLayout ;
    QLabel* mainLabel ;

};


#endif //PLANGUIDEPLANNER_NEWREPORT_H
