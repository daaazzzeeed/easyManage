#ifndef BRTRCONTROL_H
#define BRTRCONTROL_H

#include <QMainWindow>

namespace Ui {
class brtrControl;
}

class brtrControl : public QMainWindow
{
    Q_OBJECT

public:
    explicit brtrControl(QWidget *parent = 0);
    ~brtrControl();

private:
    Ui::brtrControl *ui;
};

#endif // BRTRCONTROL_H
