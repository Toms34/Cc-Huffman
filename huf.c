#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int nb; // occurence
    char car; // caractere
} data;
void exc(data* tab,int nb,int m){
    data temp=tab[nb];
    tab[nb].nb,tab[nb].car=tab[m].nb,tab[m].car;
    tab[m].nb,tab[m].car=tab[nb].nb,tab[nb].car;
}
int ord(data* tab,int max){
    int nb,m;
    while(nb<max){
        m=nb+1;
        while (m<=max){
            
        }
        nb++;
        printf("%d\n",m);
    }    
}
int main(int argv, char **argc){
    if(argv!=3){ // huf , fichier entrant, fichier sortant
        printf("2 arguments attendus\n"); 
        return 1;
    }
    FILE *fe=fopen(argc[1],"r");
    if(fe==NULL){ // test existence fichier
        printf("fichier non existant\n");
        return 1;
    }    
    // printf("%c\n",fgetc(fe));
    FILE *fs=fopen(argc[2],"w"); // fichier sortie

    data *tab= malloc(sizeof(*tab)); // tableau dynamique contient pour chaque case un char et un nb
    int max=0;
    char c="";
    int lenght=0;
    while(!feof(fe)){
        c+=fgetc(fe); // recuperation du char
        lenght++;
        printf("%c\n",c);
    }
    while(lenght>0){
        int n=0; 
        int find=0;
        while(n<=max && find ){
            if(tab[n].car==c){
                tab[n].nb+=1;
                find=1;}
            n++;
        }

        if(find==0){
        max++;
        tab[max].car=c;
        tab[max].nb=1;
        lenght--;
        }
    }
    //exc(tab,0,1);
    //ord(tab,max);
    /*for(int i;i<=max;i++){
    printf("%d \nOccurence :%d \n nombre max :%d \n",tab[i].car,tab[i].nb,max);
    }*/
    return 0;
}
