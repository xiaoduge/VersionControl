#include "ddockwidget.h"
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QFrame>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSqlQuery>
#include <QDebug>


DDockWidget::DDockWidget(bool type, QWidget *parent) : QFrame(parent)
{
    m_type = type;
    initUI();
    buildTranslation();
}

const QString DDockWidget::currentTypeName()
{
    return m_pComboType->currentText();
}

void DDockWidget::updateMachineType()
{
    if(m_type)
    {
        QSqlQuery query;
        QStringList comboList;
        QString strQuery = QString("select machine_Type from MACHINETYPE");
        query.exec(strQuery);
        while (query.next())
        {
                QString str = query.value(0).toString();
                comboList << str;
                if(!str.isEmpty())
                {
                    QString CREATE_TABLE = QString("CREATE TABLE IF NOT EXISTS  %1 (version_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                                   "version_Num VARCHAR(24) NOT NULL,"
                                                                                   "version_Caption VARCHAR(1024) NOT NULL,"
                                                                                   "version_Board VARCHAR(24) NOT NULL,"
                                                                                   "version_Time VARCHAR(20) NOT NULL)")
                                                                                    .arg(str);
                    QSqlQuery query;
                    bool success = query.exec(CREATE_TABLE);
                    if(success)
                        qDebug()<< QObject::tr("create table: %1 successed").arg(str);
                    else
                        qDebug()<< QObject::tr("create table: %1 failed").arg(str);
                }
        }

        m_pComboType->clear();
        m_pComboType->addItems(comboList);
    }
}

void DDockWidget::on_queryBtn_clicked()
{
    if(m_type)
    {
        emit queryFiterChanged(m_pLinedEdit->text());
    }
    emit btn_clicked(BTN_QUERY_CLICKED);
}

void DDockWidget::on_displayAllBtn_clicked()
{
    emit btn_clicked(BTN_DISPALYALL_CLICKED);
}
void DDockWidget::on_submitBtn_clicked()
{
    emit btn_clicked(BTN_SUBMIT_CLICKED);
}
void DDockWidget::on_recoverBtn_clicked()
{
    emit btn_clicked(BTN_RECOVER_CLICKED);
}
void DDockWidget::on_addBtn_clicked()
{
    emit btn_clicked(BTN_ADD_CLICKED);
}
void DDockWidget::on_deleteBtn_clicked()
{
    emit btn_clicked(BTN_DELETE_CLICKED);
}

void DDockWidget::on_AscendingBtn_clicked()
{
    emit btn_clicked(BTN_ASC_CLICKED);
}

void DDockWidget::on_DescendingBtn_clicked()
{
    emit btn_clicked(BTN_DES_CLICKED);
}

void DDockWidget::on_comboBoxType_currentTextChanged(const QString &text)
{
    emit type_Changed(text);
}

void DDockWidget::initUI()
{
    this->setFrameStyle(QFrame::Box | QFrame::Raised);
    QVBoxLayout *v1Layout = new QVBoxLayout;

    if(m_type)
    {
        m_pLinedEdit = new QLineEdit;
        m_pLinedEdit->setMaximumWidth(150);
        m_pComboType = new QComboBox;
        m_pComboType->setMaximumWidth(150);

        for(int i = 0; i < LAB_NUM; i++)
        {
            m_pLab[i] = new QLabel;
            m_pLab[i]->setMaximumWidth(120);
        }

        QGridLayout *gLayout = new QGridLayout;
        gLayout->setAlignment(Qt::AlignLeft);
        gLayout->addWidget(m_pLab[LAB_TYPE], 0, 0, 1, 1);
        gLayout->addWidget(m_pComboType, 0, 1, 1, 1);

        gLayout->addWidget(m_pLab[LAB_SELECT], 1, 0, 1, 1);
        gLayout->addWidget(m_pLinedEdit, 1, 1, 1, 1);
        v1Layout->addLayout(gLayout);

        connect(m_pComboType, SIGNAL(currentTextChanged(const QString&)),
                this, SLOT(on_comboBoxType_currentTextChanged(const QString&)));
    }

    for(int i = 0; i < BTN_NUM; i++)
    {
        m_pBtn[i] = new QPushButton;
        m_pBtn[i]->setMaximumWidth(120);
        v1Layout->addWidget(m_pBtn[i]);
    }
    if(!m_type)
    {
        m_pBtn[BTN_QUERY]->hide();
    }
    QSpacerItem *spacerItem = new QSpacerItem(20, 50, QSizePolicy::Expanding);
    v1Layout->addSpacerItem(spacerItem);

    this->setLayout(v1Layout);

    connect(m_pBtn[BTN_QUERY], SIGNAL(clicked()), this, SLOT(on_queryBtn_clicked()));
    connect(m_pBtn[BTN_DISPALYALL], SIGNAL(clicked()), this, SLOT(on_displayAllBtn_clicked()));
    connect( m_pBtn[BTN_SUBMIT], SIGNAL(clicked()), this, SLOT(on_submitBtn_clicked()));
    connect(m_pBtn[BTN_RECOVER], SIGNAL(clicked()), this, SLOT(on_recoverBtn_clicked()));
    connect(m_pBtn[BTN_ADD], SIGNAL(clicked()), this, SLOT(on_addBtn_clicked()));
    connect(m_pBtn[BTN_DELETE], SIGNAL(clicked()), this, SLOT(on_deleteBtn_clicked()));
    connect(m_pBtn[BTN_ASC], SIGNAL(clicked()), this, SLOT(on_AscendingBtn_clicked()));
    connect(m_pBtn[BTN_DES], SIGNAL(clicked()), this, SLOT(on_DescendingBtn_clicked()));

    updateMachineType();

    QString qssFrame = "QLabel{color: white;}";
    this->setStyleSheet(qssFrame);
}

void DDockWidget::buildTranslation()
{
    if(m_type)
    {
        m_pLab[LAB_TYPE]->setText(tr("Type"));
        m_pLab[LAB_SELECT]->setText(tr("Select"));
    }

    m_pBtn[BTN_QUERY]->setText(tr("Query"));
    m_pBtn[BTN_DISPALYALL]->setText(tr("DisplayAll"));
    m_pBtn[BTN_SUBMIT]->setText(tr("Submit"));
    m_pBtn[BTN_RECOVER]->setText(tr("Recver"));
    m_pBtn[BTN_ADD]->setText(tr("Add"));
    m_pBtn[BTN_DELETE]->setText(tr("Delete"));
    m_pBtn[BTN_ASC]->setText(tr("Ascending"));
    m_pBtn[BTN_DES]->setText(tr("Descending"));
}
