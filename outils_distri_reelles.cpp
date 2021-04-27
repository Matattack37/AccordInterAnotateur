#include "outils_distri_reelles.hpp"
//calcul d'accords : multikappa et multipi
/*
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

const int Max_obs=3000; //nb maximal d'observables
const int Max_classes = MAXCL; // nb maximal de classes
const int Max_annot = MAXA; // nb maximal d'annotateurs
*/

//lectures_fichiers
string pathcsv() {
  return "./corpus/annotation_" ;
}

void lecture_prem_ligne(string ligne, int lg, int T[MAXIT][MAXA], int& nbA) {
  int i = 0, a = 0;
  while (i < lg) {
    string nblu = "";
    while ((i < lg) && (ligne[i] != '\t')) {
      nblu += ligne[i];
      i++;
    } //on est sur la tabulation
    //cout << "nblu=" << nblu << endl;
    if (nblu[0] == '-')
      T[0][a] = -1 * (nblu[1] - 48) + 2;
    else
      T[0][a] = nblu[0] - 48 + 2;
    a++;
    i++;
    //cout << "ligne lue\n";
  }
  nbA = a;
  cout << nbA << " annotateurs\n";
}

//pour les fichiers déjà sous la forme 0-4
void lecture_prem_ligne2(string ligne, int lg, int T[MAXIT][MAXA], int& nbIt) {
  int i = 0, it = 0;
  while (i < lg) {
    string nblu = "";
    while ((i < lg) && (ligne[i] != '\t')) {
      nblu += ligne[i];
      i++;
    } //on est sur la tabulation
    T[it][0] = nblu[0] - 48;
    it++;
    i++;
  }
  nbIt = it;
  cout << nbIt << " items - ";
}

void lecture_ligne(string ligne, int lg, int it, int T[MAXIT][MAXA]) {
  int i = 0, a = 0;
  while (i < lg) {
    string nblu = "";
    while ((i < lg) && (ligne[i] != '\t')) {
      nblu += ligne[i];
      i++;
    } //on est sur la tabulation
    if (nblu[0] == '-')
      T[it][a] = -1 * (nblu[1] - 48) + 2;
    else
      T[it][a] = nblu[0] - 48 + 2;
    i++;
    a++;
  }
}

//fichiers 0-4
void lecture_ligne2(string ligne, int lg, int a, int T[MAXIT][MAXA]) {
  int i = 0, it = 0;
  while (i < lg) {
    string nblu = "";
    while ((i < lg) && (ligne[i] != '\t')) {
      nblu += ligne[i];
      i++;
    } //on est sur la tabulation
    T[it][a] = nblu[0] - 48;
    i++;
    it++;
  }
}

int lire(string nomfich, int T[MAXIT][MAXA], int& nbA, int& nbIt) {
  string nomfichalire = nomfich;
  cout << "nomfichalire="<< nomfichalire << endl;
  const char* nom_fichier = nomfichalire.c_str();
  ifstream file(nom_fichier);
  if (!file){
    cout << "erreur ouverture fichier " << nomfichalire << endl;
    return 0;
  }
  else {
    int it = 0;
    string ligne;
    int lg;
    getline(file, ligne);
    lg = ligne.size();
    lecture_prem_ligne(ligne, lg, T, nbA);
    while (!file.eof()) {
      getline(file, ligne);
      lg = ligne.size();
      if (lg > 5) { //si ligne non vide, en fait
        it++;
        lecture_ligne(ligne,lg,it,T);
      }
    }
    nbIt = it;
    cout << nbIt << " items\n";
  }
}

int lire2(string nomfich, int T[MAXIT][MAXA], int& nbA, int& nbIt) {
  string nomfichalire = nomfich;
  cout << "nomfichalire=" << nomfichalire << endl;
  const char* nom_fichier = nomfichalire.c_str();
  ifstream file(nom_fichier);
  if (!file){
    cout << "erreur ouverture fichier " << nomfichalire << endl;
    return 0;
  }
  else {
    string ligne;
    int lg;
    nbA = 0;
    getline(file, ligne);
    lg = ligne.size();
    lecture_prem_ligne2(ligne, lg, T, nbIt);
    while (!file.eof()) {
      getline(file, ligne);
      //cout << ligne << endl;
      lg = ligne.size();
      if (lg > 5) {//si ligne non vide, en fait
        nbA++;
        lecture_ligne2(ligne, lg, nbA, T);
      }
    }
    cout << nbA << " annotateurs\n";
  }
}

