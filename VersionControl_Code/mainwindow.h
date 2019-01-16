#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QTableView;
class QFrame;
class DDockWidget;
class DTypeDialog;
class ExportSetDialog;
class QSqlTableModel;
class QTranslator;
class QPrinter;

class MainWindow : public QMainWindow
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
        BTN_ASC_CLICKED,
        BTN_DES_CLICKED,
        BTN_CLICKED_NUM
    };
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:


protected slots:
    void on_dockWidgetBtn_clicked(int id);
    void on_dockWidget_typeChanged(const QString &typeName);
    void on_dockWidget_queryFiterChanged(const QString &text);

    void on_typeAction_triggered();
    void on_csvAction_triggered();
    void on_pdfAction_triggered();
    void on_exportSetAction_triggered();

    void query();
    void displayAll();
    void submit();
    void recover();
    void add();
    void deleteRow();
    void ascendingSort();
    void descendingSort();

    void updateExportList(const QStringList& strList);
    void updateExportTableName(const QStringList& strList);

private:
    const QString getHtml();
    const QString getMulHtml();
    void initTranslation();
    void buildTranslation();

    void initDockWidget();
    void initUI();

    void initTableName();

    void initMenuAndTool();
    void setFixViewWidth();
    void initTabelHeaderData(QSqlTableModel *tableModel);

private:
    Ui::MainWindow *ui;

    DDockWidget *m_pWidget;
    QDockWidget *m_pDockWidget;

    QTableView *m_pTableView;
    QSqlTableModel *m_pTableModel;

    QString m_tableName;
    QString m_versionName;
    QString m_boardVersion;
    QStringList m_exportTableList;
    QStringList m_exportTableName;

    DTypeDialog *m_pTypeDialog;
    ExportSetDialog *m_pExportSetDialog;

    QPrinter *m_pPrinter;

};

#endif // MAINWINDOW_H
