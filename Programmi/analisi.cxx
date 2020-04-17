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
	vector<double> err_misura;
	vector<double> err_delta_misura;

	vector<double> misura_mark1; //dati grezzi
	vector<double> misura_mark2; //dati grezzi
	vector<double> misura_mark3; //dati grezzi
	vector<double> misura_mark4; //dati grezzi
	vector<double> misura_mark5; //dati grezzi

	vector<double> misura_tom;	//dati grezzi
	vector<double> misura_fab;	//dati grezzi
	vector<double> misura_mark; //dati grezzi

	vector<double> misura_media_mark;			//primo metodo, medie 5 tempi per ciascuna tacca
	vector<double> err_misura_media_mark;		//
	vector<double> delta_mark_primo_metodo;		//calcolo tramite differenza
	vector<double> err_delta_mark_primo_metodo; //propagazione

	vector<double> delta_mark1; //secondo metodo
	vector<double> delta_mark2; //secondo metodo
	vector<double> delta_mark3; //secondo metodo
	vector<double> delta_mark4; //secondo metodo
	vector<double> delta_mark5; //secondo metodo

	vector<double> delta_mark_secondo_metodo;	  //calcolo tramite media
	vector<double> err_delta_mark_secondo_metodo; //errorre tramite dstd media

	//vector<double> err_mark;

	vector<double> delta_misura_tom;	 //servono per il multiplo
	vector<double> delta_misura_fab;	 //servono per il multiplo
	vector<double> err_delta_misura_tom; //servono per il multiplo
	vector<double> err_delta_misura_fab; //servono per il multiplo

	vector<double> err_misura_fab; //contengono gli errori dei viscosimetri multipli di fab e tom
	vector<double> err_misura_tom; //contengono gli errori dei viscosimetri multipli di fab e tom

	//double ptl_frame_in_sec;
};

