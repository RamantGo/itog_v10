#include "DatabaseManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <limits>

// Разбиение строки по разделителю
static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Загрузка из CSV
bool DatabaseManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return false;
    }

    items.clear();
    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        ++lineNum;
        if (line.empty()) continue;

        std::vector<std::string> parts = split(line, ';');
        if (parts.size() != 5) {
            std::cerr << "Предупреждение: строка " << lineNum
                << " имеет неверный формат, пропущена." << std::endl;
            continue;
        }

        try {
            unsigned int code = std::stoul(parts[0]);
            std::string name = parts[1];
            std::string type = parts[2];
            float basePrice = std::stof(parts[3]);
            float discount = std::stof(parts[4]);

            items.emplace_back(code, name, type, basePrice, discount);
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка преобразования в строке " << lineNum
                << ": " << e.what() << ", пропущена." << std::endl;
        }
    }

    file.close();
    std::cout << "Загружено " << items.size() << " записей." << std::endl;
    return true;
}

// Сохранение в CSV
bool DatabaseManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось сохранить файл " << filename << std::endl;
        return false;
    }

    for (const auto& p : items) {
        file << p.getCode() << ";"
            << p.getName() << ";"
            << p.getType() << ";"
            << p.getBasePrice() << ";"
            << p.getDiscount() << "\n";
    }
    file.close();
    std::cout << "Сохранено " << items.size() << " записей." << std::endl;
    return true;
}

// Просмотр таблицы
void DatabaseManager::displayAll() const {
    if (items.empty()) {
        std::cout << "База данных пуста." << std::endl;
        return;
    }

    std::cout << std::left
        << std::setw(4) << "№"
        << std::setw(8) << "Код"
        << std::setw(20) << "Название"
        << std::setw(15) << "Тип"
        << std::setw(12) << "Базовая"
        << std::setw(10) << "Скидка%"
        << std::setw(12) << "Цена прод."
        << std::endl;
    std::cout << std::string(81, '-') << std::endl;

    for (size_t i = 0; i < items.size(); ++i) {
        const auto& p = items[i];
        std::cout << std::left
            << std::setw(4) << i + 1
            << std::setw(8) << p.getCode()
            << std::setw(20) << p.getName()
            << std::setw(15) << p.getType()
            << std::setw(12) << std::fixed << std::setprecision(2) << p.getBasePrice()
            << std::setw(10) << std::fixed << std::setprecision(1) << p.getDiscount()
            << std::setw(12) << std::fixed << std::setprecision(2) << p.getSalePrice()
            << std::endl;
    }
}

// Добавление записи
void DatabaseManager::addRecord() {
    Product p;
    unsigned int code;
    std::string name, type;
    float price, disc;

    std::cout << "Введите код товара (целое): ";
    while (!(std::cin >> code)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некорректный ввод. Повторите: ";
    }
    std::cin.ignore(); // убрать \n

    std::cout << "Название: ";
    std::getline(std::cin, name);
    std::cout << "Тип: ";
    std::getline(std::cin, type);

    std::cout << "Базовая цена: ";
    while (!(std::cin >> price) || price < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Цена должна быть неотрицательным числом. Повторите: ";
    }

    std::cout << "Скидка (%): ";
    while (!(std::cin >> disc) || disc < 0 || disc > 100) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Скидка должна быть от 0 до 100. Повторите: ";
    }

    p.setCode(code);
    p.setName(name);
    p.setType(type);
    p.setBasePrice(price);
    p.setDiscount(disc);

    items.push_back(p);
    std::cout << "Товар успешно добавлен." << std::endl;
}

// Удаление по номеру
void DatabaseManager::deleteRecordByIndex() {
    if (items.empty()) {
        std::cout << "Нет записей для удаления." << std::endl;
        return;
    }

    displayAll();
    std::cout << "Введите номер записи для удаления (1.." << items.size() << "): ";
    size_t idx;
    while (!(std::cin >> idx) || idx < 1 || idx > items.size()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный номер. Повторите: ";
    }

    items.erase(items.begin() + idx - 1);
    std::cout << "Запись удалена." << std::endl;
}

// Сортировка по коду
void DatabaseManager::sortByCode() {
    std::sort(items.begin(), items.end(),
        [](const Product& a, const Product& b) { return a.getCode() < b.getCode(); });
    std::cout << "База отсортирована по коду товара." << std::endl;
}

