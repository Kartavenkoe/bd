#include "mainwindow.h" // Подключение заголовочного файла MainWindow
#include "ui_mainwindow.h" // Подключение заголовочного файла, сгенерированного Qt Designer

MainWindow::MainWindow(QWidget *parent) // Конструктор класса MainWindow
    : QMainWindow(parent) // Инициализация базового класса QMainWindow
    , ui(new Ui::MainWindow) // Инициализация указателя на пользовательский интерфейс
{
    ui->setupUi(this); // Настройка пользовательского интерфейса

    db = new database(); // Создание объекта базы данных
    if (!db->createDatabase()) { // Проверка создания базы данных
        close(); // Закрытие приложения, если создание базы данных не удалось
    }

    QSqlTableModel *model = db->getModel(); // Получение модели данных из базы данных
    ui->all_clients->setModel(model); // Установка модели данных для таблицы клиентов
    ui->all_clients->setSelectionMode(QAbstractItemView::SingleSelection); // Установка режима выбора одной строки
    ui->all_clients->setSelectionBehavior(QAbstractItemView::SelectRows); // Установка поведения выбора строк
    ui->all_clients->setColumnHidden(0, true); // Скрытие колонки customer_id
    ui->all_clients->setColumnHidden(4, true); // Скрытие колонки image

    loadClients(); // Загрузка клиентов
}

MainWindow::~MainWindow() // Деструктор класса MainWindow
{
    delete ui; // Освобождение памяти, занятой пользовательским интерфейсом
    delete db; // Освобождение памяти, занятой объектом базы данных
}

void MainWindow::on_add_client_clicked() { // Обработчик нажатия кнопки "Добавить клиента"
    QDialog dialog(this); // Создание диалогового окна
    dialog.setWindowTitle("Добавить клиента"); // Установка заголовка диалогового окна

    QVBoxLayout layout(&dialog); // Создание вертикального макета для диалогового окна

    QLabel *labelName = new QLabel("Имя:", &dialog); // Создание метки для поля "Имя"
    QLineEdit *lineEditName = new QLineEdit(&dialog); // Создание поля ввода для "Имя"

    QLabel *labelEmail = new QLabel("Email:", &dialog); // Создание метки для поля "Email"
    QLineEdit *lineEditEmail = new QLineEdit(&dialog); // Создание поля ввода для "Email"

    QLabel *labelPhone = new QLabel("Телефон:", &dialog); // Создание метки для поля "Телефон"
    QLineEdit *lineEditPhone = new QLineEdit(&dialog); // Создание поля ввода для "Телефон"

    QLabel *labelImage = new QLabel("Фотография:", &dialog); // Создание метки для поля "Фотография"
    QLineEdit *lineEditImage = new QLineEdit(&dialog); // Создание поля ввода для "Фотография"
    QPushButton *buttonSelectImage = new QPushButton("Выбрать фотографию", &dialog); // Создание кнопки для выбора фотографии

    QPushButton *buttonSave = new QPushButton("Сохранить", &dialog); // Создание кнопки для сохранения

    layout.addWidget(labelName); // Добавление метки "Имя" в макет
    layout.addWidget(lineEditName); // Добавление поля ввода "Имя" в макет
    layout.addWidget(labelEmail); // Добавление метки "Email" в макет
    layout.addWidget(lineEditEmail); // Добавление поля ввода "Email" в макет
    layout.addWidget(labelPhone); // Добавление метки "Телефон" в макет
    layout.addWidget(lineEditPhone); // Добавление поля ввода "Телефон" в макет
    layout.addWidget(labelImage); // Добавление метки "Фотография" в макет
    layout.addWidget(lineEditImage); // Добавление поля ввода "Фотография" в макет
    layout.addWidget(buttonSelectImage); // Добавление кнопки "Выбрать фотографию" в макет
    layout.addWidget(buttonSave); // Добавление кнопки "Сохранить" в макет

    connect(buttonSelectImage, &QPushButton::clicked, [&]() { // Подключение обработчика нажатия кнопки "Выбрать фотографию"
        QString fileName = QFileDialog::getOpenFileName(this, tr("Выбрать фотографию"), "", tr("Images (*.png *.xpm *.jpg *.jpeg)")); // Открытие диалога выбора файла
        if (!fileName.isEmpty()) { // Проверка, что файл выбран
            lineEditImage->setText(fileName); // Установка пути к файлу в поле ввода "Фотография"
        }
    });

    connect(buttonSave, &QPushButton::clicked, [&]() { // Подключение обработчика нажатия кнопки "Сохранить"
        QString name = lineEditName->text(); // Получение текста из поля ввода "Имя"
        QString email = lineEditEmail->text(); // Получение текста из поля ввода "Email"
        QString phone = lineEditPhone->text(); // Получение текста из поля ввода "Телефон"
        QString image = lineEditImage->text(); // Получение текста из поля ввода "Фотография"

        if (name.isEmpty() || email.isEmpty()) { // Проверка, что поля "Имя" и "Email" не пусты
            QMessageBox::warning(&dialog, tr("Input Error"), // Показ предупреждающего сообщения
                                 tr("Name and Email fields cannot be empty."));
            return;
        }

        if (db->addClient(name, email, phone, image)) { // Добавление клиента в базу данных
            QMessageBox::information(&dialog, tr("Success"), // Показ информационного сообщения
                                     tr("Client added successfully."));
            dialog.accept(); // Закрытие диалогового окна
            loadClients(); // Загрузка клиентов
        }
    });

    dialog.exec(); // Отображение диалогового окна
}

