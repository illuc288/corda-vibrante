#pragma once

#define G 6.6742e-11
#define M_SOLE 1.98844e30
#define R0 147098074e3
#define V0 30.287e3
//tutte in SI

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
// Classe concreta, usata nei test

class OscillatoreArmonico : public FunzioneVettorialeBase {
public:
  OscillatoreArmonico(double omega0) : m_omega0{omega0} {}
  std::vector<double> Eval(double t,
                           const std::vector<double> &a) const override {
    std::vector<double> derivata{ a[1], -m_omega0 * m_omega0 * a[0]};
    //myVectUtils::Print(derivata);
    return derivata;
  }

private:
  double m_omega0;
};

//////////////////////////////////////////////////////////////
// Classe concreta, oscillatore armonico smorzato con forzante

class OAforzante : public FunzioneVettorialeBase {
public:
  OAforzante(double omega0, double alpha, double omega) : m_omega0{omega0}, m_alpha{alpha}, m_omega{omega} {}
  std::vector<double> Eval(double t,
                           const std::vector<double> &a) const override {
    std::vector<double> derivata{ a[1], -m_omega0 * m_omega0 * a[0] - m_alpha *a[1] + sin(m_omega * t)};
    //myVectUtils::Print(derivata);
    return derivata;
  }

private:
  double m_omega0;
  double m_alpha;
  double m_omega;
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

//////////////////////////////////////////////////////////////
// Classe concreta, pendolo

class pendolo : public FunzioneVettorialeBase {
public:
  pendolo(double l) : m_l{l} {}
  std::vector<double> Eval(double t,
                           const std::vector<double> &a) const override {
    std::vector<double> derivata{ a[1], -9.8/m_l * sin( a[0] )};
    //myVectUtils::Print(derivata);
    return derivata;
  }

private:
  double m_l;
};

//////////////////////////////////////////////////////////////
// Classe concreta, forza gravitazionale

class Gravitazione : public FunzioneVettorialeBase {
public:
  Gravitazione(double m): m_m{m} {}
  std::vector<double> Eval(double t,
                           const std::vector<double> &a) const override {
    // a = {x, y, vx, vy}
    // derivata = {vx, vy, ax, ay}
    // ax = c * x;
    double r = sqrt(a[0]*a[0] + a[1]*a[1]);
    double c = -G * M_SOLE / (r*r*r);
    std::vector<double> derivata{ a[2], a[3], c*a[0], c*a[1]};
    //myVectUtils::Print(derivata);
    return derivata;
  }

private:
  double m_m;
};

//////////////////////////////////////////////////////////////
// Classe concreta, forza elettrica e magnetica

class campoEM : public FunzioneVettorialeBase {
public:
  campoEM(double m, double q, std::vector<double> E, std::vector<double> B): m_m{m}, m_q{q}, m_E{E}, m_B{B} {}
  std::vector<double> Eval(double t,
                           const std::vector<double> &a) const override {
    // a = {x, y, z, vx, vy, vz}
    // derivata = {vx, vy, vz, ax, ay, az}
    // B[0] = B_x
    // a[3+0] = vx
    // ax = c * x; no più complicato
    //double r = sqrt(a[0]*a[0] + a[1]*a[1]);
    double c = m_q / m_m;
    std::vector<double> derivata1{a[3], 
                                  a[4], 
                                  a[5],
                                                     c*m_B[2]*a[3+1] - c*m_B[1]*a[3+2], 
                                  -c*m_B[2]*a[3+0]                   + c*m_B[0]*a[3+2],
                                   c*m_B[1]*a[3+0] - c*m_B[0]*a[3+1]
                                  };
    std::vector<double> derivata2{0,
                                  0,
                                  0,
                                  c*m_E[0],
                                  c*m_E[1],
                                  c*m_E[2]
                                  };
                                  
    //myVectUtils::Print(derivata1 + derivata2);
    
    return derivata1 + derivata2;
  }

private:
  double m_m;
  double m_q;
  std::vector<double> m_E;
  std::vector<double> m_B;
};
