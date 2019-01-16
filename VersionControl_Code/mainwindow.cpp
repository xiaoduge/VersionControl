#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ddockwidget.h"
#include "dtypedialog.h"
#include "dtexteditdelegate.h"
#include "exportsetdialog.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTableView>
#include <QFrame>
#include <QSpacerItem>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDateTime>
#include <QMessageBox>
#include <QTranslator>
#include <QTextCodec>
#include <QFile>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTextBlock>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initTranslation();
    this->resize(800, 600);
    initUI();
    initMenuAndTool();
    initTableName();
    this->setWindowTitle(tr("VersionController"));
    m_pPrinter = new QPrinter;
}

MainWindow::~MainWindow()
{
    if(m_pPrinter != Q_NULLPTR)
    {
        delete m_pPrinter;
        m_pPrinter = Q_NULLPTR;
    }
    delete ui;
}

void MainWindow::on_dockWidgetBtn_clicked(int id)
{
    if(m_tableName.isEmpty())
    {
        return;
    }
    switch (id)
    {
    case BTN_QUERY_CLICKED:
        query();
        qDebug() << tr("Dock btn clicked:%1").arg(id);
        break;
    case BTN_DISPALYALL_CLICKED:
        displayAll();
        qDebug() << tr("Dock btn clicked:%1").arg(id);
        break;
    case BTN_SUBMIT_CLICKED:
        submit();
        qDebug() << tr("Dock btn clicked:%1").arg(id);
        break;
    case BTN_RECOVER_CLICKED:
        recover();
        qDebug() << tr("Dock btn clicked:%1").arg(id);
        break;
    case BTN_ADD_CLICKED:
        add();
        qDebug() << tr("Dock btn clicked:%1").arg(id);
        break;
    case BTN_DELETE_CLICKED:
        deleteRow();
        qDebug() << tr("Dock btn clicked:%1").arg(id);
        break;
    case BTN_ASC_CLICKED:
        ascendingSort();
        qDebug() << tr("Dock btn clicked:%1").arg(id);
        break;
    case BTN_DES_CLICKED:
        descendingSort();
        qDebug() << tr("Dock btn clicked:%1").arg(id);
        break;
    default:
        break;
    }
}

void MainWindow::on_dockWidget_typeChanged(const QString &typeName)
{
    m_tableName = typeName;

    QString queryBoard = QString("select version_Board from MACHINETYPE where machine_Type = '%1'").arg(m_tableName);
    QSqlQuery query;
    bool ret = query.exec(queryBoard);
    if(ret)
    {
        if(query.next())
        {
            m_boardVersion = query.value(0).toString();
        }

    }
    else
    {
        qDebug() << "query board version failed";
    }

    qDebug() << tr("typeName: %1; BoardVersion: %2").arg(m_tableName).arg(m_boardVersion);
}

void MainWindow::on_dockWidget_queryFiterChanged(const QString &text)
{
    m_versionName = text;
    qDebug() << tr("VersionName: %1").arg(m_versionName);
}

void MainWindow::on_typeAction_triggered()
{
    m_pTypeDialog->show();
}

void MainWindow::on_csvAction_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save CSV"),
                                                    "C:/Users/ADMIN/Desktop/",
                                                    "CSV (*.csv)");
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("Waring"), tr("Save csv file Failed"), QMessageBox::Ok);
        return;
    }
    QTextStream out(&file);
    out << QString("ID") << ","
        << QString("Version") << ","
        << QString("Caption") << ","
        << QString("Board") << ","
        << QString("Time") << "\n";

    QSqlQuery query;
    query.exec(QString("select * from %1").arg(m_tableName));
    while(query.next())
    {
        out << query.value(0).toInt() << ","
            << query.value(1).toString() << ","
            << query.value(2).toString().remove("\n") << ","
            << query.value(3).toString() << ","
            << query.value(4).toString() << "\n";
    }

    file.close();
}