void MainWindow::on_input_client_clicked() { // Обработчик нажатия кнопки "Ввод клиента"
    QItemSelectionModel *selectionModel = ui->all_clients->selectionModel(); // Получение модели выбора таблицы клиентов
    if (selectionModel->hasSelection()) { // Проверка, что выбрана строка
        QModelIndexList selectedRows = selectionModel->selectedRows(); // Получение списка выбранных строк
        if (!selectedRows.isEmpty()) { // Проверка, что список не пуст
            QModelIndex index = selectedRows.first(); // Получение первой выбранной строки
            int row = index.row(); // Получение номера строки
            displayClientDetails(row); // Отображение деталей клиента
            QSqlRecord record = db->getModel()->record(row); // Получение записи клиента
            int customer_id = record.value("customer_id").toInt(); // Получение идентификатора клиента
            loadOrdersForClient(customer_id); // Загрузка заказов для клиента
        }
    } else {
        QMessageBox::warning(this, tr("Selection Error"), // Показ предупреждающего сообщения
                             tr("Please select a client from the table."));
    }
}

void MainWindow::on_exit_clicked() { // Обработчик нажатия кнопки "Выход"
    clearClientDetails(); // Очистка деталей клиента
    ui->orders_clients->setModel(nullptr); // Очистка таблицы заказов
}

void MainWindow::displayClientDetails(int row) { // Отображение деталей клиента
    QSqlRecord record = db->getModel()->record(row); // Получение записи клиента
    ui->line_name->setText(record.value("name").toString()); // Установка имени клиента
    ui->line_email->setText(record.value("email").toString()); // Установка email клиента
    ui->line_phone->setText(record.value("phone").toString()); // Установка телефона клиента
    QString imagePath = record.value("image").toString(); // Получение пути к фотографии
    if (!imagePath.isEmpty()) { // Проверка, что путь к фотографии не пуст
        QPixmap pixmap(imagePath); // Создание объекта QPixmap из пути к фотографии
        pixmap = pixmap.scaled(ui->avatar_user->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation); // Масштабирование изображения
        ui->avatar_user->setScene(new QGraphicsScene(this)); // Установка новой сцены для QGraphicsView
        ui->avatar_user->scene()->addPixmap(pixmap); // Добавление изображения в сцену
        ui->avatar_user->setSceneRect(pixmap.rect()); // Установка прямоугольника сцены
    }
}

