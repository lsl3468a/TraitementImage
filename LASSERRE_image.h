#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef int matrice[3][3];
typedef int tablo[255];

// structure d'entete du fichier
struct entete_fichier
{
    char idformat[2];
    int taille_fichier;
    int reserve;
    int debut;
};

// structure d'entete de l'image
struct entete_bmp
{
    int taille_image;
    int largeur;
    int alignement;
    int hauteur;
    short plans;
    short profondeur;
    int compression;
    int taille_image_totale;
    int resolutionh;
    int resolutionv;
    int nbrcouleur;
    int nbrcouleuri;
};

// structure d'un pixel
struct pixels
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
};

// structure du fichier
struct fichierimage
{
    struct entete_fichier entetefichier;
    struct entete_bmp entetebmp;
    struct pixels image[5000][5000];
};

struct fichierimage* charger(char* nom)
{
    FILE* f;
    int i, j, k;
    unsigned char zero;
    unsigned char* buffer;
    int position = 0;
    int longueur = 0;
    struct fichierimage* fichier = (struct fichierimage*)malloc(sizeof(struct fichierimage));

    f = fopen(nom, "rb");
    fseek(f, 0, SEEK_END);
    longueur = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = (unsigned char*)malloc(longueur * sizeof(unsigned char));
    fread((unsigned char*)buffer, 1, longueur, f);
    fclose(f);

    // lecture de l'entete du fichier

    memcpy(&fichier->entetefichier.idformat, (buffer + position), 2);
    position += 2;
    memcpy(&fichier->entetefichier.taille_fichier, (buffer + position), 4);
    position += 4;
    memcpy(&fichier->entetefichier.reserve, (buffer + position), 4);
    position += 4;
    memcpy(&fichier->entetefichier.debut, (buffer + position), 4);
    position += 4;

    /*
    printf("id format                :%c%c\n",fichier->entetefichier.idformat[0],fichier->entetefichier.idformat[1]);
    printf("taille fichier           :%d\n",fichier->entetefichier.taille_fichier);
    printf("reserve                  :%d\n",fichier->entetefichier.reserve);
    printf("debut                    :%d\n",fichier->entetefichier.debut);
    */

    // lecture de l'entete de l'image

    memcpy(&fichier->entetebmp.taille_image, (buffer + position), 4);
    position += 4;
    memcpy(&fichier->entetebmp.largeur, (buffer + position), 4);
    position += 4;
    memcpy(&fichier->entetebmp.hauteur, (buffer + position), 4);
    position += 4;
    memcpy(&fichier->entetebmp.plans, (buffer + position), 2);
    position += 2;
    memcpy(&fichier->entetebmp.profondeur, (buffer + position), 2);
    position += 2;
    memcpy(&fichier->entetebmp.compression, (buffer + position), 4);
    position += 4;
    memcpy(&fichier->entetebmp.taille_image_totale, (buffer + position), 4);
    position += 4;
    memcpy(&fichier->entetebmp.resolutionh, (buffer + position), 4);
    position += 4;
    memcpy(&fichier->entetebmp.resolutionv, (buffer + position), 4);
    position += 4;
    memcpy(&fichier->entetebmp.nbrcouleur, (buffer + position), 4);
    position += 4;
    memcpy(&fichier->entetebmp.nbrcouleuri, (buffer + position), 4);
    position += 4;

    /*
    printf("taille image             :%d\n",fichier->entetebmp.taille_image);
    printf("largeur                  :%d\n",fichier->entetebmp.largeur);
    printf("hauteur                  :%d\n",fichier->entetebmp.hauteur);
    printf("plans                    :%d\n",fichier->entetebmp.plans);
    printf("profondeur               :%d\n",fichier->entetebmp.profondeur);
    printf("compression              :%d\n",fichier->entetebmp.compression);
    printf("taille image totale      :%d\n",fichier->entetebmp.taille_image_totale);
    printf("resolution h             :%d\n",fichier->entetebmp.resolutionh);
    printf("resulution v             :%d\n",fichier->entetebmp.resolutionv);
    printf("nbr couleurs             :%d\n",fichier->entetebmp.nbrcouleur);
    printf("nbr couleurs importantes :%d\n",fichier->entetebmp.nbrcouleuri);
    */

    if (fichier->entetebmp.plans != 1 || fichier->entetebmp.profondeur != 24 || fichier->entetebmp.compression != 0 || fichier->entetebmp.nbrcouleur != 0 || fichier->entetebmp.nbrcouleuri != 0)
    {
        printf("Impossible de charger l'image\n");
        free(fichier);
        free(buffer);
        return 0;
    }

