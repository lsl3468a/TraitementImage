#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct fichierimage *charger(char *nom)
{
FILE *f;
int i,j,k;
unsigned char zero;
unsigned char *buffer;
int position=0;
int longueur=0;
struct fichierimage *fichier=(struct fichierimage *)malloc(sizeof(struct fichierimage));

f=fopen(nom,"rb");
fseek(f,0,SEEK_END);
longueur=ftell(f);
fseek(f,0,SEEK_SET);
buffer=(unsigned char *)malloc(longueur*sizeof(unsigned char));
fread((unsigned char *)buffer,1,longueur,f);
fclose(f);

// lecture de l'entete du fichier

memcpy(&fichier->entetefichier.idformat,(buffer+position),2);
position+=2;
memcpy(&fichier->entetefichier.taille_fichier,(buffer+position),4);
position+=4;
memcpy(&fichier->entetefichier.reserve,(buffer+position),4);
position+=4;
memcpy(&fichier->entetefichier.debut,(buffer+position),4);
position+=4;

/*
printf("id format                :%c%c\n",fichier->entetefichier.idformat[0],fichier->entetefichier.idformat[1]);
printf("taille fichier           :%d\n",fichier->entetefichier.taille_fichier);
printf("reserve                  :%d\n",fichier->entetefichier.reserve);
printf("debut                    :%d\n",fichier->entetefichier.debut);
*/

// lecture de l'entete de l'image

memcpy(&fichier->entetebmp.taille_image,(buffer+position),4);
position+=4;
memcpy(&fichier->entetebmp.largeur,(buffer+position),4);
position+=4;
memcpy(&fichier->entetebmp.hauteur,(buffer+position),4);
position+=4;
memcpy(&fichier->entetebmp.plans,(buffer+position),2);
position+=2;
memcpy(&fichier->entetebmp.profondeur,(buffer+position),2);
position+=2;
memcpy(&fichier->entetebmp.compression,(buffer+position),4);
position+=4;
memcpy(&fichier->entetebmp.taille_image_totale,(buffer+position),4);
position+=4;
memcpy(&fichier->entetebmp.resolutionh,(buffer+position),4);
position+=4;
memcpy(&fichier->entetebmp.resolutionv,(buffer+position),4);
position+=4;
memcpy(&fichier->entetebmp.nbrcouleur,(buffer+position),4);
position+=4;
memcpy(&fichier->entetebmp.nbrcouleuri,(buffer+position),4);
position+=4;

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

if(fichier->entetebmp.plans!=1||fichier->entetebmp.profondeur!=24||fichier->entetebmp.compression!=0||fichier->entetebmp.nbrcouleur!=0||fichier->entetebmp.nbrcouleuri!=0)
{
printf("Impossible de charger l'image\n");
free(fichier);
free(buffer);
return 0;
}

// mise de l'image en memoire

for(i=fichier->entetebmp.hauteur-1;i>=0;i--)
{
for(j=0;j<fichier->entetebmp.largeur;j++)
                                {
                                memcpy(&fichier->image[i][j].b,(buffer+position),1);
                                position+=1;
                                memcpy(&fichier->image[i][j].g,(buffer+position),1);
                                position+=1;
                                memcpy(&fichier->image[i][j].r,(buffer+position),1);
                                position+=1;
                                }
for(k=0;k<fichier->entetebmp.largeur%4;k++)
                                           {
                                           memcpy(&zero,(buffer+position),1);
                                           position+=1;
                                           }
}

free(buffer);
return fichier;
}