void MainWindow::on_pdfAction_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save PDF"),
                                                    "C:/Users/ADMIN/Desktop/",
                                                    "PDF (*.PDF)");

     QString html;
    if(m_exportTableList.isEmpty())
    {
        html = getHtml();
    }
    else
    {
        html = getMulHtml();
    }

    qDebug() << "Html: " << html;

    m_pPrinter->setPageSize(QPrinter::A4);
    m_pPrinter->setOutputFormat(QPrinter::PdfFormat);
    m_pPrinter->setOutputFileName(fileName);

    QTextDocument textDoc;
    textDoc.setHtml(html);
    textDoc.print(m_pPrinter);
    textDoc.end();
}

void MainWindow::on_exportSetAction_triggered()
{
    m_pExportSetDialog->show();
}

void MainWindow::query()
{
    m_pTableModel->setTable(m_tableName);
    m_pTableModel->setFilter(QString("version_Num = '%1'").arg(m_versionName));
    m_pTableModel->select();
    initTabelHeaderData(m_pTableModel);
    m_pTableView->setModel(m_pTableModel);
    setFixViewWidth();
}

void MainWindow::displayAll()
{
    m_pTableModel->setTable(m_tableName);
    m_pTableModel->select();
    initTabelHeaderData(m_pTableModel);
    m_pTableView->setModel(m_pTableModel);
    setFixViewWidth();
}
void MainWindow::submit()
{
    m_pTableModel->database().transaction(); //
    if(m_pTableModel->submitAll())
    {
        m_pTableModel->database().commit();
    }
    else
    {
        m_pTableModel->database().rollback(); //
        QMessageBox::warning(this, tr("tableModel"),
                             tr("Database Error:%1").arg(m_pTableModel->lastError().text()));
    }
}
void MainWindow::recover()
{
    m_pTableModel->revertAll();
}
void MainWindow::add()
{
    int rowNum = m_pTableModel->rowCount();
    m_pTableModel->insertRow(rowNum);
    QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    m_pTableModel->setData(m_pTableModel->index(rowNum, 4, QModelIndex()), curTime);

    if(!m_boardVersion.isEmpty())
    {
        m_pTableModel->setData(m_pTableModel->index(rowNum, 3, QModelIndex()), m_boardVersion);
    }
}
void MainWindow::deleteRow()
{
    int curRow = m_pTableView->currentIndex().row();

    int ok = QMessageBox::warning(this, tr("Delete"), tr("Are you sure delete current row?"),
                                  QMessageBox::Yes, QMessageBox::No);
    if(QMessageBox::Yes == ok)
    {
        m_pTableModel->removeRow(curRow);
        m_pTableModel->revertAll();
    }
    else
    {
        m_pTableModel->submitAll();
    }

}

void MainWindow::ascendingSort()
{
    m_pTableModel->setTable(m_tableName);
    m_pTableModel->setSort(0, Qt::AscendingOrder);
    m_pTableModel->select();
    initTabelHeaderData(m_pTableModel);
    m_pTableView->setModel(m_pTableModel);
    setFixViewWidth();
}

void MainWindow::descendingSort()
{
    m_pTableModel->setTable(m_tableName);
    m_pTableModel->setSort(0, Qt::DescendingOrder);
    m_pTableModel->select();
    initTabelHeaderData(m_pTableModel);
    m_pTableView->setModel(m_pTableModel);
    setFixViewWidth();
}

void MainWindow::updateExportList(const QStringList& strList)
{
    m_exportTableList = strList;
    for(int i = 0; i < m_exportTableList.size(); i++)
    {
        qDebug() <<"List: " << m_exportTableList.at(i);
    }
}

void MainWindow::updateExportTableName(const QStringList &strList)
{
    m_exportTableName = strList;
    for(int i = 0; i < m_exportTableName.size(); i++)
    {
        qDebug() <<"List: " << m_exportTableName.at(i);
    }
}

