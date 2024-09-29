#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaEnum>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QSaveFile>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionopenFile,&QAction::triggered,this,&MainWindow::onopenfile);
    connect(ui->actionsaveAsFile,&QAction::triggered,this,&MainWindow::onsaveasfile);
    connect(ui->actionsaveFile,&QAction::triggered,this,&MainWindow::onsavefile);
    connect(ui->actionnewFile,&QAction::triggered,this,&MainWindow::onnewfile);
    connect(ui->actionQuit,&QAction::triggered,[=](){
        QApplication::quit();
    });
    connect(ui->actioncopy,&QAction::triggered,[=](){
        ui->textEdit->copy();
    });
    connect(ui->actioncut,&QAction::triggered,[=](){
        ui->textEdit->cut();
    });
    connect(ui->actionundo,&QAction::triggered,[=](){
        ui->textEdit->undo();
    });
    connect(ui->actionpaste,&QAction::triggered,[=](){
        ui->textEdit->paste();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onopenfile()
{
    if(maybesave()){
        QString filename = QFileDialog::getOpenFileName(this);
        if(!filename.isEmpty()){
            loadFile(filename);
        }
    }
}

bool MainWindow::onsavefile()
{
    if(cur.isEmpty()){
        return onsaveasfile();
    }
    else
        return saveFile(cur);
}

bool MainWindow::onsaveasfile()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted) {
        // 用户没有点击保存或取消，这里通常不需要输出调试信息，因为用户可能只是关闭了对话框
        return false;
    }
    else {
        // 用户点击了保存
        qDebug() << "File saved to:" << dialog.selectedFiles().first();
        return saveFile(dialog.selectedFiles().first());
    }
}

void MainWindow::onnewfile()
{
    if(maybesave()){
        ui->textEdit->clear();
        cur=QString();
        ui->textEdit->document()->setModified(false);
        statusBar()->showMessage("新建文件成功",2000);
    }
}

bool MainWindow::maybesave()
{
    if(!ui->textEdit->document()->isModified()) return true;

    const QMessageBox::StandardButton btn = QMessageBox::warning(this,qApp->applicationName(),"文件未保存",QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
    switch (btn) {
        case QMessageBox::Save:
            return onsavefile();
            break;
        case QMessageBox::Cancel:
            return false;
    }
    return true;
}

void MainWindow::loadFile(QString str)
{
    QFile file(str);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,qApp->applicationDisplayName(),"无法打开文件");
        return;
    }

    QTextStream in(&file);
    ui->textEdit->setPlainText(in.readAll());
    cur=str;
    ui->textEdit->document()->setModified(false);
    statusBar()->showMessage("文件打开成功",2000);
}

bool MainWindow::saveFile(QString path)
{
    QSaveFile file(path);
    if(file.open(QFile::WriteOnly|QFile::Text)){
        QTextStream out(&file);
        out<<ui->textEdit->toPlainText();
        if(!file.commit()){
            QMessageBox::warning(this,qApp->applicationName(),"文件保存失败");
            return false;
        }
    }
    else return false;
    cur=path;
    ui->textEdit->document()->setModified(false);
    statusBar()->showMessage("文件保存成功",2000);
    return true;
}
