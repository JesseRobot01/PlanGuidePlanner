//
// Created by Jesse on 04-10-2025.
//

#ifndef PLANGUIDEPLANNER_NEWGUIDE_H
#define PLANGUIDEPLANNER_NEWGUIDE_H
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

#include "guide/NewGuideData.h"
#include "NewReport.h"
#include "NewTest.h"
#include "NewGoal.h"

namespace Ui {
    class GuideBase;
}

class NewGuide : public QWidget {
    Q_OBJECT

public:
    explicit NewGuide(QWidget* parent = nullptr, const NewGuideData::Data* data = nullptr);

    ~NewGuide() override;

    void updateStyle();

    void retranslateUi();

    void processGuide(NewGuideData::Data data);

    NewGuideData::Data getGuide();

private:
    Ui::GuideBase* ui;
    QLayout* mainLayout;

    QSpacerItem* lastSpacer;

    QVector<GoalFrame*> goalFrames;
    QVector<NewTest*> testWidgets;
    QVector<NewReport*> reportWidgets;

    QVector<NewGuideData::ObjectTypes> objectOrder;
    QVector<NewGuideData::Object> nonGoalObjects;

    NewGuideData::Data baseData;

};


#endif //PLANGUIDEPLANNER_NEWGUIDE_H
