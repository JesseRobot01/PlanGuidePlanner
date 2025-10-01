//
// Created by Jesse on 30-09-2025.
//

#ifndef PLANGUIDEPLANNER_CONTRASTTHEME_H
#define PLANGUIDEPLANNER_CONTRASTTHEME_H

#include <qcolor.h>

#include "GuidePalette.h"

QColor getContrastColour(GuidePalette::GuideElements element) {
    GuidePalette palette;
    switch (element) {
        case GuidePalette::HeaderBackground:
            return palette.color(QPalette::Window);

        case GuidePalette::HeaderText:
            return palette.color(QPalette::Text);

        case GuidePalette::ObjectText:
            return palette.color(QPalette::Text);

        case GuidePalette::WorkIndicatorBackground:
            return palette.color(QPalette::Window);

        case GuidePalette::WorkIndicatorText:
            return palette.color(QPalette::Text);

        case GuidePalette::WorkIndicatorExample:
            return palette.color(QPalette::Window);

        case GuidePalette::WatchIndicatorBackground:
            return palette.color(QPalette::Window);

        case GuidePalette::WatchIndicatorText:
            return palette.color(QPalette::Text);

        case GuidePalette::WatchIndicatorExample:
            return palette.color(QPalette::Window);

        case GuidePalette::ReadIndicatorBackground:
            return palette.color(QPalette::Window);

        case GuidePalette::ReadIndicatorText:
            return palette.color(QPalette::Text);

        case GuidePalette::ReadIndicatorExample:
            return palette.color(QPalette::Window);

        case GuidePalette::ProcessIndicatorBackground:
            return palette.color(QPalette::Window);

        case GuidePalette::ProcessIndicatorText:
            return palette.color(QPalette::Text);

        case GuidePalette::ProcessIndicatorExample:
            return palette.color(QPalette::Window);

        case GuidePalette::Progress_NotStarted:
            return {234, 67, 53};

        case GuidePalette::Progress_Working:
            return {255, 153, 0};

        case GuidePalette::Progress_Finished:
            return {52, 168, 83};

        case GuidePalette::TestBackground:
            return palette.color(QPalette::Window);

        case GuidePalette::InfoIndicatorBackground:
            return palette.color(QPalette::Window);

        case GuidePalette::InfoIndicatorText:
            return palette.color(QPalette::Text);

        case GuidePalette::InfoIndicatorExample:
            return palette.color(QPalette::Window);

        default:
            return getDarkBlueColour(element);
    }
}
#endif //PLANGUIDEPLANNER_CONTRASTTHEME_H
