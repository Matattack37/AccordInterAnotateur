/* Ce programme contient actuellement le main
   il utilise outils_distances.cpp qui utilise lui-même calcul_krippendorff.cpp qui utilise outils_distri_reelles.cpp
   il y a beaucoup de fonctions inutilisées dans ces programmes, restes d'essais infructueux mais conservés néanmoins pour diverses raisons (pas toutes bonnes)...
*/
#include "simulation_sur_distri_reelles_2021_03.hpp"
//#include <random>
//#include <chrono>
//#include "outils_distances.hpp"
//using namespace std;
//#define MAXA 30
//#define MAXIT 400

// Le programme est divisé en deux parties

/*************---------------I - Etude de l'annotation réelle*********
1 - la référence est calculée par vote majoritaire, avec tirage au sort en cas d'exaequo
2 - on étudie ensuite la distribution des "erreurs" (erreur = annotation non conforme à la référence).
    a) Tableau (à une dimension) du taux d'erreurs par item (prise en compte de la difficulté d'annotation par item)
    b) Tableau (à deux dimensions) ; pour chaque item (ligne), taux de répartition des classes "en erreur"  (comment se répartissent les annotations entre les classes autres que la référence = prise en compte de la "confusion" entre classes)

/*************---------------- II- Méthode simulation*******************
1 - on choisit le nombre d'annotateurs
2 - soit deux paramètres m et sigma : on tire au sort le nb d'écarts à la ref pour chaque annotateur (loi normale m, sigma)
3 - ensuite, on utilise soit a ((choix1,choix2)=(1,0)), soit b (choix (0,1)) soit ni a ni b (choix (0,0)),soit les deux (choix (1,1)) où :
a - choix des items sur lesquels l'annotateur a des désaccords avec la ref :
 si choix1=1 : proportionnel au % taux de désaccords à la ref par item
b - si choix2 =1, pour chaque désaccord, le choix de la classe de désaccord est à 75% proportionnel
 à celui observé dans les annotations réelles
Par défaut, choix1 et choix2 sont tous les deux égaux à 1.

/****************************III - stratégie création groupes annotateurs *********
1 - On fixe un taux d'erreurs t (l'écart type étant actuellement constant) par rapport à la référence de l'annotation réelle et on considère que ce taux suit une loi normale de paramètres (t,s)  (où l'écart type s est actuellement une constante) sur le groupe d'annotateurs.
    a) on crée un groupe d'annotateurs ayant ce taux d'erreurs : on calcule sa ref RefA, et son cositem par rapport à une distribution des erreurs au hasard
    b) on crée nb groupes d'annotateurs ayant les mêmes caractéristiques : on calcule le taux d'erreurs moyen de chacune des Ref par rapport à RefA
    c) on réitère nb fois ce processus : on calcule les moyennes des cositem et des taux d'écarts entre Ref
2 - On fait varier le taux d'errurs moyen et on applique le même protocole


 *************************************************************************/
/*************---------------Calcul accord----------*************************
1 - à partir de l'annotation réelle, on calcule une matrice de confusion sur les paires d'annotateurs, par item
2 - on en déduit un écart entre les classes concernant la distribution des paires d'annotation du type (A,B) avec A\=B
3 - on calcule le alpha en utilisant cette mesure d'écart entre classes.
**************************************************************************/

/*
Première partie :
ETUDE DE LA DISTRIBUTION REELLE :    1 : la ref
                                     2 : tableau taux de désaccords à la ref par item ;
                                     3 : un tableau résumant les taux de désaccords par classe pour chaque ITEM */


// I - ETUDE DE LA DISTRIBUTION DES ANNOTATIONS REELLES (REFERENCE et ECARTS à la REFERENCE) :

//La Référence est obtenue par vote majoritaire//
//tirage au sort en cas d'exaequo
//paramètres : tableau des annotations, nb d'annotateurs, nb d'observables, la réf, le nb de classes
void definition_ref(int T[MAXIT][MAXA], int nbA, int nbIt, int Ref[], int nbC) {
  int TS[nbC], choix;
  for (int it = 0; it < nbIt; it++) {
    for (int i = 0; i < nbC; i++)
      TS[i] = 0;
    for (int a = 0; a < nbA; a++) {
      choix = T[it][a];
      TS[choix]++;
    }
    int Max = TS[0], nbex = 0; Ref[it] = 0;
    for (int i = 1; i < nbC; i++) {
      if (TS[i] > Max) {
        Max = TS[i];
        Ref[it] = i;
        nbex = 0;
      }
      else
        if (TS[i] == Max)
          nbex++;
    }
    //cas des exaequo : on les compte et on les stocke
    if (nbex > 1) {
      //cout << "tirage ref\n";
      bool TChoix[nbC];
      for (int i = 0; i < nbC; i++)
        if (TS[i] == Max)
          TChoix[i] = true;
        else
          TChoix[i] = false;
        int tirage = rand() % nbex;
        int ind = 0, candidat = 0;
        while ((ind < nbC) && (candidat < tirage)) {
          if (TChoix[ind])
            candidat++;
          if (candidat < tirage)
            ind++;
        }
        Ref[it] = ind;
    }
  } //fin pour chaque item
}

