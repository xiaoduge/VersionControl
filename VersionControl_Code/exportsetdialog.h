#ifndef EXPORTSETDIALOG_H
#define EXPORTSETDIALOG_H

#include <QDialog>

namespace Ui {
class ExportSetDialog;
}

class ExportSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportSetDialog(QWidget *parent = 0);
    ~ExportSetDialog();

public slots:
    void updateMachineType();

private slots:
    void on_pushButton_Add_clicked();

    void on_pushButton_Sub_clicked();

    void on_pushButton_OK_clicked();

signals:
    void exportTableList(const QStringList& strList);
    void exportTableName(const QStringList& strList);

private:
    void buildTranslation();

private:
    Ui::ExportSetDialog *ui;

    QStringList m_targetList;
    QStringList m_targetNameList;
};

#endif // EXPORTSETDIALOG_H