void MainWindow::clearClientDetails() { // Очистка деталей клиента
    ui->line_name->clear(); // Очистка поля "Имя"
    ui->line_email->clear(); // Очистка поля "Email"
    ui->line_phone->clear(); // Очистка поля "Телефон"
    ui->avatar_user->setScene(new QGraphicsScene(this)); // Установка новой сцены для QGraphicsView
    ui->avatar_user->scene()->clear(); // Очистка сцены
}

void MainWindow::on_add_tovar_clicked() // Обработчик нажатия кнопки "Добавить товар"
{
    QDialog dialog(this); // Создание диалогового окна
    dialog.setWindowTitle("Добавить товар"); // Установка заголовка диалогового окна

    QVBoxLayout layout(&dialog); // Создание вертикального макета для диалогового окна

    QLabel *labelName = new QLabel("Название:", &dialog); // Создание метки для поля "Название"
    QLineEdit *lineEditName = new QLineEdit(&dialog); // Создание поля ввода для "Название"

    QLabel *labelPrice = new QLabel("Цена:", &dialog); // Создание метки для поля "Цена"
    QLineEdit *lineEditPrice = new QLineEdit(&dialog); // Создание поля ввода для "Цена"

    QLabel *labelCategory = new QLabel("Категория:", &dialog); // Создание метки для поля "Категория"
    QLineEdit *lineEditCategory = new QLineEdit(&dialog); // Создание поля ввода для "Категория"

    QPushButton *buttonSave = new QPushButton("Сохранить", &dialog); // Создание кнопки для сохранения

    layout.addWidget(labelName); // Добавление метки "Название" в макет
    layout.addWidget(lineEditName); // Добавление поля ввода "Название" в макет
    layout.addWidget(labelPrice); // Добавление метки "Цена" в макет
    layout.addWidget(lineEditPrice); // Добавление поля ввода "Цена" в макет
    layout.addWidget(labelCategory); // Добавление метки "Категория" в макет
    layout.addWidget(lineEditCategory); // Добавление поля ввода "Категория" в макет
    layout.addWidget(buttonSave); // Добавление кнопки "Сохранить" в макет

    connect(buttonSave, &QPushButton::clicked, [&]() { // Подключение обработчика нажатия кнопки "Сохранить"
        QString name = lineEditName->text(); // Получение текста из поля ввода "Название"
        double price = lineEditPrice->text().toDouble(); // Получение текста из поля ввода "Цена" и преобразование в double
        QString categoryName = lineEditCategory->text(); // Получение текста из поля ввода "Категория"

        if (name.isEmpty() || categoryName.isEmpty()) { // Проверка, что поля "Название" и "Категория" не пусты
            QMessageBox::warning(&dialog, tr("Input Error"), // Показ предупреждающего сообщения
                                 tr("Name and Category fields cannot be empty."));
            return;
        }

        int category_id = -1; // Инициализация идентификатора категории
        QSqlQuery query; // Создание объекта QSqlQuery для выполнения SQL-запросов
        query.prepare("SELECT category_id FROM category WHERE name = :name"); // Подготовка запроса для получения идентификатора категории
        query.bindValue(":name", categoryName); // Привязка значения к параметру запроса
        if (query.exec() && query.next()) { // Выполнение запроса и проверка наличия результата
            category_id = query.value(0).toInt(); // Получение идентификатора категории
        } else {
            if (!db->addCategory(categoryName)) { // Добавление категории в базу данных, если она не существует
                QMessageBox::critical(&dialog, tr("Database Error"), // Показ критического сообщения
                                      tr("Failed to add category."));
                return;
            }
            query.exec(); // Повторное выполнение запроса
            query.next(); // Переход к следующей записи
            category_id = query.value(0).toInt(); // Получение идентификатора категории
        }

        if (db->addProduct(name, price, category_id)) { // Добавление товара в базу данных
            QMessageBox::information(&dialog, tr("Success"), // Показ информационного сообщения
                                     tr("Product added successfully."));
            dialog.accept(); // Закрытие диалогового окна
        } else {
            QMessageBox::critical(&dialog, tr("Database Error"), // Показ критического сообщения
                                  tr("Failed to add product."));
        }
    });

    dialog.exec(); // Отображение диалогового окна
}