    // mise de l'image en memoire

    for (i = fichier->entetebmp.hauteur - 1; i >= 0; i--)
    {
        for (j = 0; j < fichier->entetebmp.largeur; j++)
        {
            memcpy(&fichier->image[i][j].b, (buffer + position), 1);
            position += 1;
            memcpy(&fichier->image[i][j].g, (buffer + position), 1);
            position += 1;
            memcpy(&fichier->image[i][j].r, (buffer + position), 1);
            position += 1;
        }
        for (k = 0; k < fichier->entetebmp.largeur % 4; k++)
        {
            memcpy(&zero, (buffer + position), 1);
            position += 1;
        }
    }

    free(buffer);
    return fichier;
}

int enregistrer(char* nom, struct fichierimage* fichier)
{
    FILE* f;
    int i, j, k;
    unsigned char zero = (unsigned char)0;
    unsigned char* buffer;
    int position = 0;
    int longueur = 14 + fichier->entetefichier.taille_fichier;
    buffer = (unsigned char*)malloc(longueur * sizeof(unsigned char));

    // ecriture de l'entete du fichier

    /*
    printf("id format                :%c%c\n",fichier->entetefichier.idformat[0],fichier->entetefichier.idformat[1]);
    printf("taille fichier           :%d\n",fichier->entetefichier.taille_fichier);
    printf("reserve                  :%d\n",fichier->entetefichier.reserve);
    printf("debut                    :%d\n",fichier->entetefichier.debut);
    */

    memcpy((buffer + position), &fichier->entetefichier.idformat, 2);
    position += 2;
    memcpy((buffer + position), &fichier->entetefichier.taille_fichier, 4);
    position += 4;
    memcpy((buffer + position), &fichier->entetefichier.reserve, 4);
    position += 4;
    memcpy((buffer + position), &fichier->entetefichier.debut, 4);
    position += 4;

    // ecriture de l'entete de l'image

    /*
    printf("taille image             :%d\n",fichier->entetebmp.taille_image);
    printf("largeur                  :%d\n",fichier->entetebmp.largeur);
    printf("hauteur                  :%d\n",fichier->entetebmp.hauteur);
    printf("plans                    :%d\n",fichier->entetebmp.plans);
    printf("profondeur               :%d\n",fichier->entetebmp.profondeur);
    printf("compression              :%d\n",fichier->entetebmp.compression);
    printf("taille image totale      :%d\n",fichier->entetebmp.taille_image_totale);
    printf("resolution h             :%d\n",fichier->entetebmp.resolutionh);
    printf("resulution v             :%d\n",fichier->entetebmp.resolutionv);
    printf("nbr couleurs             :%d\n",fichier->entetebmp.nbrcouleur);
    printf("nbr couleurs importantes :%d\n",fichier->entetebmp.nbrcouleuri);
    */

    memcpy((buffer + position), &fichier->entetebmp.taille_image, 4);
    position += 4;
    memcpy((buffer + position), &fichier->entetebmp.largeur, 4);
    position += 4;
    memcpy((buffer + position), &fichier->entetebmp.hauteur, 4);
    position += 4;
    memcpy((buffer + position), &fichier->entetebmp.plans, 2);
    position += 2;
    memcpy((buffer + position), &fichier->entetebmp.profondeur, 2);
    position += 2;
    memcpy((buffer + position), &fichier->entetebmp.compression, 4);
    position += 4;
    memcpy((buffer + position), &fichier->entetebmp.taille_image_totale, 4);
    position += 4;
    memcpy((buffer + position), &fichier->entetebmp.resolutionh, 4);
    position += 4;
    memcpy((buffer + position), &fichier->entetebmp.resolutionv, 4);
    position += 4;
    memcpy((buffer + position), &fichier->entetebmp.nbrcouleur, 4);
    position += 4;
    memcpy((buffer + position), &fichier->entetebmp.nbrcouleuri, 4);
    position += 4;

    if (fichier->entetebmp.plans != 1 || fichier->entetebmp.profondeur != 24 || fichier->entetebmp.compression != 0 || fichier->entetebmp.nbrcouleur != 0 || fichier->entetebmp.nbrcouleuri != 0)
    {
        printf("Impossible d'enregistrer l'image\n");
        free(buffer);
        return 0;
    }

    for (i = fichier->entetebmp.hauteur - 1; i >= 0; i--)
    {
        for (j = 0; j < fichier->entetebmp.largeur; j++)
        {
            memcpy((buffer + position), &fichier->image[i][j].b, 1);
            position += 1;
            memcpy((buffer + position), &fichier->image[i][j].g, 1);
            position += 1;
            memcpy((buffer + position), &fichier->image[i][j].r, 1);
            position += 1;
        }
        for (k = 0; k < fichier->entetebmp.largeur % 4; k++)
        {
            memcpy((buffer + position), &zero, 1);
            position += 1;
        }
    }

    f = fopen(nom, "wb");
    fwrite((unsigned char*)buffer, 1, longueur, f);
    fclose(f);
    free(buffer);
    return 1;
}

