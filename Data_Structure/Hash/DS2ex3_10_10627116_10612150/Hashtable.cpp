// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version

#include "Hashtable.h"
#include <iostream>
using namespace std;

Hashtable::Hashtable(int rowSize)
{
    rows = rowSize;
    setSize();
    successful = 0;

    // new table
    hashtable = new Table[size];
    for (int i = 0; i < size; i++)
        hashtable[i].hashcode = -1;
}

void Hashtable::clear() { delete hashtable; }

void Hashtable::setSize() { size = findPrimeMoreThan(rows * 1.2); }

void Hashtable::setMaxStep() { maxStep = 1; }

int Hashtable::getStep(char *str, int collisions)
{
    return maxStep * collisions;
}

int Hashtable::hash(char *str, int num)
{
    int hashkey = 1;
    while (*str != '\0')
        hashkey = (hashkey * *str) % num, str++;

    return hashkey;
}

void Hashtable::insert(Data &data)
{
    int hash_index = hash(data.getId(), size);
    int index = hash_index;
    int step;
    int collisions = 0;

    // address not empty
    while (hashtable[index].hashcode != -1) {
        collisions++;
        step = getStep(data.getId(), collisions);
        index = (hash_index + step) % size;

        if (collisions > size * 4)
            throw "Timeout";
    }

    successful += 1 + collisions;
    // set data
    hashtable[index].hashcode = hash_index;
    hashtable[index].column = data.getColumn();
}

ostream &operator<<(ostream &out, Table &table)
{
    if (table.hashcode != -1) {
        out << setw(10) << table.hashcode << ", " << setw(10)
            << table.column.sid << ", " << setw(10) << table.column.sname
            << ", " << setw(10) << table.column.average;
    }

    return out;
}

void Hashtable::save(fstream &fout)
{

    for (int i = 0; i < size; i++) {
        fout << '[' << setw(3) << i << "] " << hashtable[i] << endl;

        int count = 0, index = i;
        while (hashtable[index].hashcode != -1) {
            count++;
            index = (i + getStep(hashtable[index].column.sid, count)) % size;
        }

        unsuccessful += count;
    }

    successful /= double(rows);
    unsuccessful /= double(size);
}

void Hashtable_Linear::save(fstream &fout)
{
    fout << " --- Hash Table X --- (linear probing)" << endl;
    Hashtable::save(fout); // pair <success, unsuccess>
    cout << "Hash Table X has been created." << endl;
    cout << "unsuccessful search: " << fixed << setprecision(4) << unsuccessful
         << " comparisons on average" << endl;
    cout << "successful search: " << fixed << setprecision(4) << successful
         << " comparisons on average" << endl;
}

void Hashtable_Quadratic::save(fstream &fout)
{
    fout << " --- Hash Table Z --- (quadratic probing)" << endl;
    Hashtable::save(fout); // pair <success, unsuccess>
    cout << "Hash Table Z has been created." << endl;
    cout << "unsuccessful search: " << fixed << setprecision(4) << unsuccessful
         << " comparisons on average" << endl;
    cout << "successful search: " << fixed << setprecision(4) << successful
         << " comparisons on average" << endl;
}

int Hashtable_Quadratic::getStep(char *str, int collisions)
{
    return collisions * collisions;
}

void Hashtable_Double::setMaxStep() { maxStep = findPrimeMoreThan(rows / 3.); }

int Hashtable_Double::getStep(char *str, int collisions)
{
    return (maxStep - hash(str, maxStep)) * collisions;
}

void Hashtable_Double::save(fstream &fout)
{
    fout << " --- Hash Table Y --- (double hashing)" << endl;
    Hashtable::save(fout); // pair <success, unsuccess>
    cout << "Hash Table Y has been created." << endl;
    cout << "successful search: " << fixed << setprecision(4) << successful
         << " comparisons on average" << endl;
}
