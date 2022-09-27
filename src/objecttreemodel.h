#ifndef __OBJECT_TREE_H__
#define __OBJECT_TREE_H__

#include <QtGui>
#include "nodetree.h"

class ObjectTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ObjectTreeModel(const QStringList&, QObject* = nullptr);
    ~ObjectTreeModel();

public:
    QModelIndex index(int, int, const QModelIndex&) const;
    QModelIndex parent(const QModelIndex&) const;

    int rowCount(const QModelIndex&) const;
    int columnCount(const QModelIndex&) const;
    QVariant data(const QModelIndex&, int) const;
    QVariant headerData(int, Qt::Orientation,int) const;

    Qt::ItemFlags flags(const QModelIndex&) const;
    bool insertRows(int, int, const QModelIndex& = QModelIndex());
    bool removeRows(int, int,const QModelIndex& = QModelIndex());

    NodeTree *getItem(const QModelIndex&) const;
    NodeTree *getRootItem(void) const {return rootItem;}

    QModelIndex CreateIndexModel(int, int, void* = nullptr) const;

private:

    NodeTree *rootItem;
};

#endif // __OBJECT_TREE_H__
