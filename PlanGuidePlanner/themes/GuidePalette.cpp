//
// Created by Jesse on 21-1-2024.
//

#include "GuidePalette.h"
#include "Application.h"
#include "BlueTheme.h"
#include "ContrastTheme.h"
#include "GreenTheme.h"
#include "OrangeTheme.h"
#include "RedTheme.h"

QColor GuidePalette::getColor(GuideElements element) {
    if (isLightMode()) {
        return getLightModeColor(element);
    }
    else {
        return getDarkModeColor(element);
    }
}

bool GuidePalette::isLightMode() {
    return window().color().value() > windowText().color().value();
}

QColor GuidePalette::getLightModeColor(GuidePalette::GuideElements element) {
    QSettings settings;
    QString currentColour = settings.value("Colour", "blue").toString();

    if (currentColour == "blue")
        return getLightBlueColour(element);
    if (currentColour == "green")
        return getLightGreenColour(element);
    if (currentColour == "red")
        return getLightRedColour(element);
    if (currentColour == "orange")
        return getLightOrangeColour(element);
if (currentColour == "contrast")
    return getContrastColour(element);

    return getLightBlueColour(element);
}

QColor GuidePalette::getDarkModeColor(GuidePalette::GuideElements element) {
    QSettings settings;
    QString currentColour = settings.value("Colour", "blue").toString();

    if (currentColour == "blue")
        return getDarkBlueColour(element);
    if (currentColour == "green")
        return getDarkGreenColour(element);
    if (currentColour == "red")
        return getDarkRedColour(element);
    if (currentColour == "orange")
        return getDarkOrangeColour(element);
    if (currentColour == "contrast")
        return getContrastColour(element);

    return getDarkBlueColour(element);
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
