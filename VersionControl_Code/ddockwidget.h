#ifndef DDOCKWIDGET_H
#define DDOCKWIDGET_H

#include <QFrame>

class QPushButton;
class QLabel;
class QComboBox;
class QLineEdit;
class QFrame;

class DDockWidget : public QFrame
{
    Q_OBJECT
public:
    enum BTN_DOCK
    {
        BTN_QUERY,
        BTN_DISPALYALL,
        BTN_SUBMIT,
        BTN_RECOVER,
        BTN_ADD,
        BTN_DELETE,
        BTN_ASC, //Ascending
        BTN_DES, //Descending
        BTN_NUM
    };
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
    enum LABELE_DOCK
    {
        LAB_TYPE,
        LAB_SELECT,
        LAB_NUM
    };
public:
    explicit DDockWidget(bool type = true, QWidget *parent = Q_NULLPTR);

    const QString currentTypeName();

public slots:
    void updateMachineType();

signals:
    void btn_clicked(int Id);
    void queryFiterChanged(const QString &text);
    void type_Changed(const QString &text);

protected slots:
    void on_queryBtn_clicked();
    void on_displayAllBtn_clicked();
    void on_submitBtn_clicked();
    void on_recoverBtn_clicked();
    void on_addBtn_clicked();
    void on_deleteBtn_clicked();
    void on_AscendingBtn_clicked();
    void on_DescendingBtn_clicked();

    void on_comboBoxType_currentTextChanged(const QString &text);
private:
    void initUI();
    void buildTranslation();

private:
    bool m_type;

    QPushButton *m_pBtn[BTN_NUM];
    QLabel *m_pLab[LAB_NUM];
    QLineEdit *m_pLinedEdit;
    QComboBox *m_pComboType;
};

#endif // DDOCKWIDGET_H
