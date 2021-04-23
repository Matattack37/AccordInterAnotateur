#include "calcul_krippendorff.cpp"

//autre piste : normaliser les désaccords contenus dans la table de confusion et les utiliser comme
//coefficient de similarité entre les classes

//mieux ? demi-somme des sommes des désaccords autres que c2 concernant c1
//divisé par  les désaccords concernant c1 et réciproquement
float calcul_distance4(int c1, int c2, int TConf[MAXCL][MAXCL], int nbC) {
  if (c1 == c2)
    return 1;
  else {
    int S1 = 0, S2 = 0;
    float res;
    for (int j = 0; j < nbC; j++) {
      if (j != c1)
        S1 += TConf[c1][j];
      if (j != c2)
        S2 += TConf[c2][j];
      }
      //res=0.5*((1.0*(S1-TConf[c1][c2]))/(1.0*S1)+(1.0*(S2-TConf[c2][c1]))/(1.0*S2));//distance
      res = 0.5 * ((1.0 * TConf[c1][c2]) / (1.0 * S1) + (1.0 * TConf[c2][c1]) / (1.0 * S2)); //similarite
      return res;
  }
}

//calcul basé sur les taux de désaccords entre classes
float calcul_distances3(int TConf[MAXCL][MAXCL], int nbC, float Tdist[MAXCL][MAXCL]) {
  float Ttauxdesa[nbC][nbC];
  for (int c1 = 0; c1 < nbC; c1++) {
    int S = 0;
    for (int c2 = 0; c2 < nbC; c2++)
      if (c1 != c2)
        S += TConf[c1][c2];
    for (int c2 = 0; c2 < nbC; c2++)
      if (c1 == c2)
        Ttauxdesa[c1][c2] = 0;
      else
        Ttauxdesa[c1][c2] = (1.0 * TConf[c1][c2]) / S;
    }
    //affichage Tconf
    cout << "table distance3\n";
    for (int c1 = 0; c1 < nbC; c1++) {
      for (int c2 = 0; c2 < nbC; c2++)
        printf("%5.2f  ", Tdist[c1][c2]);
      cout << endl;
  }
  for (int c1 = 0; c1 < nbC; c1++)
    for (int c2 = 0; c2 < nbC; c2++)
      if (c1 == c2)
        Tdist[c1][c2] = 0;
      else
        Tdist[c1][c2] = 1 - Ttauxdesa[c1][c2];
}

//il reste deux choix 3 ou 4 (le meilleur)
void distance(int TConf[MAXCL][MAXCL], int nbC, float Tdist[MAXCL][MAXCL], int choix) {
  if (choix == 4) {
    for (int c1 = 0; c1 < nbC; c1++)
      for (int c2 = 0; c2 < nbC; c2++)
        Tdist[c1][c2] = calcul_distance4(c1, c2, TConf, nbC);
  }
  else
    calcul_distances3(TConf, nbC, Tdist);
    //affichage de la table des distances
    cout << "similarite 4\n";
    /*for (int c1=0;c1<nbC;c1++){
      for (int c2=0;c2<nbC;c2++)
        printf("%5.2f  ",Tdist[c1][c2]);
      cout << endl;
    } */
}

//autre piste : normaliser les désaccords contenus dans la table de confusion et les utiliser comme
//coefficient de similarité entre les classes
void confusionnormalisee(int TConf[MAXCL][MAXCL], int nbC, float Tdist[MAXCL][MAXCL]) {
  int S = 0;
  for (int c1 = 0; c1 < nbC; c1++)
    for (int c2 = 0; c2 < nbC; c2++)
      S += TConf[c1][c2];
  //  cout << "S=" << S << endl;
  for (int c1 = 0; c1 < nbC; c1++)
    for (int c2 = 0; c2 < nbC; c2++) {
      Tdist[c1][c2] = (1.0 * TConf[c1][c2]) / (1.0 * S);
      if (Tdist[c1][c2] < 0)
        Tdist[c1][c2] = 0;
      }
  //affichage de la table de confusion normalisée
 /* cout << "table de confusion normalisée\n";
   for (int c1=0;c1<nbC;c1++){
      for (int c2=0;c2<nbC;c2++)
        printf("%6.4f  ",Tdist[c1][c2]);
      cout << endl;
    } */
}

void SLSC(float Tdist[MAXCL][MAXCL], int nbC, float SLignes[], float SCol[], float &Sdiag) {
  float SC, SL;
  Sdiag = 0;
  for (int i = 0; i < nbC; i++) {
    Sdiag += Tdist[i][i];
    SC = 0;
    SL = 0;
    for (int j = 0; j < nbC; j++) {
      SC += Tdist[j][i];
      SL += Tdist[i][j];
    }
    SLignes[i] = SL;
    SCol[i] = SC;
  }
}

