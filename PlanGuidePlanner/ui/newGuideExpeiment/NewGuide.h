//
// Created by Jesse on 04-10-2025.
//

#ifndef PLANGUIDEPLANNER_NEWGUIDE_H
#define PLANGUIDEPLANNER_NEWGUIDE_H
#include <QWidget>

#include "guide/GuideData.h"


class NewGuide : public QWidget {
Q_OBJECT

public:
    explicit NewGuide(QWidget* parent = nullptr, OldGuideData::Data* data = nullptr);

    ~NewGuide() override;

};


#endif //PLANGUIDEPLANNER_NEWGUIDE_H