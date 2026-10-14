# GameOfLife-cpp

A terminal simulation of Conway's Game of Life written in C++. Cells outside the grid are dead; edges do not wrap.

## Build and run

Requires a C++17 compiler. From the repository root:

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o game-of-life
./game-of-life
```

The program reads `src/1.txt` relative to the working directory. Edit this file to choose the initial state: use equally sized, nonempty rows of whitespace-separated `0` (dead) and `1` (alive) cells. Tabs and extra spaces are accepted. Empty grids, unequal row lengths, and other values are rejected.

Enter a positive integer when prompted for the number of generations. The terminal is cleared with ANSI escape sequences; each displayed state is followed by an update and a 100 ms pause. The first displayed generation is the initial state.

## Regression checks

Requires Python 3 and `c++` on PATH; no Python packages are needed:

```sh
python3 -m unittest discover -s tests -v
```

Tests compile with warnings treated as errors, check whitespace handling, and reject malformed grids. They use temporary fixtures without changing `src/1.txt`.

## Limitations

The input path is fixed, and there is no graphical interface. End-of-input at the generation prompt is not currently handled as an exit condition; provide a positive generation count when piping input.
