//Risonanza corda vista in laboratorio

#include "EquazioniDifferenziali.h"

#include <fstream>

#include "gplot++.h"
//preso da https://tinyurl.com/yyoeskq7

using namespace std;

int sign(double x){ return x == 0. ? 0 : (x > 0 ? 1 : -1) ;}

void plot( EquazioneDifferenzialeBase * rk, FunzioneVettorialeBase * oa, const double lastt_graph){
  
  double h = 0.01; //per oa funziona stra bene già con 0.1, per oasmorzato meglio 0.01
  vector<double> pos{0., 0.};
  vector<double> x{pos[0]};
  
  vector<double> ts{0.};
  for (double t{}; t <= lastt_graph; t += h) {
    vector<double> last_pos = pos;
    pos = rk -> Passo(t, pos, h, oa);
    
    ts.push_back(t);
    x.push_back( pos[0] );

  }

  
  Gnuplot plt{};
  plt.plot(ts, x);
  plt.set_ylabel("x[m]");
  plt.set_xlabel("Tempo[s]");
  plt.show(); 
}

int main(int argc, const char *argv[]) { 

  double alpha = 3.0;
  double l0 = 0.626;
  double lr = 0.587;
  double k  = 75.7; 
  double m  = 0.0134;
  double T0 = k*(l0-lr);
  double rho= m/l0;
  double v  = sqrt(T0/rho);
  double lambda = 2*l0;
  double omega_0= 2*M_PI*v / lambda; //omega propria iniziale
  
  cout << "v e omega max iniziali: " << v << "  " << omega_0 << endl;

  RungeKutta rk;
  
  
  const double lastt = 10. /alpha;
  
  
  double h = 0.001; //per oa funziona molto bene già con 0.1, per oasmorzato meglio 0.01
  // per corda grafico viene meglio con 0.001 ma sembra funzionare anche con 0.01
  
  //omega della forzante
  vector<double> omega;
  
  //ampiezza oscillazione stabile
  vector<double> A;
 
  //salvo i valori trovati
  ofstream out("corda_ampiezza.txt");
  
  cout << "Calcolo ampiezza oscillazione stabile al variare della pulsazione omega della forzante" << endl;
  cout << "I dati sono salvati in corda_ampiezza.txt" << endl;
  
  double larghezza_curva = 2 ;
  double passo = 0.01;
  int it=0;
  double omega_p = omega_0;
  //for(double om = omega_0-larghezza_curva; om <= omega_0+larghezza_curva*4; om += passo, it++ ){ // da sinistra
  for(double om = omega_0+larghezza_curva*4; om >= omega_0-larghezza_curva; om -= passo, it++ ){   // da destra 
    
    // sembra che sia solo da cambiare la omega, in modo che sia dipendente dal'ampiezza
    
    corda_forzante oa{omega_p, alpha, om, 10.};
    vector<double> pos{0., 0.};
    double A_temp;
    
    vector<double> allpos;
    vector<double> allt;
    for (double t{}; t <= lastt; t += h) {
      vector<double> last_pos = pos;
      pos = rk.Passo(t, pos, h, &oa);
      
      allpos.push_back(pos[0]);
      allt.push_back(t);
      // salvo l'ampiezza ogni volta che la velocità cambia segno
      if( sign (pos[1] ) * sign( last_pos[1]) < 0 ) A_temp = abs(pos[0]); 
    }
    //Gnuplot plt{};
    //plt.plot(allt, allpos);
    //plt.show(); 
    //getchar();
    

    double l = sqrt(  pow( l0/2, 2)  +  A_temp*A_temp) * 2;
    double T = k*(l-lr);
    rho =  m/l;
    v   =  sqrt(T/rho);
    double omega_old = omega_p;
    omega_p = 2*M_PI*v / lambda;
    // double omega_1, omega_2;
    // if(it==0)
    //   omega_p = 2*M_PI*v / lambda;
    // else if(it % 2 == 1)
    //   omega_1 = 2*M_PI*v / lambda;
    // else if(it % 2 == 0)
    //   omega_2 = 2*M_PI*v / lambda;

    if(it>100 )
      omega_p = (omega_p + omega_old) /2.;

    cout << "v e omega max aggiornati: " << v << "  " << omega_p << endl;

    //all'uscita dal ciclo avrò l'ampiezza dell'oscillazione stabile
    A.push_back(A_temp);
    omega.push_back(om);
    out << om << ", " << A_temp << endl;
    
    if ( int(round(om * 100 )) % 10 == 0)  
      cout << "Omega = " << om <<" su " << omega_p+larghezza_curva << endl;
    
  }
  
  
  cout << "Mostro il grafico dell'ampiezza in funzione di omega " << endl;
  
  // mostro il grafico
  Gnuplot plt{};
  plt.plot(omega, A);
  plt.set_ylabel("Ampiezza[un. arb.]");
  plt.set_xlabel("Pulsazione[rad/s]");
  plt.show(); 
  
  getchar();

  // salvo il grafico
  Gnuplot output_plt{};
  output_plt.redirect_to_png("OAforzante_ampiezza2.png");
  output_plt.plot(omega, A);
  output_plt.set_ylabel("Ampiezza[un. arb.]");
  output_plt.set_xlabel("Pulsazione[rad/s]");
  output_plt.show();
}
