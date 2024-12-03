#include <iostream>
#include <string>
using namespace std;

// Máximo común divisor, para verificar si son coprimos (1)
int mcd(int a, int b) { 
    while (b != 0){ 
        int temp = b;
        b = a % b;
        a= temp;
    }
    return a;
}

int modInverse(int a, int m) {
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1; // No hay inverso si no se encuentra
}

string toUpperCase(const std::string& text) {
    string result;
    for (char c : text) {
        if (isalpha(c)) {
            result += toupper(c);
        } else {
            result += c; // Conservar otros caracteres (espacios)
        }
    }
    return result;
}

string cifrarAfin(const string& mensaje, int a, int b) {
    string cifrado;
    int m = 26; // Tamaño del alfabeto inglés

    for (char c : mensaje) {
        if (isalpha(c)) {
            char letra = toupper(c);
            int x = letra - 'A';
            int y = (a * x + b) % m;
            cifrado += 'A' + y;
        } else {
            cifrado += c; // Conservar espacios y otros caracteres
        }
    }

    return cifrado;
}

string descifrarAfin(const string& mensajeCifrado, int a, int b) {
    std::string descifrado;
    int m = 26;
    int a_inv = modInverse(a, m); // Inverso modular de 'a'

    if (a_inv == -1) {
        std::cerr << "El valor de 'a' no tiene inverso modular. Elija otro valor para 'a'." << std::endl;
        return "";
    }

    for (char c : mensajeCifrado) {
        if (isalpha(c)) {
            char letra = toupper(c);
            int y = letra - 'A';
            int x = (a_inv * (y - b + m)) % m;
            descifrado += 'A' + x;
        } else {
            descifrado += c; // Conservar espacios y otros caracteres
        }
    }

    return descifrado;
}

int main() {
    int a = 9; // Debe ser coprimo con 26
    int b = 5;

    // f(p) = (ap + b) mod 26   

    string mensaje = "HOLA";
    
    // Convertimos el mensaje a mayúsculas y eliminamos caracteres no alfabéticos (opcional)
    mensaje = toUpperCase(mensaje);

    // Cifrado
    string mensajeCifrado = cifrarAfin(mensaje, a, b);
    cout << "Mensaje cifrado: " << mensajeCifrado << endl;

    // Descifrado
    string mensajeDescifrado = descifrarAfin(mensajeCifrado, a, b);
    cout << "Mensaje descifrado: " << mensajeDescifrado << endl;

    return 0;
}
