#include <iostream>
#include <fstream>
#include <tuple>
#include <chrono>

std::string MATRIX1_PATH = "/Users/vadimkotlarskij/Desktop/c++/parallelprogLAB1/lab1/matrix1.txt";
std::string MATRIX2_PATH = "/Users/vadimkotlarskij/Desktop/c++/parallelprogLAB1/lab1/matrix2.txt";
std::string RESULT_PATH = "/Users/vadimkotlarskij/Desktop/c++/parallelprogLAB1/lab1/result.txt";
std::string STATS_PATH = "/Users/vadimkotlarskij/Desktop/c++/parallelprogLAB1/lab1/stats.txt";


struct stats {
    int size;
    double time;
};


void writeStats(stats st, std::string path) {
    std::ofstream file(path, std::ios_base::app);
    file << st.size << " " << st.time << std::endl;
    file.close();
}


void clearFile(std::string path) {
    std::ofstream file(path, std::ios_base::trunc);
    file.close();
}


int** generateMatrix(size_t rows, size_t cols, int low, int up) {
    srand(time(NULL));
    int** matrix = new int*[rows];
    for (size_t i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
    }
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            matrix[i][j] = rand() % (up - low + 1) + low;
        }
    }
    return matrix;
}

void printMatrix(const int* const* matrix, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            std::cout << matrix[i][j] << "  ";
        }
        std::cout << "\n";
    }
}

int** mulMatrix(const int* const* matrix1, const int* const* matrix2, size_t rows1, size_t cols1, size_t rows2, size_t cols2) {
    if (cols1 != rows2) {
        return NULL;
    }
    int** result = new int*[rows1];
    for (size_t i = 0; i < rows1; ++i) {
        result[i] = new int[cols2]();
    }
    for (size_t i = 0; i < rows1; ++i) {
        for (size_t j = 0; j < cols2; ++j) {
            for (size_t k = 0; k < rows2; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

int** mulMatrix(const int* const* matrix1, const int* const* matrix2, size_t size) {
    int** result = new int* [size];
    for (size_t i = 0; i < size; ++i) {
        result[i] = new int[size]();
    }
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            for (size_t k = 0; k < size; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}


void writeMatrix(const int* const* matrix, size_t rows, size_t cols, const std::string path) {
    std::ofstream file(path);
    file << rows << " " << cols << std::endl;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            file << matrix[i][j] << " ";
        }
        file << std::endl;
    }
    file.close();
}

std::tuple<int**, size_t, size_t> readMatrix(std::string path) {
    std::ifstream file(path);
    size_t rows, cols;
    file >> rows >> cols;
    int** result = new int* [rows];
    for (size_t i = 0; i < rows; ++i) {
        result[i] = new int[cols]();
    }
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            file >> result[i][j];
        }
    }
    file.close();
    return std::make_tuple(result, rows, cols);
}

int** mulMatrix(std::string mat1Path, std::string mat2Path) {
    std::tuple<int**, size_t, size_t> m1 = readMatrix(mat1Path);
    std::tuple<int**, size_t, size_t> m2 = readMatrix(mat2Path);
    int** mat1 = std::get<0>(m1);
    int** mat2 = std::get<0>(m2);
    size_t size = std::get<1>(m2);
    int** result = new int* [size];
    for (size_t i = 0; i < size; ++i) {
        result[i] = new int[size]();
    }
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            for (size_t k = 0; k < size; ++k) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return result;
}

int main() {
    clearFile(MATRIX1_PATH);
    clearFile(MATRIX2_PATH);
    clearFile(RESULT_PATH);
    clearFile(STATS_PATH);
    size_t size = 500;
    stats result;
    int range = 1000000;
    while (size <= 1600) {
        result.size = size;
        result.time = 0;
        std::cout << "SIZE: " << size << "\n";
        for (size_t i = 0; i < 10; ++i) {
            std::cout << "---------- Iteration " << i << " ----------\n";
            int** mat1 = generateMatrix(size, size, -range, range);
            int** mat2 = generateMatrix(size, size, -range, range);
            std::cout << "Matrices are generated" << "\n";
            writeMatrix(mat1, size, size, MATRIX1_PATH);
            writeMatrix(mat2, size, size, MATRIX2_PATH);
            std::cout << "Matrices are written" << "\n";
            auto start = std::chrono::high_resolution_clock::now();
            int** res = mulMatrix(MATRIX1_PATH, MATRIX2_PATH);
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            writeMatrix(res, size, size, RESULT_PATH);
            std::cout << "Result is written" << "\n";
            result.time += (double)duration.count();
        }
        result.time /= 10;
        std::cout << result.time << "\n";
        writeStats(result, STATS_PATH);
        std::cout << "Stats is written" << "\n\n\n";
        size += 100;
    }
    return 0;
}