void MainWindow::on_edit_user_clicked() // Обработчик нажатия кнопки "Редактировать клиента"
{
    QItemSelectionModel *selectionModel = ui->all_clients->selectionModel(); // Получение модели выбора таблицы клиентов
    if (selectionModel->hasSelection()) { // Проверка, что выбрана строка
        QModelIndexList selectedRows = selectionModel->selectedRows(); // Получение списка выбранных строк
        if (!selectedRows.isEmpty()) { // Проверка, что список не пуст
            QModelIndex index = selectedRows.first(); // Получение первой выбранной строки
            int row = index.row(); // Получение номера строки
            editClient(row); // Редактирование клиента
        }
    } else {
        QMessageBox::warning(this, tr("Selection Error"), // Показ предупреждающего сообщения
                             tr("Please select a client from the table."));
    }
}

void MainWindow::on_delete_user_clicked() // Обработчик нажатия кнопки "Удалить клиента"
{
    QItemSelectionModel *selectionModel = ui->all_clients->selectionModel(); // Получение модели выбора таблицы клиентов
    if (selectionModel->hasSelection()) { // Проверка, что выбрана строка
        QModelIndexList selectedRows = selectionModel->selectedRows(); // Получение списка выбранных строк
        if (!selectedRows.isEmpty()) { // Проверка, что список не пуст
            QModelIndex index = selectedRows.first(); // Получение первой выбранной строки
            int row = index.row(); // Получение номера строки
            deleteClient(row); // Удаление клиента
        }
    } else {
        QMessageBox::warning(this, tr("Selection Error"), // Показ предупреждающего сообщения
                             tr("Please select a client from the table."));
    }
}

