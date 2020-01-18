#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Seat{
	public:
		int row=0;
		char col='\0';
		char clas='\0';
		Seat *next=NULL;
		Seat(){}
		Seat(int row,char col,char clas,Seat *next);
};

Seat::Seat(int row,char col,char clas,Seat *next=NULL){
	this->row = row;
	this->col = col;
	this->clas = clas;
	this->next = next;
}

Seat* addSeat(Seat *root,Seat *s){
	if(root == NULL)
		root = s;
	else{
		Seat *ptr=root;
		while(ptr->next != NULL){
			ptr = ptr->next;
		}
		ptr->next = s;
	}
	return root;
}

class Flight{
	public:
		string flightNumber;
		string departureDateTime;
		string dAirportCode;
		string aAirportCode;
		int numAvailableSeatsInB=0;
		int numAvailableSeatsInE=0;
		Seat *availableSeats;
	Flight(){}
	Flight(string &flightNumber,string &departureDateTime,string &dAirportCode,string &aAirportCode,int &numAvailableSeatsInB,int &numAvailableSeatsInE,Seat* &s);
	void displayBasic();
	void displayAvailableSeats();
	void booking(int row,char clas,char col);
	int checkBooking(int row,char clas,string col,int pos);
	~Flight();
};

Flight::Flight(string &flightNumber,string &departureDateTime,string &dAirportCode,string &aAirportCode,int &numAvailableSeatsInB,int &numAvailableSeatsInE,Seat* &s){
	this->flightNumber = flightNumber;
	this->departureDateTime = departureDateTime;
	this->dAirportCode = dAirportCode;
	this->aAirportCode = aAirportCode;
	this->numAvailableSeatsInB = numAvailableSeatsInB;
	this->numAvailableSeatsInE = numAvailableSeatsInE;
	this->availableSeats = s;
}

void Flight::displayBasic(){
	cout << "Flight Number : " << this->flightNumber <<endl;
	cout << "Departure Date and Time : " << this->departureDateTime << endl;
	cout << "Departure AirportCode : " << this->dAirportCode << endl;
	cout << "Arrival AirportCode : " << this->aAirportCode <<endl;
	cout << "Available seats in class B : " << this->numAvailableSeatsInB <<endl;
	cout << "Available seats in class E : " << this->numAvailableSeatsInE <<endl <<endl;
}

void Flight::displayAvailableSeats(){
	cout << "Available seats : " <<endl;
	Seat * tmp = availableSeats;
	while(tmp != NULL){
		cout << "    " << tmp->row << " " << tmp->clas << " " << tmp->col <<endl;
		tmp = tmp->next;
	}	
}

Flight::~Flight(){

	ofstream f("saved.txt",ios::app);
	f << flightNumber << endl;
	f << departureDateTime << endl;
	f << dAirportCode << endl;
	f << aAirportCode;
	Seat *tmp;

	int row = 0;
	char clas = '\0';

	while(availableSeats != NULL){
		if((row != availableSeats->row) || (clas != availableSeats->clas)){
			row = availableSeats->row;
			clas = availableSeats->clas;
			f << endl;
			f << availableSeats->row << " ";
			f << availableSeats->clas << " ";
		}
		f << availableSeats->col;
		tmp = availableSeats;
		availableSeats = availableSeats->next;
		tmp->next = NULL;
		free(tmp);
	}
	f << endl <<endl;
	f.close();
}

// Read whole file and store data
void readAndStore(vector<Flight *> &allFlights){

	string flightNumber="";
	string departureDateTime = "";
	string dAirportCode = "";
	string aAirportCode = "";
	Seat *availableSeats=NULL;
	int numAvailableSeatsInB=0;
	int numAvailableSeatsInE=0;

	//open file in read mode
	ifstream sf("./sample dataset.txt");

	//if file successfully open
	if(sf.fail()){
		cout << "ERROR : File openning failed..." << endl;
		exit(0);
	}

	string line;
	int count = 0;
	//read file line by line until eof is reach
	while(sf.good()){
		count++;
		getline(sf,line);

		if(line.empty()){
			Flight *tmp = new Flight(flightNumber,departureDateTime,dAirportCode,aAirportCode,numAvailableSeatsInB,numAvailableSeatsInE,availableSeats);
			allFlights.push_back(tmp);
			count = 0;
			flightNumber="";
			departureDateTime = "";
			dAirportCode = "";
			aAirportCode = "";
			availableSeats=NULL;
			numAvailableSeatsInB=0;
			numAvailableSeatsInE=0;
			continue;
		}

		switch(count){
			case 1:
				flightNumber = line;
				break;
			
			case 2:
				departureDateTime = line;
				break;
				
			case 3:
				dAirportCode = line;
				break;

			case 4:
				aAirportCode = line;
				break;

			default:
				if(count >4){

					int row;
					char clas;
					string col;
					stringstream s(line);
					s >> row >> clas >> col;

					for(int i=0; i<col.size(); i++){
						if(clas == 'B')
							numAvailableSeatsInB++;
						else
							numAvailableSeatsInE++;
						Seat *newSeat = new Seat(row,col[i],clas);
						availableSeats = addSeat(availableSeats,newSeat);
					}
				}
		}

	}	
	sf.close();
	return;
}

