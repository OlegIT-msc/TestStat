#include <QFileDialog>
#include <QDebug>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    ContCountFile->setVisible(false);
    ContCountSubDir->setVisible(false);

    QStringList ListHeaders;
    ListHeaders << QString("Имя");
    TreeModel = new ObjectTreeModel(ListHeaders,this);
    RootItem = TreeModel->getRootItem();
    ViewFS->setModel(TreeModel);

    SelectDir->setText(tr("Выбор папки"));

    StatDir = new RunStat(this);
    connect(StatDir, SIGNAL(finished()),
            this, SLOT(EndOfThread()));
    connect(StatDir, SIGNAL(SendDirsFiles()),
            this, SLOT(SendDirsFiles()));
    connect(StatDir, SIGNAL(SendNextDir()),
            this, SLOT(SendNextDir()));
    connect(StatDir, SIGNAL(SendPrevDir()),
            this, SLOT(SendPrevDir()));
}

MainWindow::~MainWindow()
{
    StatDir->exit();
    while(StatDir->isRunning() == true) {}
    delete StatDir;
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    ViewFS->resize(size().width()-ViewFS->pos().x()-10,
                   size().height()-ViewFS->pos().y()-Cont->size().height());

    Cont->resize(size().width()-Cont->pos().x()-10,Cont->size().height());
    Cont->move(Cont->x(),size().height()-Cont->size().height()-5);
    ViewData->resize(Cont->size().width()-ViewData->pos().x()-10,
                     Cont->size().height()-20);
}

void MainWindow::EndOfThread(void)
{
    SelectDir->setEnabled(true);
}

void MainWindow::SendDirsFiles(void)
{
    int i,j;
    bool AddSuffix;
    GroupFile GFile;
    NodeTree *Item;
    QFileInfoList ListF = StatDir->GetCurListFile();
    QFileInfoList ListD = StatDir->GetCurListDirs();

//    qDebug() << "Dirs  " << CurPath;
//    for (i = 0; i < ListD.count(); ++i)
//    {
//        qDebug() << ListD[i].fileName();
//    }
//    qDebug() << "///////////////";
//    qDebug() << "Files  " << CurPath;
//    for (i = 0; i < ListF.count(); ++i)
//    {
//        qDebug() << ListF[i].fileName();
//    }
//    qDebug() << "///////////////\n\n";

    for (i = 0; i < ListD.count(); ++i)
    {
        Item = new NodeTree(CurItem);
        Item = CurItem->insertChildren(Item,CurItem->childCount());
        Item->SetName(ListD[i].filePath());
        Item->SetSize(ListD[i].size());
        Item->SetIsFile(false);
    }
    for (i = 0; i < ListF.count(); ++i)
    {
        Item = new NodeTree(CurItem);
        Item = CurItem->insertChildren(Item,CurItem->childCount());
        Item->ClearSuffixFile();
        Item->SetName(ListF[i].filePath());
        Item->SetSize(ListF[i].size());
        Item->SetIsFile(true);

        AddSuffix = true;
        for (j = 0; j < CurItem->GetSizeSuffixFile(); ++j)
        {
            if (CurItem->GetSFile(j) == ListF.at(i).suffix())
            {
                CurItem->AddSizeTotal(j,ListF.at(i).size());
                CurItem->AddSizeMidle(j,ListF.at(i).size());
                AddSuffix = false;
                break;
            }
        }
        if (AddSuffix == true)
        {
            GFile.SFile = ListF.at(i).suffix();
            GFile.SizeMidle = ListF.at(i).size();
            GFile.SizeTotal = ListF.at(i).size();
            CurItem->AddGroupFile(GFile);
        }
    }

    ViewFS->reset();
    ViewFS->setExpanded(TreeModel->index(0,0,QModelIndex()),true);

    StatDir->unlock();
}

void MainWindow::SendNextDir(void)
{
    QString CurPath = QFileInfo(StatDir->GetCurPath()).fileName();

    for (int i = 0; i < CurItem->childCount(); ++i)
    {
        if (CurPath == CurItem->child(i)->data(0))
        {
            CurItem = CurItem->child(i);
            break;
        }
    }

    StatDir->unlock();
}

void MainWindow::SendPrevDir(void)
{
    int Size = 0;
    int CountFile = 0;
    int CountSubDir = 0;
    for (int i = 0; i < CurItem->childCount(); ++i)
    {
        Size += CurItem->child(i)->GetSize();
        if (CurItem->child(i)->GetIsFile() == true)
            CountFile++;
        else
            CountSubDir++;
    }
    CurItem->SetSize(Size);
    CurItem->SetCountFile(CountFile);
    CurItem->SetCountSubDir(CountSubDir);
    if (CurItem != nullptr)
        CurItem = CurItem->parent();

    StatDir->unlock();
}

void MainWindow::on_SelectDir_clicked()
{
//    QString Dir = QString("d:/Work");
    QString Dir = QFileDialog::getExistingDirectory(this,"Выбор папки");
    if (Dir.isEmpty() == false)
    {
        SelectDir->setEnabled(false);
        TreeModel->removeRows(0,TreeModel->rowCount(QModelIndex()),QModelIndex());
        CurItem = RootItem->insertChildren(RootItem->childCount());
        CurItem->SetName(Dir);

        StatDir->SetDir(Dir);
        StatDir->start();
    }
}

void MainWindow::on_ViewFS_clicked(const QModelIndex &index)
{
    QString S;
    NodeTree *Item = TreeModel->getItem(index);
    Name->setText(Item->GetName());
    Size->setText(QString().setNum(Item->GetSize()));

    ContCountFile->setVisible(Item->GetCountFile() > 0);
    CountFile->setText(QString().setNum(Item->GetCountFile()));
    ContCountSubDir->setVisible(Item->GetCountSubDir() > 0);
    CountSubDir->setText(QString().setNum(Item->GetCountSubDir()));

    ViewData->clear();
    for (int i = 0; i < Item->GetSizeSuffixFile(); i++)
    {
        S = QString("Расширение '")+Item->GetSFile(i)+QString("' - Средний размер ")+
                QString().setNum(Item->GetSizeMidle(i));
        ViewData->addItem(S);
    }
}
