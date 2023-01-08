/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 * 
 * Author: SANDRA MARIA GONCALVES PERDIGAO a2019102697
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"
#include "structs.h"

// Funcoes

int resposta(){
    char resp='z';
    
    while((resp != 's') && (resp != 'n') && (resp != 'S') && (resp != 'N')){
        printf("\nIntroduza s-Sim ou n-Nao: ");
        scanf(" %c",&resp);
    };
    
    if((resp=='s') || (resp=='S'))
        return 1;
    else
        return 0;
}

int pedenomebin(char * fileName1){
    FILE *f;
    char *extensao;
    
    do{
        printf("Insira o nome do ficheiro de espaco a abrir(ex: E1.bin):");
        scanf("%s",fileName1);   
        
        //verificar que tem a extensao pretendida .bin
        extensao = strrchr(fileName1,'.'); //apanha os carateres a partir do ponto
        
        if(strcmp(extensao, ".bin") !=0){
            printf("O ficheiro nao tem a extensao adequada. Introduzir novamente?");
            if(resposta()==1)
                continue; // pede novamente o nome do ficheiro ao user
            else
                return 1; //devolve 1 e depois termina
        }
        
        //abrir o ficheiro
        f=fopen(fileName1,"rb");
        
        if(f==NULL){
            printf("Erro no acesso ao ficheiro. Introduzir novamente?");
            if(resposta()==1)
                continue; // pede novamente o nome do ficheiro ao user
            else
                return 1; //devolve 1 e depois termina
        }
        
        
        //nome do ficheiro validado
        else{
            fclose(f); //fecha o ficheiro
            return 0; // devolve 0
        }
        
    }while(1); //executa continuamente enquanto nada fizer o return. Ciclo do-while infinito
 
}

plocalaux carregabin(plocalaux paux, int *size, int *ct){
    FILE *f;
    local auxiliar; // estrutura auxiliar do tipo local para ler os dados do fichero binario
    char fileName1[MAX]; //nome para ficheiro binario de espacos 
      
    //Pedir o nome do ficheiro ao user
    if(pedenomebin(fileName1)==1)
        return NULL;
    
    //Ler ficheiro
    f= fopen(fileName1,"rb");
    if(f==NULL){
        printf("Erro no acesso ao ficheiro\n");
        return NULL;
    }
    
    //Usa a struct do tipo local apenas para ler
    //Usa a struct do tipo localaux para guardar
    while(fread(&auxiliar,sizeof(local),1,f)==1){
        paux = realloc(paux,sizeof(localaux)*((*size)+1));
        paux[(*size)].capacidade=auxiliar.capacidade;
        paux[(*size)].id=auxiliar.id;
        paux[(*size)].liga[0] = auxiliar.liga[0];
        paux[(*size)].liga[1] = auxiliar.liga[1];
        paux[(*size)].liga[2] = auxiliar.liga[2];
        paux[(*size)].contador = 0;
        paux[(*size)].pess = NULL;
        paux[(*size)].p1 = NULL;
        paux[(*size)].p2 = NULL;
        paux[(*size)].p3 = NULL;
        
        (*ct)+=auxiliar.capacidade;
        (*size)++;
    }
    
    //fecha o ficheiro binario
    fclose(f);
    
    //caso nao hajam locais devolve null
    if((*size)==0)
        return NULL;
    
    //se tudo ok devolve o ponteiro para o vetor
    return paux;
}

void escrevetodos(plocalaux paux,int size){
    int i;
    printf("\n");
    if(paux==NULL) //o ficheiro nao tinha locais para ler para vetor
        return;
    
    printf("LOCAIS CARREGADOS:\n\n");
    for(i=0; i<size; i++)
        printf("ID: %d\tCapacidade: %d\tLigacoes: %d %d %d\n",paux[i].id, paux[i].capacidade,paux[i].liga[0], paux[i].liga[1],paux[i].liga[2]);
    
    printf("\n");
}

