#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTextCodec>
#include <QTranslator>
#include <QDebug>

void initSql()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    db.setHostName("DCJ");
    db.setDatabaseName("DCJ");
    db.setUserName("DCJ");
    db.setPassword("DCJ");
    if(!db.open())
    {
        QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("Init Database Failed"), QMessageBox::Ok);
        return;
    }

    QString CREATE_TABLE_MACHINETYPE = "CREATE TABLE IF NOT EXISTS  MACHINETYPE (machine_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                                "machine_Type VARCHAR(24) NOT NULL,"
                                                                                "machine_Name VARCHAR(48) NOT NULL,"
                                                                                "machine_Time VARCHAR(20) NOT NULL,"
                                                                                "version_Board varchar(24))";


    QSqlQuery query;
    bool success = query.exec(CREATE_TABLE_MACHINETYPE);
    if(success)
        qDebug()<< QObject::tr("create MachineType type table successed");
    else
        qDebug()<< QObject::tr("create MachineType table failed");
    //NOTE:数据库表插入一行
 /*
    QString INSERT_COLUMN = "alter table MACHINETYPE add version_Board varchar(24) ";
    success = query.exec(INSERT_COLUMN);
    if(success)
        qDebug()<< QObject::tr("Add column successed");
    else
        qDebug()<< QObject::tr("Add column failed");

*/

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //NOTE : 初始化数据库
    initSql();
    MainWindow w;
    w.show();

    return a.exec();
}