void MainWindow::editClient(int row) { // Редактирование клиента
    QSqlRecord record = db->getModel()->record(row); // Получение записи клиента
    QDialog dialog(this); // Создание диалогового окна
    dialog.setWindowTitle("Редактировать клиента"); // Установка заголовка диалогового окна

    QVBoxLayout layout(&dialog); // Создание вертикального макета для диалогового окна

    QLabel *labelName = new QLabel("Имя:", &dialog); // Создание метки для поля "Имя"
    QLineEdit *lineEditName = new QLineEdit(&dialog); // Создание поля ввода для "Имя"
    lineEditName->setText(record.value("name").toString()); // Установка текста в поле ввода "Имя"

    QLabel *labelEmail = new QLabel("Email:", &dialog); // Создание метки для поля "Email"
    QLineEdit *lineEditEmail = new QLineEdit(&dialog); // Создание поля ввода для "Email"
    lineEditEmail->setText(record.value("email").toString()); // Установка текста в поле ввода "Email"

    QLabel *labelPhone = new QLabel("Телефон:", &dialog); // Создание метки для поля "Телефон"
    QLineEdit *lineEditPhone = new QLineEdit(&dialog); // Создание поля ввода для "Телефон"
    lineEditPhone->setText(record.value("phone").toString()); // Установка текста в поле ввода "Телефон"

    QLabel *labelImage = new QLabel("Фотография:", &dialog); // Создание метки для поля "Фотография"
    QLineEdit *lineEditImage = new QLineEdit(&dialog); // Создание поля ввода для "Фотография"
    lineEditImage->setText(record.value("image").toString()); // Установка текста в поле ввода "Фотография"
    QPushButton *buttonSelectImage = new QPushButton("Выбрать фотографию", &dialog); // Создание кнопки для выбора фотографии

    QPushButton *buttonSave = new QPushButton("Сохранить", &dialog); // Создание кнопки для сохранения

    layout.addWidget(labelName); // Добавление метки "Имя" в макет
    layout.addWidget(lineEditName); // Добавление поля ввода "Имя" в макет
    layout.addWidget(labelEmail); // Добавление метки "Email" в макет
    layout.addWidget(lineEditEmail); // Добавление поля ввода "Email" в макет
    layout.addWidget(labelPhone); // Добавление метки "Телефон" в макет
    layout.addWidget(lineEditPhone); // Добавление поля ввода "Телефон" в макет
    layout.addWidget(labelImage); // Добавление метки "Фотография" в макет
    layout.addWidget(lineEditImage); // Добавление поля ввода "Фотография" в макет
    layout.addWidget(buttonSelectImage); // Добавление кнопки "Выбрать фотографию" в макет
    layout.addWidget(buttonSave); // Добавление кнопки "Сохранить" в макет

    connect(buttonSelectImage, &QPushButton::clicked, [&]() { // Подключение обработчика нажатия кнопки "Выбрать фотографию"
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выбрать фотографию"), "", tr("Images (*.png *.xpm *.jpg *.jpeg)")); // Открытие диалога выбора файла
    if (!fileName.isEmpty()) { // Проверка, что файл выбран
        lineEditImage->setText(fileName); // Установка пути к файлу в поле ввода "Фотография"
    }
});

connect(buttonSave, &QPushButton::clicked, [&, row]() { // Подключение обработчика нажатия кнопки "Сохранить"
    QString name = lineEditName->text(); // Получение текста из поля ввода "Имя"
    QString email = lineEditEmail->text(); // Получение текста из поля ввода "Email"
    QString phone = lineEditPhone->text(); // Получение текста из поля ввода "Телефон"
    QString image = lineEditImage->text(); // Получение текста из поля ввода "Фотография"

    if (name.isEmpty() || email.isEmpty()) { // Проверка, что поля "Имя" и "Email" не пусты
        QMessageBox::warning(&dialog, tr("Input Error"), // Показ предупреждающего сообщения
                             tr("Name and Email fields cannot be empty."));
        return;
    }

    QSqlQuery query; // Создание объекта QSqlQuery для выполнения SQL-запросов
    query.prepare("UPDATE clients SET name = :name, email = :email, phone = :phone, image = :image WHERE customer_id = :customer_id"); // Подготовка запроса для обновления данных клиента
    query.bindValue(":name", name); // Привязка значения к параметру запроса
    query.bindValue(":email", email); // Привязка значения к параметру запроса
    query.bindValue(":phone", phone); // Привязка значения к параметру запроса
    query.bindValue(":image", image); // Привязка значения к параметру запроса
    query.bindValue(":customer_id", record.value("customer_id").toInt()); // Привязка значения к параметру запроса

    if (query.exec()) { // Выполнение запроса
        QMessageBox::information(&dialog, tr("Success"), // Показ информационного сообщения
                                 tr("Client updated successfully."));
        dialog.accept(); // Закрытие диалогового окна
        loadClients(); // Обновить таблицу после редактирования клиента
        displayClientDetails(row); // Обновить данные клиента в интерфейсе
    } else {
        QMessageBox::critical(&dialog, tr("Database Error"), // Показ критического сообщения
                              query.lastError().text());
    }
});

dialog.exec(); // Отображение диалогового окна
}

void MainWindow::deleteClient(int row) { // Метод для удаления клиента
    QSqlRecord record = db->getModel()->record(row); // Получение записи клиента
    QMessageBox::StandardButton reply; // Создание переменной для хранения ответа пользователя
    reply = QMessageBox::question(this, tr("Confirmation"), // Показ диалога подтверждения
                                  tr("Are you sure you want to delete this client?"),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) { // Проверка, что пользователь подтвердил удаление
        QSqlQuery query; // Создание объекта QSqlQuery для выполнения SQL-запросов
        query.prepare("DELETE FROM clients WHERE customer_id = :customer_id"); // Подготовка запроса для удаления клиента
        query.bindValue(":customer_id", record.value("customer_id").toInt()); // Привязка значения к параметру запроса

        if (query.exec()) { // Выполнение запроса
            QMessageBox::information(this, tr("Success"), // Показ информационного сообщения
                                     tr("Client deleted successfully."));
            loadClients(); // Обновить таблицу после удаления клиента
            clearClientDetails(); // Очистить данные клиента в интерфейсе
        } else {
            QMessageBox::critical(this, tr("Database Error"), // Показ критического сообщения
                                  query.lastError().text());
        }
    }
}

