#include <iostream>
using namespace std;

struct ComandaHistorica{
	char fecha[11];
	char nombreMozo[50];
	int codigoProducto;
	int cantidad;
	float comision;
};

struct Producto{
	int codigo;
	char descripcion[50];
	float precio;
	int stockActual;
};

struct Mozo{
	int idMozo;
	char nombre[50];
	char password[20];
	float totalComision;
};

struct Comanda{
	int idMozo;
	int codigoProducto;
	int cantidad;
	float comision;
};

const float TASA_COMISION = 0.10f;

void ordenamientoPorFecha(const char* nombreArchivo);

int main(){
	ordenamientoPorFecha("comandas_historicas.dat");
}

void ordenamientoPorFecha(const char* nombreArchivo){
	int len=0;
	ComandaHistorica ch;
	FILE*archivo=fopen(nombreArchivo,"rb+");
	if(archivo==NULL){
		cout<<"No se pudo abrir el archivo"<<endl;
		return 1;
	}
	while(fread(&ch,sizeof(ComandaHistorica),1,archivo)==1){
		len++;
	}
	for(int i=0;i<(len-1);i++){
		for(int j=0;j<(len-i-1);j++){
		    ComandaHistorica ch1, ch2, temp;
		    
		    // Inicio de valores de ch1 y ch2 para luego comparar si estan ordenados
	    	fseek(archivo,j*sizeof(ComandaHistorica),SEEK_SET);
            fread(&ch1,sizeof(ComandaHistorica),1,archivo);
            fseek(archivo,(j+1)*sizeof(ComandaHistorica),SEEK_SET);
            fread(&ch2,sizeof(ComandaHistorica),1,archivo);
                
            // Se verifica y en el caso, se ordena
            if(strcmp(ch1.fecha,ch2.fecha)>0){
                temp=ch1;
                ch1=ch2;
        		ch2=temp;
            	fseek(archivo,j*sizeof(ComandaHistorica),SEEK_SET);
            	fwrite(&ch1,sizeof(ComandaHistorica),1,archivo);   
            	fseek(archivo,(j+1)*sizeof(ComandaHistorica),SEEK_SET);
            	fwrite(&ch2,sizeof(ComandaHistorica),1,archivo);
            }
	    }
	}
	fclose(archivo);
}

