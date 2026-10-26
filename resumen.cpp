#include <iostream>
#include <cstdio>
#include <iomanip>

using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

bool leerComanda(FILE *archivo, Comanda &comanda) {
    return fread(&comanda, sizeof(Comanda), 1, archivo) == 1;
}

int concatenarSemanales(char mes[], const char nombreConcatenado[], int &archivosLeidos) {
    FILE *salida = fopen(nombreConcatenado, "wb");

    if (salida == NULL) {
        cout << "No se pudo crear " << nombreConcatenado << "." << endl;
        return -1;
    }

    int totalRegistros = 0;
    archivosLeidos = 0;
    Comanda comanda;

    char nombreSemana1[40];
    sprintf(nombreSemana1, "comandas_semana_s1-%s.dat", mes);
    FILE *semana1 = fopen(nombreSemana1, "rb");

    if (semana1 == NULL) {
        cout << "No se encontro " << nombreSemana1 << " (se saltea)." << endl;
    } else {
        archivosLeidos++;
        while (leerComanda(semana1, comanda)) {
            fwrite(&comanda, sizeof(Comanda), 1, salida);
            totalRegistros++;
        }
        fclose(semana1);
    }

    char nombreSemana2[40];
    sprintf(nombreSemana2, "comandas_semana_s2-%s.dat", mes);
    FILE *semana2 = fopen(nombreSemana2, "rb");

    if (semana2 == NULL) {
        cout << "No se encontro " << nombreSemana2 << " (se saltea)." << endl;
    } else {
        archivosLeidos++;
        while (leerComanda(semana2, comanda)) {
            fwrite(&comanda, sizeof(Comanda), 1, salida);
            totalRegistros++;
        }
        fclose(semana2);
    }

    char nombreSemana3[40];
    sprintf(nombreSemana3, "comandas_semana_s3-%s.dat", mes);
    FILE *semana3 = fopen(nombreSemana3, "rb");

    if (semana3 == NULL) {
        cout << "No se encontro " << nombreSemana3 << " (se saltea)." << endl;
    } else {
        archivosLeidos++;
        while (leerComanda(semana3, comanda)) {
            fwrite(&comanda, sizeof(Comanda), 1, salida);
            totalRegistros++;
        }
        fclose(semana3);
    }

    char nombreSemana4[40];
    sprintf(nombreSemana4, "comandas_semana_s4-%s.dat", mes);
    FILE *semana4 = fopen(nombreSemana4, "rb");

    if (semana4 == NULL) {
        cout << "No se encontro " << nombreSemana4 << " (se saltea)." << endl;
    } else {
        archivosLeidos++;
        while (leerComanda(semana4, comanda)) {
            fwrite(&comanda, sizeof(Comanda), 1, salida);
            totalRegistros++;
        }
        fclose(semana4);
    }

    char nombreSemana5[40];
    sprintf(nombreSemana5, "comandas_semana_s5-%s.dat", mes);
    FILE *semana5 = fopen(nombreSemana5, "rb");

    if (semana5 == NULL) {
        cout << "No se encontro " << nombreSemana5 << " (se saltea)." << endl;
    } else {
        archivosLeidos++;
        while (leerComanda(semana5, comanda)) {
            fwrite(&comanda, sizeof(Comanda), 1, salida);
            totalRegistros++;
        }
        fclose(semana5);
    }

    fclose(salida);

    return totalRegistros;
}

bool ordenarPorMozo(const char nombreArchivo[]) {
    FILE *archivo = fopen(nombreArchivo, "rb");

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

    Comanda *comandas = new Comanda[cantidadRegistros];

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

    bool guardado = fwrite(comandas, sizeof(Comanda), cantidadRegistros, archivo)
        == (size_t)cantidadRegistros;

    fclose(archivo);
    delete[] comandas;

    return guardado;
}

void imprimirResumen(const char nombreArchivo[]) {
    FILE *archivo = fopen(nombreArchivo, "rb");

    if (archivo == NULL) {
        cout << "No se pudo abrir " << nombreArchivo << "." << endl;
        return;
    }

    Comanda comanda;
    int totalProductos = 0;
    int mozoActual = -1;
    int cantidadMozo = 0;
    float comisionMozo = 0.0f;

    cout << fixed << setprecision(2);
    cout << setw(10) << "Mozo" << setw(14) << "Cantidad" << setw(14) << "Comision" << endl;
    cout << "--------------------------------------" << endl;

    while (leerComanda(archivo, comanda)) {
        if (mozoActual == -1) {
            mozoActual = comanda.idMozo;
        }

        if (mozoActual != comanda.idMozo) {
            cout << setw(10) << mozoActual << setw(14) << cantidadMozo << setw(14) << comisionMozo << endl;

            cantidadMozo = 0;
            comisionMozo = 0.0f;
            mozoActual = comanda.idMozo;
        }

        cantidadMozo += comanda.cantidad;
        comisionMozo += comanda.comision;
        totalProductos += comanda.cantidad;
    }

    if (mozoActual != -1) {
        cout << setw(10) << mozoActual << setw(14) << cantidadMozo << setw(14) << comisionMozo << endl;
    }

    fclose(archivo);

    cout << "--------------------------------------" << endl;
    cout << "Total de productos vendidos por el buffet: " << totalProductos << endl;
}

int main() {
    char mes[10];
    const char nombreConcatenado[] = "comandas.dat";

    cout << "=== RESUMEN MENSUAL ===" << endl;
    cout << "Mes (MM): ";
    cin >> mes;

    if (mes[2] != '\0'
        || !((mes[0] == '0' && mes[1] >= '1' && mes[1] <= '9')
        || (mes[0] == '1' && mes[1] >= '0' && mes[1] <= '2'))) {
        cout << "Mes invalido." << endl;
        return 1;
    }

    int archivosLeidos = 0;
    int registros = concatenarSemanales(mes, nombreConcatenado, archivosLeidos);

    if (registros < 0) {
        return 1;
    }

    if (archivosLeidos == 0) {
        cout << "No se encontro ninguna comanda semanal. No se genera resumen." << endl;
        return 1;
    }

    if (!ordenarPorMozo(nombreConcatenado)) {
        cout << "No se pudo ordenar " << nombreConcatenado << "." << endl;
        return 1;
    }

    cout << endl
         << nombreConcatenado << " generado con " << registros
         << " comandas de " << archivosLeidos << " archivo(s) semanal(es)." << endl
         << endl;

    imprimirResumen(nombreConcatenado);

    return 0;
}
