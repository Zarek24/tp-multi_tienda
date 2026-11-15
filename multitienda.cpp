#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <stdexcept>
#include <cstdio>

using namespace std;

// Lectura por linea: rechaza texto sobrante, negativos y valores no finitos.
// EOF termina el programa de forma controlada.
struct FinEntrada {};
template<class T> void leerNumero(T& valor, T minimo, T maximo) {
    string linea;
    while (true) {
        if (!getline(cin, linea)) throw FinEntrada{};
        istringstream entrada(linea);
        T candidato;
        if ((entrada >> candidato) && (entrada >> ws).eof()
            && isfinite(static_cast<double>(candidato))
            && candidato >= minimo && candidato <= maximo) {
            valor = candidato;
            return;
        }
        cout << "[Error] Ingrese un numero entre " << minimo << " y " << maximo << ": ";
    }
}
void leerTexto(string& valor) {
    while (true) {
        if (!getline(cin, valor)) throw FinEntrada{};
        if (valor.find_first_not_of(" \t\r") != string::npos) return;
        cout << "[Error] El texto no puede estar vacio: ";
    }
}

// ==================== ESTRUCTURAS ====================
struct Tienda {
    string codigo;
    string nombre;
    string direccion;
    string distrito;
    string encargado;
    bool estado; // true = Activa, false = Inactiva
};

struct Producto {
    string codigo;
    string nombre;
    double precio;
    int stock;
};

struct Empleado {
    string codigo;
    string nombre;
    string puesto;
    double sueldo;
    string tiendaCodigo;
};

// ==================== PROTOTIPOS DE FUNCIONES ====================
// Gestión de Tiendas
bool existeTienda(Tienda* tiendas, int tam, const string& codigo);
void registrarTienda(Tienda*& tiendas, int& tam);
void mostrarTiendas(Tienda* tiendas, int tam);
void modificarTienda(Tienda* tiendas, int tam);

// Matriz de Ventas
void crearMatrizVentas(double**& matriz, int filasAnt, int colsAnt, int nuevasFilas, int nuevasCols);
void registrarVentas(double** matriz, Tienda* tiendas, int numTiendas, int numDias);
void mostrarMatrizVentas(double** matriz, Tienda* tiendas, int numTiendas, int numDias);

// Gestión de Productos
bool existeProducto(Producto* productos, int tam, const string& codigo);
void registrarProducto(Producto*& productos, int& tam);
void mostrarProductos(Producto* productos, int tam);

// Gestión de Empleados
void registrarEmpleado(Empleado*& empleados, int& tam, Tienda* tiendas, int numTiendas);
void mostrarEmpleados(Empleado* empleados, int tam);

// Cálculos del Sistema
void calcularVentasPorTienda(double** matriz, Tienda* tiendas, int numTiendas, int numDias);
void calcularPromedios(double** matriz, int numTiendas, int numDias);
void ventaMaximaMinima(double** matriz, Tienda* tiendas, int numTiendas, int numDias);
void totalGeneralYParticipacion(double** matriz, Tienda* tiendas, int numTiendas, int numDias);

// Reportes del Sistema
void reportesSistema(double** matriz, Tienda* tiendas, int numTiendas, int numDias);

// Gestión de Archivos
void guardarDatos(Tienda* tiendas, int numTiendas, double** matriz, int numDias, Producto* productos, int numProductos, Empleado* empleados, int numEmpleados);
void cargarDatos(Tienda*& tiendas, int& numTiendas, double**& matriz, int& numDias, Producto*& productos, int& numProductos, Empleado*& empleados, int& numEmpleados);

