#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QSqlQuery>
#include "initdatabase.h"
#include <QList>
#include <QMapIterator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlError err = initDatabase(db);
        if (err.type() != QSqlError::NoError) {
            qDebug() << "initError" << err;
            showError(err);
            return;
        } else {
        menager.initMenager(db);
        }

//    QSqlQuery query;
//    query.setForwardOnly(true);
    // WYSWIETLANIE
//    query.prepare("INSERT INTO PACJENT VALUES (1, 'Jakub', 'Michalowski', 'Bydgoszcz', 'mail@mail.com')");
//        query.addBindValue( contactid );
//        query.addBindValue( ct_name );
//        query.addBindValue( extno );
//        query.addBindValue( ct_private );
//        query.addBindValue( ct_favorite );


//        if(!q.exec()) {
//        qDebug() << query.executedQuery();
//        qDebug() <<  query.lastError();
//        } else { qDebug() << " ---- aded client to table PACJENT ----"; }



    connect(&menager,&SQLMenager::updateComboForChoroba,this,&MainWindow::updateComboChoroba);
    connect(&menager,&SQLMenager::updateComboForLekarz,this,&MainWindow::updateComboLekarz);
    connect(&menager,&SQLMenager::updateComboForLek,this,&MainWindow::updateComboLek);
    connect(this,&MainWindow::updateComboPacjentSig,this,&MainWindow::updateComboPacjent);

    connect(&menager,&SQLMenager::updateChorobaId,this,&MainWindow::updateChorobaId);
    connect(&menager,&SQLMenager::updateinterakcjaId,this,&MainWindow::updateinterakcjaId);
    connect(&menager,&SQLMenager::updatedawkowanieId,this,&MainWindow::updatedawkowanieId);
    connect(&menager,&SQLMenager::updatewizytaId,this,&MainWindow::updatewizytaId);

    modelPacjent = new QStandardItemModel(this);
    modelLekarz = new QStandardItemModel(this);
    modelLek = new QStandardItemModel(this);
    modelChoroba = new QStandardItemModel(this);
    modelDawkowanie = new QStandardItemModel(this);
    modelInterakcja = new QStandardItemModel(this);
    modelWizyta = new QStandardItemModel(this);
    modelSerwis = new QStandardItemModel(this);
//    tModel = new QSqlTableModel(this, db);

//    tModel->setTable("PACJENT");
//    tModel->setEditStrategy(QSqlTableModel::OnFieldChange);
//    tModel->select();
//    qDebug() << tModel->lastError();
//    tModel->fetchMore();
//    ui->tableView_2->setModel(tModel);
    setModelPacjent(); //populate model from a query
    menager.setupModelLekarze(modelLekarz);
    menager.setupModelLek(modelLek);
    menager.setupModelChoroba(modelChoroba);
    menager.setupModelDawkowanie(modelDawkowanie);
    menager.setupModelInterakcja(modelInterakcja);
    menager.setupModelWizyta(modelWizyta);

    ui->tableView->setModel(modelPacjent);
    ui->tableView_2->setModel(modelLekarz);
    ui->tableView_3->setModel(modelLek);
    ui->tableView_4->setModel(modelInterakcja);
    ui->tableView_5->setModel(modelChoroba);
    ui->tableView_4dawka->setModel(modelDawkowanie);
    ui->tableView_6wizyta->setModel(modelWizyta);
    ui->tableView_11->setModel(modelSerwis);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateComboPacjent(QMap<int, QString> &mapa)
{
    ui->comboBox_wizytaPacjent->clear();
    QMapIterator<int, QString> i(mapa);
    while (i.hasNext()) {
        i.next();
        ui->comboBox_wizytaPacjent->addItem(i.value(), QVariant(i.key()));
    }
}

void MainWindow::updateComboLekarz(QMap<int, QString> &mapa)
{
    ui->comboBox_wizytaLekarz->clear();
    QMapIterator<int, QString> i(mapa);
    while (i.hasNext()) {
        i.next();
        ui->comboBox_wizytaLekarz->addItem(i.value(), QVariant(i.key()));
    }

}

