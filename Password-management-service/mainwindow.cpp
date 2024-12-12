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
    QString filePath = ui->Path_L->text();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Путь к файлу не указан!");
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Невозможно открыть файл для чтения/записи!");
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString content = in.readAll();
    file.seek(0); // Возвращаем указатель на начало файла

    QString app = ui->App_L->text();
    if (app.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Поле 'Сайт/Приложение' не заполнено!");
        return;
    }

    QStringList lines = content.split('\n');
    QString login, password;

    // 3. Поиск записи
    bool entryFound = false;
    for (const QString &line : lines) {
        QStringList columns = line.split('|');
        if (columns.size() > 3 && columns[3].trimmed() == app) {
            login = columns[1].trimmed();
            password = columns[2].trimmed();
            entryFound = true;
            break;
        }
    }

    if (entryFound) {
        ui->Logint_L->setText(login);
        ui->Password_L->setText(password);
        file.close();
        return;
    }

    // 4. Проверка заполненности логина и пароля
    login = ui->Logint_L->text();
    password = ui->Password_L->text();
    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Необходимо заполнить поля логина и пароля!");
        return;
    }

    // 5. Проверка первых строк
    if (lines.size() < 2 || !lines[0].contains("|Login|Password|App/Site|") || !lines[1].contains("|-|-|-|")) {
        file.resize(0); // Очищаем файл
        file.write("\xEF\xBB\xBF"); // UTF-8 BOM
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << "|Login|Password|App/Site|\n";
        out << "|-|-|-|\n";
    }

    // 6. Добавление новой строки
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out.seek(file.size()); // Переходим в конец файла
    out << "|" << login << "|" << password << "|" << app << "|\n";

    file.close();
}






void MainWindow::on_Save_serch_B_clicked()
{
    saveToMarkdownFile();
}