//lecture des fichiers istex
string pathEtdebutnomcsv() {
  string res = "./corpus/annotations_corpus_gold_istex_";
  return res;
}

string int2string(int i) { //i compris entre 1 et 20
  char C;
  string res = "";
  if (i < 10) {
    C = i + 48;
    res += C;
  }
  else {
    if (i < 20) {
      res += "1";
      C = (i % 10) + 48;
      res += C;
    }
    else
      res += "20";
  }
  return res;
}

void lireistexajout(string nomfich, int T[MAXIT][MAXA], int Nbobs, int& nbobsfich, int Nba) {
  const char* nom_fichier = nomfich.c_str();
  ifstream file(nom_fichier);
  if (!file) {
    cout << "erreur ouverture fichier " << nomfich << endl;
  }
  else { //0
    string ligne;
    int lg;
    nbobsfich = 0;
    getline(file, ligne); //on passe la ligne Nbannotateurs
    getline(file, ligne); //on passe la ligne Nbclasses
    getline(file, ligne); // on lit le nombre d'observations
    lg = ligne.size();
    int pos = ligne.find("Nbobservables=");
    if (pos != string::npos) {
      pos += 14;
      while ((pos < lg) && (ligne[pos] >= 48) && (ligne[pos] <= 57)) {
        nbobsfich = 10 * nbobsfich + ligne[pos] - 48;
        pos++;
      }
      cout << "\nNbobsfich=" << nbobsfich << endl;
    }
    else
      cout << "erreur lecture nbobsfich\n";
    bool commentaire = true;
    while (commentaire) {
      getline(file, ligne);
      commentaire = (ligne[0] == '%');
    } //on n'est plus sur une ligne de commentaire
      // lecture du tableau proprement dit
    for (int an = 0; an < Nba; an++) { //une ligne par annotateur
      lg = ligne.size();
      int i = 0; //indiceligne
      for (int ob = 0; ob < nbobsfich; ob++) { //nbobs à lire
        if (i >= lg)
          cout << "erreur lecture ligne " << an << ", obs " << ob << endl;
        else {
          char Clu = ligne[i];
          if ((Clu == '1') && (i + 1 < lg) && (ligne[i + 1] == '0')) {
            T[Nbobs + ob][an] = 0;
            i = i + 2;
          }
          else {
            int cl = Clu - 48;
            T[Nbobs + ob][an] = cl;
            i++; //on se met sur le ;
          }
          i++; //on passe le ;
        }
      }
      getline(file, ligne);
    } //fin de la lecture des annotations (for...)
  } //fin de la lecture (else //0)
}


/* lecture fichier istex********/
void conversion_classes(int T[MAXIT][MAXA], bool Tabclasses[], int nbA, int nbIt, int nbC) {
  int decalage[9], c;
  decalage[0] = 0;
  int decale = 0;
  for (int c = 0; c < 9; c++) {
    if (Tabclasses[c])
      decalage[c] = decale;
    else
      decale++;
  }
  for (int a = 0; a < nbA; a++)
    for (int i = 0; i < nbIt; i++) {
      c = T[a][i];
      T[i][a] -= decalage[c];
      if ((T[i][a] < 0) || (T[i][a] >= nbC))
        cout << "bug numero classe :" << T[i][a] << "\n";
    }
  cout << "après conversion classes\n";
  for (int it = 0; it < nbIt; it++) {
    for (int a = 0; a < nbA; a++)
      cout << T[it][a] << " ";
    cout << endl;
  }
}

