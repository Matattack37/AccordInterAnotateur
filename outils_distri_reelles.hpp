#ifndef OUTILS_DISTRI_REELLES_HPP_INCLUDED
#define OUTILS_DISTRI_REELLES_HPP_INCLUDED

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <cstdio>
#include <algorithm>
using namespace std;

#define MAXIT 1700
#define MAXA 30
#define MAXCL 12

const int Max_classes = MAXCL; // nb maximal de classes
const int Max_annot = MAXA; // nb maximal d'annotateurs

string pathcsv();
void lecture_prem_ligne(string ligne, int lg, int T[MAXIT][MAXA], int& nbA);
void lecture_prem_ligne2(string ligne, int lg, int T[MAXIT][MAXA], int& nbIt);
void lecture_ligne(string ligne, int lg, int it, int T[MAXIT][MAXA]);
void lecture_ligne2(string ligne, int lg, int a, int T[MAXIT][MAXA]);
int lire(string nomfich, int T[MAXIT][MAXA], int& nbA, int& nbIt);
int lire2(string nomfich, int T[MAXIT][MAXA], int& nbA, int& nbIt);
string pathEtdebutnomcsv();
string int2string(int i);
void lireistexajout(string nomfich, int T[MAXIT][MAXA], int Nbobs, int& nbobsfich, int Nba);
void conversion_classes(int T[MAXIT][MAXA], bool Tabclasses[], int nbA, int nbIt, int nbC);
int lireistex(int T[MAXIT][MAXA], int num, int& nbA, int& nbIt, int& nbC);
void lecture_ligne_news(string ligne, int lg, int it, int T[MAXIT][MAXA], int& nbmanquantes);
int lecture_news(string nomfich, int T[][MAXA], int nbA, int& nbannotations, int& nbIt);
void onze2cinqclasses(int T[][MAXA], int nbit, int nba);
void lirenews(int T[][MAXA], int& nbAR, int& nbIt, int& nbC);
void cinq2troisclasses(int T[MAXIT][MAXA], int nbA, int nbIt);
void lecture_corpus(string corpus, int& nbC, int& nbAR, int& nbIt, int T[MAXIT][MAXA]);
void ajout_un(int T[MAXIT][MAXA], int nbIt, int nba, int T1[MAXIT][MAXA]);
int xikres(int i, int k, int nba, int vAnnot[][MAXIT]);
double pkc(int c, int k, int nblignes, int vAnnot[][MAXIT]);
double Aekappa(int nblignes, int nbclasses, int nba, int vAnnot[][MAXIT]);
float Aokappa(int nblignes, int nbclasses, int nba, int vAnnot[][MAXIT]);
float Aepi(int nblignes, int nbclasses, int nba, int vAnnot[][MAXIT]);
float kappaAP(int nblignes, int nbclasses, int nba, int vAnnot[][MAXIT]);
float piAP(int nblignes, int nbclasses, int nba, int vAnnot[][MAXIT]);
void kappa_pi_vecteur(int vAnnot[][MAXIT], int nbannot, int nblignes, int nbclasses, float& res_kappa, float& res_pi);
int Minimun(int a, int b);
int Maximum(int a, int b);
float entropiep(float p);
float entropie_va(int nb, int X[]);
float esperance_va(int nb, float X[]);
void calculRef(int vAnnot[][MAXIT], int Ref[], int nbo, int nba, int nbc);
float tauxdiff2Ref(int Ref1[], int Ref2[], int nbo);
float taux_desaccord_avec_ref(int T[][MAXIT], int RefA[], int nbA, int nbIt);
float taux_moyen_DiffRef(int vnRef[][MAXIT], int n, int nbo);
float ecart_type_TauxDiffRef(int vnRef[][MAXIT], float TDF, int n, int nbo);
void  affiche_Ref(int Ref[], int nbIt);
void affiche_annot(int  Tabannot[][MAXA], int nbIt, int nbA);
int comparefloat (const void* a, const void* b);

#endif
