#ifndef OUTILS_DISTANCES_HPP_INCLUDED
#define OUTILS_DISTANCES_HPP_INCLUDED

#include <vector>
#include "calcul_krippendorff.hpp"
using namespace std;

float calcul_distance4(int c1, int c2, int TConf[MAXCL][MAXCL], int nbC);
float calcul_distances3(int TConf[MAXCL][MAXCL], int nbC, float Tdist[MAXCL][MAXCL]);
void distance(int TConf[MAXCL][MAXCL], int nbC, float Tdist[MAXCL][MAXCL], int choix);
void confusionnormalisee(int TConf[MAXCL][MAXCL], int nbC, float Tdist[MAXCL][MAXCL]);
void SLSC(float Tdist[MAXCL][MAXCL], int nbC, float SLignes[], float SCol[], float& Sdiag);
float similaritecos1(int Taboccur[MAXIT][MAXCL], int c1, int c2, int nbIt);
void distancecosinus(int T[MAXA][MAXIT], int nbAR, int nbIt, int nbC, float Tdist[MAXCL][MAXCL]);
void distancegrossiere(int nbC, float Tdist[MAXCL][MAXCL]);
float similaritecos2(vector<float> v1, vector<float> v2, int nbIt);
void mesuresrepartitiondesaccords(int TConf[MAXCL][MAXCL], int T[MAXIT][MAXA], int nbC, int nbIt, int nbAR, float& sim_uniforme, float& moy, float& sigma);
float tauxdiff2Refdist(int Ref1[], int Ref2[], int nbIt, float Tdist[MAXCL][MAXCL]);
void calculmoytaux_ecartdist(int TRefA[][MAXIT], int nbG, int nbIt, float Tdist[MAXCL][MAXCL], float& mtauxecart, float& ettauxecart);
float estimationtauxG(float tauGsim, float Tdist2[MAXCL][MAXCL], int nbC);
void Tdist2Tdist2(float Tdist[MAXCL][MAXCL], int nbC, float Tdist2[MAXCL][MAXCL]);
float distance_uniforme(float Tdist2[MAXCL][MAXCL], int nbC);
float cosinus_uniforme(float Tdist2[MAXCL][MAXCL], int nbC);
int cmpfunct(const void* a, const void* b);
void nb_desaccords_par_item(int TA[MAXIT][MAXA], int nbA, int nbIt, int Vreel[]);
void vect_desaccords_annotation_aleatoire(int nbA, int nbIt, int nbC, int Vrand[]);
float cosinus_vect_entier(int v1[], int v2[], int nbIt);
float distance_vect_entier(int v1[], int v2[], int nbIt);
float distance2_vect_entier(int v1[], int v2[], int nbIt);
void distance_distri_item(int TA[MAXIT][MAXA], int nbA, int nbIt, int nbC, float& distance_annot_hasard, float& cosinus_annot_hasard);
float cosinus_taux_distri_hasard(int T[MAXIT][MAXA], int nbA, int nbIt, int nbC);

#endif
