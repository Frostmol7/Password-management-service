# Password Management Service

**Password Management Service** — это приложение на основе Qt для управления вашими паролями и данными авторизации. Оно поддерживает создание, хранение и поиск паролей, а также генерацию логинов и паролей.

---

## 🛠️ Основные функции

- Генерация логина (8-12 символов, без спецсимволов, не начинается с цифры).
- Генерация пароля (20-30 символов, без спецсимволов).
- Сохранение данных в формате таблицы Markdown.
- Поиск и обновление существующих записей.
- Полная поддержка файлов `.md`, которые можно использовать в Obsidian.

---

## 📋 Установка

### Требования:
- Qt 6 или выше.
- Компилятор, совместимый с вашей ОС (например, MSVC для Windows).

### Инструкции:
1. Клонируйте репозиторий:
   ```bash
   git clone https://github.com/ваш-логин/password-management-service.git
   ```
### Дальнейшие планы для развития:
1. Добавить полноценную инструкцию со скриншотами
2. Сделать Инструкцию так же на английском
3. Сделать установщик, который будет просто скачать и поставить на любой пк без Qt
4. Добавление флагов для генерации пароля (такие как спец символы)
5. Поддержка экспорта/импорта данных в других форматах (JSON, CSV)
