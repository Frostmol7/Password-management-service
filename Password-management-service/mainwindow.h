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
    void on_Review_B_clicked();
    void on_GenerateLogin_B_clicked();
    void on_GeneratPassword_B_clicked();

private:
    Ui::MainWindow *ui;
    // Новый метод для загрузки пути
    void loadFilePath();
};
#endif // MAINWINDOW_H
