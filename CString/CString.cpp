#include "CString.h"

CString::CString() : pStr(new char[1]) {
    pStr[0] = '\0';
}

CString::CString(char* str) {
    pStr = new char[strlen(str) + 1];
    if (str == nullptr) {
        pStr[0] = '\0';
    }
    else {
        // копируем символы в pStr из str[] используя strcpy_s
        strcpy_s(pStr, strlen(str) + 1, str);
    }
}

int CString::getLength() {
    return strlen(pStr);
}

CString::CString(CString&& source) {
    pStr = source.pStr;
    source.pStr = nullptr;
}

CString::CString(const CString& source) {
    // копируем строку из источника
    pStr = new char[strlen(source.pStr) + 1];
    strcpy_s(pStr, strlen(source.pStr) + 1, source.pStr);
}

char* CString::GetCopy() {
    // если строки нету, то возвращаем пустую строку
    if (pStr == nullptr) {
        return new char[] {'\0'};
    }

    // создаем копию строки и возвращаем её
    char* ret = new char[strlen(pStr) + 1];
    strcpy_s(ret, strlen(pStr) + 1, pStr);
    return ret;
}

CString& CString::operator=(const CString& rhs) {
    // если текущий экземпляр класса эквивалентен правому классу,
    // то возвращаем обратно указатель на это же экземпляр класса
    if (this == &rhs) {
        return *this;
    }

    // удаляем текущую строку
    delete[] pStr;

    // копируем содержимое из правой части в текущий экземпляр класса
    pStr = new char[strlen(rhs.pStr) + 1];
    strcpy_s(pStr, strlen(rhs.pStr) + 1, rhs.pStr);

    // возвращаем обратно указатель на это же экземпляр класса
    return *this;
}

CString& CString::operator=(const char*& rhs) {
    // если строки эквивалентны,
    // то возвращаем обратно указатель на это же экземпляр класса
    if (strcmp(this->GetCopy(), rhs) == 0) {
        return *this;
    }

    // удаляем текущую строку
    delete[] pStr;

    // копируем содержимое из правой части в текущий экземпляр класса
    pStr = new char[strlen(rhs) + 1];
    strcpy_s(pStr, strlen(rhs) + 1, rhs);

    // возвращаем обратно указатель на это же экземпляр класса
    return *this;
}

// Переопределение оператора записи потока
std::istream& operator>>(std::istream& is, CString& obj) {
    /// А) железно выделим 1Кб для записи (не практично)
    /*
    char* buff = new char[1024];
    // Записываем из потока в буфер
    is >> buff;
    // устанавливаем для obj новый экземпляр класса с buff[]
    obj = CString{ buff };
    // удаляем временный buff[] 
    delete[] buff;
    */

    /// Б) автоматически выделять память (тоже не особо практично - нужен алгоритм...)

    int size = 1;
    // выделяем память для строки в 1 символ
    char* buff = (char*)malloc(size);

    // поулчаем символы из потока
    // и по нажатию на Enter (\n) ввод завершается
    for (char ch; is.get(ch) && ch != '\n';) {
        buff[size - 1] = ch;
        // увеличиваем размер буффера для нового символа
        buff = (char*)realloc(buff, ++size);
    }

    // если символы были введены,
    // то устанавливаем экземпляр класса с новой строкой
    if (size > 1) {
        buff[size - 1] = '\0';
        obj = CString{ buff };
    }

    // удаляем временный buff[] 
    delete[] buff;

    return is;
}

// Переопределение оператора вывода потока
std::ostream& operator<<(std::ostream& os, const CString& obj) {
    // выводим содержимое строки в поток
    os << obj.pStr;
    return os;
}

CString operator+(const CString& lhs, const CString& rhs) {
    // длина новой строки
    int length = strlen(lhs.pStr) + strlen(rhs.pStr);

    char* buff = new char[length + 1];

    // копируем левую строку в buff[]
    strcpy_s(buff, length + 1, lhs.pStr);

    // дописываем правую строку в buff[]
    strcat_s(buff, length + 1, rhs.pStr);

    // создаем временный класс для возврата с buff[]
    CString temp{ buff };

    // удаляем временный buff[] 
    delete[] buff;

    // возвращаем класс с объединенными строками 
    return temp;
}

CString& CString::WriteToFile(std::string path) {
    std::ofstream writer(path, std::ios::out);
    //writer.write(reinterpret_cast<char*>(pStr), getLength());
    writer << pStr;
    writer.close();

    return *this;
}

CString& CString::ReadFromFile(std::string path) {
    /*std::ifstream in(path);
    if (!in) {
        std::cout << "Cannot open input file\n";
        return *this;
    }
    int fsize = 1024;
    char* buff = (char*)malloc(fsize + 1);
    fread(string, fsize, 1, in);
    fclose(f);
    in.close();*/

    // Открываем файл
    FILE* file;
    if (fopen_s(&file, path.c_str(), "rb") != 0) {
        std::cout << "Cannot open input file\n";
        return *this;
    }

    // Переходим в конец файла
    fseek(file, 0, SEEK_END);
    // Получаем длину строки файла
    long length = ftell(file);
    // Возвращаем курсор в начало файла
    fseek(file, 0, SEEK_SET);

    // Выделяем память для буфера с нужным размером
    char* buff = (char*)malloc(length + 1);
    fread(buff, length, 1, file);
    fclose(file);

    buff[length] = 0;
    pStr = buff;

    return *this;
}
