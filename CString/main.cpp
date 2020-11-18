#include <iostream>
#include <cassert>
#include <windows.h>
#include "CString.h"

#pragma warning(disable : 4996)

// Дефайны для тестирования
#define __FILENAME__ "main.cpp"
#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_error(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILENAME__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_succsecc(M, ...) fprintf(stdout, "[SUCESS] (%s:%d) " M "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define assertf(A, M, ...) if(!(A)) { log_error(M, ##__VA_ARGS__); assert(A); } else { log_succsecc(M, ##__VA_ARGS__); }

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::cout << "Starting test interface..." << std::endl;

    char text[] = "Hello";
    /* test */ assertf(strlen(text) == 5, "Длина текста равна 5");

    CString str1{ text };
    std::cout << "1 => " << str1 << std::endl;
    /* test */ assertf(std::string(str1.GetCopy()) == text, "Текст идентичен");

    CString str2 = (char*)"Nope";
    str2 = (char*)" World";
    std::cout << "2 => " << str2 << std::endl;
    std::cout << "2 len => " << str2.getLength() << std::endl;

    CString str3{ str2 };
    std::cout << "3 => " << str3 << std::endl;

    CString str4{ CString { text } };
    std::cout << "4 => " << str4 << std::endl;
    /* test */ assertf(std::string(str4.GetCopy()) == text, "Текст идентичен");

    CString str5 = str1;
    std::cout << "5 => " << str5 << std::endl;
    std::cout << "5 len => " << str5.getLength() << std::endl;
    /* test */ assertf(str1.getLength() == str5.getLength(), "Длина одинакова");

    CString str6;
    std::cin >> str6;

    std::cout << "6 => " << str6 << std::endl;
    std::cout << "6 len => " << str6.getLength() << std::endl;

    CString str7 = str1 + str3 + CString{ (char*)": " } + str6;
    std::cout << "7 => " << str7 << std::endl;
    std::cout << "7 len => " << str7.getLength() << std::endl;
    /* test */ assertf(std::string(str7.GetCopy()) == std::string() + str1.GetCopy() + str3.GetCopy() + ": " + str6.GetCopy(), "Текст идентичен");

    str7.WriteToFile("HelloW.txt");

    CString str8;
    str8.ReadFromFile("HelloW.txt");

    std::cout << "8 => " << str8 << std::endl;
    std::cout << "8 len => " << str8.getLength() << std::endl;
    /* test */ assertf(std::string(str7.GetCopy()) == std::string(str8.GetCopy()), "Текст идентичен");

    return 0;
}