//distance cosinus entre classes
float similaritecos1(int Taboccur[MAXIT][MAXCL], int c1, int c2, int nbIt) {
    float res;
    float num = 0, v1car = 0 , v2car = 0;
    for (int it = 0;it < nbIt; it++) {
      v1car += 1.0 * Taboccur[it][c1] * Taboccur[it][c1];
      v2car += 1.0 * Taboccur[it][c2] * Taboccur[it][c2];
      num += 1.0 * Taboccur[it][c1] * Taboccur[it][c2];
    }
    res = (num / (sqrt(v1car) * sqrt(v2car)));
    return res;
}

void distancecosinus(int T[MAXA][MAXIT], int nbAR, int nbIt, int nbC, float Tdist[MAXCL][MAXCL]) {
  int Taboccur[MAXIT][MAXCL], choix;
  //construction de la table des occurrences des classes par item
  for (int it = 0; it < nbIt; it++) {
    for (int c = 0; c < nbC; c++)
      Taboccur[it][c] = 0;
    for (int a = 0; a < nbAR; a++) {
      choix=T[a][it];
      Taboccur[it][choix]++;
    }
  }
  //calcul des distances entre classes
  for (int c1 = 0; c1 < nbC; c1++)
    for (int c2 = c1; c2 < nbC; c2++) {
      Tdist[c1][c2] = similaritecos1(Taboccur, c1, c2, nbIt);
      if (c2 != c1)
        Tdist[c2][c1] = Tdist[c1][c2];
    }
}

void distancegrossiere(int nbC, float Tdist[MAXCL][MAXCL]) {
  for (int c1 = 0; c1 < nbC; c1++)
    for (int c2 = 0; c2 < nbC; c2++)
      if (c2 == c1)
        Tdist[c1][c2] = 0;
      else
        Tdist[c1][c2] = 1;
}

// calcul de la similarité : répartition uniforme des désaccords avec la répartition réelle
float similaritecos2(vector<float> v1, vector<float> v2, int nbIt) {
  float num = 0, v1car = 0, v2car = 0;
  for (int it = 0; it < nbIt; it++) {
    v1car += 1.0 * v1[it] * v1[it];
    v2car += 1.0 * v2[it] * v2[it];
    num += 1.0 * v1[it] * v2[it];
  }
  float res = (num / (sqrt(v1car) * sqrt(v2car)));
  return res;
}

//distance coisinus entre v1 : mesures de desaccords par item
void mesuresrepartitiondesaccords(int TConf[MAXCL][MAXCL], int T[MAXIT][MAXA], int nbC, int nbIt, int nbAR, float &sim_uniforme, float &moy, float &sigma) {
  int nbdesa = 0;
  for (int c1 = 0; c1 < nbC; c1++) {
    for (int c2 = 0; c2 <= c1; c2++) {
      if (c1 != c2)
        nbdesa += TConf[c1][c2];
    }
  }
  vector<float> v1(nbIt), v2(nbIt);
  for (int i = 0; i < nbIt; i++) {
    v1[i] = (1.0 * nbdesa) / nbIt;
  }
  float S = 0, S2 = 0, ajout;
  for (int it = 0; it < nbIt; it++) {
    v2[it] = 0;
    for (int a1 = 0; a1 < nbAR; a1++)
      for (int a2 = 0; a2 < a1; a2++)
        if (T[it][a1] != T[it][a2])
          v2[it] = v2[it] + 1;
        ajout = (2.0 * v2[it]) / (nbAR * (nbAR - 1));
        S += ajout;
        S2 += ajout * ajout;
  }
  sim_uniforme = similaritecos2(v1, v2, nbIt);
  moy = S / nbIt;
  sigma = sqrt(S2 / nbIt - moy * moy);
  cout << "sim=" << sim_uniforme <<", moy=" << moy << ", sigma=" <<sigma << endl;
}

//taux d'écarts entre deux ref, pondéré par la distance entre classes
float tauxdiff2Refdist(int Ref1[], int Ref2[], int nbIt, float Tdist[MAXCL][MAXCL]) {
  float sommediff = 0;
  int c1, c2;
  for (int i = 0; i < nbIt; i++) {
    c1 = Ref1[i];
    c2 = Ref2[i];
    if (c1 != c2)
      sommediff += Tdist[c1][c2];
  }
  return sommediff / nbIt;
}

