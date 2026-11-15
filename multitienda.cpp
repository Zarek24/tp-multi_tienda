#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

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
    Tienda* tiendas = nullptr;
    int numTiendas = 0;

    Producto* productos = nullptr;
    int numProductos = 0;

    Empleado* empleados = nullptr;
    int numEmpleados = 0;

    double** matrizVentas = nullptr;
    int numDias = 0; // Se inicializa al crear la matriz

    int opcion;

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
        cout << "13. Reportes del sistema (Rentabilidad y Rankings)\n";
        cout << "14. Guardar datos en archivos\n";
        cout << "15. Cargar datos desde archivos\n";
        cout << " 0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1: {
            int tiendasAntes = numTiendas;
            registrarTienda(tiendas, numTiendas);
            // Mantener una fila de ventas por cada tienda registrada.
            if (numTiendas > tiendasAntes && matrizVentas != nullptr) {
                crearMatrizVentas(matrizVentas, tiendasAntes, numDias, numTiendas, numDias);
            }
            break;
        }
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
            cin >> nuevosDias;
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
    Tienda nueva;
    cout << "\n--- REGISTRAR TIENDA ---\n";
    cout << "Codigo: "; cin >> nueva.codigo;
    if (existeTienda(tiendas, tam, nueva.codigo)) {
        cout << "[Error] El codigo ya existe.\n";
        return;
    }
    cin.ignore();
    cout << "Nombre: "; getline(cin, nueva.nombre);
    cout << "Direccion: "; getline(cin, nueva.direccion);
    cout << "Distrito: "; getline(cin, nueva.distrito);
    cout << "Encargado: "; getline(cin, nueva.encargado);
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
    cout << "Codigo de tienda a modificar: "; cin >> cod;
    int idx = -1;
    for (int i = 0; i < tam; i++) {
        if (tiendas[i].codigo == cod) { idx = i; break; }
    }
    if (idx == -1) { cout << "[Error] Tienda no encontrada.\n"; return; }

    int opc;
    cout << "1. Cambiar Encargado\n2. Cambiar Direccion/Distrito\n3. Cambiar Estado\nOpcion: ";
    cin >> opc;
    cin.ignore();
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
        cout << "Indique el indice de la tienda (0 a " << numTiendas - 1 << "): "; cin >> tIndex;
        cout << "Indique el dia (1 a " << numDias << "): "; cin >> dIndex;
        cout << "Monto vendido: "; cin >> monto;

        if (tIndex >= 0 && tIndex < numTiendas && dIndex >= 1 && dIndex <= numDias) {
            matriz[tIndex][dIndex - 1] = monto;
            cout << "[Exito] Venta registrada.\n";
        }
        else {
            cout << "[Error] Indices fuera de rango.\n";
        }

        cout << "Desea registrar otra venta? (1: Si / 0: No): "; cin >> opc;
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
    Producto p;
    cout << "\n--- REGISTRAR PRODUCTO ---\n";
    cout << "Codigo: "; cin >> p.codigo;
    if (existeProducto(productos, tam, p.codigo)) { cout << "[Error] Codigo duplicado.\n"; return; }
    cin.ignore();
    cout << "Nombre: "; getline(cin, p.nombre);
    cout << "Precio: "; cin >> p.precio;
    cout << "Stock: "; cin >> p.stock;

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
            << " | Precio: $" << productos[i].precio << " | Stock: " << productos[i].stock << "\n";
    }
}

// --- GESTIÓN DE EMPLEADOS ---
void registrarEmpleado(Empleado*& empleados, int& tam, Tienda* tiendas, int numTiendas) {
    if (numTiendas == 0) { cout << "Debe registrar tiendas primero.\n"; return; }
    Empleado e;
    cout << "\n--- REGISTRAR EMPLEADO ---\n";
    cout << "Codigo: "; cin >> e.codigo;
    cin.ignore();
    cout << "Nombre: "; getline(cin, e.nombre);
    cout << "Puesto: "; getline(cin, e.puesto);
    cout << "Sueldo: "; cin >> e.sueldo;
    mostrarTiendas(tiendas, numTiendas);
    cout << "Codigo de tienda asignada: "; cin >> e.tiendaCodigo;

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
            << " | Puesto: " << empleados[i].puesto << " | Sueldo: $" << empleados[i].sueldo
            << " | Tienda: " << empleados[i].tiendaCodigo << "\n";
    }
}

// --- CÁLCULOS DEL SISTEMA ---
void calcularVentasPorTienda(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    cout << "\n--- VENTAS TOTALES POR TIENDA ---\n";
    for (int i = 0; i < numTiendas; i++) {
        double suma = 0;
        for (int j = 0; j < numDias; j++) suma += matriz[i][j];
        cout << tiendas[i].nombre << " (" << tiendas[i].codigo << "): $" << suma << "\n";
    }
}