int validalocais(plocalaux paux,int size){
    
    int i, j, k, l; //auxiliares para ciclos for
    int identif; //variavel para guardar id do local
    int identif2; //variavel para guardar id do local para comparar com identif
    int ligacao; //variavel para guardar um id de uma ligacao[3]
    int flag=0; //flag para verificar se a ligacao esta estabelecida
    
    //se nao havia locais para ler no ficheiro binario devolve 1
    if(paux==NULL){
        printf("Nao foi possivel iniciar a simulacao.\n");
        return 1;
    }
    
    // 1. Condicao
    // Verificar que os IDS sao unicos
    for(i=0; i<size-1; i++){
        identif=paux[i].id; //agarra no primeiro e compara-o com os seguintes. 
                            //O ultimo do ciclo e o penultimo elemento que so e comparado com o ultimo
        
        for(j=i+1;j<size; j++){
            identif2=paux[j].id;
            if(identif == identif2){
                printf("Ficheiro nao e valido pois existem locais com o mesmo id: %d.\n",identif);
                return 1;
            }
        }
    }
    
    // 2. Condicao
    //Verificar que os ID sao positivos
    for(i=0; i<size; i++){
        identif=paux[i].id;
        if(identif < 0){
            printf("Ficheiro nao e valido pois existe local com id negativo: %d.\n",identif);
            return 1;
        }   
    }
    
    //3. Condicao
    //Verificar que as ligacoes no vetor nao estao ligadas ao proprio ID
    // (por exemplo, o ID 3 nao pode ter no seu array liga o ID 3 [3 4 -1] X
    // Isto significaria que o local ID 3 esta ligado ao local ID 3 (a si proprio) o que nao faz sentido
    
    for(i=0; i<size; i++){
       identif=paux[i].id;
       for(j=0; j<=2;j++){
           if(paux[i].liga[j]==identif){
               printf("O ficheiro nao e valido pois existe local ligado a si proprio: ID %d",identif);
               return 1;
           }
       }
    }
    
    // 4. Condicao
    // Verificar que as ligacoes no vetor nao estao duplicadas no array liga (nao existe por exemplo ligacao: 4 4 -1 mas pode existir ligacao: 4 -1 -1)
    for(i=0; i<size; i++){
        identif=paux[i].id; // neste ciclo for so guarda o identif para o printf pois nao precisa dele
        
        //array liga[3]
        //agarra no primeiro e compara-o com os restantes e assim sucessivamente
        //o penultimo liga[1] so e comparado com o ultimo liga[2]
        for(j=0;j<=1;j++){ 
            for(k=j+1;k<=2;k++){
                if((paux[i].liga[j] == paux[i].liga[k]) && (paux[i].liga[j]) != -1){
                    printf("Ficheiro nao e valido pois existe local com ligacoes duplicadas: %d.\n",identif);
                    return 1;
                }
            }
        }
    }
    
    //5. Condicao
    // Verificar que as ligacoes sao reciprocas
    //Ciclo for "i" percorre os varios locais
    for(i=0;i<size;i++){
        //ciclo for "j" percorre as ligacoes do local onde se encontra
        for(j=0;j<3;j++){
            //Se a ligacao for -1 avanca pois nao liga a nada
            if(paux[i].liga[j] != -1){
                ligacao = paux[i].liga[j];
                identif=paux[i].id; 
                //ciclo for "k" percorre novamente o vetor a procura do id da ligacao
                for(k=0; k<size; k++){
                    //se encontrar esse id da ligacao vai procurar se esta ligado reciprocamente ao identif
                    if(paux[k].id == ligacao){
                        for(l=0; l<3; l++){
                            //procurar se o id da ligacao esta ligado reciprocamente ao identif mete a flag a 1
                            if(paux[k].liga[l] == identif)
                                flag=1;
                        }
                        
                        if (flag==0){
                            printf("Ficheiro nao e valido pois o local com id=%d nao esta ligado ao local com id=%d.\n",identif,ligacao);
                            return 1;
                        }
   
                        //Zerar novamente o valor da flag
                        flag=0;
                    }
                }
            }
        }    

    }
    
    //caso tudo esteja bem faz o printf e devolve 0
    printf("Locais validados: a 1a fase de preparacao foi concluida com sucesso.\n");
    return 0;
}

int pedenometxt(char * fileName2){
    FILE *f;
    char *extensao;
    
    do{
        printf("\nInsira o nome do ficheiro de pessoas a abrir(ex: pessoasA.txt):");
        scanf("%s",fileName2);   
        
        //verificar que tem a extensao pretendida .txt
        extensao = strrchr(fileName2,'.'); //apanha os carateres a partir do ponto
        
        if(strcmp(extensao, ".txt") !=0){
            printf("O ficheiro nao tem a extensao adequada. Introduzir novamente?");
            if(resposta()==1)
                continue; //pede ao user para introduzir novamente o nome do ficheiro
            else
                return 1; //devolve 1 e depois termina
        }
        
        //abrir o ficheiro
        f=fopen(fileName2,"rt");
        
        if(f==NULL){
            printf("Erro no acesso ao ficheiro. Introduzir novamente?");
            if(resposta()==1)
                continue; //pede ao user para introduzir novamente o nome do ficheiro
            else
                return 1; //devolve 1 e depois termina
        }
        
        else{
            fclose(f);
            return 0;
        }
        
    }while(1); //executa continuamente enquanto nada fizer o return. Ciclo do-while infinito
    
}

