#include <iostream>
#include <math.h>
#include <stdexcept>
#include <vector>

using std::abs;
using std::cbrt;
using std::pow;
using std::sqrt;

class Mat
{
public:
    double *items;
    unsigned int rows;
    unsigned int cols;

    double at(unsigned int i, unsigned int j)
    {
        if (i > this->rows || j > cols)
        {
            std::string errMsg = "Can't access Mat element at " + std::to_string(i) + " " + std::to_string(j);

            throw std::runtime_error(errMsg);
        }

        return this->items[i * cols + j];
    }

    void set(unsigned int i, unsigned int j, double val)
    {
        this->items[i * cols + j] = val;
    }

    bool is_min(unsigned i, unsigned j)
    {
        double cur = this->at(i, j);

        /**
         * Does not include diagonals for now
         */
        if (
            cur < this->at(i + 1, j) && cur < this->at(i, j + 1) && cur < this->at(i - 1, j) && cur < this->at(i, j - 1))
        {
            return true;
        }

        return false;
    }

    bool is_max(unsigned i, unsigned j)
    {
        double cur = this->at(i, j);

        /**
         * Does not include diagonals for now
         */
        if (
            cur > this->at(i + 1, j) && cur > this->at(i, j + 1) && cur > this->at(i - 1, j) && cur > this->at(i, j - 1))
        {
            return true;
        }

        return false;
    }

    Mat(unsigned int i, unsigned int j)
    {
        if (i == 0 || j == 0)
        {
            throw new std::runtime_error("Cannot instantiate empty matrix");
        }

        this->rows = i;
        this->cols = j;
        this->items = new double[i * j]();
    }

    ~Mat()
    {
        delete[] this->items;
        this->items = nullptr;
    }
};

class Extrema
{
public:
    double i;
    double j;

    Extrema(double i, double j)
    {
        this->i = i;
        this->j = j;
    }
};

double F(double x, double y)
{
    return sqrt(x) + cbrt(y) - (1 / (sqrt(x * x + y * y * y)));
}

int main()
{
    unsigned int n;
    double x1, x2, y1, y2;

    std::cout << "n = ";
    std::cin >> n;
    std::cout << "x1 x2 y1 y2 = ";
    std::cin >> x1 >> x2 >> y1 >> y2;

    double x_step = (x2 - x1) / (n - 1);
    double y_step = (y2 - y1) / (n - 1);

    Mat m(n, n); // Allocate on stack

    std::vector<Extrema> minimas = std::vector<Extrema>();
    std::vector<Extrema> maximas = std::vector<Extrema>();

    /**
     * Populate the matrix with F(x,y) values
     */
    for (int i = 0; i < m.rows; i++)
    {
        for (int j = 0; j < m.cols; j++)
        {
            double x = x1 + x_step * i;
            double y = y1 + y_step * j;

            m.set(i, j, F(x, y));
        }
    }

    for (int i = 0; i < m.rows; i++)
    {
        for (int j = 0; j < m.cols; j++)
        {
            if (m.is_min(i, j))
            {
                minimas.emplace_back(i, j);
            }
            else if (m.is_max(i, j))
            {
                maximas.emplace_back(i, j);
            }
        }
    }

    /**
     * Find cell distances
     */
    for (int min = 0, minlen = minimas.size(); min < minlen; min++)
    {
        for (int max = 0, maxlen = maximas.size(); max < maxlen; max++)
        {
            Extrema &minima = minimas.at(min);
            Extrema &maxima = maximas.at(max);

            int dist = abs(maxima.i - minima.i) + abs(maxima.j - minima.j);

            std::cout << dist << '\n';
        }
    }
};