int lireistex(int T[MAXIT][MAXA], int num, int& nbA, int& nbIt, int& nbC) {
  nbA = 8;
  nbIt = 0;
  string nomfich = pathEtdebutnomcsv();
  nomfich += int2string(num);
  nomfich += ".csv";
  const char* nom_fichier = nomfich.c_str();
  ifstream file(nom_fichier);
  if (!file) {
    cout << "erreur ouverture fichier " << nomfich << endl;
  }
  else { //0
    bool Tabclasses[9];
    int Nbclu = 0, Nbobs = 0; //le nombre de classes déclaré dans l'en-tête
    for (int i = 0; i < 9; i++)
      Tabclasses[i] = false;
    string ligne;
    int lg;
    getline(file, ligne); //on passe la ligne Nbannotateurs
    while (((Nbobs == 0) || (Nbclu == 0)) && (!file.eof())) {
      getline(file, ligne);
      lg = ligne.size();
      int pos = 0;
      if (ligne[0] != '%') { //lecture du nombre de classes
        pos = ligne.find("Nbclasses=");
        if ((Nbclu == 0) && (pos != string::npos)) {
          pos += 10;
          while ((pos < lg) && (ligne[pos] >= 48) && (ligne[pos] <= 57)) {
            Nbclu = 10 * Nbclu + ligne[pos] - 48;
            pos++;
          }
          cout << "Nb de classes lues =" << Nbclu << endl;
	        nbC = Nbclu;
        }
        else { //2
          pos = ligne.find("Nbobservables=");
          if ((Nbobs == 0) && (pos != string::npos)) {
            pos += 14;
            while ((pos<lg) && (ligne[pos] >= 48) && (ligne[pos] <= 57)) {
              Nbobs = 10 * Nbobs + ligne[pos] - 48;
              pos++;
            }
            cout << "\nNbobs=" << Nbobs << endl;
          }
        } //fin else 2
      } //fin ligne qui n'est pas un commentaire
    } //fin while lecture paramètres
    bool commentaire = true;
    while (commentaire) {
      getline(file, ligne);
      commentaire = (ligne[0] == '%');
    } //on n'est plus sur une ligne de commentaire
      // lecture du tableau proprement dit
    //cout << "Nba=" << Nba << endl;
    for (int an = 0; an < nbA; an++) { //une ligne par annotateur
      lg = ligne.size();
      int i = 0; //indiceligne
      for (int ob = 0; ob < Nbobs; ob++) { //Nbobs à lire
        if (i >= lg)
          cout << "erreur lecture ligne " << an << ", obs " << ob << endl;
        else {
          char Clu = ligne[i];
          if ((Clu == '1') && (i + 1 < lg) && (ligne[i + 1] == '0')) {
            Tabclasses[0] = true;
            T[ob][an] = 0;
            i = i + 2;
          }
          else {
            int cl = Clu - 48;
            Tabclasses[cl] = true;
            T[ob][an] = cl;
            i++; //on se met sur le ;
          }
          i++; //on passe le ;
        }
      }
      getline(file, ligne);
    } //fin de la lecture des annotations (for...)
    int nbC = 0;
    for (int i = 0; i < 9; i++)
      if (Tabclasses[i])
        nbC++;
    cout << "nb de classes differentes trouvees=" << nbC << endl;
    nbIt = Nbobs;
    conversion_classes(T, Tabclasses, nbA, nbIt, nbC);
  } //fin de la lecture (else //0)
}

//lecture de l'annotation NEWSLETTER
//
void lecture_ligne_news(string ligne, int lg, int it, int T[MAXIT][MAXA], int& nbmanquantes) {
  int i = 0;
  while ((i < lg) && (ligne[i] != ',')) i++;
  i++;
  while ((i < lg) && (ligne[i] != ',')) i++; // on passe les deux premiers champs
  i++;
  //on passe la Ref
  i += 2;
  for (int a = 0; a < 6; a++) {
    int nblu = (int) ligne[i];
    nblu = nblu - 48;
    if ((nblu >= 0) && (nblu <= 10)) {
      if (a > 0) T[it][a - 1] = nblu; //on ne lit pas le premier annotateur
       i += 2;
    }
    else {
       T[it][a] = -1; //valeur manquante
       nbmanquantes++;
       i += 4;
    }
  }
}

int lecture_news(string nomfich, int T[][MAXA], int nbA, int& nbannotations, int& nbIt) {
  const char* nom_fichier = nomfich.c_str();
  ifstream file(nom_fichier);
  if (!file){
    cout << "erreur ouverture fichier " << nomfich << endl;
    return 0;
  }
  else {
    int nbmanquantes = 0;
    int it = 0;
    string ligne;
    int lg;
    getline(file, ligne); //on passe la première ligne
    lg = ligne.size();
    while (!file.eof()) {
      getline(file, ligne);
	    //cout << ligne << endl;
    	lg = ligne.size();
        if (lg > 5) { //si ligne non vide, en fait
          lecture_ligne_news(ligne, lg, it, T, nbmanquantes);
          it++;
		    }
    }
    nbIt = it;
    cout << nbIt << " items\n";
    nbannotations = nbIt * nbA - nbmanquantes;
    cout << "nb total d'annotations=" << nbannotations << endl;
    return 1;
  }
}

