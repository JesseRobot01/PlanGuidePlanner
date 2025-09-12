//
// Created by Jesse on 22-1-2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_About.h" resolved

#include "AboutWindow.h"
#include "ui_About.h"
#include "Config.h"
#include "themes/GuidePalette.h"
#include <QSvgRenderer>
#include <QPainter>


AboutWindow::AboutWindow(QWidget* parent) : QDialog(parent), ui(new Ui::About) {
    ui->setupUi(this);

    QSvgRenderer renderer(QStringLiteral(":/logos/SG-LightMode.svg"));
    QPixmap pixmap(600, 200); // or use QSize dynamically
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    renderer.render(&painter);

    ui->label->setPixmap(pixmap.scaledToWidth(100));


    //Set Logo
    GuidePalette palette;
    if (palette.isLightMode())
        ui->label->setPixmap(QPixmap(":/logos/SG-LightMode.svg").scaledToWidth(320));
    else ui->label->setPixmap(QPixmap(":/logos/SG-DarkMode.svg").scaledToWidth(320));


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