//calcule le pourcentage du nb d'erreurs à la référence, par item.
//résultat : le tableau TE
void pourcent_erreurs_ref(int Ref[], int T[MAXIT][MAXA], int TE[MAXIT], int nbIt, int nbA) {
  cout << "nbIt = " << nbIt << ", nbA = " << nbA << endl;
  for (int i = 0; i < nbIt; i++)
    TE[i] = 0;
    for (int it = 0; it < nbIt; it++) {
      for (int a = 0; a < nbA; a++)
        if (T[it][a] != Ref[it])
          TE[it] += 1;
    }
   /* cout << "avant la division\n" ;
    for (int it=0;it<nbIt;it++)  cout << TE[it] << ","; cout << endl;
    for (int it=0;it<nbIt;it++) TE[it]=TE[it]/(1.0*nbA);*/
}

//calcule le tableau des erreurs à la ref, par item et par classe (pourcentage par classe, pour chaque item
//le résultat est le tableau TEIt (somme = 1 sur chaque ligne)
void erreurs_ref_par_item_par_classe(int Ref[], int T[MAXIT][MAXA], float TEIt[][MAXCL], int nbIt, int nbA, int nbC) {
  for (int it = 0; it < nbIt; it++) { //pour chaque item
    int TEr[nbC]; //nb de choix par classe différente de la Ref
    int S = 0, Ch = Ref[it]; //classe ref pour it
    for (int c = 0; c < nbC; c++)
      TEr[c] = 0;
    for (int a = 0; a < nbA; a++) {
      int choix = T[it][a];
      if (choix != Ch)
        TEr[choix]++;
    }
    for (int c = 0; c < nbC; c++)
      S += TEr[c]; //somme des "erreurs" sur l'item
    if (S != 0) {
      for (int c = 0; c < nbC; c++)
        TEIt[it][c] = (1.0 * TEr[c]) / S;
    }
    else { //cas d'une classe qui fait l'unanimité
      for (int c = 0; c < nbC; c++)
        if (c == Ch)
          TEIt[it][c] = 0;
        else
          TEIt[it][c] = 1.0 / (nbC - 1);
    }
  }
}

//calcul de la moyenne et de l'écart-type du taux (nb d'écarts à la référence/nb d'annotateurs) par item
void moy_et_ecarttype_pourcenterr_par_item(int Ref[], int T[MAXIT][MAXA], int nbIt, int nbAR, float& moyer,float& siger) {
  moyer = 0;
  float moyercarre = 0;
  for (int it = 0; it < nbIt; it++) {
    int Stemp = 0;
    for (int a = 0; a < nbAR; a++)
      if (T[it][a] != Ref[it])
        Stemp++;
    float ajouter = (1.0 * Stemp) / (1.0 * nbAR);
    moyer += ajouter;
    moyercarre += ajouter * ajouter;
  }
  moyer = moyer / nbIt;
  siger = moyercarre / nbIt - (moyer * moyer);
}

//utilisation groupée des calculs précédents
void etude_distri_reelle(int T[MAXIT][MAXA], int nbAR, int nbIt, int nbC, int Ref[], int TE[], float TEIt[][MAXCL]) {
  definition_ref(T, nbAR, nbIt, Ref, nbC);
  // cout << "\nref\n";
  //for (int i=0;i<nbIt;i++) printf("%2d\t",Ref[i]); cout << endl;
  float moyer,siger;
  pourcent_erreurs_ref(Ref, T, TE, nbIt, nbAR);
  moy_et_ecarttype_pourcenterr_par_item(Ref, T, nbIt, nbAR, moyer, siger);
  cout << "taux erreurs à la ref = " << moyer << endl;
  cout << "ecart-type erreurs à la ref = " << siger << endl;
  erreurs_ref_par_item_par_classe(Ref, T, TEIt, nbIt, nbAR, nbC);
  cout << "TEIt OK\n";
}
/* FIN ETUDE DISTRIBUTION DES DESACCORDS REELS */

