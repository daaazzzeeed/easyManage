#include "brtrcontrol.h"
#include "ui_brtrcontrol.h"
#include <QDebug>
#include <QFile>
#include <QSqlError>
#include <QHash>

brtrControl::brtrControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::brtrControl)
{
    ui->setupUi(this);

    ui->sysComBox->setDisabled(true);
    ui->addrInp->setDisabled(true);
    ui->submitSysPb->setDisabled(true);
    ui->delSysPb->setDisabled(true);
    ui->subSysComBox->setDisabled(true);
    ui->subaddrInp->setDisabled(true);
    ui->countDataWordInp->setDisabled(true);
    ui->nameInp->setDisabled(true);
    ui->namInp->setDisabled(true);
    ui->submitSubSysPb->setDisabled(true);
    ui->delSubSysPb->setDisabled(true);
    ui->comTmiSwComBox->setDisabled(true);
    ui->commRb->setChecked(true);
    ui->countBitInp->setDisabled(true);
    ui->numBitInp->setDisabled(true);
    ui->numDataWInp->setDisabled(true);
    ui->reactionTimeInp->setDisabled(true);
    ui->nameCmdTelInp->setDisabled(true);
    ui->descrInp->setDisabled(true);
    ui->submitComTmiPb->setDisabled(true);
    ui->delComTmiPb->setDisabled(true);

    ui->sysComBox->setDuplicatesEnabled(false);
s
    connect(ui->submitKaPb,QPushButton::clicked, this, brtrControl::onSubmitKaClicked);
    connect(ui->submitSysPb,QPushButton::clicked, this, brtrControl::onSubmitSysClicked);
    connect(ui->submitSubSysPb,QPushButton::clicked, this, brtrControl::onSubmitSubSysClicked);
    connect(ui->submitComTmiPb,QPushButton::clicked, this, brtrControl::onSubmitComTmiClicked);

    connect(ui->delKaPb,QPushButton::clicked, this, brtrControl::onDelKaClicked);
    connect(ui->delSysPb,QPushButton::clicked, this, brtrControl::onDelSysClicked);
    connect(ui->delSubSysPb,QPushButton::clicked, this, brtrControl::onDelSubSysClicked);
    connect(ui->delComTmiPb,QPushButton::clicked, this, brtrControl::onDelComTmiClicked);

    connect(ui->kAComBox,SIGNAL(currentIndexChanged(int)), this, SLOT(kaChanged(int)));

    connect(ui->kAComBox, SIGNAL(activated(int)), SLOT(onItemActivated()));
    connect(ui->sysComBox, SIGNAL(activated(int)), SLOT(onSysItemActivated()));
    connect(ui->subSysComBox, SIGNAL(activated(int)), SLOT(onSubSysItemActivated()));
    connect(ui->comTmiSwComBox, SIGNAL(activated(int)), SLOT(onComItemActivated()));
    connect(ui->sysComBox, SIGNAL(activated(int)), SLOT(dispProps()));
    connect(ui->commRb, SIGNAL(clicked(bool)),SLOT(update_com_tmi()));
    connect(ui->telemRb, SIGNAL(clicked(bool)),SLOT(update_com_tmi()));
    connect(ui->commRb, SIGNAL(clicked(bool)),SLOT(update_labels()));
    connect(ui->telemRb, SIGNAL(clicked(bool)),SLOT(update_labels()));


    const QString path = "C:/Users/DmitryP/Documents/sqLiteDb/BrtrMpiControl.db";

    if(QFile::exists(path)){

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("localhost");
        db.setDatabaseName(path);

        if(db.open()){
            qDebug() << "db opened OK...";
        }else{
            qDebug() << " db opening failed...";
        }
    }else{
        qDebug() << "file doesnot exist";
        exit(0);
    }

update_ka();
update_system();
update_subsystem();
update_com_tmi();

}

brtrControl::~brtrControl()
{
    delete ui;
}