struct fichierimage* nouveau(int largeur, int hauteur)
{
    int i, j;
    struct fichierimage* fichier;
    int longueur;

    if (largeur > 5000 || hauteur > 5000 || largeur <= 0 || hauteur <= 0)
    {
        return(NULL);
    }

    longueur = 14 + 40 + 3 * largeur * hauteur + (largeur % 4) * hauteur;
    fichier = (struct fichierimage*)malloc(sizeof(struct fichierimage));

    // ecriture de l'entete du fichier

    fichier->entetefichier.idformat[0] = 'B';
    fichier->entetefichier.idformat[1] = 'M';
    fichier->entetefichier.taille_fichier = 40 + 3 * largeur * hauteur + (largeur % 4) * hauteur;
    fichier->entetefichier.reserve = 0;
    fichier->entetefichier.debut = 54;

    // ecriture de l'entete de l'image

    fichier->entetebmp.taille_image = 40;
    fichier->entetebmp.largeur = largeur;
    fichier->entetebmp.hauteur = hauteur;
    fichier->entetebmp.plans = 1;
    fichier->entetebmp.profondeur = 24;
    fichier->entetebmp.compression = 0;
    fichier->entetebmp.taille_image_totale = 40 + 3 * largeur * hauteur + (largeur % 4) * hauteur;
    fichier->entetebmp.resolutionh = 1;
    fichier->entetebmp.resolutionv = 1;
    fichier->entetebmp.nbrcouleur = 0;
    fichier->entetebmp.nbrcouleuri = 0;

    // lecture de l'ente du fichier

    /*
    printf("id format                :%c%c\n",fichier->entetefichier.idformat[0],fichier->entetefichier.idformat[1]);
    printf("taille fichier           :%d\n",fichier->entetefichier.taille_fichier);
    printf("reserve                  :%d\n",fichier->entetefichier.reserve);
    printf("debut                    :%d\n",fichier->entetefichier.debut);
    */

    // lecture de l'ente de l'image

    /*
    printf("taille image             :%d\n",fichier->entetebmp.taille_image);
    printf("largeur                  :%d\n",fichier->entetebmp.largeur);
    printf("hauteur                  :%d\n",fichier->entetebmp.hauteur);
    printf("plans                    :%d\n",fichier->entetebmp.plans);
    printf("profondeur               :%d\n",fichier->entetebmp.profondeur);
    printf("compression              :%d\n",fichier->entetebmp.compression);
    printf("taille image totale      :%d\n",fichier->entetebmp.taille_image_totale);
    printf("resolution h             :%d\n",fichier->entetebmp.resolutionh);
    printf("resulution v             :%d\n",fichier->entetebmp.resolutionv);
    printf("nbr couleurs             :%d\n",fichier->entetebmp.nbrcouleur);
    printf("nbr couleurs importantes :%d\n",fichier->entetebmp.nbrcouleuri);
    */

    // mise de l'image en memoire

    for (i = fichier->entetebmp.hauteur - 1; i >= 0; i--)
    {
        for (j = 0; j < fichier->entetebmp.largeur; j++)
        {
            fichier->image[i][j].b = 0;
            fichier->image[i][j].g = 0;
            fichier->image[i][j].r = 0;
        }
    }

    return fichier;
}

//Fonction qui renvoie l'image miroir d'une image bmp
//Date de création : 05/11/21
//Date de dernière modification : 05/11/21
void miroir(struct fichierimage* fichier, char nomfic[100]) {
    int i, j = 0;
    struct fichierimage* fichiermiroir = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
    for (i = fichier->entetebmp.hauteur; i >= 0; i--) {
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
            fichiermiroir->image[i][j].r = fichier->image[i][fichier->entetebmp.largeur - j].r;
            fichiermiroir->image[i][j].g = fichier->image[i][fichier->entetebmp.largeur - j].g;
            fichiermiroir->image[i][j].b = fichier->image[i][fichier->entetebmp.largeur - j].b;
        }
    }
    enregistrer(nomfic, fichiermiroir);
    free(fichiermiroir);
}