// II - SIMULATIONS : DISTRIBUTION DES ERREURS //

//on suppose que le nb de désaccords par annotateurs avec la ref initiale des données réelles suit une loi normale
//résultat : le tableau nbErparA, du nombre d'erreurs (écarts à la ref) pour chaque annotateur
void nberreursparannot(int nbErparA[], int nbA, float moyErparAnnot, float sigmaA, int& nberTot) {
  nberTot = 0;
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generateur (seed);
  normal_distribution<double> distributionA(moyErparAnnot, sigmaA);
  for (int a = 0; a < nbA; a++) { //pour chaque annotateur
    int nbea = round(distributionA(generateur));
    if (nbea < 0)
      nbea = 0;
    nbErparA[a] = nbea;
    nberTot += nbea;
  }
  //cout << "nb d'erreurs par annotateur" << endl;
  //for (int a=0;a<nbA;a++) cout << nbErparA[a] << ", ";
  //cout << endl;
}

/* le choix des écarts à la réf d'un annotateur
 1 - les items de désaccords pour un annotateur sont choisis proportionnellement aux tableaux
       du taux de désaccords par item.
 2 - la valeur de l'annotation est choisie en fonction de la distribution des désaccords sur l'item
     on utilise un biais pour que les classes non choisies par les annotateurs aient une petite chance d'être choisies
*/

//la fonction rend l'indice d'un item pour lequel on aura un écart à la référence
// avec prise en compte de la distribution des désaccords sur un item donné
int min(int a, int b) {
  if (a <= b)
    return a;
  else
    return b;
}

int tirageErreur(int Ref, float TEIt[], int nbC) {
  //cout << "debut tirage erreur : Ref=" << Ref << endl;
  float Tchoix[nbC];
  for (int c = 0; c < nbC; c++)
    if (c == Ref)
      Tchoix[c] = 0;
    else
      Tchoix[c] = 0.9 * TEIt[c] + 0.1 * (nbC - 1); //somme sur Tchoix=1.0 ; on laisse un peu de place au hasard
  float tirage = rand() / RAND_MAX;
  int choix = 0;
  float S = Tchoix[choix];
  while ((choix < nbC) && (S < tirage)) {
    choix++;
    S += Tchoix[choix];
  }
  if (choix == nbC) {
    cout << "bug choix ?" << endl;
    return Ref;
  }
  else
    return choix;
}

// la fonction rend une classe au hasard, différente de la référence
// ici, le choix d'une classe au hasard \= de la Ref (ce n'est pas le choix par défaut)
int tirageErreurhasard(int Ref, int nbC) {
  int ampli = nbC - 1;
  int tirage = rand() % ampli;
  if (tirage < Ref)
    return tirage;
  else
    return tirage + 1;
}


//la fonction rend l'annotation d'un annotateur pour l'ensemble des items (résultat : tableau Tabannot),
//sachant que l'annotateur doit commettre nber nb d'écarts à la ref,
// si choix2, on tient compte de la distribution des classes en erreur sur les annotations réelles
void choixunannotateur1(int nber, int nbIt, int nbC, int Ref[], int Tabannot[], int TE[], float TEIt[][MAXCL], bool& possible, int choix2) {
  int TEtemp[nbIt];
  for (int i = 0; i < nbIt; i++) {
    Tabannot[i] = Ref[i]; // la ref par défaut
    TEtemp[i] = TE[i]; //recopiage de TE dans TEtemp
  }
  int sommediff = 0;
  for (int it = 0; it < nbIt; it++)
    sommediff += TEtemp[it];
  // cout << "sommediff=" << sommediff << endl;
  // les erreurs de l'annotateur
  for (int i = 0; i < nber; i++) { // pour chaque erreur à définir
    int tirage = rand() % sommediff;
    int e = 0 ,stemp = TEtemp[0];
    while ((e < nbIt) && (tirage > stemp)) {
      e++;
      stemp += TEtemp[e];
    }
    if (e < nbIt) {
      sommediff -= TEtemp[e];
      TEtemp[e] = 0;
      //Tabannot[e]=tirageErreur(Ref[e],TEIt[e],nbC);
      if (choix2 == 0)
        Tabannot[e] = tirageErreurhasard(Ref[e], nbC);
      else
        Tabannot[e] = tirageErreur(Ref[e], TEIt[e], nbC);
    }
    else
      possible = false;
  }
}

