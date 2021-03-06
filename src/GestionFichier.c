﻿/*
Override
GestionFichier.c

2014/2015

@author Alexandre Courcoul, Merlin Loison & Massamba Sène

**/

#include "GestionFichier.h"
int nombrePointVirguleDansLigne(FILE* fichier)
{
	wchar_t lettre = NULL;
	int nombre = 0;
	while (lettre != '\n')//premiere ligne, cas general
	{
		fwscanf(fichier, L"%lc", &lettre);
		if (lettre == ';')
		{
			nombre++;
		}
	}
	return nombre;
}


void lectureFichiers(Region** tab_region, int** taille_tab_region)// => a remplacer par Dépendances
{
	FILE* fichier = NULL;
	wchar_t ligne_en_cours[1000] = { 0 };
	wchar_t numerodep_tmp[10] = { 0 };
	wchar_t departement_tmp[200] = { 0 };
	wchar_t prefecture_tmp[200] = { 0 };
	wchar_t nom_region_tmp[200] = { 0 };;
	wchar_t* token = NULL;
	int is_departement_exist = 0;
	int is_region_exist = 0;
	int i = 0;
	int* taille_tab_departement = NULL;
	int* ref_taille_tab_departement = NULL;
	Departement* tab_departement = NULL;
	int* ref_tab_departement = NULL;


	// ****************--------------------- premier fichier, fichier des departements ---------------******************************
#if _DEBUG
	fichier = fopen("../../../../GIT/Recensement/Import/departements.csv", "r");
#endif
#if !_DEBUG
	fichier = fopen("../Import/departements.csv", "r");
#endif
	if (fichier == NULL)
	{
		return NULL;
	}
	else
	{
		*taille_tab_region = malloc(sizeof(int));
		**taille_tab_region = 0;

		while ((fgetws(ligne_en_cours, sizeof(ligne_en_cours), fichier)) != NULL)// parcours le fichier par ligne
		{
			is_region_exist = 0;
			token = wcstok(ligne_en_cours, ";");
			wcscpy(numerodep_tmp, token);
			token = wcstok(NULL, ";");
			wcscpy(departement_tmp, token);
			token = wcstok(NULL, ";");
			wcscpy(prefecture_tmp, token);
			token = wcstok(NULL, "\n");
			wcscpy(nom_region_tmp, token);

			wprintf(L"%ls \n", departement_tmp);
			for (int compteur_region = 0; compteur_region < **taille_tab_region; compteur_region++)
			{
				if (!(wcscmp((((*tab_region) + compteur_region)->nom_reg), nom_region_tmp)))//on verifie si la region est deja cree
				{
					is_region_exist = 1;
					((*tab_region) + compteur_region)->tab_departement = ajouterDepartement(&(((*tab_region) + compteur_region)->tab_departement), departement_tmp, numerodep_tmp, prefecture_tmp, &(((*tab_region) + compteur_region)->taille_tab_departement));
					wprintf(L"Departement ajouté à région existante. \n");
					printf("%d \n", **taille_tab_region);
					break;
				}
			}

			if (!is_region_exist)//sinon on l'ajoute
			{
				*tab_region = ajouterRegion(tab_region, nom_region_tmp, taille_tab_region);

				wprintf(L"R%lcgion %ls cr%lc%lce \n", 130, nom_region_tmp, 130, 130);
				// test non concluant pour affichage accents fwprintf(stdout, L"%ls\n", nom_region_tmp);
				printf("%d \n", **taille_tab_region);
				((*tab_region) + **taille_tab_region - 1)->tab_departement = ajouterDepartement(&(((*tab_region) + (**taille_tab_region) - 1)->tab_departement), departement_tmp, numerodep_tmp, prefecture_tmp, &(((*tab_region) + **taille_tab_region - 1)->taille_tab_departement));//ne marche pas encore

			}
		}

		fclose(fichier);
		//ecritureFichierDepartements(*tab_region, *taille_tab_region);
	}
	// ****************--------------------- deuxieme fichier, fichier des recensements  ---------------******************************

	fichier = NULL;
	token = NULL;
	i = NULL;



#if _DEBUG
	fichier = fopen("../../../../GIT/Recensement/Import/recensements.csv", "r");
#endif
#if !_DEBUG
	fichier = fopen("../Import/recensements.csv", "r");
#endif
	if (fichier == NULL)
	{
		return NULL;
	}
	else
	{
		int nombre_recensements = 0;
		nombre_recensements = nombrePointVirguleDansLigne(fichier) - 2;
		int tableau_annee_reference[50];
		int valeur_recensement_tmp = 0;
		wchar_t nom_ville_tmp[200];
		wchar_t depcom_tmp[200];
		wchar_t no_dep_tmp[15];
		int is_num_dep_egal = 0;
		int nombre_ville_total = 0;
		Departement** tmp;

		rewind(fichier);
		fgetws(ligne_en_cours, sizeof(ligne_en_cours), fichier);
		token = wcstok(ligne_en_cours, ";");
		wcscpy(ligne_en_cours, token);
		for (i = 0; i < 2; i++)
		{
			token = wcstok(NULL, ";");
			wcscpy(ligne_en_cours, token);
		}

		for (i = 0; i < nombre_recensements - 1; i++)
		{
			token = wcstok(NULL, ";");
			wcscpy(ligne_en_cours, token);
			tableau_annee_reference[i] = _wtoi(ligne_en_cours);

		}
		token = wcstok(NULL, "\n");
		wcscpy(ligne_en_cours, token);
		tableau_annee_reference[i] = _wtoi(ligne_en_cours);

		while (fgetws(ligne_en_cours, sizeof(ligne_en_cours), fichier) != NULL)
		{
			//wprintf(L"%ls", ligne_en_cours);
			token = wcstok(ligne_en_cours, ";");
			wcscpy(depcom_tmp, token);//depcom
			token = wcstok(NULL, ";");
			wcscpy(no_dep_tmp, token);//dep
			token = wcstok(NULL, ";");
			wcscpy(nom_ville_tmp, token);//nomville

			for (i = 0; i < **taille_tab_region; i++)//pour chaque région
			{
				tmp = &(((*tab_region) + i)->tab_departement);
				for (int j = 0; j < *(((*tab_region) + i)->taille_tab_departement); j++)//pour chaque dep de cette region
				{
					
					/*wprintf(L"No de dep en cours : %ls \n", ((*tmp) + j)->numero_dep);
					wprintf(L"No lu dans fichier : %ls \n", no_dep_tmp);*/

					if ((wcscmp(((*tmp) + j)->numero_dep, L"2A") == 0) || (wcscmp(((*tmp) + j)->numero_dep, L"2B") == 0) || (wcscmp(no_dep_tmp, L"2B") == 0) || (wcscmp(no_dep_tmp, L"2A") == 0))//cas speciaux de la CORSE
					{
						if (wcscmp((((*tmp) + j)->numero_dep), no_dep_tmp) == 0)
						{
							is_num_dep_egal = 1;
						}
					}
					else
					{
						if (_wtoi(((*tmp) + j)->numero_dep) == _wtoi(no_dep_tmp))
						{
							is_num_dep_egal = 1;
						}
					}
					if (is_num_dep_egal) //si le no du dep = no en cours => ya SOUCIS
					{
						((*tmp) + j)->tab_ville = ajouterVille(&(((*tmp) + j)->tab_ville), nom_ville_tmp, depcom_tmp, &(((*tmp) + j)->taille_tab_ville));
						nombre_ville_total++;

						if (nombre_ville_total == 9120)
						{
							printf("ca va peter");
						}

						/*wprintf(L"%ls ajouté dans la region %ls", nom_ville_tmp, ((*tab_region) + i)->nom_reg);
						wprintf(L" dans le departement %ls\n", (((*tmp) + j)->nom_dep));*/
						printf("nombre ville total : %d \n", nombre_ville_total);
						int taille_tableau_ville_tmp = *(((*tmp) + j)->taille_tab_ville);
						Ville** ville_tmp = &(((*tmp) + j)->tab_ville);
						Ville* ville_en_cours = ((*ville_tmp) + taille_tableau_ville_tmp - 1);

						int k;

						for (k = 0; k < nombre_recensements - 1; k++)
						{
							token = wcstok(NULL, ";");
							wcscpy(ligne_en_cours, token);
							valeur_recensement_tmp = _wtoi(ligne_en_cours);
							ville_en_cours->tab_recensement = ajouterRecensement(&(ville_en_cours->tab_recensement), tableau_annee_reference[k], valeur_recensement_tmp, &(ville_en_cours->taille_tab_recensement));
						}
						token = wcstok(NULL, "\n");
						wcscpy(ligne_en_cours, token);
						valeur_recensement_tmp = _wtoi(ligne_en_cours);
						ville_en_cours->tab_recensement = ajouterRecensement(&(ville_en_cours->tab_recensement), tableau_annee_reference[k], valeur_recensement_tmp, &(ville_en_cours->taille_tab_recensement));
						is_num_dep_egal = 0;
					}
				}
			}
		}
	}

	fclose(fichier);
}

