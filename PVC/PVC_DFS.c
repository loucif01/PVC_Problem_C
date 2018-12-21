//
// Created by Loucif on 11/11/18.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

#define max 20

int Matrix[max][max];
int nb_sommet=15;
int optimalPath[max];
int optimalCout;

int generate(int n);
void afficher2d(int tab2d[max][max], int n);
void DFS(int tab[max],int n);
int calculCout(int tab[max]);
void swap(int *m, int *n);
void afficher1d(int tab[max], int n);
void initialize(int tab[max], int n);
void copyTable(int tab1[max], int n, int tab2[max]);
void cycleOptimal(int nb);

void cycleOptimal(int nb){
    nb_sommet = nb;
    generate(nb);
    printf("\nLa Matrix generee aleatoirement de taille %d x %d\n", nb, nb);
    afficher2d(Matrix, nb);
    int tab[max];
    initialize(tab, nb);
    DFS(tab, nb);
    printf("\nCycle Optimal: ");
    afficher1d(optimalPath,nb+1);
    printf("\nAvec un cout optimal de :  %d", optimalCout);
}
int main(){
    struct timeval tv1,tv2;
    long temps;
int n;
    printf("le probleme du voyageur de commerce Utilisant DFS.\nVeillez entrez le nombre maximal de sommets(ne pas depasser 20): ");
    scanf("%d", &n);
    for (int i = 4; i <= n; ++i) {
        gettimeofday(&tv1,NULL);
        cycleOptimal(i);
        gettimeofday(&tv2,NULL);
        temps=(tv2.tv_usec-tv1.tv_usec);
        printf("\nPour %d sommets, l'algorithme s'excecute en %ld micro-secondes",
                i, temps);
        printf("\n******************************************");
    }

    return 0;
}
int generate(int n){
    srand((unsigned int) time(NULL));
    //La diagonal
    for (int i = 0; i < n; ++i) {
        Matrix[i][i] = 0;
    }
    //La partie inferieur
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= i; ++j) {
            if(i == j){
                Matrix[i][j]=0;
            } else{
                Matrix[i][j]= rand() % 10 + 1;
                Matrix[j][i] = Matrix[i][j];
            }
        }
    }
    return 0;
}

void afficher2d(int tab2d[max][max], int n){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%4d", tab2d[i][j]);
        }
        printf("\n");
    }
}

int calculCout(int tab[max]){
    int somme = 0;
    for(int i=0; i<nb_sommet; i++){
        somme = somme+ Matrix[tab[i]][tab[(i+1)%nb_sommet]];
    }
    return somme;
}

void swap(int *m, int *n){
    int tmp = *m;
    *m = *n;
    *n = tmp;
}
void afficher1d(int* tab, int n){
    printf("\n");
    for (int i = 0; i < n; ++i) {
        printf("%d, ", tab[i]);
    }
}


void DFS(int tab[max],int n){

    if(n==1){
        if(calculCout(tab) < optimalCout){
            optimalCout = calculCout(tab);
            copyTable(tab, nb_sommet+1, optimalPath);
        }
        return;
    }
    for (int i = 0; i < n-1; ++i) {
        swap(&tab[i+1], &tab[n-1]);
        DFS(tab, n - 1);
        swap(&tab[i+1], &tab[n-1]);
    }
}

void copyTable(int tab1[max], int n, int tab2[max]){
    for (int i = 0; i < n; ++i) {
        tab2[i] = tab1[i];
    }
}


void initialize(int tab[max], int n){
    for (int i = 0; i < n; ++i) {
        tab[i]=i;
    }
    tab[n]=0;
    optimalCout = calculCout(tab);
}
