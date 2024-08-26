#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib> // Для використання системної команди

using namespace std;

// Оголошення абсолютного шляху до файлу з результатами
const string outputFilePath = "/Users/nikmas/Desktop/output.txt";

// Функція для виведення матриці на екран
void printMatrix(vector<vector<double>>& matrix) {
    int n = matrix.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

// Функція для обміну двох рядків матриці
void swapRows(vector<vector<double>>& matrix, int row1, int row2) {
    int n = matrix.size();
    for (int i = 0; i < n + 1; i++) {
        swap(matrix[row1][i], matrix[row2][i]);
    }
}

// Функція для знаходження максимального елемента в стовпці
double findMaxInColumn(vector<vector<double>>& matrix, int column, int startRow) {
    int n = matrix.size();
    double maxVal = abs(matrix[startRow][column]);
    for (int i = startRow + 1; i < n; i++) {
        double currentVal = abs(matrix[i][column]);
        if (currentVal > maxVal) {
            maxVal = currentVal;
        }
    }
    return maxVal;
}

// Функція для обчислення визначника матриці
double calculateDeterminant(vector<vector<double>>& matrix) {
    int n = matrix.size();
    if (n == 1) {
        return matrix[0][0]; // Для 1x1 матриці визначник дорівнює її єдиному елементу
    }

    double det = 1.0;
    for (int i = 0; i < n; i++) {
        // Знаходимо максимальний елемент в поточному стовпці
        double maxVal = findMaxInColumn(matrix, i, i);

        // Перевіряємо, чи максимальний елемент не дорівнює нулю
        if (maxVal == 0) {
            return 0.0; // Повертаємо визначник, який дорівнює нулю
        }

        // Пошук рядка з максимальним елементом та обмін з поточним рядком
        for (int j = i; j < n; j++) {
            if (abs(matrix[j][i]) == maxVal) {
                swapRows(matrix, i, j);
                break;
            }
        }

        // Після обміну зробимо головний елемент 1
        double pivot = matrix[i][i];
        for (int j = i; j < n + 1; j++) {
            matrix[i][j] /= pivot;
        }

        // Віднімання поточного рядка від інших рядків для створення нулів під головним елементом
        for (int j = i + 1; j < n; j++) {
            double factor = matrix[j][i];
            for (int k = i; k < n + 1; k++) {
                matrix[j][k] -= factor * matrix[i][k];
            }
        }
    }

    return det;
}

// Функція для обчислення похибки
double calculateError(vector<vector<double>>& matrix, vector<double>& solution) {
    int n = matrix.size();
    double error = 0.0;
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++) {
            sum += matrix[i][j] * solution[j];
        }
        error += abs(sum - matrix[i][n]); // Додаємо похибку для кожного рівняння
    }
    return error;
}

// Головна функція для методу Гауса з поступковим вибором головного елемента
vector<double> gaussianElimination(vector<vector<double>>& matrix) {
    int n = matrix.size();

    // Прямий хід (елімінація)
    for (int i = 0; i < n; i++) {
        // Знаходимо максимальний елемент в поточному стовпці
        double maxVal = findMaxInColumn(matrix, i, i);

        // Пошук рядка з максимальним елементом та обмін з поточним рядком
        for (int j = i; j < n; j++) {
            if (abs(matrix[j][i]) == maxVal) {
                swapRows(matrix, i, j);
                break;
            }
        }

        // Після обміну зробимо головний елемент 1
        double pivot = matrix[i][i];
        for (int j = i; j < n + 1; j++) {
            matrix[i][j] /= pivot;
        }

        // Віднімання поточного рядка від інших рядків для створення нулів під головним елементом
        for (int j = i + 1; j < n; j++) {
            double factor = matrix[j][i];
            for (int k = i; k < n + 1; k++) {
                matrix[j][k] -= factor * matrix[i][k];
            }
        }
    }

    // Зворотній хід (знаходження розв'язку)
    vector<double> x(n); // Масив для збереження розв'язку
    for (int i = n - 1; i >= 0; i--) {
        x[i] = matrix[i][n]; // Початкове наближення розв'язку

        // Віднімання відомих значень
        for (int j = i + 1; j < n; j++) {
            x[i] -= matrix[i][j] * x[j];
        }
    }

    return x;
}

int main() {
    // Масив для збереження імен файлів з вхідними даними
    string inputFiles[] = {"/Users/nikmas/Desktop/input1.txt", "/Users/nikmas/Desktop/input2.txt", "/Users/nikmas/Desktop/input3.txt", "/Users/nikmas/Desktop/input4.txt"};

    // Відкрити файл для запису результатів
    ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        cerr << "Помилка: Неможливо відкрити файл " << outputFilePath << " для запису результатів." << endl;
        return 1;
    }

    for (int example = 0; example < 4; example++) {
        // Відкрити файл для зчитування вхідних даних
        ifstream inputFile(inputFiles[example]);
        if (!inputFile.is_open()) {
            cerr << "Помилка: Неможливо відкрити файл " << inputFiles[example] << " для зчитування вхідних даних." << endl;
            return 1;
        }

        int n; // Розмір матриці
        inputFile >> n; // Зчитати розмір матриці з файлу
        vector<vector<double>> matrix(n, vector<double>(n + 1));

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                inputFile >> matrix[i][j];
            }
        }

        cout << "Вхідні дані для прикладу " << example + 1 << ":" << endl;
        // Вивести матрицю на консоль
        printMatrix(matrix);

        double determinant = calculateDeterminant(matrix);

        if (determinant != 0.0) {
            vector<double> solution = gaussianElimination(matrix);

            if (!solution.empty()) {
                cout << "\nРезультати обчислення для прикладу " << example + 1 << ":" << endl;
                cout << "Визначник: " << determinant << endl;
                cout << "Розв'язок системи:" << endl;
                for (int i = 0; i < solution.size(); i++) {
                    cout << "x" << i + 1 << " = " << solution[i] << endl;
                }
                double error = calculateError(matrix, solution);
                cout << "Похибка: " << error << endl;

                // Записати результати у файл
                outputFile << "\nРезультати обчислення для прикладу " << example + 1 << ":" << endl;
                outputFile << "Визначник: " << determinant << endl;
                outputFile << "Розв'язок системи:" << endl;
                for (int i = 0; i < solution.size(); i++) {
                    outputFile << "x" << i + 1 << " = " << solution[i] << endl;
                }
                outputFile << "Похибка: " << error << endl;
            } else {
                cout << "\nМатриця в прикладі " << example + 1 << " вироджена. Розв'язків не існує." << endl;

                // Записати повідомлення у файл, що матриця вироджена
                outputFile << "\nМатриця в прикладі " << example + 1 << " вироджена. Розв'язків не існує." << endl;
            }
        } else {
            cout << "\nМатриця в прикладі " << example + 1 << " вироджена. Розв'язків не існує." << endl;

            // Записати повідомлення у файл, що матриця вироджена
            outputFile << "\nМатриця в прикладі " << example + 1 << " вироджена. Розв'язків не існує." << endl;
        }

        // Закрити файл з вхідними даними
        inputFile.close();
    }

    // Закрити файл з результатами
    outputFile.close();

    // Відкрити файл з результатами на macOS
    system(("open " + outputFilePath).c_str());

    return 0;
}
