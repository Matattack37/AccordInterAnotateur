#include <iostream>
using namespace std;
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath> 
#include <cstring>
#include <fstream>
#include <cstdio>
#include "outils_distri_reelles.cpp"




//CALCUL du alpha de KRIPPENDORFF avec données manquantes
//les classes sont numérotées de 1 à n
// une donnée manquante est indiquée par un zéro

//remplit le tableau classes x observables du nombre d'annotations 

void TabnbAnnotParClasseParItem(int tabannot[MAXIT][MAXA],int nbit,int nba,int nbc,
				int TAnParClasseParItem[MAXCL][MAXIT],int TSommeLigne[],
				int TSommeCol[MAXIT],int & annotutiles){
  // initialisations
  for (int it=0;it<nbit;it++){
    TSommeCol[it]=0;
    for (int c=0;c<nbc;c++) TAnParClasseParItem[c][it]=0;
  }
  for (int c=0;c<nbc;c++) TSommeLigne[c]=0 ;
  //remplissage de TAnParClasseParItem et TSommeCol
  for (int it=0;it<nbit;it++){
    for (int a=0;a<nba;a++){
      int c=tabannot[it][a]-1; //numérotation des classes
      if (c>=0) {
	TAnParClasseParItem[c][it]+=1;
        TSommeCol[it]+=1;
      }
    }
  }
  annotutiles=0;
  for (int c=0;c<nbc;c++) {
    for (int it=0;it<nbit;it++){
      if (TSommeCol[it]>1){
	TSommeLigne[c]+=TAnParClasseParItem[c][it];
	annotutiles+=TAnParClasseParItem[c][it];
      }
    }
  }
}

/* calcul de TC[classe][classe]
   paramètres : tableau des annotations, nb d'observables et d'annotateurs, la classe concernée, le tableau du nombre d'observations par item*/

float calcul1(int tabannot[MAXIT][MAXA],int nbit,int nba,int classe,int Tnbobs[]){
  float res=0;
  for (int it=0;it<nbit;it++){//pour chaque observable
    int temp=0;
    for (int k1=0;k1<nba;k1++)
      for (int k2=0;k2<nba;k2++)
	if ((k1!=k2)&&(tabannot[it][k1]==classe+1)&&(tabannot[it][k2]==classe+1)) temp++;
    if (Tnbobs[it]>1) res+=(temp*1.0)/(Tnbobs[it]-1);
  }
  return res;
}


float calcul2(int tabannot[MAXIT][MAXA],int nbit,int nbannot,int c1,int c2,int Tbnobs[]) {
  float tempi,tempj,res=0;
  for (int it=0;it<nbit;it++) { 
    tempi=0; tempj=0;
    for (int k=0;k<nbannot;k++) {
	if (tabannot[it][k]==c1+1) tempi++;
	else
	  if (tabannot[it][k]==c2+1) tempj++;
    }
    if (Tbnobs[it]>1)
      res+=(tempi*tempj*1.0)/(Tbnobs[it]-1);
  }
  return res;
}

//calcul du tableau des coincidences
void coincidences(int tabannot[MAXIT][MAXA],int nbit,int nba,int nbc,int Tbnobs[],
		  float C[MAXCL][MAXCL]) {
  for (int i=0;i<nbc;i++) {
    for (int j=0;j<nbc;j++) 
      if (i==j)
	C[i][j]=calcul1(tabannot,nbit,nba,i,Tbnobs);
      else
	C[i][j]=calcul2(tabannot,nbit,nba,i,j,Tbnobs);
  }
}


//DISTANCES
float delta_car(int i,int j) {
  return (i-j)*(i-j);
}
float delta_euc(int i,int j) {
  return sqrt((i-j)*(i-j));
}
float delta(int i,int j) {
  if (i==j) return 0; else return 1;
} 


void affiche_coincidences(float C[MAXCL][MAXCL],int nbclasses) {
  int i,j;
  float sum=0;
  for (i=0;i<nbclasses;i++) {
    for (j=0;j<nbclasses;j++) {
      printf("%7.2f  ",C[i][j]);
      sum+=C[i][j];
    }
    cout << endl;
  }
  printf("somme=%f\n",sum);
}

float numalpha(int TAnParClasseParItem[MAXCL][MAXIT],int TSomparCol[],int nbit,int nbc){
  float res=0;
  for (int it=0;it<nbit;it++) {
    float sum=0;
    for (int c=0;c<nbc;c++)
      for (int k=c+1;k<nbc;k++)
	sum+=1.0*TAnParClasseParItem[c][it]*TAnParClasseParItem[k][it]*delta_euc(c,k);
    if (TSomparCol[it]>1)
      res+=sum/(TSomparCol[it]-1);
  }
  return res;
}


