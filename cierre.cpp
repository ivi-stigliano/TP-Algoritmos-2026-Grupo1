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

// cantidad de dias que tiene un mes (considera bisiesto simple)
int diasDelMes(int mes, int anio) {
    int dias[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (mes == 2 && anio % 4 == 0) return 29;
    return dias[mes - 1];
}

// avanza la fecha (dia, mes, anio) un dia, pasando de mes/anio si corresponde
void sumarUnDia(int &dia, int &mes, int &anio) {
    dia++;
    if (dia > diasDelMes(mes, anio)) {
        dia = 1;
        mes++;
        if (mes > 12) { mes = 1; anio++; }
    }
}

int main() {
    int diaIni, mesIni, anioIni, cantidadDias;
    int semana, mesSemana;

    // datos que ingresa el usuario: rango de dias a unir y datos del archivo semanal de salida
    cout << "Fecha del primer dia (DD MM AAAA): ";
    cin >> diaIni >> mesIni >> anioIni;
    cout << "Cantidad de dias a juntar (max " << MAX_DIAS << "): ";
    cin >> cantidadDias;
    cout << "Numero de semana: ";
    cin >> semana;
    cout << "Mes de la semana (para el nombre del archivo): ";
    cin >> mesSemana;

    if (cantidadDias > MAX_DIAS) cantidadDias = MAX_DIAS; // por las dudas, no pasar el tope

    // un FILE*, un candidato (proximo registro a comparar) y su validez, por cada dia
    FILE* archivos[MAX_DIAS];
    Comanda candidato[MAX_DIAS];
    bool valido[MAX_DIAS];

    int dia = diaIni, mes = mesIni, anio = anioIni;

    // abre cada archivo del rango y carga su primer registro (si el archivo existe)
    for (int i = 0; i < cantidadDias; i++) {
        char nombre[40];
        sprintf(nombre, "comandas_%02d-%02d-%04d.dat", dia, mes, anio);

        archivos[i] = fopen(nombre, "rb");
        if (archivos[i] != NULL) {
            valido[i] = (fread(&candidato[i], sizeof(Comanda), 1, archivos[i]) == 1);
        } else {
            valido[i] = false; // ese dia no se trabajo y no genero archivo: no participa del merge
        }

        sumarUnDia(dia, mes, anio); // paso a la fecha siguiente para la proxima vuelta
    }

    // preguntar mozo x mozo (siguiente parte)

    return 0;
}