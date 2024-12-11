#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QRandomGenerator>

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

void MainWindow::on_GenerateLogin_B_clicked()
{
    // Диапазоны символов
    const QString letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const QString digits = "0123456789";

    // Определяем длину логина
    int length = QRandomGenerator::global()->bounded(8, 13); // От 8 до 12 включительно

    QString login;
    login.append(letters[QRandomGenerator::global()->bounded(letters.size())]); // Первый символ - буква

    for (int i = 1; i < length; ++i) {
        // Случайно выбираем: буква или цифра
        if (QRandomGenerator::global()->bounded(2) == 0) {
            login.append(letters[QRandomGenerator::global()->bounded(letters.size())]);
        } else {
            login.append(digits[QRandomGenerator::global()->bounded(digits.size())]);
        }
    }

    // Устанавливаем сгенерированный логин в поле ввода
    ui->Logint_L->setText(login);
}
