#pragma once
struct SmartEquipment{
    int ProductCode;
    char NameProduct[20] = {};
    float PriceProduct;
    int AmountProduct;

    SmartEquipment() {}
    SmartEquipment(SmartEquipment& itSelf) {
    this->ProductCode = itSelf.ProductCode;
    this->PriceProduct = itSelf.PriceProduct;
    this->AmountProduct = itSelf.AmountProduct;
    strncpy(this->NameProduct, itSelf.NameProduct, 20);
}

    bool setPrice(float price);
    bool setAmount(int amount);
    void menu();
    void edit();
    void add();
    void list();
    void buy();
    bool getProCodeAndAmount(SmartEquipment& itSelf);
};