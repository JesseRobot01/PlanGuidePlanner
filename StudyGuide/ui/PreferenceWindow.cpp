//
// Created by Jesse on 30 aug. 2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PreferenceWindow.h" resolved

#include "PreferenceWindow.h"
#include "ui_PreferenceWindow.h"
#include "../Application.h"
#include <QComboBox>
#include <qdir.h>
#include <QPushButton>
#include <QVBoxLayout>
#include <qstandardpaths.h>

#include "dialogs/RestartDialog.h"


PreferenceWindow::PreferenceWindow(QWidget *parent)
        : QDialog(parent), ui(new Ui::PreferenceWindow) {
    ui->setupUi(this);

    ui->languageSelector->addItem(tr("English"), "en");
    ui->languageSelector->addItem(tr("Dutch"), "nl");

    loadSettings();
}

PreferenceWindow::~PreferenceWindow() {
    delete ui;
}

void PreferenceWindow::loadSettings() {
    QSettings settings;

    // Qt, why did you made this part so wierd, just add a SetCurrentData() function!
    ui->languageSelector->setCurrentIndex(ui->languageSelector->findData(settings.value("Lang", "en")));

    ui->autoSaveDir->setText(settings.value("AutoSaveDir",
                                            (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                             "/open guides")).
            toString());
    ui->autoOpenDir->setText(settings.value("AutoOpenDir",
                                            (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                             "/open guides")).
            toString());


    ui->copyNewFilesCheckBox->setChecked(settings.value("AutoCopyGuide", "1").toBool());
    if (!settings.value("AutoSave", "1").toBool()) {
        ui->useAutoSaveCheck->setChecked(false);
        ui->autoSaveDir->setEnabled(false);
    }
    if (!settings.value("AutoOpen", "1").toBool()) {
        ui->useAutoOpenCheck->setChecked(false);
        ui->autoOpenDir->setEnabled(false);
        ui->copyNewFilesCheckBox->setEnabled(false);
    }
    ui->logsDirectory->setText(settings.value("LogsDir",
                                              (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                               "/logs")).toString());
}


void PreferenceWindow::saveSettings() {
    QSettings settings;
    settings.setValue("Lang", ui->languageSelector->currentData());

    settings.setValue("AutoSave", ui->useAutoSaveCheck->isChecked());
    settings.setValue("AutoOpen", ui->useAutoOpenCheck->isChecked());
    settings.setValue("AutoSaveDir", ui->autoSaveDir->text());
    settings.setValue("AutoOpenDir", ui->autoOpenDir->text());
    settings.setValue("LogsDir", ui->logsDirectory->text());

    settings.setValue("AutoCopyGuide", ui->copyNewFilesCheckBox->isChecked());
}

void PreferenceWindow::accept() {
    saveSettings();
    delete this;
    RestartDialog *restartDialog = new RestartDialog();
    restartDialog->show();
}

void PreferenceWindow::on_clearDirCheck_clicked(bool isChecked) {
    ui->clearDirButton->setEnabled(isChecked);
}

void PreferenceWindow::on_clearSettingsCheck_clicked(bool isChecked) {
    ui->clearSettingsButton->setEnabled(isChecked);
}


void PreferenceWindow::on_fullResetCheck_clicked(bool isChecked) {
    ui->fullResetButton->setEnabled(isChecked);
}

void PreferenceWindow::on_clearDirButton_pressed() {
    QSettings settings;
    QDir autoSaveDir(settings.value("AutoSaveDir",
                                    (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                     "/open guides")).toString());
    QDir autoOpenDir(settings.value("AutoOpenDir",
                                    (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                     "/open guides")).toString());
    QDir logsDir(settings.value("LogsDir",
                                (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                 "/logs")).toString());

    if (autoSaveDir.exists()) {
        if (autoSaveDir.removeRecursively())
            qDebug() << "Successfully cleared the auto save dir.";
        else
            qWarning() << "Failed to clear the auto save dir.";
    }
    if (autoSaveDir.exists()) {
        if (autoOpenDir.removeRecursively())
            qDebug() << "Successfully cleared the auto open dir.";
        else
            qWarning() << "Failed to clear the auto open dir.";
    }
    if (logsDir.exists()) {
        if (logsDir.removeRecursively())
            qDebug() << "Successfully cleared the logs dir";
        else
            qWarning() << "Failed to clear the logs dir.";
    }
}

void PreferenceWindow::on_clearSettingsButton_pressed() {
    QSettings settings;
    settings.clear();
    settings.sync();
    accept();
}


void PreferenceWindow::on_fullResetButton_pressed() {
    // this does the same as clear dirs and clear settings, so just call them.
    on_clearDirButton_pressed();
    on_clearSettingsButton_pressed();
}

void PreferenceWindow::on_useAutoSaveCheck_clicked(bool isChecked) {
    ui->autoSaveDir->setEnabled(isChecked);
}

void PreferenceWindow::on_useAutoOpenCheck_clicked(bool isChecked) {
    ui->autoOpenDir->setEnabled(isChecked);
    ui->copyNewFilesCheckBox->setEnabled(isChecked);
}
