#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>

// Function prototypes
std::vector<std::vector<bool>> readInitialState(const std::string& filePath);
void printState(const std::vector<std::vector<bool>>& grid);
void runGameOfLife(std::vector<std::vector<bool>>& initialState, int numGenerations);
int countNeighbors(const std::vector<std::vector<bool>>& grid, int row, int col);
void clearConsole();
int promptForPositiveInteger(const std::string& prompt);

int main() {
    std::string filePath;
    filePath = "src/1.txt";

    // Read the initial state from the file
    auto initialState = readInitialState(filePath);
    if (initialState.empty()) {
        std::cerr << "Error reading initial state." << std::endl;
        return 1;
    }

    // Print the initial state
    printState(initialState);

    // Get the width and height of the grid
    int width = initialState[0].size();
    int height = initialState.size();
    if (width <= 0 || height <= 0) {
        std::cerr << "Invalid input." << std::endl;
        return 1;
    }

    // Prompt the user for the number of generations
    int numGenerations = promptForPositiveInteger("Enter number of generations: ");

    // Run the game of life
    runGameOfLife(initialState, numGenerations);

    return 0;
}

std::vector<std::vector<bool>> readInitialState(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "File not found: " << filePath << std::endl;
        return {};
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    int height = lines.size();
    int width = 0;
    if (height > 0) {
        width = std::count(lines[0].begin(), lines[0].end(), ' ') + 1;
    }

    std::vector<std::vector<bool>> initialState(height, std::vector<bool>(width));
    for (int i = 0; i < height; ++i) {
        std::istringstream iss(lines[i]);
        for (int j = 0; j < width; ++j) {
            int value;
            iss >> value;
            initialState[i][j] = (value == 1);
        }
    }

    std::cout << "Initial state loaded from " << filePath << std::endl;
    std::cout << "Width: " << width << ", Height: " << height << std::endl;
    return initialState;
}

void runGameOfLife(std::vector<std::vector<bool>>& initialState, int numGenerations) {
    int height = initialState.size();
    int width = initialState[0].size();

    for (int gen = 1; gen <= numGenerations; ++gen) {
        std::cout << "Generation " << gen << ":" << std::endl;
        printState(initialState);

        std::vector<std::vector<bool>> nextGeneration(height, std::vector<bool>(width));
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int neighbors = countNeighbors(initialState, i, j);
                nextGeneration[i][j] = (neighbors == 3) || (initialState[i][j] && neighbors == 2);
            }
        }
        initialState = nextGeneration;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int countNeighbors(const std::vector<std::vector<bool>>& grid, int row, int col) {
    int count = 0;
    int height = grid.size();
    int width = grid[0].size();
    for (int i = row - 1; i <= row + 1; ++i) {
        for (int j = col - 1; j <= col + 1; ++j) {
            if (i >= 0 && i < height && j >= 0 && j < width && !(i == row && j == col)) {
                if (grid[i][j]) {
                    ++count;
                }
            }
        }
    }
    return count;
}

void printState(const std::vector<std::vector<bool>>& grid) {
    clearConsole();
    for (const auto& row : grid) {
        for (bool cell : row) {
            std::cout << (cell ? "1 " : "0 ");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void clearConsole() {
    std::cout << "\033[H\033[2J" << std::flush;
}

int promptForPositiveInteger(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value > 0) {
            return value;
        } else {
            std::cin.clear(); // clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            std::cerr << "Invalid input. Please enter a positive integer." << std::endl;
        }
    }
}