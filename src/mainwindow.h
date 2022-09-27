#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "runstat.h"
#include "ui_mainwindow.h"
#include "objecttreemodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent*) override;

private:
    RunStat *StatDir;

    ObjectTreeModel *TreeModel;
    NodeTree *RootItem;
    NodeTree *CurItem;

public slots:
    void EndOfThread(void);
    void SendDirsFiles(void);
    void SendNextDir(void);
    void SendPrevDir(void);

private slots:
    void on_SelectDir_clicked();
    void on_ViewFS_clicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
