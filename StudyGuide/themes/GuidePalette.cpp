//
// Created by Jesse on 21-1-2024.
//

#include "GuidePalette.h"
#include "Application.h"

QColor GuidePalette::getColor(GuideElements element) {
    if (window().color().value() > windowText().color().value()) {
        return getLightModeColor(element);
    } else {
        return getDarkModeColor(element);
    }
}

QColor GuidePalette::getLightModeColor(GuidePalette::GuideElements element) {
    switch (element) {
        case HeaderBackground:
            return QColor(8, 73, 149);
            break;
        case HeaderText:
            return QColor(255, 255, 255);
            break;
        case ObjectText:
            return QColor(248, 122, 44);
            break;
        case WorkIndicatorBackground:
            return QColor(147, 196, 125);
            break;
        case WorkIndicatorText:
            return QColor(255, 255, 255);
            break;
        case WorkIndicatorExample:
            return QColor(56, 118, 29);
            break;
        case WatchIndicatorBackground:
            return QColor(111, 168, 220);
            break;
        case WatchIndicatorText:
            return QColor(255, 255, 255);
            break;
        case WatchIndicatorExample:
            return QColor(11, 83, 148);
            break;
        case ReadIndicatorBackground:
            return QColor(255, 217, 102);
            break;
        case ReadIndicatorText:
            return QColor(255, 255, 255);
            break;
        case ReadIndicatorExample:
            return QColor(191, 144, 0);
            break;
        case ProcessIndicatorBackground:
            return QColor(142, 124, 195);
            break;
        case ProcessIndicatorText:
            return QColor(255, 255, 255);
            break;
        case ProcessIndicatorExample:
            return QColor(53, 28, 117);
            break;
        case Progress_NotStarted:
            return QColor(234, 67, 53);
            break;
        case Progress_Working:
            return QColor(255, 153, 0);
            break;
        case Progress_Finished:
            return QColor(52, 168, 83);
            break;
        case TestBackground:
            return QColor(248, 122, 44);
            break;
    }
}

QColor GuidePalette::getDarkModeColor(GuidePalette::GuideElements element) {
    switch (element) {

        case HeaderBackground:
            return QColor(3, 56, 118);
            break;
        case HeaderText:
            return QColor(255, 255, 255);
            break;
        case ObjectText:
            return QColor(255, 109, 1);
            break;
        case WorkIndicatorBackground:
            return QColor(56, 118, 29);
            break;
        case WorkIndicatorText:
            return QColor(255, 255, 255);
            break;
        case WorkIndicatorExample:
            return QColor(39, 78, 19);
            break;
        case WatchIndicatorBackground:
            return QColor(11, 83, 148);
            break;
        case WatchIndicatorText:
            return QColor(255, 255, 255);
            break;
        case WatchIndicatorExample:
            return QColor(7, 55, 99);
            break;
        case ReadIndicatorBackground:
            return QColor(191, 144, 0);
            break;
        case ReadIndicatorText:
            return QColor(255, 255, 255);
            break;
        case ReadIndicatorExample:
            return QColor(127, 96, 0);
            break;
        case ProcessIndicatorBackground:
            return QColor(53, 28, 117);
            break;
        case ProcessIndicatorText:
            return QColor(255, 255, 255);
            break;
        case ProcessIndicatorExample:
            return QColor(32, 18, 77);
            break;
        case Progress_NotStarted:
            return QColor(234, 67, 53);
            break;
        case Progress_Working:
            return QColor(255, 153, 0);
            break;
        case Progress_Finished:
            return QColor(52, 168, 83);
            break;
        case TestBackground:
            return QColor(203, 86, 13);
            break;
    }
}

void GuidePalette::setFusionDark() {
   APPLICATION->setStyle("fusion");
    setColor(QPalette::Window, QColor(53, 53, 53));
    setColor(QPalette::WindowText, Qt::white);
    setColor(QPalette::Base, QColor(25, 25, 25));
    setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    setColor(QPalette::ToolTipBase, Qt::white);
    setColor(QPalette::ToolTipText, Qt::white);
    setColor(QPalette::Text, Qt::white);
    setColor(QPalette::Button, QColor(53, 53, 53));
    setColor(QPalette::ButtonText, Qt::white);
    setColor(QPalette::BrightText, Qt::red);
    setColor(QPalette::Link, QColor(42, 130, 218));
    setColor(QPalette::Highlight, QColor(42, 130, 218));
    setColor(QPalette::HighlightedText, Qt::black);
    APPLICATION->setPalette(*this);
}

void GuidePalette::setFusionLight() {
    APPLICATION->setStyle("fusion");
setColor(QPalette::Window, Qt::white);
setColor(QPalette::WindowText, Qt::black);
setColor(QPalette::Base, Qt::white);
setColor(QPalette::AlternateBase, QColor(240, 240, 240));
setColor(QPalette::ToolTipBase, Qt::white);
setColor(QPalette::ToolTipText, Qt::black);
setColor(QPalette::Text, Qt::black);
setColor(QPalette::Button, QColor(240, 240, 240));
setColor(QPalette::ButtonText, Qt::black);
setColor(QPalette::BrightText, Qt::red);
setColor(QPalette::Link, QColor(42, 130, 218));
setColor(QPalette::Highlight, QColor(42, 130, 218));
setColor(QPalette::HighlightedText, Qt::white);
    APPLICATION-> setPalette(*this);
}