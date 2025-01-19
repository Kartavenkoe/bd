#ifndef DATABASE_H // Проверка, не определен ли уже макрос DATABASE_H
#define DATABASE_H // Определение макроса DATABASE_H для предотвращения повторного включения заголовочного файла

#include <QSqlDatabase> // Подключение заголовочного файла для работы с базой данных SQL
#include <QSqlTableModel> // Подключение заголовочного файла для работы с моделью таблицы SQL
#include <QSqlQueryModel> // Подключение заголовочного файла для работы с моделью запроса SQL
#include <QSqlQuery> // Подключение заголовочного файла для выполнения SQL-запросов
#include <QSqlError> // Подключение заголовочного файла для работы с ошибками SQL
#include <QMessageBox> // Подключение заголовочного файла для отображения сообщений пользователю

class database // Объявление класса database
{
public:
    database(); // Конструктор класса database
    ~database(); // Деструктор класса database

    bool createDatabase(); // Метод для проверки создания базы данных
    bool addClient(const QString &name, const QString &email, const QString &phone, const QString &image); // Метод для добавления клиента
    bool addCategory(const QString &name); // Метод для добавления категории
    bool addProduct(const QString &name, double price, int category_id); // Метод для добавления товара
    bool addOrder(int customer_id, int product_id, double price); // Метод для добавления заказа
    bool updateOrderStatus(int order_id, const QString &status); // Метод для обновления статуса заказа
    bool deleteOrder(int order_id); // Метод для удаления заказа

    QSqlTableModel* getModel(); // Метод для получения модели таблицы
    QSqlQueryModel* getCategoriesModel(); // Метод для получения модели категорий
    QSqlQueryModel* getProductsModel(int category_id); // Метод для получения модели товаров
    QSqlQueryModel* getOrdersModel(int customer_id); // Метод для получения модели заказов

private:
    QSqlDatabase db; // Объект базы данных SQL
    QSqlTableModel *model; // Модель таблицы SQL
    void createTables(); // Метод для создания таблиц в базе данных
};

#endif // DATABASE_H // Конец условной компиляции
