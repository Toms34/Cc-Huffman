/**
 * @file huff.c
 * @brief Compresseur de Huffman d'un fichier passé en paramètre, génere le codage dans un autre fichier passé en paramètre
 * @author Ryan Bengoufa et Thomas Bergon
 * @version 0.1
 * @date 13/12/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**
 * @brief structure d'un noeud d'un arbre avec le nombre d'occurence du caratère à son indice ascii,
 * l'indice de son parent et de ses fils gauche et droit
*/
typedef struct{
  int nbOcc;
  int gauche;
  int droit;
  int parent;
} noeud;

/**
 * @brief structure de mon index pour le transmettre dans le fichier compresser
 * avec l'indice ascii et son code binaire en chaine de caratère
*/
typedef struct{
  unsigned char ascii;
  char *c;
} indexi;


/**
 * @brief ajout d'un caractere c à la fin d'une chaine de caratère s
 * @param s  chaine de caratère
 * @param c  caratère
*/
void ajoutChar(char* s, char c) { // ajoute un caractere
  int len = strlen(s);
  s[len] = c;
  s[len+1] = '\0';
}

/**
 * @brief  inverse une chaine de caratère
 * @param s  chaine de caratère
*/
void reverse(char *s){ // renverse une chaine de caractere
  int len = strlen(s), i, j;
  char p[len];

  for(i=0; i<len; i++){
    p[i] = s[i];
  }
  j = 0;
  for (i=len-1; i>=0;i--)
	{
		s[j++] = p[i];
	}
  s[j] = '\0';
}


/**
 * @brief encodage du fichier avec la methode de Huffman
 * @param fichiers  fichier qui correspond à la source
 * @param fichiers  fichier qui correspond à la destination
 * @param tabBin  tableau de chaine de caractère qui contient les codes binaires des différents
 * code ascii qu'il y a dans le fichier source
*/
void encodage(char* fichiers ,char* fichierd,char** tabBin){
  FILE* fics = fopen(fichiers, "r"); // on a deja tester dans comptage l'ouverture
  FILE* ficd = fopen(fichierd, "wb"); // creer s'il n'existe pas

  int ascii, nbBit = 0;
  int octet = 0;

  // je creer l'index pour la decompression
  int nbCaractere = 0;
  int p = 0;
  while(p<256){
    if(tabBin[p] != NULL)
      nbCaractere++;
    p++;
  }

  indexi *ind = malloc(sizeof(indexi) * nbCaractere);

  int n=0;
  for(int i=0; i<256; i++){
    if(tabBin[i] != NULL){
      int size = strlen(tabBin[i]) + 1;
      ind[n].ascii = i;
      ind[n].c = (char*)malloc(size * sizeof(char));
      strcpy(ind[n].c, tabBin[i]);
      n++;
    }
  }
  // Fin index

  fwrite(&nbCaractere, sizeof(int), 1, ficd);

  for(int i=0; i<nbCaractere; i++){
    fwrite(&ind[i].ascii, sizeof(int), 1, ficd);
    fprintf(ficd, "%s\n", ind[i].c); // Copie de l'index
  }

  // On creer les octets que l'on ajoute au fichier
  while((ascii = fgetc(fics)) != EOF){
    int i =0;
    while(i<strlen(tabBin[ascii])){
      if(nbBit != 8){
        nbBit++;
        octet = octet | (tabBin[ascii][i]-'0');
        if(nbBit != 8)
          octet = octet << 1;
        i++;
      }else{
        fwrite(&octet, sizeof(char), 1, ficd);
        nbBit =0;
        octet = 0;
      }
    }
  }

  if(nbBit != 0 ){
    while(nbBit !=8){ // On complète le dernier octet avec des 0
      nbBit++;
      octet = octet | 0;
      if(nbBit !=8)
        octet = octet << 1;
    }
    fwrite(&octet, sizeof(char), 1, ficd);
  }

  fclose(fics);
  fclose(ficd);


  printf("Compression OK \n");

}

/**
 * @brief Ajoute le code binaire en suivant l'arbre de Huffman depuis la feuille donc le code est à l'envers
 * @param arbre  arbre de Huffman
 * @param feuille  code ascii de la feuille dont je recherche le code
 * @param chaine  chaine de caratère où sera stocké le code binaire
*/
void codeFeuille(noeud* arbre, int feuille, char* chaine){
  int i = 1;
  if(arbre[feuille].parent != -1){
    i++;
    chaine = realloc(chaine, i * sizeof(char));
    if(arbre[arbre[feuille].parent].droit == feuille){
      ajoutChar(chaine, '1');
    }else if(arbre[arbre[feuille].parent].gauche == feuille){
      ajoutChar(chaine, '0');
    }
    codeFeuille(arbre, arbre[feuille].parent, chaine);
  }else if(arbre[feuille].parent == -1 && arbre[feuille].droit == -1 && arbre[feuille].gauche == -1){ // Cas ou il n'y a que 1 caractere
    ajoutChar(chaine, '1');
  }

}

/**
 * @brief Cherche les code binaire necessaires pour coder le fichier et les enregistre dans un tableau
 * @param arbre  arbre de Huffman
 * @param tabBin  tableau avec les différents code bianires (vide avant d'avoir lancer la focntion)
*/
void codeBin(noeud* arbre, char** tabBin){
  for(int i=0; i < 256; i++){
    if(arbre[i].nbOcc >= 1){
      char *chaine = (char*)malloc(1 * sizeof(char));
      *chaine = '\0';
      codeFeuille(arbre, i, chaine);
      reverse(chaine);
      tabBin[i]=(char*)malloc((strlen(chaine))*sizeof(char));
      strcpy(tabBin[i], chaine);
      free(chaine);
    }
  }
}


