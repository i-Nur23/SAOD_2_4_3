#include <iostream>
#include <string>
#include <set>
#include <locale>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DBG_NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define newDBG_NEW

using namespace std;

const int m = 5;
int count = 0;

struct HashCell
{
    HashCell* next;
    string key;
};

struct HashHead
{
    string key;
    HashCell* begin;
    HashCell* end;
};

int CheckedInput(int begin, int end) // Ввод целочисленного значения с проверкой
{
    int choice;
    while (true)
    {
        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(32767, '\n');
            cout << "Вводите данные корректно: ";
            continue;
        }
        cin.ignore(32767, '\n');
        if (choice < begin || choice > end)
        {
            cout << "Вводите данные корректно: ";
            continue;
        }
        break;
    }
    return choice;
}

int GetHashCode(string key)
{
    int total = 0;
    for (int i = 0; i < key.length(); i++)
    {
        total += key[i] - '0';
    }
    return total % m;
}

void PrintHashTable(HashHead** array)
{
    HashCell* current;
    for (int i = 0; i < m; i++)
    {
        if (!(array[i]->key).empty())
        {
            cout << "Индекс: " << i << "\nКлючи:\n";
            cout << "\t" << array[i]->key << "\n";
            current = array[i]->begin;
            while (current != nullptr)
            {
                cout << "\t" << current->key << "\n";
                current = current->next;
            }
        }
    }
}

int Search(HashHead** array, string key, int& counter)
{
    HashCell* current;
    counter = 0;
    int index = GetHashCode(key);
    counter++;
    if (array[index]->key == key)
    {
        return index;
    }
    current = array[index]->begin;
    while(current != nullptr)
    {
        counter++;
        if (current->key == key)
        {
            return index;
        }
        current = current->next;
    }
    return -1;
}

void Add(HashHead** array, string key)
{
    int counter = 0;
    HashCell* temp = nullptr;
    int index = GetHashCode(key);

    counter++;
    if ((array[index]->key).empty())
    {
        array[index]->key = key;
        cout << "Ключ добавлен успешно. Число сравнений: " << counter;
        ::count++;
        return;
    }

    counter++;
    if (array[index]->key == key)
    {
        cout << "Данный ключ уже находится в ячейке. Число сравнений: " << counter;
        return;
    }

    counter++;
    if (array[index]->begin == nullptr)
    {
        array[index]->begin = array[index]->end = temp = new HashCell();
        temp->key = key;
        temp->next = nullptr;
        cout << "Ключ добавлен успешно. Число сравнений: " << counter;
        ::count++;
    }

    else
    {
        temp = new HashCell();
        temp->key = key;
        temp->next = nullptr;
        array[index]->end->next = temp;
        cout << "Ключ добавлен успешно. Число сравнений: " << counter;
        ::count++;
    }
}

void Delete(HashHead** array, string key)
{
    int index = GetHashCode(key);
    HashHead* cell = array[index];

    if (cell->key == key)
    {
        if (cell->begin == nullptr)
        {
            cell->key = "";
            ::count--;
        }
        else
        {
            cell->key = cell->begin->key;
            if (cell->end != cell->begin)
            {
                cell->begin = cell->begin->next;
                delete cell->begin;
                ::count--;
            }
            else
            {
                delete cell->begin;
                cell->begin = cell->end = nullptr;
                ::count--;
            }
        }
    }
    else if (cell->begin != nullptr)
    {
        HashCell* curr = cell->begin;
        if (curr->key == key)
        {
            if (curr->next == nullptr)
            {
                delete curr;
                cell->begin = cell->end = nullptr;
                ::count--;
            }
            else
            {
                cell->begin = curr->next;
                delete curr;
                ::count--;
            }
        }
        else
        {
            HashCell* prev = curr;
            curr = curr->next;
            while (curr->key != key)
            {
                prev = curr;
                curr = curr->next;
            }
            prev->next = curr->next;
            if (curr->next == nullptr)
            {
                cell->end = prev;
            }
            delete curr;
            ::count--;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "RUS");
    bool work = true;
    int choice;
    HashHead** array = new HashHead*[m];

    for (int i = 0; i < m; i++)
    {
        array[i] = new HashHead();
        array[i]->key = "";
        array[i]->begin = nullptr;
        array[i]->end = nullptr;
    }

    while (work)
    {
        cout << "\nВыборите действие:\n1 - Вывод хэш-таблицы\n2 - Поиск заданного ключа\n3 - Добавление ключа\n4 - Удаление ключа\n";
        cout << "5 - Завершение работы\n";
        cout << "Действие: "; choice = CheckedInput(1, 5);
        int service;
        int counter = 0;
        string key;
        HashCell *temp, *curr;
        switch (choice)
        {
        case 1:
            if (::count == 0)
            {
                cout << "Таблица пустая";
                cout << "\n\n";
                break;
            }
            cout << "Хэш-таблица: ";
            PrintHashTable(array);
            cout << "\n\n";
            break;
        case 2:
            if (::count == 0)
            {
                cout << "Таблица пустая";
                cout << "\n\n";
                break;
            }
            cout << "Введите ключ для поиска : ";
            getline(cin, key);
            service = Search(array, key, counter);
            if (service == -1)
            {
                cout << "Данного ключа в таблице нет. Число сравнений: " << counter;
            }
            else
            {
                cout << "Ключ найден. Число сравнений: " << counter;
            }
            cout << "\n\n";
            break;
        case 3:
            cout << "Введите ключ, который хотели бы добавить : ";
            getline(cin, key);
            Add(array, key);
            cout << "\n\n";
            break;
        case 4:
            if (::count == 0)
            {
                cout << "Таблица пустая";
                cout << "\n\n";
                break;
            }
            cout << "Введите ключ, который хотели бы удалить : ";
            getline(cin, key);
            if (Search(array, key, counter) == -1)
            {
                cout << "Такого элемента в таблице нет";
                cout << "\n\n";
                break;
            }
            Delete(array, key);
            cout << "\n\n";
            break;
        case 5:
            work = false;

            for (int i = 0; i < m; i++)
            {
                curr = array[i]->begin;
                while (curr != nullptr)
                {
                    temp = curr;
                    curr = curr->next;
                    delete temp;
                }
                delete array[i];
            }
            delete[] array;

            break;
        default:
            break;
        }
    }
    _CrtDumpMemoryLeaks();
}