//Fonction qui transforme une image en couleur en image en niveaux de gris
//Date de création : 29/10/21
//Date de dernière modification : 29/10/21 
void niveauxGris(struct fichierimage* fichier, char nomfic[100]) {
    int i, j = 0;
    for (i = fichier->entetebmp.hauteur; i >= 0; i--)
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
            fichier->image[i][j].r = (fichier->image[i][j].r + fichier->image[i][j].g + fichier->image[i][j].b) / 3;
            fichier->image[i][j].g = fichier->image[i][j].r;
            fichier->image[i][j].b = fichier->image[i][j].r;
        }
    enregistrer(nomfic, fichier);
}

//Fonction qui transforme une image RVB en image monochrone
//Date de création : 02/11/21
//Date de dernière modification : 02/11/21
void monochrome(struct fichierimage* fichier, char couleur, char nomfic[100]) {
    int i, j = 0;
    for (i = fichier->entetebmp.hauteur; i >= 0; i--)
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
            switch (couleur) {
            case 'R':
                fichier->image[i][j].g = 0;
                fichier->image[i][j].b = 0;
                break;
            case 'V':
                fichier->image[i][j].r = 0;
                fichier->image[i][j].b = 0;
                break;
            case 'B':
                fichier->image[i][j].g = 0;
                fichier->image[i][j].r = 0;
                break;
            }
        }
    enregistrer(nomfic, fichier);
}

//Fonction qui renvoie le négatif d'une image
//Date de création : 02/11/21
//Date de dernière modification : 02/11/21
void negatif(struct fichierimage* fichier, char nomfic[100]) {
    int i, j = 0;
    for (i = fichier->entetebmp.hauteur; i >= 0; i--)
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
            fichier->image[i][j].r = 255 - fichier->image[i][j].r;
            fichier->image[i][j].g = 255 - fichier->image[i][j].r;
            fichier->image[i][j].b = 255 - fichier->image[i][j].r;
        }
    enregistrer(nomfic, fichier);
}

//Fonction qui seuille une image
//Date de création : 10/11/21
//Date de dernière modification : 15/11/21
void seuillage(struct fichierimage* fichier, int seuil, char nomfic[100]) {
    int i, j = 0;
    for (i = fichier->entetebmp.hauteur; i >= 0; i--)
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
            if (fichier->image[i][j].r < seuil) {
                fichier->image[i][j].r = 0;
                fichier->image[i][j].g = 0;
                fichier->image[i][j].b = 0;
            }
            else {
                fichier->image[i][j].r = 255;
                fichier->image[i][j].g = 255;
                fichier->image[i][j].b = 255;
            }
        }
    enregistrer(nomfic, fichier);
}

//Fonction qui renvoie la symétrie verticale d'une image
//Date de création : 06/11/21
//Date de dernière modification : 08/11/21
void symetrieV(struct fichierimage* fichier, char nomfic[100]) {
    int i, j = 0;
    struct fichierimage* fichierSym = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
    for (i = fichier->entetebmp.hauteur / 2; i >= 0; i--) {
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
            fichierSym->image[i][j].r = fichier->image[i][fichier->entetebmp.largeur - j].r;
            fichierSym->image[i][j].g = fichier->image[i][fichier->entetebmp.largeur - j].g;
            fichierSym->image[i][j].b = fichier->image[i][fichier->entetebmp.largeur - j].b;
        }
    }
    enregistrer(nomfic, fichierSym);
    free(fichierSym);
}

//Fonction qui fait pivoter à gauche une image
//Date de création : 16/11/21
//Date de denière modification : 17/11/21
void pivotD(struct fichierimage* fichier, char nomfic[100]) {
    int i, j = 0;
    struct fichierimage* fichierPivot = nouveau(fichier->entetebmp.hauteur, fichier->entetebmp.largeur);
    for (i = fichier->entetebmp.hauteur; i >= 0; i--) {
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
            fichierPivot->image[j][fichier->entetebmp.hauteur - i].r = fichier->image[i][j].r;
            fichierPivot->image[j][fichier->entetebmp.hauteur - i].g = fichier->image[i][j].g;
            fichierPivot->image[j][fichier->entetebmp.hauteur - i].b = fichier->image[i][j].b;
        }
    }
    enregistrer(nomfic, fichierPivot);
    free(fichierPivot);
}