const QString MainWindow::getHtml()
{
    QString html;
    QSqlQuery query;
    query.exec(QString("select * from %1").arg(m_tableName));

    int rows = 0;
    QList<QStringList> html_list;
    QStringList titleList;
    titleList << "ID" << "Version" << "Caption" << "Board" << "Time";
    html_list << titleList;
    rows++;

    while(query.next())
    {
        QStringList tmpList;
        tmpList << QString("%1").arg(query.value(0).toInt())
                << query.value(1).toString()
                << query.value(2).toString()
                << query.value(3).toString()
                << query.value(4).toString();;
        html_list << tmpList;
        rows++;
    }

    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    html += "<h1 align=\"center\">" + QString("%1").arg(m_tableName) + "</h1>";
    html += "<h2 align=\"center\">" + currentTime + "</h2>";
    html += "<table border=\"1\" align=\"center\" style=\"border-collapse:collapse;\" bordercolor=\"gray\">";//width=\"500\"
    html += "<tr style=\"background-color:gray\">";

    for(int i = 0; i < 5; i++)
    {
        html += QString("<th>%1</th>").arg(html_list.at(0).at(i)); //绘制表头
    }

    html += "</tr>";
    for(int i = 1; i < rows; i++)
    {
        html += "<tr>";
        QStringList contentList = html_list.at(i);

        for(int k = 0; k < 5; k++)
        {
            QString tmp = contentList.at(k);
            html += QString("<td>%1</td>").arg(tmp);
        }
        html += "</tr>";
    }
    html += "</table>";
    return html;
}

const QString MainWindow::getMulHtml()
{
    int tableNum = m_exportTableList.size();
    QString html;

    html += "<style>\
            table.gridtable {\
                font-size:11px;\
                color:#333333;\
                border-width: 1px;\
                border-color: #666666;\
                border-collapse: collapse;\
            }\
            table.gridtable th {\
                border-width: 1px;\
                padding: 8px;\
                border-style: solid;\
                border-color: #666666;\
                background-color: #C7EDCC;\
            }\
            table.gridtable td {\
                border-width: 1px;\
                padding: 8px;\
                border-style: solid;\
                border-color: #666666;\
                background-color: #ffffff;\
            }\
            </style>";

    for(int i = 0; i < tableNum; i++)
    {
        QSqlQuery query;
        query.exec(QString("select * from %1").arg(m_exportTableList.at(i)));
        int rows = 0;
        QList<QStringList> html_list;
        QStringList titleList;
        titleList << tr("ID") << tr("Version") << tr("Caption") << tr("Board") << tr("Time");
        html_list << titleList;
        rows++;

        while(query.next())
        {
            QStringList tmpList;
            tmpList << QString("%1").arg(query.value(0).toInt())
                    << query.value(1).toString()
                    << query.value(2).toString()
                    << query.value(3).toString()
                    << query.value(4).toString();;
            html_list << tmpList;
            rows++;
        }

        QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

        html += "<h1 align=\"center\">" + QString("    ") + "</h1>";
        html += "<h1 align=\"center\">" + QString("%1").arg(m_exportTableName.at(i)) + "</h1>";
        html += "<h2 align=\"center\">" + currentTime + "</h2>";
        html += "<table class=\"gridtable\" align=\"center\">";
        html += "<tr>";

        for(int i = 0; i < 5; i++)
        {
            html += QString("<th>%1</th>").arg(html_list.at(0).at(i)); //绘制表头
        }

        html += "</tr>";
        for(int i = 1; i < rows; i++)
        {
            html += "<tr>";
            QStringList contentList = html_list.at(i);

            for(int k = 0; k < 5; k++)
            {
                QString tmp = contentList.at(k);
                html += QString("<td>%1</td>").arg(tmp);
            }
            html += "</tr>";
        }
        html += "</table>";
    }

    qDebug() << "html: " << html;
    return html;
}

void MainWindow::initTranslation()
{
    QTranslator *pTranslator_CN = new QTranslator(this);
    bool ok = pTranslator_CN->load(QLatin1String(":/language/cn.qm"));
    if(!ok)
    {
        qDebug() << "Load language failed";
    }

    ok = qApp->installTranslator(pTranslator_CN);
    if(!ok)
    {
        qDebug() << "qApp->installTranslator falied";
    }

}

void MainWindow::buildTranslation()
{

}

