#include "brtrcontrol.h"
#include "ui_brtrcontrol.h"

brtrControl::brtrControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::brtrControl)
{
    ui->setupUi(this);
}

brtrControl::~brtrControl()
{
    delete ui;
}
