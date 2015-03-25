#ifndef REGION_H
#define REGION_H

#include <stdlib.h>
#include <string.h>

#include "Departement.h"

typedef struct Region Region;

struct Region {

char* nom_reg;
Departement* tab_departement;
int* taille_tab_departement;
};

void creerTabRegion(Region** tab_region, int** taille_tab_region, int taille_nom_rg);
void ajouterRegion(Region** tab_region, char nom_reg[], int** taille_tab_region);
void modifierNomRegion(Region* region, char nom_reg[]);
void supprimerRegion(Region** tab_region, Region* region_supp, int** taille_tab_region);
void detruireTabRegion(Region** tab_region, int** taille_tab_region);

#endif