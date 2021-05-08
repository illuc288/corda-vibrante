#pragma once

#include "VectorOperations.h"
#include "FunzioniVettoriali.h"
#include <cmath>
#include <vector>

//////////////////////////////////////////////////////////////
// Classe astratta per un integratore di equazioni differenziali

class EquazioneDifferenzialeBase {
public:
  virtual std::vector<double> Passo(double t, const std::vector<double> &x,
                                    double h,
                                    FunzioneVettorialeBase *f) const = 0;
};

//////////////////////////////////////////////////////////////
// Classe concreta, metodo di Eulero

class Eulero : public EquazioneDifferenzialeBase {
public:
  std::vector<double> Passo(double t, const std::vector<double> &x, double h,
                            FunzioneVettorialeBase *f) const override {
      std::vector<double> x_succ = x + f->Eval(t, x) * h;
      return x_succ;
  } 
};

//////////////////////////////////////////////////////////////
// Classe concreta, metodo di Runge-Kutta

class RungeKutta : public EquazioneDifferenzialeBase {
public:
  std::vector<double> Passo(double t, const std::vector<double> &x, double h,
                            FunzioneVettorialeBase *f) const override {
      // da implementare
      std::vector<double> k1 = f->Eval(t     , x);
      std::vector<double> k2 = f->Eval(t+h/2., x + h*k1/2.);
      std::vector<double> k3 = f->Eval(t+h/2., x + h*k2/2.);
      std::vector<double> k4 = f->Eval(t+h   , x + h*k3);    
      std::vector<double> x_succ = x + (k1 + 2.* k2 + 2.*k3 + k4) *h/6.;
      return x_succ;
  }
};

void test_euler();
void test_runge_kutta();
