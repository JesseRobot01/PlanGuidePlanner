//
// Created by Jesse on 27-08-2025.
//

#ifndef PLANGUIDEPLANNER_LEGACYXMLPARSERS_H
#define PLANGUIDEPLANNER_LEGACYXMLPARSERS_H
#include "guide/NewGuideData.h"


class LegacyXmlParsers {
public:
    static NewGuideData::Data v1Reader(QFile* xmlFileP);
    static NewGuideData::Data v1PGPReader(QFile* xmlFileP);
};


#endif //PLANGUIDEPLANNER_LEGACYXMLPARSERS_H