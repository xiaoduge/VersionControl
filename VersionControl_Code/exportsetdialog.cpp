#include "exportsetdialog.h"
#include "ui_exportsetdialog.h"
#include <QSqlQuery>

ExportSetDialog::ExportSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportSetDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Export Config"));

    m_targetList.clear();
    m_targetList.clear();
    buildTranslation();
    updateMachineType();
}

ExportSetDialog::~ExportSetDialog()
{
    delete ui;
}

void ExportSetDialog::updateMachineType()
{
    QSqlQuery query;
    QStringList strList;
    QString strQuery = QString("select machine_Type from MACHINETYPE");
    query.exec(strQuery);
    while (query.next())
    {
            QString str = query.value(0).toString();
            if(!str.isEmpty())
            {
                strList << str;
            }
    }
    ui->listWidget_Source->clear();
    ui->listWidget_Source->addItems(strList);
}

void ExportSetDialog::buildTranslation()
{
    ui->pushButton_Add->setText(tr("Add"));
    ui->pushButton_Sub->setText(tr("Sub"));
    ui->pushButton_OK->setText(tr("OK"));
    ui->label_Source->setText(tr("SourceTable"));
    ui->label_Target->setText(tr("TargetTable"));

}

void ExportSetDialog::on_pushButton_Add_clicked()
{
    QString str = ui->listWidget_Source->currentItem()->text();
    if(!m_targetList.contains(str))
    {
        m_targetList << str;
    }
    ui->listWidget_Target->clear();
    ui->listWidget_Target->addItems(m_targetList);
}

void ExportSetDialog::on_pushButton_Sub_clicked()
{
    if(!ui->listWidget_Target->currentItem())
    {
        return;
    }
    QString strTmp = ui->listWidget_Target->currentItem()->text();
    m_targetList.removeOne(strTmp);
    ui->listWidget_Target->clear();
    ui->listWidget_Target->addItems(m_targetList);
}

void ExportSetDialog::on_pushButton_OK_clicked()
{
    for(int i = 0; i < m_targetList.size(); i++)
    {
        QSqlQuery query;
        QString strQuery = QString("select machine_Name from MACHINETYPE where machine_Type = '%1'")
                                  .arg(m_targetList.at(i));
        query.exec(strQuery);
        while (query.next())
        {
                QString str = query.value(0).toString();
                if(!str.isEmpty())
                {
                    m_targetNameList << str;
                }
        }
    }
    emit exportTableList(m_targetList);
    emit exportTableName(m_targetNameList);
    close();
}
