//
// Created by Jesse on 27-09-2025.
//

#ifndef PLANGUIDEPLANNER_ORANGETHEME_H
#define PLANGUIDEPLANNER_ORANGETHEME_H

#include <qcolor.h>

#include "GuidePalette.h"

QColor getLightOrangeColour(GuidePalette::GuideElements element) {
    switch (element) {
        case GuidePalette::HeaderBackground:
            return "#ff8c00";
            break;
              case GuidePalette::TestBackground:
               return "#ff8201";
            break;
        default:
            return getLightBlueColour(element);
    }
}

QColor getDarkOrangeColour(GuidePalette::GuideElements element) {
    switch (element) {
        case GuidePalette::HeaderBackground:
            return "#ff8201";
        case GuidePalette::TestBackground:
            return "#ff8c00";
        default:
            return getDarkBlueColour(element);
    }
}

#endif //PLANGUIDEPLANNER_GREENTHEME_H