void MainWindow::loadOrdersForClient(int customer_id) { // Метод для загрузки заказов для клиента
    QSqlQueryModel *ordersModel = db->getOrdersModel(customer_id); // Получение модели данных заказов для клиента
    ui->orders_clients->setModel(ordersModel); // Установка модели данных для таблицы заказов
    ui->orders_clients->setColumnHidden(0, false); // Отображать колонку статуса
    ui->orders_clients->setColumnHidden(1, false); // Отображать колонку названия заказа
    ui->orders_clients->setColumnHidden(2, false); // Отображать колонку цены
}

void MainWindow::loadClients() { // Метод для загрузки клиентов
    db->getModel()->select(); // Выполнение запроса для выбора всех клиентов
}

void MainWindow::on_add_order_clicked() { // Обработчик нажатия кнопки "Добавить заказ"
    QItemSelectionModel *selectionModel = ui->all_clients->selectionModel(); // Получение модели выбора таблицы клиентов
    if (selectionModel->hasSelection()) { // Проверка, что выбрана строка
        QModelIndexList selectedRows = selectionModel->selectedRows(); // Получение списка выбранных строк
        if (!selectedRows.isEmpty()) { // Проверка, что список не пуст
            QModelIndex index = selectedRows.first(); // Получение первой выбранной строки
            int row = index.row(); // Получение номера строки
            QSqlRecord record = db->getModel()->record(row); // Получение записи клиента
            int customer_id = record.value("customer_id").toInt(); // Получение идентификатора клиента

            QDialog dialog(this); // Создание диалогового окна
            dialog.setWindowTitle("Создать заказ"); // Установка заголовка диалогового окна

            QVBoxLayout layout(&dialog); // Создание вертикального макета для диалогового окна

            QLabel *labelCategory = new QLabel("Категория:", &dialog); // Создание метки для поля "Категория"
            QComboBox *comboCategory = new QComboBox(&dialog); // Создание выпадающего списка для "Категория"
            QLabel *labelProduct = new QLabel("Товар:", &dialog); // Создание метки для поля "Товар"
            QComboBox *comboProduct = new QComboBox(&dialog); // Создание выпадающего списка для "Товар"
            QPushButton *buttonSave = new QPushButton("Сохранить", &dialog); // Создание кнопки для сохранения

            layout.addWidget(labelCategory); // Добавление метки "Категория" в макет
            layout.addWidget(comboCategory); // Добавление выпадающего списка "Категория" в макет
            layout.addWidget(labelProduct); // Добавление метки "Товар" в макет
            layout.addWidget(comboProduct); // Добавление выпадающего списка "Товар" в макет
            layout.addWidget(buttonSave); // Добавление кнопки "Сохранить" в макет

            QSqlQueryModel *categoriesModel = db->getCategoriesModel(); // Получение модели данных категорий
            comboCategory->setModel(categoriesModel); // Установка модели данных для выпадающего списка "Категория"
            comboCategory->setModelColumn(1); // Установка отображаемой колонки для выпадающего списка "Категория"

            // Установить текущий индекс для comboCategory
            comboCategory->setCurrentIndex(0);

            connect(comboCategory, &QComboBox::currentIndexChanged, [=](int index) { // Подключение обработчика изменения текущего индекса выпадающего списка "Категория"
                int category_id = categoriesModel->data(categoriesModel->index(index, 0)).toInt(); // Получение идентификатора категории
                QSqlQueryModel *productsModel = db->getProductsModel(category_id); // Получение модели данных товаров для выбранной категории
                comboProduct->setModel(productsModel); // Установка модели данных для выпадающего списка "Товар"
                comboProduct->setModelColumn(1); // Установка отображаемой колонки для выпадающего списка "Товар"
            });

            connect(buttonSave, &QPushButton::clicked, [=, &dialog]() { // Подключение обработчика нажатия кнопки "Сохранить"
                int categoryIndex = comboCategory->currentIndex(); // Получение текущего индекса выпадающего списка "Категория"
                int productIndex = comboProduct->currentIndex(); // Получение текущего индекса выпадающего списка "Товар"

                if (categoryIndex == -1 || productIndex == -1) { // Проверка, что выбраны категория и товар
                    QMessageBox::warning(&dialog, tr("Input Error"), // Показ предупреждающего сообщения
                                         tr("Please select a category and a product."));
                    return;
                }

                int product_id = comboProduct->model()->data(comboProduct->model()->index(productIndex, 0)).toInt(); // Получение идентификатора товара
                double price = comboProduct->model()->data(comboProduct->model()->index(productIndex, 2)).toDouble(); // Получение цены товара

                if (db->addOrder(customer_id, product_id, price)) { // Добавление заказа в базу данных
                    QMessageBox::information(&dialog, tr("Success"), // Показ информационного сообщения
                                             tr("Order added successfully."));
                    dialog.accept(); // Закрытие диалогового окна
                    loadOrdersForClient(customer_id); // Загрузка заказов для клиента
                } else {
                    QMessageBox::critical(&dialog, tr("Database Error"), // Показ критического сообщения
                                          tr("Failed to add order."));
                }
            });

            dialog.exec(); // Отображение диалогового окна
        }
    } else {
        QMessageBox::warning(this, tr("Selection Error"), // Показ предупреждающего сообщения
                             tr("Please select a client from the table."));
    }
}

