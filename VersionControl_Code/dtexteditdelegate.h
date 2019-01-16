#ifndef DTEXTEDITDELEGATE_H
#define DTEXTEDITDELEGATE_H

#include <QItemDelegate>

class DTextEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    DTextEditDelegate(QObject *parent = Q_NULLPTR);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // DTEXTEDITDELEGATE_H
