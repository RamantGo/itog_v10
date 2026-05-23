#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
private:
    unsigned int code;
    std::string name;
    std::string type;
    float basePrice;
    float discount;  // процент скидки [0; 100]

public:
    // Конструкторы
    Product();
    Product(unsigned int code, const std::string& name, const std::string& type,
        float basePrice, float discount);

    // Геттеры
    unsigned int getCode() const;
    std::string getName() const;
    std::string getType() const;
    float getBasePrice() const;
    float getDiscount() const;

    // Сеттеры
    void setCode(unsigned int newCode);
    void setName(const std::string& newName);
    void setType(const std::string& newType);
    void setBasePrice(float newPrice);
    void setDiscount(float newDiscount);   // проверяет границы 0–100

    // Вычисление продажной цены
    float getSalePrice() const;

    // Применить дополнительную скидку (не превышая 100%)
    void applyExtraDiscount(float extraPercent);
};

#endif