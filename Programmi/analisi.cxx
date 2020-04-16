#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct viscosimetro{
	
	
};

int main(){
	
	double c;
	vector <double> misura1;
	vector <double> misura2;
	vector <double> misura3;
	vector <double> misura4;
	vector <double> misura5;
	
	for(int k=1;k<=11;k++){				//scorrimento viscosimetri
		if((k==3)||(k==7)||(k==8)){									//misure cronometro
			for(int j=1;j<=5;j++){
				ifstream fin("Misure/"+ to_string(k) +"/"+ to_string(k) +"_"+ to_string(j) +".txt");			//cambiare a seconda che si sia su documenti o drive
				//ifstream fin("../Dati/Singolo/"+ to_string(k) +"/"+ to_string(k) +"_"+ to_string(j) +".txt");
				if(!fin){
					cout<<"Errore lettura file "<<k<<endl;
					return 1;
				}
				while(fin>>c){
					if(j==1) misura1.push_back(c);
					if(j==2) misura2.push_back(c);
					if(j==3) misura3.push_back(c);
					if(j==4) misura4.push_back(c);
					if(j==5) misura5.push_back(c);
				}
			}
		}
		if((k==1)||(k==2)||(k==4)||(k==5)||(k==10)||(k==11)){		//misure frame by frame
			
		}
		if((k==6)){		//viscosimetro 6 (3 misure)
			
		}
		if((k==9)){		//viscosimetro 9 (2 misure fbf)
			
		}
	}
	
	
	
    return 0;
}
