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
            return {8, 73, 149};
            break;
        case HeaderText:
            return {255, 255, 255};
            break;
        case ObjectText:
            return {248, 122, 44};
            break;
        case WorkIndicatorBackground:
            return {147, 196, 125};
            break;
        case WorkIndicatorText:
            return {255, 255, 255};
            break;
        case WorkIndicatorExample:
            return {56, 118, 29};
            break;
        case WatchIndicatorBackground:
            return {111, 168, 220};
            break;
        case WatchIndicatorText:
            return {255, 255, 255};
            break;
        case WatchIndicatorExample:
            return {11, 83, 148};
            break;
        case ReadIndicatorBackground:
            return {255, 217, 102};
            break;
        case ReadIndicatorText:
            return {255, 255, 255};
            break;
        case ReadIndicatorExample:
            return {191, 144, 0};
            break;
        case ProcessIndicatorBackground:
            return {142, 124, 195};
            break;
        case ProcessIndicatorText:
            return {255, 255, 255};
            break;
        case ProcessIndicatorExample:
            return {53, 28, 117};
            break;
        case Progress_NotStarted:
            return {234, 67, 53};
            break;
        case Progress_Working:
            return {255, 153, 0};
            break;
        case Progress_Finished:
            return {52, 168, 83};
            break;
        case TestBackground:
            return {248, 122, 44};
            break;
        case InfoIndicatorBackground:
            return {182, 182, 182};
            break;
        case InfoIndicatorText:
            return {255,255,255};
            break;
        case InfoIndicatorExample:
            return  {124, 124, 124};
            break;
        default:
            return {0, 0, 0};
    }
}

QColor GuidePalette::getDarkModeColor(GuidePalette::GuideElements element) {
    switch (element) {

        case HeaderBackground:
            return {3, 56, 118};
            break;
        case HeaderText:
            return {255, 255, 255};
            break;
        case ObjectText:
            return {255, 109, 1};
            break;
        case WorkIndicatorBackground:
            return {56, 118, 29};
            break;
        case WorkIndicatorText:
            return {255, 255, 255};
            break;
        case WorkIndicatorExample:
            return {39, 78, 19};
            break;
        case WatchIndicatorBackground:
            return {11, 83, 148};
            break;
        case WatchIndicatorText:
            return {255, 255, 255};
            break;
        case WatchIndicatorExample:
            return {7, 55, 99};
            break;
        case ReadIndicatorBackground:
            return {191, 144, 0};
            break;
        case ReadIndicatorText:
            return {255, 255, 255};
            break;
        case ReadIndicatorExample:
            return {127, 96, 0};
            break;
        case ProcessIndicatorBackground:
            return {53, 28, 117};
            break;
        case ProcessIndicatorText:
            return {255, 255, 255};
            break;
        case ProcessIndicatorExample:
            return {32, 18, 77};
            break;
        case Progress_NotStarted:
            return {234, 67, 53};
            break;
        case Progress_Working:
            return {255, 153, 0};
            break;
        case Progress_Finished:
            return {52, 168, 83};
            break;
        case TestBackground:
            return {203, 86, 13};
            break;
        case InfoIndicatorBackground:
            return  {124, 124, 124};
            break;
        case InfoIndicatorText:
            return {255,255,255};
            break;
        case InfoIndicatorExample:
            return  {73, 73, 73};
            break;
        default:
            return {0, 0, 0};
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
    APPLICATION->setPalette(*this);
}