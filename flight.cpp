#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <fstream>

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
	Flight(string &flightNumber,string &departureDateTime,string &dAirportCode,string &aAirportCode,int numAvailableSeatsInB,int numAvailableSeatsInE,Seat *s);
	void displayBasic();
	void displayAvailableSeats();
	void booking(int row,char clas,char col);
	~Flight();
};

Flight::Flight(string &flightNumber,string &departureDateTime,string &dAirportCode,string &aAirportCode,int numAvailableSeatsInB,int numAvailableSeatsInE,Seat *s){
	this->flightNumber = flightNumber;
	this->departureDateTime = departureDateTime;
	this->dAirportCode = dAirportCode;
	this->aAirportCode = aAirportCode;
	this->numAvailableSeatsInB = numAvailableSeatsInB;
	this->numAvailableSeatsInE = numAvailableSeatsInE;

	Seat *temp= (Seat*)malloc(sizeof(Seat)*(this->numAvailableSeatsInB+numAvailableSeatsInE));
	int count = 0;
	while(count != (numAvailableSeatsInB+numAvailableSeatsInE)){
		temp[count] = s[count];
		count++;
	}
	this->availableSeats = temp;
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
	Seat *tmp;
	while(availableSeats != NULL){
		tmp = availableSeats;
		availableSeats = availableSeats->next;
		tmp->next = NULL;
		free(tmp);
	}
}

int readAndStore(Flight **allFlights){
	int numOfFlight=0;
	string flightNumber="";
	string departureDateTime = "";
	string dAirportCode = "";
	string aAirportCode = "";
	Seat *availableSeats=NULL;
	int numAvailableSeatsInB=0;
	int numAvailableSeatsInE=0;

	ifstream sf("./sample dataset.txt");

	string line;
	int count = 0,noFlight=0;
	while(!sf.eof()){
		count++;
		getline(sf,line);

		if(line.empty()){
			allFlights[numOfFlight] = new Flight(flightNumber,departureDateTime,dAirportCode,aAirportCode,numAvailableSeatsInB,numAvailableSeatsInE,availableSeats);
			numOfFlight++;
			count = 0;
			flightNumber="";
			departureDateTime = "";
			dAirportCode = "";
			aAirportCode = "";
			availableSeats=NULL;
			numAvailableSeatsInB=0;
			numAvailableSeatsInE=0;
			noFlight++;
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
					int pos = line.find(" ");
					int row = stoi(line.substr(0,pos));
					pos++;
					char clas = (line.substr(pos,1))[0];

					pos+=2;

					while(line[pos] != '\0'){
						if(clas == 'B')
							numAvailableSeatsInB++;
						else
							numAvailableSeatsInE++;
						Seat *newSeat = new Seat(row,line[pos],clas);
						availableSeats = addSeat(availableSeats,newSeat);
						pos++;
					}
					Seat *a = availableSeats;
				}
		}

	}	
	sf.close();
	return noFlight;
}

void Flight::booking(int row,char clas,char col){
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
					return;
				}
				ptr = tmp;
				tmp = tmp->next;
			}
			return;
		}
		ptr = tmp;
		tmp = tmp->next;
	}
	return;
}

int main(){
	Flight *allFlights[5];
	int noFlight = readAndStore(allFlights);
	int op=0;

	while(true){
		cout << "1. Display available flights" <<endl;
		cout << "2. View flight" << endl << "3. Seat availability" << endl << "4. Seat booking" << endl << "5.Exit" << endl << "Select a option : ";
		cin >> op;

		switch(op){

			case 1 :{
				for (int i=0; i<noFlight; i++){
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

				for(int i=0; i<noFlight; i++){
					if(fl == allFlights[i]->flightNumber){
						allFlights[i]->displayBasic();
						allFlights[i]->displayAvailableSeats();					
						flag = 1;
					}
				}
				if(flag==0){
					cout << "Requiested flight is not fount. Please check and try again..."<<endl<<endl;
				}
				break;
			}

			case 3 :{
					string fl;
					cout << "Search Flight : " ;
					cin >> fl;
					int flag=0;

					for(int i=0; i<noFlight; i++){
						if(fl == allFlights[i]->flightNumber){
							flag = 1;
							int numSeats;
							cout << "Number of seats : ";
							cin >> numSeats;
							if((allFlights[i]->numAvailableSeatsInE + allFlights[i]->numAvailableSeatsInB) >= numSeats){
								cout << "Number of seats available : " << (allFlights[i]->numAvailableSeatsInE + allFlights[i]->numAvailableSeatsInB) <<endl;
								allFlights[i]->displayAvailableSeats();
							}else{
								cout << "Requiested number of seats are not available.." << endl <<endl;
							}
						}
					}
					if(flag == 0)
						cout << "Input Flight number is not found..." <<endl << endl;

					break;
			}
			case 4:{
				string fl;
				cout << "Search Flight : " ;
				cin >> fl;
				int flag=0;

				for(int i=0; i<noFlight; i++){
					if(fl == allFlights[i]->flightNumber){
						flag = 1;
						cout << "Enter seat(row class col) : ";
						int pos=0;
						string srow;
						int row;
						string col;
						char clas;

						cin >> srow >> clas >> col;
						row = stoi(srow);

						
						while(col[pos] != '\0'){
							allFlights[i]->booking(row,clas,col[pos]);
							pos++;
						}
					}
				}
				if(flag == 1){
					cout << "success" << endl;
				}
				break;
			}
			case 5:{
				for(int i=0; i<noFlight; i++){
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