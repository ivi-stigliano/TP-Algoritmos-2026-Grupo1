# TP Integrador — Buffet Albert Einstein

Trabajo Práctico grupal de **Algoritmos y Estructuras de Datos** (UTN FRBA) —
unidad de **archivos binarios en C/C++**.

**Grupo:** 1

**Integrantes:** 
- Jano Manuel - jmanuel-frba
- Thiago Nuñez - nunezthiago77-prog
- Mariano Nicolás Romero - marianoromero75
- Franco Santomil - frsantomil-oss
- Iván Nicolas Stigliano - ivi-stigliano

## División de Tareas

- normalizacion.cpp: Thiago Nuñez e Iván Stigliano
- ventas.cpp: Jano Manuel
- cierre.cpp: Mariano Romero
- resumen.cpp: Franco Santomil

## Orden de Ejecución

- normalizacion.cpp (respecto a comandas_historicas.dat inicializa lista de mozos, crea comandas por día y actualiza stock)
- ventas.cpp (carga ventas, las une a la planilla del dia y actualiza stock correspondiente)
- cierre.cpp (une todas las planillas diarias (las creadas en normalizacion y las creadas en ventas) en una semanal)
- resumen.cpp (de la planilla semanal muestra por pantalla total vendido y comisiones a pagar)

## Resolución de Partes Clave

- normalizacion.cpp: Para realizar la lista de mozos, las comandas por dia y la actualización de stock, la clave es ordenar comandas_historicas.dat mediante un criterio adecuado (para mozos por su nombre, para comandas por día la fecha de la comanda y para actualiazación de stock por codigo de producto). 
- ventas.cpp:
- cierre.cpp: Junta las planillas diarias de ventas de una semana del mes en una única planilla semanal, con los registros ordenados por mozo (idMozo). La clave es que según el año, mes y semana que elija el usuario va a dar como salida un archivo binario comandas_semana_sX-mm.dat, con todos los registros de esos días ordenados por mozo.
- resumen.cpp:

La consigna completa está en **`enunciado.pdf`**. Leela antes de arrancar: el
cliente (Alberto) cuenta su problema a su manera y ustedes tienen que descubrir
cómo resolverlo con lo que vimos de archivos.

## Estructura del repo

```
.
├── enunciado.pdf              # la consigna
├── datos/                     # archivos PROVISTOS por la cátedra (no se tocan)
│   ├── comandas_historicas.dat
│   ├── inventario.dat
│   └── dump_datos_de_prueba.txt   # los dos .dat en texto, para verificar la lectura
├── .gitignore
│
│   # --- estos los crean USTEDES (no vienen en el repo base) ---
├── normalizacion.cpp          # cada uno con su propio main; NO hay main.cpp
├── ventas.cpp
├── cierre.cpp
└── resumen.cpp
```

Los 4 programas los escriben ustedes desde cero: el repo base no trae código.
Créenlos en la raíz del repo (como en el diagrama del enunciado).

## Cómo trabajar

Los programas leen y escriben archivos `.dat` en la carpeta donde se ejecutan.
Los archivos de `datos/` son los **originales** y no se modifican: copiálos a la
carpeta donde vas a compilar y correr, y trabajá sobre esas copias.

```bash
cp datos/comandas_historicas.dat datos/inventario.dat .
```

Compilar y correr cada programa (cada uno tiene su propio `main`, no hay `main.cpp`):

```bash
g++ -O2 -o normalizacion normalizacion.cpp
g++ -O2 -o ventas        ventas.cpp
g++ -O2 -o cierre        cierre.cpp
g++ -O2 -o resumen       resumen.cpp
```

Orden de ejecución (cada programa deja los archivos que usa el siguiente):

```
./normalizacion     # historicas + inventario  ->  mozos.dat, comandas_dd-mm-aaaa.dat (varios), inventario actualizado
./ventas            # carga interactiva de la semana  ->  agrega a las planillas del día
./cierre            # junta los días de la semana      ->  comandas_semana_sX-mm.dat
./resumen           # imprime el resumen por mozo + total del buffet
```

## Verificar que leen bien el binario

`datos/dump_datos_de_prueba.txt` tiene el contenido de los dos archivos provistos
en texto legible. Si tu programa imprime otra cosa al leerlos, revisá los `struct`
(orden de campos / tamaños): tienen que dar `sizeof(ComandaHistorica)=76` y
`sizeof(Producto)=64`.

## Qué se entrega

- Este mismo repo (uno por grupo), con los **4 programas** `.cpp` en la raíz
  (cada uno con su propio `main`, no hay `main.cpp`).
- El **README** con el grupo, los integrantes (nombre + usuario de GitHub) y, si
  hace falta, cualquier aclaración de cómo correrlo.
- Los `.dat` de `datos/` son los **provistos por la cátedra** y ya vienen en el
  repo: no los borren ni agreguen otros. Cualquier `.dat` que ustedes copien a la
  raíz o generen al correr **no** se versiona (el `.gitignore` ya los deja afuera).
  La cátedra corre sus programas sobre su propio dataset.
- **Commits repartidos:** cada integrante tiene que tener commits propios a lo
  largo del trabajo. El historial es parte de lo que se mira para la defensa
  individual (no vale un único commit final ni que suba todo una sola persona).
