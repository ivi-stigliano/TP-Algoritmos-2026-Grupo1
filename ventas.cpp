#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

const float TASA_COMISION = 0.10f;
const int K = 3;

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

bool actualizarComisionMozo(int idMozo, float comision) {
    FILE* archivo = fopen("mozos.dat", "rb+");

    if (archivo == NULL) {
        return false;
    }

    Mozo mozo;

    while (fread(&mozo, sizeof(Mozo), 1, archivo) == 1) {
        if (mozo.idMozo == idMozo) {
            mozo.totalComision += comision;

            fseek(archivo, -(long)sizeof(Mozo), SEEK_CUR);
            fwrite(&mozo, sizeof(Mozo), 1, archivo);

            fclose(archivo);
            return true;
        }
    }

    fclose(archivo);
    return false;
}

void generarNombreArchivo(char fecha[], char nombreArchivo[]) {
    strcpy(nombreArchivo, "comandas_");
    strcat(nombreArchivo, fecha);
    strcat(nombreArchivo, ".dat");
}

bool guardarComanda(char nombreArchivo[], Comanda& comanda) {
    FILE* archivo = fopen(nombreArchivo, "ab");

    if (archivo == NULL) {
        return false;
    }

    bool guardado = fwrite(&comanda, sizeof(Comanda), 1, archivo) == 1;

    fclose(archivo);
    return guardado;
}

bool ordenarPorMozo(char nombreArchivo[]) {
    FILE* archivo = fopen(nombreArchivo, "rb");

    if (archivo == NULL) {
        return false;
    }

    fseek(archivo, 0, SEEK_END);
    long cantidadRegistros = ftell(archivo) / sizeof(Comanda);

    if (cantidadRegistros <= 1) {
        fclose(archivo);
        return true;
    }

    fseek(archivo, 0, SEEK_SET);

    Comanda* comandas = new Comanda[cantidadRegistros];

    if (fread(comandas, sizeof(Comanda), cantidadRegistros, archivo)
        != (size_t)cantidadRegistros) {
        delete[] comandas;
        fclose(archivo);
        return false;
    }

    fclose(archivo);

    for (long i = 0; i < cantidadRegistros - 1; i++) {
        long posicionMenor = i;

        for (long j = i + 1; j < cantidadRegistros; j++) {
            if (comandas[j].idMozo < comandas[posicionMenor].idMozo) {
                posicionMenor = j;
            }
        }

        if (posicionMenor != i) {
            Comanda auxiliar = comandas[i];
            comandas[i] = comandas[posicionMenor];
            comandas[posicionMenor] = auxiliar;
        }
    }

    archivo = fopen(nombreArchivo, "wb");

    if (archivo == NULL) {
        delete[] comandas;
        return false;
    }

    bool guardado = fwrite(
        comandas,
        sizeof(Comanda),
        cantidadRegistros,
        archivo
    ) == (size_t)cantidadRegistros;

    fclose(archivo);
    delete[] comandas;

    return guardado;
}

int main() {
    char fecha[11], clave[20] ;
    char continuar;
    int idMozo, codigoProducto, cantidad;

    cout << "Fecha (DD-MM-AAAA): ";
    cin >> fecha;

    char nombreArchivo[40];
    generarNombreArchivo(fecha, nombreArchivo);

    do {
        cout << "\nID del mozo: ";
        cin >> idMozo;

        cout << "Clave: ";
        cin >> clave;

        if (!validarMozo(idMozo, clave)) {
            cout << "Mozo o clave incorrectos." << endl;
        } else {
            cout << "Codigo del producto: ";
            cin >> codigoProducto;

            Producto producto;
            long posicion;

            if (!buscarProducto(codigoProducto, producto, posicion)) {
                cout << "El producto no existe." << endl;
            } else {
                cout << "Producto: " << producto.descripcion << endl;
                cout << "Precio: $" << producto.precio << endl;
                cout << "Stock actual: " << producto.stockActual << endl;

                cout << "Cantidad: ";
                cin >> cantidad;

                if (cantidad <= 0) {
                    cout << "La cantidad debe ser mayor a cero." << endl;
                } else if (!actualizarStock(posicion, cantidad)) {
                    cout << "No hay suficiente stock." << endl;
                } else {
                    float comision = producto.precio * cantidad * TASA_COMISION;

                    Comanda comanda;
                    comanda.idMozo = idMozo;
                    comanda.codigoProducto = codigoProducto;
                    comanda.cantidad = cantidad;
                    comanda.comision = comision;

                    if (!guardarComanda(nombreArchivo, comanda)) {
                        cout << "No se pudo guardar la venta." << endl;
                    } else if (!actualizarComisionMozo(idMozo, comision)) {
                        cout << "No se pudo actualizar la comision del mozo." << endl;
                    } else {
                        cout << "Venta registrada correctamente." << endl;
                        cout << "Comision: $" << comision << endl;
                    }
                }
            }
        }

        cout << "\nQuiere cargar otra venta? (S/N): ";
        cin >> continuar;

    } while (continuar == 'S' || continuar == 's');

    if (ordenarPorMozo(nombreArchivo)) {
        cout << "\nPlanilla ordenada por mozo." << endl;
    } else {
        cout << "\nNo se pudo ordenar la planilla." << endl;
    }

    return 0;
}
