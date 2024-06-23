//
// Created by Jesse on 23-1-2024.
//

#ifndef BUILDCONFIG_H
#define BUILDCONFIG_H
#include <QString>


class BuildConfig {
public:
    BuildConfig();

    QString version;
    QString buildDate;
    QString gitCommit;
    QString gitTag;
    QString gitBranch;
    QString gitCommitNumber;
};
extern const BuildConfig Config;

#endif //BUILDCONFIG_H
