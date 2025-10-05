//
// Created by Jesse on 04-10-2025.
//

#include "NewGuide.h"

#include <QVBoxLayout>

#include "guide/GuideData.h"

NewGuide::NewGuide(QWidget* parent, OldGuideData::Data* data) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setObjectName("verticalLayout_5");
    mainLayout->setContentsMargins(30, 10, 30, 10);


}