// ==================== FUNCION PRINCIPAL ====================
int main() {
    cout << fixed << setprecision(2);
    Tienda* tiendas = nullptr;
    int numTiendas = 0;

    Producto* productos = nullptr;
    int numProductos = 0;

    Empleado* empleados = nullptr;
    int numEmpleados = 0;

    double** matrizVentas = nullptr;
    int numDias = 0; // Se inicializa al crear la matriz

    int opcion;

    try {
    do {
        cout << "\n============================================\n";
        cout << "       SISTEMA MULTITIENDA - AVANZADO       \n";
        cout << "============================================\n";
        cout << " 1. Registrar tienda\n";
        cout << " 2. Mostrar tiendas\n";
        cout << " 3. Modificar tienda\n";
        cout << " 4. Crear/Redimensionar Matriz de Ventas y Registrar\n";
        cout << " 5. Mostrar matriz de ventas\n";
        cout << " 6. Registrar producto\n";
        cout << " 7. Mostrar productos\n";
        cout << " 8. Registrar empleado\n";
        cout << " 9. Mostrar empleados\n";
        cout << "10. Calcular ventas por tienda y promedios\n";
        cout << "11. Mostrar venta maxima y minima\n";
        cout << "12. Mostrar total general y participacion\n";
        cout << "13. Reportes del sistema (Recaudacion y Ranking)\n";
        cout << "14. Guardar datos en archivos\n";
        cout << "15. Cargar datos desde archivos\n";
        cout << " 0. Salir\n";
        cout << "Seleccione una opcion: ";
        leerNumero(opcion, 0, 15);

        switch (opcion) {
        case 1:
            {
                int anteriores = numTiendas;
                registrarTienda(tiendas, numTiendas);
                if (matrizVentas && numTiendas != anteriores)
                    crearMatrizVentas(matrizVentas, anteriores, numDias, numTiendas, numDias);
            }
            break;
        case 2:
            mostrarTiendas(tiendas, numTiendas);
            break;
        case 3:
            modificarTienda(tiendas, numTiendas);
            break;
        case 4: {
            if (numTiendas == 0) {
                cout << "[Error] Primero debe registrar al menos una tienda.\n";
                break;
            }
            int nuevosDias;
            cout << "Ingrese el numero de dias a registrar en la matriz: ";
            leerNumero(nuevosDias, 1, 366);
            if (nuevosDias <= 0) {
                cout << "[Error] Numero de dias invalido.\n";
                break;
            }
            crearMatrizVentas(matrizVentas, numTiendas, numDias, numTiendas, nuevosDias);
            numDias = nuevosDias;
            registrarVentas(matrizVentas, tiendas, numTiendas, numDias);
            break;
        }
        case 5:
            mostrarMatrizVentas(matrizVentas, tiendas, numTiendas, numDias);
            break;
        case 6:
            registrarProducto(productos, numProductos);
            break;
        case 7:
            mostrarProductos(productos, numProductos);
            break;
        case 8:
            registrarEmpleado(empleados, numEmpleados, tiendas, numTiendas);
            break;
        case 9:
            mostrarEmpleados(empleados, numEmpleados);
            break;
        case 10:
            if (matrizVentas == nullptr || numDias == 0) {
                cout << "[Aviso] Primero debe inicializar la matriz de ventas y registrar datos.\n";
            }
            else {
                calcularVentasPorTienda(matrizVentas, tiendas, numTiendas, numDias);
                calcularPromedios(matrizVentas, numTiendas, numDias);
            }
            break;
        case 11:
            if (matrizVentas == nullptr || numDias == 0) {
                cout << "[Aviso] Matriz de ventas vacia.\n";
            }
            else {
                ventaMaximaMinima(matrizVentas, tiendas, numTiendas, numDias);
            }
            break;
        case 12:
            if (matrizVentas == nullptr || numDias == 0) {
                cout << "[Aviso] Matriz de ventas vacia.\n";
            }
            else {
                totalGeneralYParticipacion(matrizVentas, tiendas, numTiendas, numDias);
            }
            break;
        case 13:
            if (matrizVentas == nullptr || numDias == 0) {
                cout << "[Aviso] Matriz de ventas vacia.\n";
            }
            else {
                reportesSistema(matrizVentas, tiendas, numTiendas, numDias);
            }
            break;
        case 14:
            guardarDatos(tiendas, numTiendas, matrizVentas, numDias, productos, numProductos, empleados, numEmpleados);
            break;
        case 15:
            cargarDatos(tiendas, numTiendas, matrizVentas, numDias, productos, numProductos, empleados, numEmpleados);
            break;
        case 0:
            cout << "Saliendo del sistema...\n";
            break;
        default:
            cout << "[Error] Opcion invalida. Intente nuevamente.\n";
        }
    } while (opcion != 0);

    } catch (const FinEntrada&) {
        cout << "\nEntrada finalizada. Guarde desde el menu antes de salir para conservar cambios.\n";
    }

    // Liberar memoria principal al salir
    delete[] tiendas;
    delete[] productos;
    delete[] empleados;
    if (matrizVentas != nullptr) {
        for (int i = 0; i < numTiendas; i++) delete[] matrizVentas[i];
        delete[] matrizVentas;
    }

    return 0;
}

