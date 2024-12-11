#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QRandomGenerator>
#include <QTextStream>

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

void MainWindow::on_GeneratPassword_B_clicked()
{
    // Диапазоны символов
    const QString letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const QString digits = "0123456789";

    // Определяем длину пароля
    int length = QRandomGenerator::global()->bounded(20, 31); // От 20 до 30 включительно

    QString password;

    for (int i = 0; i < length; ++i) {
        // Случайно выбираем: буква или цифра
        if (QRandomGenerator::global()->bounded(2) == 0) {
            password.append(letters[QRandomGenerator::global()->bounded(letters.size())]);
        } else {
            password.append(digits[QRandomGenerator::global()->bounded(digits.size())]);
        }
    }

    // Устанавливаем сгенерированный пароль в поле ввода
    ui->Password_L->setText(password);
}

void MainWindow::saveToMarkdownFile()
{
    // Проверяем, указан ли путь к файлу
    QString filePath = ui->Path_L->text();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Путь к файлу не указан!");
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи!");
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    // Если файл пустой, добавляем заголовок таблицы
    if (file.size() == 0) {
        file.write("\xEF\xBB\xBF"); // UTF-8 BOM
        out << "|login|password|site/app|\n";
        out << "|-|-|-|\n";
    }

    // Получаем данные из полей ввода
    QString login = ui->Logint_L->text().trimmed();
    QString password = ui->Password_L->text().trimmed();
    QString app = ui->App_L->text().trimmed();

    // Проверяем, что все поля заполнены
    if (login.isEmpty() || password.isEmpty() || app.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля перед сохранением!");
        return;
    }

    // Добавляем строку в таблицу
    out << "|" << login << "|" << password << "|" << app << "|\n";

    file.close();

    // Уведомление об успешном сохранении
    QMessageBox::information(this, "Успех", "Данные успешно сохранены!");
}

void MainWindow::on_Save_serch_B_clicked()
{
    saveToMarkdownFile();
}