void MainWindow::updateComboLek(QMap<int, QString> &mapa)
{
    ui->comboBoxLekInterakcja->clear();
    ui->comboBoxLekInterakcja_3->clear();
    ui->comboBox_2lekDawkowanie->clear();
    ui->comboBox_wizytaLek->clear();
    ui->comboBox_wizytaLek_2->clear();
    ui->comboBox_wizytaLek_3->clear();
    ui->comboBox_wizytaLek_4->clear();

    ui->comboBox_wizytaLek->addItem("", "");
    ui->comboBox_wizytaLek_2->addItem("", "");
    ui->comboBox_wizytaLek_3->addItem("", "");
    ui->comboBox_wizytaLek_4->addItem("", "");

    QMapIterator<int, QString> i(mapa);
    while (i.hasNext()) {
        i.next();
        ui->comboBoxLekInterakcja->addItem(i.value(), QVariant(i.key()));
        ui->comboBoxLekInterakcja_3->addItem(i.value(), QVariant(i.key()));
        ui->comboBox_2lekDawkowanie->addItem(i.value(), QVariant(i.key()));
        ui->comboBox_wizytaLek->addItem(i.value(), QVariant(i.key()));
        ui->comboBox_wizytaLek_2->addItem(i.value(), QVariant(i.key()));
        ui->comboBox_wizytaLek_3->addItem(i.value(), QVariant(i.key()));
        ui->comboBox_wizytaLek_4->addItem(i.value(), QVariant(i.key()));
    }
}

void MainWindow::updateComboChoroba(QMap<int, QString> &mapa)
{
    ui->comboBoxChorobaDoKasowania->clear();
    ui->comboBoxChorobaDoKasowania->addItem("", "");
    ui->comboBox_3chorobaDawkowanie->clear();
    ui->comboBox_wizytaChoroba->clear();
    ui->comboBox_wizytaChoroba->addItem("", "");

    int index = 0;
    QMapIterator<int, QString> i(mapa);
    while (i.hasNext()) {
        i.next();
        ui->comboBoxChorobaDoKasowania->insertItem(index++,i.value(), QVariant(i.key()));
        ui->comboBox_3chorobaDawkowanie->insertItem(index++,i.value(), QVariant(i.key()));
        ui->comboBox_wizytaChoroba->insertItem(index++,i.value(), QVariant(i.key()));
    }
}

void MainWindow::updateChorobaId(int id)
{
    chorobaid = id;
}

void MainWindow::updateinterakcjaId(int id)
{
    interakcjaId = id;
}

void MainWindow::updatedawkowanieId(int id)
{
    dawkowanieId = id;
}

void MainWindow::updatewizytaId(int id)
{
    wizytaId = id;
}

void MainWindow::showError(const QSqlError &err) {
    QMessageBox::critical(this, "Unable to initialize Database",
                          "Error initializing database: " + err.text());
}

void MainWindow::setModelPacjent()
{
    modelPacjent->clear();
    QSqlQuery query;
     query.setForwardOnly(true);
    // WYSWIETLANIE
    QString queryStr = "SELECT * FROM PACJENT ;";
    // DODAWANIE


    qDebug() << queryStr;
    if (!query.exec(queryStr))
         qDebug() <<  query.lastError();

    QList<QStandardItem*> listaItemow0;
    QList<QStandardItem*> listaItemow1;
    QList<QStandardItem*> listaItemow2;
    QList<QStandardItem*> listaItemow3;
    QStandardItem* item0;
    QStandardItem* item1;
    QStandardItem* item2;
    QStandardItem* item3;
    mapaPacjentow.clear();
    while ( query.next()) {
//         qDebug() << "----------";
////        for(int i=0; i<5; i++){
//            qDebug() <<  query.value(0).toString() <<  query.value(1).toString() <<  query.value(2).toString() <<  query.value(3).toString();
////        }
        mapaPacjentow.insert(query.value(0).toInt(),query.value(1).toString());
        item0 = new QStandardItem( query.value(0).toString());
        item1 = new QStandardItem( query.value(1).toString());
        item2 = new QStandardItem( query.value(2).toString());
        item3 = new QStandardItem( query.value(3).toString());
        listaItemow0.append(item0);
        listaItemow1.append(item1);
        listaItemow2.append(item2);
        listaItemow3.append(item3);


    }
    modelPacjent->appendColumn(listaItemow0);
    modelPacjent->appendColumn(listaItemow1);
    modelPacjent->appendColumn(listaItemow2);
    modelPacjent->appendColumn(listaItemow3);
    modelPacjent->setHeaderData(0, Qt::Horizontal, tr("Nr Ubezpieczenia"));
    modelPacjent->setHeaderData(1, Qt::Horizontal, tr("Imię i Nazwisko"));
    modelPacjent->setHeaderData(2, Qt::Horizontal, tr("Adres"));
    modelPacjent->setHeaderData(3, Qt::Horizontal, tr("Medium Kontaktu"));

    emit updateComboPacjentSig(mapaPacjentow);
}

void MainWindow::on_pushButton_clicked()
{
    QSqlQuery  query;
    QString queryStr("INSERT INTO PACJENT VALUES ( ");     //10,'Jan','AAAA','bbbb',500400300)"
    queryStr.append(ui->lineEdit->text());
    queryStr.append(" , '");
    queryStr.append(ui->lineEdit_2->text());
    queryStr.append("' , '");
    queryStr.append(ui->lineEdit_3->text());
    queryStr.append("' , '");
    queryStr.append(ui->lineEdit_4->text());
    queryStr.append(" ') ;");
    qDebug() << "inserting with query: "<< queryStr;

    if (!query.exec(queryStr))
         qDebug() <<  query.lastError();
        setModelPacjent();
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQuery query;
    QString queryStr("DELETE FROM PACJENT WHERE NR_UBEZPIECZENIA =  ");
    queryStr.append(ui->lineEdit->text());
    qDebug() << queryStr;

    if (!query.exec(queryStr))
         qDebug() <<  query.lastError();
    setModelPacjent();
}

