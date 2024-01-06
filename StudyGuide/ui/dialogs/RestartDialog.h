//
// Created by Jesse on 18-11-2023.
//

#ifndef RESTARTDIALOG_H
#define RESTARTDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class RestartDialog; }
QT_END_NAMESPACE

class RestartDialog : public QDialog {
Q_OBJECT

public:
    explicit RestartDialog(QWidget *parent = nullptr);
    ~RestartDialog() override;

private slots:
    void accept();

private:
    Ui::RestartDialog *ui;
};


#endif //RESTARTDIALOG_H