// ==================== DESARROLLO DE MÓDULOS ====================

// --- GESTIÓN DE TIENDAS ---
bool existeTienda(Tienda* tiendas, int tam, const string& codigo) {
    for (int i = 0; i < tam; i++) {
        if (tiendas[i].codigo == codigo) return true;
    }
    return false;
}

void registrarTienda(Tienda*& tiendas, int& tam) {
    if (tam >= 1000) { cout << "Limite: 1000 tiendas.\n"; return; }
    Tienda nueva;
    cout << "\n--- REGISTRAR TIENDA ---\n";
    cout << "Codigo: "; leerTexto(nueva.codigo);
    if (existeTienda(tiendas, tam, nueva.codigo)) {
        cout << "[Error] El codigo ya existe.\n";
        return;
    }
    cout << "Nombre: "; leerTexto(nueva.nombre);
    cout << "Direccion: "; leerTexto(nueva.direccion);
    cout << "Distrito: "; leerTexto(nueva.distrito);
    cout << "Encargado: "; leerTexto(nueva.encargado);
    nueva.estado = true;

    Tienda* aux = new Tienda[tam + 1];
    for (int i = 0; i < tam; i++) aux[i] = tiendas[i];
    aux[tam] = nueva;
    delete[] tiendas;
    tiendas = aux;
    tam++;
    cout << "[Exito] Tienda registrada.\n";
}

void mostrarTiendas(Tienda* tiendas, int tam) {
    if (tam == 0) { cout << "No hay tiendas registradas.\n"; return; }
    cout << "\n--- LISTA DE TIENDAS ---\n";
    for (int i = 0; i < tam; i++) {
        cout << "Codigo: " << tiendas[i].codigo << " | Nombre: " << tiendas[i].nombre
            << " | Direccion: " << tiendas[i].direccion << " (" << tiendas[i].distrito
            << ") | Encargado: " << tiendas[i].encargado
            << " | Estado: " << (tiendas[i].estado ? "Activa" : "Inactiva") << "\n";
    }
}

void modificarTienda(Tienda* tiendas, int tam) {
    if (tam == 0) { cout << "No hay tiendas.\n"; return; }
    string cod;
    cout << "Codigo de tienda a modificar: "; leerTexto(cod);
    int idx = -1;
    for (int i = 0; i < tam; i++) {
        if (tiendas[i].codigo == cod) { idx = i; break; }
    }
    if (idx == -1) { cout << "[Error] Tienda no encontrada.\n"; return; }

    int opc;
    cout << "1. Cambiar Encargado\n2. Cambiar Direccion/Distrito\n3. Cambiar Estado\nOpcion: ";
    leerNumero(opc, 1, 3);
    if (opc == 1) {
        cout << "Nuevo encargado: "; getline(cin, tiendas[idx].encargado);
    }
    else if (opc == 2) {
        cout << "Nueva direccion: "; getline(cin, tiendas[idx].direccion);
        cout << "Nuevo distrito: "; getline(cin, tiendas[idx].distrito);
    }
    else if (opc == 3) {
        tiendas[idx].estado = !tiendas[idx].estado;
        cout << "Estado actualizado.\n";
    }
}