int enregistrer(char *nom,struct fichierimage *fichier)
{
FILE *f;
int i,j,k;
unsigned char zero=(unsigned char)0;
unsigned char *buffer;
int position=0;
int longueur=14+fichier->entetefichier.taille_fichier;
buffer=(unsigned char *)malloc(longueur*sizeof(unsigned char));

// ecriture de l'entete du fichier

/*
printf("id format                :%c%c\n",fichier->entetefichier.idformat[0],fichier->entetefichier.idformat[1]);
printf("taille fichier           :%d\n",fichier->entetefichier.taille_fichier);
printf("reserve                  :%d\n",fichier->entetefichier.reserve);
printf("debut                    :%d\n",fichier->entetefichier.debut);
*/

memcpy((buffer+position),&fichier->entetefichier.idformat,2);
position+=2;
memcpy((buffer+position),&fichier->entetefichier.taille_fichier,4);
position+=4;
memcpy((buffer+position),&fichier->entetefichier.reserve,4);
position+=4;
memcpy((buffer+position),&fichier->entetefichier.debut,4);
position+=4;

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

memcpy((buffer+position),&fichier->entetebmp.taille_image,4);
position+=4;
memcpy((buffer+position),&fichier->entetebmp.largeur,4);
position+=4;
memcpy((buffer+position),&fichier->entetebmp.hauteur,4);
position+=4;
memcpy((buffer+position),&fichier->entetebmp.plans,2);
position+=2;
memcpy((buffer+position),&fichier->entetebmp.profondeur,2);
position+=2;
memcpy((buffer+position),&fichier->entetebmp.compression,4);
position+=4;
memcpy((buffer+position),&fichier->entetebmp.taille_image_totale,4);
position+=4;
memcpy((buffer+position),&fichier->entetebmp.resolutionh,4);
position+=4;
memcpy((buffer+position),&fichier->entetebmp.resolutionv,4);
position+=4;
memcpy((buffer+position),&fichier->entetebmp.nbrcouleur,4);
position+=4;
memcpy((buffer+position),&fichier->entetebmp.nbrcouleuri,4);
position+=4;

if(fichier->entetebmp.plans!=1||fichier->entetebmp.profondeur!=24||fichier->entetebmp.compression!=0||fichier->entetebmp.nbrcouleur!=0||fichier->entetebmp.nbrcouleuri!=0)
{
printf("Impossible d'enregistrer l'image\n");
free(buffer);
return 0;
}

for(i=fichier->entetebmp.hauteur-1;i>=0;i--)
{
for(j=0;j<fichier->entetebmp.largeur;j++)
                                {
                                memcpy((buffer+position),&fichier->image[i][j].b,1);
                                position+=1;
                                memcpy((buffer+position),&fichier->image[i][j].g,1);
                                position+=1;
                                memcpy((buffer+position),&fichier->image[i][j].r,1);
                                position+=1;
                                }
for(k=0;k<fichier->entetebmp.largeur%4;k++)
                                           {
                                           memcpy((buffer+position),&zero,1);
                                           position+=1;
                                           }
}

f=fopen(nom,"wb");
fwrite((unsigned char *)buffer,1,longueur,f);
fclose(f);
free(buffer);
return 1;
}

struct fichierimage *nouveau(int largeur,int hauteur)
{
int i,j;
struct fichierimage *fichier;
int longueur;

if(largeur>5000||hauteur>5000||largeur<=0||hauteur<=0)
	{
	return(NULL);
	}

longueur=14+40+3*largeur*hauteur+(largeur%4)*hauteur;
fichier=(struct fichierimage *)malloc(sizeof(struct fichierimage));

// ecriture de l'entete du fichier

fichier->entetefichier.idformat[0]='B';
fichier->entetefichier.idformat[1]='M';
fichier->entetefichier.taille_fichier=40+3*largeur*hauteur+(largeur%4)*hauteur;
fichier->entetefichier.reserve=0;
fichier->entetefichier.debut=54;

// ecriture de l'entete de l'image

fichier->entetebmp.taille_image=40;
fichier->entetebmp.largeur=largeur;
fichier->entetebmp.hauteur=hauteur;
fichier->entetebmp.plans=1;
fichier->entetebmp.profondeur=24;
fichier->entetebmp.compression=0;
fichier->entetebmp.taille_image_totale=40+3*largeur*hauteur+(largeur%4)*hauteur;
fichier->entetebmp.resolutionh=1;
fichier->entetebmp.resolutionv=1;
fichier->entetebmp.nbrcouleur=0;
fichier->entetebmp.nbrcouleuri=0;

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

for(i=fichier->entetebmp.hauteur-1;i>=0;i--)
{
for(j=0;j<fichier->entetebmp.largeur;j++)
                                {
                                fichier->image[i][j].b=0;
                                fichier->image[i][j].g=0;
                                fichier->image[i][j].r=0;
                                }
}

return fichier;
}

