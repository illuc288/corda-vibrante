//Risonanza corda vista in laboratorio

#include "EquazioniDifferenziali.h"

#include <fstream>
//#include <string> // per stod

#include "gplot++.h"
//preso da https://tinyurl.com/yyoeskq7

using namespace std;

int sign(double x){ return x == 0. ? 0 : (x > 0 ? 1 : -1) ;}

void getData(string filename, vector<double> &om, vector<double> &a){

  ifstream in(filename);

  if(in)
    cout << "Prendo i dati da " << filename << endl;
  else {
    cerr << "Errore: impossibile leggere il file" << endl;
    exit(1);
  }

  while(in){
    string temp;

    getline(in, temp, ',');
    if(!in) break;
    om.push_back(stod(temp));

    getline(in, temp, '\n');
    a.push_back(stod(temp));
  }

}

int main(int argc, const char *argv[]) { 

  vector<double> omega;
  vector<double> A;

  int numfile;
  cout << "Da quanti file vuoi prendere dati?" << endl;
  cin >> numfile;

  for(int n = 0 ; n < numfile; n++){
    string filename;
    cout << "Enter the name of file without extension "<< n+1<< endl; 
    cin >> filename;
    filename += ".txt";
    getData(filename, omega, A);
  }
  
  // string filename = "dati_1_destra.txt";
  // getData(filename, omega, A);
  
  // filename = "dati_1_sinistra.txt";
  // getData(filename, omega, A);
  
  

  // mostro il grafico
  Gnuplot plt{};
  plt.plot(omega, A);
  plt.set_ylabel("Ampiezza[m]");
  plt.set_xlabel("Pulsazione[rad/s]");
  plt.show(); 
  

  cout << "Premere enter per salvare il grafico, altrimenti terminare il programma" << endl;
  getchar();


  // salvo il grafico
  Gnuplot output_plt{};
  output_plt.redirect_to_png("corda_ampiezza.png");
  output_plt.plot(omega, A);
  output_plt.set_ylabel("Ampiezza[m]");
  output_plt.set_xlabel("Pulsazione[rad/s]");
  output_plt.show();

  cout << "Grafico salvato in corda_ampiezza.png" << endl;
}