void brtrControl::onSubmitKaClicked(){
    QString query = "INSERT INTO KA (ka) VALUES ('" + ui->kaInput->text() + "')";
    QSqlQuery sqlQuery(db);
    qDebug() << "QUERY: " << query;

    if(sqlQuery.exec(query)){

        ui->kAComBox->addItem(ui->kaInput->text());
    }
    else{
          qDebug() << "query failed...";
      }
     update_ka();
     update_system();
     update_subsystem();
     update_com_tmi();
}

void brtrControl::onSubmitSysClicked(){

     QSqlQuery sqlQuery(db);
     id_ka = ui->kAComBox->itemData(ui->kAComBox->currentIndex()).toString();
     QString query = "INSERT INTO System (id_ka, address, name) VALUES ('" + id_ka + "','" + ui->addrInp->text() + "','" + ui->namInp->text() + "')";
     qDebug() << query;
     if(sqlQuery.exec(query)){
         qDebug() << query << " INSERTED OK...";
     }else{
         qDebug() << query << " query failed...";
         qDebug() << sqlQuery.lastError();
     }

     update_system();
     update_subsystem();
     update_com_tmi();
}

void brtrControl::onSubmitSubSysClicked(){
    QSqlQuery sqlQuery(db);
    id_sys = ui->sysComBox->itemData(ui->sysComBox->currentIndex()).toString();
    QString query = "INSERT INTO SubSystem (id_system, subaddress, count_data_word, name) VALUES ('" + id_sys + "','" + ui->subaddrInp->text() + "','" + ui->countDataWordInp->text() + "','" + ui->nameInp->text() + "')";
    qDebug() << query;
    if(sqlQuery.exec(query)){
        qDebug() << query << " INSERTED OK...";
    }else{
        qDebug() << query << " query failed...";
        qDebug() << sqlQuery.lastError();
    }
    ui->submitComTmiPb->setEnabled(1);
    ui->delComTmiPb->setEnabled(1);
    ui->comTmiSwComBox->setEnabled(1);
    ui->countBitInp->setEnabled(1);
    ui->numDataWInp->setEnabled(1);
    ui->reactionTimeInp->setEnabled(1);
    ui->nameCmdTelInp->setEnabled(1);
    ui->descrInp->setEnabled(1);
    ui->numBitInp->setEnabled(1);

    update_subsystem();
    update_com_tmi();
}

void brtrControl::onSubmitComTmiClicked(){
    if(ui->commRb->isChecked()){
        QSqlQuery sqlQuery(db);
        id_subsys = ui->subSysComBox->itemData(ui->subSysComBox->currentIndex()).toString();
        QString query = "INSERT INTO Command (id_subsystem, count_bit, number_bit, number_data_word, reaction_time, name, description) VALUES ('" + id_subsys + "','" + ui->countBitInp->text() + "','" + ui->numBitInp->text() + "','" + ui->numDataWInp->text() + "','" + ui->reactionTimeInp->text() + "','" + ui->nameCmdTelInp->text() + "','" + ui->descrInp->text() + "')";
        qDebug() << query;
        if(sqlQuery.exec(query)){
            qDebug() << query << " INSERTED OK...";
        }else{
            qDebug() << query << " query failed...";
            qDebug() << sqlQuery.lastError();
        }

        update_com_tmi();
    }else{
        if(ui->telemRb->isChecked()){
            QSqlQuery sqlQuery(db);
            id_subsys = ui->subSysComBox->itemData(ui->subSysComBox->currentIndex()).toString();
            QString query = "INSERT INTO TMI (id_subsystem, number_parameter, count_bit, number_bit, number_data_word, name, description) VALUES ('" + id_subsys + "','" + ui->countBitInp->text() + "','" + ui->numBitInp->text() + "','" + ui->numDataWInp->text() + "','" + ui->reactionTimeInp->text() + "','" + ui->nameCmdTelInp->text() + "','" + ui->descrInp->text() + "')";
            qDebug() << query;
            if(sqlQuery.exec(query)){
                qDebug() << query << " INSERTED OK...";
            }else{
                qDebug() << query << " query failed...";
                qDebug() << sqlQuery.lastError();
            }
            update_com_tmi();
        }
    }
}

