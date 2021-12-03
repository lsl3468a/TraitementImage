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
	// exemple de déclaration d'un pointeur image
	struct fichierimage* fichier = NULL;
	char nomfic[100], nomfictraitee[100];
	char ch;
	float pourcentage = 0;
	int test = 0;
	printf("Veuillez entrer le nom de l'image avec l'extension .bmp:\n");
	scanf("%s", &nomfic);
	clearBuffer();
	//Afficher l'image
	printf("Pour pouvoir afficher l'image charg%ce appuyer sur une touche : \n", 130);
	ch = getchar();
	system(nomfic);
	fichier = charger(nomfic);
	printf("Veuillez entrer le nom que vous souhaitez donner %c l'image trait%ce : \n", 133, 130);
	scanf("%s", &nomfictraitee);
	clearBuffer();
	//Traitement de l'image
	menu(fichier, nomfictraitee);
	printf("Pour pouvoir afficher l'image charg%ce appuyer sur une touche : \n", 130);
	ch = getchar();
	system(nomfictraitee);
	free(fichier);

}

