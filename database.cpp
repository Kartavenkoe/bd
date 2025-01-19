#include "database.h" // Подключение заголовочного файла database

database::database() // Конструктор класса database
{
    db = QSqlDatabase::addDatabase("QSQLITE"); // Создание объекта базы данных SQLite
    db.setDatabaseName("clients.db"); // Установка имени файла базы данных
    if (!db.open()) { // Проверка открытия базы данных
        QMessageBox::critical(nullptr, QObject::tr("Database Error"), // Показ критического сообщения об ошибке
                              db.lastError().text());
    }

    createTables(); // Создание таблиц в базе данных

    model = new QSqlTableModel(nullptr, db); // Создание модели таблицы для работы с базой данных
    model->setTable("clients"); // Установка таблицы для модели
    model->setEditStrategy(QSqlTableModel::OnManualSubmit); // Установка стратегии редактирования
    model->select(); // Выполнение запроса для выбора всех записей
}

database::~database() // Деструктор класса database
{
    db.close(); // Закрытие базы данных
}

bool database::createDatabase() { // Метод для проверки создания базы данных
    return db.isOpen(); // Возврат состояния открытия базы данных
}

bool database::addClient(const QString &name, const QString &email, const QString &phone, const QString &image) { // Метод для добавления клиента
    QSqlQuery query; // Создание объекта QSqlQuery для выполнения SQL-запросов
    query.prepare("INSERT INTO clients (name, email, phone, image) VALUES (:name, :email, :phone, :image)"); // Подготовка запроса для вставки данных
    query.bindValue(":name", name); // Привязка значения к параметру запроса
    query.bindValue(":email", email); // Привязка значения к параметру запроса
    query.bindValue(":phone", phone); // Привязка значения к параметру запроса
    query.bindValue(":image", image); // Привязка значения к параметру запроса

    if (!query.exec()) { // Выполнение запроса
        QMessageBox::critical(nullptr, QObject::tr("Database Error"), // Показ критического сообщения об ошибке
                              query.lastError().text());
        return false; // Возврат false в случае ошибки
    }
    return true; // Возврат true в случае успеха
}

bool database::addCategory(const QString &name) { // Метод для добавления категории
    QSqlQuery query; // Создание объекта QSqlQuery для выполнения SQL-запросов
    query.prepare("INSERT INTO category (name) VALUES (:name)"); // Подготовка запроса для вставки данных
    query.bindValue(":name", name); // Привязка значения к параметру запроса

    if (!query.exec()) { // Выполнение запроса
        QMessageBox::critical(nullptr, QObject::tr("Database Error"), // Показ критического сообщения об ошибке
                              query.lastError().text());
        return false; // Возврат false в случае ошибки
    }
    return true; // Возврат true в случае успеха
}

bool database::addProduct(const QString &name, double price, int category_id) { // Метод для добавления товара
    QSqlQuery query; // Создание объекта QSqlQuery для выполнения SQL-запросов
    query.prepare("INSERT INTO tovar (name, price, category_id) VALUES (:name, :price, :category_id)"); // Подготовка запроса для вставки данных
    query.bindValue(":name", name); // Привязка значения к параметру запроса
    query.bindValue(":price", price); // Привязка значения к параметру запроса
    query.bindValue(":category_id", category_id); // Привязка значения к параметру запроса

    if (!query.exec()) { // Выполнение запроса
        QMessageBox::critical(nullptr, QObject::tr("Database Error"), // Показ критического сообщения об ошибке
                              query.lastError().text());
        return false; // Возврат false в случае ошибки
    }
    return true; // Возврат true в случае успеха
}

bool database::addOrder(int customer_id, int product_id, double price) { // Метод для добавления заказа
    QSqlQuery query; // Создание объекта QSqlQuery для выполнения SQL-запросов
    query.prepare("INSERT INTO orders (customer_id, status) VALUES (:customer_id, 'в ожидании')"); // Подготовка запроса для вставки данных
    query.bindValue(":customer_id", customer_id); // Привязка значения к параметру запроса

    if (!query.exec()) { // Выполнение запроса
        QMessageBox::critical(nullptr, QObject::tr("Database Error"), // Показ критического сообщения об ошибке
                              query.lastError().text());
        return false; // Возврат false в случае ошибки
    }

    int order_id = query.lastInsertId().toInt(); // Получение идентификатора последней вставленной записи

    query.prepare("INSERT INTO ordered_tovar (order_id, product_id, price) VALUES (:order_id, :product_id, :price)"); // Подготовка запроса для вставки данных
    query.bindValue(":order_id", order_id); // Привязка значения к параметру запроса
    query.bindValue(":product_id", product_id); // Привязка значения к параметру запроса
    query.bindValue(":price", price); // Привязка значения к параметру запроса

    if (!query.exec()) { // Выполнение запроса
        QMessageBox::critical(nullptr, QObject::tr("Database Error"), // Показ критического сообщения об ошибке
                              query.lastError().text());
        return false; // Возврат false в случае ошибки
    }

    return true; // Возврат true в случае успеха
}