void MainWindow::on_edit_order_clicked() { // Обработчик нажатия кнопки "Редактировать заказ"
    QItemSelectionModel *selectionModel = ui->orders_clients->selectionModel(); // Получение модели выбора таблицы заказов
    if (selectionModel->hasSelection()) { // Проверка, что выбрана строка
        QModelIndexList selectedRows = selectionModel->selectedRows(); // Получение списка выбранных строк
        if (!selectedRows.isEmpty()) { // Проверка, что список не пуст
            QModelIndex index = selectedRows.first(); // Получение первой выбранной строки
            int row = index.row(); // Получение номера строки
            editOrder(row); // Редактирование заказа
        }
    } else {
        QMessageBox::warning(this, tr("Selection Error"), // Показ предупреждающего сообщения
                             tr("Please select an order from the table."));
    }
}

void MainWindow::on_delete_order_clicked() { // Обработчик нажатия кнопки "Удалить заказ"
    QItemSelectionModel *selectionModel = ui->orders_clients->selectionModel(); // Получение модели выбора таблицы заказов
    if (selectionModel->hasSelection()) { // Проверка, что выбрана строка
        QModelIndexList selectedRows = selectionModel->selectedRows(); // Получение списка выбранных строк
        if (!selectedRows.isEmpty()) { // Проверка, что список не пуст
            QModelIndex index = selectedRows.first(); // Получение первой выбранной строки
            int row = index.row(); // Получение номера строки
            deleteOrder(row); // Удаление заказа
        }
    } else {
        QMessageBox::warning(this, tr("Selection Error"), // Показ предупреждающего сообщения
                             tr("Please select an order from the table."));
    }
}

