//
// Created by Jesse on 30 aug. 2023.
//

#ifndef PLANGUIDEPLANNER_PREFERENCEWINDOW_H
#define PLANGUIDEPLANNER_PREFERENCEWINDOW_H

#include <QWidget>
#include <QSettings>
#include <QDialog>

QT_BEGIN_NAMESPACE

namespace Ui {
    class PreferenceWindow;
}

QT_END_NAMESPACE


class PreferenceWindow : public QDialog {
    Q_OBJECT

public:
    explicit PreferenceWindow(QWidget* parent = nullptr);

    ~PreferenceWindow() override;

private slots:
    void accept() override;

    void on_clearDirCheck_clicked(bool isChecked);

    void on_clearSettingsCheck_clicked(bool isChecked);

    void on_fullResetCheck_clicked(bool isChecked);

    void on_clearDirButton_pressed();

    void on_clearSettingsButton_pressed();

    void on_fullResetButton_pressed();

    void on_logsDirectoryPicker_pressed();

    void on_autoSaveDirPicker_pressed();
    void on_languageSelector_currentIndexChanged(int index);
void on_themes_currentIndexChanged(int index);
    void on_colourDropDown_currentIndexChanged(int index);

    void reject() override;
private:
    Ui::PreferenceWindow* ui;

    void loadSettings();

    void saveSettings();

    QString originalColour;

    bool isLoaded= false;
};


#endif //PLANGUIDEPLANNER_PREFERENCEWINDOW_H
