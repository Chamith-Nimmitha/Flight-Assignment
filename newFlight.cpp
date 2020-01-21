#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
void optionMenu(int menuSelector);
using namespace std;

class Seat{
	private:
		int row=0;
		char col='\0';
		char clas='\0';
		Seat *next=NULL;

	public:
		Seat(){}
		Seat(int row,char col,char clas,Seat *next);

		int getRow(){
			return row;
		}

		char getCol(){
			return col;
		}

		char getClas(){
			return clas;
		}

		Seat* getNext(){
			return next;
		}

		void setNext(Seat *next){
			this->next = next;
		}
};

Seat::Seat(int row,char col,char clas,Seat *next=NULL){
	this->row = row;
	this->col = col;
	this->clas = clas;
	this->next = next;
}

// add available seat into linklist
Seat* addAvailableSeat(Seat *root,Seat *newSeat){
	if(root == NULL)
		root = newSeat;
	else{
		Seat *ptr=root;
		while(ptr->getNext() != NULL){
			ptr = ptr->getNext();
		}
		ptr->setNext(newSeat);
	}
	return root;
}

class Flight{
	private:
		string flightNumber;
		string departureDateTime;
		string dAirportCode;
		string aAirportCode;
		int numAvailableSeatsInB=0;
		int numAvailableSeatsInE=0;
		Seat *availableSeats;

	public:
		//default constructor
		Flight(){}
		//overloaad constructor
		Flight(string &flightNumber,string &departureDateTime,string &dAirportCode,string &aAirportCode,int &numAvailableSeatsInB,int &numAvailableSeatsInE,Seat* &s);

		string getFlightNumber(){
			return flightNumber;
		}

		string getDepartureDateTime(){
			return departureDateTime;
		}

		string getDAirportCode(){
			return dAirportCode;
		}

		string getAAirportCode(){
			return aAirportCode;
		}

		int getNumAvailableSeatsInB(){
			return numAvailableSeatsInB;
		}

		int getNumAvailableSeatsInE(){
			return numAvailableSeatsInE;
		}

		int getNumAvailableSeats(){
			return numAvailableSeatsInB+numAvailableSeatsInE;
		}
		Seat * getAvailableSeats(){
			return availableSeats;
		}

		void displayBasic(); // Display basic informations
		void displayAvailableSeats(); // Display seat informations in the Flight
		int checkBooking(int row,char clas,string col,int pos); // check requested seat is available
		void booking(int row,char clas,char col); // then book the seat
		~Flight(); //default destructor
};

// Overloaded Constructor
Flight::Flight(string &flightNumber,string &departureDateTime,string &dAirportCode,string &aAirportCode,int &numAvailableSeatsInB,int &numAvailableSeatsInE,Seat* &s){
	this->flightNumber = flightNumber;
	this->departureDateTime = departureDateTime;
	this->dAirportCode = dAirportCode;
	this->aAirportCode = aAirportCode;
	this->numAvailableSeatsInB = numAvailableSeatsInB;
	this->numAvailableSeatsInE = numAvailableSeatsInE;
	this->availableSeats = s;
}

// Display basic informations of a Flight 
void Flight::displayBasic(){
	cout << "Flight Number : " << this->flightNumber <<endl;
	cout << "Departure Date and Time : " << this->departureDateTime << endl;
	cout << "Departure AirportCode : " << this->dAirportCode << endl;
	cout << "Arrival AirportCode : " << this->aAirportCode <<endl;
	cout << "Available seats in class B : " << this->numAvailableSeatsInB <<endl;
	cout << "Available seats in class E : " << this->numAvailableSeatsInE <<endl <<endl;
}

// Display seat informations in the Flight
void Flight::displayAvailableSeats(){
	cout << "Available seats : " <<endl;
	Seat * tmp = availableSeats;
	while(tmp != NULL){
		cout << "    " << tmp->getRow() << " " << tmp->getClas() << " " << tmp->getCol() <<endl;
		tmp = tmp->getNext();
	}	
}

