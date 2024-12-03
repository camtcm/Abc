#include <iostream>
#include <cstring>
using namespace std;

void multiplicarMatrices(char** A, int filasA, int columnasA,
    char** B, int filasB, int columnasB,
    char**& C) {
    if (columnasA != filasB) {
        cerr << "No se puede multiplicar matrices de: ("
            << filasA << "," << columnasA << ") x ("
            << filasB << "," << columnasB << ")\n";
        return;
    }

    C = new char* [filasA];
    for (int i = 0; i < filasA; ++i) {
        C[i] = new char[columnasB]();  
        for (int j = 0; j < columnasB; ++j) {
            for (int k = 0; k < columnasA; ++k) {
                C[i][j] ^= (A[i][k] & B[k][j]);
            }
        }
    }
}

int distancia(const char* a, const char* b) {
    int len = strlen(a);
    if (len != strlen(b)) {
        cerr << "Las cadenas deben ser de la misma longitud\n";
        return -1;
    }

    int diff = 0;
    for (int i = 0; i < len; ++i) {
        if (a[i] != b[i]) diff++;
    }
    return diff;
}

int distanciaMinima(const char** grupo, int size) {
    if (size < 2) {
        cerr << "Se requiere al menos dos elementos\n";
        return -1;
    }

    int minDistancia = distancia(grupo[0], grupo[1]);
    if (minDistancia < 0) return -1;

    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            int dist = distancia(grupo[i], grupo[j]);
            if (dist >= 0 && dist < minDistancia) {
                minDistancia = dist;
            }
        }
    }

    return minDistancia;
}

void transponerMatriz(char** A, int filasA, int columnasA, char**& AT) {
    AT = new char* [columnasA];
    for (int j = 0; j < columnasA; ++j) {
        AT[j] = new char[filasA];
        for (int i = 0; i < filasA; ++i) {
            AT[j][i] = A[i][j];
        }
    }
}

void crearIdentidad(int n, char**& I) {
    I = new char* [n];
    for (int i = 0; i < n; ++i) {
        I[i] = new char[n]();
        I[i][i] = 1;
    }
}

void generarMatrizH(char** A, int filasA, int columnasA, char**& H) {
    char** AT, ** I;
    transponerMatriz(A, filasA, columnasA, AT);
    crearIdentidad(filasA, I);

    int filasAT = columnasA;
    int columnasAT = filasA;

    H = new char* [filasAT];
    for (int i = 0; i < filasAT; ++i) {
        H[i] = new char[columnasAT + filasAT]();

        // Copy A^T
        for (int j = 0; j < columnasAT; ++j) {
            H[i][j] = AT[i][j];
        }

        // Copy I
        for (int j = 0; j < filasAT; ++j) {
            H[i][j + columnasAT] = I[i][j];
        }
    }

    // Free temporary matrices
    for (int i = 0; i < columnasA; ++i) delete[] AT[i];
    delete[] AT;
    for (int i = 0; i < filasA; ++i) delete[] I[i];
    delete[] I;
}

void imprimirMatriz(char** matriz, int filas, int columnas) {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            cout << (int)matriz[i][j] << " ";
        }
        cout << endl;
    }
}

void generarMatrizG(char** A, int filasA, int columnasA, char**& G) {
    char** I;
    crearIdentidad(filasA, I);

    G = new char* [filasA];
    for (int i = 0; i < filasA; ++i) {
        G[i] = new char[columnasA + filasA]();

        for (int j = 0; j < filasA; ++j) {
            G[i][j] = I[i][j];
        }
        // Copy A^T
        for (int j = 0; j < columnasA; ++j) {
            G[i][j + columnasA] = A[i][j];
        }

    }

    // Free temporary matrices
    for (int i = 0; i < filasA; ++i) delete[] I[i];
    delete[] I;
}

void codificar(char** G, int filasG, int columnasG, char** w, char**& c) {
    multiplicarMatrices(w, 1, filasG, G, filasG, columnasG, c);
}

void decodificar(char** H, int filasH, int columnasH, char** c, char**& d) {
    char** CT;
    transponerMatriz(c, 1, columnasH, CT);
    multiplicarMatrices(H, filasH, columnasH, CT, columnasH, 1, d);
}

int main() {
    const char* grupo[] = { "000000", "101010", "010101", "111111" };
    int size = sizeof(grupo) / sizeof(grupo[0]);

    int minDist = distanciaMinima(grupo, size);
    if (minDist != -1) {
        cout << "Minimum distance: " << minDist << endl;
    }

    int filasA = 3, columnasA = 3;
    int filasG = filasA, columnasG = filasA + columnasA;
    char** A = new char* [filasA];
    A[0] = new char[columnasA] {1, 1, 0};
    A[1] = new char[columnasA] {0, 1, 1};
    A[2] = new char[columnasA] {1, 0, 1};

    char** G;
    generarMatrizG(A, filasA, columnasA, G);

    char** H;
    generarMatrizH(A, filasA, columnasA, H);

    cout << "Matrix A:" << endl;
    imprimirMatriz(A, filasA, columnasA);

    cout << "\nMatrix G = [I | A]:" << endl;
    imprimirMatriz(G, filasA, columnasA + filasA);

    cout << "\nMatrix H = [A^T | I]:" << endl;
    imprimirMatriz(H, columnasA, columnasA + filasA);

    cout << "\nCodificacion:\n";
    char** c;
    char** w = new char* [1];
    w[0] = new char[columnasA] {1, 0, 1};

    imprimirMatriz(w, 1, columnasA);
    codificar(G, filasG, columnasG, w, c);
    imprimirMatriz(c, 1, columnasG);

    cout << "\nDecodificacion:\n";
    char** d;
    c = new char* [1];
    c[0] = new char[columnasG] {1, 1, 0, 1, 1, 0};

    decodificar(H, columnasA, columnasA + filasA, c, d);
    imprimirMatriz(d, columnasA, 1);

    for (int i = 0; i < filasA; ++i) {
        delete[] A[i];
        delete[] H[i];
    }
    delete[] A;
    delete[] H;


    return 0;
}