void miroir(struct fichierimage *fichier) {
    struct fichierimage *fichiermiroir = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
    for (int i = fichier->entetebmp.hauteur; i >= 0; i--) {
        for (int j = 0; j < fichier->entetebmp.largeur; j++) {
            fichiermiroir->image[i][j].r = fichier->image[i][fichier->entetebmp.largeur - j].r;
            fichiermiroir->image[i][j].g = fichier->image[i][fichier->entetebmp.largeur - j].g;
            fichiermiroir->image[i][j].b = fichier->image[i][fichier->entetebmp.largeur - j].b;
        }
    }
    enregistrer("LASSERRE_resultatMIRROIR.bmp", fichiermiroir);
    free(fichiermiroir);
}

void niveauxGris(struct fichierimage *fichier) {
    for (int i = fichier->entetebmp.hauteur; i >= 0; i--)
        for (int j = 0; j < fichier->entetebmp.largeur; j++) {
            fichier->image[i][j].r = (fichier->image[i][j].r + fichier->image[i][j].g + fichier->image[i][j].b) / 3;
            fichier->image[i][j].g = fichier->image[i][j].r;
            fichier->image[i][j].b = fichier->image[i][j].r;
        }
    enregistrer("LASSERRE_resultatNG.bmp", fichier);
}

