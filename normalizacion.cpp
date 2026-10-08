#include <iostream>
#include <cstring>
#include <cstdio>
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

void ordenamientoPorMozo(const char* nombreArchivo){
	int len=0;
	ComandaHistorica ch;
	FILE*archivo=fopen(nombreArchivo,"rb+");
	if(archivo==NULL){
		cout<<"No se pudo abrir el archivo"<<endl;
		return;
	}
	while(fread(&ch,sizeof(ComandaHistorica),1,archivo)==1){
		len++;
	}
	for(int i=0;i<(len-1);i++){
		for(int j=0;j<(len-i-1);j++){
		    ComandaHistorica ch1, ch2, temp;
	    	fseek(archivo,j*sizeof(ComandaHistorica),SEEK_SET);
            fread(&ch1,sizeof(ComandaHistorica),1,archivo);
            fseek(archivo,(j+1)*sizeof(ComandaHistorica),SEEK_SET);
            fread(&ch2,sizeof(ComandaHistorica),1,archivo);
            if(strcmp(ch1.nombreMozo,ch2.nombreMozo)>0){
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

void listaDeMozos(const char* nombreArchivo){
	ComandaHistorica ch;
	Mozo m;
	int idMozo=1;
	FILE*archivo=fopen(nombreArchivo,"rb");
	if(archivo==NULL){
		cout<<"No se pudo abrir el archivo"<<endl;
		return;
	}
	FILE*mozos=fopen("mozos.dat","wb");
	if(mozos==NULL){
		cout<<"No se pudo crear el archivo"<<endl;
		return;
	}
	int leido=fread(&ch,sizeof(ComandaHistorica),1,archivo);
	while(leido==1){
		m.totalComision=0;
		char nombreGuia[50], password[20];
		strcpy(nombreGuia,ch.nombreMozo);
		strcpy(m.nombre,nombreGuia);
		cout<<"Ingrese una contrasenia para el mozo "<<ch.nombreMozo<<" (no mas de 20 caracteres):"<<endl; // Asignación de contraseña por mozo
		cin.getline(password,20);
		for(int p=0;password[p]!='\0';p++){
			password[p]+=3;
		}
		strcpy(m.password,password);
		while(leido==1 && strcmp(nombreGuia,ch.nombreMozo)==0){
			m.totalComision+=ch.comision;
			leido=fread(&ch,sizeof(ComandaHistorica),1,archivo);
		}
		m.idMozo=idMozo;
		fwrite(&m,sizeof(Mozo),1,mozos);
		idMozo++;
	}
	fclose(mozos);
	fclose(archivo);
}

void ordenamientoPorFecha(const char* nombreArchivo);
void comandasPorDia(const char* nombreArchivo1, const char* nombreArchivo2);
void ordenamientoPorCodigo(const char* nombreArchivo);
void actualizacionDeStock(const char* nombreArchivo1, const char* nombreArchivo2);

int main(){
	ordenamientoPorFecha("comandas_historicas.dat");
	comandasPorDia("comandas_historicas.dat","mozos.dat");
	ordenamientoPorCodigo("comandas_historicas.dat");
	actualizacionDeStock("comandas_historicas.dat","inventario.dat");
}

void ordenamientoPorFecha(const char* nombreArchivo){
	int len=0;
	ComandaHistorica ch;
	FILE*archivo=fopen(nombreArchivo,"rb+");
	if(archivo==NULL){
		cout<<"No se pudo abrir el archivo"<<endl;
		return;
	}
	while(fread(&ch,sizeof(ComandaHistorica),1,archivo)==1){
		len++;
	}
	for(int i=0;i<(len-1);i++){
		for(int j=0;j<(len-i-1);j++){
		    ComandaHistorica ch1, ch2, temp; // Se inicializan los valores de ch1 y ch2 para la comparación
	    	fseek(archivo,j*sizeof(ComandaHistorica),SEEK_SET);
            fread(&ch1,sizeof(ComandaHistorica),1,archivo);
            fseek(archivo,(j+1)*sizeof(ComandaHistorica),SEEK_SET);
            fread(&ch2,sizeof(ComandaHistorica),1,archivo); 
            if(strcmp(ch1.fecha,ch2.fecha)>0){ // Se comparan y en el caso, se cambian de lugar
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

void comandasPorDia(const char* nombreArchivo1, const char* nombreArchivo2){
	ComandaHistorica ch;
	Mozo m;
	Comanda c;
	FILE*archivo=fopen(nombreArchivo1,"rb");
	if(archivo==NULL){
		cout<<"No se pudo abrir el archivo"<<endl;
		return;
	}
	FILE*mozos=fopen(nombreArchivo2,"rb");
	if(mozos==NULL){
		cout<<"No se pudo abrir el archivo"<<endl;
		fclose(archivo);
		return;
	}
	int leido=fread(&ch,sizeof(ComandaHistorica),1,archivo);
	while(leido==1){
		char fechaGuia[11]; // Se crea cada archivo de las comandas por día
		strcpy(fechaGuia,ch.fecha);
		char archivoActual[24]="comandas_";
		strcat(archivoActual,fechaGuia);
		strcat(archivoActual,".dat");
		FILE*comandas=fopen(archivoActual,"wb");
		if(comandas==NULL){
			cout<<"No se pudo crear el archivo"<<endl;
			return;
		}
		while(leido==1 && strcmp(fechaGuia,ch.fecha)==0){
			c.cantidad=ch.cantidad;
			c.codigoProducto=ch.codigoProducto;
			c.comision=ch.comision;
			c.idMozo=0; // Se inicializa idMozo por si no se encuentra en el archivo
			while(fread(&m,sizeof(Mozo),1,mozos)==1){
				if(strcmp(ch.nombreMozo,m.nombre)==0){
					c.idMozo=m.idMozo;
					break;
				}
			}
			fseek(mozos,0,SEEK_SET);
			fwrite(&c,sizeof(Comanda),1,comandas);
			leido=fread(&ch,sizeof(ComandaHistorica),1,archivo);
		}
		fclose(comandas);
	}
	fclose(mozos);
	fclose(archivo);
}

void ordenamientoPorCodigo(const char* nombreArchivo){
	int len=0;
	ComandaHistorica ch;
	FILE*archivo=fopen(nombreArchivo,"rb+");
	if(archivo==NULL){
		cout<<"No se pudo abrir el archivo"<<endl;
		return;
	}
	while(fread(&ch,sizeof(ComandaHistorica),1,archivo)==1){
		len++;
	}
	for(int i=0;i<(len-1);i++){
		for(int j=0;j<(len-i-1);j++){
		    ComandaHistorica ch1, ch2, temp;
	    	fseek(archivo,j*sizeof(ComandaHistorica),SEEK_SET);
            fread(&ch1,sizeof(ComandaHistorica),1,archivo);
            fseek(archivo,(j+1)*sizeof(ComandaHistorica),SEEK_SET);
            fread(&ch2,sizeof(ComandaHistorica),1,archivo); 
            if(ch1.codigoProducto>ch2.codigoProducto){
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

void actualizacionDeStock(const char* nombreArchivo1, const char* nombreArchivo2){
	ComandaHistorica ch;
	Producto p;
	FILE*archivo=fopen(nombreArchivo1,"rb");
	if(archivo==NULL){
		cout<<"No se pudo abrir el archivo"<<endl;
		return;
	}
	FILE*inventario=fopen(nombreArchivo2,"rb+");
	if(inventario==NULL){
		cout<<"No se pudo abrir el archivo"<<endl;
		fclose(archivo);
		return;
	}
	int leido=fread(&ch,sizeof(ComandaHistorica),1,archivo);
	while(leido==1){
		fseek(inventario,0,SEEK_SET);
		while(fread(&p,sizeof(Producto),1,inventario)==1){
			if(p.codigo==ch.codigoProducto){
				p.stockActual-=ch.cantidad;
				fseek(inventario,-sizeof(Producto),SEEK_CUR);
				fwrite(&p,sizeof(Producto),1,inventario);
				break;
			}
		}
		leido=fread(&ch,sizeof(ComandaHistorica),1,archivo);
	}
	fclose(inventario);
	fclose(archivo);
}
