#ifndef SIMULATION_SUR_DISTRI_REELLES_2021_03_HPP
#define SIMULATION_SUR_DISTRI_REELLES_2021_03_HPP

#include <random>
#include <chrono>
#include "outils_distances.hpp"
using namespace std;

void definition_ref(int T[MAXIT][MAXA], int nbA, int nbIt, int Ref[], int nbC);
void pourcent_erreurs_ref(int Ref[], int T[MAXIT][MAXA], int TE[MAXIT], int nbIt, int nbA);
void erreurs_ref_par_item_par_classe(int Ref[], int T[MAXIT][MAXA], float TEIt[][MAXCL], int nbIt, int nbA, int nbC);
void moy_et_ecarttype_pourcenterr_par_item(int Ref[], int T[MAXIT][MAXA], int nbIt, int nbAR, float& moyer,float& siger);
void etude_distri_reelle(int T[MAXIT][MAXA], int nbAR, int nbIt, int nbC, int Ref[], int TE[], float TEIt[][MAXCL]);
void nberreursparannot(int nbErparA[], int nbA, float moyErparAnnot, float sigmaA, int& nberTot);
int min(int a, int b);
int tirageErreur(int Ref, float TEIt[], int nbC);
int tirageErreurhasard(int Ref, int nbC);
void choixunannotateur1(int nber, int nbIt, int nbC, int Ref[], int Tabannot[], int TE[], float TEIt[][MAXCL], bool& possible, int choix2);
void choixunannotateur0(int nber, int nbIt, int nbC, int Ref[], int Tabannot[], float TEIt[][MAXCL], bool& possible, int choix2);
void choixunannotateur(int nber, int nbIt, int nbC, int Ref[], int Tabannot[], int TE[], float TEIt[][MAXCL], bool& possible, int choix1, int choix2);
void choixannotations(int nbErparA[], int nbA, int nbIt, int nbC, int Ref[], int TE[], float TEIt[][MAXCL], int Tabannot[][MAXA], bool possible, int choix1, int choix2);
void annotations1groupe(int RefIni[], int nbIt, int nbC, int TE[], float TEIt[][MAXCL], int nbA, float moyA, float sigmaA, int Tabannot[][MAXA], int Ref[], int choix1, int choix2);
void table_confusion_classes(int T[][MAXA], int nbA, int nbIt, int nbC, int TConf[MAXCL][MAXCL]);
void unplusnbGgroupe(int nbG, int RefIni[], int nbIt, int nbC, int TE[], float TEIt[][MAXCL], int nbA, float tauxErparAnnot, float sigmatauxEr, int choix1, int choix2, float& mtauxErRef, float& sigmatauxErRef, float& mtauxconf, float& alphaR, float& alphaconf, float& cos_uniforme, float& distri_hasard, float& cos_distri_hasard, float& costaux_distri_hasard);
void nbfois_unplusnbGgroupe(int nb, int nbG, int RefIni[], int nbIt, int nbC, int TE[], float TEIt[][MAXCL], int nbA, float tauxErparAnnot, float sigmatauxEr, int choix1, int choix2, float& moymtauxErRef, float& moysigmatauxErRef, float& moymtauxconf, float& moyalpha, float& moyalphaconf, float& moycos_uniforme, float& moydistri_hasard, float& moycos_distri_hasard, float& cosmoytaux_distri_hasard);
void affichage_ligne_R(int nbval, string titre, float Tab[]);
void affiche_res_series(int nbval, float tauxErparAnnot[], float moymtauxErRef[], float moysigmatauxErRef[], float moymtauxconf[], float moyalpha[], float moyalphaconf[], float moycos_uniforme[], float moydistri_hasard[], float moycos_distri_hasard[], float cosmoytaux_distri_hasard[]);
void serie_expes(int nbval, int nb, int nbG, int RefIni[], int nbIt, int nbC, int TE[], float TEIt[][MAXCL], int nbA, float tauxErparAnnot[], float sigmatauxEr, int choix1, int choix2, float moymtauxErRef[], float moysigmatauxErRef[], float moymtauxconf[], float moyalpha[], float moyalphaconf[], float moycos_uniforme[], float moydistri_hasard[], float moycos_distri_hasard[], float cosmoytaux_distri_hasard[]);
int main(int n, char* param[]);

#endif
