
/*
    gengrupos_s.c   SERIE

    Entrada: dbgen.dat    fichero con la informacion genetica de cada muestra
             dbenf.dat    fichero con la informacion sobre las enfermedades de cada muestra
    Salida:  dbgen_s.out  centroides, densidad, analisis

    compilar con el modulo fun_s.c y la opcion -lm
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "defineg.h"
#include "fun.h"

float  elem[MAXE][NCAR];               // elementos (muestras) a procesar
float  enf[MAXE][TENF];                // enfermedades asociadas a las muestras
struct lista_grupos listag[NGRUPOS];   // lista de elementos de los grupos


// programa principal
// ==================

void main (int argc, char *argv[])
{
  float   cent[NGRUPOS][NCAR], newcent[NGRUPOS][NCAR]; // centroides
  float   densidad[NGRUPOS];	                       // densidad de cada cluster
  struct  analisis prob_enf[TENF];                     // analisis de los tipos de enfermedades

  int     popul[MAXE];                                 // grupo de cada elemento
  double  additions[NGRUPOS][NCAR+1]; 
  
  int     i, j, nelem, grupo, num;
  int     fin = 0, num_ite = 0; 
  double  discent;

  FILE   *fd;
  struct timespec  t1, t2;
  double texe;

  if ((argc < 3)  || (argc > 4)) {
    printf ("ERROR:  gengrupos bd_muestras bd_enfermedades [num_elem]\n");
    exit (-1);
  }

  printf ("\n >> Ejecucion serie\n");
  clock_gettime (CLOCK_REALTIME, &t1);
  

  // lectura de datos (muestras): elem[i][j]
  // =======================================

  fd = fopen (argv[1], "r");
  if (fd == NULL) {
    printf ("Error al abrir el fichero %s\n", argv[1]);
    exit (-1);
  }

  fscanf (fd, "%d", &nelem);
  if (argc == 4) nelem = atoi(argv[3]);	// 4. parametro: numero de elementos

  for (i=0; i<nelem; i++) 
  for (j=0; j<NCAR; j++) 
    fscanf (fd, "%f", &(elem[i][j]));

  fclose (fd);


  // lectura de datos (enfermedades): enf[i][j]
  // ==========================================

  fd = fopen (argv[2], "r");
  if (fd == NULL) {
    printf ("Error al abrir el fichero %s\n", argv[2]);
    exit (-1);
  }

  for (i=0; i<nelem; i++) {
    for (j=0; j<TENF; j++)
      fscanf (fd, "%f", &(enf[i][j]));
  }
  fclose (fd);


  // generacion de los primeros centroides de forma aleatoria
  // ========================================================

  srand (147);
  for (i=0; i<NGRUPOS; i++) 
  for (j=0; j<NCAR/2; j++)
   { 
     cent[i][j] = (rand() % 10000) / 100.0; 
     cent[i][j+(NCAR/2)] = cent[i][j]; 
   } 


  // 1. fase: clasificar los elementos y calcular los nuevos centroides
  // ==================================================================

  num_ite = 0; fin = 0;
  while ((fin == 0) && (num_ite < MAXIT))
  {
    // calcular el grupo mas cercano 
    grupo_cercano (nelem, elem, cent, popul);

    // calcular los nuevos centroides de los grupos
    // media de cada caracteristica
    // acumular los valores de cada caracteristica (100); numero de elementos al final
    for (i=0; i<NGRUPOS; i++)
     for (j=0; j<NCAR+1; j++) 
      additions[i][j] = 0.0;

    for (i=0; i<nelem; i++)
    {
      for (j=0; j<NCAR; j++) additions[popul[i]][j] += elem[i][j];
      additions[popul[i]][NCAR]++;
    }

    // calcular los nuevos centroides y decidir si el proceso ha finalizado o no (en funcion de DELTA)
    fin = 1;
    for (i=0; i<NGRUPOS; i++) 
    {
      if (additions[i][NCAR] > 0) // ese grupo (cluster) no esta vacio
      {
        for (j=0; j<NCAR; j++) newcent[i][j] = additions[i][j] / additions[i][NCAR];

        // decidir si el proceso ha finalizado
        discent = gendist (&newcent[i][0], &cent[i][0]);
        if (discent > DELTA) fin = 0;  // en alguna centroide hay cambios; continuar 

        // copiar los nuevos centroides
        for (j=0; j<NCAR; j++) cent[i][j] = newcent[i][j];
      }
    }

    num_ite++;
  } // while
  //esto es agregado por mi asi que ojo a lo que haces
  for(int i=0; i<MAXE; i++){
      //printf("el elemento %d pertenece al grupo %d\n", i, popul[i]);
  }



  // 2. fase: numero de elementos de cada grupo; densidad; analisis enfermedades
  // ===========================================================================

  for (i=0; i<NGRUPOS; i++) listag[i].nelemg = 0;

  // numero de elementos y su clasificacion
  for (i=0; i<nelem; i++) 
  {
    grupo = popul[i];
    num=listag[grupo].nelemg;    
    listag[grupo].elemg[num] = i;// elementos de cada grupo (cluster)
    listag[grupo].nelemg++;
    //printf("el grupo del elemento %d es %d y su numero de elementos es %d\n ", i, grupo, listag[grupo].nelemg);
  }

  // densidad de cada cluster: media de las distancias entre todos los elementos
  calcular_densidad (elem, listag, densidad);
  
  // analisis de enfermedades
  analizar_enfermedades (listag, enf, prob_enf);

  // escritura de resultados en el fichero de salida
  // ===============================================

  fd = fopen ("dbgen_s.out", "w");
  if (fd == NULL) {
    printf ("Error al abrir el fichero dbgen_out.s\n");
    exit (-1);
  }

  fprintf (fd,">> Centroides de los clusters\n\n");
  for (i=0; i<NGRUPOS; i++) {
    for (j=0; j<NCAR; j++) fprintf (fd, "%7.3f", newcent[i][j]);
    fprintf (fd,"\n");
  }

  fprintf (fd,"\n\n>> Numero de elementos de cada cluster y densidad del cluster\n\n");
  for (i=0; i<NGRUPOS; i++)
    fprintf (fd, " %6d  %.3f \n", listag[i].nelemg, densidad[i]);

  fprintf (fd,"\n\n>> Analisis de enfermedades en los grupos\n\n");
  for (i=0; i<TENF; i++)
    fprintf (fd,"Enfermedad: %2d - max: %4.2f (grupo %2d) - min: %4.2f (grupo %2d)\n",
                 i, prob_enf[i].max, prob_enf[i].gmax, prob_enf[i].min, prob_enf[i].gmin);

  fclose (fd);

  clock_gettime (CLOCK_REALTIME, &t2);
  texe = (t2.tv_sec-t1.tv_sec) + (t2.tv_nsec-t1.tv_nsec)/(double)1e9;



  // mostrar por pantalla algunos resultados
  // =======================================

  printf ("\n>> Centroides 0, 40 y 80, y su valor de densidad\n ");
  for (i=0; i<NGRUPOS; i+=40) {
    printf ("\n  cent%2d -- ", i);
    for (j=0; j<NCAR; j++) printf ("%5.1f", cent[i][j]);
    printf ("\n          %5.6f\n", densidad[i]);
  }

  printf ("\n>> Tamano de los grupos \n");
  for (i=0; i<10; i++) {
    for (j=0; j<10; j++) printf ("%7d", listag[10*i+j].nelemg);
    printf("\n");
  }

  printf ("\n>> Analisis de enfermedades en los grupos\n");
  for (i=0; i<TENF; i++)
    printf ("Enfermedad: %2d - max: %4.2f (grupo %2d) - min: %4.2f (grupo %2d)\n", 
             i, prob_enf[i].max, prob_enf[i].gmax, prob_enf[i].min, prob_enf[i].gmin);
  
  printf ("\n >> Numero de iteraciones: %d", num_ite);
  printf ("\n >> Tex (serie): %1.3f s\n\n", texe);
}