//Fonction qui fait pivoter à droite une image
//Date de création : 16/11/21
//Date de dernière modification : 17/11/21
void pivotG(struct fichierimage* fichier, char nomfic[100]) {
    int i, j = 0;
    struct fichierimage* fichierPivot = nouveau(fichier->entetebmp.hauteur, fichier->entetebmp.largeur);
    for (i = fichier->entetebmp.hauteur; i >= 0; i--) {
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
            fichierPivot->image[fichier->entetebmp.largeur - j - 1][i].r = fichier->image[i][j].r;
            fichierPivot->image[fichier->entetebmp.largeur - j - 1][i].g = fichier->image[i][j].g;
            fichierPivot->image[fichier->entetebmp.largeur - j - 1][i].b = fichier->image[i][j].b;
        }
    }
    enregistrer(nomfic, fichierPivot);
    free(fichierPivot);
}

//Fonction qui inverse les couleurs d'une image
//Date de création : 11/10/21
//Date de dernière modification : 11/10/21
void inversion(struct fichierimage* fichier, char nomfic[100]) {
    int i, j = 0;
    struct fichierimage* fichierInversion = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
    for (i = fichier->entetebmp.hauteur; i >= 0; i--) {
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
            fichierInversion->image[i][j].r = fichier->image[i][j].b;
            fichierInversion->image[i][j].g = fichier->image[i][j].r;
            fichierInversion->image[i][j].b = fichier->image[i][j].g;
        }
    }
    enregistrer(nomfic, fichierInversion);
    free(fichierInversion);
}

//Fonction qui réduit la taille d'une image
//Date de création : 18/11/21
//Date de dernière modification : 20/11/21
void reduction(struct fichierimage* fichier, int echelle, char nomfic[100]) {
    int i, j = 0;
    struct fichierimage* fichierRedu = NULL;
    fichierRedu = nouveau(fichier->entetebmp.largeur / echelle, fichier->entetebmp.hauteur / echelle);
    for (i = 0; i <= fichier->entetebmp.hauteur; i++)
        for (j = 0; j <= fichier->entetebmp.largeur; j++)
        {
            fichierRedu->image[i / echelle][j / echelle].r = fichier->image[i][j].r;
            fichierRedu->image[i / echelle][j / echelle].g = fichier->image[i][j].g;
            fichierRedu->image[i / echelle][j / echelle].b = fichier->image[i][j].b;
        }
    enregistrer(nomfic, fichierRedu);
    free(fichierRedu);
}

//Fonction qui agrandit la taille d'une image
//Date de création : 18/11/21
//Date de dernière modification : 20/11/21
void agrandissement(struct fichierimage* fichier, int echelle, char nomfic[100]) {
    int i, j, k, l = 0;
    struct fichierimage* fichierAgran = NULL;
    fichierAgran = nouveau(echelle * fichier->entetebmp.largeur, echelle * fichier->entetebmp.hauteur);
    for (i = 1; i < fichier->entetebmp.hauteur; i++)
        for (j = 1; j < fichier->entetebmp.largeur; j++)
        {
            for (k = ((echelle * i) - 1); k <= (echelle * i); k++)
                for (l = ((echelle * j) - 1); l <= (echelle * j); l++)
                {
                    fichierAgran->image[k][l].r = fichier->image[i][j].r;
                    fichierAgran->image[k][l].g = fichier->image[i][j].g;
                    fichierAgran->image[k][l].b = fichier->image[i][j].b;
                }
        }
    enregistrer(nomfic, fichierAgran);
    free(fichierAgran);
}

//Fonction qui efface la mémoire tampon
//Date de création : 17/11/21
//Date de dernière modification : 17/11/21
void clearBuffer() {
    char c = getchar();
    while ('\n' != c) {
        c = getchar();
    }
}

//Fonction qui ajoute un bord pour traiter l'image afin de réaliser une convolution
//Utilisation de la méthode 3 : ajout de 0 autour de l'image
//Date de création : 24/11/21
//Date de dernière modification : 24/11/21
struct fichierimage* bord(struct fichierimage *fichier) {
    int i, j = 0;
    struct fichierimage* fichierBord=NULL;
    int epaisseurH = fichier->entetebmp.hauteur / 2;
    int epaisseurL = fichier->entetebmp.largeur / 2;
    fichierBord = nouveau(fichier->entetebmp.largeur*2,fichier->entetebmp.hauteur*2);
    for (i = 0; i < fichier->entetebmp.hauteur; i++) {
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
                    fichierBord->image[i+epaisseurH][j+epaisseurL].r = fichier->image[i][j].r;
                    fichierBord->image[i+epaisseurH][j+epaisseurL].g = fichier->image[i][j].g;
                    fichierBord->image[i+epaisseurH][j + epaisseurL].b = fichier->image[i][j].b;
        }
    }  
    return fichierBord;
}

