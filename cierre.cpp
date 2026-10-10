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
    int dias[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return dias[mes - 1];
    }

bool calcularSemana(int mes, int semana, int &diaInicio, int &cantDias) {
    int totalDias = diasDelMes(mes);
 
    diaInicio = (semana - 1) * MAX_DIAS_SEMANA + 1;
    if (diaInicio > totalDias) {
        return false;
    }
 
    int diaFin = diaInicio + MAX_DIAS_SEMANA - 1;
    if (diaFin > totalDias) {
        diaFin = totalDias;
    }
 
    cantDias = diaFin - diaInicio + 1;
    return true;
}
 
// Ej: valor = 6, cifras = 2 -> "06"g
void numeroATexto(char destino[], int valor, int cifras) {
    for (int i = cifras - 1; i >= 0; i--) {
        destino[i] = '0' + valor % 10;   // ultima cifra como caracter
        valor = valor / 10;              // me quedo con el resto de cifras
    }
    destino[cifras] = '\0';
}
 
// "comandas_dd-mm-aaaa.dat"
void armarNombreDia(char nombre[], int dia, int mes, int anio) {
    char parte[10];
 
    strcpy(nombre, "comandas_");
 
    numeroATexto(parte, dia, 2);
    strcat(nombre, parte);
 
    strcat(nombre, "-");
 
    numeroATexto(parte, mes, 2);
    strcat(nombre, parte);
 
    strcat(nombre, "-");
 
    numeroATexto(parte, anio, 4);
    strcat(nombre, parte);
 
    strcat(nombre, ".dat");
}
 
// "comandas_semana_sX-mm.dat"
void armarNombreSemana(char nombre[], int semana, int mes) {
    char parte[10];
 
    strcpy(nombre, "comandas_semana_s");
 
    if (semana < 10) {
        numeroATexto(parte, semana, 1);
    } else {
        numeroATexto(parte, semana, 2);
    }
    strcat(nombre, parte);
 
    strcat(nombre, "-");
 
    numeroATexto(parte, mes, 2);
    strcat(nombre, parte);
 
    strcat(nombre, ".dat");
}

bool pedirDatos(int &mes, int &anio, int &semana, int &diaInicio, int &cantDias) {
    cout << "=== CIERRE SEMANAL ===" << endl;
    cout << "Mes (1-12): ";
    cin >> mes;
    cout << "Anio: ";
    cin >> anio;
    cout << "Numero de semana del mes (1, 2, 3...): ";
    cin >> semana;
 
    if (mes < 1 || mes > 12) {
        cout << "Mes invalido." << endl;
        return false;
    }
    if (semana < 1) {
        cout << "Numero de semana invalido." << endl;
        return false;
    }
    if (!calcularSemana(mes, semana, diaInicio, cantDias)) {
        cout << "Ese mes no tiene semana " << semana << "." << endl;
        return false;
    }
    return true;
}

 //lee el siguiente registro de un archivo, devuelve true si lee, false si se termino

bool leerComanda(FILE *archivo, Comanda &comanda) {
    return fread(&comanda, sizeof(Comanda), 1, archivo) == 1;
}

int abrirPlanillas(FILE *dias[], Comanda actual[], bool hay[],
                   int cantDias, int diaInicio, int mes, int anio) {
    int abiertos = 0;
    char nombre[40];
 
    for (int i = 0; i < cantDias; i++) {
        int diaActual = diaInicio + i;
        armarNombreDia(nombre, diaActual, mes, anio);
 
        dias[i] = fopen(nombre, "rb");
        hay[i] = false;
 
        if (dias[i] == NULL) {
            cout << "Sin planilla para " << nombre << " (se saltea)" << endl;
        } else {
            abiertos++;
            // si el archivo esta vacio, hay[i] queda en false
            hay[i] = leerComanda(dias[i], actual[i]);
        }
    }
    return abiertos;
}

int aparear(FILE *dias[], Comanda actual[], bool hay[], int cantDias, FILE *salida) {
    int totalEscritos = 0;
    bool quedaAlguno = true;
 
    while (quedaAlguno) {
        //busco cual de los candidatos actuales es el menor
        int menor = -1;
        for (int i = 0; i < cantDias; i++) {
            if (hay[i]) {
                if (menor == -1 || actual[i].idMozo < actual[menor].idMozo) {
                    menor = i;
                }
            }
        }
 
        if (menor == -1) {
            // ningun archivo tiene candidato, termino
            quedaAlguno = false;
        } else {
            // vuelco el menor y avanzo solo en ese archivo
            fwrite(&actual[menor], sizeof(Comanda), 1, salida);
            totalEscritos++;
            hay[menor] = leerComanda(dias[menor], actual[menor]);
        }
    }
 
    return totalEscritos;
}

void cerrarPlanillas(FILE *dias[], int cantDias) {
    for (int i = 0; i < cantDias; i++) {
        if (dias[i] != NULL) {
            fclose(dias[i]);
        }
    }
}
int main() {
    int mes, anio, semana;
    int diaInicio, cantDias;
 
    if (!pedirDatos(mes, anio, semana, diaInicio, cantDias)) {
        return 1;
    }
 
    FILE *dias[MAX_DIAS_SEMANA];       // un archivo por dia de la semana
    Comanda actual[MAX_DIAS_SEMANA];   // registro pendiente de cada archivo
    bool hay[MAX_DIAS_SEMANA];         // true si actual[i] tiene un registro valido
 
    int abiertos = abrirPlanillas(dias, actual, hay, cantDias, diaInicio, mes, anio);
 
    if (abiertos == 0) {
        cout << "No se encontro ninguna planilla diaria. No se genera la semanal." << endl;
        return 1;
    }
 
    char nombreSemana[40];
    armarNombreSemana(nombreSemana, semana, mes);
 
    FILE *salida = fopen(nombreSemana, "wb");
    if (salida == NULL) {
        cout << "No se pudo crear " << nombreSemana << endl;
        cerrarPlanillas(dias, cantDias);
        return 1;
    }
 
    int total = apareo(dias, actual, hay, cantDias, salida);
 
    cerrarPlanillas(dias, cantDias);
    fclose(salida);
 
    cout << "Listo: " << nombreSemana << " generado con " << total
         << " ventas de " << abiertos << " dia(s)." << endl;
 
    return 0;
}