// Поиск по коду
void DatabaseManager::searchByCode() const {
    if (items.empty()) {
        std::cout << "База данных пуста." << std::endl;
        return;
    }

    std::cout << "Введите код для поиска: ";
    unsigned int searchCode;
    while (!(std::cin >> searchCode)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некорректный код. Повторите: ";
    }

    auto it = std::find_if(items.begin(), items.end(),
        [searchCode](const Product& p) { return p.getCode() == searchCode; });
    if (it != items.end()) {
        std::cout << "Найден товар:" << std::endl;
        std::cout << "Код: " << it->getCode() << std::endl;
        std::cout << "Название: " << it->getName() << std::endl;
        std::cout << "Тип: " << it->getType() << std::endl;
        std::cout << "Базовая цена: " << it->getBasePrice() << std::endl;
        std::cout << "Скидка: " << it->getDiscount() << "%" << std::endl;
        std::cout << "Цена продажи: " << it->getSalePrice() << std::endl;
    }
    else {
        std::cout << "Товар с кодом " << searchCode << " не найден." << std::endl;
    }
}

// Выборка по диапазону базовой цены
void DatabaseManager::selectByBasePriceRange() const {
    if (items.empty()) {
        std::cout << "База данных пуста." << std::endl;
        return;
    }

    float minPrice, maxPrice;
    std::cout << "Введите минимальную базовую цену: ";
    while (!(std::cin >> minPrice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некорректное число. Повторите: ";
    }
    std::cout << "Введите максимальную базовую цену: ";
    while (!(std::cin >> maxPrice) || maxPrice < minPrice) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (maxPrice < minPrice)
            std::cout << "Максимальная цена не может быть меньше минимальной. Повторите: ";
        else
            std::cout << "Некорректное число. Повторите: ";
    }

    std::cout << "\nТовары с базовой ценой от " << minPrice << " до " << maxPrice << ":\n";
    int count = 0;
    for (const auto& p : items) {
        if (p.getBasePrice() >= minPrice && p.getBasePrice() <= maxPrice) {
            ++count;
            std::cout << p.getCode() << " " << p.getName() << " ("
                << p.getType() << ") базовая: " << p.getBasePrice()
                << " продажная: " << p.getSalePrice() << std::endl;
        }
    }
    if (count == 0)
        std::cout << "Нет товаров в заданном диапазоне." << std::endl;
    else
        std::cout << "Всего найдено: " << count << std::endl;
}

// Индивидуальная задача 1: акция на заданные типы товаров
void DatabaseManager::applyPromotion() {
    if (items.empty()) {
        std::cout << "База данных пуста." << std::endl;
        return;
    }

    std::cout << "Введите типы товаров, участвующих в акции, через пробел (Enter для окончания):\n";
    std::cin.ignore();  // очистить буфер после предыдущего ввода
    std::string typesLine;
    std::getline(std::cin, typesLine);
    std::vector<std::string> promoTypes = split(typesLine, ' ');

    if (promoTypes.empty()) {
        std::cout << "Не указано ни одного типа. Операция отменена." << std::endl;
        return;
    }

    float extraPercent;
    std::cout << "Введите дополнительную скидку (%): ";
    while (!(std::cin >> extraPercent) || extraPercent < 0 || extraPercent > 100) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некорректный процент. Повторите: ";
    }

    int count = 0;
    for (auto& p : items) {
        // Проверяем, содержится ли тип товара в списке акционных
        if (std::find(promoTypes.begin(), promoTypes.end(), p.getType()) != promoTypes.end()) {
            p.applyExtraDiscount(extraPercent);
            ++count;
        }
    }

    std::cout << "Акция применена к " << count << " товарам." << std::endl;
}

// Индивидуальная задача 2: удаление товаров с продажной ценой ниже порога
void DatabaseManager::removeBelowSalePrice() {
    if (items.empty()) {
        std::cout << "База данных пуста." << std::endl;
        return;
    }

    float threshold;
    std::cout << "Введите порог цены продажи: ";
    while (!(std::cin >> threshold)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некорректное число. Повторите: ";
    }

    size_t before = items.size();
    items.erase(std::remove_if(items.begin(), items.end(),
        [threshold](const Product& p) {
            return p.getSalePrice() < threshold;
        }),
        items.end());

    size_t removed = before - items.size();
    std::cout << "Удалено " << removed << " товаров с ценой продажи ниже " << threshold << ".\n";
}

bool DatabaseManager::isEmpty() const {
    return items.empty();
}