int main()
{
	double p;
	vector<viscosimetro> v(11);
	const double fps = 29.97;
	double durata_frame = 1.0 / fps;														   //in secondi bisogna convertire in millisec
	vector<double> frame_incertezza = {9.0, 8.0, 4.0, 4.0, 3.0, 3.0, 2.0, 3.0, 0.0, 2.0, 2.0}; //frame di incertezza per ciascun visco

	//LETTURA
	for (int k = 0; k < v.size(); k++)
	{										  //INIZIO PROCESSO LETTURA DATI GREZZI
		if ((k == 2) || (k == 6) || (k == 7)) //sono 3 7 8 perchè botdello con k (abbiamo contato -1)
		{									  //misure cronometro
			for (int j = 1; j <= 5; j++)
			{
				ifstream fin_crono("../Dati/Singolo/" + to_string(k + 1) + "/" + to_string(k + 1) + "_" + to_string(j) + ".txt");
				if (!fin_crono)
				{
					cout << "Errore lettura file " << k << endl;
					return 1;
				}
				while (fin_crono >> p) //legge e archivia in misura_mark_iesima il contenuto dei file
				{
					if (j == 1)
					{
						v[k].misura_mark1.push_back(p);
					}
					if (j == 2)
					{
						v[k].misura_mark2.push_back(p);
					}
					if (j == 3)
					{
						v[k].misura_mark3.push_back(p);
					}
					if (j == 4)
					{
						v[k].misura_mark4.push_back(p);
					}
					if (j == 5)
					{
						v[k].misura_mark5.push_back(p);
					}
				}
			} //riempie e BASTA
		}

		if ((k == 0) || (k == 1) || (k == 3) || (k == 4) || (k == 9) || (k == 10)) //sono i viscosimetri di Fabio e Di Tommaso
		{
			ifstream fin_frame("../Dati/Singolo/" + to_string(k + 1) + ".txt");
			while (fin_frame >> p)
			{
				v[k].misura.push_back(p);
			}
		} //legge e BASTA

		if ((k == 5)) //viscosimetro 6 (3 misure)
		{
			//l'errore della singola misura è di 3 frame, usiamo la triangolare su 3 frame
			//double frame_incertezza = 3.0; //varia da viscosimetro a viscosimetro

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
			//legge e BASTA quelli di Fabio e di Tom

			for (int i = 1; i <= 5; i++) //Legge tutti i file di mark e li archivia
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
			//questo è per il grafico
			//ofstream fout_6("6_viscosimetro.txt");
			//if (!fout_6)
			//{
			//	cout << "Errore scrittura set 6" << endl;
			//	return 1;
			//}
			//fout_6 << "#Num\tDelta_tom\t#Err_tom\t#Delta_fab\t#Err_fab\t#Delta_mark\t#Err_mark" << endl;
			//for (int i = 0; i < v[k].delta_misura_tom.size(); i++)
			//{
			//	fout_6 << i + 1 << "\t" << v[k].delta_misura_tom[i] << "\t" << v[k].err_delta_misura_tom[i] << "\t" << v[k].delta_misura_fab[i] << "\t" << v[k].err_delta_misura_fab[i] << "\t" << v[k].delta_misura_mark[i] << "\t" << v[k].err_delta_misura_mark[i] << endl;
			//}
		}

		if ((k == 8))
		{ //viscosimetro 9, sono solo le misure di Fabio e di Tommaso per il metodo frame by frame
			ifstream fin_9_fab("../Dati/Multiplo/fab/9.txt");
			ifstream fin_9_tom("../Dati/Multiplo/tom/9.txt");

			while (fin_9_fab >> p)
			{
				v[k].misura_fab.push_back(p);
			}
			while (fin_9_tom >> p)
			{
				v[k].misura_tom.push_back(p);
			}
		}
	}
	//FINE PROCESSO LETTURA DATI GREZZI







	//INIZIO PROCESSO CALCOLO DI DELTA T IN DUE MODI PER VISCOS MARC

	//PRIMO METODO

	for (int k = 0; k < v.size(); k++)
	{

		//leggo solo I SILGOLI Di mark (3,6,7,8)
		if ((k == 2) || (k == 5) || (k == 6) || (k == 7))
		{
			for (int i = 0; i < v[k].misura_mark1.size(); i++) //calcolo di tempi medi dai 5 tempi misurati
			{
				vector<double> righe;
				righe.push_back(v[k].misura_mark1[i]);
				righe.push_back(v[k].misura_mark2[i]);
				righe.push_back(v[k].misura_mark3[i]);
				righe.push_back(v[k].misura_mark4[i]);
				righe.push_back(v[k].misura_mark5[i]);
				v[k].misura_media_mark.push_back(media(righe));
				v[k].err_misura_media_mark.push_back(dstd_media(righe));
			}

			for (int i = 0; i < v[k].misura_media_mark.size() - 1; i++) //calcolo di delta con differenza e di errore con propagazione
			{
				v[k].delta_mark_primo_metodo.push_back(v[k].misura_media_mark[i + 1] - v[k].misura_media_mark[i]);
				v[k].err_delta_mark_primo_metodo.push_back(sqrt(pow(v[k].err_misura_media_mark[i + 1], 2) + pow(v[k].err_misura_media_mark[i], 2)));
			}
		}
	}
	//FINE PRIMO METODO

	//INIZIO SECONDO METODO
	for (int k = 0; k < v.size(); k++)
	{
		if ((k == 2) || (k == 5) || (k == 6) || (k == 7))
		{
			for (int i = 0; i < v[k].misura_mark1.size() - 1; i++)
			{
				v[k].delta_mark1.push_back(v[k].misura_mark1[i + 1] - v[k].misura_mark1[i]); //calcolo dei delta t
				v[k].delta_mark2.push_back(v[k].misura_mark2[i + 1] - v[k].misura_mark2[i]);
				v[k].delta_mark3.push_back(v[k].misura_mark3[i + 1] - v[k].misura_mark3[i]);
				v[k].delta_mark4.push_back(v[k].misura_mark4[i + 1] - v[k].misura_mark4[i]);
				v[k].delta_mark5.push_back(v[k].misura_mark5[i + 1] - v[k].misura_mark5[i]);

				vector<double> righe; //preparazione alla media delta t
				righe.push_back(v[k].delta_mark1[i]);
				righe.push_back(v[k].delta_mark2[i]);
				righe.push_back(v[k].delta_mark3[i]);
				righe.push_back(v[k].delta_mark4[i]);
				righe.push_back(v[k].delta_mark5[i]);
				v[k].delta_mark_secondo_metodo.push_back(media(righe));
				v[k].err_delta_mark_secondo_metodo.push_back(dstd_media(righe)); //ATTENZIONE CHE QUI NON SIA UN ALTRA STIMA DA USARE
																				 //TIPO LA MEDIA DELLE VARIANZE
			}
		}
	}
	//FINE SECONDO METODO

	//AGGIUNTA ERRORI DIST TRIANG SU VISCOSIMETRI ANALIZZATI CON FBF
	for (int k = 0; k < v.size(); k++)
	{

		if ((k == 0) || (k == 1) || (k == 3) || (k == 4) || (k == 9) || (k == 10))
		{
			for (int i = 0; i < v[k].misura.size(); i++) //per i non speciali singoli
			{
				v[k].err_misura.push_back(sigma_dist_tri(frame_incertezza[k] / (fps * 1000.0), 1)); //occhio al 1000 per la trasformazione in millisecondi
			}
		}
		else if ((k == 5)) //caso specifico per visco analizzati da tom e fab ma che non hanno le misre in vector misura
		{
			for (int i = 0; i < v[k].misura_fab.size(); i++)
			{
				v[k].err_misura_fab.push_back(sigma_dist_tri(frame_incertezza[k] / (fps * 1000.0), 1)); //occhio al 1000 per la trasformazione in millisecondi
				v[k].err_misura_tom.push_back(sigma_dist_tri(frame_incertezza[k] / (fps * 1000.0), 1)); //occhio al 1000 per la trasformazione in millisecondi
			}
		}
		else if ((k == 8))
		{
			//si è fatta la media alla 2 tacca, alla 8, alla 9 , alla 10 , alla 11.
			for (int i = 0; i < v[k].misura_fab.size(); i++)
			{

				if ((i == 1) || (i == 7) || (i == 8) || (i == 9) || (i == 10)) //se siamo in queste misure, avendo fatto la media fra i frame, si usa la propagazione
				{
					v[k].err_misura_fab.push_back(sqrt(2) * sigma_dist_tri(frame_incertezza[k] / (fps * 1000.0), 1));
					v[k].err_misura_tom.push_back(sqrt(2) * sigma_dist_tri(frame_incertezza[k] / (fps * 1000.0), 1));
				}
				else
				{
					v[k].err_misura_fab.push_back(sigma_dist_tri(frame_incertezza[k] / (fps * 1000.0), 1));
					v[k].err_misura_tom.push_back(sigma_dist_tri(frame_incertezza[k] / (fps * 1000.0), 1));
				}
			}
		}
	}
	//FINE AGGIUNTA ERRORI SPECIALI

	//CALCOLO DI DELTA ED ERR PER TUTTI i viscosimetri non speciali con gli errori dovuti all'incertezza sui frame
	for (int k = 0; k < v.size(); k++)
	{
		if ((k == 0) || (k == 1) || (k == 3) || (k == 4) || (k == 9) || (k == 10)) //per i viscosimetri dei singoli
		{
			for (int i = 0; i < v[k].misura.size() - 1; i++)
			{
				v[k].delta_misura.push_back(v[k].misura[i + 1] - v[k].misura[i]);
				v[k].err_delta_misura.push_back(sqrt(pow(v[k].err_misura[i + 1], 2) + pow(v[k].err_misura[i], 2)));
			}
		}
		else if ((k == 5) || (k == 8)) //per i visc condivisi
		{
			for (int i = 0; i < v[k].misura_fab.size() - 1; i++)
			{
				v[k].delta_misura_fab.push_back(v[k].misura_fab[i + 1] - v[k].misura_fab[i]);
				v[k].delta_misura_tom.push_back(v[k].misura_tom[i + 1] - v[k].misura_tom[i]);
				v[k].err_delta_misura_fab.push_back(sqrt(pow(v[k].err_misura_fab[i + 1], 2) + pow(v[k].err_misura_fab[i], 2)));
				v[k].err_delta_misura_tom.push_back(sqrt(pow(v[k].err_misura_tom[i + 1], 2) + pow(v[k].err_misura_tom[i], 2)));
			}
		}
	}
	//FINE CALCOLO DELTA ED ERR PER VISC NON SPECIALI




	return 0;
}