#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// Lectura por linea para no dejar caracteres pendientes en cin.
bool textoValido(const string& texto) {
    return texto.find_first_not_of(" \t\r") != string::npos
        && texto.find('|') == string::npos;
}

bool leerTexto(const string& mensaje, string& valor) {
    while (true) {
        cout << mensaje;
        if (!getline(cin, valor)) return false;
        if (textoValido(valor)) return true;
        cout << "[Error] Ingrese texto no vacio y sin el caracter |.\n";
    }
}

bool leerEntero(const string& mensaje, int& valor, int minimo, int maximo) {
    string linea;
    while (true) {
        cout << mensaje;
        if (!getline(cin, linea)) return false;
        stringstream entrada(linea);
        int numero;
        if ((entrada >> numero) && (entrada >> ws).eof()
            && numero >= minimo && numero <= maximo) {
            valor = numero;
            return true;
        }
        cout << "[Error] Ingrese un entero entre " << minimo << " y " << maximo << ".\n";
    }
}

bool leerMonto(const string& mensaje, double& valor) {
    string linea;
    while (true) {
        cout << mensaje;
        if (!getline(cin, linea)) return false;
        stringstream entrada(linea);
        double numero;
        if ((entrada >> numero) && (entrada >> ws).eof()
            && numero >= 0 && numero <= 1000000000000.0) {
            valor = numero;
            return true;
        }
        cout << "[Error] Ingrese un monto entre 0 y 1000000000000.\n";
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
    cout.setf(ios::fixed);
    cout.precision(2);
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
        cout << "13. Reportes del sistema (Ventas y Ranking)\n";
        cout << "14. Guardar datos en archivos\n";
        cout << "15. Cargar datos desde archivos\n";
        cout << " 0. Salir\n";
        cout << "Seleccione una opcion: ";
        if (!leerEntero("", opcion, 0, 15)) break;

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
            if (!leerEntero("", nuevosDias, 1, 366)) break;
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
    } while (opcion != 0 && !cin.eof());

    if (cin.eof()) cout << "\nEntrada finalizada.\n";

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
    if (tam >= 1000) { cout << "[Error] Limite de 1000 tiendas.\n"; return; }
    Tienda nueva;
    cout << "\n--- REGISTRAR TIENDA ---\n";
    if (!leerTexto("Codigo: ", nueva.codigo)) return;
    if (existeTienda(tiendas, tam, nueva.codigo)) {
        cout << "[Error] El codigo ya existe.\n";
        return;
    }
    if (!leerTexto("Nombre: ", nueva.nombre)
        || !leerTexto("Direccion: ", nueva.direccion)
        || !leerTexto("Distrito: ", nueva.distrito)
        || !leerTexto("Encargado: ", nueva.encargado)) return;
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
    if (!leerTexto("Codigo de tienda a modificar: ", cod)) return;
    int idx = -1;
    for (int i = 0; i < tam; i++) {
        if (tiendas[i].codigo == cod) { idx = i; break; }
    }
    if (idx == -1) { cout << "[Error] Tienda no encontrada.\n"; return; }
    int opc;
    cout << "1. Cambiar Encargado\n2. Cambiar Direccion/Distrito\n3. Cambiar Estado\n";
    if (!leerEntero("Opcion: ", opc, 1, 3)) return;
    Tienda modificada = tiendas[idx];
    if (opc == 1) {
        if (!leerTexto("Nuevo encargado: ", modificada.encargado)) return;
    } else if (opc == 2) {
        if (!leerTexto("Nueva direccion: ", modificada.direccion)
            || !leerTexto("Nuevo distrito: ", modificada.distrito)) return;
    } else {
        modificada.estado = !modificada.estado;
    }
    tiendas[idx] = modificada;
    cout << "[Exito] Tienda actualizada.\n";
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
    bool hayActiva = false;
    for (int i = 0; i < numTiendas; i++) if (tiendas[i].estado) hayActiva = true;
    if (!hayActiva) { cout << "[Error] No hay tiendas activas.\n"; return; }
    int opc;
    do {
        cout << "\n--- REGISTRAR VENTAS ---\n";
        mostrarTiendas(tiendas, numTiendas);
        string codigo;
        if (!leerTexto("Codigo de tienda: ", codigo)) return;
        int indice = -1;
        for (int i = 0; i < numTiendas; i++) {
            if (tiendas[i].codigo == codigo) { indice = i; break; }
        }
        if (indice == -1) {
            cout << "[Error] Tienda no encontrada.\n";
        } else if (!tiendas[indice].estado) {
            cout << "[Error] La tienda esta inactiva.\n";
        } else {
            int dia;
            double monto;
            cout << "Dias disponibles: 1 a " << numDias << "\n";
            if (!leerEntero("Dia: ", dia, 1, numDias)
                || !leerMonto("Monto total vendido en ese dia (reemplaza el anterior): ", monto)) return;
            matriz[indice][dia - 1] = monto;
            cout << "[Exito] Venta registrada.\n";
        }
        if (!leerEntero("Desea registrar otra venta? (1: Si / 0: No): ", opc, 0, 1)) return;
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
    if (tam >= 100000) { cout << "[Error] Limite de productos alcanzado.\n"; return; }
    Producto p;
    cout << "\n--- REGISTRAR PRODUCTO ---\n";
    if (!leerTexto("Codigo: ", p.codigo)) return;
    if (existeProducto(productos, tam, p.codigo)) { cout << "[Error] Codigo duplicado.\n"; return; }
    if (!leerTexto("Nombre: ", p.nombre) || !leerMonto("Precio: ", p.precio)
        || !leerEntero("Stock: ", p.stock, 0, 1000000000)) return;
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
    if (tam >= 100000) { cout << "[Error] Limite de empleados alcanzado.\n"; return; }
    Empleado e;
    cout << "\n--- REGISTRAR EMPLEADO ---\n";
    if (!leerTexto("Codigo: ", e.codigo)) return;
    for (int i = 0; i < tam; i++) {
        if (empleados[i].codigo == e.codigo) { cout << "[Error] Codigo duplicado.\n"; return; }
    }
    if (!leerTexto("Nombre: ", e.nombre) || !leerTexto("Puesto: ", e.puesto)
        || !leerMonto("Sueldo: ", e.sueldo)) return;
    mostrarTiendas(tiendas, numTiendas);
    if (!leerTexto("Codigo de tienda asignada: ", e.tiendaCodigo)) return;
    if (!existeTienda(tiendas, numTiendas, e.tiendaCodigo)) {
        cout << "[Error] La tienda asignada no existe.\n";
        return;
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
    cout << "\n--- VENTAS TOTALES POR TIENDA ---\n";
    for (int i = 0; i < numTiendas; i++) {
        double suma = 0;
        for (int j = 0; j < numDias; j++) suma += matriz[i][j];
        cout << tiendas[i].nombre << " (" << tiendas[i].codigo << "): S/ " << suma << "\n";
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

    cout << "Tienda con mayor venta: " << tiendas[idxMax].nombre << " con S/ " << totales[idxMax] << "\n";
    cout << "Tienda con menor venta: " << tiendas[idxMin].nombre << " con S/ " << totales[idxMin] << "\n";

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
void guardarDatos(Tienda* tiendas, int numTiendas, double** matriz, int numDias, Producto* productos, int numProductos, Empleado* empleados, int numEmpleados) {
    // Comprobar acceso sin borrar los archivos existentes.
    ofstream ft("tiendas.txt", ios::app), fv("ventas.txt", ios::app);
    ofstream fp("productos.txt", ios::app), fe("empleados.txt", ios::app);
    if (!ft || !fv || !fp || !fe) {
        cout << "[Error] No se pudieron abrir todos los archivos para guardar.\n";
        return;
    }
    ft.close(); fv.close(); fp.close(); fe.close();
    ft.open("tiendas.txt", ios::trunc); fv.open("ventas.txt", ios::trunc);
    fp.open("productos.txt", ios::trunc); fe.open("empleados.txt", ios::trunc);
    if (!ft || !fv || !fp || !fe) {
        cout << "[Error] No se pudieron preparar los archivos para guardar.\n";
        return;
    }
    fv.precision(17);
    fp.precision(17);
    fe.precision(17);
    for (int i = 0; i < numTiendas; i++) {
        ft << tiendas[i].codigo << "|" << tiendas[i].nombre << "|"
           << tiendas[i].direccion << "|" << tiendas[i].distrito << "|"
           << tiendas[i].encargado << "|" << tiendas[i].estado << "\n";
    }
    fv << numTiendas << " " << numDias << "\n";
    for (int i = 0; i < numTiendas; i++) {
        for (int j = 0; j < numDias; j++) fv << matriz[i][j] << " ";
        fv << "\n";
    }
    for (int i = 0; i < numProductos; i++) {
        fp << productos[i].codigo << "|" << productos[i].nombre << "|"
           << productos[i].precio << "|" << productos[i].stock << "\n";
    }
    for (int i = 0; i < numEmpleados; i++) {
        fe << empleados[i].codigo << "|" << empleados[i].nombre << "|"
           << empleados[i].puesto << "|" << empleados[i].sueldo << "|"
           << empleados[i].tiendaCodigo << "\n";
    }
    ft.close(); fv.close(); fp.close(); fe.close();
    if (!ft || !fv || !fp || !fe) {
        cout << "[Error] Escritura incompleta. Revise los archivos antes de cerrar.\n";
        return;
    }
    cout << "[Exito] Tiendas, ventas, productos y empleados guardados.\n";
}

// Cuenta registros y vuelve al inicio sin modificar los datos de la sesion.
bool contarRegistros(ifstream& archivo, int& cantidad, int maximo) {
    string linea;
    cantidad = 0;
    while (getline(archivo, linea)) {
        if (!textoValido(linea.substr(0, linea.find('|'))) || ++cantidad > maximo) return false;
    }
    if (!archivo.eof()) return false;
    archivo.clear();
    archivo.seekg(0);
    return bool(archivo);
}

void cargarDatos(Tienda*& tiendas, int& numTiendas, double**& matriz, int& numDias, Producto*& productos, int& numProductos, Empleado*& empleados, int& numEmpleados) {
    ifstream ft("tiendas.txt"), fv("ventas.txt"), fp("productos.txt"), fe("empleados.txt");
    if (!ft || !fv || !fp || !fe) {
        cout << "[Error] Se necesitan tiendas.txt, ventas.txt, productos.txt y empleados.txt.\n"
             << "No se modificaron los datos actuales.\n";
        return;
    }
    int nt = 0, nd = 0, np = 0, ne = 0, filas = 0;
    string linea;
    getline(fv, linea);
    stringstream cabecera(linea);
    if (!(cabecera >> nt >> nd) || !(cabecera >> ws).eof()
        || nt < 0 || nt > 1000 || nd < 0 || nd > 366 || (nt == 0 && nd != 0)
        || !contarRegistros(ft, filas, 1000) || filas != nt
        || !contarRegistros(fp, np, 100000) || !contarRegistros(fe, ne, 100000)) {
        cout << "[Error] Archivos invalidos o cantidades inconsistentes. No se modificaron los datos actuales.\n";
        return;
    }
    // Leer en arreglos temporales. La sesion se reemplaza solo si todo es valido.
    Tienda* ts = new Tienda[nt];
    Producto* ps = new Producto[np];
    Empleado* es = new Empleado[ne];
    double** vs = new double*[nt];
    for (int i = 0; i < nt; i++) vs[i] = new double[nd]();
    bool valido = true;
    for (int i = 0; i < nt && valido; i++) {
        if (!getline(ft, linea)) { valido = false; break; }
        stringstream ss(linea);
        string estado;
        Tienda& t = ts[i];
        valido = bool(getline(ss, t.codigo, '|')) && bool(getline(ss, t.nombre, '|'))
            && bool(getline(ss, t.direccion, '|')) && bool(getline(ss, t.distrito, '|'))
            && bool(getline(ss, t.encargado, '|')) && bool(getline(ss, estado));
        stringstream valorEstado(estado);
        int numeroEstado;
        valido = valido && textoValido(t.codigo) && textoValido(t.nombre)
            && textoValido(t.direccion) && textoValido(t.distrito) && textoValido(t.encargado)
            && !existeTienda(ts, i, t.codigo)
            && bool(valorEstado >> numeroEstado) && (valorEstado >> ws).eof()
            && (numeroEstado == 0 || numeroEstado == 1);
        if (valido) t.estado = numeroEstado == 1;
    }
    for (int i = 0; i < nt && valido; i++) {
        if (!getline(fv, linea)) { valido = false; break; }
        stringstream ss(linea);
        for (int j = 0; j < nd && valido; j++) {
            valido = bool(ss >> vs[i][j]) && vs[i][j] >= 0 && vs[i][j] <= 1000000000000.0;
        }
        if (!(ss >> ws).eof()) valido = false;
    }
    if (!(fv >> ws).eof()) valido = false;
    for (int i = 0; i < np && valido; i++) {
        if (!getline(fp, linea)) { valido = false; break; }
        stringstream ss(linea);
        string precioStock;
        Producto& p = ps[i];
        valido = bool(getline(ss, p.codigo, '|')) && bool(getline(ss, p.nombre, '|'))
            && bool(getline(ss, precioStock));
        stringstream valores(precioStock);
        char separador = 0;
        valido = valido && textoValido(p.codigo) && textoValido(p.nombre)
            && !existeProducto(ps, i, p.codigo)
            && bool(valores >> p.precio >> separador >> p.stock) && separador == '|'
            && (valores >> ws).eof() && p.precio >= 0 && p.precio <= 1000000000000.0
            && p.stock >= 0 && p.stock <= 1000000000;
    }
    for (int i = 0; i < ne && valido; i++) {
        if (!getline(fe, linea)) { valido = false; break; }
        stringstream ss(linea);
        string sueldo;
        Empleado& e = es[i];
        valido = bool(getline(ss, e.codigo, '|')) && bool(getline(ss, e.nombre, '|'))
            && bool(getline(ss, e.puesto, '|')) && bool(getline(ss, sueldo, '|'))
            && bool(getline(ss, e.tiendaCodigo));
        stringstream valorSueldo(sueldo);
        valido = valido && textoValido(e.codigo) && textoValido(e.nombre) && textoValido(e.puesto)
            && textoValido(e.tiendaCodigo) && existeTienda(ts, nt, e.tiendaCodigo)
            && bool(valorSueldo >> e.sueldo) && (valorSueldo >> ws).eof()
            && e.sueldo >= 0 && e.sueldo <= 1000000000000.0;
        for (int j = 0; j < i && valido; j++) if (es[j].codigo == e.codigo) valido = false;
    }
    if (!valido) {
        for (int i = 0; i < nt; i++) delete[] vs[i];
        delete[] vs; delete[] ts; delete[] ps; delete[] es;
        cout << "[Error] Archivo incompleto, duplicado o con datos invalidos. No se modificaron los datos actuales.\n";
        return;
    }
    // Liberar con la cantidad ANTERIOR de tiendas, antes de cambiar los contadores.
    if (matriz != nullptr) {
        for (int i = 0; i < numTiendas; i++) delete[] matriz[i];
        delete[] matriz;
    }
    delete[] tiendas; delete[] productos; delete[] empleados;
    tiendas = ts; matriz = vs; productos = ps; empleados = es;
    numTiendas = nt; numDias = nd; numProductos = np; numEmpleados = ne;
    cout << "[Exito] Datos cargados. Se reemplazo la sesion actual.\n";
}
