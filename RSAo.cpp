#include <iostream>
#include <string>
#include <ctime>
using namespace std;

int mcd(int num1, int num2) {
    while (num2 != 0) {
        int temp = num2;
        num2 = num1 % num2;
        num1 = temp;
    }
    return num1;
}

bool esPrimo(int numero) {
    if (numero <= 1)
        return false;

    for (int i = 2; i * i <= numero; i++) {
        if (numero % i == 0) {
            return false;
        }
    }

    return true;
}

// int generarPrimo(int limiteInferior, int limiteSuperior) {
//     int primo = 0;
//     while (!esPrimo(primo)) {
//         primo = rand() % limiteSuperior + limiteInferior; // rango de números + inferior
//     }
//     return primo;
// }

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

int modExp(int base, int exponente, int modulo) {
    int resultado = 1;
    base = base % modulo;

    while (exponente > 0) {
        if (exponente % 2 == 1) {
            resultado = (resultado * base) % modulo; 
        }
        exponente = exponente >> 1;
        base = (base * base) % modulo;
    }
    return resultado;
}

int inversoModular(int numero, int modulo) {
    if (!mcd(numero,modulo)) {
        cout << "inversoModular no existe porque el mcd(" << numero << "," << modulo << ") = " << mcd(numero,modulo) << endl;
        return -1;
    }

    int x = 0, y = 1;
    int moduloCpy = modulo;

    while (numero > 1) {
        int cociente = numero / modulo;
        int temp = modulo;

        modulo = numero % modulo;
        numero = temp;

        int tempX = x;
        x = y - cociente * x;
        y = tempX;
    }

    if (y < 0) {
        y += moduloCpy;
    }

    return y;
}

void generarClaves(int &n, int &e, int &d) {
    srand(time(0));

    int p, q;
    generarPrimos(p,q);

    n = p * q;

    int totiente = (p - 1) * (q - 1);

    do {
        e = 3 + rand() % (totiente - 3) ; // 2 < e < totiente
    } while (mcd(e, totiente) != 1);

    d = inversoModular(e, totiente);
}

string cifrarMensaje(string mensaje, int e, int n) {
    string mensajeCifrado = "";
    
    for (char caracter : mensaje) {
        // Cifrar cada carácter
        int valorCifrado = modExp((int)caracter, e, n);
        
        // Convertir el valor cifrado a cadena
        mensajeCifrado += to_string(valorCifrado) + " ";
    }
    
    return mensajeCifrado;
}

string descifrarMensaje(string mensajeCifrado, int d, int n) {
    string mensajeDescifrado = "";
    string bloqueActual = "";
    
    for (char caracter : mensajeCifrado) {
        if (caracter != ' ') {
            bloqueActual += caracter;
        } else {
            // Convertir bloque a entero
            int valorCifrado = stoi(bloqueActual);
            
            // Descifrar el valor
            int valorDescifrado = modExp(valorCifrado, d, n);
            
            // Agregar carácter descifrado
            mensajeDescifrado += (char)valorDescifrado;
            
            // Resetear bloque
            bloqueActual = "";
        }
    }
    
    // Procesar el último bloque si existe
    if (!bloqueActual.empty()) {
        int valorCifrado = stoi(bloqueActual);
        int valorDescifrado = modExp(valorCifrado, d, n);
        
        // Verificar el rango del valor descifrado
        if (valorDescifrado < 0 || valorDescifrado > 255) {
            valorDescifrado = valorDescifrado & 0xFF; // Máscara para 8 bits
        }
        
        mensajeDescifrado += (char)valorDescifrado;
    }
    
    return mensajeDescifrado;
}


int main() {
    int n, e, d;    
    generarClaves(n,e,d);
    cout << "Clave encriptacion: (" << n << ", " << e <<")\n";
    cout << "Clave desencriptacion: (" << d << ", " << e <<")\n";

    string mensaje = "hola mALDITOS cucucucu";
    cout << "\nMensaje original: " << mensaje;

    string mensajeCifrado = cifrarMensaje(mensaje, e, n);
    cout << "\nMensaje cifrado: " << mensajeCifrado;
    
    // Descifrar mensaje
    string mensajeDescifrado = descifrarMensaje(mensajeCifrado, d, n);
    cout << "\nMensaje descifrado: " << mensajeDescifrado << endl;


}