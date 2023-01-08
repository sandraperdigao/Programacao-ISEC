/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   functions.h
 * Author: SANDRA MARIA GONCALVES PERDIGAO a2019102697
 *
 * Created on 27 de Abril de 2020, 18:38
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "structs.h"

int resposta();

plocalaux carregabin(plocalaux paux, int *size, int *ct);

void escrevetodos(plocalaux paux,int size);

int validalocais(plocalaux paux,int size);

pno carregatxt(pno lista,int *tam, int ct, int *flag2);

void escrevepessoas(pno lista);

int validapessoas(pno lista);

void inserepessoasnosespacos( plocalaux paux , pno lista, int size);

void eliminarlista(pno lista);

void apresentaestatistica(plocalaux paux,int size, int ct, int tam);

int menu();

void incrementadiasmudaestado(pno p1, float taxaimune);

void avancaiteracao(plocalaux paux, int tam, int size, int *iteracoes);

int verificaIDnovo(plocalaux paux, char *nome,int size);

void adicionadoente(plocalaux paux, int *tam, int size, int *iteracoes);

void verificacondicoestransf(plocalaux paux,int size,int qtd,int idinicial,int idfinal,int *flag1,int *flag2,int *flag3,int *flag4);

void efetuatransferencia(plocalaux paux,int size,int qtd, int idinicial,int idfinal);

void transferepessoas(plocalaux paux, int size, int *iteracoes);

void listapessoas(plocalaux paux, int size);

void listapessoasp1(plocalaux paux, int size);

void listapessoasp2(plocalaux paux, int size);

void listapessoasp3(plocalaux paux, int size);

void atualizalistasparaiteracoes(plocalaux paux, int size);

void retrocedeiteracoes(plocalaux paux,int size, int iteracoes);

void guardapessoastxt(plocalaux paux, int size);

void elaborarelatorio(plocalaux paux,int size,int ct,int tam);

void relatorio(plocalaux paux, int size, int ct, int tam);

// Inicializa o gerador de numeros aleatorios.
// Esta funcao deve ser chamada apenas uma vez no inicio da execucao do programa
void initRandom();

//Devolve um valor inteiro aleatorio distribuido uniformemente entre [a, b]
int intUniformRnd(int a, int b);

// Devolve o valor 1 com probabilidade prob. Caso contrario, devolve 0
int probEvento(float prob);

#endif /* FUNCTIONS_H */

