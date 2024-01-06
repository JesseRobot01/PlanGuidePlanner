//
// Created by Jesse on 18-11-2023.
//

#ifndef LOADGUIDE_H
#define LOADGUIDE_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class LoadGuide;
}

QT_END_NAMESPACE

class LoadGuide : public QDialog {
    Q_OBJECT

public:
    explicit LoadGuide(QWidget* parent = nullptr, int maxProgress = 0);

    ~LoadGuide() override;

public slots:
    void increaseProgress(int amount = 1);

private:
    Ui::LoadGuide* ui;
};


#endif //LOADGUIDE_H
