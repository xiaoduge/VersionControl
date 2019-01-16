#include "dtypedialog.h"
#include "ddockwidget.h"
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QGridLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QHeaderView>
#include <QDebug>

DTypeDialog::DTypeDialog(QWidget *parent, Qt::WindowFlags f):QDialog(parent, f)
{
    this->resize(925, 350);
    initUI();
    this->setWindowTitle(tr("Type Set"));
}

void DTypeDialog::on_dockWidgetBtn_clicked(int id)
{
    switch (id)
    {
    case BTN_QUERY_CLICKED:
        qDebug() << tr("Dock btn clicked:%1").arg(id);
        break;
    case BTN_DISPALYALL_CLICKED:
        displayAll();
        break;
    case BTN_SUBMIT_CLICKED:
        submit();
        break;
    case BTN_RECOVER_CLICKED:
        recover();
        break;
    case BTN_ADD_CLICKED:
        add();
        break;
    case BTN_DELETE_CLICKED:
        deleteRow();
        break;
    default:
        break;
    }
}

void DTypeDialog::initUI()
{
    m_pDockWidget = new DDockWidget(false);
    m_pTableView = new QTableView;

    QGridLayout *gLayout = new QGridLayout;
    gLayout->addWidget(m_pDockWidget, 0, 0, 1, 3);
    gLayout->addWidget(m_pTableView, 0, 3, 1, 1);
    this->setLayout(gLayout);

    m_pTableModel = new QSqlTableModel(this);
    m_pTableModel->setTable("MACHINETYPE");
    m_pTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pTableModel->select();
    initTabelHeaderData(m_pTableModel);
    m_pTableView->setModel(m_pTableModel);

    setFixView();

    connect(m_pDockWidget, SIGNAL(btn_clicked(int)), this, SLOT(on_dockWidgetBtn_clicked(int)));
}

void DTypeDialog::query()
{

}

void DTypeDialog::displayAll()
{
    m_pTableModel->setTable("MACHINETYPE");
    m_pTableModel->select();
    initTabelHeaderData(m_pTableModel);
    setFixView();
}

void DTypeDialog::submit()
{
    m_pTableModel->database().transaction(); //
    if(m_pTableModel->submitAll())
    {
        m_pTableModel->database().commit();
        emit typeListChanged();
    }
    else
    {
        m_pTableModel->database().rollback(); //
        QMessageBox::warning(this, tr("tableModel"),
                             tr("Database Error:%1").arg(m_pTableModel->lastError().text()));
    }
}

void DTypeDialog::recover()
{
    m_pTableModel->revertAll();
}

void DTypeDialog::add()
{
    int rowNum = m_pTableModel->rowCount();
    m_pTableModel->insertRow(rowNum);
    QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    m_pTableModel->setData(m_pTableModel->index(rowNum, 3, QModelIndex()), curTime);
    setFixView();
}

void DTypeDialog::deleteRow()
{
    int curRow = m_pTableView->currentIndex().row();
    m_pTableModel->removeRow(curRow);

    int ok = QMessageBox::warning(this, tr("Delete"), tr("Are you sure delete current row?"),
                                  QMessageBox::Yes, QMessageBox::No);
    if(QMessageBox::No == ok)
    {
        m_pTableModel->revertAll();
    }
    else
    {
        m_pTableModel->submitAll();
        emit typeListChanged();
    }
}

void DTypeDialog::setFixView()
{
    m_pTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_pTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    m_pTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    m_pTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    m_pTableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    m_pTableView->setColumnWidth(0, 90);
    m_pTableView->setColumnWidth(1, 180);
    m_pTableView->setColumnWidth(2, 178);
    m_pTableView->setColumnWidth(3, 175);
    m_pTableView->setColumnWidth(4, 160);
}

void DTypeDialog::initTabelHeaderData(QSqlTableModel *tableModel)
{
    tableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    tableModel->setHeaderData(1, Qt::Horizontal, tr("TableName"));
    tableModel->setHeaderData(2, Qt::Horizontal, tr("Name"));
    tableModel->setHeaderData(3, Qt::Horizontal, tr("Time"));
    tableModel->setHeaderData(4, Qt::Horizontal, tr("version_Board"));
}