void MainWindow::on_pushButton_dodajLekarza_clicked()
{
    menager.insertIntoLekarze(ui->lineEdit_lekarz_nr->text(),ui->lineEdit_lekarz_imieNazw->text(),ui->lineEdit_telefon->text());
    menager.setupModelLekarze(modelLekarz);
}

void MainWindow::on_pushButton_kasujLekarza_clicked()
{
    menager.deleteFromLekarze(ui->lineEdit_lekarz_nr->text());
    menager.setupModelLekarze(modelLekarz);
}

void MainWindow::on_pushButton_DodajLek_clicked()
{
    menager.insertIntoLek(ui->lineEdit_5kodleku->text(),ui->lineEdit_8nazwalieku->text());
    menager.setupModelLek(modelLek);
}

void MainWindow::on_pushButton_KasujLek_clicked()
{
    menager.deleteFromLek(ui->lineEdit_5kodleku->text());
    menager.setupModelLek(modelLek);
}

void MainWindow::on_pushButton_DodajLek_3_clicked()
{
    //gdy interakcja
    menager.insertIntoInterakcja(QString::number(++interakcjaId),ui->comboBoxLekInterakcja_3->currentText(),ui->comboBoxLekInterakcja->currentText());
    menager.setupModelInterakcja(modelInterakcja);
}

void MainWindow::on_pushButton_KasujLek_3_clicked()
{
    menager.deleteFromInterakcja(ui->lineEdit_5kodleku->text());
    menager.setupModelInterakcja(modelInterakcja);
    //gdy interakcja
}

void MainWindow::on_pushButton_DodajChorobe_clicked()
{
    menager.insertIntoChoroba(QString::number(++chorobaid),ui->lineEdit_16nazwaChoroby->text(),ui->plainTextEditObjawyChoroby->toPlainText(),ui->plainTextEditOpisChoroby->toPlainText());
    menager.setupModelChoroba(modelChoroba);
}

void MainWindow::on_pushButton_KasujChorobe_clicked()
{
    menager.deleteFromChoroba(ui->comboBoxChorobaDoKasowania->currentData().toString());
    menager.setupModelChoroba(modelChoroba);
}

void MainWindow::on_pushButton_4_clicked() // kasuj wizytę
{
    QMessageBox::warning(this,"Uwaga!!!","Kasowanie danych o wizytach tylko przez administratora aplikacji!!!");
}

void MainWindow::on_pushButton_3_clicked()  // dodaj wizytę
{
//    menager.insertIntoWizyta(QString::number(++wizytaId),ui->dateEdit->text(),ui->comboBox_wizytaPacjent->currentData().toString(),ui->comboBox_wizytaLekarz->currentData().toString(),ui->comboBox_wizytaChoroba->currentData().toString(),ui->comboBox_wizytaLek->currentData().toString(),ui->comboBox_wizytaLek_2->currentData().toString(),ui->comboBox_wizytaLek_3->currentData().toString(),ui->comboBox_wizytaLek_4->currentData().toString());
    menager.insertIntoWizyta(QString::number(++wizytaId),ui->dateEdit->text(),ui->comboBox_wizytaPacjent->currentText(),ui->comboBox_wizytaLekarz->currentText(),ui->comboBox_wizytaChoroba->currentText(),ui->comboBox_wizytaLek->currentText(),ui->comboBox_wizytaLek_2->currentText(),ui->comboBox_wizytaLek_3->currentText(),ui->comboBox_wizytaLek_4->currentText());
    menager.setupModelWizyta(modelWizyta);
}

void MainWindow::on_pushButton_DodajDawkowanie_clicked()
{
    menager.insertIntoDawkowanie(ui->comboBox_2lekDawkowanie->currentText(),ui->comboBox_3chorobaDawkowanie->currentText(),ui->lineEdit_11->text());
    menager.setupModelDawkowanie(modelDawkowanie);
}

void MainWindow::on_pushButton_KasujDawkowanie_clicked()
{
    menager.deleteFromDawkowanie(ui->comboBox_2lekDawkowanie->currentText(),ui->comboBox_3chorobaDawkowanie->currentText());
    menager.setupModelDawkowanie(modelDawkowanie);
}

void MainWindow::on_pushButton_9_clicked()
{
    menager.setupModelSerwis(modelSerwis,ui->plainTextEdit->toPlainText());
}