pno carregatxt(pno lista,int *tam, int ct, int *flag2){
    
    char fileName2[MAX]; //nome para ficheiro txt de pessoas
    pessoas aux; //estrutura pessoas auxiliar para ler de acordo com o txt
    FILE *f;
    pno novo = NULL; //ponteiro para pessoa auxiliar
    
    //Pedir o nome do ficheiro ao user
    if(pedenometxt(fileName2)==1){
        printf("Nao foi possivel iniciar a simulacao.\n");
        return NULL;
    }

    //Ler ficheiro
    f= fopen(fileName2,"rt");
    if(f==NULL){
        printf("Erro no acesso ao ficheiro\n");
        return NULL;
    }
    
        while((fscanf(f,"%s %d %s", aux.nome, &aux.idade, aux.estado) ==3) && ((*tam)<ct)) 
            // Aloca novo nó para pessoa
            // Se houver mais pessoas que a capacidade total do espaco ignora as restantes
        {  
            novo = malloc(sizeof(pessoas));
            if (novo == NULL)
            {
                printf("Erro na alocação de memória para pessoa!\n");
                return NULL;
            }
        
        
            if (strcmp(aux.estado,"I")==0 || strcmp(aux.estado,"S")==0){
                strcpy(novo->nome, aux.nome); // Preenche o novo nó com as variáveis lidas
                strcpy(novo->estado, aux.estado);
                novo->idade = aux.idade;
                novo->prox = lista;
          
                (*tam)++; // incrementa o nr de pessoas
            }
     
            if (strcmp(aux.estado,"D")==0)
            {
                fscanf(f,"%d",&aux.dias);
                if(aux.dias>0){
                 
                strcpy(novo->nome, aux.nome); // Preenche o novo nó com as variáveis lidas
                strcpy(novo->estado, aux.estado);
                novo->idade = aux.idade;
                novo->dias = aux.dias;
                novo->prox = lista;
                
                (*tam)++;               
                }
            }
            
            else if (strcmp(aux.estado,"D")!=0 && strcmp(aux.estado,"I")!=0 && strcmp(aux.estado,"S")!=0){
                printf("\nO ficheiro txt e invalido pois tem Estados diferentes de D,S e I.\n");
                printf("Nao foi possivel iniciar a simulacao.\n\n");
                (*flag2)=1;
                return NULL;
            }
            
            lista = novo; //insercao na cabeca da lista
        }
        
  fclose(f);
  return lista; 
}

void escrevepessoas(pno lista){
    if(lista==NULL) //o ficheiro nao tinha pessoas para ler para lista
        return;
    
    printf("\nPESSOAS CARREGADAS:\n\n");
    while(lista != NULL){
        if (strcmp(lista->estado,"I")==0 || strcmp(lista->estado,"S")==0)
            printf("%s\t %d\t %s\n",lista->nome, lista->idade,lista->estado);
        if (strcmp(lista->estado,"D")==0)
            printf("%s\t %d\t %s\t %d\n",lista->nome, lista->idade,lista->estado, lista->dias);
        
       lista=lista->prox;
    }
    
    printf("\n");
}

int validapessoas(pno lista){
    
    if(lista==NULL) //o ficheiro nao tinha pessoas para ler para lista
        return 1;
    
    pno p1=lista; //ponteiro para a lista
    pno p2=NULL; //ponteiro auxiliar que vai percorrer a lista dos seguintes a p1
    
    char id[MAX]; //string para guardar o nome
    
    while(p1!=NULL){
        
        strcpy(id,p1->nome);
        p2=p1->prox;
        
        while(p2!=NULL){
            //printf(" %s %s \n",id,p2->nome);
            if(strcmp(p2->nome,id)==0){
                printf("\nO ficheiro txt nao e valido pois existem pessoas com o mesmo ID.\n");
                printf("Nao foi possivel iniciar a simulacao.\n\n");
                return 1;  
            }
            p2=p2->prox;
        }
        
        p1=p1->prox;
    }
    
    printf("Pessoas validadas: a 2a fase de preparacao foi concluida com sucesso.\n");
    return 0;
}

void inserepessoasnosespacos( plocalaux paux , pno lista, int size){
   
    int j; //geracao de numero aleatorio
    pno auxiliar; //ponteiro auxiliar de pessoa, que vai ligar-se ao vetor, enquanto a lista é percorrida
    
    if(paux==NULL || lista==NULL)
      return;
    
    //a lista vai diminuindo a medida que os nos vao sendo distribuidos pelo vetor dos locais
    while(lista != NULL){

        do{
            j= intUniformRnd(1, size); //local onde se vai inserir a pessoa
        } 
        while(paux[j-1].contador >= paux[j-1].capacidade);
    
        auxiliar = lista; //o auxiliar aponta para a lista
        lista = lista->prox; //o ponteiro de lista vai apontar para o proximo da lista
        
        auxiliar->prox = paux[j-1].pess; //insercao na cabeca da lista. O auxiliar prox vai apontar o ponteiro do vetor
        paux[j-1].pess = auxiliar; // o ponteiro pessoa do vetor passa a apontar para o auxiliar
    
        paux[j-1].contador++; //incrementa o contador 
    }
    
    free(lista);
    lista=NULL;

}

void eliminarlista(pno lista){
    pno aux;
    
    while(lista != NULL){
        aux = lista;
        lista = lista->prox;
        free(aux);
    }
    
    return;
}

