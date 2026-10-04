#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// Prototipado de funciones

void registrar_Tiendas(string *nombres, int numTiendas);
void registrar_Ventas(string *nombres, float **ventas, int numTiendas,
                      int numDias);

void ver_Ventas(string *nombres, float **ventas, int numTiendas, int numDias);

void ver_Promedio(string *nombres, float **ventas, int numTiendas, int numDias);
void ver_Mayor(string *nombres, float **ventas, int numTiendas, int numDias);
void ver_Menor(string *nombres, float **ventas, int numTiendas, int numDias);

void guardar_Datos(string *nombres, float **ventas, int numTiendas,
                   int numDias);

void limpiar_Pantalla();

int main() {
  // Configuracion global de manipuladores de flujo para 2 decimales
  cout << fixed << setprecision(2);

  int numTiendas = 0;
  int numDias = 0;
  int opcion;

  do {
    cout << "=== SPEEDYBITE DELIVERY - GESTION MULTITIENDA ===" << endl;
    cout << "Ingrese la cantidad de tiendas: ";
    cin >> numTiendas;
  } while (numTiendas <= 0);

  do {
    cout << "Ingrese la cantidad de dias a evaluar: ";
    cin >> numDias;
  } while (numDias <= 0);

  string *nombres = new string[numTiendas];

  float **ventas = new float *[numTiendas];

  for (int i = 0; i < numTiendas; i++) {
    ventas[i] = new float[numDias];
  }

  registrar_Tiendas(nombres, numTiendas);
  registrar_Ventas(nombres, ventas, numTiendas, numDias);
  guardar_Datos(nombres, ventas, numTiendas, numDias);

  limpiar_Pantalla();

  do {
    cout << "\n========= Menu ===============\n\n";
    cout << "1. Mostrar tiendas por venta\n";
    cout << "2. Mostrar promedio de ventas\n";
    cout << "3. Mostrar la tienda mas rentable\n";
    cout << "4. Mostrar la tienda menor venta\n";
    cout << "5. Guardar datos\n";
    cout << "6. Salir\n";
    cout << "Ingrese la opcion: ";
    cin >> opcion;

    switch (opcion) {
    case 1:
      ver_Ventas(nombres, ventas, numTiendas, numDias);
      break;

    case 2:
      ver_Promedio(nombres, ventas, numTiendas, numDias);
      break;

    case 3:
      ver_Mayor(nombres, ventas, numTiendas, numDias);
      break;

    case 4:
      ver_Menor(nombres, ventas, numTiendas, numDias);
      break;

    case 5:
      guardar_Datos(nombres, ventas, numTiendas, numDias);
      cout << "\nDatos guardados correctamente en ventas.txt\n";
      break;

    case 6:
      cout << "\nSaliendo del programa\n";
      break;

    default:
      cout << "\nOpcion invalida.\n";
      cout << "Ingrese una opcion valida por favor.\n";
    }

  } while (opcion != 6);

  for (int i = 0; i < numTiendas; i++) {
    delete[] ventas[i];
  }

  delete[] ventas;
  delete[] nombres;

  return 0;
}

void limpiar_Pantalla() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void registrar_Tiendas(string *nombres, int numTiendas) {
  cin.ignore();

  for (int i = 0; i < numTiendas; i++) {
    cout << "Ingrese el nombre de la tienda " << i + 1 << ": ";
    getline(cin, nombres[i]);
  }
}

void registrar_Ventas(string *nombres, float **ventas, int numTiendas,
                      int numDias) {
  for (int i = 0; i < numTiendas; i++) {
    cout << "\nTienda: " << nombres[i] << "\n";

    for (int j = 0; j < numDias; j++) {
      cout << "Ingrese la venta del dia " << j + 1 << ": ";
      cin >> ventas[i][j];
    }
  }
}

void ver_Ventas(string *nombres, float **ventas, int numTiendas, int numDias) {
  cout << "\n=== Ventas por tiendas ====\n";
  cout << "Tienda\t\tTotal\n";
  cout << "---------------------------\n";

  for (int i = 0; i < numTiendas; i++) {
    float total = 0;

    for (int j = 0; j < numDias; j++) {
      total = total + ventas[i][j];
    }

    cout << nombres[i] << "\t\tS/ " << total << "\n";
  }

  cout << "===========================\n";
}

void ver_Promedio(string *nombres, float **ventas, int numTiendas,
                  int numDias) {
  cout << "\n=== Promedio de venta(s) ===\n";
  cout << "Tienda\t\tPromedio\n";
  cout << "---------------------------\n";

  for (int i = 0; i < numTiendas; i++) {
    float total = 0;

    for (int j = 0; j < numDias; j++) {
      total = total + ventas[i][j];
    }

    float promedio = total / numDias;

    cout << nombres[i] << "\t\tS/ " << promedio << "\n";
  }

  cout << "===========================\n";
}

void ver_Mayor(string *nombres, float **ventas, int numTiendas, int numDias) {
  float mayor = 0;
  int posicion = 0;

  for (int i = 0; i < numTiendas; i++) {
    float total = 0;

    for (int j = 0; j < numDias; j++) {
      total = total + ventas[i][j];
    }

    if (i == 0 || total > mayor) {
      mayor = total;
      posicion = i;
    }
  }

  cout << "\n=== La tienda mas rentable ===\n";
  cout << "Tienda\t\t: " << nombres[posicion] << "\n";
  cout << "Venta total\t: S/ " << mayor << "\n";
  cout << "===============================\n";
}

void ver_Menor(string *nombres, float **ventas, int numTiendas, int numDias) {
  float menor = 0;
  int posicion = 0;

  for (int i = 0; i < numTiendas; i++) {
    float total = 0;

    for (int j = 0; j < numDias; j++) {
      total = total + ventas[i][j];
    }

    if (i == 0 || total < menor) {
      menor = total;
      posicion = i;
    }
  }

  cout << "\n=== Menor venta ===\n";
  cout << "Tienda\t\t: " << nombres[posicion] << "\n";
  cout << "Venta total\t: S/ " << menor << "\n";
  cout << "===============================\n";
}

void guardar_Datos(string *nombres, float **ventas, int numTiendas,
                   int numDias) {
  ofstream archivo("ventas.txt", ios::app);

  archivo << fixed << setprecision(2);

  for (int i = 0; i < numTiendas; i++) {
    float total = 0;

    archivo << "Tienda: " << nombres[i] << "\n";

    for (int j = 0; j < numDias; j++) {
      archivo << "Dia " << j + 1 << "\t: S/ " << ventas[i][j] << "\n";
      total = total + ventas[i][j];
    }

    archivo << "Total\t: S/ " << total << "\n";
    archivo << "Promedio: S/ " << total / numDias << "\n\n";
  }

  archivo.close();
}
