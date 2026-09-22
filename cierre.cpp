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
void sumarUnDia(int &dia, int &mes, int &anio) {
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

int escribirTexto(char destino[], int pos, const char texto[]) {
    int i = 0;
    while (texto[i] != '\0') {
        destino[pos] = texto[i];
        pos++;
        i++;
    }
    return pos;
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

bool pedirDatos(int &dia, int &mes, int &anio, int &cantidadDias,
                int &semana, int &mesSemana) {

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
 
bool leerComanda(FILE *archivo, Comanda &comanda) {
    return fread(&comanda, sizeof(Comanda), 1, archivo) == 1;
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
 