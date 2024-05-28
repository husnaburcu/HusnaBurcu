#include <fstream>
#include <iostream>

using namespace std;

const int MAX_SIZE = 100; 

struct ArrayData {
    int arr[MAX_SIZE];
    int size;
};

bool writeArrayToFile(const ArrayData& data, const char* filename) {
    ofstream outfile(filename, ios::binary);
    if (!outfile.is_open()) {
        cerr << "Error: Could not open file for writing." << endl;
        return false;
    }
    outfile.write((char*)data.arr, sizeof(data.arr[0]) * data.size);
    outfile.close();
    cout << "Array written to file: " << filename << endl;
    return true;
}

bool readArrayFromFile(ArrayData& data, const char* filename) {
    ifstream infile(filename, ios::binary);
    if (!infile.is_open()) {
        cerr << "Error: Could not open file for reading." << endl;
        return false;
    }
    infile.seekg(0, ios::end);
    int file_size = infile.tellg();
    if (file_size % sizeof(data.arr[0]) != 0) {
        cerr << "Error: File format invalid (not a valid array data)." << endl;
        return false;
    }
    data.size = file_size / sizeof(data.arr[0]);
    infile.seekg(0, ios::beg);
    infile.read((char*)data.arr, sizeof(data.arr[0]) * data.size);
    infile.close();
    cout << "Array read from file: " << filename << endl;
    return true;
}

int findFirstOccurrence(const char* filename, int value) {
    ArrayData data;
    if (!readArrayFromFile(data, filename)) {
        return -1;
    }
    for (int i = 0; i < data.size; i++) {
        if (data.arr[i] == value) {
            return i;
        }
    }
    return -1;
}

int findLastOccurrence(const char* filename, int value) {
    ArrayData data;
    if (!readArrayFromFile(data, filename)) {
        return -1;
    }
    int last_index = -1;
    for (int i = data.size - 1; i >= 0; i--) {
        if (data.arr[i] == value) {
            last_index = i;
            break;
        }
    }
    return last_index;
}

bool replaceByIndex(const char* filename, int index, int new_value) {
    if (index < 0) {
        cerr << "Error: Invalid index." << endl;
        return false;
    }
    ArrayData data;
    if (!readArrayFromFile(data, filename)) {
        return false;
    }
    if (index >= data.size) {
        cerr << "Error: Index out of bounds." << endl;
        return false;
    }
    data.arr[index] = new_value;
    return writeArrayToFile(data, filename);
}

bool replaceByValue(const char* filename, int old_value, int new_value) {
    ArrayData data;
    if (!readArrayFromFile(data, filename)) {
        return false;
    }
    for (int i = 0; i < data.size; i++) {
        if (data.arr[i] == old_value) {
            data.arr[i] = new_value;
        }
    }
    return writeArrayToFile(data, filename);
}

int main() {
    int arr[] = { 1, 5, 3, 2, 5, 7 };
    int size = sizeof(arr) / sizeof(arr[0]);

    ArrayData data;
    data.size = size;
    for (int i = 0; i < size; i++) {
        data.arr[i] = arr[i];
    }

    if (!writeArrayToFile(data, "data.bin")) {
        return 1;
    }

    ArrayData new_data;
    if (!readArrayFromFile(new_data, "data.bin")) {
        return 1;
    }

    cout << "Read array: ";
    for (int i = 0; i < new_data.size; i++) {
        cout << new_data.arr[i] << " ";
    }
    cout << endl;

    int value = 5;
    int first_occurrence = findFirstOccurrence("data.bin", value);
    int last_occurrence = findLastOccurrence("data.bin", value);
    cout << "First occurrence of " << value << ": " << first_occurrence << endl;
    cout << "Last occurrence of " << value << ": " << last_occurrence << endl;

    int index = 2;
    int new_value = 10;
    if (replaceByIndex("data.bin", index, new_value)) {
        cout << "Replaced value at index " << index << " with " << new_value << endl;
    }

    int old_value = 3;
    new_value = 8;
    if (replaceByValue("data.bin", old_value, new_value)) {
        cout << "Replaced all occurrences of " << old_value << " with " << new_value << endl;
    }

    return 0;
}
