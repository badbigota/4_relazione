#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "statistica_viscosimetro.h"
using namespace std;

struct viscosimetro
{
	vector<double> misura;
	vector<double> delta_misura;
	vector<double> err;
	vector<double> err_delta;

	vector<double> misura_mark1;
	vector<double> misura_mark2;
	vector<double> misura_mark3;
	vector<double> misura_mark4;
	vector<double> misura_mark5;
	vector<double> delta_misura_mark1;
	vector<double> delta_misura_mark2;
	vector<double> delta_misura_mark3;
	vector<double> delta_misura_mark4;
	vector<double> delta_misura_mark5;

	vector<double> misura_tom;
	vector<double> misura_fab;
	vector<double> misura_mark;
	vector<double> delta_misura_tom;
	vector<double> delta_misura_fab;
	vector<double> delta_misura_mark;
	double ptl_frame_in_sec;
	vector<double> err_delta_misura_mark;
	vector<double> err_delta_misura_tom;
	vector<double> err_delta_misura_fab;
};

int main()
{
	double p;
	vector<viscosimetro> v(11);
	const double fps = 29.97;
	double durata_frame = 1.0 / fps;

	//LETTURA
	for (int k = 0; k < 11; k++)
	{ //scorrimento viscosimetri
		if ((k == 2) || (k == 6) || (k == 7))
		{ //misure cronometro
			for (int j = 1; j <= 5; j++)
			{
				ifstream fin_crono("../Dati/Singolo/" + to_string(k + 1) + "/" + to_string(k + 1) + "_" + to_string(j) + ".txt");
				if (!fin_crono)
				{
					cout << "Errore lettura file " << k << endl;
					return 1;
				}
				int n = 0;
				while (fin_crono >> p)
				{
					if (j == 1)
					{
						v[k].misura_mark1.push_back(p);
						if (n != 0)
						{
							v[k].delta_misura_mark1.push_back(p - v[k].misura_mark1[n - 1]);
						}
					}
					if (j == 2)
					{
						v[k].misura_mark2.push_back(p);
						if (n != 0)
						{
							v[k].delta_misura_mark2.push_back(p - v[k].misura_mark2[n - 1]);
						}
					}
					if (j == 3)
					{
						v[k].misura_mark3.push_back(p);
						if (n != 0)
						{
							v[k].delta_misura_mark3.push_back(p - v[k].misura_mark3[n - 1]);
						}
					}
					if (j == 4)
					{
						v[k].misura_mark4.push_back(p);
						if (n != 0)
						{
							v[k].delta_misura_mark4.push_back(p - v[k].misura_mark4[n - 1]);
						}
					}
					if (j == 5)
					{
						v[k].misura_mark5.push_back(p);
						if (n != 0)
						{
							v[k].delta_misura_mark5.push_back(p - v[k].misura_mark5[n - 1]);
						}
					}
					n++;
				}
			}
		}

		if ((k == 0) || (k == 1) || (k == 3) || (k == 4) || (k == 9) || (k == 10))
		{ //misure frame by frame
			ifstream fin_frame("../Dati/Singolo/" + to_string(k + 1) + ".txt");
			while (fin_frame >> p)
			{
				int n = 0;
				v[k].misura.push_back(p);
				if (n != 0)
				{
					v[k].delta_misura.push_back(p - v[k].delta_misura[n - 1]);
				}
				n++;
			}
		}

		if ((k == 5))
		{ //viscosimetro 6 (3 misure)
			//l'errore della singola misura è di 3 frame, usiamo la triangolare su 3 frame
			double frame_incertezza = 3.0;

			ifstream fin_6_tom("../Dati/Multiplo/tom/" + to_string(k + 1) + ".txt");
			ifstream fin_6_fab("../Dati/Multiplo/fab/" + to_string(k + 1) + ".txt");

			while (fin_6_tom >> p) //Legge i file grezzi per tom e fab e archivia i dati nei vettori del 6 viscos.
			{
				v[k].misura_tom.push_back(p);
			}
			while (fin_6_fab >> p)
			{
				v[k].misura_fab.push_back(p);
			}

			for (int i = 0; i < v[k].misura_tom.size() - 1; i++)
			{																				  //one-less-the-end per fare anche l'ultimo
				v[k].delta_misura_tom.push_back(v[k].misura_tom[i + 1] - v[k].misura_tom[i]); //Aggiunge i delta tempi
				v[k].err_delta_misura_tom.push_back(sigma_dist_tri(frame_incertezza * durata_frame, 1));
			}
			for (int i = 0; i < v[k].misura_fab.size() - 1; i++)
			{																				  //one-past-the-end per fare anche l'ultimo
				v[k].delta_misura_fab.push_back(v[k].misura_fab[i + 1] - v[k].misura_fab[i]); //Aggiunge i delta tempi
				v[k].err_delta_misura_fab.push_back(sigma_dist_tri(frame_incertezza * durata_frame, 1));
			}

			for (int i = 1; i <= 5; i++) //Legge tutti i file di mark e li archivia, archivia anche i delta t
			{
				ifstream fin_6_mark("../Dati/Multiplo/mar/6_" + to_string(i) + ".txt");
				while (fin_6_mark >> p)
				{
					if (i == 1)
					{
						v[k].misura_mark1.push_back(p);
					}
					if (i == 2)
					{
						v[k].misura_mark2.push_back(p);
					}
					if (i == 3)
					{
						v[k].misura_mark3.push_back(p);
					}
					if (i == 4)
					{
						v[k].misura_mark4.push_back(p);
					}
					if (i == 5)
					{
						v[k].misura_mark5.push_back(p);
					}
				}
			}

			for (int i = 1; i < v[k].misura_mark1.size(); i++)
			{
				v[k].delta_misura_mark1.push_back(v[k].misura_mark1[i] - v[k].misura_mark1[i - 1]);
				v[k].delta_misura_mark2.push_back(v[k].misura_mark2[i] - v[k].misura_mark2[i - 1]);
				v[k].delta_misura_mark3.push_back(v[k].misura_mark3[i] - v[k].misura_mark3[i - 1]);
				v[k].delta_misura_mark4.push_back(v[k].misura_mark4[i] - v[k].misura_mark4[i - 1]);
				v[k].delta_misura_mark5.push_back(v[k].misura_mark5[i] - v[k].misura_mark5[i - 1]);
			}

			//Archivia le medie e le dstd medie per tutti i 5 file di mark
			vector<double> righe;
			for (int i = 0; i < v[k].delta_misura_mark1.size(); i++)
			{
				righe.push_back(v[k].delta_misura_mark1[i]);
				righe.push_back(v[k].delta_misura_mark2[i]);
				righe.push_back(v[k].delta_misura_mark3[i]);
				righe.push_back(v[k].delta_misura_mark4[i]);
				righe.push_back(v[k].delta_misura_mark5[i]);
				v[k].delta_misura_mark.push_back(media(righe));
				v[k].err_delta_misura_mark.push_back(dstd_media(righe)); //Attenzione a vedere se sono più piccole di err di 2 frame
			}
			//creo un vettore di errrori per il viscosimetro 6. Uso la triangolare su 3 frame

			ofstream fout_6("6_viscosimetro.txt");
			if (!fout_6)
			{
				cout << "Errore scrittura set 6" << endl;
				return 1;
			}
			fout_6 << "#Num\tDelta_tom\t#Err_tom\t#Delta_fab\t#Err_fab\t#Delta_mark\t#Err_mark" << endl;
			for (int i = 0; i < v[k].delta_misura_tom.size(); i++)
			{
				fout_6 << i+1 <<"\t"<< v[k].delta_misura_tom[i] << "\t" << v[k].err_delta_misura_tom[i] << "\t" << v[k].delta_misura_fab[i] << "\t" << v[k].err_delta_misura_fab[i] << "\t" << v[k].delta_misura_mark[i] << "\t" << v[k].err_delta_misura_mark[i] << endl;
			}
		}
		if ((k == 8))
		{ //viscosimetro 9 (2 misure fbf)
			ifstream fin_9_fab("../Dati/Multiplo/fab/9.txt");
			while (fin_9_fab >> p)
			{
				v[k].misura_fab.push_back(p);
				for (int i = 0; i < v[k].misura_fab.size() - 1; i++)
				{
					v[k].delta_misura_fab.push_back(v[k].misura_fab[i + 1] - v[k].misura_fab[i]);
				}
			}
		
			ifstream fin_9_tom("../Dati/Multiplo/tom/9.txt");
			while (fin_9_tom >> p)
			{
				v[k].misura_tom.push_back(p);
				for (int i = 0; i < v[k].misura_tom.size() - 1; i++)
				{
					v[k].delta_misura_tom.push_back(v[k].misura_tom[i + 1] - v[k].misura_tom[i]);
				}
			}
		}
	}
	
	//ANALISI
	//Cronometro
	for (int k = 1; k <= 11; k++)
	{ //scorrimento viscosimetri
		if ((k == 3) || (k == 7) || (k == 8))
		{
			for (int i = 0; i < v[k].misura_mark1.size(); i++)
			{
				vector<double> righe;
				righe.push_back(v[k].misura_mark1[i]);
				righe.push_back(v[k].misura_mark2[i]);
				righe.push_back(v[k].misura_mark3[i]);
				righe.push_back(v[k].misura_mark4[i]);
				righe.push_back(v[k].misura_mark5[i]);
				v[k].misura.push_back(media(righe));
				v[k].err.push_back(dstd_media(righe));
			}
		}
		if ((k == 1) || (k == 2) || (k == 4) || (k == 5) || (k == 10) || (k == 11))
		{
		}
		if (k == 6)
		{
		}
		if (k == 9)
		{
		}
	}
	return 0;
}