void MainWindow::initDockWidget()
{
    m_pDockWidget = new QDockWidget(tr("Manage"), this);
    m_pDockWidget->setObjectName("dockWidget");

    QString qss = "QFrame#dockWidget{color:white;}";
    this->setStyleSheet(qss);

    m_pWidget = new DDockWidget(m_pDockWidget);
    m_pDockWidget->setWidget(m_pWidget);

    this->addDockWidget(Qt::LeftDockWidgetArea, m_pDockWidget);

    connect(m_pWidget, SIGNAL(btn_clicked(int)), this, SLOT(on_dockWidgetBtn_clicked(int)));
    connect(m_pWidget, SIGNAL(queryFiterChanged(const QString&)),
            this, SLOT(on_dockWidget_queryFiterChanged(const QString&)));
    connect(m_pWidget, SIGNAL(type_Changed(const QString&)),
            this, SLOT(on_dockWidget_typeChanged(const QString&)));

}

void MainWindow::initUI()
{
    initDockWidget();

    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QColor(0, 55, 120))); //
    this->setPalette(palette);

    m_pTableView = new QTableView(this);
//    m_pTableView ->setAlternatingRowColors(true);
    //#003778
    QString qss = "QTableView {color: white;   \
                               gridline-color: black;       \
                               background-color: rgb(0, 55, 120);       \
                               alternate-background-color: rgb(0, 80, 120);\
                               selection-color: white;                  \
                               selection-background-color: rgb(0, 100, 0);  \
                               border: 2px groove gray;\
                               border-radius: 0px;\
                               padding: 2px 4px;\
                               }";
    m_pTableView->setStyleSheet(qss);

    QString qssHeader = "QHeaderView {color: blue; \
                                      font: bold 10pt; \
                                      background-color: rgb(0, 55, 120);       \
                                      border: 0px solid rgb(144, 144, 144);\
                                      border:0px solid rgb(191,191,191);\
                                      border-left-color: rgba(255, 255, 255, 0);\
                                      border-top-color: rgba(255, 255, 255, 0);\
                                      border-radius:0px;\
                                      min-height:29px;\
                                      }";
    m_pTableView->horizontalHeader()->setStyleSheet(qssHeader);

    QHBoxLayout *hLayout =  new QHBoxLayout;

    hLayout->addWidget(m_pTableView);
    ui->centralWidget->setLayout(hLayout);

    DTextEditDelegate *textEditDelegate = new DTextEditDelegate(this);
    m_pTableView->setItemDelegate(textEditDelegate);

    m_pTypeDialog = new DTypeDialog(this);
    connect(m_pTypeDialog, SIGNAL(typeListChanged()), m_pWidget, SLOT(updateMachineType()));

    m_pExportSetDialog = new ExportSetDialog(this);
    connect(m_pTypeDialog, SIGNAL(typeListChanged()), m_pExportSetDialog, SLOT(updateMachineType()));
    connect(m_pExportSetDialog, SIGNAL(exportTableList(const QStringList&)),
            this, SLOT(updateExportList(const QStringList&)));
    connect(m_pExportSetDialog, SIGNAL(exportTableName(const QStringList&)),
            this, SLOT(updateExportTableName(const QStringList&)));
//
    m_pTableModel = new QSqlTableModel(this);
}

void MainWindow::initTableName()
{
    on_dockWidget_typeChanged(m_pWidget->currentTypeName());
}

