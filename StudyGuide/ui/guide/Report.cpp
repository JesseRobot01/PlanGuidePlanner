//
// Created by Jesse on 3 okt. 2023.
//

#include "Report.h"
#include "ui_Report.h"

Report::Report(QWidget* parent) : QWidget(parent), ui(new Ui::Report) {
    ui->setupUi(this);
}

Report::~Report() {
    delete ui;
}

void Report::addTest(const QString&name, const QString&weight) {
    GuideData::ReportTests test;
    test.name = name;
    test.weight = weight;
    tests.append(test);

    QLabel* testName = new QLabel(ui->frame);
    QLabel* testWeight = new QLabel(ui->frame);


    // first the test name
    testName->setGeometry(QRect(0, size, 1100, 40));
    QFont testNameFont;
    testNameFont.setPointSize(12);
    testNameFont.setBold(false);
    testName->setFont(testNameFont);
    testName->setStyleSheet(QString::fromUtf8("color : black;"));
    testName->setFrameShape(QFrame::NoFrame);
    testName->setLineWidth(3);
    testName->setAlignment(Qt::AlignCenter);
    testName->setText(name);

    // and now the weight
    testWeight->setGeometry(QRect(1110, size, 125, 40));
    QFont testWeightFont;
    testWeightFont.setPointSize(15);
    testWeightFont.setBold(true);
    testWeight->setFont(testWeightFont);
    testWeight->setStyleSheet(QString::fromUtf8("color : black;"));
    testWeight->setFrameShape(QFrame::NoFrame);
    testWeight->setLineWidth(3);
    testWeight->setAlignment(Qt::AlignCenter);
    testWeight->setText(weight + "x");

    size += 40;
}

void Report::finalise() {
    ui->frame->resize(1240, size + 5);
}

GuideData::GuideObject Report::getGuideobject() {
    GuideData::GuideObject object;
    object.objectType = GuideData::Report;
    object.tests = tests;

    return object;
}