// --- MATRIZ DE VENTAS ---
void crearMatrizVentas(double**& matriz, int filasAnt, int colsAnt, int nuevasFilas, int nuevasCols) {
    double** nuevaMatriz = new double* [nuevasFilas];
    for (int i = 0; i < nuevasFilas; i++) {
        nuevaMatriz[i] = new double[nuevasCols];
        for (int j = 0; j < nuevasCols; j++) {
            if (matriz != nullptr && i < filasAnt && j < colsAnt) {
                nuevaMatriz[i][j] = matriz[i][j]; // Mantener datos previos si existían
            }
            else {
                nuevaMatriz[i][j] = 0.0;
            }
        }
    }
    if (matriz != nullptr) {
        for (int i = 0; i < filasAnt; i++) delete[] matriz[i];
        delete[] matriz;
    }
    matriz = nuevaMatriz;
}

void registrarVentas(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    int opc;
    do {
        cout << "\n--- REGISTRAR VENTAS --- \n";
        mostrarTiendas(tiendas, numTiendas);
        int tIndex, dIndex;
        double monto;
        cout << "Indique el indice de la tienda (0 a " << numTiendas - 1 << "): "; leerNumero(tIndex, 0, numTiendas - 1);
        cout << "Indique el dia (1 a " << numDias << "): "; leerNumero(dIndex, 1, numDias);
        cout << "Monto vendido: "; leerNumero(monto, 0.0, 1e12);

        if (tIndex >= 0 && tIndex < numTiendas && dIndex >= 1 && dIndex <= numDias) {
            if (!tiendas[tIndex].estado) {
                cout << "[Error] La tienda esta inactiva.\n";
                continue;
            }
            matriz[tIndex][dIndex - 1] = monto;
            cout << "[Exito] Venta registrada.\n";
        }
        else {
            cout << "[Error] Indices fuera de rango.\n";
        }

        cout << "Desea registrar otra venta? (1: Si / 0: No): "; leerNumero(opc, 0, 1);
    } while (opc == 1);
}

void mostrarMatrizVentas(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    if (matriz == nullptr || numDias == 0) { cout << "Matriz no inicializada.\n"; return; }
    cout << "\n--- MATRIZ DE VENTAS ---\n" << "Tienda\\Dia\t";
    for (int d = 1; d <= numDias; d++) cout << "D" << d << "\t";
    cout << "\n";
    for (int i = 0; i < numTiendas; i++) {
        cout << tiendas[i].codigo << "\t\t";
        for (int j = 0; j < numDias; j++) {
            cout << matriz[i][j] << "\t";
        }
        cout << "\n";
    }
}

// --- GESTIÓN DE PRODUCTOS ---
bool existeProducto(Producto* productos, int tam, const string& codigo) {
    for (int i = 0; i < tam; i++) if (productos[i].codigo == codigo) return true;
    return false;
}

void registrarProducto(Producto*& productos, int& tam) {
    if (tam >= 100000) { cout << "Limite: 100000 productos.\n"; return; }
    Producto p;
    cout << "\n--- REGISTRAR PRODUCTO ---\n";
    cout << "Codigo: "; leerTexto(p.codigo);
    if (existeProducto(productos, tam, p.codigo)) { cout << "[Error] Codigo duplicado.\n"; return; }
    cout << "Nombre: "; leerTexto(p.nombre);
    cout << "Precio: "; leerNumero(p.precio, 0.0, 1e12);
    cout << "Stock: "; leerNumero(p.stock, 0, 1000000000);

    Producto* aux = new Producto[tam + 1];
    for (int i = 0; i < tam; i++) aux[i] = productos[i];
    aux[tam] = p;
    delete[] productos;
    productos = aux;
    tam++;
    cout << "[Exito] Producto registrado.\n";
}

void mostrarProductos(Producto* productos, int tam) {
    if (tam == 0) { cout << "No hay productos.\n"; return; }
    cout << "\n--- LISTA DE PRODUCTOS ---\n";
    for (int i = 0; i < tam; i++) {
        cout << "Cod: " << productos[i].codigo << " | Nombre: " << productos[i].nombre
            << " | Precio: S/ " << productos[i].precio << " | Stock: " << productos[i].stock << "\n";
    }
}