//Fonction qui fait la convolution d'une image
//Date de création : 24/11/21
//Date de dernière modification : 29/11/21
struct fichierimage* convolution(struct fichierimage *fichier, matrice m, char nomfic[100]) {
    int i, j;
    struct fichierimage* fichier_res = NULL;
    fichier_res = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
    for (i = fichier->entetebmp.hauteur/2; i < fichier->entetebmp.hauteur-(fichier->entetebmp.hauteur/2); i++) {
        for (j = fichier->entetebmp.largeur/2; j < fichier->entetebmp.largeur-(fichier->entetebmp.largeur/2); j++) {
            fichier_res->image[i][j].r = fichier->image[i - 1][j - 1].r * m[0][0] + fichier->image[i][j - 1].r * m[1][0] +
                fichier->image[i + 1][j - 1].r * m[2][0] +
                fichier->image[i - 1][j].r * m[0][1] +
                fichier->image[i][j].r * m[1][1] +
                fichier->image[i + 1][j].r * m[2][1] +
                fichier->image[i - 1][j + 1].r * m[0][2] +
                fichier->image[i][j + 1].r * m[1][2] +
                fichier->image[i + 1][j + 1].r * m[2][2];
            fichier_res->image[i][j].b = fichier_res->image[i][j].r;
            fichier_res->image[i][j].g = fichier_res->image[i][j].r;
      }
    }
    enregistrer(nomfic, fichier_res);
    return fichier_res;
    free(fichier_res);
}

//Fonction qui applique le filtre moyen sur une image
//Date de création : 01/12/21
//Date de dernière modification : 01/12/21
void filtre_moyen(struct fichierimage *fichier, char nomfic[100])
{
    matrice masque = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
    convolution(fichier, masque, nomfic);
}

//Fonction qui applique le filtre de Sobel sur une image donnée
//Date de création : 01/12/2021
//Date de dernière modification : 01/12/2021
void sobel(struct fichierimage *fichier, char nomfic[100]) {
    int i, j;
    matrice matriceX = { {-1,0,1},{-2,0,2},{-1,0,1} };
    matrice matriceY = { {1,2,1},{0,0,0},{-1,-2,-1} };
    struct fichierimage *fichierBord = bord(fichier);
    struct fichierimage* fichierresX = nouveau(fichierBord->entetebmp.largeur, fichierBord->entetebmp.hauteur);
    fichierresX = convolution(fichier, matriceX,nomfic);
    struct fichierimage *fichierresY = nouveau(fichierBord->entetebmp.largeur, fichierBord->entetebmp.hauteur);
    fichierresY = convolution(fichier, matriceY,nomfic);
    struct fichierimage *fichierSobel = nouveau(fichierBord->entetebmp.largeur, fichierBord->entetebmp.hauteur);;
    
    for (i = fichier->entetebmp.hauteur / 2; i < fichier->entetebmp.hauteur-(fichier->entetebmp.hauteur / 2); i++) {
        for (j = fichier->entetebmp.largeur / 2; j < fichier->entetebmp.largeur-(fichier->entetebmp.largeur / 2); j++) {
            fichierSobel->image[i][j].r = sqrt( pow(fichierresX->image[i][j].r,2) + pow(fichierresY->image[i][j].r,2));
            fichierSobel->image[i][j].g = fichierSobel->image[i][j].r;
            fichierSobel->image[i][j].b = fichierSobel->image[i][j].r;
        }
    }
    enregistrer(nomfic, fichierSobel);
    free(fichierSobel);
    free(fichierresX);
    free(fichierresY);
}

//Fonction qui calcule le pourcentage de couleur d'une image
//Date de création : 11/10/21
//Date de dernière modification : 11/10/21
float pourcentageCouleur(char couleur, struct fichierimage* fichier)
{
    int nb = 0;
    for (int i = 0; i < fichier->entetebmp.hauteur; i++)
    {
        for (int j = 0; j < fichier->entetebmp.largeur; j++)
        {
            if (couleur == 'R')
                nb += fichier->image[i][j].r;
            else if (couleur == 'G')
                nb += fichier->image[i][j].g;
            else if (couleur == 'B')
                nb += fichier->image[i][j].b;
        }
    }
    return ((float)nb / ((fichier->entetebmp.hauteur * fichier->entetebmp.largeur) * 255)) * 100;
}

