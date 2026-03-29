#include <iostream>
#include <math.h>
#include <stdexcept>
#include <string>

using std::abs;
using std::cbrt;
using std::pow;
using std::sqrt;

template <typename T>
class DArray
{
private:
    int size;
    int capacity;
    T *arr;

public:
    void push_back(T item)
    {
        if (this->capacity == (this->size - 1))
        {
            this->capacity *= 2;
            T *newArr = new T[capacity * 2]();
            for (int i = 0; i < this->size; i++)
                newArr[i] = arr[i];
            delete[] arr;
            this->arr = newArr;
            newArr = nullptr;
        }

        this->arr[size] = item;
        this->size++;
    }

    int get_size()
    {
        return this->size;
    }

    T at(int index)
    {
        return this->arr[index];
    }

    DArray(int capacity) : size(0), capacity(capacity), arr(new T[capacity]()) {}
    ~DArray()
    {
        delete[] this->arr;
        this->arr = nullptr;
    }
};

struct MatElement
{
    double F_value;
    double x;
    double y;
    unsigned int i;
    unsigned int j;
};

class Mat
{
public:
    MatElement **items;
    unsigned int rows;
    unsigned int cols;

    MatElement *at(int i, int j)
    {
        if (i > this->rows || j > this->cols || i < 0 || j < 0)
        {
            std::string errMsg = "Can't access Mat element at " + std::to_string(i) + " " + std::to_string(j);

            throw std::runtime_error(errMsg);
        }

        return (this->items[i * cols + j]);
    }

    /**
     * Currently implementing check for isnan
     */
    bool exists(int i, int j)
    {
        return i < rows && j < cols && i >= 0 && j >= 0 && !isnan(at(i, j)->F_value);
    }

    void set(unsigned int i, unsigned int j, MatElement *el)
    {
        this->items[i * cols + j] = el;
    }

    bool is_min(unsigned i, unsigned j)
    {
        double cur = this->at(i, j)->F_value;

        if (std::isnan(cur))
            return false;

        double s1 = this->exists(i + 1, j) ? cur < this->at(i + 1, j)->F_value : true;
        double s2 = this->exists(i, j + 1) ? cur < this->at(i, j + 1)->F_value : true;
        double s3 = this->exists(i - 1, j) ? cur < this->at(i - 1, j)->F_value : true;
        double s4 = this->exists(i, j - 1) ? cur < this->at(i, j - 1)->F_value : true;

        /**
         * Does not include diagonals for now
         */
        if (
            s1 && s2 && s3 && s4)
        {
            return true;
        }

        return false;
    }

    bool is_max(unsigned i, unsigned j)
    {
        double cur = this->at(i, j)->F_value;

        if (std::isnan(cur))
            return false;

        double s1 = this->exists(i + 1, j) ? cur > this->at(i + 1, j)->F_value : true;
        double s2 = this->exists(i, j + 1) ? cur > this->at(i, j + 1)->F_value : true;
        double s3 = this->exists(i - 1, j) ? cur > this->at(i - 1, j)->F_value : true;
        double s4 = this->exists(i, j - 1) ? cur > this->at(i, j - 1)->F_value : true;

        /**
         * Does not include diagonals for now
         */
        if (
            s1 && s2 && s3 && s4)
        {
            return true;
        }

        return false;
    }

    void print()
    {
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                std::cout << this->at(i, j)->F_value << " ";
            }

            std::cout << '\n';
        }

        std::cout << std::endl;
    }

    Mat(unsigned int i, unsigned int j)
    {
        if (i == 0 || j == 0)
        {
            throw new std::runtime_error("Cannot instantiate empty matrix");
        }

        this->rows = i;
        this->cols = j;
        this->items = new MatElement *[i * j]();
    }

    ~Mat()
    {
        for (int i = 0, len = rows * cols; i < len; i++)
        {
            delete this->items[i];
            this->items[i] = nullptr;
        }
        delete[] items;
        this->items = nullptr;
    }
};

/**
 * @returns
 * NAN if (x^2 + y^3) is zero;
 * else double
 */
double F(double x, double y)
{
    return sqrt(x) + cbrt(y) - (1.0 / (sqrt(x * x + y * y * y)));
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

    Mat m(n, n);

    DArray<MatElement *> *minimas = new DArray<MatElement *>(1024);
    DArray<MatElement *> *maximas = new DArray<MatElement *>(1024);

    /**
     * 1) Populate the matrix with F(x,y) values
     */
    for (unsigned int i = 0; i < m.rows; i++)
    {
        for (unsigned int j = 0; j < m.cols; j++)
        {
            double x = x1 + x_step * i;
            double y = y1 + y_step * j;

            m.set(i, j, new MatElement{F(x, y), x, y, i + 1, j + 1});
        }
    }

    /**
     * 2) Find minimas & maximas
     */
    for (int i = 0; i < m.rows; i++)
    {
        for (int j = 0; j < m.cols; j++)
        {
            if (m.is_min(i, j))
            {
                minimas->push_back(m.at(i, j));
            }
            else if (m.is_max(i, j))
            {
                maximas->push_back(m.at(i, j));
            }
        }
    }

    /**
     * 3) Find cell distances between minimas and maximas
     */
    for (int min = 0, minlen = minimas->get_size(); min < minlen; min++)
    {
        for (int max = 0, maxlen = maximas->get_size(); max < maxlen; max++)
        {
            MatElement *minima = minimas->at(min);
            MatElement *maxima = maximas->at(max);

            int dist = abs(maxima->i - minima->i) + abs(maxima->j - minima->j);

            std::cout << dist << '\n';
        }
    }

    m.print();

    std::cout << "MINIMAS" << "\n";
    for (int i = 0; i < minimas->get_size(); i++)
    {
        MatElement *e = minimas->at(i);
        std::cout << std::string("(" + std::to_string(e->i) + "," + std::to_string(e->j) + ")") << " ";
    }
    std::cout << "\n";

    std::cout << "MAXIMAS" << "\n";
    for (int i = 0; i < maximas->get_size(); i++)
    {
        MatElement *e = maximas->at(i);
        std::cout << std::string("(" + std::to_string(e->i) + "," + std::to_string(e->j) + ")") << " ";
    }
    std::cout << "\n";

    delete minimas;
    delete maximas;
};