//même chose dans le cas où les items de désaccords sont choisis au hasard
void choixunannotateur0(int nber, int nbIt, int nbC, int Ref[], int Tabannot[], float TEIt[][MAXCL], bool& possible, int choix2) {
  bool TIt[nbIt];
  for (int it = 0; it < nbIt; it++) {
    TIt[it] = true; //tous les items sont possibles
    Tabannot[it] = Ref[it]; // la ref par défaut
  }
  int sommeIt = nbIt; //nb d'items prenables
  for (int i = 0; i < nber; i++) { // pour chaque erreur à définir
    int tirage = rand() % sommeIt;
    int e = 0, compt  = 0;
    while ((e < nbIt) && (tirage > compt)) {
      e++;
      if (TIt)
        compt++;
    }
    if (e < nbIt) {
      sommeIt--;
      TIt[e] = false;
      if (choix2 == 0)
        Tabannot[e] = tirageErreurhasard(Ref[e], nbC);
      else
        Tabannot[e] = tirageErreur(Ref[e], TEIt[e], nbC);
    }
    else
      possible = false;
  }
}

//Les différentes options pour simuler les annotations avec nber écarts à la ref
void choixunannotateur(int nber, int nbIt, int nbC, int Ref[], int Tabannot[], int TE[], float TEIt[][MAXCL], bool& possible, int choix1, int choix2) {
  if (choix1 == 1) // la distri des erreurs entre items est prise en compte
    choixunannotateur1(nber, nbIt, nbC, Ref, Tabannot, TE, TEIt, possible, choix2);
  else
    choixunannotateur0(nber, nbIt, nbC, Ref, Tabannot, TEIt, possible, choix2);
}

//choix des annotations d'un groupe d'annotateurs
// résultat : Tabannot ; paramètres : le nb d'écarts à la ref par annotateurs et les tableaux de distributions des écarts à la ref dans la distribution réelle
void choixannotations(int nbErparA[], int nbA, int nbIt, int nbC, int Ref[], int TE[], float TEIt[][MAXCL], int Tabannot[][MAXA], bool possible, int choix1, int choix2) {
  for (int a = 0; a < nbA; a++) {
    // cout << "\nannotateur " << a << endl;
    int Tabres[MAXIT];
    choixunannotateur(nbErparA[a], nbIt, nbC, Ref, Tabres, TE, TEIt, possible, choix1, choix2);
    for (int it = 0; it < nbIt; it++)
      Tabannot[it][a] = Tabres[it];
    //cout << "possible=" << possible << endl;
  }
}

/* III - les GROUPES D'ANNOTATEURS */

/*Ref,nbIt,nbC,TE,TEIt = annotation réelle initiale ; nbA,moyA,sigmaA = annotation simulée ; alpha,
 RefA = les résultats ; Tdist tableau des distances entre classes */

//choix des annotations d'un groupe d'annotateurs :
// on récupère Tabannot et la référence induite
void annotations1groupe(int RefIni[], int nbIt, int nbC, int TE[], float TEIt[][MAXCL], int nbA, float moyA, float sigmaA, int Tabannot[][MAXA], int Ref[], int choix1, int choix2) {
  int nbErparA[MAXA];
  int sommeErreurs = 0;
  nberreursparannot(nbErparA, nbA, moyA, sigmaA, sommeErreurs);
  bool possible = true;
  for (int a = 0; a < nbA; a++) {
    //cout << "annotations un groupe a=" << a << endl;
    int Tchoixun[MAXIT];
    choixunannotateur(nbErparA[a], nbIt, nbC, RefIni, Tchoixun, TE, TEIt, possible, choix1, choix2);
    for (int it = 0; it < nbIt; it++)
      Tabannot[it][a] = Tchoixun[it];
  }
  //affiche_annot(Tabannot,nbIt,nbA);
  definition_ref(Tabannot, nbA, nbIt, Ref, nbC);
}

//calcul de la table de confusion entre classes : somme par paires d'annotateurs
void table_confusion_classes(int T[][MAXA], int nbA, int nbIt, int nbC, int TConf[MAXCL][MAXCL]) {
  int c1, c2;
  for (int i = 0; i < nbC; i++)
    for (int j = 0; j < nbC; j++)
      TConf[i][j] = 0;
    for (int a1 = 0; a1 < nbA; a1++)
      for (int a2 = a1 + 1; a2 < nbA; a2++) { //pour chaque paire d'annotateurs distincts
        for (int it = 0; it < nbIt; it++) {
          c1 = T[it][a1];
          c2 = T[it][a2];
          TConf[c1][c2] = TConf[c1][c2] + 1;
          TConf[c2][c1] = TConf[c2][c1] + 1;
        }
      }
}

