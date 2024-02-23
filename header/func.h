#pragma once
#include <iostream>
#include <fstream>
#include <string.h>
#include "struct.h"

bool SmartEquipment::setPrice(float price){
    if(price < 0){
        return false;
    }
    this->PriceProduct = price;
    return true;
}
bool SmartEquipment::setAmount(int amount){
    if(amount < 0){
        return false;
    }
    this->AmountProduct = amount;
    return true;
}
bool SmartEquipment::getProCodeAndAmount(SmartEquipment& itSelf) {
    SmartEquipment device;
    int token = 0;
    std::fstream data;
    data.open("../resource/product.txt", std::ios::in);
    if (!data) {
        return false;
    }
    else {
        while (!data.eof()) {
            data >> device.ProductCode;
            data.get();
            data.getline(device.NameProduct, 20, ' ');
            data >> device.PriceProduct >> device.AmountProduct;
            if (device.ProductCode == itSelf.ProductCode) {
                if (itSelf.AmountProduct < 0 || itSelf.AmountProduct > device.AmountProduct) {
                    token = 0;
                }
                else {
                    token++;
                    itSelf = device;
                }
            }
        }
    }

    data.close();

    if (token == 1) {
        return true;
    }
    return false;
}
void SmartEquipment::menu() {
    step:
    char c;
    std::cout   << "\n\t\t\t\t______________________________________\n"
	            <<"\t\t\t\t                                      \n"
                << "\t\t\t\t                 MENU                \n"
                << "\t\t\t\t______________________________________\n"
                << "\t\t\t\t                                      \n"
                << "\t\t\t\t|  1) Add             |\n"
                << "\t\t\t\t|                     |\n"
                << "\t\t\t\t|  2) List            |\n"
                << "\t\t\t\t|                     |\n"
                << "\t\t\t\t|  3) Buy             |\n"
                << "\t\t\t\t|                     |\n"
                << "\t\t\t\t|  4) Exit            |\n"
                << "\t\t\t\t|                     |\n"
                << "\n\t\t\t Please select: ";
    std::cin >> c;
    std::cin.ignore();
    int choice = (int)c - (int)('0');
    switch(choice) {
        case 1:
            {
                system("cls");
                edit();
			}
			break;

        case 2:
            {
                list();
            }
            break;

        case 3:
            {
                buy();
            }
            break;

        case 4:
            {
                exit(0);
            }
            break;

        default:
            {   
                system("cls");
				std::cout << "\t\t\t\tPlease select from the given options φ(゜▽゜*)♪\n\n";
			}
            break;
    }

    goto step;
}
void SmartEquipment::edit() {
    step:
    char c;
    std::cout << "\n\n\n\t\t\t Edit Menu"
	        <<"\n\t\t\t|____1) Add product______|"
	        <<"\n\t\t\t|                        |"
            <<"\n\t\t\t|____2) Main menu________|"
            <<"\n\n\t Please enter your choice: ";

    std::cin >> c;
    int choice = (int)c - (int)('0');
    std::cin.ignore();
    switch(choice) {
        case 1:
            {
                add();
            }
            break;

        case 2:
            {
                menu();
            }
            break;

        default:
            {
                system("cls");
                std::cout << "\t\t\tInvalid choice (┬┬﹏┬┬)";
            }
            break;
    }

    goto step;
}
void SmartEquipment::add() {
    system("cls");
    std::fstream data;
    int proCode, choice;
    char namePro[20] = {};
    float pricePro;
    int amountPro;
    int token = 0;

    std::cout << "\n\n\t\t\t! Add new product !";
    do {
        std::cout <<   "\n\n\t\t\t  How many products do you want to add? : ";
        std::cin >> choice;
        std::cin.ignore();
    } while(choice < 0);

    SmartEquipment* ptr = nullptr;
    ptr = new SmartEquipment[choice];

    for(int i = 0; i < choice; i++){
        step_3:
        std::cout << "\n\n\t Product number " << i + 1 << "\n\n\t Product code of the product: ";
        std::cin >> ptr[i].ProductCode;
        std::cin.ignore();

        std::cout << "\n\n\t Name of the product: ";
        std::cin.getline(ptr[i].NameProduct, 20);

        step_1:
        std::cout << "\n\n\t Price of the product: ";
        float check_1;
        std::cin >> check_1;
        std::cin.ignore();
        if(ptr[i].setPrice(check_1)) {}
        else {
            std::cout << "Please type the true price !";
            goto step_1;
        }

        step_2:
        std::cout << "\n\n\t The number of product: ";
        int check_2;
        std::cin >> check_2;
        std::cin.ignore();
        if(ptr[i].setAmount(check_2)) {}
        else {
            std::cout << "Please type the true amount !";
            goto step_2;
        }

        data.open("../resource/product.txt", std::ios::in);

        if(!data) {
            data.open("../resource/product.txt", std::ios::app | std::ios::out);
            data << ptr[i].ProductCode << " " << ptr[i].NameProduct << " "
                << ptr[i].PriceProduct << " " << ptr[i].AmountProduct;
            data.close();
        }
        else {
            data >> proCode;
            data.get();
            data.getline(namePro, 20, ' ');
            data >> pricePro >> amountPro;

            while(!data.eof()){
                if(proCode == ptr[i].ProductCode){
                    token++;
                }
                data >> proCode;
                data.get();
                data.getline(namePro, 20, ' ');
                data >> pricePro >> amountPro;
            }
            data.close();

            if(token == 1){
                std::cout << "This Product Code existed !";
                goto step_3;
            }
            else{
                data.open("../resource/product.txt", std::ios::app | std::ios::out);
                data << "\n" << ptr[i].ProductCode << " " << ptr[i].NameProduct << " "
                    << ptr[i].PriceProduct << " " << ptr[i].AmountProduct;
                data.close();
            }
        }
        std::cout<<"\n\n\t\t Record inserted !\n\n";
    }
    delete[] ptr;
}
void SmartEquipment::list(){
    system("cls");
    SmartEquipment Device;
    std::fstream data;
	data.open("../resource/product.txt", std::ios::in);
    if(!(data.is_open())) {
        return;
    }
	std::cout << "\n\n|\tProductCode\t|\tName\t|\tPrice\t|\tAmount\t|\n";
	data >> Device.ProductCode;
    data.get();
    data.getline(Device.NameProduct, 20, ' ');
    data >> Device.PriceProduct >> Device.AmountProduct;

    std::cout << "\n|\t" << Device.ProductCode << "\t\t|\t" << Device.NameProduct << "\t|\t"
            << Device.PriceProduct << "\t|\t" << Device.AmountProduct;

	while(!data.eof())
	{
        data >> Device.ProductCode;
        data.get();
        data.getline(Device.NameProduct, 20, ' ');
        data >> Device.PriceProduct >> Device.AmountProduct;

		std::cout << "\n|\t" << Device.ProductCode << "\t\t|\t" << Device.NameProduct << "\t|\t"
                << Device.PriceProduct << "\t|\t" << Device.AmountProduct;
	}
	data.close();
}
void SmartEquipment::buy() {
    system("cls");
    std::fstream data;
    SmartEquipment* Devices = nullptr;
    int c = 0;
    float total = 0, totalAll = 0;
    data.open("../resource/product.txt", std::ios::in);
    if(!(data.is_open())) {
        std::cout << "\n\n Empty database! See you next time ☆*: .｡. o(≧▽≦)o .｡.:*☆";
        system("pause");
    }
    else {
        data.close();                                   
        list();
        do {
            std::cout << "\n\t\tHow many types of Smart Devices do you want to buy?: ";
            std::cin >> c;
            std::cin.ignore();
        } while(c < 0);
        std::cout << "\n\t\t ____________________________"
                 <<  "\n\t\t|                            |"
                 <<  "\n\t\t|   Please place the order   |"
                 <<  "\n\t\t|____________________________|\n";
       
        Devices = new SmartEquipment[c];
        for(int i = 0; i < c; i++) {
            std::cout << "\n\nOrder Number " << i + 1;
            std::cout << "\n\n Product Code: ";
            std::cin >> Devices[i].ProductCode;
            std::cin.ignore();
            std::cout << "\n Product Quantity: ";
            std::cin >> Devices[i].AmountProduct;
            std::cin.ignore();

            bool check = getProCodeAndAmount(Devices[i]);
            if(!check) {
                std::cout << "Product Code doesn't exist or Product Quantity's wrong! Please try again\n";
                i--;
                continue;
            }
            for(int j = i - 1; j >= 0; j--) {
                if(Devices[i].ProductCode == Devices[j].ProductCode) {
                    std::cout << "\n\n Duplicate Product Code! Please try again";
                    i--;
					break;
                }
            }
        }
        std::cout << "\n\n\t\t\t__________RECEIPT______________\n";
        std::cout << "\nProduct Code\tProduct Name\tQuantity\tUnit Price\tTotal\n";
        
        for(int i = 0; i < c; i++) {
            total = Devices[i].PriceProduct * Devices[i].AmountProduct;
            totalAll += total;
            std::cout << "\n" << Devices[i].ProductCode << "\t" << Devices[i].NameProduct 
                    << "\t" << Devices[i].AmountProduct << "\t" << Devices[i].PriceProduct 
                    << "\t" << total;
        }
        std::cout << "\n\n----------------------------------------";
        std::cout << "\n Total All: "<< totalAll;

        delete[] Devices;
    }
}