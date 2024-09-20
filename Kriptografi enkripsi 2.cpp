#include <iostream>
#include <vector>
#include <cctype>
#include <string>
#include <algorithm>
#include <sstream>
#include <algorithm>
using namespace std;

// Fungsi untuk mengubah huruf jadi kapital
string toUpperCase(string text) {
    for (int i = 0; i < text.length(); i++) {
        text[i] = toupper(text[i]);
    }
    return text;
}

// Fungsi untuk membersihkan teks (hapus spasi, non-alfabet)
string cleanText(string text) {
    string clean = "";
    for (int i = 0; i < text.length(); i++) {
        if (isalpha(text[i])) {
            clean += toupper(text[i]);
        }
    }
    return clean;
}

// Implementasi Vigenere Cipher
string vigenere(string text, string key) {
    text = cleanText(text);
    key = cleanText(key);
    string result = "";
    
    int j = 0;
    for (int i = 0; i < text.length(); i++) {
        char letter = text[i];
        char shift = key[j % key.length()] - 'A';
        char encrypted = ((letter - 'A' + shift) % 26) + 'A';
        result += encrypted;
        j++;
    }
    return result;
}

// Implementasi Playfair Cipher
string playfair(string text, string key) {
    text = cleanText(text);
    key = cleanText(key);
    
    // Matriks Playfair 5x5
    char table[5][5];
    string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ"; // I dan J digabung
    string matrixKey = "";

    // Hilangkan huruf ganda di kata kunci
    for (char c : key) {
        if (matrixKey.find(c) == string::npos) {
            matrixKey += c;
        }
    }
    
    // Tambahkan sisa huruf alfabet ke matriks
    for (char c : alphabet) {
        if (matrixKey.find(c) == string::npos) {
            matrixKey += c;
        }
    }

    // Isi matriks 5x5
    int index = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            table[i][j] = matrixKey[index++];
        }
    }
    
    // Pasangkan huruf
    string digraphs = "";
    for (int i = 0; i < text.length(); i += 2) {
        char first = text[i];
        char second = (i + 1 < text.length()) ? text[i + 1] : 'X';
        
        if (first == second) {
            second = 'X'; // Tambahkan 'X' jika huruf sama
        }
        
        digraphs += first;
        digraphs += second;
    }
    
    // Enkripsi digraphs
    string result = "";
    for (int i = 0; i < digraphs.length(); i += 2) {
        char a = digraphs[i];
        char b = digraphs[i + 1];
        int rowA, colA, rowB, colB;

        // Cari posisi di matriks
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 5; col++) {
                if (table[row][col] == a) {
                    rowA = row;
                    colA = col;
                }
                if (table[row][col] == b) {
                    rowB = row;
                    colB = col;
                }
            }
        }

        // Enkripsi berdasarkan aturan Playfair
        if (rowA == rowB) {
            result += table[rowA][(colA + 1) % 5];
            result += table[rowB][(colB + 1) % 5];
        } else if (colA == colB) {
            result += table[(rowA + 1) % 5][colA];
            result += table[(rowB + 1) % 5][colB];
        } else {
            result += table[rowA][colB];
            result += table[rowB][colA];
        }
    }
    
    return result;
}

// Hill Cipher
// Fungsi untuk mengalikan matriks
vector<int> multiplyMatrix(const vector<vector<int>>& key, const vector<int>& block) {
    vector<int> result(key.size(), 0);
    for (size_t i = 0; i < key.size(); ++i) {
        for (size_t j = 0; j < key[i].size(); ++j) {
            result[i] += key[i][j] * block[j];
        }
    }
    return result;
}

// Fungsi untuk mengenkripsi teks menggunakan Hill Cipher
string hill(const string& plaintext, const vector<vector<int>>& key) {
    string ciphertext = "";
    int blockSize = key[0].size();
    
    for (size_t i = 0; i < plaintext.length(); i += blockSize) {
        vector<int> block(key.size(), 0);
        for (size_t j = 0; j < blockSize; ++j) {
            if (i + j < plaintext.length()) {
                block[j] = toupper(plaintext[i + j]) - 'A';
            } else {
                block[j] = 0;
            }
        }
        
        vector<int> encryptedBlock = multiplyMatrix(key, block);
        for (size_t j = 0; j < encryptedBlock.size(); ++j) {
            ciphertext += (encryptedBlock[j] % 26) + 'A';
        }
    }
    
    return ciphertext;
}

// Fungsi untuk membuat matriks kunci dari kunci input
vector<vector<int>> keyMatrix(const string& kunciInput) {
    vector<vector<int>> key(3, vector<int>(3));  // Matriks 3x3
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int index = i * 3 + j;
            key[i][j] = toupper(kunciInput[index]) - 'A';
        }
    }
    return key;
}

int main() {
    string plaintext, kunciInput;
    int cipherChoice;
    
    cout << "Masukkan teks yang ingin dienkripsi: ";
    cin >> plaintext;

    cout << "Pilih metode enkripsi:\n1. Vigenere Cipher \n2. Playfair Cipher \n3. Hill Cipher \nPilihan Anda: ";
    cin >> cipherChoice;
    cin.ignore(); // Membersihkan buffer input
    
    int minKeyLength = (cipherChoice == 3) ? 9 : 12;

    while (true) {
        cout << "Masukkan Kunci (minimal " << minKeyLength << " karakter): ";
        cin >> kunciInput;
        // Validasi panjang kunci
        if (kunciInput.length() >= minKeyLength) {
            break;
        }
        cout << "Kunci terlalu pendek! Kunci minimal " << minKeyLength << " karakter." << endl;
    }
    
    // Memastikan kunci hanya terdiri dari huruf
    if (!all_of(kunciInput.begin(), kunciInput.end(), [](char c) { return isalpha(c); })) {
        cout << "Kunci harus berisi huruf." << endl;
        return 1;
    }

    switch (cipherChoice) {
        case 1: {
            string ciphertext = vigenere(plaintext, kunciInput);
            cout << "Hasil enkripsi Vigenere Cipher: " << ciphertext << endl;
            break;
        }
        case 2: {
            string ciphertext = playfair(plaintext, kunciInput);
            cout << "Hasil enkripsi Playfair Cipher: " << ciphertext << endl;
            break;
        }
        case 3: {
            // Membuat matriks kunci dari input kunci (3x3 untuk Hill Cipher)
            vector<vector<int>> key = keyMatrix(kunciInput);
            string ciphertext = hill(plaintext, key);
            cout << "Hasil enkripsi Hill Cipher: " << ciphertext << endl;
            break;
        }
        default:
            cout << "Pilihan tidak valid!" << endl;
    }
    system("pause");
    return 0;
}