// conversion des parametres du alpha vers le kappa
void alpha2kappa(int nbIt, int nbA, int TA1[MAXIT][MAXA], int vAnnot[MAXA][MAXIT]) {
  for (int i = 0; i < nbIt; i++) {
    for (int j = 0; j < nbA; j++) {
      vAnnot[j][i] = TA1[i][j];
    }
  }
}

//SIMULATION d'UN GROUPE de nbA annotateurs
void unplusnbGgroupe(string metric, int nbG, int RefIni[], int nbIt, int nbC, int TE[], float TEIt[][MAXCL], int nbA, float tauxErparAnnot, float sigmatauxEr, int choix1, int choix2, float& mtauxErRef, float& sigmatauxErRef, float& mtauxconf, float& accord, float& accordconf, float& cos_uniforme, float& distri_hasard, float& cos_distri_hasard, float& costaux_distri_hasard) {
  //cout << "\n\n Annotations fictives\n";
  float moyA = tauxErparAnnot * nbIt;
  float sigmaA = sigmatauxEr * nbIt;
  srand(time(NULL));
  //création du groupe initial
  int RefA[nbIt], TA[MAXIT][MAXA], TA1[MAXIT][MAXA];
  //tables de confusion normalisées avec et sans la diagonale
  float Tdist[MAXCL][MAXCL], Tdist2[MAXCL][MAXCL];
  annotations1groupe(RefIni, nbIt, nbC, TE, TEIt, nbA, moyA, sigmaA, TA, RefA, choix1, choix2);
  ajout_un(TA, nbIt, nbA, TA1); //classes numérotées de 1 à N
  if (metric == "alpha")
    accord = alpha(TA1, nbIt, nbA, nbC);
  else if (metric == "kappa") {
    int nblignes = nbA, vAnnot[MAXA][MAXIT];
    alpha2kappa(nbIt, nbA, TA1, vAnnot);
    accord = kappaAP(nblignes, nbC, nbA, vAnnot);
  }
  else
    cout << "métrique inconnue" << endl;
  //cout << "alpha=" << alphaR << endl;
  //alphaconf=calcul_alphaconf(TA,nbIt,nbA,nbC,Tdist,Tdist2);
  accordconf = 0;
  //cout << "alphaconf=" << alphaconf << endl;
  //cos_uniforme=cosinus_uniforme(Tdist2,nbC);//distance à une confusion uniforme entre classes
  cos_uniforme = 0;
  //distance_distri_item(TA,nbA,nbIt,nbC,distri_hasard,cos_distri_hasard);
  costaux_distri_hasard = cosinus_taux_distri_hasard(TA, nbA, nbIt, nbC);
  //float taux_desac=taux_desaccord_avec_ref(TA,RefA,nbA,nbIt);
  //cout << "taux_moyen_de_desaccords_avec la ref=" << taux_desac << endl;
  //cout << "cosinus uniforme=" << cos_uniforme << endl;
  //création de nbG groupes sur le même modèle ; on calcule les tauxerreurs à RefA : avec et sans confusion
  mtauxErRef = 0;
  sigmatauxErRef = 0;
  mtauxconf = 0;
  int Refg[nbIt], Tg[MAXIT][MAXA];
  for (int g = 0; g < nbG; g++) {
    annotations1groupe(RefIni, nbIt, nbC, TE, TEIt, nbA, moyA, sigmaA, Tg, Refg, choix1, choix2);
    mtauxconf += tauxdiff2Refdist(RefA, Refg, nbIt, Tdist2);
    mtauxErRef += tauxdiff2Ref(RefA, Refg, nbIt);
  }
  mtauxconf = mtauxconf / nbG;
  mtauxErRef = mtauxErRef / nbG;
  //printf("tauxEr=%.3f, tauxconf=%.3f, alphaconf=%.3f, cos_uniforme=%.3f\n",mtauxErRef,mtauxconf,alphaconf,cos_uniforme);
}