void ecritureFichierDepartements(Region* tab_region, int* taille_tab_region)
{
	FILE* fichier = NULL;
	int i;
	Departement* tmp;
#if _DEBUG
	fichier = _wfopen(L"../../../../GIT/Recensement/Import/departements_test.csv", L"w+");//!!!--A changer --!!!
#endif
#if !_DEBUG
	fichier = _wfopen("../Import/departements.csv", "w+");
#endif
	if (fichier == NULL)
	{
		return NULL;
	}
	else
	{
		for (i = 0; i < *taille_tab_region; i++)//héhéhé
		{
			tmp = ((tab_region)+i)->tab_departement;
			for (int j = 0; j < *((tab_region)+i)->taille_tab_departement; j++)//pour chaque dep de cette region	
			{
				fwprintf(fichier, L"%ls;%ls;%ls;%ls\n", (tmp + j)->numero_dep, (tmp + j)->nom_dep, (tmp + j)->prefecture, (tab_region + i)->nom_reg);
			}
		}
		fclose(fichier);
	}

}

void ecritureFichierRecensements(Region* tab_region, int* taille_tab_region)
{
	FILE* fichier = NULL;
	Departement* tmp;
	Ville* ville_tmp;
	Recensement* recen_tmp;
	int i;

#if _DEBUG
	fichier = fopen("../../../../GIT/Recensement/Import/recensements_test.csv", "w+");
#endif
#if !_DEBUG
	fichier = fopen("../Import/recensements.csv", "w+");
#endif
	if (fichier == NULL)
	{
		return NULL;
	}
	else// ! Les recencements sont par ordre antechronologique !
	{
		int nombre_recensements = nombrePointVirguleDansLigne(fichier) - 2;
		fwprintf(fichier, L"%ls;%ls;%ls;", "DEPCOM", "DEP", "LIBMIN");//cas de la premiere ligne

		for (i = 0; i < nombre_recensements; i++)//annees de references
		{
			tmp = tab_region->tab_departement->tab_ville->tab_recensement->annee + i;
			printf(fichier, "%d;", tmp);
		}
		

		for (i = 0; i < *taille_tab_region; i++)
		{
			tmp = ((tab_region)+i)->tab_departement;
			for (int j = 0; j < *((tab_region)+i)->taille_tab_departement; j++)//pour chaque dep de cette region
			{
				ville_tmp = &((tmp) + j)->tab_ville;//test
				fwprintf(fichier, L"\n%d;%d;%ls;", ville_tmp->dep_com, tmp->numero_dep, ville_tmp->nom_ville);
				for (i = 0; i < nombre_recensements; i++)
				{
					recen_tmp = &(ville_tmp->tab_recensement);
					fwprintf(fichier, "%ls;", recen_tmp->valeur_recen + i);//a finir
				}
				
			}
			fclose(fichier);
		}
	}
}
