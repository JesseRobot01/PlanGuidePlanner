//
// Created by Jesse on 08-10-2025.
//

#include "NewReport.h"

#include <QLabel>
#include <QVBoxLayout>

#include "guide/NewGuideData.h"
#include "guide/NewGuideData.h"
#include "themes/GuidePalette.h"

NewReport::NewReport(QWidget* parent, const NewGuideData::Object* reportObject) : QFrame(parent) {
    mainLabel = new QLabel(this);
    mainLayout = new QVBoxLayout(this);

    QFont font;
    font.setPointSize(23);
    font.setBold(true);


    setFrameShape(WinPanel);
    setFrameShadow(Raised);

    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLabel->setFont(font);
    mainLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(mainLabel);


    for (auto test: reportObject->tests) {
        addTest(test);
    }

    updateStyle();
    retranslateUi();
}

void NewReport::addTest(NewGuideData::ReportTest test) {
    QWidget* testWidget = new QWidget(this);
    QHBoxLayout* testLayout = new QHBoxLayout(testWidget);

    testLayout->setSpacing(0);
    testLayout->setContentsMargins(0, 0, 0, 0);


    QLabel* name = new QLabel(testWidget);
    QLabel* weight = new QLabel(testWidget);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    QFont nameFont;
    nameFont.setPointSize(12);

    QFont labelFont;
    labelFont.setPointSize(15);
    labelFont.setBold(true);

    sizePolicy.setHeightForWidth(name->sizePolicy().hasHeightForWidth());
    name->setSizePolicy(sizePolicy);
    name->setMinimumSize(0, 40);
    name->setMaximumSize(16777215, 40);
    name->setFont(nameFont);
    name->setLineWidth(3);
    name->setFrameShape(NoFrame);
    name->setAlignment(Qt::AlignCenter);
    name->setTextFormat(Qt::MarkdownText);


    sizePolicy.setHeightForWidth(weight->sizePolicy().hasHeightForWidth());
    weight->setSizePolicy(sizePolicy);
    weight->setMinimumSize(0, 40);
    weight->setMaximumSize(125, 40);
    weight->setFont(labelFont);
    weight->setLineWidth(3);
    weight->setAlignment(Qt::AlignCenter);
    weight->setFrameShape(QFrame::NoFrame);

    testLayout->addWidget(name);
    testLayout->addWidget(weight);

    mainLayout->addWidget(testWidget);

    name->setText(test.name);
    weight->setText(QString::number(test.weight) + test.weightType);
}

void NewReport::updateStyle() {
    GuidePalette palette;
    setStyleSheet(QString::fromUtf8("background-color: %1;").arg(palette.color(QPalette::Base).name()));
    mainLabel->setStyleSheet(QString::fromUtf8("color: %1;").arg(palette.getColor(GuidePalette::ObjectText).name()));
}

void NewReport::retranslateUi() {
    mainLabel->setText(tr("Report"));
}
