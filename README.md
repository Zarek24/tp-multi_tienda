# Sistema de Gestión Multitienda - SpeedyBite Delivery

Proyecto en C++ para la gestión y análisis de recaudación de ventas diarias por tienda.

## Características
- Registro dinámico de tiendas y días de evaluación con memoria dinámica en tiempo de ejecución.
- Registro de ventas diarias mediante matriz bidimensional (`float**`).
- Reportes tabulados en consola:
  - Ventas totales por tienda.
  - Promedio de ventas diarias.
  - Tienda con mayor recaudación (más rentable).
  - Tienda con menor recaudación.
- Exportación de balances a archivo de texto (`ventas.txt`).

## Mejoras de Presentación (v1.1.0)
- **Formato numérico (`<iomanip>`):** Uso de `fixed` y `setprecision(2)` para mostrar montos monetarios con dos decimales exactos.
- **Alineación (`\t`):** Organización de reportes en columnas tabuladas.
- **Limpieza de interfaz (`<cstdlib>`):** Uso de `system("cls")` antes de desplegar el menú principal para mantener una consola ordenada.

## Compilación y Ejecución
```bash
g++ multitienda.cpp -o multitienda
./multitienda
```

## Estructura
- `multitienda.cpp`: Código fuente del proyecto.
- `ventas.txt`: Archivo generado al guardar los datos desde el menú.
