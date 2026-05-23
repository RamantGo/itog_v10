#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>
#include <string>
#include "Product.h"

class DatabaseManager {
private:
    std::vector<Product> items;

public:
    // Работа с файлами
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;

    // Основные операции
    void displayAll() const;
    void addRecord();
    void deleteRecordByIndex();
    void sortByCode();
    void searchByCode() const;
    void selectByBasePriceRange() const;

    // Индивидуальные задачи
    void applyPromotion();
    void removeBelowSalePrice();

    // Вспомогательные
    bool isEmpty() const;
};

#endif