void apresentaestatistica(plocalaux paux,int size, int ct, int tam){
    plocalaux p= paux; //ponteiro para local auxiliar
    pno p1=NULL; //ponteiro para pessoas auxiliar;
    int i;
    float d=0.0, s=0.0, imu=0.0;
    int doentes=0, saudaveis=0, imunes=0;
    float pdoentes, psaudaveis, pimunes;
    
    printf("\n\n***************************** ESTATISTICAS *****************************\n\n");
    printf("\nCAPACIDADE TOTAL DO ESPACO: %d\nNUMERO DE LOCAIS: %d\nNUMERO DE PESSOAS EM ESTUDO: %d\n", ct, size,tam);
   
    printf("\n------------------------------------------------------------------------\n");
    for(i=0;i<size;i++){
        
        printf("\nLOCAL %d\tCAPACIDADE-%d pessoas\tOCUPACAO-%d pessoas\n",p[i].id,p[i].capacidade,p[i].contador);
        p1 = p[i].pess;
        
        while(p1!=NULL){
            
            if(strcmp(p1->estado, "D")==0){
                printf("%s\t%d\t%s\t%d\n",p1->nome,p1->idade,p1->estado,p1->dias);
                doentes++;
                d++;
            }

            else if(strcmp(p1->estado,"S")==0 || strcmp(p1->estado,"I")==0){
                printf("%s\t%d\t%s\n",p1->nome,p1->idade,p1->estado);  
                if(strcmp(p1->estado,"S")==0){
                    saudaveis++;
                    s++;
                }
                else{
                    imunes++;
                    imu++;
                }
            }
            
        p1=p1->prox;
           
        }
    
        printf("\nPessoas Doentes: %d\n", doentes);
        printf("Pessoas saudaveis: %d\n", saudaveis);
        printf("Pessoas imunes: %d\n", imunes); 
        printf("\n------------------------------------------------------------------------\n");
        
        doentes=saudaveis=imunes=0;
     
    }
    
        pdoentes=(d/tam)*100;
        psaudaveis=(s/tam)*100;
        pimunes=(imu/tam)*100;
        
        printf("\nTOTAL PESSOAS DOENTES: %1.0f\t%1.2f pct\n", d,pdoentes);
        printf("TOTAL PESSOAS SAUDAVEIS: %1.0f\t%1.2f pct\n", s, psaudaveis);
        printf("TOTAL PESSOAS IMUNES: %1.0f\t%1.2f pct\n", imu, pimunes); 
        
        printf("\n\n*************************** FIM ESTATISTICAS ***************************\n\n");
}

int verificaIDnovo(plocalaux paux, char *nome,int size){
    
    int i;
    int flag=0;
    pno auxiliar; //ponteiro para pessoas auxiliar
    
    for(i=0;i<size;i++){
        auxiliar=paux[i].pess;
                
        while(auxiliar != NULL){     
            if(strcmp(auxiliar->nome,nome)==0){
                printf("Nao e possivel adicionar doente pois o ID ja existe.\n");
                return 1;
            }
        auxiliar=auxiliar->prox;
        } 
    }
    
    return 0;
}

void adicionadoente(plocalaux paux, int *tam, int size, int *iteracoes){
    pno novo; //ponteiro para pessoas
    plocalaux p = paux; //ponteiro para local auxiliar
    int temp; //variavel auxiliar para receber id do user
    int i;
    int flag;
    char nome[MAX];
     
    printf("Insira o ID do local: ");
    scanf("%d",&temp);
    
    for(i=0;i<size;i++){
        if(p[i].id == temp && (p[i].capacidade > p[i].contador)){
            
            novo=malloc(sizeof(pessoas));
            if(novo==NULL){
            printf("Nao e possivel adicionar doente");
            return;
            }
        
            printf("Insira o nome do doente: ");
            scanf("%s",nome);
            // Verificacao que o ID e unico:
            flag=verificaIDnovo(paux,nome,size);
            if(flag==1){
                free(novo);
                return;
            }
            strcpy(novo->nome,nome);
                    
            do{
            printf("Insira a idade do doente: ");
            scanf("%d",&novo->idade);                
            } while(novo->idade <0);
            
            do{
                printf("Insira o numero de dias da infecao: ");
                scanf("%d",&novo->dias);
            }while(novo->dias <0 || novo->dias >= ((novo->idade)/10 + 5));

            strcpy(novo->estado,"D");
       
            atualizalistasparaiteracoes(paux,size);
            (*iteracoes)++;
            
            novo->prox = p[i].pess;
            p[i].pess = novo;
            
            p[i].contador++;
            (*tam)++;
             
            return;
        }               
     }
    
    printf("Nao foi possivel adicionar o doente\n");
     
}  

void incrementadiasmudaestado(pno p1, float taxaimune){
    
    float probrecuperar;      // 1/idade
    int nr;  //probabilidade da probrecuperar ocorrer (0 ou 1)
    int nr1; //probabilidade de taxaimune ocorrer(0 ou 1)
    
        while(p1 != NULL){
            if(strcmp(p1->estado,"D")==0){
               (p1->dias)++; 
                probrecuperar = (1.0/(p1->idade));
                nr = probEvento(probrecuperar); 
                //printf("Probabilidade de recuperar: %1.2f Nr gerado: %d\n", probrecuperar, nr);
                if(((p1->dias) > (5+ floor((p1->idade)/10))) || (nr==1) )
                {
                    nr1= probEvento(taxaimune);
                    //printf("Probabilidade de ficar imune: %d\n",nr1);
                    if(nr1==1)
                        strcpy(p1->estado,"I");
                    else
                        strcpy(p1->estado,"S");
                } 
            }
            p1=p1->prox;
        }
}


