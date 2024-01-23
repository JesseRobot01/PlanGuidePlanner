//
// Created by Jesse on 22-1-2024.
//

#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class About;
}

QT_END_NAMESPACE

class AboutWindow : public QWidget {
    Q_OBJECT

public:
    explicit AboutWindow(QWidget* parent = nullptr);

    ~AboutWindow() override;

private:
    Ui::About* ui;
};


#endif //ABOUT_H
