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

bool buscarProducto(int codigo, Producto& producto, long& posicion) {
    FILE* archivo = fopen("inventario.dat", "rb");

    if (archivo == NULL) {
        return false;
    }

    fseek(archivo, 0, SEEK_END);
    long cantidadRegistros = ftell(archivo) / sizeof(Producto);

    long izquierda = 0;
    long derecha = cantidadRegistros - 1;

    while (izquierda <= derecha) {
        long medio = (izquierda + derecha) / 2;

        fseek(archivo, medio * sizeof(Producto), SEEK_SET);

        if (fread(&producto, sizeof(Producto), 1, archivo) != 1) {
            fclose(archivo);
            return false;
        }

        if (producto.codigo == codigo) {
            posicion = medio;
            fclose(archivo);
            return true;
        }

        if (producto.codigo < codigo) {
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }

    fclose(archivo);
    return false;
}

bool actualizarStock(long posicion, int cantidad) {
    FILE* archivo = fopen("inventario.dat", "rb+");

    if (archivo == NULL) {
        return false;
    }

    Producto producto;

    fseek(archivo, posicion * sizeof(Producto), SEEK_SET);

    if (fread(&producto, sizeof(Producto), 1, archivo) != 1) {
        fclose(archivo);
        return false;
    }

    if (producto.stockActual < cantidad) {
        fclose(archivo);
        return false;
    }

    producto.stockActual -= cantidad;

    fseek(archivo, posicion * sizeof(Producto), SEEK_SET);

    fwrite(&producto, sizeof(Producto), 1, archivo);

    fclose(archivo);

    return true;
}

int main() {
    char fecha[11], clave[20];
    int idMozo, codigoProducto, cantidad;

    cout << "Fecha (DD-MM-AAAA): ";
    cin >> fecha;

    cout << "ID del mozo: ";
    cin >> idMozo;

    cout << "Clave: ";
    cin >> clave;

    if (!validarMozo(idMozo, clave)) {
        cout << "Mozo o clave incorrectos." << endl;
        return 0;
    }

    cout << "Codigo del producto: ";
    cin >> codigoProducto;

    Producto producto;
    long posicion;

    if (buscarProducto(codigoProducto, producto, posicion)) {
        cout << "Producto: " << producto.descripcion << endl;
        cout << "Precio: $" << producto.precio << endl;
        cout << "Stock actual: " << producto.stockActual << endl;
        cout << "Cantidad: ";
        cin >> cantidad;

        if (cantidad > 0 && actualizarStock(posicion, cantidad)) {
            float comision = producto.precio * cantidad * TASA_COMISION;
            Comanda comanda;
            comanda.idMozo = idMozo;
            comanda.codigoProducto = codigoProducto;
            comanda.cantidad = cantidad;
            comanda.comision = comision;
            
            char nombreArchivo[40];
            strcpy(nombreArchivo, "comandas_");
            strcat(nombreArchivo, fecha);
            strcat(nombreArchivo, ".dat");
            
            FILE* archivo = fopen(nombreArchivo, "ab");

            if (archivo == NULL) {
                cout << "No se pudo abrir la planilla del dia." << endl;
                return 0;
            }
            fwrite(&comanda, sizeof(Comanda), 1, archivo);
            fclose(archivo);
            cout << "Venta registrada correctamente." << endl;
            cout << "Comision: $" << comision << endl;
        } else {
            cout << "No hay suficiente stock o la cantidad es invalida." << endl;
        }
    } else {
        cout << "El producto no existe." << endl;
    }
    return 0;
}