void monochrome(struct fichierimage *fichier, char couleur) {
    for (int i = fichier->entetebmp.hauteur; i >= 0; i--)
        for (int j = 0; j < fichier->entetebmp.largeur; j++) {
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
        enregistrer("LASSERRE_resultatMONOCHRONE.bmp", fichier);
}

void negatif(struct fichierimage *fichier) {
    for (int i = fichier->entetebmp.hauteur; i >= 0; i--)
        for (int j = 0; j < fichier->entetebmp.largeur; j++) {
            fichier->image[i][j].r = 255 - fichier->image[i][j].r;
            fichier->image[i][j].g = 255 - fichier->image[i][j].r;
            fichier->image[i][j].b = 255 - fichier->image[i][j].r;
        }
    enregistrer("LASSERRE_resultatNEGATIF.bmp", fichier);
}

void seuillage(struct fichierimage *fichier, int seuil) {
    for (int i = fichier->entetebmp.hauteur; i >= 0; i--)
        for (int j = 0; j < fichier->entetebmp.largeur; j++) {
            if(fichier->image[i][j].r < seuil) {
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
    enregistrer("LASSERRE_resultatSEUIL.bmp", fichier);
}

void symetrieV(struct fichierimage *fichier) {
    struct fichierimage *fichierSym = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
    for (int i = fichier->entetebmp.hauteur/2; i >= 0; i--) {
        for (int j = 0; j < fichier->entetebmp.largeur; j++) {
            fichierSym->image[i][j].r = fichier->image[i][fichier->entetebmp.largeur - j].r;
            fichierSym->image[i][j].g = fichier->image[i][fichier->entetebmp.largeur - j].g;
            fichierSym->image[i][j].b = fichier->image[i][fichier->entetebmp.largeur - j].b;
        }
    }
    enregistrer("LASSERRE_resultatSymV.bmp", fichierSym);
    free(fichierSym);
}

void pivotD(struct fichierimage *fichier) {
    struct fichierimage *fichierPivot = nouveau(fichier->entetebmp.hauteur, fichier->entetebmp.largeur);
    for (int i = fichier->entetebmp.hauteur; i >= 0; i--) {
        for (int j = 0; j < fichier->entetebmp.largeur; j++) {
            fichierPivot->image[j][fichier->entetebmp.hauteur - i].r = fichier->image[i][j].r;
            fichierPivot->image[j][fichier->entetebmp.hauteur - i].g = fichier->image[i][j].g;
            fichierPivot->image[j][fichier->entetebmp.hauteur - i].b = fichier->image[i][j].b;
        }
    }
    enregistrer("LASSERRE_resultatPIVOTD.bmp", fichierPivot);
    free(fichierPivot);
}

void pivotG(struct fichierimage *fichier) {
    struct fichierimage *fichierPivot = nouveau(fichier->entetebmp.hauteur, fichier->entetebmp.largeur);
    for (int i = fichier->entetebmp.hauteur; i >= 0; i--) {
        for (int j = 0; j < fichier->entetebmp.largeur; j++) {
            fichierPivot->image[fichier->entetebmp.largeur-j-1][i].r = fichier->image[i][j].r;
            fichierPivot->image[fichier->entetebmp.largeur-j-1][i].g = fichier->image[i][j].g;
            fichierPivot->image[fichier->entetebmp.largeur-j-1][i].b = fichier->image[i][j].b;
        }
    }
    enregistrer("LASSERRE_resultatPIVOTG.bmp", fichierPivot);
    free(fichierPivot);
}

void inversion(struct fichierimage *fichier) {
    struct fichierimage* fichierInversion = nouveau(fichier->entetebmp.largeur,fichier->entetebmp.hauteur);
    for (int i = fichier->entetebmp.hauteur; i >= 0; i--) {
        for (int j = 0; j < fichier->entetebmp.largeur; j++) {
            fichierInversion->image[i][j].r = fichier->image[i][j].b;
            fichierInversion->image[i][j].g = fichier->image[i][j].r;
            fichierInversion->image[i][j].b = fichier->image[i][j].g;
        }
    }
    enregistrer("LASSERRE_resultatInversion.bmp", fichierInversion);
    free(fichierInversion);
}

void reduction(struct fichierimage *fichier, int echelle) {
    struct fichierimage* fichierRedu = NULL;
    fichierRedu = nouveau(fichier->entetebmp.largeur /echelle, fichier->entetebmp.hauteur / echelle);
    for (int i = 0; i <= fichier->entetebmp.hauteur; i++)
        for (int j = 0; j <= fichier->entetebmp.largeur; j++)
        {
            fichierRedu->image[i / echelle][j / echelle].r = fichier->image[i][j].r;
            fichierRedu->image[i / echelle][j / echelle].g = fichier->image[i][j].g;
            fichierRedu->image[i / echelle][j / echelle].b = fichier->image[i][j].b;
        }
    enregistrer("LASSERRE_Reduction.bmp", fichierRedu);
    free(fichierRedu);
}

void agrandissement(struct fichierimage *fichier, int echelle) {
        struct fichierimage* fichierAgran = NULL;
        fichierAgran = nouveau(echelle * fichier->entetebmp.largeur, echelle * fichier->entetebmp.hauteur);
        for (int i = 1; i < fichier->entetebmp.hauteur; i++)
            for (int j = 1; j < fichier->entetebmp.largeur; j++)
            {
                for (int k = ((echelle * i) - 1); k <= (echelle * i); k++)
                    for (int l = ((echelle * j) - 1); l <= (echelle * j); l++)
                    {
                        fichierAgran->image[k][l].r = fichier->image[i][j].r;
                        fichierAgran->image[k][l].g = fichier->image[i][j].g;
                        fichierAgran->image[k][l].b = fichier->image[i][j].b;
                    }
            }
        enregistrer("LASSERRE_agrandissement.bmp", fichierAgran);
        free(fichierAgran);
}
