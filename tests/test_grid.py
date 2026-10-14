"""CLI regression tests; requires Python 3 and a C++17 compiler."""
import pathlib
import subprocess
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[1]


class GridTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.build = tempfile.TemporaryDirectory()
        cls.binary = pathlib.Path(cls.build.name) / "life"
        subprocess.run(["c++", "-std=c++17", "-Wall", "-Wextra", "-Wpedantic", "-Werror", str(ROOT / "main.cpp"), "-o", str(cls.binary)], check=True)

    @classmethod
    def tearDownClass(cls):
        cls.build.cleanup()

    def run_grid(self, grid):
        with tempfile.TemporaryDirectory() as directory:
            source = pathlib.Path(directory) / "src"
            source.mkdir()
            (source / "1.txt").write_text(grid)
            return subprocess.run([str(self.binary)], cwd=directory, input="1\n", text=True, capture_output=True, timeout=5)

    def test_whitespace(self):
        result = self.run_grid("  0\t1  0  \n1 0 1\n")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("Width: 3, Height: 2", result.stdout)

    def test_invalid_grids(self):
        for grid in ("", "\n", "0 1\n1\n", "0 1\n1 0 1\n", "0 x\n", "0 2\n", "0 1x\n"):
            with self.subTest(grid=grid):
                result = self.run_grid(grid)
                self.assertEqual(result.returncode, 1)
                self.assertIn("Error reading initial state", result.stderr)


if __name__ == "__main__":
    unittest.main()
