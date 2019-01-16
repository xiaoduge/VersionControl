#ifndef DTYPEDIALOG_H
#define DTYPEDIALOG_H

#include <QDialog>

class QTableView;
class DDockWidget;
class QSqlTableModel;

class DTypeDialog : public QDialog
{
    Q_OBJECT

public:
    enum BTN_CLICKED_SIGNAL //clicked
    {
        BTN_QUERY_CLICKED = 0,
        BTN_DISPALYALL_CLICKED,
        BTN_SUBMIT_CLICKED,
        BTN_RECOVER_CLICKED,
        BTN_ADD_CLICKED,
        BTN_DELETE_CLICKED,
        BTN_CLICKED_NUM
    };

public:
    DTypeDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

signals:
    void typeListChanged();

protected slots:
    void on_dockWidgetBtn_clicked(int id);

private:
    void initUI();

    void query();
    void displayAll();
    void submit();
    void recover();
    void add();
    void deleteRow();

    void setFixView();
    void initTabelHeaderData(QSqlTableModel *tableModel);

private:
    DDockWidget *m_pDockWidget;
    QTableView *m_pTableView;

    QSqlTableModel *m_pTableModel;

    QStringList m_addList;
};

#endif // DTYPEDIALOG_H
