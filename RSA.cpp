
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


void imprimir_divisores(int n) {
    cout << "Los divisores de " << n << " son: ";
    
    // Iteramos hasta la raíz cuadrada del número
    for(int i = 1; i * i <= n; i++) {
        if(n % i == 0) {
            cout << i << " ";  // Imprime el divisor menor
            
            // Si i² != n, imprime el divisor complementario
            if(i * i != n) {
                cout << n/i << " ";
            }
        }
    }
    cout << endl;
}


// Función para calcular el máximo común divisor
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Función para calcular la exponenciación modular rápida
int expMod(int base, int exponente, int mod) {
    int resultado = 1;
    base = base % mod;
    while (exponente > 0) {
        if (exponente % 2 == 1) { // Si el exponente es impar
            resultado = (resultado * base) % mod;
        }
        exponente = exponente >> 1; // Dividimos el exponente por 2
        base = (base * base) % mod;
    }
    return resultado;
}

// Función para calcular el inverso modular
int modInverso(int a, int m) {
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // Si no hay inverso, devuelve -1
}

// Función para verificar si un número es primo
bool esPrimo(int num) {
    if (num <= 1) return false; // Los números menores o iguales a 1 no son primos
    for (int i = 2; i < num; i++) { // Comprobamos si es divisible por cualquier número menor que num
        if (num % i == 0) return false; // Si es divisible, no es primo
    }
    return true; // Si no es divisible por ningún número, es primo
}

// Función para generar los primos p y q aleatorios
void generarPrimos(int &p, int &q) {
    int a[2];
    int i = 0;
    while(i < 2) {
        int random = rand() % 40 + 3; // Genera un número aleatorio entre 3 y 42
        if(esPrimo(random)) {
            if(i == 0 || random != a[0]) {
                a[i] = random;
                i++;
            }
        }
    }

    p = a[0];
    q = a[1];
}

// Generación de claves RSA con selección de e aleatoria y p, q aleatorios
void generarClaves(int &n, int &e, int &d) {
    srand(time(0));

    int p, q;
    generarPrimos(p, q);  // Generación de p y q aleatorios

    n = p * q;
    int phi = (p - 1) * (q - 1);

    // Generar valores aleatorios para e que sean coprimos con phi
    do {
        e = 3 + rand() % (phi - 3); // Genera un valor entre 3 y phi - 1
    } while (gcd(e, phi) != 1); // Verificar que e y phi sean coprimos

    // Calcular el inverso modular de e para obtener d
    d = modInverso(e, phi);

    // Comprobar que el inverso modular exista
    if (d == -1) {
        cerr << "Error: No se encontró un inverso modular para e. Intente con otros valores de p y q.\n";
        exit(1);
    }

    cout << "Valores generados:\n";
    cout << "p: " << p << ", q: " << q << ", n: " << n << ", phi: " << phi << endl;
}

// Función para cifrar un carácter usando RSA
int cifrar(int mensaje, int e, int n) {
    return expMod(mensaje, e, n);
}

// Función para descifrar un carácter usando RSA
int descifrar(int cifrado, int d, int n) {
    return expMod(cifrado, d, n);
}

int main() {
    int n, e, d;

    generarClaves(n, e, d); // Generación de claves RSA
    cout << "Claves generadas:\n";
    cout << "n: " << n << ", e: " << e << ", d: " << d << endl;

    string mensaje;
    cout << "Ingrese un mensaje para cifrar: ";
    getline(cin, mensaje);

    // Cifrado del mensaje
    cout << "Mensaje cifrado: ";
    for (char c : mensaje) {
        int cifrado = cifrar(int(c), e, n);
        cout << cifrado << " ";
    }
    cout << endl;

    // Descifrado del mensaje
    cout << "Mensaje descifrado: ";
    for (char c : mensaje) {
        int cifrado = cifrar(int(c), e, n);
        char descifrado = char(descifrar(cifrado, d, n));
        cout << descifrado;
    }
    cout << endl;

    return 0;
}