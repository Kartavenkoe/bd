#include "mainwindow.h" // Подключение заголовочного файла MainWindow

#include <QApplication> // Подключение заголовочного файла QApplication для создания приложения Qt

int main(int argc, char *argv[]) // Определение точки входа в программу
{
    QApplication a(argc, argv); // Создание объекта приложения Qt

    // Загрузка стилей из файла
    QFile file(":/styles.qss"); // Создание объекта QFile для чтения файла стилей
    if (file.open(QFile::ReadOnly | QFile::Text)) { // Открытие файла в режиме только для чтения и текстовом режиме
        QString styleSheet = QLatin1String(file.readAll()); // Чтение всего содержимого файла и сохранение его в строку
        a.setStyleSheet(styleSheet); // Установка стилей для приложения
        file.close(); // Закрытие файла
    } else {
        qWarning("Unable to open styles.qss"); // Вывод предупреждения, если файл не может быть открыт
    }

    MainWindow w; // Создание объекта главного окна приложения
    w.show(); // Отображение главного окна
    return a.exec(); // Запуск цикла обработки событий приложения
}