//
// Created by Jesse on 30 aug. 2023.
//

#ifndef PLANGUIDEPLANNER_MAINWINDOW_H
#define PLANGUIDEPLANNER_MAINWINDOW_H

#include <QMainWindow>
#include "PreferenceWindow.h"
#include "guide/OldGuideData.h"
#include<QCloseEvent>
#include "StartScreen.h"


class Guide;

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

    void processGuide(OldGuideData::Data guide, bool updateStart = true);

    QVector<Guide *> guides;

    void updateStart();

    void closeGuide(int guideIndex, bool updateStart = true);

    void setTabName(int tab, QString name);

private slots:
    void on_actionPreference_triggered();

    void on_actionOpen_File_triggered();

    void on_actionSave_Guide_As_triggered();

    void on_actionAbout_triggered();

    void on_guideSwitcher_tabCloseRequested(int tab);

    void on_guideSwitcher_currentChanged(int tab);

    void on_actionSave_All_Guides_triggered();

    void on_actionOpen_Creator_triggered();

    void on_actionOpen_In_Creator_triggered();

    void on_actionReport_an_issue_triggered();

    void on_actionClose_guide_triggered();

    void on_actionClose_all_guides_triggered();

private:
    Ui::MainWindow* ui;
    PreferenceWindow* preferenceWindow;
    StartScreen* startScreen;


    void addGuide(Guide* guide, const QString&name);

    void closeEvent(QCloseEvent* event) override;

    void saveGuideAs(OldGuideData::Data guide);
};


#endif //PLANGUIDEPLANNER_MAINWINDOW_H
