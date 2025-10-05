//
// Created by Jesse on 04-10-2025.
//

#ifndef PLANGUIDEPLANNER_NEWGUIDEPROTOTYPE_H
#define PLANGUIDEPLANNER_NEWGUIDEPROTOTYPE_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class newGuidePrototype;
}

QT_END_NAMESPACE

class newGuidePrototype : public QWidget {
    Q_OBJECT

public:
    explicit newGuidePrototype(QWidget* parent = nullptr);

    ~newGuidePrototype() override;

private:
    Ui::newGuidePrototype* ui;
};


#endif //PLANGUIDEPLANNER_NEWGUIDEPROTOTYPE_H