/**
 * @brief compte le nombre de caractere différents et enregistre à l'indice ascii son nombre d'occurence
 * @param fichier  fichier en lecture
 * @param nbOcc  tableau de int
 * @return nbFeuille  le nombre de caractere différents dans le fichier
*/
int comptage(char* fichier, int* nbOcc){
  int nbFeuille = 0, ascii; // compte le nb de caractere différent
  FILE* fic = fopen(fichier, "r"); // ouverture du fichier passé en paramètre
  if(fic==NULL){
    fprintf(stderr, "Impossible d'ouvrir le fichier \n");
    exit (1);
  }

  if(fgetc(fic) == EOF){
    fprintf(stderr, "Fichier vide \n");
    exit(1);
  }

  rewind(fic); // on remet à zero le curseur pour reprendre le fgetc au début

  while((ascii=fgetc(fic))!=EOF){  // EOF macro égal à -1, pour ça que j'ai déclaré ascii en int et pas en char
    if(nbOcc[ascii] == 0){
      nbFeuille++;
    }
    nbOcc[ascii]++;// incrementation du compteur à l'indice ascii
  }


  fclose(fic);

  return nbFeuille;
}

/**
 * @brief affiche l'arbre binaire
 * @param arbre  arbre binaire
 * @param taille  taille de l'arbre
*/
void affichageArbre(noeud* arbre, int taille){
  for(int i=0; i<taille; i++){
      if(arbre[i].nbOcc > 0)
        printf(" %i : Parent : %i, gauche : %i, droit : %i, nbOcc = %i \n",i, arbre[i].parent, arbre[i].gauche, arbre[i].droit, arbre[i].nbOcc); // On affiche
  }
}


/**
 * @brief construit un arbre binaire avec les différents caratères enregistrer dans la fonction comptage
 * @param nbOcc  tableau de int avec les différents code ascii et leur fréquence
 * @param arbre  arbre binaire où sera stocker
 * @param taille  taille de l'arbre
*/
void construction(int* nbOcc, noeud* arbre, int taille){
  // initialisation de l'arbre avec la table ASCII entre 0 et 255 et le nbOcc - Puis les noeuds entre 256 et 510
  for(int i=0; i<taille; i++){
    arbre[i].parent = arbre[i].gauche = arbre[i].droit = -1; // on initialise le tout à -1;
    arbre[i].nbOcc = 0;
    if(i<256){
      arbre[i].nbOcc = nbOcc[i]; // recopie le nombre d'occurence au debut du tableau
    }
  }

  int idnoeud = 255, imin1, imin2, nbOcc1, nbOcc2;

  do{
    imin1 = imin2 = -1;
    nbOcc1 = nbOcc2 = 999999;
    // recherche du caractère avec le moins d'occurence possible
    for(int i=0; i<=idnoeud; i++){
      if(arbre[i].nbOcc > 0 && arbre[i].parent == -1 && arbre[i].nbOcc < nbOcc1){
        imin1 = i;
        nbOcc1 = arbre[i].nbOcc;
      }
    }

    for(int p=0; p<=idnoeud; p++){
      if( p != imin1 && arbre[p].nbOcc > 0 && arbre[p].parent == -1 && arbre[p].nbOcc < nbOcc2){
        imin2 = p;
        nbOcc2 = arbre[p].nbOcc;
      }
    }

    // On associe les deux trouvé
    if(imin1 != -1 && imin2 != -1){
      idnoeud++;
      arbre[idnoeud].droit = imin1;
      arbre[idnoeud].gauche = imin2;
      arbre[idnoeud].nbOcc = nbOcc1 + nbOcc2;
      arbre[imin1].parent = idnoeud; // Pour les retirer de la recherche
      arbre[imin2].parent = idnoeud; // Pour les retirer de la recherche
    }

  }while(imin1 != -1 && imin2 != -1);

}

/**
 * @brief fonction principal du compresseur
 * @param argc  nombre d'argument passé en paramètre
 * @param argv  tableau avec le nom des differents paramètres
*/
int main(int argc, char *argv[]) {
  if(argc != 3){
    fprintf(stderr, "Il faut fournir un argument (fichier) !");
    exit (1);
  }

  int nbNoeud;
  char** tabBin = (char**)malloc(sizeof(char*)*256); // Tableu qui va stocker les codes binaires
  int* nbOcc = (int*)malloc(sizeof(int)*256); // creation d'un tableau de la taille de l'alphabet ascii

  nbNoeud = comptage(argv[1], nbOcc);
  nbNoeud = nbNoeud + 256 - 1; //
  noeud* arbre = (noeud*)malloc(sizeof(noeud)*nbNoeud); // creation de l'arbre

  construction(nbOcc, arbre, nbNoeud);

  affichageArbre(arbre, nbNoeud);

  codeBin(arbre, tabBin);

  for(int y=0; y<256; y++){
    if(tabBin[y] != NULL)
      printf("Code(%c) : %s \n", y, tabBin[y]);
  }

  encodage(argv[1] , argv[2],tabBin);

  // Affichage taille des deux fichiers et le taux de compression
  struct stat f1, f2;
  stat(argv[1], &f1);
  stat(argv[2], &f2);
  float size1, size2, gain;
  size1 = f1.st_size;
  size2 = f2.st_size;
  gain = ((size2-size1)/size1)*100;
  printf("Taille fichier : source (%.2f) et destination(%.2f) \n", size1, size2);
  if(gain>0){
    printf("Augmentation de %.2f%% \n", gain);
  }else{
    printf("Diminition de %.2f%% \n", (gain*-1));
  }

  free(arbre);
  free(nbOcc);
  free(tabBin);

  return 0;
}