void MainWindow::editOrder(int row) { // Метод для редактирования заказа
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->orders_clients->model()); // Получение модели данных таблицы заказов
    QSqlRecord record = model->record(row); // Получение записи заказа
    int order_id = record.value("order_id").toInt(); // Получение идентификатора заказа

    QDialog dialog(this); // Создание диалогового окна
    dialog.setWindowTitle("Редактировать заказ"); // Установка заголовка диалогового окна

    QVBoxLayout layout(&dialog); // Создание вертикального макета для диалогового окна

    QLabel *labelStatus = new QLabel("Статус:", &dialog); // Создание метки для поля "Статус"
    QComboBox *comboStatus = new QComboBox(&dialog); // Создание выпадающего списка для "Статус"
    comboStatus->addItem("в ожидании"); // Добавление элемента в выпадающий список
    comboStatus->addItem("в процессе"); // Добавление элемента в выпадающий список
    comboStatus->addItem("завершен"); // Добавление элемента в выпадающий список
    comboStatus->setCurrentText(record.value("status").toString()); // Установка текущего текста выпадающего списка

    QPushButton *buttonSave = new QPushButton("Сохранить", &dialog); // Создание кнопки для сохранения

    layout.addWidget(labelStatus); // Добавление метки "Статус" в макет
    layout.addWidget(comboStatus); // Добавление выпадающего списка "Статус" в макет
    layout.addWidget(buttonSave); // Добавление кнопки "Сохранить" в макет

    connect(buttonSave, &QPushButton::clicked, [&, order_id, row]() { // Подключение обработчика нажатия кнопки "Сохранить"
        QString status = comboStatus->currentText(); // Получение текущего текста выпадающего списка "Статус"

        if (db->updateOrderStatus(order_id, status)) { // Обновление статуса заказа в базе данных
            QMessageBox::information(&dialog, tr("Success"), // Показ информационного сообщения
                                     tr("Order status updated successfully."));
            dialog.accept(); // Закрытие диалогового окна
            // Обновить статус в модели данных
            model->setData(model->index(row, 0), status);
        } else {
            QMessageBox::critical(&dialog, tr("Database Error"), // Показ критического сообщения
                                  tr("Failed to update order status."));
        }
    });

    dialog.exec(); // Отображение диалогового окна
}

void MainWindow::deleteOrder(int row) { // Метод для удаления заказа
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->orders_clients->model()); // Получение модели данных таблицы заказов
    QSqlRecord record = model->record(row); // Получение записи заказа
    int order_id = record.value("order_id").toInt(); // Получение идентификатора заказа

    QMessageBox::StandardButton reply; // Создание переменной для хранения ответа пользователя
    reply = QMessageBox::question(this, tr("Confirmation"), // Показ диалога подтверждения
                                  tr("Are you sure you want to delete this order?"),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) { // Проверка, что пользователь подтвердил удаление
        if (db->deleteOrder(order_id)) { // Удаление заказа из базы данных
            QMessageBox::information(this, tr("Success"), // Показ информационного сообщения
                                     tr("Order deleted successfully."));
            // Удалить заказ из модели данных
            model->removeRow(row);
        } else {
            QMessageBox::critical(this, tr("Database Error"), // Показ критического сообщения
                                  tr("Failed to delete order."));
        }
    }
}

void MainWindow::on_reload_clicked() // Обработчик нажатия кнопки "Перезагрузить"
{
    QItemSelectionModel *selectionModel = ui->all_clients->selectionModel(); // Получение модели выбора таблицы клиентов
    if (selectionModel->hasSelection()) { // Проверка, что выбрана строка
        QModelIndexList selectedRows = selectionModel->selectedRows(); // Получение списка выбранных строк
        if (!selectedRows.isEmpty()) { // Проверка, что список не пуст
            QModelIndex index = selectedRows.first(); // Получение первой выбранной строки
            int row = index.row(); // Получение номера строки
            QSqlRecord record = db->getModel()->record(row); // Получение записи клиента
            int customer_id = record.value("customer_id").toInt(); // Получение идентификатора клиента
            loadOrdersForClient(customer_id); // Загрузка заказов для клиента
        }
    } else {
        QMessageBox::warning(this, tr("Selection Error"), // Показ предупреждающего сообщения
                             tr("Please select a client from the table."));
    }
}