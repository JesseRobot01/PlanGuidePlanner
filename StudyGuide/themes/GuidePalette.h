//
// Created by Jesse on 21-1-2024.
//

#ifndef STUDYGUIDE_GUIDEPALETTE_H
#define STUDYGUIDE_GUIDEPALETTE_H

#include <QPalette>

// this class is used to determine the colours for custom objects.
class GuidePalette : public QPalette {
public:
    enum GuideElements {
        HeaderBackground,
        HeaderText,
        ObjectText,
        WorkIndicatorBackground,
        WorkIndicatorText,
        WorkIndicatorExample,
        WatchIndicatorBackground,
        WatchIndicatorText,
        WatchIndicatorExample,
        ReadIndicatorBackground,
        ReadIndicatorText,
        ReadIndicatorExample,
        ProcessIndicatorBackground,
        ProcessIndicatorText,
        ProcessIndicatorExample,
        Progress_NotStarted,
        Progress_Working,
        Progress_Finished,
        TestBackground,
        InfoIndicatorBackground,
        InfoIndicatorText,
        InfoIndicatorExample,
    };

    QColor getColor(GuideElements element);

    bool isLightMode();

    void setFusionDark();

    void setFusionLight();

private:
    QColor getLightModeColor(GuideElements element);

    QColor getDarkModeColor(GuideElements element);
};


#endif //STUDYGUIDE_GUIDEPALETTE_H