void avancaiteracao(plocalaux paux, int tam, int size, int *iteracoes){
  
    plocalaux p = paux; //ponteiro para local auxiliar
    pno p1, p2; //ponteiro para pessoas auxiliar;
    int i;
    float taxadissemin;      // floor(0.05*numero de pessoas na sala)
    float taxaimune = 0.2;  // apos recuperar ou maxdias
    int j; //contador de ciclo
    int nr; //gerar numero random
    int conta=0; //identifica o nr da pessoa na lista
    int contaaux=1; //contador de nr da pessoa na lista secundario
    
    atualizalistasparaiteracoes(paux,size);
    (*iteracoes)++;
    
    for(i=0; i<size;i++){
        
        p1 = p[i].pess; //o ponteiro auxiliar de pessoas passa a apontar para o ponteiro de pessoas q esta no vetor
        p2 = p[i].pess; //segundo ponteiro auxiliar para percorrer o ponteiro de pessoas q esta no vetor
        taxadissemin = floor(0.05*p[i].contador); //calcula o nr pessoas que um doente infeta e arredonda para baixo
        
        //AUMENTAR DIAS A ITERACAO E ATUALIZAR O ESTADO SE NECESSARIO
        incrementadiasmudaestado(p1, taxaimune);
        
        //APLICAR A TAXA DE DISSEMINACAO
        while(p1 != NULL){
            conta++;
            
            //tem que ser uma pessoa doente a infetar, nao pode ser um novo doente infetado nesta iteracao(dias>1)
            if(strcmp(p1->estado,"D")==0 && (p1->dias >=1)){    
                for(j=1;j<=taxadissemin;j++){
                     //gerar numero aleatorio para escolher a/as pessoa/as a infetar
                    do{
                        nr= intUniformRnd(1, p[i].contador);
                    }while(nr==conta); //o doente nao se pode infetar a ele proprio
                     
                    //com um ponteiro auxiliar vai procurar a pessoa a infetar
                    while(contaaux != nr){
                        contaaux++;
                        p2=p2->prox;
                    }
                    
                    //passa para aqui quando encontrou a pessoa a infetar
                    //so as pessoas que estao saudaveis e que contraem a doenca
                    if(strcmp(p2->estado,"S")==0){
                        strcpy(p2->estado,"D");
                        p2->dias = 0;
                    }
                              
                    //reinicia o contador auxiliar
                    contaaux=1;
                    //volta a apontar o p2 para o inicio da lista 
                    p2 = p[i].pess;   
                }   
            }
        
            //segue para a proxima pessoa da lista
            p1=p1->prox;
        }
    }      
}

void listapessoas(plocalaux paux, int size){
    int i;
    pno p1=NULL; //ponteiro de pessoas auxiliar
    
    printf("********************************************************************\n");
    printf("LISTA DE PESSOAS NOS DIFERENTES LOCAIS:\n\n");
    for(i=0; i<size; i++){
        printf("LOCAL ID: %d\tCAPACIDADE: %d\tCONTADOR:%d\n",paux[i].id,paux[i].capacidade,paux[i].contador);
        p1 = paux[i].pess;
        
        while(p1 != NULL){
            if(strcmp(p1->estado, "D")==0){
                printf("%s\t%d\t%s\t%d\n",p1->nome,p1->idade,p1->estado,p1->dias);
            }

            else if(strcmp(p1->estado,"S")==0 || strcmp(p1->estado,"I")==0){
                printf("%s\t%d\t%s\n",p1->nome,p1->idade,p1->estado); 
            }
            
            p1=p1->prox;
         }
        printf("\n");
    }
    printf("********************************************************************\n");   
}

void listapessoasp1(plocalaux paux, int size){
    int i;
    pno p1=NULL; //ponteiro de pessoas auxiliar
    
    printf("********************************************************************\n");
    printf("LISTA DE PESSOAS NOS DIFERENTES LOCAIS:\n\n");
    for(i=0; i<size; i++){
        printf("LOCAL ID: %d\tCAPACIDADE: %d\n",paux[i].id,paux[i].capacidade);
        p1 = paux[i].p1;
        
        while(p1 != NULL){
            if(strcmp(p1->estado, "D")==0){
                printf("%s\t%d\t%s\t%d\n",p1->nome,p1->idade,p1->estado,p1->dias);
            }

            else if(strcmp(p1->estado,"S")==0 || strcmp(p1->estado,"I")==0){
                printf("%s\t%d\t%s\n",p1->nome,p1->idade,p1->estado); 
            }
            
            p1=p1->prox;
         }
        printf("\n");
    }
    printf("********************************************************************\n");   
}

