//
// Created by Jesse on 27-09-2025.
//

#ifndef PLANGUIDEPLANNER_BLUETHEME_H
#define PLANGUIDEPLANNER_BLUETHEME_H
#include <qcolor.h>

#include "GuidePalette.h"

QColor getLightBlueColour(GuidePalette::GuideElements element) {
    switch (element) {
        case GuidePalette::HeaderBackground:
            return {8, 73, 149};
        case GuidePalette::HeaderText:
            return {255, 255, 255};
        case GuidePalette::ObjectText:
            return {248, 122, 44};
        case GuidePalette::WorkIndicatorBackground:
            return {147, 196, 125};
        case GuidePalette::WorkIndicatorText:
            return {255, 255, 255};
        case GuidePalette::WorkIndicatorExample:
            return {56, 118, 29};
        case GuidePalette::WatchIndicatorBackground:
            return {111, 168, 220};
        case GuidePalette::WatchIndicatorText:
            return {255, 255, 255};
        case GuidePalette::WatchIndicatorExample:
            return {11, 83, 148};
        case GuidePalette::ReadIndicatorBackground:
            return {255, 217, 102};
        case GuidePalette::ReadIndicatorText:
            return {255, 255, 255};
        case GuidePalette::ReadIndicatorExample:
            return {191, 144, 0};
        case GuidePalette::ProcessIndicatorBackground:
            return {142, 124, 195};
        case GuidePalette::ProcessIndicatorText:
            return {255, 255, 255};
        case GuidePalette::ProcessIndicatorExample:
            return {53, 28, 117};
        case GuidePalette::Progress_NotStarted:
            return {234, 67, 53};
        case GuidePalette::Progress_Working:
            return {255, 153, 0};
        case GuidePalette::Progress_Finished:
            return {52, 168, 83};
        case GuidePalette::TestBackground:
            return {248, 122, 44};
        case GuidePalette::InfoIndicatorBackground:
            return {182, 182, 182};
        case GuidePalette::InfoIndicatorText:
            return {255, 255, 255};
        case GuidePalette::InfoIndicatorExample:
            return {124, 124, 124};
        default:
            return {0, 0, 0};
    }
}


QColor getDarkBlueColour(GuidePalette::GuideElements element) {
    switch (element) {
        case GuidePalette::HeaderBackground:
            return {3, 56, 118};
        case GuidePalette::HeaderText:
            return {255, 255, 255};
        case GuidePalette::ObjectText:
            return {255, 109, 1};
        case GuidePalette::WorkIndicatorBackground:
            return {56, 118, 29};
        case GuidePalette::WorkIndicatorText:
            return {255, 255, 255};
        case GuidePalette::WorkIndicatorExample:
            return {39, 78, 19};
        case GuidePalette::WatchIndicatorBackground:
            return {11, 83, 148};
        case GuidePalette::WatchIndicatorText:
            return {255, 255, 255};
        case GuidePalette::WatchIndicatorExample:
            return {7, 55, 99};
        case GuidePalette::ReadIndicatorBackground:
            return {191, 144, 0};
        case GuidePalette::ReadIndicatorText:
            return {255, 255, 255};
        case GuidePalette::ReadIndicatorExample:
            return {127, 96, 0};
        case GuidePalette::ProcessIndicatorBackground:
            return {53, 28, 117};
        case GuidePalette::ProcessIndicatorText:
            return {255, 255, 255};
        case GuidePalette::ProcessIndicatorExample:
            return {32, 18, 77};
        case GuidePalette::Progress_NotStarted:
            return {234, 67, 53};
        case GuidePalette::Progress_Working:
            return {255, 153, 0};
        case GuidePalette::Progress_Finished:
            return {52, 168, 83};
        case GuidePalette::TestBackground:
            return {203, 86, 13};
        case GuidePalette::InfoIndicatorBackground:
            return {124, 124, 124};
        case GuidePalette::InfoIndicatorText:
            return {255, 255, 255};
        case GuidePalette::InfoIndicatorExample:
            return {73, 73, 73};
        default:
            return {0, 0, 0};
    }
}

#endif //PLANGUIDEPLANNER_BLUETHEME_H
