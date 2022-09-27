#ifndef RUNSTAT_H
#define RUNSTAT_H

#include <QThread>
#include <QFileInfoList>
#include <QMutex>
#include <QEvent>
#include <QList>
#include <QDir>

enum {
    StatEventType = QEvent::User + 1
};
enum Event_User {
    DirsFiles,
    NextDir,
    PrevDir
};

class StatEvent : public QEvent
{
private:
    int ID;
    static int NumEvent;

public:

    StatEvent() : QEvent((Type)StatEventType)
    {
        ID = -1;
    }

    void SetID(int V)
    {
        ID = V;
        NumEvent++;
    }

    int GetID() const
    {
        return ID;
    }

    int GetNumEvent() const
    {
        return NumEvent;
    }
};

class RunStat : public QThread, public QDir, public QMutex
{
    Q_OBJECT
public:
    explicit RunStat(QObject *parent = nullptr);
    virtual  ~RunStat();

protected:
    QObject *Receiver;
    QFileInfoList CurListFile;
    QFileInfoList CurListDirs;
    QString CurPath;
    QString Dir;

protected:
    void StartIterationInfo(QString);
    void run() Q_DECL_OVERRIDE;

public:
    void SetDir(QString);
    QFileInfoList GetCurListFile(void) {return CurListFile;}
    QFileInfoList GetCurListDirs(void) {return CurListDirs;}
    QString GetCurPath(void) {return CurPath;}

signals:
    void SendDirsFiles(void);
    void SendNextDir(void);
    void SendPrevDir(void);
};

#endif // RUNSTAT_H