// --- GESTIÓN DE EMPLEADOS ---
void registrarEmpleado(Empleado*& empleados, int& tam, Tienda* tiendas, int numTiendas) {
    if (numTiendas == 0) { cout << "Debe registrar tiendas primero.\n"; return; }
    if (tam >= 100000) { cout << "Limite: 100000 empleados.\n"; return; }
    Empleado e;
    cout << "\n--- REGISTRAR EMPLEADO ---\n";
    cout << "Codigo: "; leerTexto(e.codigo);
    for (int i = 0; i < tam; i++) if (empleados[i].codigo == e.codigo) {
        cout << "[Error] Codigo duplicado.\n"; return;
    }
    cout << "Nombre: "; leerTexto(e.nombre);
    cout << "Puesto: "; leerTexto(e.puesto);
    cout << "Sueldo: "; leerNumero(e.sueldo, 0.0, 1e12);
    mostrarTiendas(tiendas, numTiendas);
    cout << "Codigo de tienda asignada: "; leerTexto(e.tiendaCodigo);

    if (!existeTienda(tiendas, numTiendas, e.tiendaCodigo)) {
        cout << "[Error] La tienda asignada no existe.\n"; return;
    }
    Empleado* aux = new Empleado[tam + 1];
    for (int i = 0; i < tam; i++) aux[i] = empleados[i];
    aux[tam] = e;
    delete[] empleados;
    empleados = aux;
    tam++;
    cout << "[Exito] Empleado registrado.\n";
}

void mostrarEmpleados(Empleado* empleados, int tam) {
    if (tam == 0) { cout << "No hay empleados.\n"; return; }
    cout << "\n--- LISTA DE EMPLEADOS ---\n";
    for (int i = 0; i < tam; i++) {
        cout << "Cod: " << empleados[i].codigo << " | Nombre: " << empleados[i].nombre
            << " | Puesto: " << empleados[i].puesto << " | Sueldo: S/ " << empleados[i].sueldo
            << " | Tienda: " << empleados[i].tiendaCodigo << "\n";
    }
}

// --- CÁLCULOS DEL SISTEMA ---
void calcularVentasPorTienda(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    cout << "\n--- VENTAS TOTALES Y PROMEDIOS POR TIENDA ---\n";
    for (int i = 0; i < numTiendas; i++) {
        double suma = 0;
        for (int j = 0; j < numDias; j++) suma += matriz[i][j];
        cout << tiendas[i].nombre << " (" << tiendas[i].codigo << "): S/ " << suma << " | Promedio diario: S/ " << suma / numDias << "\n";
    }
}

void calcularPromedios(double** matriz, int numTiendas, int numDias) {
    cout << "\n--- PROMEDIOS DE VENTAS ---\n";
    double totalGlobal = 0;


    for (int i = 0; i < numTiendas; i++) {
        for (int j = 0; j < numDias; j++) {
            totalGlobal += matriz[i][j];
        }
    }

    // Calcular el promedio diario considerando el total de días entre todas las tiendas o por día general
    double promedioDiario = (numDias > 0) ? (totalGlobal / numDias) : 0;

    cout << "Promedio Diario General: S/ " << promedioDiario << "\n";
    cout << "Total del periodo: S/ " << totalGlobal << "\n";
}

void ventaMaximaMinima(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    double maxV = matriz[0][0], minV = matriz[0][0];
    int tMax = 0, dMax = 0, tMin = 0, dMin = 0;

    for (int i = 0; i < numTiendas; i++) {
        for (int j = 0; j < numDias; j++) {
            if (matriz[i][j] > maxV) { maxV = matriz[i][j]; tMax = i; dMax = j; }
            if (matriz[i][j] < minV) { minV = matriz[i][j]; tMin = i; dMin = j; }
        }
    }
    cout << "\n--- VENTA MAXIMA Y MINIMA ---\n";
    cout << "Venta Maxima: S/ " << maxV << " en la tienda " << tiendas[tMax].nombre << " el Dia " << (dMax + 1) << "\n";
    cout << "Venta Minima: S/ " << minV << " en la tienda " << tiendas[tMin].nombre << " el Dia " << (dMin + 1) << "\n";
}

