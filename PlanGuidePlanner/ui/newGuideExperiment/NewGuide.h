//
// Created by Jesse on 04-10-2025.
//

#ifndef PLANGUIDEPLANNER_NEWGUIDE_H
#define PLANGUIDEPLANNER_NEWGUIDE_H
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

#include "guide/NewGuideData.h"

namespace Ui {
    class GuideBase;
}

class NewGuide : public QWidget {
Q_OBJECT

public:
    explicit NewGuide(QWidget* parent = nullptr, const NewGuideData::Data* data = nullptr);

    ~NewGuide() override;

    void updateStyle();

private:
    Ui::GuideBase* ui;
    QLayout* mainLayout;

    QSpacerItem* lastSpacer;

};


#endif //PLANGUIDEPLANNER_NEWGUIDE_H