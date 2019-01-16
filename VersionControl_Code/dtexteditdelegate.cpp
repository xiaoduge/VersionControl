#include "dtexteditdelegate.h"
#include <QTextEdit>
#include <QSpinBox>
#include <QLineEdit>

DTextEditDelegate::DTextEditDelegate(QObject *parent):QItemDelegate(parent)
{

}

QWidget *DTextEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    if(0 == index.column())
    {
        QSpinBox *spinBox = new QSpinBox(parent);
        return spinBox;
    }
    else if(2 == index.column())
    {
        QTextEdit *textEdit = new QTextEdit(parent);
        return textEdit;
    }
    else
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        return lineEdit;
    }

}

void DTextEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(0 == index.column())
    {
        int iValue = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(iValue);
    }
    else if(2 == index.column())
    {
        QString strValue = index.model()->data(index, Qt::EditRole).toString();
        QTextEdit *textEdit = static_cast<QTextEdit*>(editor);
        textEdit->setText(strValue);
    }
    else
    {
        QString strValue = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(strValue);
    }

}

void DTextEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(0 == index.column())
    {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int iValue = spinBox->value();
        model->setData(index, iValue, Qt::EditRole);
    }
    else if (2 == index.column())
    {
        QTextEdit *textEdit = static_cast<QTextEdit*>(editor);
        QString strValue = textEdit->toPlainText();
        model->setData(index, strValue, Qt::EditRole);
    }
    else
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString strValue = lineEdit->text();
        model->setData(index, strValue, Qt::EditRole);
    }

}

void DTextEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
