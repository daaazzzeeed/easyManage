#ifndef BRTRCONTROL_H
#define BRTRCONTROL_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QComboBox>
#include <QMenu>
#include <QMenuBar>
#include <QSqlRelationalTableModel>

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
    QString id, id_ka, id_sys, address, name, id_s, id_ss,
    subaddress, count_data_word, name_s, id_subsys, id_subs,
    c_bit, n_bit, n_d_word, r_time, com_name, com_description,
    id_com, id_tmi, n_par, tmi_name, tmi_description;

    QSqlRelationalTableModel *model;
    QSqlRelationalTableModel *kamodel;
    QSqlRelationalTableModel *sysmodel;
    QSqlRelationalTableModel *subsysmodel;
    QSqlRelationalTableModel *telemetryModel;

    int row, column;

private:
    Ui::brtrControl *ui;
    void update_ka();
    QString loadDatabaseFile();



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

    void delete_records(QString tableName, QString row_name, QComboBox *comBox);
    void dispProps();
    void onSubSysItemActivated();
    void onComItemActivated();

    void update_com_tmi();

    void update_labels();

    void updateKaFilter();
    void updateSysFilter();
    void updateSubSysFilter();

    void onTableClicked(const QModelIndex &index);

    void delete_record();
    void add_record();

    void update_records();
    void onCheckBoxClicked();
};

#endif // BRTRCONTROL_H
