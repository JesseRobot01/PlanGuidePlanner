//
// Created by Jesse on 18-11-2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RestartDialog.h" resolved

#include "RestartDialog.h"
#include "ui_RestartDialog.h"
#include "../../Application.h"


RestartDialog::RestartDialog(QWidget* parent) : QDialog(parent), ui(new Ui::RestartDialog) {
    ui->setupUi(this);

    this->setFixedSize(350, 90);
}

RestartDialog::~RestartDialog() {
    delete ui;
}

void RestartDialog::accept() {
    Application::restart();
}
