//
// Created by Jesse on 27-09-2025.
//

#ifndef PLANGUIDEPLANNER_GREENTHEME_H
#define PLANGUIDEPLANNER_GREENTHEME_H

#include <qcolor.h>

#include "GuidePalette.h"

QColor getLightGreenColour(GuidePalette::GuideElements element) {
    switch (element) {
        case GuidePalette::HeaderBackground:
            return "#008f4d";
        case GuidePalette::TestBackground:
            return "#63d92f";
        default:
            return getLightBlueColour(element);
    }
}

QColor getDarkGreenColour(GuidePalette::GuideElements element) {
    switch (element) {
        case GuidePalette::HeaderBackground:
            return {56, 118, 29};
        case GuidePalette::TestBackground:
            return "#6aa84f";
        default:
            return getDarkBlueColour(element);
    }
}

#endif //PLANGUIDEPLANNER_GREENTHEME_H
