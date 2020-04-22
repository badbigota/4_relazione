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

	//INIZIO PROCESSO CALCOLO DI DELTA T IN DUE MODI PER VISCOS MARC

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

			//for (int i = 0; i < v[k].misura_media_mark.size() - 1; i++) //calcolo di delta con differenza e di errore con //propagazione
			//{
			//	v[k].delta_mark_primo_metodo.push_back(v[k].misura_media_mark[i + 1] - v[k].misura_media_mark[i]);
			//	v[k].err_delta_mark_primo_metodo.push_back(sqrt(pow(v[k].err_misura_media_mark[i + 1], 2) + pow(v[k].//err_misura_media_mark[i], 2)));
			//	//Da decommentare quando sapremo il metodo corretto
			//	//if(k!=5){
			//	//	v[k].delta_misure.push_back(media(righe));
			//	//	v[k].err_delta_misure.push_back(dstd_media(righe));
			//	//}
			//}
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

	//COMPATIBILITA' (ATTENZIONE SE USIAMO IL PRIMO METODO PER MARK O IL SECONDO)
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

	//INIZIO GENERAZIONE "T MEDI" ASSSE X GRAFICI VELOCITÀ
	for (int k = 0; k < v.size(); k++)
	{
		double somma_cumulativa = 0;
		for (int i = 0; i < v[k].delta_misura.size(); i++)
		{
			v[k].asse_x.push_back(somma_cumulativa + v[k].delta_misura[i] / 2);
			cout << somma_cumulativa + v[k].delta_misura[i] / 2 << endl;
			somma_cumulativa = somma_cumulativa + v[k].delta_misura[i];
		}
		cout << endl
			 << endl;
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
	double delta_x_10_e_11 = 500.0;								//definita in millimetri
	double err_delta_x = sqrt(2) * sigma_dist_tri(1.5875, 1.0); //DIPENDE DALLO SPESSORE DELLA TACCA E PARLLASSE E PROSPETTIVA

	for (int k = 0; k < v.size(); k++)
	{
		if ((k != 9) || (k != 10)) //IL CICLO È PER AVERE IL GIUSTO DELTA X
		{
			ofstream fout_vel("../Grafici_Velocity/visc_" + to_string(k + 1) + "_v.txt");
			fout_vel << "#T_intermedio[ms]\t#Vel_media[mm/ms]\t#err_vel_med[mm/ms]" << endl;
			for (int i = 0; i < v[k].delta_misura.size(); i++)
			{
				double err_vel = sqrt(pow((err_delta_x / v[k].delta_misura[i]), 2) + pow(delta_x * v[k].err_delta_misura[i] / pow(v[k].delta_misura[i], 2), 2));
				double vel = delta_x / v[k].delta_misura[i];
				if (i % 2 == 0)
				{ //mette nel vettore solo le velocità indipendenti
					v[k].velocity.push_back(vel);
					v[k].err_velocity.push_back(err_vel);
				}
				fout_vel << v[k].asse_x[i] << "\t" << vel << "\t" << err_vel << endl;
			}
		}
		else if ((k == 9) || (k == 10)) //SOLO QUESTI DUE HANNO UN DELTA X MOLTO PIÙ GRANDE
		{
			ofstream fout_vel("../Grafici_Velocity/visc_" + to_string(k + 1) + "_v.txt");
			fout_vel << "#T_intermedio[ms]\t#Vel_media[mm/ms]\t#err_vel_med[mm/ms]" << endl;
			for (int i = 0; i < v[k].delta_misura.size(); i++)
			{
				double err_vel = sqrt(pow((err_delta_x / v[k].delta_misura[i]), 2) + pow(delta_x_10_e_11 * v[k].err_delta_misura[i] / pow(v[k].delta_misura[i], 2), 2));
				double vel = delta_x_10_e_11 / v[k].delta_misura[i];
				if (i % 2 == 0)
				{ //mette nel vettore solo le velocità indipendenti
					v[k].velocity.push_back(vel);
					v[k].err_velocity.push_back(err_vel);
				}
				fout_vel << v[k].asse_x[i] << "\t" << vel << "\t" << err_vel << endl;
			}
		}
	}

	//FINE GENERAZIONE VELOCITÀ

	//STAMPA DELTA T
	//for (int k = 0; k < v.size(); k++)
	//{
	//	ofstream fout_dtm("../Grafici/" + to_string(k+1) + "deltat_medi.txt");
	//	for (int i = 0; i < v[k].delta_misura.size(); i++)
	//	{
	//		fout_dtm << v[k].delta_misura[i] << endl;
	//	}
	//}

	return 0;
}
