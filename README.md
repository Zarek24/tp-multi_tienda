# Sistema de Gestión Multitienda - SpeedyBite Delivery

Proyecto académico en C++ para registrar tiendas, ventas diarias, productos y empleados, y consultar reportes.

## Funcionalidades
- Registro, consulta y modificación de tiendas (incluye suspensión y reactivación).
- Matriz dinámica de ventas por tienda y día.
- Registro y consulta de productos y empleados.
- Reportes de ventas, promedios, máximos, mínimos y participación.
- Guardado y carga de datos mediante archivos de texto.

## Archivos de datos
El programa genera estos archivos al seleccionar **Guardar datos** (opción 6):

- `tiendas.txt`: `codigo;nombre;direccion;distrito;encargado;estado` (estado: `1` activa, `0` suspendida).
- `ventas.txt`: primera línea `cantidadTiendas cantidadDias`; después, una línea por tienda con los montos diarios separados por espacios.
- `productos.txt`: `codigo;nombre;precio;stock`.
- `empleados.txt`: `codigo;nombre;puesto;sueldo;codigoTienda`.

Cada registro de tiendas, productos y empleados ocupa una línea. Los archivos no llevan encabezados. Los campos de texto no admiten el carácter `;`.

**Importante:** Los archivos guardados en versiones anteriores con el separador `|` no son compatibles con esta versión. Realiza una copia de seguridad antes de sobrescribirlos. La opción **Cargar datos** (7) requiere los cuatro archivos en el directorio de trabajo.

## Compilación y ejecución
Con un compilador compatible con C++11 o superior:

```bash
g++ -std=c++11 multitienda.cpp -o multitienda
./multitienda
```

En Windows, ejecuta `multitienda.exe` después de compilar.

## Estructura
- `multitienda.cpp`: código fuente.
- `README.md`: documentación del proyecto.
- Los cuatro archivos `.txt` se generan durante la ejecución.