void brtrControl::onDelKaClicked(){
    QSqlQuery sqlQuery(db);
    sqlQuery.prepare("DELETE FROM KA WHERE id = ?");
    sqlQuery.bindValue(0, ui->kAComBox->currentData().toInt());

    if(!sqlQuery.exec()){
      qDebug() << "onDelKaClicked: query failed..." << sqlQuery.lastError().text();
      qDebug() << "bound =" << sqlQuery.boundValue(":id");
      qDebug() << sqlQuery.executedQuery();
      return;
    }

    delete_records("System", "id_ka", ui->kAComBox);
    delete_records("SubSystem", "id_system", ui->sysComBox);
    delete_records("Command", "id_subsystem",ui->subSysComBox);
    delete_records("TMI", "id_subsystem",ui->subSysComBox);

    ui->kAComBox->removeItem(ui->kAComBox->currentIndex());
    ui->sysComBox->clear();
    ui->namInp->clear();
    ui->addrInp->clear();

    update_ka();
    update_system();
    update_subsystem();
    update_com_tmi();

    ui->comTmiSwComBox->clear();
    ui->countBitInp->clear();
    ui->numBitInp->clear();
    ui->numDataWInp->clear();
    ui->reactionTimeInp->clear();
    ui->nameCmdTelInp->clear();
    ui->descrInp->clear();

    ui->subaddrInp->clear();
    ui->countDataWordInp->clear();
    ui->nameInp->clear();

    ui->addrInp->clear();
    ui->nameInp->clear();

    ui->subSysComBox->clear();
}

void brtrControl::onDelSysClicked(){
    QSqlQuery sqlQuery(db);
    QString query = "DELETE FROM System WHERE id = " + ui->sysComBox->itemData(ui->sysComBox->currentIndex()).toString();
    sqlQuery.prepare(query);
    qDebug() << query;

    if(!sqlQuery.exec()){
      qDebug() << "onDelSysClicked: query failed..." << sqlQuery.lastError().text();
      qDebug() << "bound =" << sqlQuery.boundValue(":id");
      qDebug() << sqlQuery.executedQuery();
      return;
    }
    delete_records("SubSystem", "id_system", ui->sysComBox);
    delete_records("Command", "id_subsystem", ui->comTmiSwComBox);
    ui->sysComBox->removeItem(ui->sysComBox->currentIndex());

    update_system();

    delete_records("Command", "id_subsystem", ui->subSysComBox);
    delete_records("TMI", "id_subsystem",ui->subSysComBox);
    ui->subSysComBox->removeItem(ui->subSysComBox->currentIndex());
    update_subsystem();
    update_com_tmi();
    ui->comTmiSwComBox->clear();
    ui->countBitInp->clear();
    ui->numBitInp->clear();
    ui->numDataWInp->clear();
    ui->reactionTimeInp->clear();
    ui->nameCmdTelInp->clear();
    ui->descrInp->clear();
    ui->subaddrInp->clear();
    ui->countDataWordInp->clear();
    ui->nameInp->clear();
    ui->addrInp->clear();
    ui->namInp->clear();
    ui->subSysComBox->clear();

}

void brtrControl::onDelSubSysClicked(){
    QSqlQuery sqlQuery(db);
    QString query = "DELETE FROM SubSystem WHERE id = " + ui->subSysComBox->itemData(ui->subSysComBox->currentIndex()).toString();
    sqlQuery.prepare(query);
    qDebug() << query;

    if(!sqlQuery.exec()){
      qDebug() << "onDelSubSysClicked: query failed..." << sqlQuery.lastError().text();
      qDebug() << "bound =" << sqlQuery.boundValue(":id");
      qDebug() << sqlQuery.executedQuery();
      return;
    }
    delete_records("Command", "id_subsystem", ui->subSysComBox);
    delete_records("TMI", "id_subsystem",ui->subSysComBox);
    ui->subSysComBox->removeItem(ui->subSysComBox->currentIndex());
    update_subsystem();
    update_com_tmi();
    ui->comTmiSwComBox->clear();
    ui->countBitInp->clear();
    ui->numBitInp->clear();
    ui->numDataWInp->clear();
    ui->reactionTimeInp->clear();
    ui->nameCmdTelInp->clear();
    ui->descrInp->clear();

    ui->subaddrInp->clear();
    ui->countDataWordInp->clear();
    ui->nameInp->clear();
}