void MainWindow::initMenuAndTool()
{
    ui->menuBar->setMinimumHeight(25);
    QString qmlMenuBar = "QMenuBar{background-color:#003778;\
                                   color:white;\
                                   font-family:仿宋;\
                                   font-size: 18px}\
                          QMenuBar:item:selected{background-color:#436EEE;\
                                                 border-radius: 5px;}";
    ui->menuBar->setStyleSheet(qmlMenuBar);

    QString qmlMenu = "QMenu{background-color:#003778;\
                             color:white;}\
                       QMenu:item:selected{color:#969696;}";

    QMenu *optionMenu = ui->menuBar->addMenu(tr("Option"));
    QAction *typeAction = new QAction(QIcon(":/images/type.png"), tr("Type"), this);
    QList<QAction*> optionMenuList;
    optionMenuList << typeAction;
    optionMenu->addActions(optionMenuList);

    optionMenu->setStyleSheet(qmlMenu);

    QMenu *toolMenu = ui->menuBar->addMenu(tr("Tools"));
    QAction *queryAction = new QAction(QIcon(":/images/query.png"), tr("Query"), this);
    QAction *displayAllAction = new QAction(QIcon(":/images/displayAll.png"), tr("DisplayAll"), this);
    QAction *submitAction = new QAction(QIcon(":/images/submit.png"), tr("Submit"), this);
    QAction *recoverAction = new QAction(QIcon(":/images/recover.png"), tr("Recover"), this);
    QAction *addAction = new QAction(QIcon(":/images/add.png"), tr("Add"), this);
    QAction *deleteAction = new QAction(QIcon(":/images/delete.png"), tr("Delete"), this);
    QAction *ascAction = new QAction(QIcon(":/images/up.png"), tr("Ascending"), this);
    QAction *descAction = new QAction(QIcon(":/images/down.png"), tr("Descending"), this);
    QList<QAction*> toolMenuList;
    toolMenuList << queryAction << displayAllAction << submitAction << recoverAction
                 << addAction << deleteAction << ascAction << descAction;
    toolMenu->addActions(toolMenuList);
    toolMenu->setStyleSheet(qmlMenu);

    QMenu *exportMenu = ui->menuBar->addMenu(tr("Export "));
    QAction *csvAction = new QAction(QIcon(":/images/text.png"), tr("Export CSV"), this);
    QAction *pdfAction = new QAction(QIcon(":/images/pdf.png"), tr("Export PDF"), this);
    QAction *exportSetAction = new QAction(QIcon(":/images/addlist.png"), tr("Export Config"), this);
    QList<QAction*> exportnenuList;
    exportnenuList << csvAction << pdfAction << exportSetAction;
    exportMenu->addActions(exportnenuList);
    exportMenu->setStyleSheet(qmlMenu);

    ui->mainToolBar->addActions(optionMenuList);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addActions(toolMenuList);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addActions(exportnenuList);

    connect(typeAction, SIGNAL(triggered()), this, SLOT(on_typeAction_triggered()));
    connect(queryAction, SIGNAL(triggered()), this, SLOT(query()));
    connect(displayAllAction, SIGNAL(triggered()), this, SLOT(displayAll()));
    connect(submitAction, SIGNAL(triggered()), this, SLOT(submit()));
    connect(recoverAction, SIGNAL(triggered()), this, SLOT(recover()));
    connect(addAction, SIGNAL(triggered()), this, SLOT(add()));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteRow()));
    connect(ascAction, SIGNAL(triggered()), this, SLOT(ascendingSort()));
    connect(descAction, SIGNAL(triggered()), this, SLOT(descendingSort()));

    connect(csvAction, SIGNAL(triggered()), this, SLOT(on_csvAction_triggered()));
    connect(pdfAction, SIGNAL(triggered()), this, SLOT(on_pdfAction_triggered()));
    connect(exportSetAction, SIGNAL(triggered()), this, SLOT(on_exportSetAction_triggered()));
}

void MainWindow::setFixViewWidth()
{
    m_pTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    m_pTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    m_pTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    m_pTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    m_pTableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive);
    m_pTableView->setColumnWidth(0, 90);
    m_pTableView->setColumnWidth(1, 180);
    m_pTableView->setColumnWidth(2, 180);
    m_pTableView->setColumnWidth(3, 150);
    m_pTableView->setColumnWidth(4, 160);

    for(int i = 0; i < m_pTableModel->rowCount(); i++)
    {
         m_pTableView->verticalHeader()->setSectionResizeMode(i, QHeaderView::Interactive);
         m_pTableView->setRowHeight(i, 60);
    }
}

void MainWindow::initTabelHeaderData(QSqlTableModel *tableModel)
{
    tableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    tableModel->setHeaderData(1, Qt::Horizontal, tr("Version"));
    tableModel->setHeaderData(2, Qt::Horizontal, tr("Caption"));
    tableModel->setHeaderData(3, Qt::Horizontal, tr("Board"));
    tableModel->setHeaderData(4, Qt::Horizontal, tr("Time"));
}

