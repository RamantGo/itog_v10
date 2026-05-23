#include "Product.h"
#include <algorithm>  // для std::min

Product::Product() : code(0), name(""), type(""), basePrice(0.0f), discount(0.0f) {}

Product::Product(unsigned int code, const std::string& name, const std::string& type,
    float basePrice, float discount)
    : code(code), name(name), type(type), basePrice(basePrice), discount(discount)
{
    // ограничение скидки
    if (this->discount < 0.0f) this->discount = 0.0f;
    if (this->discount > 100.0f) this->discount = 100.0f;
}

unsigned int Product::getCode() const { return code; }
std::string Product::getName() const { return name; }
std::string Product::getType() const { return type; }
float Product::getBasePrice() const { return basePrice; }
float Product::getDiscount() const { return discount; }

void Product::setCode(unsigned int newCode) { code = newCode; }
void Product::setName(const std::string& newName) { name = newName; }
void Product::setType(const std::string& newType) { type = newType; }
void Product::setBasePrice(float newPrice) { basePrice = newPrice; }

void Product::setDiscount(float newDiscount) {
    if (newDiscount < 0.0f) discount = 0.0f;
    else if (newDiscount > 100.0f) discount = 100.0f;
    else discount = newDiscount;
}

float Product::getSalePrice() const {
    return basePrice * (1.0f - discount / 100.0f);
}

void Product::applyExtraDiscount(float extraPercent) {
    discount = std::min(100.0f, discount + extraPercent);
}