bool database::updateOrderStatus(int order_id, const QString &status) { // Метод для обновления статуса заказа
    QSqlQuery query; // Создание объекта QSqlQuery для выполнения SQL-запросов
    query.prepare("UPDATE orders SET status = :status WHERE order_id = :order_id"); // Подготовка запроса для обновления данных
    query.bindValue(":status", status); // Привязка значения к параметру запроса
    query.bindValue(":order_id", order_id); // Привязка значения к параметру запроса

    if (!query.exec()) { // Выполнение запроса
        QMessageBox::critical(nullptr, QObject::tr("Database Error"), // Показ критического сообщения об ошибке
                              query.lastError().text());
        return false; // Возврат false в случае ошибки
    }
    return true; // Возврат true в случае успеха
}

bool database::deleteOrder(int order_id) { // Метод для удаления заказа
    QSqlQuery query; // Создание объекта QSqlQuery для выполнения SQL-запросов
    query.prepare("DELETE FROM orders WHERE order_id = :order_id"); // Подготовка запроса для удаления данных
    query.bindValue(":order_id", order_id); // Привязка значения к параметру запроса

    if (!query.exec()) { // Выполнение запроса
        QMessageBox::critical(nullptr, QObject::tr("Database Error"), // Показ критического сообщения об ошибке
                              query.lastError().text());
        return false; // Возврат false в случае ошибки
    }
    return true; // Возврат true в случае успеха
}

QSqlTableModel* database::getModel() { // Метод для получения модели таблицы
    return model; // Возврат модели таблицы
}

QSqlQueryModel* database::getCategoriesModel() { // Метод для получения модели категорий
    QSqlQueryModel *model = new QSqlQueryModel(); // Создание объекта QSqlQueryModel
    model->setQuery("SELECT category_id, name FROM category", db); // Установка запроса для выбора данных
    return model; // Возврат модели категорий
}

QSqlQueryModel* database::getProductsModel(int category_id) { // Метод для получения модели товаров
    QSqlQueryModel *model = new QSqlQueryModel(); // Создание объекта QSqlQueryModel
    model->setQuery(QString("SELECT product_id, name, price FROM tovar WHERE category_id = %1").arg(category_id), db); // Установка запроса для выбора данных
    return model; // Возврат модели товаров
}

QSqlQueryModel* database::getOrdersModel(int customer_id) { // Метод для получения модели заказов
    QSqlQueryModel *model = new QSqlQueryModel(); // Создание объекта QSqlQueryModel
    model->setQuery(QString("SELECT o.order_id, o.status, t.name, ot.price "
                            "FROM orders o "
                            "JOIN ordered_tovar ot ON o.order_id = ot.order_id "
                            "JOIN tovar t ON ot.product_id = t.product_id "
                            "WHERE o.customer_id = %1").arg(customer_id), db); // Установка запроса для выбора данных
    return model; // Возврат модели заказов
}

void database::createTables() { // Метод для создания таблиц в базе данных
    QSqlQuery query; // Создание объекта QSqlQuery для выполнения SQL-запросов

    query.exec("CREATE TABLE IF NOT EXISTS clients (" // Выполнение запроса для создания таблицы клиентов
               "customer_id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name VARCHAR(100),"
               "email VARCHAR(100) UNIQUE,"
               "phone VARCHAR(15),"
               "image VARCHAR(100))");

    query.exec("CREATE TABLE IF NOT EXISTS orders (" // Выполнение запроса для создания таблицы заказов
               "order_id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "customer_id INTEGER,"
               "status VARCHAR(20) DEFAULT 'в ожидании',"
               "FOREIGN KEY (customer_id) REFERENCES clients(customer_id))");

    query.exec("CREATE TABLE IF NOT EXISTS tovar (" // Выполнение запроса для создания таблицы товаров
               "product_id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name VARCHAR(100),"
               "price DECIMAL(10, 2),"
               "category_id INTEGER,"
               "FOREIGN KEY (category_id) REFERENCES category(category_id))");

    query.exec("CREATE TABLE IF NOT EXISTS ordered_tovar (" // Выполнение запроса для создания таблицы заказанных товаров
               "order_item_id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "order_id INTEGER,"
               "product_id INTEGER,"
               "quantity INTEGER DEFAULT 1,"
               "price DECIMAL(10, 2),"
               "FOREIGN KEY (order_id) REFERENCES orders(order_id),"
               "FOREIGN KEY (product_id) REFERENCES tovar(product_id))");

    query.exec("CREATE TABLE IF NOT EXISTS category (" // Выполнение запроса для создания таблицы категорий
               "category_id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name VARCHAR(100))");

    if (query.lastError().isValid()) { // Проверка наличия ошибок при выполнении запросов
        QMessageBox::critical(nullptr, QObject::tr("Database Error"), // Показ критического сообщения об ошибке
                              query.lastError().text());
    }
}