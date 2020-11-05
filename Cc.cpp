// Cc.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <stdlib.h>
#include <string>
typedef struct data {
    int nb;
    char car;
}data;
void exc(data* tab,int nb,int m){
    data temp=tab[nb];
    tab[nb].nb,tab[nb].car=tab[m].nb,tab[m].car;
    tab[m].nb,tab[m].car=tab[nb].nb,tab[nb].car;
}
void ord(){
    /*a refaire */
}
int main(int argc, char**argv)
{
    //if (argc != 3) { std::cout << "nombre d'argument invalide" << std::endl; return 1; } // test nombre d'arguments
    std::ifstream fin; // fichier entrée en lecture seule
    std::ofstream fout; // fichier sortie en ecriture seule
    fin.open("text"); // ouverture du fichier entrée
    fout.open(argv[2]); // ouverture du fichier sortie
    if(!fin.is_open()){ std::cout << "fichier entrant invalide" << std::endl; return 1; } // test existance du fichier entrant
    std::string c;
    int max = 1;
    data* tab = (data*)malloc(sizeof(*tab));
    while (fin.good()) {
        getline(fin,c);
        for (size_t i=0;i<=c.size();i++){
            std::cout<<c[i]<<std::endl;
            for (int n = 0; n<=max; n++) {
                if (tab[n].car ==char(c[i])) { tab[n].nb+=1; }
                else if (n+1 == max) { tab[n++].car = c[i]; tab[n++].nb=1;max++; }
            }
        }
    }
    for(int i=0;i<max;i++){
         printf("%d \nOccurence :%d \n nombre max :%d \n",tab[i].car,tab[i].nb,max);
    }
    
}


