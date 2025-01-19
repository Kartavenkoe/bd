#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> // Подключение заголовочного файла для работы с главным окном приложения
#include <QSqlTableModel> // Подключение заголовочного файла для работы с моделью таблицы SQL
#include <QSqlQueryModel> // Подключение заголовочного файла для работы с моделью запроса SQL
#include <QSqlQuery> // Подключение заголовочного файла для выполнения SQL-запросов
#include <QSqlError> // Подключение заголовочного файла для работы с ошибками SQL
#include <QMessageBox> // Подключение заголовочного файла для отображения сообщений пользователю
#include <QFileDialog> // Подключение заголовочного файла для работы с диалогами выбора файлов
#include <QDialog> // Подключение заголовочного файла для работы с диалоговыми окнами
#include <QVBoxLayout> // Подключение заголовочного файла для работы с вертикальным макетом
#include <QLabel> // Подключение заголовочного файла для работы с метками
#include <QLineEdit> // Подключение заголовочного файла для работы с полями ввода текста
#include <QPushButton> // Подключение заголовочного файла для работы с кнопками
#include <QComboBox> // Подключение заголовочного файла для работы с выпадающими списками
#include <QSpinBox> // Подключение заголовочного файла для работы с полями ввода чисел
#include <QGraphicsView> // Подключение заголовочного файла для работы с графическим представлением
#include <QGraphicsScene> // Подключение заголовочного файла для работы с графической сценой
#include <QPixmap> // Подключение заголовочного файла для работы с изображениями
#include <QItemSelectionModel> // Подключение заголовочного файла для работы с моделью выбора элементов
#include <QSqlRecord> // Подключение заголовочного файла для работы с записями SQL
#include <QSqlDatabase> // Подключение заголовочного файла для работы с базой данных SQL
#include "database.h" // Подключение заголовочного файла для работы с классом database

QT_BEGIN_NAMESPACE // Начало пространства имен Qt
namespace Ui { // Объявление пространства имен Ui
class MainWindow; // Объявление класса MainWindow в пространстве имен Ui
}
QT_END_NAMESPACE // Конец пространства имен Qt

class MainWindow : public QMainWindow // Объявление класса MainWindow, наследующего QMainWindow
{
    Q_OBJECT // Макрос для включения системы сигналов и слотов Qt

public:
    MainWindow(QWidget *parent = nullptr); // Конструктор класса MainWindow
    ~MainWindow(); // Деструктор класса MainWindow

private slots: // Объявление слотов (методов, которые могут быть вызваны в ответ на сигналы)
    void on_add_client_clicked(); // Слот для обработки нажатия кнопки добавления клиента
    void loadClients(); // Слот для загрузки клиентов
    void on_input_client_clicked(); // Слот для обработки нажатия кнопки ввода клиента
    void on_exit_clicked(); // Слот для обработки нажатия кнопки выхода
    void on_add_tovar_clicked(); // Слот для обработки нажатия кнопки добавления товара
    void on_edit_user_clicked(); // Слот для обработки нажатия кнопки редактирования пользователя
    void on_delete_user_clicked(); // Слот для обработки нажатия кнопки удаления пользователя
    void on_add_order_clicked(); // Слот для обработки нажатия кнопки добавления заказа
    void on_edit_order_clicked(); // Слот для обработки нажатия кнопки редактирования заказа
    void on_delete_order_clicked(); // Слот для обработки нажатия кнопки удаления заказа
    void on_reload_clicked(); // Слот для обработки нажатия кнопки перезагрузки

private:
    Ui::MainWindow *ui; // Указатель на объект пользовательского интерфейса MainWindow
    database *db; // Указатель на объект базы данных
    void displayClientDetails(int row); // Метод для отображения деталей клиента
    void clearClientDetails(); // Метод для очистки деталей клиента
    void editClient(int row); // Метод для редактирования клиента
    void deleteClient(int row); // Метод для удаления клиента
    void loadOrdersForClient(int customer_id); // Метод для загрузки заказов для клиента
    void editOrder(int row); // Метод для редактирования заказа
    void deleteOrder(int row); // Метод для удаления заказа
};

#endif // MAINWINDOW_H // Конец условной компиляции
