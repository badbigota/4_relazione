#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
using namespace std;

//Media
double media(vector<double> dati, int inizio = 0, int fine = 0)
{
    if (fine != 0)
    {
        double media_, counter_parz = 0, sum_parz = 0;
        for (int i = inizio; i < fine; i++) // Attenzione a <, la funzione non tiene in considerazione l'ultimo elemento, come i vettori.
        {
            sum_parz = sum_parz + dati[i];
            counter_parz++;
        }
        media_ = sum_parz / counter_parz;
        return media_;
    }
    else if (fine == 0)
    {
        double media_, sum = 0;
        for (auto c : dati)
        {
            sum = sum + c;
        }
        media_ = sum / dati.size();
        return media_;
    }
}

//Deviazione Standard del CAMPIONE (N a denominatore)
double dev_stand_campione(vector<double> dati, int inizio = 0, int fine = 0)
{
    double squad;
    double media_camp;
    double somma_camp;
    int counter = 0;

    if (fine != 0)
    {
        media_camp = media(dati, inizio, fine);
        for (int j = inizio; j < fine; j++)
        {
            somma_camp = somma_camp + (media_camp - dati[j]) * (media_camp - dati[j]);
            counter++;
        }
        squad = sqrt(somma_camp / counter);
    }
    else if (fine == 0)
    {
        media_camp = media(dati);
        for (auto d : dati)
        {
            somma_camp = somma_camp + (media_camp - d) * (media_camp - d);
        }
        counter = dati.size();
        squad = sqrt(somma_camp / counter);
    }

    return squad;
}

//Deviazione standard della POPOLAZIONE (N-1 a denominatore)
double dstd(vector<double> dati, int inizio = 0, int fine = 0)
{
    double dstd_camp;
    double media_camp;
    double somma_camp;
    int counter = 0;
    media_camp = media(dati, inizio, fine);
    if (fine != 0)
    {
        for (int j = inizio; j < fine; j++)
        {
            somma_camp = somma_camp + (media_camp - dati[j]) * (media_camp - dati[j]);
            counter++;
        }
        dstd_camp = sqrt(somma_camp / (counter - 1));
    }
    else if (fine == 0)
    {
        for (auto c : dati)
        {
            somma_camp = somma_camp + (media_camp - c) * (media_camp - c);
        }
        counter = dati.size();
        dstd_camp = sqrt(somma_camp / (counter - 1));
    }
    return dstd_camp;
}

//Deviazione standard della media
double dstd_media(vector<double> dati, int inizio = 0, int fine = 0)
{
    double dstd_media_;
    if (fine != 0)
    {
        dstd_media_ = dstd(dati, inizio, fine) / sqrt(fine - inizio);
    }
    else if (fine == 0)
    {
        dstd_media_ = dstd(dati) / sqrt(dati.size());
    }
    return dstd_media_;
}

//Errore distribuzione triangolare
double sigma_dist_tri(double ptl, double coeff_aff)
{
    return abs(ptl / coeff_aff) / sqrt(24); //ptl con il coeff aff � doppio di err max
}

//Compatibilit� 
double comp(double a, double b, double sigma_a, double sigma_b)
{
    return abs(a - b) / sqrt(pow(sigma_a, 2) + pow(sigma_b, 2));
}


//Media ponderata con errori
double media_ponderata(vector<double> valori, vector<double> errori, int inizio = 0, int fine = 0)
{
    double num = 0, den = 0;
    if (fine == 0)
    {
        fine = valori.size();
    }
    if (valori.size() != errori.size())
    {
        cout << "dimensione di vettore valori non è uguale a quella di vettore errori" << endl;
        return 1;
    }
    for (int i = inizio; i < fine; i++)
    {
        num += valori[i] * pow((1 / errori[i]), 2);
        den += pow((1 / errori[i]), 2);
    }
    return num / den;
}

//Errore medio (media ponderata)
double errore_media_ponderata(vector<double> errori)
{
    double sum = 0;
    for (auto d : errori)
    {
        sum += pow((1 / d), 2);
    }
    return (1 / sqrt(sum));
}