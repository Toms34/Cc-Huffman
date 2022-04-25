/**
 * @file dehuff.c
 * @brief Decompression de Huffman d'un fichier passé en paramètre sur la sortie standard
 * @author Ryan Bengoufa et Thomas Bergon
 * @version 0.1
 * @date 13/12/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief structure de mon index pour le recuperer du fichier compresser
*/
typedef struct{
  int ascii;
  char c[255];
} indexi;

int main(int argc, char *argv[]){
  FILE* file = fopen(argv[1], "r");
  int n;

  unsigned char octet;
  fread(&n, sizeof(int), 1, file);
  //printf("Ok pour n = %i \n", n);

  indexi *tab = malloc(sizeof(indexi) * n);
  int max,b=1;
  while(b<n){ //taille du plus grand nomber de bits pour un char
    b=b*2;
    max++;
  }

  for(int i=0; i<n; i++){
    fread(&tab[i].ascii, sizeof(int), 1, file);
    fscanf(file, "%s\n", tab[i].c);
    //printf("%i %s \n", tab[i].ascii, tab[i].c);
  }

  // ON a l'index
  int nbin_buffer=0; // nombre de bits dans le buffer
  unsigned int a=0; // utilisation quand on recupere un octect
  int result[8]; // utilisation quand l'octect est convertit en binaire (amelioration short int ?)
  //FILE* filesort=fopen("decompress.txt","w");
  int nboctect=0;
  while (!feof(file)){ //recup du nombre d'octect restant
      a=0;
      fread(&a,1,1,file);
      nboctect++;
  }
  fclose(file);
   

  FILE* file2=fopen(argv[1],"r"); // on reouvre le fichier
  char bin[255];
  int bin2;
  for(int i=0; i<n; i++){
    fread(&bin2, sizeof(int), 1, file);
    fscanf(file2, "%s\n",bin);
  }// on est de retour à la case départ

    nbin_buffer = 0;
    char *buffer=malloc(sizeof(char)*nboctect*8);// creation du buffer avec les bits à traduire
    while(!feof(file2)){
        a = 0;
        fread(&a, 1, 1, file2);
        int c=7;
        //printf("a :%d  ",a);
        while(c>=0){ // traduction de l'entier en binaire stocker dans result[].
            result[c]=a%2;
            a=a/2;
            c--;}
        for (int i = 0; i < 8; i++){ // on met la traduction dans le buffer
            buffer[nbin_buffer]=result[i];
            nbin_buffer++;
        }
    }
    fclose(file2);
    nbin_buffer=0;
    int find=0,taille=0,z=0;
    while(nbin_buffer+max<(nboctect-1)*8){
    find=0;taille=0;z=0;
        for(int alpha=0;alpha<n;alpha++) // parcour l'aphabet
        {
            taille=strlen(tab[alpha].c); // aussi la longueur en bits
            find=0;z=0;
            while(z<taille && find!=1){
              if(tab[alpha].c[z]-'0'!=buffer[nbin_buffer+z]){
                  find=1;// si jamais c diff on termine
              }
              z++;
            }
            if(find==0){
              printf("%c", tab[alpha].ascii);
              fflush(stdout);
              nbin_buffer+=taille;
            }
        }
    }
    printf("\n");

    free(tab);
    free(buffer);

}