void listapessoasp2(plocalaux paux, int size){
    int i;
    pno p1=NULL; //ponteiro de pessoas auxiliar
    
    printf("********************************************************************\n");
    printf("LISTA DE PESSOAS NOS DIFERENTES LOCAIS:\n\n");
    for(i=0; i<size; i++){
        printf("LOCAL ID: %d\tCAPACIDADE: %d\n",paux[i].id,paux[i].capacidade);
        p1 = paux[i].p2;
        
        while(p1 != NULL){
            if(strcmp(p1->estado, "D")==0){
                printf("%s\t%d\t%s\t%d\n",p1->nome,p1->idade,p1->estado,p1->dias);
            }

            else if(strcmp(p1->estado,"S")==0 || strcmp(p1->estado,"I")==0){
                printf("%s\t%d\t%s\n",p1->nome,p1->idade,p1->estado); 
            }
            
            p1=p1->prox;
         }
        printf("\n");
    }
    printf("********************************************************************\n");   
}

void listapessoasp3(plocalaux paux, int size){
    int i;
    pno p1=NULL; //ponteiro de pessoas auxiliar
    
    printf("********************************************************************\n");
    printf("LISTA DE PESSOAS NOS DIFERENTES LOCAIS:\n\n");
    for(i=0; i<size; i++){
        printf("LOCAL ID: %d\tCAPACIDADE: %d\n",paux[i].id,paux[i].capacidade);
        p1 = paux[i].p3;
        
        while(p1 != NULL){
            if(strcmp(p1->estado, "D")==0){
                printf("%s\t%d\t%s\t%d\n",p1->nome,p1->idade,p1->estado,p1->dias);
            }

            else if(strcmp(p1->estado,"S")==0 || strcmp(p1->estado,"I")==0){
                printf("%s\t%d\t%s\n",p1->nome,p1->idade,p1->estado); 
            }
            
            p1=p1->prox;
         }
        printf("\n");
    }
    printf("********************************************************************\n");   
}

void verificacondicoestransf(plocalaux paux,int size,int qtd,int idinicial,int idfinal,int *flag1,int *flag2,int *flag3,int *flag4){
    
    int i, j; //auxiliares ciclo for
    for(i=0;i<size;i++){

        //Procura ID Local de Origem
        if(paux[i].id == idinicial){
            //1. Verifica se encontra o id de origem e a quantidade de pessoas que tem
           if(paux[i].contador>=qtd) 
               (*flag1)=1;
        
            //2. Verifica se os locais tem ligacao direta
           for(j=0;j<3;j++){
               if(paux[i].liga[j]==idfinal)
                   (*flag3)=1;
           }    
        }
        
        //Procura ID Local de Destino
        else if(paux[i].id ==idfinal){
            //3. Verifica se encontra o id de origem e a capacidade que tem
            if((paux[i].contador + qtd)<= paux[i].capacidade){
                (*flag2)=1;
            }
            
            //2. Verifica se os locais tem ligacao direta
            for(j=0;j<3;j++){
               if(paux[i].liga[j]==idinicial)
                  (*flag4)=1;
           }
        }    
    }  
}

void efetuatransferencia(plocalaux paux,int size,int qtd, int idinicial,int idfinal){
    
    //Ciclo de transferencia
    int i, j, k; //auxiliares ciclo for
    pno p1; // ponteiro pessoa que anda a frente
    pno anterior; // ponteiro pessoa que vai atras
    int nr; //nr aleatorio
    int conta=1; //contador auxiliar para encontrar a pessoa na posicao gerada pelo nr

    for(i=0;i<size;i++){
        
        //Procura ID Local de Origem
        if(paux[i].id == idinicial){
           
            //Para a quantidade de pessoas a mover vai transferindo uma a uma
            for(j=1;j<=qtd;j++){
                //inicializa os ponteiros
                p1=paux[i].pess;
                anterior=NULL;
                
                //gera nr aleatorio para obter a posicao da pessoa a transferir
                nr= intUniformRnd(1, paux[i].contador);
                
                //Se a pessoa a transferir for a primeira da lista
                if(conta == nr){
                    anterior=p1->prox;
                    paux[i].pess=anterior;
                }
                
                //Se a pessoa a transferir nao for a primeira da lista
                else{
                    //incrementa o contador ate encontrar a pessoa a transferir, que esta na posicao "nr"
                    //e vai avancando com os ponteiros anterior e p1. O anterior anda sempre atras do p1
                    while(conta != nr){
                    conta++; 
                    anterior=p1;
                    p1=p1->prox;
                    }
                
                    //avanca para aqui quando ja encontrou a pessoa a transferir
                    conta=1; //volta a inicializar o contador
                
                    //O anterior passa a apontar para o proximo do p1
                    anterior->prox=p1->prox;
                }
                
                //o p1 tem que ser inserido na cabeca da lista do Local ID Destino
                for(k=0;k<size;k++){
                    if(paux[k].id == idfinal){
                        p1->prox = paux[k].pess;
                        paux[k].pess = p1;
                        
                        paux[k].contador++;
                    }
                }
                
                paux[i].contador--;
            } 
        } 
    }
}

