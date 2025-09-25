//
// Created by Jesse on 04-11-2024.
//

#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class StartScreen;
}

QT_END_NAMESPACE

class StartScreen : public QWidget {
    Q_OBJECT

public:
    explicit StartScreen(QWidget* parent = nullptr);

    ~StartScreen() override;

    void updateStart();

private:
    Ui::StartScreen* ui;

    void addWeekLabel();
};


#endif //STARTSCREEN_H