void nbfois_unplusnbGgroupe(string metric, int nb, int nbG, int RefIni[], int nbIt, int nbC, int TE[], float TEIt[][MAXCL], int nbA, float tauxErparAnnot, float sigmatauxEr, int choix1, int choix2, float& moymtauxErRef, float& moysigmatauxErRef, float& moymtauxconf, float& moymetric, float& moymetricconf, float& moycos_uniforme, float& moydistri_hasard, float& moycos_distri_hasard, float& cosmoytaux_distri_hasard) {
  float mtauxErRef, sigmatauxErRef, mtauxconf, accord, accordconf, cos_uniforme, distri_hasard, cos_distri_hasard, costaux_distri_hasard;
  moymtauxErRef = 0;
  moysigmatauxErRef = 0;
  moymtauxconf = 0;
  moymetric = 0;
  moymetricconf = 0;
  moydistri_hasard = 0;
  moycos_distri_hasard = 0;
  for (int i = 0; i < nb; i++) {
    //cout << "\n unplusnbGgroupe, i=" << i << endl;
    unplusnbGgroupe(metric, nbG, RefIni, nbIt, nbC, TE, TEIt, nbA, tauxErparAnnot, sigmatauxEr, choix1, choix2, mtauxErRef, sigmatauxErRef, mtauxconf, accord, accordconf, cos_uniforme, distri_hasard, cos_distri_hasard, costaux_distri_hasard);
    moymtauxErRef += mtauxErRef;
    moysigmatauxErRef += 0;
    moymtauxconf += mtauxconf;
    moymetric += accord;
    moymetricconf += accordconf;
    moycos_uniforme += cos_uniforme;
    moydistri_hasard += distri_hasard;
    moycos_distri_hasard += cos_distri_hasard;
    cosmoytaux_distri_hasard += costaux_distri_hasard;
  }
  moymtauxErRef = moymtauxErRef / nb;
  moymtauxconf = moymtauxconf / nb;
  moymetric = moymetric / nb;
  moymetricconf = moymetricconf / nb;
  moycos_uniforme = moycos_uniforme / nb;
  moydistri_hasard = moydistri_hasard / nb;
  moycos_distri_hasard = moycos_distri_hasard / nb;
  cosmoytaux_distri_hasard = cosmoytaux_distri_hasard / nb;
}

//series d'expés suivant valeurs de taux d'erreurs à la ref initiale (réelle)
void affichage_ligne_R(int nbval, string titre, float Tab[]) {
  cout << titre ;
  cout << "<- c(";
  for (int i = 0; i < (nbval - 1); i++)
    printf("%.4f, ", Tab[i]);
  printf("%.4f)\n", Tab[nbval - 1]);
}

// ecrit les résultats d'expérience dans un fichier csv
void write_res_series(string metric, string corpus, int nbval, float moymtauxErRef[], float moymetric[], float cosmoytaux_distri_hasard[]) {
  string nomfich = "./res/" + metric + "_" + corpus + ".csv";
  ofstream file(nomfich.c_str());
  if (!file)
    cout << "erreur ouverture fichier " << nomfich << endl;
  else {
    file << metric << ",taux,cos" << endl;
    for (int nb = 0; nb < nbval; nb++)
      file << moymetric[nb] << "," << moymtauxErRef[nb] << "," << cosmoytaux_distri_hasard[nb] << endl;
  }
}

void affiche_res_series(string metric, int nbval, float tauxErparAnnot[], float moymtauxErRef[], float moysigmatauxErRef[], float moymtauxconf[], float moymetric[], float moymetricconf[], float moycos_uniforme[], float moydistri_hasard[], float moycos_distri_hasard[], float cosmoytaux_distri_hasard[]) {
  affichage_ligne_R(nbval, metric + "              ", moymetric);
  //affichage_ligne_R(nbval,"alphaconf          ",moyalphaconf);
  //affichage_ligne_R(nbval,"tauxconf           ",moymtauxconf);
  affichage_ligne_R(nbval, "taux               ", moymtauxErRef);
  //affichage_ligne_R(nbval,"cos_uniforme       ",moycos_uniforme);
  //affichage_ligne_R(nbval,"dist_itemrand      ",moydistri_hasard);
  //affichage_ligne_R(nbval,"cos_itemrand       ",moycos_distri_hasard);
  affichage_ligne_R(nbval, "costauxitem_hasard ", cosmoytaux_distri_hasard);
  /*printf("tauStauconf  <- c(");
  for (int i=0;i<(nbval-1);i++)
    printf("%.4f, ",moymtauxErRef[i]/moymtauxconf[i]);
    printf("%.4f)\n",moymtauxErRef[nbval-1]/moymtauxconf[nbval-1]);*/
}