float denomalpha(int TSomparLigne[],int nbc){
  float res=0;
  for (int c=0;c<nbc;c++)
    for (int k=c+1;k<nbc;k++)
      res+=1.0*TSomparLigne[c]*TSomparLigne[k]*delta_euc(c,k);
  return res;
}

// avec un tableau de distances
float numalphadist(int TAnParClasseParItem[MAXCL][MAXIT],int TSomparCol[],int nbit,
	       int nbc, float Tdist[MAXCL][MAXCL]){
  float res=0;
  for (int it=0;it<nbit;it++) {
    float sum=0;
    for (int c=0;c<nbc;c++)
      for (int k=c+1;k<nbc;k++)
	sum+=1.0*TAnParClasseParItem[c][it]*TAnParClasseParItem[k][it]*Tdist[c][k];
    if (TSomparCol[it]>1)
      res+=sum/(TSomparCol[it]-1);
  }
  return res;
}


float denomalphadist(int TSomparLigne[],int nbc,float Tdist[MAXCL][MAXCL]){
  float res=0;
  for (int c=0;c<nbc;c++)
    for (int k=c+1;k<nbc;k++)
      res+=1.0*TSomparLigne[c]*TSomparLigne[k]*Tdist[c][k];
  return res;
}


float alpha(int tabannot[MAXIT][MAXA],int nbit,int nba,int nbc){
  float TC[MAXCL][MAXCL];
  int TAnParClasseParItem[MAXCL][MAXIT];
  int TSomLigne[MAXCL],TSommeCol[MAXIT],annotutiles=0;
  TabnbAnnotParClasseParItem(tabannot,nbit,nba,nbc,TAnParClasseParItem,TSomLigne,
			     TSommeCol,annotutiles);
  coincidences(tabannot,nbit,nba,nbc,TSommeCol,TC);
  //affiche_coincidences(TC,nbc);
  float num=numalpha(TAnParClasseParItem,TSommeCol,nbit,nbc);
  float deno=denomalpha(TSomLigne,nbc);
  // printf("num=%f\n",num);
  //printf("deno=%f\n",deno);
  float res=1-(annotutiles-1)*num/deno;
  //printf("alpha=%f\n",res);
  return res;
}

float alphadist(int tabannot[MAXIT][MAXA],int nbit,int nba,
		int nbc,float Tdist[MAXCL][MAXCL]){
  float TC[MAXCL][MAXCL];
  int TAnParClasseParItem[MAXCL][MAXIT];
  int TSomLigne[MAXCL],TSommeCol[MAXIT],annotutiles=0;
  TabnbAnnotParClasseParItem(tabannot,nbit,nba,nbc,TAnParClasseParItem,TSomLigne,
			     TSommeCol,annotutiles);
  coincidences(tabannot,nbit,nba,nbc,TSommeCol,TC);
  //affiche_coincidences(TC,nbc);
  float num=numalphadist(TAnParClasseParItem,TSommeCol,nbit,nbc,Tdist);
  float deno=denomalphadist(TSomLigne,nbc,Tdist);
  //printf("num=%f\n",num);
  //printf("deno=%f\n",deno);
  float res=1-(annotutiles-1)*num/deno;
  //printf("alpha=%f\n",res);
  return res;
}

    
/*
//pour vérification sur exemple Krippendorff
int main() {
  int tabannot[MAXIT][MAXA]={{1,1,0,1},{2,2,3,2},{3,3,3,3},{3,3,3,3},{2,2,2,2},{1,2,3,4},{4,4,4,4},{1,1,2,1},{2,2,2,2},{-1,5,5,5},{0,0,1,1},{0,0,3,0}};
  int nbit=12,nba=4,nbc=5;
  float C[MAXCL][MAXCL];
  int TAnParClasseParItem[MAXCL][MAXIT],TSommeLigne[MAXCL],TSommeCol[MAXIT],annotutiles;
  TabnbAnnotParClasseParItem(tabannot,nbit,nba,nbc,TAnParClasseParItem,TSommeLigne,
			     TSommeCol,annotutiles);
  coincidences(tabannot,nbit,nba,nbc,TSommeCol,C);
  affiche_coincidences(C,nbc);
  printf("alpha=%f\n",alpha(tabannot,nbit,nba,nbc));
  return 0;
  }*/