// check requested seat is available in selected Flight
// seats is available, then return 1.. otherwise return 0
int Flight::checkBooking(int row,char clas,string col,int pos){
	//pre - previous node  current - current node
	Seat* pre=availableSeats,*current=availableSeats;

	if(col[pos] == '\0'){
		return 1;
	}

	while(current != NULL){
		if(current->getRow() == row){
			while(current->getRow() == row){
				if(current->getClas() == clas && current->getCol() == col[pos] ){
					return checkBooking(row,clas,col,++pos);
				}
				pre = current;
				current = current->getNext();
			}
			cout << "Your input class or Column is invalid.." << endl;
			return 0;
		}
		pre = current;
		current = current->getNext();
	}
	cout << "Please Enter available seat..." << endl;
	return 0;
}

//if requested seat is available(call checkBooking() to check this) then,book the requested seat
void Flight::booking(int row,char clas,char col){
	//current - current node/seat , ptr = previous node/seat
	Seat* ptr=availableSeats,*current=availableSeats;
	while(current != NULL){
		if(current->getRow() == row){
			while(current->getRow() == row){
				if(current->getClas() == clas && current->getCol() == col ){
					if(availableSeats->getCol() == col){
						availableSeats = current->getNext();
						current->setNext(NULL);
					}
					else{
						ptr->setNext(current->getNext());
						current->setNext(NULL);
					}
					free(current);
					if(clas == 'E'){
						numAvailableSeatsInE--;
					}else{
						numAvailableSeatsInE--;
					}
					cout << "Seat " << row <<" "<< clas <<" "<<col<<" booking success..."<<endl; 
					return;
				}
				ptr = current;
				current = current->getNext();
			}
			cout << "Your input class or Column is invalid.." << endl;
			return;
		}
		ptr = current;
		current = current->getNext();
	}
	cout << "Please Enter available seat..." << endl;
	return;
}

//Destructor
Flight::~Flight(){
	//save all data into data file
	// free allocated space
	ofstream f("saved.txt",ios::app);
	f << flightNumber << endl;
	f << departureDateTime << endl;
	f << dAirportCode << endl;
	f << aAirportCode;
	Seat *tmp;

	int row = 0;
	char clas = '\0';

	while(availableSeats != NULL){
		if((row != availableSeats->getRow()) || (clas != availableSeats->getClas())){
			row = availableSeats->getRow();
			clas = availableSeats->getClas();
			f << endl;
			f << availableSeats->getRow() << " ";
			f << availableSeats->getClas() << " ";
		}
		f << availableSeats->getCol();
		tmp = availableSeats;
		availableSeats = availableSeats->getNext();
		tmp->setNext(NULL);
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
	ifstream dataFile("./sample dataset.txt");

	//if file successfully open
	if(dataFile.fail()){
		cout << "ERROR : File openning failed..." << endl;
		exit(0);
	}

	string line;
	int count = 0;
	//read file line by line until eof is reach
	while(dataFile.good()){
		count++;
		getline(dataFile,line);

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

			//read seats
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
						availableSeats = addAvailableSeat(availableSeats,newSeat);
					}
				}
		}

	}	
	dataFile.close();
	return;
}

