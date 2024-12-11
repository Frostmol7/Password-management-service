#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Сервис управления паролями");
    // Загрузка сохранённого пути
    loadFilePath();  // Вызов метода для загрузки пути из предыдущих сессий
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Review_B_clicked() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Выберите файл для работы",
        "",
        "Файлы данных (*.txt *.md);;Текстовые файлы (*.txt);;Markdown (*.md);;Все файлы (*.*)");

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.exists()) {
            QMessageBox::warning(this, "Ошибка", "Выбранный файл не существует.");
            return;
        }

        ui->Path_L->setText(filePath);

        // Сохранить путь в настройки
        QSettings settings("MyCompany", "PasswordManager");
        settings.setValue("filePath", filePath);
    }
}

// Загрузить путь при запуске приложения
void MainWindow::loadFilePath() {
    QSettings settings("MyCompany", "PasswordManager");
    QString filePath = settings.value("filePath", "").toString();
    if (!filePath.isEmpty()) {
        ui->Path_L->setText(filePath);
    }
}