void serie_expes(string metric, int nbval, int nb, int nbG, int RefIni[], int nbIt, int nbC, int TE[], float TEIt[][MAXCL], int nbA, float tauxErparAnnot[], float sigmatauxEr, int choix1, int choix2, float moymtauxErRef[], float moysigmatauxErRef[], float moymtauxconf[], float moymetric[], float moymetricconf[], float moycos_uniforme[], float moydistri_hasard[], float moycos_distri_hasard[], float cosmoytaux_distri_hasard[]) {
  for (int i = 0; i < nbval; i++) {
    //for (int i=0;i<nbval;i++) {
    cout << "test = " << i << endl;
    nbfois_unplusnbGgroupe(metric, nb, nbG, RefIni, nbIt, nbC, TE, TEIt, nbA, tauxErparAnnot[i], sigmatauxEr, choix1, choix2, moymtauxErRef[i], moysigmatauxErRef[i], moymtauxconf[i], moymetric[i], moymetricconf[i], moycos_uniforme[i], moydistri_hasard[i], moycos_distri_hasard[i], cosmoytaux_distri_hasard[i]);
  }
  affiche_res_series(metric, nbval, tauxErparAnnot, moymtauxErRef, moysigmatauxErRef, moymtauxconf, moymetric, moymetricconf, moycos_uniforme, moydistri_hasard, moycos_distri_hasard, cosmoytaux_distri_hasard);
}