int main(){
	//allFlights array point each Flight object
	vector<Flight *> allFlights;
	readAndStore(allFlights);
	int numOfFlight = allFlights.size();
	int op=0,menuSelector=0;
	string ops;

	
	while(true){
		
		optionMenu(menuSelector++);
		cout << endl  << "\e[32mSelect a option : ";
		getline(cin,ops);
		stringstream s(ops);
		s >> op ;
		if(op == 0 | op >5){
			cout << "Invalid input." << endl;
			continue;
		}
		switch(op){


			case 1 :{
				for (int i=0; i<numOfFlight; i++){
					cout << endl << "\e[35m~~~~~~~~~~~ RESULT ~~~~~~~~~~~~\e[37m" << endl << endl;
					if((allFlights[i]->getNumAvailableSeats()) >0){
						allFlights[i]->displayBasic();
					}
				}
				break;
			}

			case 2 :{
				string fl;
				cout << "Search Flight : " ;
				getline(cin,fl);
				int flag=0;

				cout << endl << "\e[35m~~~~~~~~~~~ RESULT ~~~~~~~~~~~~\e[37m" << endl << endl;
				for(int i=0; i<numOfFlight; i++){
					if(fl == allFlights[i]->getFlightNumber()){
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
					string searchFlight;
					cout << "Search Flight : " ;
					cin >> searchFlight;
					int flag=0;

					for(int i=0; i<numOfFlight; i++){
						if(searchFlight == allFlights[i]->getFlightNumber()){
							flag = 1;
							string strNumSeats;
							cout << "Number of seats : ";
							int numSeats;
							cin >> strNumSeats;
							cout << endl << "\e[35m~~~~~~~~~~~ RESULT ~~~~~~~~~~~~\e[37m" << endl << endl;
							stringstream s(strNumSeats);
							s >> numSeats;
							//if user input is not integer
							if(numSeats == 0 || numSeats > 1000){
								cout << "Invalid Number of seats." <<endl;
								break;
							}
							if((allFlights[i]->getNumAvailableSeats()) >= numSeats){
								cout << "Number of seats available : " << (allFlights[i]->getNumAvailableSeats()) <<endl;
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
				int bokingCheckFlag=0;
				int flag=0;

				for(int i=0; i<numOfFlight; i++){
					if(flg == allFlights[i]->getFlightNumber()){
						flag = 1;
						int row;
						char clas;
						string col;
						string userInput2;
						int pos=0;
						cout << "Enter seat(row class col) : ";
						getline(cin,userInput2);
						getline(cin,userInput2);
						cout << endl << "\e[35m~~~~~~~~~~~ RESULT ~~~~~~~~~~~~\e[37m" << endl << endl;
						stringstream s(userInput2);
						s >> row >> clas >> col;
						// cout << row << clas << col<<endl;
						if(row ==0 || row >1000){
							cout << "Row number is invalid" << endl;
							break;
						}
	
						bokingCheckFlag = allFlights[i]->checkBooking(row,clas,col,0);
						while(bokingCheckFlag==1 && col[pos] != '\0'){
							allFlights[i]->booking(row,clas,col[pos]);
							pos++;
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

void optionMenu(int menuSelector){

	if(menuSelector != 0)
		menuSelector = (menuSelector % 2)+1;

	switch(menuSelector){

		case 0:{
			cout <<endl << "\e[31m         ############################################|" << endl;
			cout << "         #                   WELCOME                # |" << endl;        
			cout << "        |############################################ | " << endl;
			cout << "        |                                           | |" << endl;
			cout << "        |        1. Display available flights       | |" <<endl;
			cout << "        |        2. View flight                     | |" << endl;
			cout << "        |        3. Seat availability               | |" << endl;
			cout << "        |        4. Seat booking                    | |" << endl;
			cout << "        |        5.Exit                             | |" << endl;
			cout << "        |-------------------------------------------| |" << endl;
			cout << "        ```````````````````````````````````````````````\e[37m" << endl;
			break;
		}

		case 1:{
			cout << endl << "\e[36m        |*|********************************************|" << endl;
			cout << "        | |                    MENU                    |" << endl;        
			cout << "        |*|******************************************|*| " << endl;
			cout << "        | |*                                       * | |" << endl;
			cout << "        |*|***    1. Display available flights   *** |*|" <<endl;
			cout << "        | |*****     2. View flight            ***** | |" << endl;
			cout << "        |*|*******     3. Seat availability   ****** |*|" << endl;
			cout << "        | |*****     4. Seat booking           ***** | |" << endl;
			cout << "        |*|***    5.Exit                         *** |*|" << endl;
			cout << "        |--------------------------------------------| |" << endl;
			cout << "        ````````````````````````````````````````````````\e[37m" << endl;
			break;
		}

		case 2:{
			cout << " \e[34m                    *               *       *        " << endl;
			cout << "         *             $ MENU $                ?   *  " << endl;        
			cout << "               ?    *                    *            " << endl;
			cout << "                              *                 *     " << endl;
			cout << "    *    *    1. Display available flights            " <<endl;
			cout << "              2. View flight                          " << endl;
			cout << "          ?   3. Seat availability          *         " << endl;
			cout << "       *      4. Seat booking       *           ?     " << endl;
			cout << "              5.Exit           *                    * " << endl;
			cout << "        ----------------------------------------------" << endl;
			cout << "        ``````````````````````````````````````````````\e[37m" << endl;
			break;
		}

		case 3:{

		}
	}
}