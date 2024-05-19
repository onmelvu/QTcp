#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connect_clicked();

    void on_selectFile_clicked();

    void on_sendFile_clicked();

signals:
    void startConnect(QString ip,unsigned short port);
    void sendfile(QString path);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
