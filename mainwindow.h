#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void onopenfile();
    bool onsavefile();
    bool onsaveasfile();
    void onnewfile();





private:
    bool maybesave();
    void loadFile(QString str);
    bool saveFile(QString path);
    QString cur;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
