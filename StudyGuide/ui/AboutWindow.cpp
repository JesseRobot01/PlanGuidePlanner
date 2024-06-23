//
// Created by Jesse on 22-1-2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_About.h" resolved

#include "AboutWindow.h"
#include "ui_About.h"
#include "Config.h"


AboutWindow::AboutWindow(QWidget* parent) : QWidget(parent), ui(new Ui::About) {
    ui->setupUi(this);

    ui->version->setText(Config.version);

    if (!Config.buildDate.isEmpty()) {
        ui->date->setText(tr("Build Date: %1").arg(Config.buildDate));
    }
    else {
        ui->date->hide();
    }
    if (!Config.gitCommit.isEmpty()) {
        ui->commit->setText(tr("Git commit: %1").arg(Config.gitCommit));
    }
    else {
        ui->commit->hide();
    }
    if (!Config.gitTag.isEmpty()) {
        ui->tag->setText(tr("Git tag: %1").arg(Config.gitTag));
    }
    else {
        ui->tag->hide();
    }
    if (!Config.gitBranch.isEmpty()) {
        ui->branch->setText(tr("Git Branch: %1").arg(Config.gitBranch));
    }
    else {
        ui->branch->hide();
    }
    if (!Config.gitCommitNumber.isEmpty()) {
        ui->branchCommit->setText(tr("Git Branch commit: %1").arg(Config.gitCommitNumber));
    }
    else {
        ui->branchCommit->hide();
    }

    connect(ui->aboutQT, &QPushButton::clicked, QApplication::aboutQt);
}

AboutWindow::~AboutWindow() {
    delete ui;
}