int main(int n, char* param[]) {
  int nbC = 5; //par défaut ; modifier à 9 pour les entités nommées à 3 éventuellement pour opinion et émotion
  srand(time(NULL));
  int T[MAXIT][MAXA];
  int nbAR = 0, nbIt = 0;
  int nbA = 5; //nb d'annotateurs dans les groupes simulés
  //LECTURE 5 classes
  string arg1 = param[1];
  string arg2 = param[2];
  int nbtests = 10;
  int nb = 100; //à revoir
  int nbG = 100; //nb de groupes auquel on compare un groupe, 100 par défaut
  int choix1 = 1, choix2 = 1;
  float  sigmatauxEr = 0.05;
  float TabtauxErparAnnot[] = {0.075, 0.1, 0.125, 0.15, 0.175, 0.2, 0.25, 0.3, 0.35, 0.4};
  if (arg1 == "all") {
    string corpus[] = {"emotion", "opinion", "conq_spat", "epidemie", "coref", "similarite", "newsletter"};
    for (int c = 0; c < 7; c++) {
      lecture_corpus(corpus[c], nbC, nbAR, nbIt, T);
      affiche_annot(T, nbIt, nbAR);
      //CALCULS SUR DISTRI Réelles
      int Ref[MAXIT];
      int TE[MAXIT]; //le nombre d'erreurs par item
      float TEIt[MAXIT][MAXCL];
      int TConfusion[MAXCL][MAXCL];
      etude_distri_reelle(T, nbAR, nbIt, nbC, Ref, TE, TEIt);
      table_confusion_classes(T, nbAR, nbIt, nbC, TConfusion);
      float Tdist[MAXCL][MAXCL];
      confusionnormalisee(TConfusion, nbC, Tdist);
      //distancegrossiere(nbC,Tdist);
      //affichage distances annotations réelles
      float SLignes[nbC], SCol[nbC], Sdiag;
      SLSC(Tdist, nbC, SLignes, SCol, Sdiag);
      /* //affichage confusion
      cout << "tableau confusion entre classes\n";
      for (int c1=0;c1<nbC;c1++) {
        for (int c2=0;c2<nbC;c2++)
          printf("%.6f ",Tdist[c1][c2]);
        printf(" | %.6f\n",SLignes[c1]);
      }
      for (int c2=0;c2<nbC;c2++) printf(" ----- ") ; cout << endl;
        for (int c2=0;c2<nbC;c2++) printf("%.6f ",SLignes[c2]);
          printf(" | %.6f\n",Sdiag);
      */
      float sim_uniforme, moy, sigma;
      int nbdesa = 0;
      mesuresrepartitiondesaccords(TConfusion, T, nbC, nbIt, nbAR, sim_uniforme, moy, sigma);
      float Tdist2[MAXCL][MAXCL];
      Tdist2Tdist2(Tdist, nbC, Tdist2);
      //affichage Tdist2 annotations réelles
      cout << "Tdist2\n";
      for (int c1 = 0; c1 < nbC - 1; c1++) {
        for (int c2 = 0; c2 < nbC; c2++)
          if (c2 <= c1)
            printf("%s  ", "_______");
          else
            printf("%f ", Tdist2[c1][c2]);
        cout << endl;
      }
      //ANNOTATIONS FICTIVES
      //float tauxErparAnnot=0.25,sigmatauxEr=0.05;
      vector<int> nbErparA;
      nbErparA.resize(nbA);
      int Tabannot[nbA][MAXIT];
      // SERIES d'expé

      //I - Variations mesure d'accords/taux d'écart entre Rf
      // pour tester
      //float TabtauxErparAnnot[2]={0.2};
      //int nbtests=1;
      // fin pour tester
      //float TabtauxErparAnnot[12]={0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7};
      float moymtauxErRef[nbtests], moysigmatauxErRef[nbtests], moymtauxconf[nbtests], moymetric[nbtests], moymetricconf[nbtests], moycos_uniforme[nbtests], moydistri_hasard[nbtests], moycos_distri_hasard[nbtests], cosmoytaux_distri_hasard[nbtests];
      serie_expes(arg2, nbtests, nb, nbG, Ref, nbIt, nbC, TE, TEIt, nbA, TabtauxErparAnnot, sigmatauxEr, choix1, choix2, moymtauxErRef, moysigmatauxErRef, moymtauxconf, moymetric, moymetricconf, moycos_uniforme, moydistri_hasard, moycos_distri_hasard, cosmoytaux_distri_hasard);
      write_res_series(arg2, corpus[c], nbtests, moymtauxErRef, moymetric, cosmoytaux_distri_hasard);
    }
    return 0;
  }
  else {
    lecture_corpus(arg1, nbC, nbAR, nbIt, T);
    affiche_annot(T, nbIt, nbAR);
    //CALCULS SUR DISTRI Réelles
    int Ref[MAXIT];
    int TE[MAXIT]; //le nombre d'erreurs par item
    float TEIt[MAXIT][MAXCL];
    int TConfusion[MAXCL][MAXCL];
    etude_distri_reelle(T, nbAR, nbIt, nbC, Ref, TE, TEIt);
    table_confusion_classes(T, nbAR, nbIt, nbC, TConfusion);
    float Tdist[MAXCL][MAXCL];
    confusionnormalisee(TConfusion, nbC, Tdist);
    //distancegrossiere(nbC,Tdist);
    //affichage distances annotations réelles
    float SLignes[nbC], SCol[nbC], Sdiag;
    SLSC(Tdist, nbC, SLignes, SCol, Sdiag);
    /* //affichage confusion
    cout << "tableau confusion entre classes\n";
    for (int c1=0;c1<nbC;c1++) {
      for (int c2=0;c2<nbC;c2++)
        printf("%.6f ",Tdist[c1][c2]);
      printf(" | %.6f\n",SLignes[c1]);
    }
    for (int c2=0;c2<nbC;c2++) printf(" ----- ") ; cout << endl;
      for (int c2=0;c2<nbC;c2++) printf("%.6f ",SLignes[c2]);
        printf(" | %.6f\n",Sdiag);
    */
    float sim_uniforme, moy, sigma;
    int nbdesa = 0;
    mesuresrepartitiondesaccords(TConfusion, T, nbC, nbIt, nbAR, sim_uniforme, moy, sigma);
    float Tdist2[MAXCL][MAXCL];
    Tdist2Tdist2(Tdist, nbC, Tdist2);
    //affichage Tdist2 annotations réelles
    cout << "Tdist2\n";
    for (int c1 = 0; c1 < nbC - 1; c1++) {
      for (int c2 = 0; c2 < nbC; c2++)
        if (c2 <= c1)
          printf("%s  ", "_______");
        else
          printf("%f ", Tdist2[c1][c2]);
      cout << endl;
    }
    //ANNOTATIONS FICTIVES
    //float tauxErparAnnot=0.25,sigmatauxEr=0.05;
    vector<int> nbErparA;
    nbErparA.resize(nbA);
    int Tabannot[nbA][MAXIT];
    // SERIES d'expé

    //I - Variations mesure d'accords/taux d'écart entre Rf
    // pour tester
    //float TabtauxErparAnnot[2]={0.2};
    //int nbtests=1;
    // fin pour tester
    //float TabtauxErparAnnot[12]={0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7};
    float moymtauxErRef[nbtests], moysigmatauxErRef[nbtests], moymtauxconf[nbtests], moymetric[nbtests], moymetricconf[nbtests], moycos_uniforme[nbtests], moydistri_hasard[nbtests], moycos_distri_hasard[nbtests], cosmoytaux_distri_hasard[nbtests];
    serie_expes(arg2, nbtests, nb, nbG, Ref, nbIt, nbC, TE, TEIt, nbA, TabtauxErparAnnot, sigmatauxEr, choix1, choix2, moymtauxErRef, moysigmatauxErRef, moymtauxconf, moymetric, moymetricconf, moycos_uniforme, moydistri_hasard, moycos_distri_hasard, cosmoytaux_distri_hasard);
    write_res_series(arg2, arg1, nbtests, moymtauxErRef, moymetric, cosmoytaux_distri_hasard);
    return 0;
  }
}
