#include "LASSERRE_image.h"
//------------------------------------------------------------------------------
// Code source pour le projet d'UE035
// description : (les fonctions sont définit dans image.h)
//
// les pointeurs images dans ce code sont de type : struct fichierimage *
//
// fonction struct fichierimage * charger(char *)
// permet de charger une image presente sur le disque en memoire vive, la fonction
// renvoie un pointeur de type : struct fichierimage *
//
// fonction int enregistrer(struct fichierimage *,char *)
// permet d'enregistrer une image sur le disque sous le nom donné en arg2, cette
// image est contenue dans une pointeur de type : struct fichierimage * fournit en arg1
//
// fonction struct fichierimage * nouveau(int,int)
// permet de creer une image en memoire de largeur arg1 et de hauteur arg2, la fonction
// retourne un pointeur de type : struct fichierimage *
//------------------------------------------------------------------------------

int main()
{
// variable permettant le parcours d'une image
int i,j;

// exemple de déclaration d'un pointeur image
struct fichierimage *fichier=NULL;

// exemple pour effectuer un copier coller
fichier=charger("LASSERRE_imagebmp.bmp");


// Modification de l'image en image miroir
agrandissement(fichier,2);
free(fichier);



}