void transferepessoas(plocalaux paux, int size, int *iteracoes){

    int qtd; //quantidade de pessoas a mover
    int idinicial; //ID Origem
    int idfinal;  // ID Destino
    int flag1=0, flag2=0, flag3=0, flag4=0; //flags auxiliares para verificar condicoes transferencia
    
    do{
        printf("Insira a quantidade de pessoas a mover: ");
        scanf("%d",&qtd);  
    }while(qtd<=0);

    do{
        printf("Insira o ID de origem: ");
        scanf("%d",&idinicial);
    }while(idinicial<=0);
    
    do{
        printf("Insira o ID de destino: ");
        scanf("%d",&idfinal);
    }while(idfinal<=0);
    
    verificacondicoestransf(paux,size,qtd,idinicial,idfinal,&flag1,&flag2,&flag3,&flag4);
    if(flag1==0 || flag2==0 || flag3==0 || flag4==0){
        printf("Nao e possivel efetuar a transferencia de %d pessoas do local ID %d para o local ID %d.\n",qtd,idinicial,idfinal);
        return;
    }
    
    atualizalistasparaiteracoes(paux,size);
    (*iteracoes)++;
    efetuatransferencia(paux,size,qtd,idinicial,idfinal);
}

void atualizalistasparaiteracoes(plocalaux paux, int size){
    
    pno novo;
    pno p;
    pno auxiliar;
    pno eliminar = NULL;
    int i;
    
    //Atualizar a lista eliminar para P3
    //Atualizar a P2 para P3
    //Eliminar a antiga P3
    //Atualizar a P1 para P2
    //Fazer copia da lista atual para P1. 
    for(i = 0;i < size; i++){
        
        p = paux[i].pess;
        
        eliminar = paux[i].p3;
        paux[i].p3 = paux[i].p2;
        eliminarlista(eliminar);
        paux[i].p2 = paux[i].p1;
        paux[i].p1=NULL;
        
        while(p!=NULL){
            novo = malloc(sizeof(pessoas));
            if (novo == NULL){
                printf("Erro na alocacao de memoria.\n");
                return;
            }
            
            else if (strcmp(p->estado,"I")==0 || strcmp(p->estado,"S")==0){
                strcpy(novo->nome, p->nome); // Preenche o novo nó com as variáveis lidas
                strcpy(novo->estado, p->estado);
                novo->idade = p->idade;
                novo->prox = NULL; 
            }
            
            else if (strcmp(p->estado,"D")==0){
                strcpy(novo->nome, p->nome); // Preenche o novo nó com as variáveis lidas
                strcpy(novo->estado, p->estado);
                novo->idade = p->idade;
                novo->dias = p->dias;
                novo->prox = NULL; 
            } 
            
            //Inserir novo no no final para garantir que fica igual a lista original:
            if(paux[i].p1==NULL){
                paux[i].p1 = novo;
            }
            else{
                auxiliar= paux[i].p1;
                while(auxiliar->prox != NULL){
                    auxiliar = auxiliar->prox;
                }
                auxiliar->prox = novo;
            }
            
            p = p->prox;
        }
    }
      
//    printf("Lista P1:\n");
//    listapessoasp1(paux,size);
//    printf("Lista P2:\n");
//    listapessoasp2(paux,size);
//    printf("Lista P3:\n");
//    listapessoasp3(paux,size);
}

void retrocedeiteracoes(plocalaux paux,int size, int iteracoes){
    int nr;
    int i; 
    int conta=0;
    pno p; 
    pno auxiliar;
    
    do{
       printf("Introduza o numero de iteracoes que pretende retroceder: ");
        scanf("%d",&nr);  
    }while(nr<1 || nr>3);
    
    //So retrocede se ja tiver o numero de iteracoes realizadas
    if( nr > iteracoes){
        printf("Nao e possivel retroceder %d iteracao/oes pois ainda foram efetuadas %d iteracoes.\n",nr,iteracoes);
        return;
    }
    
    //Trocar os ponteiros
    for(i=0; i<size; i++){
        if(nr==1)
        {
            auxiliar = paux[i].p1;
            paux[i].p1 = paux[i].pess;
            paux[i].pess=auxiliar;
        }
        
        if(nr==2)
        {
            auxiliar = paux[i].p2;
            paux[i].p2 = paux[i].p1;
            paux[i].p1 = paux[i].pess;
        
            paux[i].pess=auxiliar;
        }
        
        if(nr==3)
        {
            auxiliar = paux[i].p3;
            paux[i].p3 = paux[i].p2;
            paux[i].p2 = paux[i].p1;
            paux[i].p1 = paux[i].pess;
        
            paux[i].pess=auxiliar;
        }
    }

    //Atualizar contador

    for(i=0; i<size; i++){
          p=paux[i].pess;
          
          while(p!=NULL){
              conta++;
              p=p->prox;
          }
          
          paux[i].contador=conta;
          conta=0;
    }

}

