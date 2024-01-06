//
// Created by Jesse on 30 aug. 2023.
//

#ifndef STUDYGUIDE_PREFERENCEWINDOW_H
#define STUDYGUIDE_PREFERENCEWINDOW_H

#include <QWidget>
#include <QSettings>
#include "ui_PreferenceWindow.h"

class PreferenceWindow : public QDialog {
    Q_OBJECT

public:
    explicit PreferenceWindow(QWidget* parent = nullptr);

    ~PreferenceWindow() override;

private slots:
    void accept();

    void on_clearDirCheck_clicked(bool isChecked);

    void on_clearSettingsCheck_clicked(bool isChecked);

    void on_fullResetCheck_clicked(bool isChecked);

    void on_clearDirButton_pressed();

    void on_clearSettingsButton_pressed();

    void on_fullResetButton_pressed();

    void on_useAutoSaveCheck_clicked(bool isChecked);

    void on_useAutoOpenCheck_clicked(bool isChecked);

private:
    Ui::PreferenceWindow* ui;

    void loadSettings();

    void saveSettings();
};


#endif //STUDYGUIDE_PREFERENCEWINDOW_H
