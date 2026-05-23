#include <iostream>
#include <string>
#include "DatabaseManager.h"


void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForEnter() {
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int main() {
    system("chcp 1251");
    DatabaseManager db;
    std::string filename = "price_list.csv";
    bool loaded = false;

    while (true) {
        clearScreen();
        std::cout << "===== Прайс-лист (Вариант 10) =====\n"
            << "1. Загрузить базу из файла\n"
            << "2. Просмотреть все записи\n"
            << "3. Добавить запись\n"
            << "4. Удалить запись по номеру\n"
            << "5. Сохранить базу в файл\n"
            << "6. Сортировать по коду\n"
            << "7. Найти по коду\n"
            << "8. Выборка по диапазону базовой цены\n"
            << "9. Применить акцию (доп. скидка на типы)\n"
            << "10. Удалить товары дешевле порога\n"
            << "0. Выход\n"
            << "Ваш выбор: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод. Попробуйте снова.\n";
            waitForEnter();
            continue;
        }

        switch (choice) {
        case 1:
            if (db.loadFromFile(filename))
                loaded = true;
            waitForEnter();
            break;
        case 2:
            db.displayAll();
            waitForEnter();
            break;
        case 3:
            if (!loaded) {
                std::cout << "Сначала загрузите базу данных (пункт 1).\n";
            }
            else {
                db.addRecord();
            }
            waitForEnter();
            break;
        case 4:
            if (!loaded) std::cout << "Сначала загрузите базу данных.\n";
            else db.deleteRecordByIndex();
            waitForEnter();
            break;
        case 5:
            if (!loaded) std::cout << "Нет данных для сохранения.\n";
            else db.saveToFile(filename);
            waitForEnter();
            break;
        case 6:
            if (!loaded) std::cout << "База не загружена.\n";
            else db.sortByCode();
            waitForEnter();
            break;
        case 7:
            if (!loaded) std::cout << "База не загружена.\n";
            else db.searchByCode();
            waitForEnter();
            break;
        case 8:
            if (!loaded) std::cout << "База не загружена.\n";
            else db.selectByBasePriceRange();
            waitForEnter();
            break;
        case 9:
            if (!loaded) std::cout << "База не загружена.\n";
            else db.applyPromotion();
            waitForEnter();
            break;
        case 10:
            if (!loaded) std::cout << "База не загружена.\n";
            else db.removeBelowSalePrice();
            waitForEnter();
            break;
        case 0:
            std::cout << "До свидания!\n";
            return 0;
        default:
            std::cout << "Неверный пункт меню.\n";
            waitForEnter();
        }
    }
}