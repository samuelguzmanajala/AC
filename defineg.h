/*
    defineg.h
    definiciones utilizadas en los modulos de la aplicacion
***********************************************************/

#define MAXE     230000   // numero de elementos (muestras)
#define NGRUPOS  100      // numero de clusters
#define NCAR     40       // dimensiones de cada muestra
#define TENF     20       // tipos de enfermedad

#define DELTA    0.01     // convergencia: cambio minimo en un centroide
#define MAXIT    10000    // convergencia: numero de iteraciones maximo


// estructuras de datos
//
struct lista_grupos  // informacion de los clusters
{
 int elemg[MAXE];   // indices de los elementos
 int nelemg;        // numero de elementos
};

struct analisis     // resultados del analisis de enfermedades
{
 float max, min;    // maximo y minimo de todos los grupos
 int gmax, gmin;    // grupos con los valores maximo y minimo
};

