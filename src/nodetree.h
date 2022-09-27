#ifndef __NODE_TREE_H__
#define __NODE_TREE_H__

#include <QString>
#include <QList>
#include <QVariant>


struct GroupFile {
    qint64 SizeTotal;
    qint64 SizeMidle;
    QString SFile;
    GroupFile ()
    {
        SizeTotal = SizeMidle = 0;
        SFile = QString();
    }
};

class NodeData
{
public:
    NodeData();
    virtual ~NodeData();

    int GetSizeSuffixFile(void);
    void AddSizeTotal(int,qint64);
    qint64 GetSizeTotal(int);
    void AddSizeMidle(int,qint64);
    qint64 GetSizeMidle(int);
    void SetSFile(int,QString);
    QString GetSFile(int);
    void AddGroupFile(GroupFile);
    void ClearSuffixFile(void);

    void SetName(QString);
    QString GetName(void);

    void SetSize(int);
    int GetSize(void);

    void SetCountFile(int);
    int GetCountFile(void);

    void SetCountSubDir(int);
    int GetCountSubDir(void);

    void SetIsFile(bool);
    bool GetIsFile(void);

protected:
    QList<GroupFile> SuffixFile;
    QString Name;
    int Size;
    int CountFile;
    int CountSubDir;
    bool isFile;
};

class NodeTree : public NodeData
{
public:
    NodeTree(NodeTree *parent = nullptr);
    NodeTree(const NodeTree *Nt,NodeTree *parent = nullptr);
    virtual ~NodeTree() {}

private:
    NodeTree            *parentItem;
    QList<NodeTree*>    childItems;

public:
    NodeTree *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int) const;
    NodeTree *insertChildren(int position);
    NodeTree *insertChildren(NodeTree *NewChildren,int position);
    NodeTree *parent();
    bool removeChildren(int position, int count);
    int childNumber() const;
};

#endif // __NODE_TREE_H__