void onze2cinqclasses(int T[][MAXA], int nbit, int nba) {
  for (int a = 0; a < nba; a++)
    for (int it = 0; it < nbit; it++) {
      int valini = T[it][a];
      if (valini >= 8)
        T[it][a] = 4;
      else if (valini >= 6)
        T[it][a]=3;
      else  if (valini >= 4)
        T[it][a]=2;
      else  if (valini >= 2)
        T[it][a]=1;
      else  if (valini >= 0)
        T[it][a]=0;
    }
}

void lirenews(int T[][MAXA], int& nbAR, int& nbIt, int& nbC) {
  string nomfich = "./corpus/labelisation_newsletter.csv";
  nbAR = 6;
  int nbannotations = 0;
  lecture_news(nomfich, T, nbAR, nbannotations, nbIt);
  //supprime_colonne_zero(T,nbIt,nbAR);
  onze2cinqclasses(T, nbIt, nbAR);
  nbAR = 5;
}
//FIN lecture newsletter

void cinq2troisclasses(int T[MAXIT][MAXA], int nbA, int nbIt) {
  for (int i = 0; i < nbA; i++)
    for (int j = 0; j < nbIt; j++) {
      if (T[j][i] <= 1)
        T[j][i] = 0;
      else {
        if (T[j][i] == 2)
          T[j][i] = 1;
        else
          T[j][i] = 2;
      }
    }
}

void lecture_corpus(string corpus, int& nbC, int& nbAR, int& nbIt, int T[MAXIT][MAXA]) {
  cout << corpus << endl;
  if (corpus == "emotion") {
    cout << "emotion\n";
    lire("./corpus/annotation_emotion.txt", T, nbAR, nbIt);
    nbAR = 25;
    cout << " 25 annotateurs (2 eliminations) ";  //on élimine les deux derniers
  }
  else {
    if (corpus == "opinion") {
      cout << "opinion\n";
      lire("./corpus/annotation_opinion.txt", T, nbAR, nbIt);
    }
    else {
      if (corpus == "conq_spat") {
        cout << "conquete spatiale\n";
        lire2("./corpus/annotation_conquete_spatiale.txt", T, nbAR, nbIt);
      }
      else {
        if (corpus == "epidemie") {
          cout << "epidemie\n";
          lire2("./corpus/annotation_epidemie.txt", T, nbAR, nbIt);
        }
        else {
          if (corpus == "coref") {
            cout << "coreference\n";
            lire2("./corpus/annotation_typage_coreference.txt", T, nbAR, nbIt);
          }
          else {
            if (corpus == "similarite") {
              cout << "similarite\n";
              lire2("./corpus/annotation_conquete_spatiale.txt", T, nbAR, nbIt);
              nbAR = 0;
              lire2("./corpus/annotation_epidemie.txt", T, nbAR, nbIt);
              nbAR = 9; //suppression d'un annotateur
            }
            else {
              if (corpus == "istex9") {
                cout << "istex9\n";
                lireistex(T, 9, nbAR, nbIt, nbC);
                nbAR = 8;
              }
              else {
                if (corpus == "newsletter") {
                  cout << "newsletter\n";
                  lirenews(T, nbAR, nbIt, nbC);
		            }
                else
                  cout << "erreur nom corpus\n";
              }
            }
          }
        }
      }
    }
  }
  if (nbC == 3)
    cinq2troisclasses(T, nbAR, nbIt);
}

//pour ramener la numérotation des classes de 1 à N
void ajout_un(int T[MAXIT][MAXA], int nbIt, int nba, int T1[MAXIT][MAXA]) {
  for (int it = 0; it < nbIt; it++)
    for (int a = 0; a < nba; a++)
      T1[it][a] = T[it][a] + 1;
}

/* ********** FIN LECTURE ***************/

//CALCULS Kappa
//Artmann Poesio

//CALCULS Kappa
//Artmann Poesio

//nb de fois où l'observable i est classé dans la catégorie k
int xikres(int i, int k, int nba, int vAnnot[][MAXIT]) {
  int j, res = 0;
  for (j = 0; j < nba; j++)
    if (vAnnot[j][i] == k)
      res++;
    //if (res!=0) printf("%d, ",res);
  return res;
}

