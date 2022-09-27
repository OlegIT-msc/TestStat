#include "nodetree.h"
#include <QMap>
#include <QFileInfo>


////////////////////////////////////////////////
NodeData::NodeData()
{
    SuffixFile.clear();
    Name = QString();
    Size = 0;
    CountFile = 0;
    CountSubDir = 0;
    isFile = true;
}

NodeData::~NodeData()
{
}

int NodeData::GetSizeSuffixFile(void)
{
    return SuffixFile.size();
}

void NodeData::AddSizeTotal(int I,qint64 SizeTotal)
{
    if (I >= 0 && I < SuffixFile.size())
        SuffixFile[I].SizeTotal += SizeTotal;
}

qint64 NodeData::GetSizeTotal(int I)
{
    if (I >= 0 && I < SuffixFile.size())
        return SuffixFile[I].SizeTotal;
    return -1;
}

void NodeData::AddSizeMidle(int I,qint64 SizeMidle)
{
    if (I >= 0 && I < SuffixFile.size())
    {
        SuffixFile[I].SizeMidle += SizeMidle;
        SuffixFile[I].SizeMidle /= 2;
    }
}

qint64 NodeData::GetSizeMidle(int I)
{
    if (I >= 0 && I < SuffixFile.size())
        return SuffixFile[I].SizeMidle;
    return -1;
}

void NodeData::SetSFile(int I,QString SFile)
{
    if (I >= 0 && I < SuffixFile.size())
    {
        SuffixFile[I].SFile = SFile;
    }
}

QString NodeData::GetSFile(int I)
{
    if (I >= 0 && I < SuffixFile.size())
        return SuffixFile[I].SFile;
    return QString();
}

void NodeData::AddGroupFile(GroupFile Gf)
{
    SuffixFile << Gf;
}

void NodeData::ClearSuffixFile(void)
{
    SuffixFile.clear();
}

void NodeData::SetName(QString S)
{
    Name = S;
}

QString NodeData::GetName(void)
{
    return Name;
}

void NodeData::SetSize(int V)
{
    Size = V;
}

int NodeData::GetSize(void)
{
    return Size;
}

void NodeData::SetCountFile(int V)
{
    CountFile = V;
}

int NodeData::GetCountFile(void)
{
    return CountFile;
}

void NodeData::SetCountSubDir(int V)
{
    CountSubDir = V;
}

int NodeData::GetCountSubDir(void)
{
    return CountSubDir;
}

void NodeData::SetIsFile(bool B)
{
    isFile = B;
}

bool NodeData::GetIsFile(void)
{
    return isFile;
}


////////////////////////////////////////////////

NodeTree::NodeTree(NodeTree *parent)
{
    parentItem = parent;
}

NodeTree *NodeTree::child(int number)
{
    return childItems.value(number);
}

int NodeTree::childCount() const
{
    return childItems.count();
}

int NodeTree::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<NodeTree*>(this));

    return 0;
}

int NodeTree::columnCount() const
{
    return 1;
}

QVariant NodeTree::data(int Column) const
{
    QString S;
    switch(Column)
    {
    case 0:
        S = QFileInfo(Name).fileName();
        break;
    }

    return QVariant(S);
}

NodeTree *NodeTree::insertChildren(int position)
{
    if (position < 0 || position > childItems.size())
        return nullptr;

    NodeTree *item = new NodeTree(this);
    childItems.insert(position, item);

    return item;
}

NodeTree *NodeTree::insertChildren(NodeTree *NewChildren,int position)
{
    if (position < 0 || position > childItems.size())
        return nullptr;

    childItems.insert(position, NewChildren);

    return NewChildren;
}

NodeTree *NodeTree::parent()
{
    return parentItem;
}

bool NodeTree::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}
