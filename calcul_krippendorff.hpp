#ifndef CALCUL_KRIPPENDORFF_HPP_INCLUDED
#define CALCUL_KRIPPENDORFF_HPP_INCLUDED

#include "outils_distri_reelles.hpp"
using namespace std;

void TabnbAnnotParClasseParItem(int tabannot[MAXIT][MAXA], int nbit, int nba, int nbc, int TAnParClasseParItem[MAXCL][MAXIT], int TSommeLigne[], int TSommeCol[MAXIT], int& annotutiles);
float calcul1(int tabannot[MAXIT][MAXA], int nbit, int nba, int classe, int Tnbobs[]);
float calcul2(int tabannot[MAXIT][MAXA], int nbit, int nbannot, int c1, int c2, int Tbnobs[]);
void coincidences(int tabannot[MAXIT][MAXA], int nbit, int nba, int nbc, int Tbnobs[], float C[MAXCL][MAXCL]);
float delta_car(int i, int j);
float delta_euc(int i, int j);
float delta(int i, int j);
void affiche_coincidences(float C[MAXCL][MAXCL], int nbclasses);
float numalpha(int TAnParClasseParItem[MAXCL][MAXIT], int TSomparCol[], int nbit, int nbc);
float denomalpha(int TSomparLigne[], int nbc);
float numalphadist(int TAnParClasseParItem[MAXCL][MAXIT], int TSomparCol[], int nbit, int nbc, float Tdist[MAXCL][MAXCL]);
float denomalphadist(int TSomparLigne[], int nbc, float Tdist[MAXCL][MAXCL]);
float alpha(int tabannot[MAXIT][MAXA], int nbit, int nba, int nbc);
float alphadist(int tabannot[MAXIT][MAXA], int nbit, int nba, int nbc, float Tdist[MAXCL][MAXCL]);

#endif
