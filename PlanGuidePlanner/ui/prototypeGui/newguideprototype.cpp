//
// Created by Jesse on 04-10-2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_newGuidePrototype.h" resolved

#include "newguideprototype.h"
#include "ui_newGuidePrototype.h"


newGuidePrototype::newGuidePrototype(QWidget* parent) : QWidget(parent), ui(new Ui::newGuidePrototype) {
    ui->setupUi(this);
}

newGuidePrototype::~newGuidePrototype() {
    delete ui;
}