//booking the requested seat
void Flight::booking(int row,char clas,char col){
	//tmp - current node , ptr = previous node
	Seat* ptr=availableSeats,*tmp=availableSeats;
	while(tmp != NULL){
		if(tmp->row == row){
			while(tmp->row == row){
				if(tmp->clas == clas && tmp->col == col ){
					if(availableSeats->col == col){
						availableSeats = tmp->next;
						tmp->next = NULL;
					}
					else{
						ptr->next = tmp->next;
						tmp->next = NULL;
					}
					free(tmp);
					if(clas == 'E'){
						numAvailableSeatsInE--;
					}else{
						numAvailableSeatsInE--;
					}
					cout << "Seat " << row <<" "<< clas <<" "<<col<<" booking success..."<<endl; 
					return;
				}
				ptr = tmp;
				tmp = tmp->next;
			}
			cout << "Your input class or Column is invalid.." << endl;
			return;
		}
		ptr = tmp;
		tmp = tmp->next;
	}
	cout << "Please Enter available seat..." << endl;
	return;
}

int Flight::checkBooking(int row,char clas,string col,int pos){
	Seat* ptr=availableSeats,*tmp=availableSeats;

	if(col[pos] == '\0'){
		return 1;
	}

	while(tmp != NULL){
		if(tmp->row == row){
			while(tmp->row == row){
				if(tmp->clas == clas && tmp->col == col[pos] ){
					return checkBooking(row,clas,col,++pos);
				}
				ptr = tmp;
				tmp = tmp->next;
			}
			cout << "Your input class or Column is invalid.." << endl;
			return 0;
		}
		ptr = tmp;
		tmp = tmp->next;
	}
	cout << "Please Enter available seat..." << endl;
	return 0;
}

int main(){
	//allFlights array point each Flight object
	vector<Flight *> allFlights;
	readAndStore(allFlights);
	int numOfFlight = allFlights.size();
	int op=0;

	while(true){
		cout << "1. Display available flights" <<endl;
		cout << "2. View flight" << endl << "3. Seat availability" << endl << "4. Seat booking" << endl << "5.Exit" << endl << "Select a option : ";
		cin >> op;

		switch(op){

			case 1 :{
				for (int i=0; i<numOfFlight; i++){
					if((allFlights[i]->numAvailableSeatsInE+allFlights[i]->numAvailableSeatsInB) >0){
						allFlights[i]->displayBasic();
					}
				}
				break;
			}

			case 2 :{
				string fl;
				cout << "Search Flight : " ;
				cin >> fl;
				int flag=0;

				for(int i=0; i<numOfFlight; i++){
					if(fl == allFlights[i]->flightNumber){
						allFlights[i]->displayBasic();
						allFlights[i]->displayAvailableSeats();					
						flag = 1;
					}
				}
				if(flag==0){
					cout << "Requiested flight is not found. Please check and try again..."<<endl<<endl;
				}
				break;
			}

			case 3 :{
					string fl;
					cout << "Search Flight : " ;
					cin >> fl;
					int flag=0;

					for(int i=0; i<numOfFlight; i++){
						if(fl == allFlights[i]->flightNumber){
							flag = 1;
							string tmp;
							cout << "Number of seats : ";
							int numSeats;
							cin >> tmp;
							stringstream s(tmp);
							s >> numSeats;
							//if user input is not integer
							if(numSeats == 0 || numSeats > 1000){
								cout << "Invalid Number of seats." <<endl;
								break;
							}
							if((allFlights[i]->numAvailableSeatsInE + allFlights[i]->numAvailableSeatsInB) >= numSeats){
								cout << "Number of seats available : " << (allFlights[i]->numAvailableSeatsInE + allFlights[i]->numAvailableSeatsInB) <<endl;
								allFlights[i]->displayAvailableSeats();
							}else{
								cout << "Requiested number of seats are not available.." << endl <<endl;
							}
							break;
						}
					}
					//if user input Flight is not found
					if(flag == 0)
						cout << "Input Flight number is not found..." <<endl << endl;
					break;
			}
			case 4:{
				string flg;
				cout << "Search Flight : ";
				cin >> flg;
				int x=0;
				int flag=0;

				for(int i=0; i<numOfFlight; i++){
					if(flg == allFlights[i]->flightNumber){
						flag = 1;
						int row;
						char clas;
						string col;
						string tmp;
						int pos=0;
						cout << "Enter seat(row class col) : ";
						getline(cin,tmp);
						getline(cin,tmp);
						stringstream s(tmp);
						s >> row >> clas >> col;
						cout << row << clas << col<<endl;
						if(row ==0 || row >1000){
							cout << "Row number is invalid" << endl;
							exit(0);
						}
	
							x = allFlights[i]->checkBooking(row,clas,col,0);
						while(x==1 && col[pos] != '\0'){
							allFlights[i]->booking(row,clas,col[pos]);
							pos++;
							if(x == 0){

							}
						}
					}
				}
				if(flag == 0){
					cout << "Requested Flight is not found.." << endl;
				}
				break;
			}
			case 5:{

				ofstream f("saved.txt");
				f.close();

				for(int i=0; i<numOfFlight; i++){
					allFlights[i]->~Flight();
					free(allFlights[i]);
				}
				exit(1);
				break;
			}
		}
	}
	return 0;
}