void brtrControl::onDelComTmiClicked(){
    QSqlQuery sqlQuery(db);
    QString query = "DELETE FROM Command WHERE id = " + ui->comTmiSwComBox->itemData(ui->comTmiSwComBox->currentIndex()).toString();
    sqlQuery.prepare(query);
    qDebug() << query;

    if(!sqlQuery.exec()){
      qDebug() << "onDelSubSysClicked: query failed..." << sqlQuery.lastError().text();
      qDebug() << "bound =" << sqlQuery.boundValue(":id");
      qDebug() << sqlQuery.executedQuery();
      return;
    }
    ui->comTmiSwComBox->removeItem(ui->comTmiSwComBox->currentIndex());
    ui->countBitInp->clear();
    ui->numDataWInp->clear();
    ui->reactionTimeInp->clear();
    ui->nameCmdTelInp->clear();
    ui->descrInp->clear();
    ui->numBitInp->clear();

    update_com_tmi();
}

 void brtrControl::onItemActivated(){
     qDebug() << "ComboBox element activated";
     qDebug() << "-ID : " << ui->kAComBox->itemData(ui->kAComBox->currentIndex());
     ui->kaInput->setText(ui->kAComBox->currentText());
     ui->addrInp->setEnabled(1);
     ui->namInp->setEnabled(1);
     ui->submitSysPb->setEnabled(1);
     ui->delSysPb->setEnabled(1);
     ui->sysComBox->setEnabled(1);

     update_system();
 }

 void brtrControl::update_ka(){
     ui->kAComBox->clear();
     QString query = "SELECT id, ka FROM KA";

     QSqlQuery sqlQ(db);

     if(!sqlQ.exec(query)) {
         qDebug() << "update_ka: query failed...";
         return;
     }

     while (sqlQ.next()){
         ui->kAComBox->addItem(sqlQ.value(1).toString(),sqlQ.value(0).toInt());
     }

     update_system();
     update_subsystem();
     update_com_tmi();
 }

 void brtrControl::kaChanged(int index)
 {
     qDebug() << "ka changed: ID =" << ui->kAComBox->itemData(index).toInt();
 }

 void brtrControl::update_system(){
     ui->sysComBox->clear();

     QString query = "SELECT id, id_ka, address, name FROM System WHERE id_ka = :id_ka";

     QSqlQuery sqlQ(db);
     sqlQ.prepare(query);
     qDebug() <<  ui->kAComBox->itemData(ui->kAComBox->currentIndex()).toInt();
     QString value =  ui->kAComBox->itemData(ui->kAComBox->currentIndex()).toString();
     sqlQ.bindValue(/*":id_ka"*/0,value);

     if(!sqlQ.exec()) {
         qDebug() << "update_system: query failed...";
         return;
     }

     qDebug() << sqlQ.executedQuery();


     while (sqlQ.next()){
          id      = sqlQ.value(0).toString();
          qDebug() << "id: " << id;
          id_ka   = sqlQ.value(1).toString();
          qDebug() << "id_ka: " << id_ka;
          address = sqlQ.value(2).toString();
          qDebug() << "address: " << address;
          name    = sqlQ.value(3).toString();
          qDebug() << "name: " << name;

          QString item = address+","+name;
          ui->sysComBox->addItem(item, id);
     }

     update_subsystem();
     update_com_tmi();
 }

 void brtrControl::onSysItemActivated(){

     QStringList qsl = ui->sysComBox->currentText().split(',');
     qDebug() << "list: " << qsl;
     qDebug() << "ComboBox element activated";
     qDebug() << "-ID : " << ui->sysComBox->itemData(ui->sysComBox->currentIndex());

     ui->subSysComBox->setEnabled(1);
     ui->subaddrInp->setEnabled(1);
     ui->countDataWordInp->setEnabled(1);
     ui->nameInp->setEnabled(1);
     ui->submitSubSysPb->setEnabled(1);
     ui->delSubSysPb->setEnabled(1);


     ui->addrInp->setText(qsl.at(0));
     ui->namInp->setText(qsl.at(1));

     update_subsystem();
 }

 void brtrControl::delete_records(QString tableName, QString row_name, QComboBox *comBox){
       QSqlQuery sqlQuery(db);
       QString query = " DELETE FROM " + tableName + " WHERE " + row_name + " = " + comBox->currentData().toString();
       sqlQuery.prepare(query);
       qDebug() << "Запрос: " << query;

       if(!sqlQuery.exec()){
           qDebug() << "delete_all_records: query failed..." << sqlQuery.lastError().text();
           qDebug() << sqlQuery.executedQuery();
           return;
       }
   }

   void brtrControl::dispProps(){
       qDebug() << "Properties: " << ui->sysComBox->currentIndex() << ui->sysComBox->currentText() << ui->sysComBox->itemData(ui->sysComBox->currentIndex());

   }

   void brtrControl::update_subsystem(){
       ui->subSysComBox->clear();

       QString query = "SELECT id, id_system, subaddress, count_data_word, name FROM SubSystem WHERE id_system = :id_sys";

       QSqlQuery sqlQ(db);
       sqlQ.prepare(query);

       qDebug() <<  ui->sysComBox->itemData(ui->sysComBox->currentIndex()).toInt();
       QString value =  ui->sysComBox->itemData(ui->sysComBox->currentIndex()).toString();
       sqlQ.bindValue(0,value);

       if(!sqlQ.exec()) {
           qDebug() << "update_subSystem: query failed...";
           return;
       }

       qDebug() << sqlQ.executedQuery();


       while (sqlQ.next()){
            id_ss     = sqlQ.value(0).toString();
            qDebug() << "id: " << id_ss;
            id_s   = sqlQ.value(1).toString();
            qDebug() << "id_system: " << id_s;
            subaddress = sqlQ.value(2).toString();
            qDebug() << "subaddress: " << subaddress;
            count_data_word    = sqlQ.value(3).toString();
            qDebug() << "count_data_word: " << count_data_word;
            name_s   = sqlQ.value(4).toString();
            qDebug() << "name_s: " << name_s;

            QString item = subaddress+","+ count_data_word + "," +name_s;
            ui->subSysComBox->addItem(item, id_ss);
       }
              update_com_tmi();
   }

   void brtrControl::onSubSysItemActivated(){

       QStringList qsl = ui->subSysComBox->currentText().split(',');
       qDebug() << "list: " << qsl;
       qDebug() << "ComboBox element activated";
       qDebug() << "-ID : " << ui->subSysComBox->itemData(ui->subSysComBox->currentIndex());


       ui->subaddrInp->setText(qsl.at(0));
       ui->countDataWordInp->setText(qsl.at(1));
       ui->nameInp->setText(qsl.at(2));

       ui->submitComTmiPb->setEnabled(1);
       ui->delComTmiPb->setEnabled(1);
       ui->comTmiSwComBox->setEnabled(1);
       ui->countBitInp->setEnabled(1);
       ui->numDataWInp->setEnabled(1);
       ui->reactionTimeInp->setEnabled(1);
       ui->nameCmdTelInp->setEnabled(1);
       ui->descrInp->setEnabled(1);
       ui->numBitInp->setEnabled(1);

       update_com_tmi();
   }

   void brtrControl::update_com_tmi(){
           if(ui->commRb->isChecked()){
               ui->comTmiSwComBox->clear();
               QString query = "SELECT id, id_subsystem, count_bit, number_bit, number_data_word, reaction_time, name, description FROM Command WHERE id_subsystem = :id_subsys";

               QSqlQuery sqlQ(db);
               sqlQ.prepare(query);

               QString value =  ui->subSysComBox->itemData(ui->subSysComBox->currentIndex()).toString();
               sqlQ.bindValue(0,value);
               qDebug() << query;
               qDebug() << "id_subsystem i need" <<value;

               if(!sqlQ.exec()) {
                   qDebug() << "update_command: query failed...";
                   return;
               }

               while (sqlQ.next()){
                    id_com     = sqlQ.value(0).toString();
                    qDebug() << "id: " << id_com;
                    id_subs   = sqlQ.value(1).toString();
                    qDebug() << "id_subSystem: " << id_subs;
                    c_bit = sqlQ.value(2).toString();
                    qDebug() << "count_bit: " << c_bit;
                    n_bit    = sqlQ.value(3).toString();
                    qDebug() << "number_bit: " << count_data_word;
                    n_d_word   = sqlQ.value(4).toString();
                    qDebug() << "number_data_word: " << n_d_word;
                    r_time   = sqlQ.value(5).toString();
                    qDebug() << "reaction_time: " << r_time;
                    com_name   = sqlQ.value(6).toString();
                    qDebug() << "name: " << com_name;
                    com_description   = sqlQ.value(7).toString();
                    qDebug() << "descriprion: " << com_description;

                    QString item = c_bit+","+ n_bit + "," + n_d_word + "," + r_time  + "," + com_name  + "," + com_description;
                    qDebug() << "item: " << item;
                    ui->comTmiSwComBox->addItem(item, id_com);
           }
       }else{
               if(ui->telemRb->isChecked()){
                   ui->comTmiSwComBox->clear();

                   QString query = "SELECT id, id_subsystem, number_parameter, count_bit, number_bit, number_data_word, name, description FROM TMI WHERE id_subsystem = :id_subsys";

                   QSqlQuery sqlQ(db);
                   sqlQ.prepare(query);

                   QString value =  ui->subSysComBox->itemData(ui->subSysComBox->currentIndex()).toString();
                   sqlQ.bindValue(0,value);
                   qDebug() << query;
                   qDebug() << "id_subsystem i need" <<value;

                   if(!sqlQ.exec()) {
                       qDebug() << "update_command: query failed...";
                       return;
                   }

                   while (sqlQ.next()){
                        id_tmi     = sqlQ.value(0).toString();
                        qDebug() << "id: " << id_com;
                        id_subs   = sqlQ.value(1).toString();
                        qDebug() << "id_subSystem: " << id_subs;
                        n_par = sqlQ.value(2).toString();
                        qDebug()<< "number_parameter: " << n_par;
                        c_bit = sqlQ.value(3).toString();
                        qDebug() << "count_bit: " << c_bit;
                        n_bit    = sqlQ.value(4).toString();
                        qDebug() << "number_bit: " << count_data_word;
                        n_d_word   = sqlQ.value(5).toString();
                        qDebug() << "number_data_word: " << n_d_word;
                        tmi_name   = sqlQ.value(6).toString();
                        qDebug() << "name: " << tmi_name;
                        tmi_description   = sqlQ.value(7).toString();
                        qDebug() << "descriprion: " << tmi_description;

                        QString item = n_par + "," + c_bit+","+ n_bit + "," + n_d_word + "," + tmi_name  + "," + tmi_description;
                        qDebug() << "item: " << item;
                        ui->comTmiSwComBox->addItem(item, id_tmi);

               }
           }
       }
   }

   void brtrControl::onComItemActivated(){

       QStringList qsl = ui->comTmiSwComBox->currentText().split(',');
       qDebug() << "list: " << qsl;
       qDebug() << "ComboBox element activated";
       qDebug() << "-ID : " << ui->comTmiSwComBox->itemData(ui->comTmiSwComBox->currentIndex());


       ui->countBitInp->setText(qsl.at(0));
       ui->numBitInp->setText(qsl.at(1));
       ui->numDataWInp->setText(qsl.at(2));
       ui->reactionTimeInp->setText(qsl.at(3));
       ui->nameCmdTelInp->setText(qsl.at(4));
       ui->descrInp->setText(qsl.at(5));

   }

   void brtrControl::update_labels(){
       if(ui->commRb->isChecked()){
           ui->cBitL->setText("count_bit");
           ui->nBitL->setText("number_bit");
           ui->nDWL->setText("number_data_word");
           ui->rtL->setText("reaction_time");
           ui->nameL->setText("name");
           ui->descrL->setText("description");

       }else{
           if(ui->telemRb->isChecked()){
               ui->cBitL->setText("number_parameter");
               ui->nBitL->setText("count_bit");
               ui->nDWL->setText("number_bit");
               ui->rtL->setText("number_data_word");
               ui->nameL->setText("name");
               ui->descrL->setText("description");
           }
       }
   }
