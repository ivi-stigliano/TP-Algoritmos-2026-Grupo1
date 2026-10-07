#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

const int MAX_DIAS = 7; // tope de dias por semana

// registro de venta, igual al usado por ventas.cpp
struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

bool esBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

// cantidad de dias que tiene un mes (considera bisiesto simple)
int diasDelMes(int mes, int anio) {
    int dias[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && esBisiesto(anio)) {
        return 29;
    }
    return dias[mes - 1];
}

// avanza la fecha (dia, mes, anio) un dia, pasando de mes/anio si corresponde
void siguienteDia(int &dia, int &mes, int &anio) {
    dia++;
    if (dia > diasDelMes(mes, anio)) {
        dia = 1;
        mes++;
        if (mes > 12){ 
            mes = 1; 
            anio++; 
        }
    }
}

// Ej: valor = 6, cifras = 2 -> "06"

int escribirNumero(char destino[], int pos, int valor, int cifras) {
    for (int i = cifras - 1; i >= 0; i--) {
        destino[pos + i] = '0' + valor % 10;   // ultima cifra como caracter
        valor = valor / 10;                    // me quedo con el resto de cifras
    }
    return pos + cifras;
}
 
// "comandas_dd-mm-aaaa.dat"
void armarNombreDia(char nombre[], int dia, int mes, int anio) {
    int pos = 0;
    pos = escribirTexto(nombre, pos, "comandas_");
    pos = escribirNumero(nombre, pos, dia, 2);
    pos = escribirTexto(nombre, pos, "-");
    pos = escribirNumero(nombre, pos, mes, 2);
    pos = escribirTexto(nombre, pos, "-");
    pos = escribirNumero(nombre, pos, anio, 4);
    pos = escribirTexto(nombre, pos, ".dat");
    nombre[pos] = '\0';
}
 
// "comandas_semana_sX-mm.dat"
void armarNombreSemana(char nombre[], int semana, int mes) {
    int pos = 0;
    pos = escribirTexto(nombre, pos, "comandas_semana_s");
    if (semana < 10) {
        pos = escribirNumero(nombre, pos, semana, 1);
    } else {
        pos = escribirNumero(nombre, pos, semana, 2);
    }
    pos = escribirTexto(nombre, pos, "-");
    pos = escribirNumero(nombre, pos, mes, 2);
    pos = escribirTexto(nombre, pos, ".dat");
    nombre[pos] = '\0';
}

bool pedirDatos(int &dia, int &mes, int &anio, int &cantidadDias, int &semana, int &mesSemana) {

    cout << "Fecha del primer dia de la semana" << endl;
    cout << "  Dia: ";
    cin >> dia;
    cout << "  Mes: ";
    cin >> mes;
    cout << "  Anio: ";
    cin >> anio;
    cout << "Cantidad de dias a juntar (1 a " << MAX_DIAS << "): ";
    cin >> cantidadDias;
    cout << "Numero de semana (X): ";
    cin >> semana;
    cout << "Mes para el nombre del archivo (mm): ";
    cin >> mesSemana;
 
    if (cantidadDias < 1 || cantidadDias > MAX_DIAS) {
        cout << "Cantidad de dias invalida." << endl;
        return false;
    }
    if (mes < 1 || mes > 12 || dia < 1 || dia > diasDelMes(mes, anio)) {
        cout << "Fecha invalida." << endl;
        return false;
    }
    return true;
}
 //lee el siguiente registro de un archivo, devuelve true si lee, false si se termino

bool leerComanda(FILE *archivo, Comanda &comanda) {
    return fread(&comanda, sizeof(Comanda), 1, archivo) == 1;
}

int abrirPlanillas(FILE *dias[], Comanda actual[], bool hay[],
                   int cantidadDias, int dia, int mes, int anio) {
    int abiertos = 0;
    char nombre[40];
 
    for (int i = 0; i < cantidadDias; i++) {
        armarNombreDia(nombre, dia, mes, anio);
 
        dias[i] = fopen(nombre, "rb");
        hay[i] = false;
 
        if (dias[i] == NULL) {
            cout << "Sin planilla para " << nombre << " (se saltea)" << endl;

        } else {
            abiertos++;
            // si el archivo esta vacio, hay[i] queda en false
            hay[i] = leerComanda(dias[i], actual[i]);
        }
 
        siguienteDia(dia, mes, anio);
    }

    return abiertos;
}

int indiceMenor(Comanda actual[], bool hay[], int cantidad) {
    int menor = -1;
    for (int i = 0; i < cantidad; i++) {
        if (hay[i]) {
            if (menor == -1 || actual[i].idMozo < actual[menor].idMozo) {
                menor = i;
            }
        }
    }
    return menor;
}

int mezclar(FILE *dias[], Comanda actual[], bool hay[],
            int cantidadDias, FILE *salida) {
    int total = 0;
    int i = indiceMenor(actual, hay, cantidadDias);
 
    while (i != -1) {
        fwrite(&actual[i], sizeof(Comanda), 1, salida);
        total++;
        hay[i] = leerComanda(dias[i], actual[i]);
        i = indiceMenor(actual, hay, cantidadDias);
    }
    return total;
}


void cerrarPlanillas(FILE *dias[], int cantidadDias) {
    for (int i = 0; i < cantidadDias; i++) {
        if (dias[i] != NULL) {
            fclose(dias[i]);
        }
    }
}

int main() {
    int dia, mes, anio;
    int cantidadDias;
    int semana, mesSemana;
 
    if (!pedirDatos(dia, mes, anio, cantidadDias, semana, mesSemana)) {
        return 1;
    }
 
    FILE *dias[MAX_DIAS];       // un archivo por dia
    Comanda actual[MAX_DIAS];   // registro pendiente de cada archivo
    bool hay[MAX_DIAS];         // true si actual[i] tiene un registro valido
 
    int abiertos = abrirPlanillas(dias, actual, hay, cantidadDias, dia, mes, anio);
 
    if (abiertos == 0) {
        cout << "No se encontro ninguna planilla diaria. No se genera la semanal." << endl;
        return 1;
    }
 
    char nombreSemana[40];
    armarNombreSemana(nombreSemana, semana, mesSemana);
 
    FILE *salida = fopen(nombreSemana, "wb");
    if (salida == NULL) {
        cout << "No se pudo crear " << nombreSemana << endl;
        cerrarPlanillas(dias, cantidadDias);
        return 1;
    }
 
    int total = mezclar(dias, actual, hay, cantidadDias, salida);
 
    cerrarPlanillas(dias, cantidadDias);
    fclose(salida);
 
    cout << "Listo: " << nombreSemana << " generado con " << total
         << " ventas de " << abiertos << " dia(s)." << endl;
 
    return 0;
}
 