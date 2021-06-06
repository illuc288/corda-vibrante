//Risonanza corda vista in laboratorio

#include "EquazioniDifferenziali.h"

#include <fstream>
#include <sstream>
#include <cstring>

#include "gplot++.h"
//preso da https://tinyurl.com/yyoeskq7

enum modalita{destra, sinistra};

using namespace std;

int sign(double x){ return x == 0. ? 0 : (x > 0 ? 1 : -1) ;}

int main(int argc, const char *argv[]) { 

  //FWHM freq 0.3, FWHM omega 0.3*2pi, Gamma = FHWMo / sqrt 3

  // dati della corda del mio gruppo, alpha solo indicativa
  // double alpha = 1.4; //per primo modo
  // double alpha = 2.87; //per secondo modo
  double alpha = stod(argv[2]);  //pensavo grafico indipendente da questo, ma non sembra. All'aumentare di questo il grafico diventa più regolare
  // primi graifici ottenuti con alpha = gamma = 3, ma quello osservato è molto minore! quindi effetto strano ancora più accentuato?
  // double A_forzante = 2.12;  // per primo modo
  double A_forzante = stod(argv[3]);  // per secondo modo
  double l0 = 0.6207;
  double lr = 0.587;
  double k  = 75.7; 
  double m  = 0.0134;
  double T0 = k*(l0-lr);
  double rho= m/l0;
  double v  = sqrt(T0/rho);
  int modo = 2;
  double lambda = 2*l0 / modo;
  double omega_0= 2*M_PI*v / lambda; //omega propria iniziale
  
  cout << "v e omega max iniziali: " << v << "  " << omega_0 << endl;

  RungeKutta rk;
  
  
  const double lastt = 10. /alpha;
  
  
  double h = 0.001;  // passo di integrazione
  
  // frequenza della forzante
  vector<double> f;
  
  // ampiezza oscillazione stabile
  vector<double> A;
 
  // salvo i valori trovati
  cout << "Dove vuoi salvare i dati? (nome file senza estensione)" << endl;
  string filename;
  cin >> filename;
  getchar(); // per far funzionare il getchar dopo
  filename = filename + ".txt";
  ofstream out(filename);
  
  cout << "Calcolo ampiezza oscillazione stabile al variare della pulsazione omega della forzante" << endl;
  cout << "I dati sono salvati in " << filename << endl;
  

  modalita mod = sinistra;
  if( strcmp(argv[1], "d") == 0 )
    mod = destra;

  double larghezza_curva = 3 ;

  double passo = 0.01;
  int it=0;
  double omega_p = omega_0;  // pulsazione sarà ricalcolata nel ciclo, poiché dipende dall'ampiezza
  double min = omega_0-larghezza_curva;
  double max = omega_0+larghezza_curva*3;
  // double min = 8.7*2*M_PI;
  // double max = 9.2*2*M_PI;
  double iniziale = min;
  if(mod == destra){
    iniziale = max;
    passo = -passo;
  }

  for(double om = iniziale; om >= min && om <= max; om += passo, it++ ){ // da sinistra
  //for(double om = ; om >= omega_0-larghezza_curva; om -= passo, it++ ){   // da destra 
    
    // cout << "f_max: " << omega_p/(2*M_PI) << endl;

    corda_forzante oa{omega_p, alpha, om, A_forzante};
    vector<double> pos{0., 0.};
    double A_temp;
    
    for (double t{}; t <= lastt; t += h) {
      vector<double> last_pos = pos;
      pos = rk.Passo(t, pos, h, &oa);
      
      // salvo l'ampiezza ogni volta che la velocità cambia segno
      if( sign (pos[1] ) * sign( last_pos[1]) < 0 ) A_temp = abs(pos[0]); 
    }

    double l = sqrt(  pow( l0/2, 2)  +  A_temp*A_temp) * 2;
    double T = k*(l-lr);
    rho =  m/l;
    v   =  sqrt(T/rho);
    double omega_old = omega_p;
    omega_p = 2*M_PI*v / lambda;
    
    // alle prime iterazioni basta la formula sopra e si stabilizza da solo
    // La riga sotto invece fa la media tra l'ampiezza calcolata in questo ciclo e il precedente.
    // Senza quella l'ampiezza della lorentziana è molto instabile
    if(it>100 )
      omega_p = (omega_p + omega_old) /2.;

    //cout << "v e omega max aggiornati: " << v << "  " << omega_p << endl;

    //salvo l'ampiezza dell'oscillazione stabile per grafico Lorentziana
    A.push_back(A_temp);
    double freq = om/(2.*M_PI);
    f.push_back( freq );

    //salvo i dati nel file di output
    if(passo < 0 || it > 100)   //da destra, passo > 0, non salvo i primi dati perché molto variabili. Da sinistra, passo <0, no problem.
    out << freq << ", " << A_temp << endl ;
    
    //stampo il progresso attuale
    if ( int(round(om * 100 )) % 100 == 0)  
      cout << "Omega = " << om <<". Estremi: " << omega_0-larghezza_curva << " " << omega_0+larghezza_curva*4 << endl;
    
  }
  
  
  cout << "Mostro il grafico dell'ampiezza in funzione di omega " << endl;
  
  // mostro il grafico
  Gnuplot plt{};
  plt.plot(f, A);
  plt.set_ylabel("Ampiezza[m]");
  plt.set_xlabel("Frequenza[Hz]");
  plt.show(); 
  

  cout << "Premere enter per salvare il grafico, altrimenti terminare il programma" << endl;
  getchar();


  // salvo il grafico
  ostringstream s;
  s << alpha; 

  string imgname = "Curva_alpha_" + s.str();
  s.str("");
  s << A_forzante; 
  imgname += "_forz_" + s.str() + ".png";
  Gnuplot output_plt{};
  output_plt.redirect_to_png(imgname);
  output_plt.plot(f, A);
  output_plt.set_ylabel("Ampiezza[m]");
  output_plt.set_xlabel("Frequenza[Hz]");
  output_plt.show();

  cout << "Grafico salvato in " << imgname << endl;
}
