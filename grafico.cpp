//Risonanza corda vista in laboratorio

#include "EquazioniDifferenziali.h"

#include <fstream>
//#include <string> // per stod

#include "gplot++.h"
//preso da https://tinyurl.com/yyoeskq7

using namespace std;

int sign(double x){ return x == 0. ? 0 : (x > 0 ? 1 : -1) ;}

int main(int argc, const char *argv[]) { 

  vector<double> omega;
  vector<double> A;

  string filename = "dati_1_destra.txt";
  ifstream in(filename);

  cout << "Prendo i dati da " << filename << endl;

  while(in){
    string temp;

    getline(in, temp, ',');
    if(!in) break;
    omega.push_back(stod(temp));

    getline(in, temp, '\n');
    A.push_back(stod(temp));

    
  }
  
  filename = "dati_1_sinistra.txt";
  ifstream in2(filename);

  cout << "Prendo i dati da " << filename << endl;

  while(in2){
    string temp;

    getline(in2, temp, ',');
    if(!in2) break;
    omega.push_back(stod(temp));

    getline(in2, temp, '\n');
    A.push_back(stod(temp));

    
  }

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
