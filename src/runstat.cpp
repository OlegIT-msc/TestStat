#include <QApplication>
#include <QDebug>

#include "runstat.h"

int StatEvent::NumEvent = 0;

RunStat::RunStat(QObject *parent) :
    QThread(parent),
    Receiver(parent)
{
}

RunStat::~RunStat()
{
}

void RunStat::SetDir(QString  S)
{
    Dir = S;
}

void RunStat::run()
{
    if (Dir.isEmpty() == false)
    {
        StartIterationInfo(Dir);
    }
}

void RunStat::StartIterationInfo(QString Path_)
{
    int i;
    CurPath = Path_;
    setPath(Path_);
    CurListFile = entryInfoList(QStringList(),QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList LListDirs = CurListDirs = entryInfoList(QStringList(),QDir::Dirs | QDir::NoDotAndDotDot);

    lock();
    emit SendDirsFiles();
    while(tryLock() == true) {}

    for (i = 0; i < LListDirs.count(); ++i)
    {
        lock();
        emit SendNextDir();
        while(tryLock() == true) {}
        StartIterationInfo(Path_+QString(separator())+LListDirs.at(i).fileName());
    }
    lock();
    emit SendPrevDir();
    while(tryLock() == true) {}
}
