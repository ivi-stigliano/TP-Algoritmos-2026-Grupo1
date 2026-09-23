#include <iostream>
#include <cstdio>

using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

int main() {
    FILE* archivo = fopen("comandas.dat", "rb");
    // abrir todos los archivos semanales.
    if (archivo == NULL) {
        cout << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    Comanda c;
    // apareo de archivos.
    float totalBuffet = 0.0f;

    int mozoactual = -1;
    int cantidadmozo = 0;
    float comisionmozo = 0.0f;
    // imprimir headers de tabla.
    while (fread(&c, sizeof(Comanda), 1, archivo) == 1) {
        if (mozoactual == -1) {
            mozoactual = c.idMozo;
        }

        if (mozoactual != c.idMozo) {
            //arreglar anchos para que coincidad con los headers de la tabla.
            cout << mozoactual << " " << cantidadmozo << " " << comisionmozo << endl;

            cantidadmozo = 0;
            comisionmozo = 0.0f;
            mozoactual = c.idMozo;
        }

        cantidadmozo += c.cantidad;
        comisionmozo += c.comision;
        totalBuffet += c.comision;
    }

    if (mozoactual != -1) {
        cout << mozoactual << " " << cantidadmozo << " " << comisionmozo << endl;
    }

    fclose(archivo);

    cout << "Total del buffet: " << totalBuffet << endl;

    return 0;
}