/***********************************************************/
void calculmoytaux_ecartdist(int TRefA[][MAXIT], int nbG, int nbIt, float Tdist[MAXCL][MAXCL], float &mtauxecart, float &ettauxecart) {
  mtauxecart = 0;
  ettauxecart = 0;
  float tecarre = 0;
  int ci, cj;
  for (int i = 1; i < nbG; i++) {
    for (int j = 0; j < i; j++) { //pour chaque paire de groupes d'annotateurs
      float sommeecarts = 0;
      for (int it = 0; it < nbIt; it++) {
        ci = TRefA[i][it];
        cj = TRefA[j][it];
        if (ci != cj)
          sommeecarts += Tdist[ci][cj];
          // cout << "nb ecarts=" << nbecarts << endl;
      }
      float te = sommeecarts / nbIt;
      float te2 = te * te;
      mtauxecart += te;
      tecarre += te2;
    }
  }
  mtauxecart = mtauxecart / (nbG * (nbG - 1));
  ettauxecart = tecarre / (nbG * (nbG - 1)) - (mtauxecart * mtauxecart);
}

/*************************************************/
// test de taug=1/(somme Tconf)tauGsim
float estimationtauxG(float tauGsim, float Tdist2[MAXCL][MAXCL], int nbC) {
  float S = 0;
  for (int c1 = 0; c1 < nbC; c1++)
    for (int c2 = c1 + 1; c2<nbC; c2++) {
      if (Tdist2[c1][c2] >= 0.001)
        S += Tdist2[c1][c2];
    }
  return tauGsim/S;
}

/*
float estimationtauxG(float tauGsim,float Tdist[MAXCL][MAXCL],int nbC) {
    float S=0;
    for (int c1=0;c1<nbC;c1++)
        S+=Tdist[c1][c1];
    //cout << "prod=" << S << endl;
    return tauGsim/S;// pourquoi 4 ????
}*/
//normalisation sur les i\=j
void Tdist2Tdist2(float Tdist[MAXCL][MAXCL], int nbC, float Tdist2[MAXCL][MAXCL]) {
  float S = 0;
  for (int i = 0; i < nbC; i++)
    for (int j = i + 1; j < nbC; j++)
      S += Tdist[i][j];
  for (int i = 0; i < nbC; i++)
    for (int j = 0; j < nbC; j++) {
      if (i == j)
        Tdist2[i][j] = Tdist[i][j];
      else {
        if (i < j)
          Tdist2[i][j] = Tdist[i][j] / S;
        else
          Tdist2[i][j] = Tdist[j][i] / S;
      }
    }
}

//distance à une répartition uniforme des cij i\=j
float distance_uniforme(float Tdist2[MAXCL][MAXCL], int nbC) {
  int nb = (nbC * (nbC - 1)) / 2;
  float moy = 1.0 / (1.0 * nb);
  float S = 0;
  for (int l = 0; l < nbC - 1; l++)
    for (int c = l + 1; c < nbC; c++)
      S += abs(Tdist2[l][c] - moy);
  return S;
}

float cosinus_uniforme(float Tdist2[MAXCL][MAXCL], int nbC) {
  int nb = (nbC * (nbC - 1)) / 2;
  float moy = 1.0 / (1.0 * nb);
  float N1 = 0, N2 = 0, Sc = 0;
  for (int l = 0; l < nbC - 1; l++)
    for (int c = l + 1; c < nbC; c++) {
      Sc += Tdist2[l][c] * moy;
      N1 += Tdist2[l][c] * Tdist2[l][c];
      N2 += moy * moy;
    }
  float res = Sc / (sqrt(N1) * sqrt(N2));
  return res;
}

//distance à une répartition du nombre de désaccords sur les items par une annotation au hasard...
//1 - annotation réelle (TA)
int cmpfunct(const void *a,const void *b) {
  return (*(int*) a - *(int*)b);
}

void nb_desaccords_par_item(int TA[MAXIT][MAXA], int nbA, int nbIt, int Vreel[]) {
  /* //"la table..."
  //cout << "annotations\n";
  affiche_annot(TA,nbA,nbIt);
  cout << "fin annotations\n";*/
  for (int it = 0; it < nbIt; it++) {
    Vreel[it] = 0;
    for (int a1 = 0; a1 < nbA; a1++)
      for (int a2 = a1 + 1; a2 < nbA; a2++)
        if (TA[it][a1] != TA[it][a2])
          Vreel[it]++;
  }
  //sort(Vreel,Vreel+nbIt);
  qsort(Vreel, MAXIT, sizeof(int), cmpfunct);
  /* cout << "distri réelle\n";
  for (int it=0;it<nbIt;it++) cout << Vreel[it] << " ";
  cout << endl;*/
}

