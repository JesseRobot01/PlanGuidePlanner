//
// Created by Jesse on 3 okt. 2023.
//

#ifndef studyGUIDE_GUIDE_H
#define STUDYGUIDE_GUIDE_H

#include "guide/GuideData.h"

#include "Index.h"
#include "Test.h"
#include "Report.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class Guide;
}

QT_END_NAMESPACE

class Guide : public QWidget {
Q_OBJECT

public:
    explicit Guide(QWidget *parent = nullptr);

    ~Guide() override;

    void addIndex(Index *index);

    void addTest(Test *test);

    void addReport(Report *Report);

    void setName(const QString &name);

    void setInfo(const QString &info);

    void setPeriod(const QString &period);

    void setShortName(const QString &shortName);

    GuideData::Data getGuide();

    QString name;

private:
    Ui::Guide *ui;

    QString shortName;

    int size = 310;

    QVector<Index *> indexes;
    QVector<Test *> tests;
    QVector<Report *> reports;
    QVector<GuideData::ObjectTypes> objectOrder;

    void updateStyle();
};

#endif //STUDYGUIDE_GUIDE_H
