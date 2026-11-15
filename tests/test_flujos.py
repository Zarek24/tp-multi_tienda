"""Pruebas de consola en carpetas temporales; no modifican datos del usuario."""
import pathlib
import subprocess
import sys
import tempfile
import unittest

BINARY = pathlib.Path(sys.argv.pop(1)).resolve()


class Flujos(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.folder = pathlib.Path(self.tmp.name)

    def run_app(self, commands):
        result = subprocess.run([str(BINARY)], input=commands, text=True,
                                capture_output=True, cwd=self.folder, timeout=10)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stderr, '')
        return result.stdout

    def tienda(self, codigo='T1'):
        return f'1\n{codigo}\nTienda {codigo}\nAv. Principal\nLima\nAna\n'

    def test_agregar_tienda_y_redimensionar(self):
        output = self.run_app(self.tienda() + '4\n2\n0\n1\n100\n0\n'
                              + self.tienda('T2') + '5\n4\n3\n1\n3\n50\n0\n10\n12\n0\n')
        self.assertIn('Total General Vendido: S/ 150.00', output)
        self.assertIn('Promedio diario: S/ 33.33', output)
        self.assertIn('Promedio diario: S/ 16.67', output)

    def test_persistencia_completa_y_carga_repetida(self):
        self.run_app(self.tienda() + '4\n1\n0\n1\n123.45\n0\n'
                     + '6\nP1\nProducto especial\n2.50\n7\n'
                     + '8\nE1\nJuan Perez\nCajero\n1200\nT1\n14\n0\n')
        original = (self.folder / 'multitienda.dat').read_text()
        output = self.run_app('15\n15\n2\n5\n7\n9\n14\n0\n')
        for expected in ['123.45', 'Producto especial', 'Juan Perez', '1200.00']:
            self.assertIn(expected, output)
        self.assertEqual(original, (self.folder / 'multitienda.dat').read_text())

    def test_archivo_corrupto_no_borra_sesion(self):
        (self.folder / 'multitienda.dat').write_text('MULTITIENDA_V1\n2 366 0 0\n')
        output = self.run_app(self.tienda() + '15\n2\n0\n')
        self.assertIn('Carga cancelada', output)
        self.assertIn('Nombre: Tienda T1', output)

    def test_negativos_texto_duplicados_y_tienda_inexistente(self):
        output = self.run_app('letras\n' + self.tienda()
                              + '6\nP1\nPan\n-1\nabc\n3\n-2\n5\n'
                              + '8\nE1\nAna\nCaja\n100\nNO_EXISTE\n'
                              + '8\nE1\nAna\nCaja\n100\nT1\n8\nE1\n'
                              + '9\n0\n')
        self.assertIn('Ingrese un numero', output)
        self.assertIn('La tienda asignada no existe', output)
        self.assertIn('Codigo duplicado', output)

    def test_vacio_y_fin_de_entrada(self):
        self.run_app('14\n15\n5\n0\n')
        self.assertIn('Entrada finalizada', self.run_app('1\n'))

    def test_carga_dimensiones_invalidas(self):
        for header in ['-1 2 0 0', '0 1 0 0', '1 999999 0 0']:
            (self.folder / 'multitienda.dat').write_text('MULTITIENDA_V1\n' + header)
            self.assertIn('Carga cancelada', self.run_app('15\n0\n'))


if __name__ == '__main__':
    unittest.main()
