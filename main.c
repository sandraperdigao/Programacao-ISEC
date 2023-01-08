/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: SANDRA MARIA GONCALVES PERDIGAO a2019102697
 *
 * Created on 27 de Abril de 2020, 18:16
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "structs.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    initRandom(); //funcao para inicializar o gerador de numeros aleatorios
    int m, i=0; // variaveis para o menu
    int tam=0; //numero de pessoas
    int size=0; // numero de espacos
    int ct=0; //capacidade total do espaco (soma das capacidades dos locais individuais)
    int flag1=0, flag2=0, flag3=0; //flags para verificacao da validacao dos ficheiros locais e pessoas
    int iteracoes=0;
    plocalaux paux = NULL;  // ponteiro para array de estruturas localaux
    pno lista = NULL; //ponteiro para lista de pessoas
    
    printf("************** SIMULACAO DA PROPAGACAO DE VIRUS COVID-19 ***********\n");
    printf("*                                                                  *\n");
    printf("*                   ISEC - Programacao 2019/2020                   *\n");
    printf("*                                                                  *\n");
    printf("*               Aluna: Sandra Perdigao Nr: 2019102697              *\n");
    printf("*                                                                  *\n");    
    printf("*          Docente Responsavel: Professor Francisco Pereira        *\n");
    printf("*                                                                  *\n");
    printf("********************************************************************\n\n");    
   
    // FASE DE PREPARACAO //
    // 1. Carregar Ficheiro Binario //
    printf("********************************************************************\n\n");
    paux = carregabin(paux,&size,&ct); //Carregar Ficheiro Binario para vetor dinamico
    escrevetodos(paux,size); //Imprimir informacao do que estava no ficheiro binario e esta agora num vetor dinamico
    flag1 = validalocais(paux,size); //Validar que os ids dos locais sao positivos e as ligacoes validas
    printf("********************************************************************\n");
    if(flag1==1) return 1;
    // 1. Concluido  //
    
    //2.Carregar Ficheiro txt //
    lista = carregatxt(lista,&tam,ct,&flag2); //Carregar Ficheiro TXT para lista ligada
    if(flag2==1){
        printf("********************************************************************\n");
        eliminarlista(lista);
        //escrevepessoas(lista);
        return 1;
    }
    escrevepessoas(lista);
    flag3 =validapessoas(lista);
    printf("********************************************************************\n");
    if(flag3==1){
        eliminarlista(lista);
        //escrevepessoas(lista);
        return 1;
    }
    inserepessoasnosespacos(paux ,lista, size); //Distribuir as pessoas nos locais
    // 2. Concluido  //
    // FASE DE PREPARACAO CONCLUIDA //
    
    printf("************************* FASE DE SIMULACAO ************************\n\n");
    // FASE DE SIMULACAO //
    //MENU PRINCIPAL
    do{
        m = menu();
        switch(m){
            case 1: avancaiteracao(paux, tam, size, &iteracoes);break;
            case 2: apresentaestatistica(paux,size,ct,tam);break;
            case 3: adicionadoente(paux,&tam,size,&iteracoes);break;
            case 4: transferepessoas(paux,size,&iteracoes);break;
            case 5: listapessoas(paux,size); break;
            case 6: retrocedeiteracoes(paux,size,iteracoes); break;
            case 7: relatorio(paux,size,ct,tam);exit(0);break;          
        }
    }while(i != 7);

    return (EXIT_SUCCESS);
}

