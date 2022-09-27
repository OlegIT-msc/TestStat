#include "objecttreemodel.h"

ObjectTreeModel::ObjectTreeModel(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new NodeTree();
    rootItem->SetName(headers.at(0));
}

ObjectTreeModel::~ObjectTreeModel()
{
    delete rootItem;
}

int ObjectTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}

QVariant ObjectTreeModel::data(const QModelIndex &index, int role) const
{
    QString StrRet;
    if (!index.isValid())
        return QVariant();

    NodeTree *item = getItem(index);
    if (item == nullptr)
        return QVariant();

    switch (role)
    {
    case Qt::FontRole:
        break;
    case Qt::DisplayRole:
        StrRet = item->data(index.column()).toString();
        return QVariant(StrRet);
    case Qt::EditRole:
        break;
    }
    return QVariant();
}

////////////////////////////////////////


Qt::ItemFlags ObjectTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags Iflags = Qt::NoItemFlags;
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Iflags;
}

NodeTree *ObjectTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        NodeTree *item = static_cast<NodeTree*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}

QVariant ObjectTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    return QVariant();
}

QModelIndex ObjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    NodeTree *parentItem = getItem(parent);

    NodeTree *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool ObjectTreeModel::insertRows(int position, int, const QModelIndex &parent)
{
    NodeTree *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position - 1);
    success = parentItem->insertChildren(position);
    endInsertRows();

    return success;
}

QModelIndex ObjectTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    NodeTree *childItem = getItem(index);
    NodeTree *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool ObjectTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    if (rows == 0)
        return false;
    NodeTree *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int ObjectTreeModel::rowCount(const QModelIndex &parent) const
{
    NodeTree *parentItem = getItem(parent);

    return parentItem->childCount();
}

QModelIndex ObjectTreeModel::CreateIndexModel(int row, int column, void *data) const
{
    return createIndex(row, column, data);
}
