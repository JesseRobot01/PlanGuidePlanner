//
// Created by Jesse on 30 aug. 2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PreferenceWindow.h" resolved

#include "PreferenceWindow.h"
#include "ui_PreferenceWindow.h"
#include "../Application.h"
#include <QComboBox>
#include <qdir.h>
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <qstandardpaths.h>

#include "dialogs/RestartDialog.h"


PreferenceWindow::PreferenceWindow(QWidget* parent)
    : QDialog(parent), ui(new Ui::PreferenceWindow) {
    ui->setupUi(this);

    ui->languageSelector->addItem(tr("English"), "en");
    ui->languageSelector->addItem(tr("Dutch"), "nl");

    ui->themes->addItem(tr("Fusion Dark"), "fusion_dark");
    ui->themes->addItem(tr("Fusion Light"), "fusion_light");


    ui->themes->addItem(tr("System"), "system");

    ui->colourDropDown->addItem(tr("Blue"), "blue");
    ui->colourDropDown->addItem(tr("Green"), "green");
    ui->colourDropDown->addItem(tr("Red"), "red");
    ui->colourDropDown->addItem(tr("Orange"), "orange");
    ui->colourDropDown->addItem(tr("High Contrast"), "contrast");



    loadSettings();
}

PreferenceWindow::~PreferenceWindow() {
    delete ui;
}

void PreferenceWindow::loadSettings() {
    QSettings settings;

    // Qt, why did you made this part so wierd, just add a SetCurrentData() function!
    ui->languageSelector->setCurrentIndex(ui->languageSelector->findData(settings.value("Lang", "en")));
    ui->themes->setCurrentIndex(ui->themes->findData(settings.value("Theme", "fusion_dark")));
    ui->colourDropDown->setCurrentIndex(ui->colourDropDown->findData(settings.value("Colour", "blue")));

    ui->autoSaveDir->setText(APPLICATION->getAutoSaveLocation());


    ui->logsDirectory->setText(APPLICATION->getLogsDirLocation());
}


void PreferenceWindow::saveSettings() {
    QSettings settings;
    settings.setValue("Lang", ui->languageSelector->currentData());
    settings.setValue("Theme", ui->themes->currentData());
    settings.setValue("Colour", ui->colourDropDown->currentData());

    settings.setValue("AutoSaveDir", ui->autoSaveDir->text());
    settings.setValue("LogsDir", ui->logsDirectory->text());
}

void PreferenceWindow::accept() {
    saveSettings();
    delete this;
    RestartDialog* restartDialog = new RestartDialog();
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
    QDir logsDir(settings.value("LogsDir",
                                (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                 "/logs")).toString());

    if (autoSaveDir.exists()) {
        if (autoSaveDir.removeRecursively())
            qDebug() << "Successfully cleared the auto save dir.";
        else
            qCritical() << "Failed to clear the auto save dir.";
    }
    if (logsDir.exists()) {
        if (logsDir.removeRecursively())
            qDebug() << "Successfully cleared the logs dir";
        else
            qCritical() << "Failed to clear the logs dir.";
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

void PreferenceWindow::on_logsDirectoryPicker_pressed() {
    auto* logsDir = ui->logsDirectory;
    QString newDir;

    newDir = QFileDialog::getExistingDirectory(this, tr("log directory"), logsDir->text());

    if (!newDir.isEmpty())
        logsDir->setText(newDir);
}

void PreferenceWindow::on_autoSaveDirPicker_pressed() {
    auto* autoSaveDir = ui->autoSaveDir;
    QString newDir;

    newDir = QFileDialog::getExistingDirectory(this, tr("Auto Save directory"), autoSaveDir->text());

    if (!newDir.isEmpty())
        autoSaveDir->setText(newDir);
}