void totalGeneralYParticipacion(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    double totalGen = 0;
    double* totalesTienda = new double[numTiendas];

    for (int i = 0; i < numTiendas; i++) {
        totalesTienda[i] = 0;
        for (int j = 0; j < numDias; j++) {
            totalesTienda[i] += matriz[i][j];
        }
        totalGen += totalesTienda[i];
    }

    cout << "\n--- TOTAL GENERAL Y PARTICIPACION ---\n";
    cout << "Total General Vendido: S/ " << totalGen << "\n";
    for (int i = 0; i < numTiendas; i++) {
        double porcentaje = (totalGen > 0) ? (totalesTienda[i] * 100.0 / totalGen) : 0;
        cout << tiendas[i].nombre << ": " << porcentaje << "% de participacion\n";
    }
    delete[] totalesTienda;
}

// --- REPORTES DEL SISTEMA ---
void reportesSistema(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    cout << "\n--- REPORTES ESTADISTICOS ---\n";
    double* totales = new double[numTiendas];
    for (int i = 0; i < numTiendas; i++) {
        totales[i] = 0;
        for (int j = 0; j < numDias; j++) totales[i] += matriz[i][j];
    }

    // Tienda mas rentable y menor venta
    int idxMax = 0, idxMin = 0;
    for (int i = 1; i < numTiendas; i++) {
        if (totales[i] > totales[idxMax]) idxMax = i;
        if (totales[i] < totales[idxMin]) idxMin = i;
    }

    cout << "Tienda con Mayor Recaudacion: " << tiendas[idxMax].nombre << " con S/ " << totales[idxMax] << "\n";
    cout << "Tienda con Menor Recaudacion: " << tiendas[idxMin].nombre << " con S/ " << totales[idxMin] << "\n";

    // Ranking simple (Burbuja)
    int* indices = new int[numTiendas];
    for (int i = 0; i < numTiendas; i++) indices[i] = i;
    for (int i = 0; i < numTiendas - 1; i++) {
        for (int j = 0; j < numTiendas - i - 1; j++) {
            if (totales[indices[j]] < totales[indices[j + 1]]) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }

    cout << "\nRanking de Tiendas (Mayor a Menor Venta):\n";
    for (int i = 0; i < numTiendas; i++) {
        cout << i + 1 << ". " << tiendas[indices[i]].nombre << " - S/ " << totales[indices[i]] << "\n";
    }

    delete[] totales;
    delete[] indices;
}

// --- GESTION DE ARCHIVOS ---
// Un solo archivo versionado evita mezclar tiendas y ventas de sesiones diferentes.
void guardarDatos(Tienda* tiendas, int numTiendas, double** matriz, int numDias, Producto* productos, int numProductos, Empleado* empleados, int numEmpleados) {
    ofstream f("multitienda.tmp");
    f << setprecision(numeric_limits<double>::max_digits10);
    f << "MULTITIENDA_V1\n" << numTiendas << " " << numDias << " " << numProductos << " " << numEmpleados << "\n";
    for (int i = 0; i < numTiendas; i++) {
        const Tienda& t = tiendas[i];
        f << quoted(t.codigo) << " " << quoted(t.nombre) << " " << quoted(t.direccion) << " "
          << quoted(t.distrito) << " " << quoted(t.encargado) << " " << t.estado << "\n";
    }
    for (int i = 0; i < numTiendas; i++) {
        for (int j = 0; j < numDias; j++) f << matriz[i][j] << " ";
        f << "\n";
    }
    for (int i = 0; i < numProductos; i++) {
        const Producto& p = productos[i];
        f << quoted(p.codigo) << " " << quoted(p.nombre) << " " << p.precio << " " << p.stock << "\n";
    }
    for (int i = 0; i < numEmpleados; i++) {
        const Empleado& e = empleados[i];
        f << quoted(e.codigo) << " " << quoted(e.nombre) << " " << quoted(e.puesto) << " "
          << e.sueldo << " " << quoted(e.tiendaCodigo) << "\n";
    }
    f.close();
    if (!f || rename("multitienda.tmp", "multitienda.dat") != 0) {
        cout << "[Error] No se pudo guardar. Se conserva el archivo anterior.\n";
        remove("multitienda.tmp");
        return;
    }
    cout << "[Exito] Datos guardados en multitienda.dat.\n";
}

void cargarDatos(Tienda*& tiendas, int& numTiendas, double**& matriz, int& numDias, Producto*& productos, int& numProductos, Empleado*& empleados, int& numEmpleados) {
    ifstream f("multitienda.dat");
    if (!f) { cout << "[Error] No se pudo abrir multitienda.dat.\n"; return; }
    Tienda* ts = nullptr;
    Producto* ps = nullptr;
    Empleado* es = nullptr;
    double** vs = nullptr;
    int nt = 0, nd = 0, np = 0, ne = 0;
    auto liberar = [&]() {
        if (vs) { for (int i = 0; i < nt; i++) delete[] vs[i]; delete[] vs; }
        delete[] ts; delete[] ps; delete[] es;
    };
    auto textoValido = [](const string& t) { return t.find_first_not_of(" \t\r\n") != string::npos; };
    auto montoValido = [](double v) { return isfinite(v) && v >= 0 && v <= 1e12; };
    try {
        string version;
        if (!(f >> version >> nt >> nd >> np >> ne) || version != "MULTITIENDA_V1"
            || nt < 0 || nt > 1000 || nd < 0 || nd > 366 || (nt == 0 && nd != 0)
            || np < 0 || np > 100000 || ne < 0 || ne > 100000) throw runtime_error("cabecera invalida");
        ts = new Tienda[nt]; ps = new Producto[np]; es = new Empleado[ne];
        vs = new double*[nt]();
        for (int i = 0; i < nt; i++) {
            Tienda& t = ts[i]; int estado;
            if (!(f >> quoted(t.codigo) >> quoted(t.nombre) >> quoted(t.direccion) >> quoted(t.distrito)
                  >> quoted(t.encargado) >> estado) || (estado != 0 && estado != 1)
                || !textoValido(t.codigo) || !textoValido(t.nombre) || !textoValido(t.direccion)
                || !textoValido(t.distrito) || !textoValido(t.encargado) || existeTienda(ts, i, t.codigo))
                throw runtime_error("tienda invalida o duplicada");
            t.estado = estado == 1;
            vs[i] = new double[nd]();
        }
        for (int i = 0; i < nt; i++) for (int j = 0; j < nd; j++)
            if (!(f >> vs[i][j]) || !montoValido(vs[i][j])) throw runtime_error("venta invalida");
        for (int i = 0; i < np; i++) {
            Producto& p = ps[i];
            if (!(f >> quoted(p.codigo) >> quoted(p.nombre) >> p.precio >> p.stock)
                || !textoValido(p.codigo) || !textoValido(p.nombre) || !montoValido(p.precio)
                || p.stock < 0 || p.stock > 1000000000 || existeProducto(ps, i, p.codigo))
                throw runtime_error("producto invalido o duplicado");
        }
        for (int i = 0; i < ne; i++) {
            Empleado& e = es[i];
            if (!(f >> quoted(e.codigo) >> quoted(e.nombre) >> quoted(e.puesto) >> e.sueldo >> quoted(e.tiendaCodigo))
                || !textoValido(e.codigo) || !textoValido(e.nombre) || !textoValido(e.puesto)
                || !montoValido(e.sueldo) || !existeTienda(ts, nt, e.tiendaCodigo)) throw runtime_error("empleado invalido");
            for (int j = 0; j < i; j++) if (es[j].codigo == e.codigo) throw runtime_error("empleado duplicado");
        }
        if (!(f >> ws).eof()) throw runtime_error("contenido sobrante");
    } catch (const exception& error) {
        liberar();
        cout << "[Error] Carga cancelada: " << error.what() << ". Se mantienen los datos actuales.\n";
        return;
    }
    if (matriz) { for (int i = 0; i < numTiendas; i++) delete[] matriz[i]; delete[] matriz; }
    delete[] tiendas; delete[] productos; delete[] empleados;
    tiendas = ts; productos = ps; empleados = es; matriz = vs;
    numTiendas = nt; numDias = nd; numProductos = np; numEmpleados = ne;
    cout << "[Exito] Datos cargados desde multitienda.dat (reemplazan la sesion actual).\n";
}
