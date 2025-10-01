//
// Created by Jesse on 27-09-2025.
//

#ifndef PLANGUIDEPLANNER_REDTHEME_H
#define PLANGUIDEPLANNER_REDTHEME_H

#include <qcolor.h>

#include "GuidePalette.h"

QColor getLightRedColour(GuidePalette::GuideElements element) {
    switch (element) {
        case GuidePalette::HeaderBackground:
            return "#FFFF00";
        case GuidePalette::TestBackground:
            return "#CC0000";
        default:
            return getLightBlueColour(element);
    }
}

QColor getDarkRedColour(GuidePalette::GuideElements element) {
    switch (element) {
        case GuidePalette::HeaderBackground:
            return "#ea4335";
        case GuidePalette::TestBackground:
            return "#FF0000";
        default:
            return getDarkBlueColour(element);
    }
}

#endif //PLANGUIDEPLANNER_GREENTHEME_H
