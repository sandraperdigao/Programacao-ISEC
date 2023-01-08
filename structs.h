/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   structs.h
 * Author: SANDRA MARIA GONCALVES PERDIGAO a2019102697
 *
 * Created on 27 de Abril de 2020, 18:37
 */

#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>
#include <stdlib.h>
#define MAX 100


typedef struct doentes pessoas, *pno;
struct doentes{
    char nome[MAX];
    int idade;
    char estado[1];
    int dias;
    pno prox;
};

typedef struct sala local, *plocal;
struct sala{
    int id; //id numerico do local
    int capacidade; //capacidade maxima
    int liga[3]; //id das ligacoes (-1) nos nao usados
};

typedef struct salaaux localaux, *plocalaux;
struct salaaux{
    int id; //id numerico do local
    int capacidade; //capacidade maxima
    int liga[3]; //id das ligacoes (-1) nos nao usados
    int contador; //contador de pessoas
    pno pess; //Cada localaux tem um ponteiro para pessoas chamado pess
    pno p1; //Lista para retroceder uma iteracao
    pno p2; //Lista para retroceder duas iteracoes
    pno p3; //Lista para retroceder tres iteracoes
};

#endif /* STRUCTS_H */