//Fonction qui calcule l'histogramme d'une image 
//Date de création : 20/11/21
//Date de dernière modification : 24/11/21
void histogramme(int *histoR, int *histoG, int *histoB, struct fichierimage *fichier) {
    int i, j;
    for (i = 0; i < fichier->entetebmp.hauteur; i++) {
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
            histoR[fichier->image[i][j].r+1] = histoR[fichier->image[i][j].r + 1] + 1;
            histoB[fichier->image[i][j].b + 1] = histoB[fichier->image[i][j].b + 1] + 1;
            histoG[fichier->image[i][j].g + 1] = histoG[fichier->image[i][j].g + 1] + 1;
        }
    }
}

//Fonction qui calcule l'histogramme cumulé RGB
//Date de création : 24/11/21
//Date de dernière modification : 28/11/21
void histogrammeCumule(int *histoR, int *histoG, int * histoB, int * histoCumuleR, int* histoCumuleG, int* histoCumuleB)
{
    histoCumuleR[0] = histoR[0];
    histoCumuleG[0] = histoG[0];
    histoCumuleB[0] = histoB[0];

    for (int i = 1; i < 256; i++)
    {
        histoCumuleR[i] = histoCumuleR[i - 1] + histoR[i];
        histoCumuleG[i] = histoCumuleG[i - 1] + histoG[i];
        histoCumuleB[i] = histoCumuleB[i - 1] + histoB[i];
    }
}

//Fonction qui égalise l'histogramme d'une image
//Date de création : 24/11/21
//Date de dernière modification : 27/11/21
void egalisationHisto(struct fichierimage* fichier, char nomfic[100]) {
    int i, j;
    int histoR[256];
    int histoG[256];
    int histoB[256];
    int histoCumuleR[256];
    int histoCumuleG[256];
    int histoCumuleB[256];
    struct fichierimage* fichierEgal = NULL;
    fichierEgal = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
    for (int i = 0; i < 256; i++)
    {
        histoR[i] = 0;
        histoG[i] = 0;
        histoB[i] = 0;
        histoCumuleR[i] = 0;
        histoCumuleG[i] = 0;
        histoCumuleB[i] = 0;
    }
    histogramme(&histoR, &histoG, &histoB, fichier);
    histogrammeCumule(&histoR, &histoG, &histoB, &histoCumuleR, &histoCumuleG, &histoCumuleB);
    int nbr_pixels = fichier->entetebmp.hauteur * fichier->entetebmp.largeur;
    for (i = 0; i < fichier->entetebmp.hauteur; i++){
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
            fichierEgal->image[i][j].r = histoCumuleR[fichier->image[i][j].r] * 255 / nbr_pixels;
            fichierEgal->image[i][j].g = histoCumuleG[fichier->image[i][j].g] * 255 / nbr_pixels;
            fichierEgal->image[i][j].b = histoCumuleB[fichier->image[i][j].b] * 255 / nbr_pixels;
        }
    }
    enregistrer(nomfic, fichierEgal);
    free(fichierEgal);
}

//Fonction qui modifie la luminosité de l'image
//Date de création : 27/11/21
//Date de dernière modification : 01/12/21
void luminosite(struct fichierimage *fichier, char nomfic[100])
{
    int i, j;
    struct fichierimage* fichierLum = nouveau(fichier->entetebmp.largeur,fichier->entetebmp.hauteur);
    for (i = 0; i < fichier->entetebmp.hauteur; i++) {
        for (j = 0; j < fichier->entetebmp.largeur; j++) {
            fichierLum->image[i][j].r = sqrt(fichier->image[i][j].r)*255;
            fichierLum->image[i][j].g = sqrt(fichier->image[i][j].g)*255;
            fichierLum->image[i][j].b = sqrt(fichier->image[i][j].b)*255;
        }
    }
     enregistrer(nomfic, fichierLum);
     free(fichierLum);
}

//Fonction qui récupère les information d'une image
//Date de création : 19/11/21
//Date de dernière modification : 19/11/21
struct entete_fichier infos(struct fichierimage *fichier)
{
    return fichier->entetefichier;
}

//Fonction qui applique le filtre laplacien sur une image donnée
//Date de création : 01/12/21
//Date de dernière mdification : 02/12/21
void filtre_laplacien(struct fichierimage* fichier, char nomfic[100])
{
    matrice masque = { {0, 1, 0}, {1, -4, 1}, {0, 1, 0} };
    convolution(fichier, masque, nomfic);
}

