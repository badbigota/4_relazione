#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "statistica_viscosimetro.h"
using namespace std;

double inch_to_mm(double inch);

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

	vector<double> misura_media_mark;	  //primo metodo, medie 5 tempi per ciascuna tacca
	vector<double> err_misura_media_mark; //
	//vector<double> delta_mark_primo_metodo;		//calcolo tramite differenza
	//vector<double> err_delta_mark_primo_metodo; //propagazione

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

	vector<double> asse_x; // è un vettore di media di tempi per l'asse delle x dei graficiù
	vector<double> velocity;
	vector<double> err_velocity;

	double test_chi_t_v;
	vector<double> tempi_passaggio; //serve per il metodo stima velocità tramite reciproco coeff ang
	vector<double> err_tempi_passaggio;

	//3 metodi per stima v lim
	double v_lim_interpolazione_t_x;
	double err_v_lim_interpolazione_t_x;
	double v_media_tutte;
	double err_v_media_tutte;
	double v_media_occhio;
	double err_v_media_occhio;

	//Stime viscosità
	double viscosity;
	double err_viscosity;
};

int main()
{
	double p;
	vector<viscosimetro> v(11);
	const double fps = 29.97;
	double durata_frame = 1.0 / fps;														   //in secondi bisogna convertire in millisec
	vector<double> frame_incertezza = {9.0, 8.0, 4.0, 4.0, 3.0, 3.0, 2.0, 3.0, 2.0, 2.0, 2.0}; //frame di incertezza per ciascun visco
	vector<double> comp_tf;
	vector<double> comp_tm;
	vector<double> comp_fm;
	double comp_tf_media, comp_tm_media, comp_fm_media;

	//INIZIO LETTURA DATI GREZZI
	for (int k = 0; k < v.size(); k++)
	{
		if ((k == 2) || (k == 6) || (k == 7)) //sono 3 7 8 perchè botdello con k (abbiamo contato -1)
		{									  //misure cronometro
			for (int j = 1; j <= 5; j++)
			{
				ifstream fin_crono("../Dati/Singolo/" + to_string(k + 1) + "/" + to_string(k + 1) + "_" + to_string(j) + ".txt");
				//ifstream fin_crono("Misure/" + to_string(k + 1) + "/" + to_string(k + 1) + "_" + to_string(j) + ".txt");
				if (!fin_crono)
				{
					cout << "Errore lettura file " << k + 1 << endl;
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
			//ifstream fin_frame("Misure/" + to_string(k + 1) + ".txt");
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
			//ifstream fin_6_tom("Misure/6/tom/6.txt");
			//ifstream fin_6_fab("Misure/6/fab/6.txt");

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
				//ifstream fin_6_mark("Misure/6/6_" + to_string(i) + ".txt");
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
		}

		if ((k == 8))
		{ //viscosimetro 9, sono solo le misure di Fabio e di Tommaso per il metodo frame by frame
			ifstream fin_9_fab("../Dati/Multiplo/fab/9.txt");
			ifstream fin_9_tom("../Dati/Multiplo/tom/9.txt");
			//ifstream fin_9_fab("Misure/9/fab/9.txt");
			//ifstream fin_9_tom("Misure/9/tom/9.txt");

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
	//FINE LETTURA

	//AGGIUNTA ERRORI DIST TRIANG SU VISCOSIMETRI ANALIZZATI CON FBF
	for (int k = 0; k < v.size(); k++)
	{

		if ((k == 0) || (k == 1) || (k == 3) || (k == 4) || (k == 9) || (k == 10))
		{
			for (int i = 0; i < v[k].misura.size(); i++) //per i non speciali singoli
			{
				v[k].err_misura.push_back(sigma_dist_tri((frame_incertezza[k] / fps * 1000.0), 1)); //occhio al 1000 per la trasformazione in millisecondi
			}
		}
		else if ((k == 5)) //caso specifico per visco analizzati da tom e fab ma che non hanno le misre in vector misura
		{
			for (int i = 0; i < v[k].misura_fab.size(); i++)
			{
				v[k].err_misura_fab.push_back(sigma_dist_tri((frame_incertezza[k] / fps * 1000.0), 1)); //occhio al 1000 per la trasformazione in millisecondi
				v[k].err_misura_tom.push_back(sigma_dist_tri((frame_incertezza[k] / fps * 1000.0), 1)); //occhio al 1000 per la trasformazione in millisecondi
			}
		}
		else if ((k == 8))
		{
			//si è fatta la media alla 2 tacca, alla 8, alla 9 , alla 10 , alla 11.
			for (int i = 0; i < v[k].misura_fab.size(); i++)
			{

				if ((i == 1) || (i == 7) || (i == 8) || (i == 9) || (i == 10)) //se siamo in queste misure, avendo fatto la media fra i frame, si usa la propagazione
				{
					v[k].err_misura_fab.push_back((1.0 / (sqrt(2))) * sigma_dist_tri((frame_incertezza[k] / fps * 1000.0), 1));
					v[k].err_misura_tom.push_back((1.0 / (sqrt(2))) * sigma_dist_tri((frame_incertezza[k] / fps * 1000.0), 1));
				}
				else
				{
					v[k].err_misura_fab.push_back(sigma_dist_tri((frame_incertezza[k] / fps * 1000.0), 1));
					v[k].err_misura_tom.push_back(sigma_dist_tri((frame_incertezza[k] / fps * 1000.0), 1));
				}
			}
		}
	}
	//FINE AGGIUNTA ERRORI SPECIALI

	//INIZIO PROCESSO CALCOLO DI DELTA T IN DUE MODI PER VISCOS MARC

	//PRIMO METODO OBSOLETO, MA LASCIARE PERCHÈ FORSE SERVE PER FARE DEI CONTI DOPO NELLA RELAZIONE
	for (int k = 0; k < v.size(); k++)
	{
		//leggo solo I SINGOLI Di mark (3,6,7,8)
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
		}
	}
	//FINE PRIMO METODO

	//INIZIO SECONDO METODO CIOÈ PER QUELLI DI MARK
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
				v[k].err_delta_mark_secondo_metodo.push_back(dstd_media(righe)); //ATTENZIONE CHE QUI NON SIA UN ALTRA STIMA DA USARE //TIPO LA MEDIA DELLE VARIANZE

				//Da decommentare quando sapremo il metodo corretto, evita di restituire le genriche anche per il 6
				if (k != 5)
				{
					v[k].delta_misura.push_back(media(righe));
					v[k].err_delta_misura.push_back(dstd_media(righe));
				}
			}
		}
	}
	//FINE SECONDO METODO

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
	ofstream fout_6("../Grafici/dati_6_sfera_t_f_m1_m2.txt");
	fout_6 << "#N\t#D_t [ms]\t#E_D_t [ms]\t#D_F [ms]\t#E_D_f [ms]\t#D_m2 [ms]\t#E_D_m2 [ms]" << endl;
	for (int i = 0; i < v[5].delta_misura_fab.size(); i++)
	{
		//fout_6 << i + 1 << "\t" << v[5].delta_misura_tom[i] << "\t" << v[5].err_delta_misura_tom[i] << "\t" << v[5].delta_misura_fab[i] << "\t" << v[5].err_delta_misura_fab[i] << "\t" << v[5].delta_mark_primo_metodo[i] << "\t" << v[5].err_delta_mark_primo_metodo[i] << "\t" << v[5].delta_mark_secondo_metodo[i] << "\t" << v[5].err_delta_mark_secondo_metodo[i] << endl;
		fout_6 << i + 1 << "\t" << v[5].delta_misura_tom[i] << "\t" << v[5].err_delta_misura_tom[i] << "\t" << v[5].delta_misura_fab[i] << "\t" << v[5].err_delta_misura_fab[i] << "\t" << v[5].delta_mark_secondo_metodo[i] << "\t" << v[5].err_delta_mark_secondo_metodo[i] << endl;
	}

	//CREAZIONE TABELLA DATI GREZZI DI TEMPI GREZZI
	ofstream fout_grezzi("dati_grezzi_aggregati_tempi.txt");
	fout_grezzi << "1  \t err_1 \t 2  \t err_2 \t 3  \t err_3 \t 4  \t err_4 \t 5  \t err_5 \t 6_1  \t err_6_1 \t 6_2  \t err_6_2 \t 6_3  \t err_6_3 \t 7  \t err_7 \t 8  \t err_8 \t 9_1  \t err_9_1 \t 9_2  \t err_9_2 \t 10  \t err_10 \t 11  \t err_11" << endl
				<< "[ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms] \t [ms]" << endl;
	for (int i = 0; i < v[0].misura.size(); i++)
	{
		fout_grezzi << v[0].misura[i] << "\t" << v[0].err_misura[i] << "\t"
					<< v[1].misura[i] << "\t" << v[1].err_misura[i] << "\t"
					<< v[2].misura_media_mark[i] << "\t" << v[2].err_misura_media_mark[i] << "\t"
					<< v[3].misura[i] << "\t" << v[3].err_misura[i] << "\t"
					<< v[4].misura[i] << "\t" << v[4].err_misura[i] << "\t"
					<< v[5].misura_tom[i] << "\t" << v[5].err_misura_tom[i] << "\t"
					<< v[5].misura_fab[i] << "\t" << v[5].err_misura_fab[i] << "\t"
					<< v[5].misura_media_mark[i] << "\t" << v[5].err_misura_media_mark[i] << "\t"
					<< v[6].misura_media_mark[i] << "\t" << v[6].err_misura_media_mark[i] << "\t"
					<< v[7].misura_media_mark[i] << "\t" << v[7].err_misura_media_mark[i] << "\t"
					<< v[8].misura_tom[i] << "\t" << v[8].err_misura_tom[i] << "\t"
					<< v[8].misura_fab[i] << "\t" << v[8].err_misura_fab[i] << "\t";
		if ((i == 1)) //CICOLO CHE STAMPA SOLO I VALORI DI 10 E 11 VISC IN CORRISPONDENZA DELLE GIUSTE TACCHE
		{			  //BUTTA FUORI SOLO AL MOMENTO GIUSTOs
			fout_grezzi << v[9].misura[0] << "\t" << v[9].err_misura[0] << "\t" << v[10].misura[0] << "\t" << v[10].err_misura[0] << "\t" << endl;
		}
		else if ((i == 10))
		{
			fout_grezzi << v[9].misura[1] << "\t" << v[9].err_misura[1] << "\t" << v[10].misura[1] << "\t" << v[10].err_misura[1] << "\t" << endl;
		}
		else
		{
			fout_grezzi << "ND\tND\tND\tND" << endl;
		}
	}
	//FINE CREAZIONE TABELLA DATI GREZZI

	//COMPATIBILITA' per visc condiviso tra tutti operatori
	for (int i = 0; i < v[5].delta_misura_tom.size(); i++)
	{
		comp_tf.push_back(comp(v[5].delta_misura_tom[i], v[5].delta_misura_fab[i], v[5].err_delta_misura_tom[i], v[5].err_delta_misura_fab[i]));
		comp_tm.push_back(comp(v[5].delta_misura_tom[i], v[5].delta_mark_secondo_metodo[i], v[5].err_delta_misura_tom[i], v[5].err_delta_mark_secondo_metodo[i]));
		comp_fm.push_back(comp(v[5].delta_mark_secondo_metodo[i], v[5].delta_misura_fab[i], v[5].err_delta_mark_secondo_metodo[i], v[5].err_delta_misura_fab[i]));
	}
	cout << "Compatibilita tom - fab" << endl;
	for (auto c : comp_tf)
	{
		cout << c << endl;
	}
	cout << endl
		 << "Compatibilita tom - mark" << endl;
	for (auto d : comp_tm)
	{
		cout << d << endl;
	}
	cout << endl
		 << "Compatibilita fab - mark" << endl;
	for (auto e : comp_fm)
	{
		cout << e << endl;
	}
	cout << endl;
	//Compatibilit� sulla media dei delta t
	comp_tf_media = comp(media(v[5].delta_misura_tom), media(v[5].delta_misura_fab), dstd_media(v[5].delta_misura_tom), dstd_media(v[5].delta_misura_fab));
	comp_tm_media = comp(media(v[5].delta_misura_tom), media(v[5].delta_mark_secondo_metodo), dstd_media(v[5].delta_misura_tom), dstd_media(v[5].delta_mark_secondo_metodo));
	comp_fm_media = comp(media(v[5].delta_mark_secondo_metodo), media(v[5].delta_misura_fab), dstd_media(v[5].delta_mark_secondo_metodo), dstd_media(v[5].delta_misura_fab));
	cout << "Compatibilita tom - fab: " << comp_tf_media << endl;
	cout << "Compatibilita tom - mark: " << comp_tm_media << endl;
	cout << "Compatibilita fab - mark: " << comp_fm_media << endl;

	//VISCOSIMETRO 6 - PROCEDO CON L'ANALISI PER IL CONFRONTO DEL CALCOLO DI UN UNICO SET DI VALORI
	for (int i = 0; i < v[5].delta_misura_tom.size(); i++) //V[5] perchè stiamo considerando il viscosimetro numero 6
	{
		vector<double> righe;
		righe.push_back(v[5].delta_misura_tom[i]);
		righe.push_back(v[5].delta_misura_fab[i]);
		righe.push_back(v[5].delta_mark_secondo_metodo[i]); // cambia se vuoi il secondo metodo
		vector<double> err_righe;
		err_righe.push_back(v[5].err_delta_misura_tom[i]);
		err_righe.push_back(v[5].err_delta_misura_fab[i]);
		err_righe.push_back(v[5].err_delta_mark_secondo_metodo[i]); //cambia se vuoi il secondo metodo
		v[5].delta_misura.push_back(media_ponderata(righe, err_righe));
		v[5].err_delta_misura.push_back(errore_media_ponderata(err_righe));
	}

	//VISCOSIMETRO 9 - PROCEDO CON L'ANALISI PER IL CONFRONTO DEL CALCOLO DI UN UNICO SET DI VALORI
	for (int i = 0; i < v[8].delta_misura_tom.size(); i++) //V[8] perchè stiamo considerando il viscosimetro numero 9
	{
		vector<double> righe;
		righe.push_back(v[8].delta_misura_tom[i]);
		righe.push_back(v[8].delta_misura_fab[i]);
		vector<double> err_righe;
		err_righe.push_back(v[8].err_delta_misura_tom[i]);
		err_righe.push_back(v[8].err_delta_misura_fab[i]);
		v[8].delta_misura.push_back(media_ponderata(righe, err_righe));
		v[8].err_delta_misura.push_back(errore_media_ponderata(err_righe));
	}
	//FINE GENERAZIONE DELTA T E ERR DELTA T PER TUTTI I VISCOSIMETRI

	//CONTROLLO SU DELTA TEMPI PER TUTTI VISC
	for (int k = 0; k < v.size(); k++)
	{
		ofstream fout_de("../Grafici_Velocity/delta_t_" + to_string(k + 1) + ".txt");
		fout_de << "#Delta_t[ms]" << endl;
		for (int i = 0; i < v[k].delta_misura.size(); i++)
		{
			fout_de << v[k].delta_misura[i] << endl;
		}
	}
	//FINE CONTROLLO SU TEMPI VISC

	//INIZIO GENERAZIONE TEMPI PASSAGGIO, TUTTI CON 0
	for (int k = 0; k < v.size(); k++)
	{
		for (int i = 0; i < v[k].delta_misura.size() + 1; i++) //è definito per tutti, ora però aggiungi uno per recuperare la tacca perduta
		{
			if ((k == 8)) //condivisi tom fab
			{
				double tom_i = v[k].misura_tom[i] - v[k].misura_tom[0];
				double fab_i = v[k].misura_fab[i] - v[k].misura_fab[0];
				double err_tom_i = sqrt(pow(v[k].err_misura_tom[i], 2) + pow(v[k].err_misura_tom[0], 2));
				double err_fab_i = sqrt(pow(v[k].err_misura_fab[i], 2) + pow(v[k].err_misura_fab[0], 2));
				v[k].tempi_passaggio.push_back(media({tom_i, fab_i}));
				v[k].err_tempi_passaggio.push_back(0.5 * sqrt(pow(err_tom_i, 2) + pow(err_fab_i, 2)));
			}
			else if ((k == 5)) //condiviso con mark
			{
				double tom_i = v[k].misura_tom[i] - v[k].misura_tom[0];
				double fab_i = v[k].misura_fab[i] - v[k].misura_fab[0];
				double err_tom_i = sqrt(pow(v[k].err_misura_tom[i], 2) + pow(v[k].err_misura_tom[0], 2));
				double err_fab_i = sqrt(pow(v[k].err_misura_fab[i], 2) + pow(v[k].err_misura_fab[0], 2));
				v[k].tempi_passaggio.push_back(media({tom_i, fab_i, v[k].misura_media_mark[i]}));
				v[k].err_tempi_passaggio.push_back(1.0 / 3.0 * sqrt(pow(err_tom_i, 2) + pow(err_fab_i, 2) + pow(v[k].err_misura_media_mark[i], 2)));
			}
			else if ((k == 2) || (k == 6) || (k == 7)) //solo di mark
			{
				v[k].tempi_passaggio.push_back(v[k].misura_media_mark[i]); //grandezze già definite in ex primo metodo
				v[k].err_tempi_passaggio.push_back(v[k].err_misura_media_mark[i]);
			}
			else if ((k == 9) || (k == 10))
			{
				continue;
			}
			else //singolo
			{
				v[k].tempi_passaggio.push_back(v[k].misura[i] - v[k].misura[0]);
				v[k].err_tempi_passaggio.push_back(sqrt(pow(v[k].err_misura[0], 2) + pow(v[k].err_misura[i], 2))); //propagazione su errore
			}
		}
	}

	//FINE GENE TEMPI PASSAGGIO

	//INIZIO GENERAZIONE "T MEDI" ASSSE X GRAFICI VELOCITÀ
	for (int k = 0; k < v.size(); k++)
	{
		double somma_cumulativa = 0;
		for (int i = 0; i < v[k].delta_misura.size(); i++)
		{
			v[k].asse_x.push_back(somma_cumulativa + v[k].delta_misura[i] / 2);
			somma_cumulativa = somma_cumulativa + v[k].delta_misura[i];
		}
	}
	//FINE GENERAZIONE ASSE X GRAFICI VELOCITÀ

	//INIZIO MEDIA VARIANZE, solo per i delta v statisticamente indipendenti per il calcolo delle velocità, di quelli di marco
	for (int k = 0; k < v.size(); k++)
	{
		if ((k == 2) || (k == 6) || (k == 7))
		{
			double sum_variance = 0;
			for (int i = 0; i < v[k].err_delta_misura.size(); i++)
			{
				if (i % 2 == 0)
				{
					sum_variance += pow(v[k].err_delta_misura[i], 2);
				}
			}
			double media_var = sum_variance / 5;
		}
		if (k == 5)
		{
		}
	}
	//FINE MEDIA VARIANZE

	//INIZIO GENEREAZIONE VELOCITÀ
	double delta_x = 50.0;										//definita in millimetri
	double delta_x_10_e_11 = 450.0;								//definita in millimetri, unica delta t tra seconda e ultima tacca
	double err_delta_x = sqrt(2) * sigma_dist_tri(1.5875, 1.0); //DIPENDE DALLO SPESSORE DELLA TACCA E PARLLASSE E PROSPETTIVA

	for (int k = 0; k < v.size(); k++)
	{
		vector<double> dummy_for_chi;
		vector<double> err_dummy_for_chi;

		if ((k != 9) || (k != 10)) //IL CICLO È PER AVERE IL GIUSTO DELTA X
		{
			ofstream fout_vel("../Grafici_Velocity/visc_" + to_string(k + 1) + "_v.txt");
			fout_vel << "#T_intermedio[ms]\t#Vel_media[mm/ms]\t#err_vel_med[mm/ms]" << endl;
			for (int i = 0; i < v[k].delta_misura.size(); i++)
			{
				double err_vel = sqrt(pow((err_delta_x / v[k].delta_misura[i]), 2) + pow(delta_x * v[k].err_delta_misura[i] / pow(v[k].delta_misura[i], 2), 2));
				double vel = delta_x / v[k].delta_misura[i];
				if (i % 2 == 1) //PRENDO LE VELOCITÀ PARI
				{				//mette nel vettore solo le velocità indipendenti
					v[k].velocity.push_back(vel);
					v[k].err_velocity.push_back(err_vel);
				}
				fout_vel << v[k].asse_x[i] << "\t" << vel << "\t" << err_vel << endl;
				dummy_for_chi.push_back(vel);
				err_dummy_for_chi.push_back(err_vel);
				v[k].test_chi_t_v = test_chi(v[k].asse_x, dummy_for_chi, err_dummy_for_chi); //FA CHI QUADRO CON TUTTE LE VELOCITÀ
			}
			//cout<<v[k].test_chi_t_v<<endl;
		}
		if ((k == 9) || (k == 10)) //SOLO QUESTI DUE HANNO UN DELTA X MOLTO PIÙ GRANDE
		{
			ofstream fout_vel("../Grafici_Velocity/visc_" + to_string(k + 1) + "_v.txt");
			fout_vel << "#T_intermedio[ms]\t#Vel_media[mm/ms]\t#err_vel_med[mm/ms]" << endl;
			for (int i = 0; i < v[k].delta_misura.size(); i++)
			{
				double vel = delta_x_10_e_11 / v[k].delta_misura[i];
				double err_vel = sqrt(pow((err_delta_x / v[k].delta_misura[i]), 2) + pow(delta_x_10_e_11 * v[k].err_delta_misura[i] / pow(v[k].delta_misura[i], 2), 2));
				v[k].velocity.push_back(vel);
				v[k].err_velocity.push_back(err_vel);
				fout_vel << v[k].asse_x[i] << "\t" << vel << "\t" << err_vel << endl;
			}
		}
	}

	//FINE GENERAZIONE VELOCITÀ

	/******************************************************
	STIME DI V LIMITE
	******************************************************/

	// 1 metodo: interpolazione x vs t
	//GRAFICI SPAZIO (ASSE X) E TEMPO (ASSE Y)
	vector<double> pos_x = {25, 75, 125, 175, 225, 275, 325, 375, 425, 475, 525}; //millimetri
	v[2].err_tempi_passaggio[0] = media(v[2].err_tempi_passaggio);				  //perchè altrimenti non riesce il chi quadro e non so perchè
	v[6].err_tempi_passaggio[0] = media(v[2].err_tempi_passaggio);				  //perchè altrimenti non riesce il chi quadro
	v[7].err_tempi_passaggio[0] = media(v[2].err_tempi_passaggio);				  //perchè altrimenti non riesce il chi quadro
	for (int k = 0; k < v.size() - 2; k++)										  //eccetto gli ultimi due
	{
		ofstream fout_tom_v("../Grafici_Velocity/x_t_" + to_string(k + 1) + ".txt");
		for (int i = 0; i < v[k].tempi_passaggio.size(); i++)
		{
			fout_tom_v << pos_x[i] << "\t" << v[k].tempi_passaggio[i] << "\t" << v[k].err_tempi_passaggio[i] << endl;
		}
		v[k].v_lim_interpolazione_t_x = 1. / b_angolare(pos_x, v[k].tempi_passaggio, v[k].err_tempi_passaggio);
		double err_tempi = sqrt(pow(media(v[k].err_tempi_passaggio), 2) * pow(1. / b_angolare(pos_x, v[k].tempi_passaggio, v[k].err_tempi_passaggio), 4)); //propagazione errori su velocità
		v[k].err_v_lim_interpolazione_t_x = err_tempi;
	}
	//FINE GENERAZIONE GRAFICO SPAZIO TEMPO

	// 2 metodo: con tutte le pari o dispari
	//VELOCITA' LIMITE : STIMA CON MEDIA
	for (int k = 0; k < v.size(); k++)
	{
		if (k < 9) //eccetto gli utlimi due
		{
			v[k].v_media_tutte = media(v[k].velocity);
			v[k].err_v_media_tutte = dstd_media(v[k].velocity);
		}
		if ((k == 9) || (k == 10))
		{
			v[k].v_media_tutte = media(v[k].velocity);
			v[k].err_v_media_tutte = v[k].err_velocity[0];
		}
	}

	// 3 metodo: togliendo ad occhio quelle che non sono su retta, con quelle pari o dispari
	cout << endl
		 << endl;
	for (int k = 0; k < v.size(); k++)
	{
		if ((k == 0) || (k == 1)) //FACCIAMO LA REIEZIONE
		{
			v[k].velocity.erase(v[k].velocity.begin()); //tolgo il primo elemento
			v[k].v_media_occhio = media(v[k].velocity);
			v[k].err_v_media_occhio = dstd_media(v[k].velocity);
		}
		else
		{
			v[k].v_media_occhio = media(v[k].velocity); //non cambia un cazzo rispetto alla media di prima, ma serve almeno per avere vettori non vuoti
			v[k].err_v_media_occhio = dstd_media(v[k].velocity);
		}
	}

	//stampa per confronto ad occhio
	cout << "V_lim|\tErr|\tErr_%" << endl;
	for (int k = 0; k < v.size(); k++)
	{
		cout << k + 1 << "|\t" << v[k].v_media_tutte << "\t" << v[k].err_v_media_tutte << "\t" << v[k].err_v_media_tutte / v[k].v_media_tutte * 100 << "|\t" << v[k].v_media_occhio << "\t" << v[k].err_v_media_occhio << "\t" << v[k].err_v_media_occhio / v[k].v_media_occhio * 100;
		cout << "|\t" << v[k].v_lim_interpolazione_t_x << "\t" << v[k].err_v_lim_interpolazione_t_x << "\t" << v[k].err_v_lim_interpolazione_t_x / v[k].v_lim_interpolazione_t_x * 100 << endl;
	}

	//CALCOLO DI VISCOSITÀ
	vector<double> diametro = {1.5, inch_to_mm(2.0 / 32.0), 2.0, inch_to_mm(3.0 / 32.0), inch_to_mm(4.0 / 32.0), inch_to_mm(5.0 / 32.0), inch_to_mm(6.0 / 32.0), inch_to_mm(7.0 / 32.0), inch_to_mm(8.0 / 32.0), inch_to_mm(8.0 / 32.0), inch_to_mm(9.0 / 32.0)};
	double err_diametro = 0.01; //mm

	double dens_sap = 1.032; //g/cm^3
	double err_dens_sap = 0.001;

	double dens_acc = 7.870; //g/cm^3
	double err_dens_acc = 0.005;

	double g = 9.806; //m/s^2
	double err_g = 0.001;
	ofstream fout_visc("../Grafici_Velocity/visc_eta.txt");
	ofstream fout_fit_visc_1("../Grafici_Velocity/visc_fit_1.txt");
	ofstream fout_fit_visc_2("../Grafici_Velocity/visc_fit_2.txt");
	fout_visc << "#N\t#Visc[MISSING]\t#Err_visc[MISSING]" << endl;
	fout_fit_visc_1<<"#Diam^+2[mm]\t#Vel^+1[ND]\t#Err_vel^+1[ND]"<<endl;
	fout_fit_visc_2<<"#Diam^-2[mm]\t#Vel^-1[ND]\t#Err_vel^-1[ND]"<<endl;
	for (int k = 0; k < v.size(); k++)
	{
		double speed = v[k].v_media_tutte;		   //per ora è così da scegliere quella giusta seconodo criterio
		double err_speed = v[k].err_v_media_tutte; //da cambiare con quello appropriato
		double d_diam = g * (dens_acc - dens_sap) * 2.0 * diametro[k] / (18.0 * speed);
		double d_g = (pow(diametro[k], 2) * (dens_acc - dens_sap) / 18.0 * speed);
		double d_dens_sap = pow(diametro[k], 2) * g / (18.0 * speed);
		double d_dens_acc = pow(diametro[k], 2) * g / (18.0 * speed);
		double d_vel = pow(diametro[k], 2) * g * (dens_acc - dens_sap) / (18.0 * pow(speed, 2));
		v[k].viscosity = (pow(diametro[k], 2) * g * (dens_acc - dens_sap) / (18 * speed));
		v[k].err_viscosity = sqrt(pow(d_diam * err_diametro, 2) + pow(d_g * err_g, 2) + pow(d_dens_sap * err_dens_sap, 2) + pow(d_dens_acc * err_dens_acc, 2) + pow(d_vel * err_speed, 2)); //propagazione
		cout << k + 1 << "|\t" << v[k].viscosity << "\t" << v[k].err_viscosity << endl;
		fout_visc << k + 1 << "\t" << v[k].viscosity << "\t" << v[k].err_viscosity << endl;
		fout_fit_visc_1<<pow(diametro[k],2)<<"\t"<<speed<<"\t"<<err_speed<<endl;
		fout_fit_visc_2<<pow(diametro[k],-2)<<"\t"<<pow(speed,-1)<<endl;//qui serve fare la propagazione errori?
	}

	return 0;
}

double inch_to_mm(double inch)
{
	return 25.4 * inch;
}