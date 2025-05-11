#pragma once
#include <iostream>
#include <string>
#include "DateTime.hpp"

class Guest
{
private:
    u_int64_t id;
    std::string firstName;
    std::string secondName;
    std::string phone;
    std::string email;
    u_int64_t passport;
    DateTime date;
    u_int loyalPoint;

public:
    Guest(
        u_int64_t id,
        std::string fname,
        std::string sname,
        std::string phone,
        std::string email,
        u_int64_t passport,
        DateTime date,
        u_int loyalPoint)
    {
        this->id = id;
        this->firstName = fname;
        this->secondName = sname;
        this->phone = phone;
        this->email = email;
        this->passport = passport;
        this->date = date;
        this->loyalPoint = loyalPoint;
    }

    void setFirstName(std::string name)
    {
        this->firstName = name; // this == self
    }

    void setSecondName(std::string name)
    {
        secondName = name;
    }

    void setPhone(std::string phoneNum)
    {
        phone = phoneNum;
    }

    void setPassport(u_int64_t passportNum)
    {
        passport = passportNum;
    }

    std::string getFullName()
    {
        return firstName + secondName;
    }

    void showInfo()
    {
        std::cout << "ID: " << this->id << std::endl;
        std::cout << "Имя: " << this->firstName << std::endl;
        std::cout << "Фамилия: " << this->secondName << std::endl;
        std::cout << "Номер телефона: " << this->phone << std::endl;
        std::cout << "Email: " << this->email << std::endl;
        std::cout << "Номер паспорта: " << this->passport << std::endl;
        std::cout << "Дата рождения: " << this->date.toString() << std::endl;
        std::cout << "Бонусы: " << this->loyalPoint << std::endl;
        std::cout << std::endl;
    }

    u_int64_t getId() const
    {
        return id;
    }
    std::string getFirstName() const
    {
        return firstName;
    }
    std::string getSecondName() const
    {
        return secondName;
    };
    std::string getPhone() const
    {
        return phone;
    };
    std::string getEmail() const
    {
        return email;
    }
};
