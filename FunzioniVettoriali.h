#pragma once

#include "VectorOperations.h"
#include <cmath>
#include <vector>

//////////////////////////////////////////////////////////////
// Classe astratta, Restituisce la derivata nel punto (t, x)

class FunzioneVettorialeBase {
public:
  virtual std::vector<double> Eval(double t,
                                   const std::vector<double> &x) const = 0;
};

//////////////////////////////////////////////////////////////
// Classe concreta, corda con forzante

class corda_forzante : public FunzioneVettorialeBase {
public:
  corda_forzante(double omega0, double alpha, double omega, double A) : m_omega0{omega0}, m_alpha{alpha}, m_omega{omega}, m_A{A} {}
  std::vector<double> Eval(double t,
                           const std::vector<double> &a) const override {
    std::vector<double> derivata{ a[1], -m_omega0 * m_omega0 * a[0] - m_alpha *a[1] + m_A*sin(m_omega * t)};
    //myVectUtils::Print(derivata);
    return derivata;
  }

private:
  double m_omega0;
  double m_alpha;
  double m_omega;
  double m_A;
};

