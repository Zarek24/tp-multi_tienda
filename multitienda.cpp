#include <iostream>
using namespace std;

// Prototipado de funciones

// pp

int main() {

  int numTiendas = 0, numDias = 0;
  cout << "=== SPEEDYBITE DELIVERY - GESTION MULTITIENDA ===" << endl;
  cout << "Ingrese la cantidad de tiendas: ";
  cin >> numTiendas;
  cout << "Ingrese la cantidad de dias a evaluar: ";
  cin >> numDias;

  string *nombres = new string[numTiendas];
  float **ventas = new float *[numTiendas];

  return 0;
}