void guardapessoastxt(plocalaux paux, int size){
    
    char filename1[MAX];
    char *extensao;
    
    do{
        printf("Introduza o nome do ficheiro txt (Ex: pessoas.txt): ");
        scanf("%s",filename1);  
        
        extensao = strrchr(filename1,'.'); //apanha os carateres a partir do ponto 
        
    }while(strcmp(extensao, ".txt") !=0); //garantir que tem a extensao pretendida .txt

    //Guarda a informacao das pessoas num txt
    FILE *f = fopen(filename1,"wt");
    if(f==NULL){
        fprintf(stderr,"Erro ao escrever o ficheiro de pessoas.\n");
        return;
    }
    
    //Escrita para o ficheiro
    int i;
    pno p;
    
    for(i=0; i<size; i++){
        p = paux[i].pess;
        
        while(p!=NULL){
            if(strcmp(p->estado, "D")==0){
                fprintf(f,"%s %d %s %d\n",p->nome,p->idade,p->estado,p->dias);
            }
            
            else if(strcmp(p->estado,"S")==0 || strcmp(p->estado,"I")==0){
                fprintf(f,"%s %d %s\n",p->nome,p->idade,p->estado);
            }
            
            p=p->prox;
        }
        
    }
    
    fclose(f);
    printf("As pessoas foram corretamente guardadas no ficheiro %s.\n",filename1);
    return;
}

void elaborarelatorio(plocalaux paux,int size,int ct,int tam){
    
    int i;
    int doentes=0,saudaveis=0,imunes=0;
    float d=0.0, s=0.0, imu=0.0;
    float pdoentes=0.0, psaudaveis=0.0, pimunes=0.0;
    pno p1=NULL;
    char *filename2= {"report.txt"};
    
        FILE *f=fopen(filename2,"wt");
    if(f==NULL){
        fprintf(stderr, "Erro ao elaborar o relatorio.\n");
        return;
    }
    
    fprintf(f,"\n***************************** RELATORIO *****************************\n");
    fprintf(f,"\nCAPACIDADE TOTAL DO ESPACO: %d\nNUMERO DE LOCAIS: %d\nNUMERO DE PESSOAS: %d\n", ct, size,tam);
    fprintf(f,"\n------------------------------------------------------------------------\n");
    
    for(i=0;i<size;i++){
        fprintf(f,"\nLOCAL %d\tCAPACIDADE-%d pessoas\tOCUPACAO-%d pessoas\n",paux[i].id,paux[i].capacidade,paux[i].contador);
        p1 = paux[i].pess;
        
        while(p1!=NULL){
            
            if(strcmp(p1->estado, "D")==0){
                fprintf(f,"%s\t%d\t%s\t%d\n",p1->nome,p1->idade,p1->estado,p1->dias);
                doentes++;
                d++;
            }

            else if(strcmp(p1->estado,"S")==0 || strcmp(p1->estado,"I")==0){
                fprintf(f,"%s\t%d\t%s\n",p1->nome,p1->idade,p1->estado);  
                if(strcmp(p1->estado,"S")==0){
                    saudaveis++;
                    s++;
                }
                else{
                    imunes++;
                    imu++;
                }
            }
            
        p1=p1->prox;
           
        }
    
        fprintf(f,"\nPessoas Doentes: %d\n", doentes);
        fprintf(f,"Pessoas saudaveis: %d\n", saudaveis);
        fprintf(f,"Pessoas imunes: %d\n", imunes); 
        fprintf(f,"\n------------------------------------------------------------------------\n");
        
        doentes=saudaveis=imunes=0;
     
    }
    
        pdoentes=(d/tam)*100;
        psaudaveis=(s/tam)*100;
        pimunes=(imu/tam)*100;
        
        fprintf(f,"\nTOTAL PESSOAS DOENTES: %1.0f\t%1.2f pct\n", d,pdoentes);
        fprintf(f,"TOTAL PESSOAS SAUDAVEIS: %1.0f\t%1.2f pct\n", s, psaudaveis);
        fprintf(f,"TOTAL PESSOAS IMUNES: %1.0f\t%1.2f pct\n", imu, pimunes); 
        
        fprintf(f,"\n\n*************************** FIM RELATORIO ***************************\n\n");
        
        printf("Relatorio Elaborado com sucesso no ficheiro %s. Ate a proxima!\n",filename2);
        printf("\n************************* FIM DA SIMULACAO ************************\n\n");
}

void relatorio(plocalaux paux, int size, int ct, int tam){
    
    int i;
    
    guardapessoastxt(paux,size);
    
    elaborarelatorio(paux,size,ct,tam);
    
    //Eliminar listas ligadas
    for(i=0; i<size; i++){
        eliminarlista(paux[i].pess);
        eliminarlista(paux[i].p1);
        eliminarlista(paux[i].p2);
        eliminarlista(paux[i].p3);
    }
    
    //Libertar o vetor
    free(paux); 

}

int menu(){
    
    int i;
    puts ("1 - Avanca Iteracao:");
    puts ("2 - Apresenta Estatistica:");
    puts ("3 - Adiciona Doente:");
    puts ("4 - Transferir Pessoas");
    puts ("5 - Lista Pessoas");
    puts ("6 - Retroceder Iteracoes");
    puts ("7 - Terminar Simulacao");
   
    do{
        scanf("%d", &i);
    }while(i < 1 || i > 7);
    return i;

}


