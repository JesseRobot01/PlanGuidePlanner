//
// Created by Jesse on 27-08-2025.
//

#ifndef PLANGUIDEPLANNER_LEGACYXMLPARSERS_H
#define PLANGUIDEPLANNER_LEGACYXMLPARSERS_H
#include "guide/GuideData.h"


class LegacyXmlParsers {
public:
    static GuideData::Data v1Reader(QFile* xmlFileP);
};


#endif //PLANGUIDEPLANNER_LEGACYXMLPARSERS_H