//2 - le nombre de désaccords n'est pas binômial : car les désaccords sur les paires ne sont pas indépendants
// on compare donc avec une annotation "au hasard" : on pourrait la simuler, dans le cas de 5 classes/5annotateurs,
// on peut la calculer
void vect_desaccords_annotation_aleatoire(int nbA, int nbIt, int nbC, int Vrand[]) {
  if ((nbC == 5) && (nbA == 5)) {
    int Tval[7] = {0, 4, 6, 7, 8, 9, 10};
    int Tcomb[7] = {1, 20, 40, 120, 180, 240, 24};
    int it = 0;
    float retenue = 0;
    for (int v = 0; v < 7; v++) {
      float nbvr = (Tcomb[v] * 1.0 * nbIt / 625) + retenue;
      int nbv = round(nbvr);
      retenue = nbvr - round(nbvr);
      for (int i = 0; i < nbv; i++) {
        Vrand[it] = Tval[v];
        it++;
      }
    }
  }
  else
    cout << "desaccords aleatoires : calcul ou simulation à faire\n";
}

float cosinus_vect_entier(int v1[], int v2[], int nbIt) {
  float num = 0, v1car = 0, v2car = 0;
  for (int it = 0; it < nbIt; it++) {
    v1car += 1.0 * v1[it] * v1[it];
    v2car += 1.0 * v2[it] * v2[it];
    num += 1.0 * v1[it] * v2[it];
  }
  float res = (num / (sqrt(v1car) * sqrt(v2car)));
}

float distance_vect_entier(int v1[], int v2[], int nbIt) {
  float res = 0;
  for (int it = 0; it < nbIt; it++) {
    res += abs(v1[it] - v2[it]);
  }
  res = (1.0 * res) / nbIt;
  return res;
}

//on compare le nombre d'items ayant entre 0 et 4 modifs (moins de 25%)
float distance2_vect_entier(int v1[], int v2[], int nbIt) {
  int S1 = 0, S2=0;
  int i = 0;
  while (v1[i] <= 0) {
    S1++;
    i++;
  }
  i = 0;
  while (v2[i] <= 0) {
    S2++;
    i++;
  }
  return (1.0 * abs(S1 - S2) / nbIt);
}

void distance_distri_item(int TA[MAXIT][MAXA], int nbA, int nbIt, int nbC, float &distance_annot_hasard, float &cosinus_annot_hasard) {
  int VdesReel[nbIt],Vrand[nbIt];
  nb_desaccords_par_item(TA, nbA, nbIt, VdesReel);
  //cout << "VdesReel= ";
  //for (int i=0;i<nbIt;i++) cout << VdesReel[i] << " ";
  //vect_desaccords_annotation_aleatoire(nbA,nbIt,nbC,Vrand);
  //cout << "\nVrand= ";
  //for (int i=0;i<nbIt;i++) cout << Vrand[i] << " ";
  //cout << endl;
  cosinus_annot_hasard = cosinus_vect_entier(Vrand, VdesReel, nbIt);
  //distance_annot_hasard=distance_vect_entier(Vrand,VdesReel,nbIt);
  distance_annot_hasard = distance2_vect_entier(Vrand, VdesReel, nbIt);
}

//Rend, dans le cas où l'on a 5 classes et 5 annotateurs, le cosinus entre 2 vecteurs dont les composantes correspondent au nombre de désaccords possibles par item entre les paires d'annotateurs :
// dans le cas de 5 annotateurs et 5 classes, on a 7 possibilités (Tval)
//     1 - vecteur qui correspond au cas d'une annotation au hasard (Tcomb)
//     2 - vecteur observé dans l'annotation réelle
float cosinus_taux_distri_hasard(int T[MAXIT][MAXA], int nbA, int nbIt, int nbC) {
  if ((nbC == 5) && (nbA == 5)) {
    int Tcomb[7] = {1, 20, 40, 120, 180, 240, 24};
    int Treel[7] = {0, 0, 0, 0, 0, 0, 0};
    int Tval[7] = {0, 4, 6, 7, 8, 9, 10};
    for (int it = 0; it < nbIt; it++) {
      int nbdes = 0;
      for (int a1 = 0; a1 < nbA; a1++)
        for (int a2 = a1 + 1; a2 < nbA; a2++)
          if (T[it][a1] != T[it][a2])
            nbdes++;
          int ind = 0;
          while ((ind < 7) && (nbdes > Tval[ind]))
            ind++;
          if (ind >= 7)
            cout << "bug cosinus_taux_distri_hasard\n";
          else
            (Treel[ind])++;
    }
    return cosinus_vect_entier(Tcomb, Treel, 7);
  }
  else {
    cout << "pas de calcul prévu pour cosinus_taux_distri_hasard\n ";
    return 0;
  }
}
