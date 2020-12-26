/*
    AC - OpenMP -- SERIE
    fun_s.c
     rutinas que se utilizan en el modulo gengrupos_s.c
****************************************************************************/
#include <math.h>
#include <float.h>
#include <omp.h>
#include <stdio.h>
#include <limits.h>

#include "defineg.h"           // definiciones

///////////////////////////////////////////////////////////////////////////////////////////////
/* 1 - Funcion para calcular la distancia genetica entre dos elementos (distancia euclidea)
       Entrada:  2 elementos con NCAR caracteristicas (por referencia)
       Salida:  distancia (double)
*************************************************************************************/

double gendist(float *elem1, float *elem2) {
    double sum = 0;
    int i = 0;
#pragma omp parallel for private(i) reduction(+:sum) schedule(static)
    for (i = 0; i < NCAR; i++) {
        sum = sum + pow((elem1[i] - elem2[i]), 2);
    }
    double solution = sqrt(sum);
    return solution;
// PARA COMPLETAR
// calcular la distancia euclidea entre dos vectores
}





///////////////////////////////////////////////////////////////////////////////////////////////
/* 2 - Funcion para calcular el grupo (cluster) mas cercano (centroide mas cercano)
       Entrada:  nelem  numero de elementos, int
                 elem   elementos, una matriz de tamano MAXE x NCAR, por referencia
                 cent   centroides, una matriz de tamano NGRUPOS x NCAR, por referencia
       Salida:   popul  grupo mas cercano a cada elemento, vector de tamano MAXE, por referencia
*****************************************************************************************/
void grupo_cercano (int nelem, float elem[][NCAR], float cent[][NCAR], int *popul)
{
    int i=0;
    double minGenDist;
#pragma omp parallel for private(i) reduction (min : minGenDist) schedule(runtime)
    for(i=0; i<nelem;i++){
        minGenDist = DBL_MAX;
        for(int k=0; k < NGRUPOS; k++){
            double distancia=gendist(elem[i], cent[k]);
            if(distancia<minGenDist){
                popul[i]=k;
                //printf("el grupo de el elemento %d es %d y la distancia es %f\n", i, k, distancia);
                minGenDist=distancia;
            }
        }
    }
// PARA COMPLETAR
// popul: grupo mas cercano a cada elemento
}



///////////////////////////////////////////////////////////////////////////////////////////////
/* 3 - Funcion para calcular la densidad del grupo (dist. media entre todos sus elementos)
       Entrada:  elem     elementos, una matriz de tamano MAXE x NCAR, por referencia
                 listag   vector de NGRUPOS structs (informacion de grupos generados), por ref.
       Salida:   densidad densidad de los grupos (vector de tamano NGRUPOS, por referencia)
*****************************************************************************************/
void calcular_densidad (float elem[][NCAR], struct lista_grupos *listag, float *densidad)
{
    float suma_dist;
#pragma omp parallel reduction(+ : suma_dist)
    {
        suma_dist = 0;
        for (int k = 0; k < NGRUPOS; k++) {
            if (listag[k].nelemg == 0 || listag[k].nelemg == 1) {
                densidad[k] = 0;
            } else {
                int i;
                int j;
#pragma omp for private(i, j) schedule (dynamic)
                for (i = 0; i < listag[k].nelemg - 1; i++) {
                    for (j = i + 1; j < listag[k].nelemg; j++) {
                        suma_dist += gendist(elem[listag[k].elemg[i]], elem[listag[k].elemg[j]]);
                    }
                }
                densidad[k] = (suma_dist / listag[k].nelemg);
            }
        }
    }

// PARA COMPLETAR
// Calcular la densidad de los grupos:
//        media de las distancia entre todos los elementos del grupo
//        si el numero de elementos del grupo es 0 o 1, densidad = 0
}



///////////////////////////////////////////////////////////////////////////////////////////////
/* 4 - Funcion para relizar el analisis de enfermedades
       Entrada:  listag   vector de NGRUPOS structs (informacion de grupos generados), por ref.
                 enf      enfermedades, una matriz de tamano MAXE x TENF, por referencia
       Salida:   prob_enf vector de TENF structs (informacion del analisis realizado), por ref.
*****************************************************************************************/
void analizar_enfermedades (struct lista_grupos *listag, float enf[][TENF], struct analisis *prob_enf){
// PARA COMPLETAR
// Realizar el analisis de enfermedades en los grupos:
//        maximo y grupo en el que se da el maximo (para cada enfermedad)
//        minimo y grupo en el que se da el minimo (para cada enfermedad)


    float sumaEnfermedades;
#pragma omp parallel //reduction(+:sumaEnfermedades)
    {
        int h;
        //float sumaEnfermedades;
#pragma omp for private(h)
        for (h = 0; h < TENF; h++) {
            prob_enf[h].min = FLT_MAX;
            prob_enf[h].max = FLT_MIN;
        }
        int i;
        float media;
//#pragma omp for private (i,j,k,media)

        for (i = 0; i < NGRUPOS; i++) {
            media= 0;
            for (int j = 0; j < TENF; j++) {
                int k;
                sumaEnfermedades = 0;
#pragma omp for private (k)
                for (k = 0; k < listag[i].nelemg; k++) {:
#pragma omp atomic
                    sumaEnfermedades = sumaEnfermedades + enf[listag[i].elemg[k]][j];
                }

                media = sumaEnfermedades / listag[i].nelemg;
#pragma omp barrier
                if (media > prob_enf[j].max) {
                    prob_enf[j].max = media;
                    prob_enf[j].gmax = i;
                }
                if (media < prob_enf[j].min) {
                    prob_enf[j].min = media;
                    prob_enf[j].gmin = i;
                }

            }
        }
    }
}




