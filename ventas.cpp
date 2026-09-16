#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

const float TASA_COMISION = 0.10f;
const int K = 5;

struct Mozo {
    int idMozo;
    char nombre[50];
    char password[20];
    float totalComision;
};

struct Producto {
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

void encriptarClave(char clave[]) {
    for (int i = 0; clave[i] != '\0'; i++) {
        clave[i] = clave[i] + K;
    }
}

bool validarMozo(int idMozo, char clave[]) {
    FILE* archivo = fopen("mozos.dat", "rb");

    if (archivo == NULL) {
        return false;
    }

    encriptarClave(clave);

    Mozo mozo;

    while (fread(&mozo, sizeof(Mozo), 1, archivo) == 1) {
        if (mozo.idMozo == idMozo) {
            bool coincide = strcmp(mozo.password, clave) == 0;

            fclose(archivo);
            return coincide;
        }
    }

    fclose(archivo);
    return false;
}

int main() {
    char fecha[11];

    cout << "Ingrese la fecha (DD-MM-AAAA): ";
    cin >> fecha;

    int idMozo;
    char clave[20];

    cout << "Ingrese ID del mozo: ";
    cin >> idMozo;

    cout << "Ingrese clave: ";
    cin >> clave;

    if (validarMozo(idMozo, clave)) {
        cout << "Mozo validado correctamente." << endl;
    } else {
        cout << "Mozo o clave incorrectos." << endl;
    }

    return 0;
}
