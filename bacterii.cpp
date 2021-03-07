//Polynomial Fit
#include<iostream>
#include<iomanip>
#include<cmath>
#include <vector>
#include <fstream>
#include <math.h>

using namespace std;


class Bacterie
{
public:

    void setVal()
    {
        double v1, v2;
        string fisier = "bacterii.txt";
        ifstream mf1(fisier);
        try
        {
            if (mf1.is_open())
            {
                for (int i = 0; i < 10; i++)
                {
                    mf1 >> v1;
                    mf1 >> v2;
                    tp.push_back(v1);
                    val.push_back(v2);
                }

            }
            else
            {
                throw fisier;
            }
        }
        catch (string fisier)
        {
            cout << "Nu exista fisierul cu denumirea " << fisier << endl;
            exit(404);
        }
    }

    void getVal()
    {
        cout << "Valorile citite initial sunt:" << endl;
        cout << "timp\tvaloare\n";
        for (int i = 0; i < tp.size(); i++)
        {
            cout << tp.at(i) << " h\t10^" << val.at(i) << endl;
        }
    }

    void setCoef(vector<double> coeficienti)
    {
        this->coef = coeficienti;
    }

    void getCoef()
    {
        cout << "Coef sunt::" << endl;
        //cout << "timp\tvaloare\n";
        for (int i = 0; i < coef.size(); i++)
        {
            cout <<coef.at(i)  << endl;
        }
    }

    void Generare()
    {
        for (int i = 0; i < tp.size(); i++)
        {
            this->val_pred.push_back(coef.at(0)+coef.at(1)*tp.at(i)+coef.at(2)*tp.at(i)*tp.at(i));
        }
        val_pred.at(5) = val_pred.at(5) * 1.2;

    }

    void getGen()
    {
        cout << "Valorile generate sunt:\n" << endl;
        cout << "timp\t\tvaloare\n\n";
        for (int i = 0; i < tp.size(); i++)
        {
            cout << tp.at(i) << " h\t10^" << val_pred.at(i) << endl;
        }
    }

    void setErr()
    {
        for (int i = 0; i < tp.size(); i++)
        {
            if ((val.at(i) - val_pred.at(i)) / val.at(i) * 100 > 0)
            {
                err.push_back((val.at(i) - val_pred.at(i)) / val.at(i) * 100);
            }
            else
            {
                err.push_back((val.at(i) - val_pred.at(i)) / val.at(i) * (-100));
            }
        }
    }
    void getErr()
    {
        cout << "Erorile sunt:\n";
        for (int i = 0; i < err.size(); i++)
        {
            cout << err.at(i) << endl;
        }
    }
    void Afisare_finala()
    {
        ofstream myfile;
        myfile.open("rezultate.txt");
        myfile << "timp\tvaloare reala\tvaloare modelata\teroare\n";
        for (int i = 0; i < tp.size(); i++)
        {
            myfile << tp.at(i) << " h\t10^" << val.at(i) <<"\t\t10^"<<val_pred.at(i)<<"\t\t"<<err.at(i)<<"%"<< endl;
        }

    }
private:
    vector<double> tp;
    vector<double> val;
    vector<double> val_pred;
    vector<double> coef;
    vector<double> err;

};



vector<double> Predictie()
{
    string fisier = "bacterii.txt";
    ifstream mf1(fisier);
    try
    {
        if (!mf1.is_open())
        {
            throw fisier;
        }
    }
    catch (const string file)
    {
        cout << "Nu exista fisierul " << file;
        exit(404);
    }

    int i, j, k, n, N;
    cout.precision(4);
    cout.setf(ios::fixed);
    cout << "\nEnter the no. of data pairs to be entered:\n";
    N = 10;
    double* x, * y;
    y = (double*)malloc(N * sizeof(double));
    x = (double*)malloc(N * sizeof(double));
    cout << "\nEnter the x-axis values:\n";
    for (i = 0; i < N; i++)
    {
        mf1 >> x[i];
        mf1 >> y[i];
        cout << x[i] << " 10^"<<y[i]<<endl;
    }

    n = 2;
    double* X;
    X = (double*)malloc((2 * n + 1) * sizeof(double));
    for (i = 0; i < 2 * n + 1; i++)
    {
        X[i] = 0;
        for (j = 0; j < N; j++)
        {
            X[i] = X[i] + pow(x[j], i);
        }
    }
    double** B = (double**)malloc((n + 1) * sizeof(double*));
    for (int i = 0; i < (n + 1); i++)
    {
        B[i] = (double*)malloc((n + 2) * sizeof(double));
    }
    double* a;
    a = (double*)malloc((n + 1) * sizeof(double));

    for (i = 0; i <= n; i++)
    {
        for (j = 0; j <= n; j++)
            B[i][j] = X[i + j];
    }

    double* Y;
    Y = (double*)malloc((n + 1) * sizeof(double));
    for (i = 0; i < n + 1; i++)
    {
        Y[i] = 0;
        for (j = 0; j < N; j++)
        {
            Y[i] = Y[i] + pow(x[j], i) * y[j];
        }
    }
    for (i = 0; i <= n; i++)
    {
        B[i][n + 1] = Y[i];
    }
    n = n + 1;


    for (i = 0; i < n; i++)
    {
        for (k = i + 1; k < n; k++)
        {
            if (B[i][i] < B[k][i])
            {
                for (j = 0; j <= n; j++)
                {
                    double temp = B[i][j];
                    B[i][j] = B[k][j];
                    B[k][j] = temp;
                }
            }
        }
    }

    for (i = 0; i < n - 1; i++)
    {
        for (k = i + 1; k < n; k++)
        {
            double t = B[k][i] / B[i][i];
            for (j = 0; j <= n; j++)
            {
                B[k][j] = B[k][j] - t * B[i][j];
            }
        }
    }
    for (i = n - 1; i >= 0; i--)
    {
        a[i] = B[i][n];
        for (j = 0; j < n; j++)
        {
            if (j != i)
            {
                a[i] = a[i] - B[i][j] * a[j];
            }
        }
        a[i] = a[i] / B[i][i];
    }
    cout << "\nThe values of the coefficients are as follows:\n";
    for (i = 0; i < n; i++)
    {
        cout << "x^" << i << "=" << a[i] << endl;
    }
    cout << "\nHence the fitted Polynomial is given by:\ny=";
    for (i = 0; i < n; i++)
    {
        cout << " + (" << a[i] << ")" << "x^" << i;
    }
    cout << "\n";
    mf1.close();
    vector<double> coef;
    for (int i = 0; i < n; i++)
    {
        coef.push_back(a[i]);
    }
    return coef;
}

int main()
{
    Bacterie b;
    b.setVal();
    b.getVal();
    b.setCoef(Predictie());
    cout << endl;
    b.getCoef();
    b.Generare();
    cout << endl;
    b.getGen();
    b.setErr();
    cout << endl;
    b.getErr();
    b.Afisare_finala();
    //Predictie();
    return 0;
}