void calcularPromedios(double** matriz, int numTiendas, int numDias) {
    cout << "\n--- PROMEDIOS DE VENTAS ---\n";
    double totalGlobal = 0;
    int totalCeldas = numTiendas * numDias;

    for (int i = 0; i < numTiendas; i++) {
        for (int j = 0; j < numDias; j++) {
            totalGlobal += matriz[i][j];
        }
    }

    // Calcular el promedio diario considerando el total de días entre todas las tiendas o por día general
    double promedioDiario = (numDias > 0) ? (totalGlobal / numDias) : 0;

    cout << "Promedio Diario General: $" << promedioDiario << "\n";
    cout << "Promedio Mensual (Total Global): $" << totalGlobal << "\n";
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
    cout << "Venta Maxima: $" << maxV << " en la tienda " << tiendas[tMax].nombre << " el Dia " << (dMax + 1) << "\n";
    cout << "Venta Minima: $" << minV << " en la tienda " << tiendas[tMin].nombre << " el Dia " << (dMin + 1) << "\n";
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
    cout << "Total General Vendido: $" << totalGen << "\n";
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

    cout << "Tienda Mas Rentable: " << tiendas[idxMax].nombre << " with $" << totales[idxMax] << "\n";
    cout << "Tienda Menos Rentable: " << tiendas[idxMin].nombre << " with $" << totales[idxMin] << "\n";

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
        cout << i + 1 << ". " << tiendas[indices[i]].nombre << " - $" << totales[indices[i]] << "\n";
    }

    delete[] totales;
    delete[] indices;
}

// --- GESTIÓN DE ARCHIVOS ---
void guardarDatos(Tienda* tiendas, int numTiendas, double** matriz, int numDias, Producto* productos, int numProductos, Empleado* empleados, int numEmpleados) {
    // 1. Guardar Tiendas
    ofstream fTiendas("tiendas.txt");
    for (int i = 0; i < numTiendas; i++) {
        fTiendas << tiendas[i].codigo << "|" << tiendas[i].nombre << "|"
            << tiendas[i].direccion << "|" << tiendas[i].distrito << "|"
            << tiendas[i].encargado << "|" << tiendas[i].estado << "\n";
    }
    fTiendas.close();

    // 2. Guardar Ventas
    ofstream fVentas("ventas.txt");
    fVentas << numTiendas << " " << numDias << "\n";
    if (matriz != nullptr) {
        for (int i = 0; i < numTiendas; i++) {
            for (int j = 0; j < numDias; j++) {
                fVentas << matriz[i][j] << (j == numDias - 1 ? "" : " ");
            }
            fVentas << "\n";
        }
    }
    fVentas.close();

    // 3. Guardar Productos
    ofstream fProd("productos.txt");
    for (int i = 0; i < numProductos; i++) {
        fProd << productos[i].codigo << "|" << productos[i].nombre << "|"
            << productos[i].precio << "|" << productos[i].stock << "\n";
    }
    fProd.close();

    cout << "[Exito] Datos guardados correctamente en archivos de texto.\n";
}

void cargarDatos(Tienda*& tiendas, int& numTiendas, double**& matriz, int& numDias, Producto*& productos, int& numProductos, Empleado*& empleados, int& numEmpleados) {
    // Cargar Tiendas
    ifstream fTiendas("tiendas.txt");
    if (fTiendas.is_open()) {
        string linea;
        delete[] tiendas;
        tiendas = nullptr;
        numTiendas = 0;
        while (getline(fTiendas, linea)) {
            stringstream ss(linea);
            string cod, nom, dir, dist, enc, estStr;
            getline(ss, cod, '|');
            getline(ss, nom, '|');
            getline(ss, dir, '|');
            getline(ss, dist, '|');
            getline(ss, enc, '|');
            getline(ss, estStr);

            Tienda t = { cod, nom, dir, dist, enc, (estStr == "1") };
            Tienda* aux = new Tienda[numTiendas + 1];
            for (int i = 0; i < numTiendas; i++) aux[i] = tiendas[i];
            aux[numTiendas] = t;
            delete[] tiendas;
            tiendas = aux;
            numTiendas++;
        }
        fTiendas.close();
    }

    // Cargar Ventas
    ifstream fVentas("ventas.txt");
    if (fVentas.is_open()) {
        int nT, nD;
        fVentas >> nT >> nD;
        crearMatrizVentas(matriz, numTiendas, numDias, nT, nD);
        numDias = nD;
        for (int i = 0; i < nT; i++) {
            for (int j = 0; j < nD; j++) {
                fVentas >> matriz[i][j];
            }
        }
        fVentas.close();
    }

    cout << "[Exito] Datos cargados exitosamente desde los archivos.\n";
}