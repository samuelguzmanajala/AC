/*
    AC - OpenMP -- SERIE
    fun_s.c
     rutinas que se utilizan en el modulo gengrupos_s.c 
****************************************************************************/
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <limits.h>

#include "defineg.h"           // definiciones

///////////////////////////////////////////////////////////////////////////////////////////////
/* 1 - Funcion para calcular la distancia genetica entre dos elementos (distancia euclidea)
       Entrada:  2 elementos con NCAR caracteristicas (por referencia)
       Salida:  distancia (double)
*************************************************************************************/

double gendist (float *elem1, float *elem2)
{
    double sum =0;

    for(int i=0 ; i<NCAR; i++){
        sum=sum+ pow((elem1[i]-elem2[i]),2);
    }
    double solution=sqrt(sum);
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

    for(int i=0; i<nelem;i++){
        double minGenDist = DBL_MAX;
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
    float suma_dist = 0;
    float cont;
    for(int k=0; k<NGRUPOS;k++){
        suma_dist=0.0;
        cont = 0;
        if(listag[k].nelemg==0||listag[k].nelemg==1) {
            densidad[k]=0;
        }else{
            for(int i = 0; i<listag[k].nelemg-1;i++){
                for(int j = i+1;j<listag[k].nelemg;j++){
                    suma_dist+=gendist(elem[listag[k].elemg[i]],elem[listag[k].elemg[j]]);
                    cont++;
                }
            }
            densidad[k] = (suma_dist/cont);
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
    for (int i = 0; i < TENF; i++) {
        prob_enf[i].min = FLT_MAX;
        prob_enf[i].max = FLT_MIN;
    }
        for (int i = 0; i < NGRUPOS; i++) {
            float media=0;
            for (int j = 0; j < TENF; j++) {
                sumaEnfermedades=0;
                for (int k = 0; k < listag[i].nelemg; k++) {
                    sumaEnfermedades=sumaEnfermedades+enf[listag[i].elemg[k]][j];
                }
                media= sumaEnfermedades/listag[i].nelemg;
                if(media> prob_enf[j].max){
                    prob_enf[j].max = media;
                    prob_enf[j].gmax = i;
                }
                if(media<prob_enf[j].min){
                    prob_enf[j].min=media;
                    prob_enf[j].gmin = i;
                }

            }
    }


}

