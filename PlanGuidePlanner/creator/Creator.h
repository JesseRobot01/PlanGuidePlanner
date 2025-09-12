//
// Created by Jesse on 16-06-2025.
//

#ifndef CREATORMAINWINDOW_H
#define CREATORMAINWINDOW_H

#include <qlistwidget.h>
#include <QMainWindow>
#include <qtreewidget.h>

#include "guide/GuideData.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class Creator;
}

QT_END_NAMESPACE

class Creator : public QMainWindow {
    Q_OBJECT

public:
    explicit Creator(QWidget* parent = nullptr);

    ~Creator() override;


    void open(GuideData::Data guide);

    QFileInfo currentGuide;

    QFileInfo appAutoSaveLocation;

    int applicationGuideIndex = -1;

private slots:
    void on_mainDisplay_itemClicked(QTreeWidgetItem* item, int column);

    void on_shortEdit_editingFinished();

    void on_extraEdit_editingFinished();

    void on_longEdit_textChanged();

    void on_typeSelector_currentTextChanged(QString string);

    void on_displayButton_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_deleteButton_clicked();

    void on_progressSlider_sliderMoved(int newValue);

    void on_addList_itemDoubleClicked(QListWidgetItem* item);

    void on_actionOpen_Guide_triggered();

    void on_actionSave_Guide_triggered();

    void on_actionSave_Guide_As_triggered();

private:
    Ui::Creator* ui;

    //Manually adding the addlist items, for easyer configuration
    QListWidgetItem* addTask;
    QListWidgetItem* addReportTest;
    QListWidgetItem* listSpacer;
    QListWidgetItem* addGoal;
    QListWidgetItem* addTest;
    QListWidgetItem* addReport;

    GuideData::Data getCurrentGuide();

    void save(GuideData::Data guide);

    void hideAddTask();

    void showAddTask();

    void hideAddReportTest();

    void showAddReportTest();

    void hideTypeSelector();

    void showTypeSelector(QString text);

    void hideShortEdit();

    void showShortEdit(QString text, QString labelName);

    void hideExtraEdit();

    void showExtraEdit(QString text, QString labelName);

    void hideLongEdit();

    void showLongEdit(QString text, QString labelName);

    void hideProgressSlider();

    void showProgressSlider(int value);

    bool canBeManipulated(QTreeWidgetItem* item);

    bool isDefaultObject(QTreeWidgetItem* item);

    void setProgressSliderColour(int colour);
};


#endif //CREATORMAINWINDOW_H