//pkc=probas pour l'annotateur c de choisir la modalité k
double pkc(int c, int k, int nblignes, int vAnnot[][MAXIT]) {
  int i, nck = 0;
  double res;
  for (i = 0; i < nblignes; i++)
    if (vAnnot[c][i] == k)
      nck++;
  res = (1.0 * nck) / nblignes;
  //printf("pkc(%d,%d)=%f\n",c,k,res);
  return res;
}

double Aekappa(int nblignes, int nbclasses, int nba, int vAnnot[][MAXIT]) {
  int comb2nba = (nba * (nba - 1)) / 2;
  //printf("comb2Nba=%d\n",comb2Nba);
  double res = 0;
  for (int k = 0; k < nbclasses; k++) {
    for (int a1 = 0; a1 < nba - 1; a1++)
      for (int a2 = a1 + 1; a2 < nba; a2++)
	       res = res + pkc(a1, k, nblignes, vAnnot) * pkc(a2, k, nblignes, vAnnot);
  }
  return (res * 1.0) / comb2nba;
}
  /*  for (j1=0;j1<(Nbareel-1);j1++)
    for (j2=j1+1;j2<Nbareel;j2++) {
      res0=res0+pkc(j1,0,nblignes,T)*pkc(j2,0,nblignes,vAnnot);
      res1=res1+pkc(j1,1,nblignes,vAnnot)*pkc(j2,1,nblignes,vAnnot);
    }
    res=(1.0/comb2Nba)*(res0+res1);*/

//on a nbclasses, exprimées dans le tableau vAnnot par un entier de 0 à nbclasses-1
float Aokappa(int nblignes, int nbclasses, int nba, int vAnnot[][MAXIT]) {
  float res = 0;
  int sum = 0, i, nik;
  for (i = 0; i < nblignes; i++) {
    for (int k = 0; k < nbclasses; k++) {
      nik = xikres(i, k, nba, vAnnot);
      sum = sum + nik * (nik - 1);
    }
  }
  //printf("sum=%d\n",sum);
  res = (sum * 1.0) / (nblignes * nba * (nba - 1));
  return res;
}

float Aepi(int nblignes, int nbclasses, int nba, int vAnnot[][MAXIT]) {
  int i, j, Nbtemp[nbclasses]; //pour stocker le nombre de votes pour chaque classe
  float res;
  for (i = 0; i < nbclasses; i++)
    Nbtemp[i] = 0;
  for (i = 0; i < nblignes; i++)
    for (j = 0; j < nba; j++) {
      Nbtemp[vAnnot[j][i]]++;
    }
  res = 0;
  for (i = 0; i < nbclasses; i++)
    res += Nbtemp[i] * Nbtemp[i];
  res = res / (nblignes * nblignes * nba * nba);
  //printf("Aepi=%f\n",res);
  return res;
}

float kappaAP(int nblignes, int nbclasses, int nba, int vAnnot[][MAXIT]) {
  float Ao, Ae;
  Ao = Aokappa(nblignes, nbclasses, nba, vAnnot);
  //printf("Ao=%f\n",Ao);
  Ae = Aekappa(nblignes, nbclasses, nba, vAnnot);
  //printf("Aekappa=%f\n",Ae);
  return (Ao - Ae) / (1.0 - Ae);
}

float piAP(int nblignes, int nbclasses, int nba, int vAnnot[][MAXIT]) {
  float Ao, Ae;
  Ao = Aokappa(nblignes, nbclasses, nba, vAnnot);
  printf("Ao=%f\n", Ao);
  Ae = Aepi(nblignes, nbclasses, nba, vAnnot);
  printf("Aepi=%f\n", Ae);
  return (Ao - Ae) / (1.0 - Ae);
}

//calcul de l'accord sur le tableau vAnnot à nblignes avec nbannot annotateurs,
// résultats  :kappa, pi
void kappa_pi_vecteur(int vAnnot[][MAXIT], int nbannot, int nblignes, int nbclasses, float& res_kappa, float& res_pi) {
  res_kappa = kappaAP(nblignes, nbclasses, nbannot, vAnnot);
  //res_pi=piAP(nblignes,nbclasses,nbannot,vAnnot);
  //cout << "kappa=" << res_kappa << endl;
}

//calcul du ALPHA....

//variable aléatoire - entropie - espérance
int Minimun(int a, int b) {
	if (a > b)
    return b;
  else
    return a;
}

int Maximum(int a, int b) {
	if (a > b)
    return a;
  else
    return b;
}

