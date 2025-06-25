//
// Created by Jesse on 18-11-2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LoadGuide.h" resolved

#include "LoadGuide.h"
#include "ui_LoadGuide.h"


LoadGuide::LoadGuide(QWidget* parent, int maxProgress) : QDialog(parent), ui(new Ui::LoadGuide) {
    ui->setupUi(this);
    ui->progressBar->setRange(0, maxProgress);
    QCoreApplication::processEvents(); // show that there is a progress bar
}

LoadGuide::~LoadGuide() {
    delete ui;
}

void LoadGuide::increaseProgress(int amount) {
    ui->progressBar->setValue(ui->progressBar->value() + amount);
    QCoreApplication::processEvents(); // force update
}

void LoadGuide::setMaxProgress(int maxProgress) {
    ui->progressBar->setRange(0, maxProgress);
}


