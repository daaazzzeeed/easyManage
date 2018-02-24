#ifndef BRTRCONTROL_H
#define BRTRCONTROL_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QComboBox>

namespace Ui {
class brtrControl;
}

class brtrControl : public QMainWindow
{
    Q_OBJECT

public:
    explicit brtrControl(QWidget *parent = 0);
    ~brtrControl();

    QSqlDatabase db;
    QHash<int, QString>*  hash_ka;
    QString id  ;  //system id
    QString id_ka   ;
    QString id_sys  ;
    QString address ;
    QString name    ;
    QString id_s;
    QString id_ss;
    QString subaddress;
    QString count_data_word;
    QString name_s;
    QString id_subsys;
    QString id_subs;
    QString c_bit;
    QString n_bit;
    QString n_d_word;
    QString r_time;
    QString com_name;
    QString com_description;
    QString id_com;

private:
    Ui::brtrControl *ui;
    void update_ka();

private slots:
    void onSubmitKaClicked();
    void onSubmitSysClicked();
    void onSubmitSubSysClicked();
    void onSubmitComTmiClicked();

    void onDelKaClicked();
    void onDelSysClicked();
    void onDelSubSysClicked();
    void onDelComTmiClicked();

    void onItemActivated();
    void onSysItemActivated();

    void kaChanged(int index);
    void update_system();
    void update_subsystem();

    void add_and_remove_repeat(QComboBox *comBox, QString item, QString id_);

    void delete_records(QString tableName, QString row_name, QComboBox *comBox);
    void dispProps();
    void onSubSysItemActivated();
    void onComItemActivated();

    void update_com_tmi();

};

#endif // BRTRCONTROL_H