float entropiep(float p) {
  if (p == 0)
    return 0;
  else
    return -1.0 * p * log2(p);
}

float entropie_va(int nb, int X[]) {
  int sum = 0;
  for (int i = 0; i < nb; i++)
    sum += X[i];
  float res = 0;
  for (int i = 0; i < nb; i++)
    res += entropiep(1.0 * X[i] / sum);
  return res;
}

float esperance_va(int nb, float X[]) {
  float S = 0, E = 0;
  for (int i = 0; i < nb; i++)
    S += X[i];
    for (int i = 0; i < nb; i++)
      E += i * X[i] / S;
  return E;
}

void calculRef(int vAnnot[][MAXIT], int Ref[], int nbo, int nba, int nbc) {
  int nbvotes[nbo][nbc];
  for (int i = 0; i < nbo; i++)
    for (int j = 0; j < nbc; j++)
      nbvotes[i][j] = 0;
  int c;
  for (int o = 0; o < nbo; o++) {
    for (int a = 0; a < nba; a++) {
      c = vAnnot[a][o];
      (nbvotes[o][c])++;
    }
    int max = nbvotes[o][0], classe = 0, nbmax = 1, tirageclasse;
    for (int k = 1; k < nbc; k++) {
      if (max < nbvotes[o][k]) {
        max = nbvotes[o][k];
        classe = k;
        nbmax = 1;
      }
      else if (max == nbvotes[o][k])
        nbmax++;
    }
    if (nbmax == 1)
      Ref[o] = classe;
    else {
      tirageclasse = rand() % nbmax + 1; //on prend la n-ieme classe éligible
      int choixc = 0, c = -1;
      while ((c < nbc) && (choixc < tirageclasse)) {
        c++;
        if (nbvotes[o][c] == max)
          choixc++;
      }
      Ref[o] = c;
    }
  }
}

//calcul du taux de différences entre deux ref
float tauxdiff2Ref(int Ref1[], int Ref2[], int nbo) {
  int nbdiff = 0;
  for (int i = 0; i < nbo; i++)
    if (Ref1[i] != Ref2[i])
      nbdiff++;
  return (1.0 * nbdiff) / nbo;
}

//calcul du taux de désaccords moyen des annotateurs avec la Ref produite
float taux_desaccord_avec_ref(int T[][MAXIT], int RefA[], int nbA, int nbIt) {
  float Tauxdesac = 0;
  for (int a = 0; a < nbA; a++) {
    int nbdesac = 0;
    for (int it = 0; it < nbIt; it++)
      if (T[a][it] != RefA[it])
        nbdesac++;
    Tauxdesac += 1.0 * nbdesac / nbIt;
  }
  return Tauxdesac / nbA;
}

//calcul du taux moyen de différences ntre un ensemble de n Ref : chacune représente une ligne...
float taux_moyen_DiffRef(int vnRef[][MAXIT], int n, int nbo) {
  float res = 0;
  for (int i = 0; i < n - 1; i++)
    for (int j = i + 1; j < n; j++) {
      res += tauxdiff2Ref(vnRef[i], vnRef[j], nbo);
    }
    return 2.0 * res / (n * (n - 1));
}

float ecart_type_TauxDiffRef(int vnRef[][MAXIT], float TDF, int n, int nbo) {
  float Xcarre = 0, temp;
  for (int i = 0; i < n - 1; i++)
    for (int j = i + 1; j < n; j++) {
      temp = tauxdiff2Ref(vnRef[i], vnRef[j], nbo);
      Xcarre += temp * temp;
    }
    return sqrt(2.0 * Xcarre / (n * (n - 1)) - pow(TDF, 2));
}

//Affichage
void  affiche_Ref(int Ref[], int nbIt) {
  cout << endl;
  for (int i = 0; i < nbIt; i++)
    cout << Ref[i] << " ";
  cout << endl;
}

void affiche_annot(int  Tabannot[][MAXA], int nbIt, int nbA) {
  cout << endl;
  for (int it = 0; it < nbIt; it++) {
    for (int a = 0; a < nbA; a++)
      cout << Tabannot[it][a] << " ";
    cout << endl;
  }
}

// tri du tableau de flottants
int comparefloat (const void* a, const void* b) {
  if (*(float*)a < *(float*)b)
    return -1;
  if (*(float*)a == *(float*)b)
    return 0;
  if (*(float*)a >  *(float*)b)
    return 1;
}
