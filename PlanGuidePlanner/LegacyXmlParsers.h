//
// Created by Jesse on 27-08-2025.
//

#ifndef PLANGUIDEPLANNER_LEGACYXMLPARSERS_H
#define PLANGUIDEPLANNER_LEGACYXMLPARSERS_H
#include "guide/OldGuideData.h"


class LegacyXmlParsers {
public:
    static OldGuideData::Data v1Reader(QFile* xmlFileP);
};


#endif //PLANGUIDEPLANNER_LEGACYXMLPARSERS_H