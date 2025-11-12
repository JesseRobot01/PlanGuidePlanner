//
// Created by Jesse on 02-11-2025.
//

#ifndef PLANGUIDEPLANNER_YELLOWTHEME_H
#define PLANGUIDEPLANNER_YELLOWTHEME_H

#include <qcolor.h>

#include "GuidePalette.h"

QColor getLightYellowColour(GuidePalette::GuideElements element) {
    switch (element) {
        case GuidePalette::HeaderBackground:
            return "#FFFF00";
        case GuidePalette::HeaderText:
            return "#000000";
        case GuidePalette::TestBackground:
            return "#FFFF75";
        default:
            return getLightBlueColour(element);
    }
}

QColor getDarkYellowColour(GuidePalette::GuideElements element) {
    switch (element) {
        case GuidePalette::HeaderBackground:
            return "#DBA800";
        case GuidePalette::TestBackground:
            return "#F6BE00";
        default:
            return getDarkBlueColour(element);
    }
}

#endif //PLANGUIDEPLANNER_YELLOWTHEME_H