//Fonction qui contratse une image RGB
//Date de création : 02/12/21
//Date de dernière modification : 02/12/21
void contraste(struct fichierimage* fichier, char nomfic[100]) {
    int i, j;
    struct fichierimage* fichierContr = NULL;
    fichierContr = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
    for (i = 0; i < fichier->entetebmp.largeur; i++) {
        for (j = 0; j < fichier->entetebmp.hauteur; j++) {
            fichierContr->image[i][j].r = (pow(fichier->image[i][j].r, 2)) / 255;
            fichierContr->image[i][j].g = (pow(fichier->image[i][j].g, 2)) / 255;
            fichierContr->image[i][j].b = (pow(fichier->image[i][j].b, 2)) / 255;
        }
    }
    enregistrer(nomfic, fichierContr);
    free(fichierContr);
}

void menu(struct fichierimage *fichier, char nomfic[100]) {
    char choix,couleur;
    int seuil,echelle, i,j, pourcentage;
    matrice m;
    struct entete_fichier info;
    int histo[256];
    printf("Veuillez entrez les lettres du traitement que vous souhaitez r%caliser : \n",130);
    printf("\tImage en niveaux de gris : (n)\n");
    printf("\tImage miroir : (m)\n");
    printf("\tImage sym%ctrique : (s)\n",130);
    printf("\tImage pivot gauche : (g) \n");
    printf("\tImage pivot droit : (d)\n");
    printf("\tCalcul du pourcentage de couleur : (P) \n");
    printf("\tImage monochrone : (M) \n");
    printf("\tInversion des couleurs (i) : \n");
    printf("\tRéduction de l'image : (r) \n");
    printf("\tAgrandissement de l'image : (a) \n");
    printf("\tImage n%cgative : (N) \n",130);
    printf("\tSeuillage de l'image : (S) \n");
    printf("\tConvolution de l'image : (c) \n");
    printf("\tEgalisation de l'histogramme : (e) \n");
    printf("\tInformation sur l'image : (I) \n");
    printf("\tModification de la luminosit%c de l'image : (l)\n",130);
    printf("\tFiltre moyen : (f) \n");
    printf("\tFiltre Laplacien : (L)\n");
    printf("\tContraster l'image : (C) \n");
    printf("\tFiltre sobel : (O)\n");
    printf("\tQuitter (q)\n");
    printf("\nVotre choix: ");
    scanf("%c", &choix);
    clearBuffer();
    switch (choix)
    {
    case 'm':
        miroir(fichier,nomfic);
        break;
    case'n':
        niveauxGris(fichier, nomfic);
        break;
    case'M':
        printf("Quelle couleur ? entrez R, V ou B :");
        scanf("%c", &couleur);
        clearBuffer();
        monochrome(fichier, couleur, nomfic);
        break;
    case 'N':
        negatif(fichier, nomfic);
        break;
    case 'S':
        printf("Entrez un seuil : \n");
        scanf("%d", &seuil);
        clearBuffer();
        seuillage(fichier, seuil, nomfic);
        break;
    case 's':
        symetrieV(fichier, nomfic);
        break;
    case 'd':
        pivotD(fichier, nomfic);
        break;
    case 'g':
        pivotG(fichier, nomfic);
        break;
    case 'i':
        inversion(fichier, nomfic);
        break;
    case'r':
        printf("Veuillez entrer l'%cchelle : ", 130);
        scanf("%d", &echelle);
        clearBuffer();
        reduction(fichier, echelle, nomfic);
        break;
    case 'a':
        printf("Veuillez entrer l'%cchelle : ", 130);
        scanf("%d", &echelle);
        clearBuffer();
        agrandissement(fichier, echelle, nomfic);
        break;
    case 'c':
        printf("Entrer les elements de la matrice: \n");
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
            {
                printf("\tElem [%d,%d]= ", i + 1, j + 1);
                scanf("%d", &m[i][j]);
            }
        convolution(fichier, m, nomfic);
        break;
    case 'f':
        filtre_moyen(fichier, nomfic);
        break;
    case 'O':
        sobel(fichier, nomfic);
        break;
    case 'P':
        printf("Quelle couleur ? entrez R, V ou B :");
        scanf("%c", &couleur);
        clearBuffer();
        pourcentage = pourcentageCouleur(couleur, fichier);
        printf("Le pourcentage est de : %d \n", pourcentage);
        break;
    case 'e':
        egalisationHisto(fichier, nomfic);
        break;
    case 'l':
        luminosite(fichier, nomfic);
        break;
    case 'I':
        info = infos(fichier);
        printf("%s", info);
        break;
    case 'L':
        filtre_laplacien(fichier, nomfic);
        break;
    case 'C':
        contraste(fichier, nomfic);
        break;
    default:
        break;
    }
}

