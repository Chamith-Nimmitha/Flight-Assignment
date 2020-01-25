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
		Flight(string &flightNumber,string &departureDateTime,string &dAirportCode,string &aAirportCode,int &numAvailableSeatsInB,int &numAvailableSeatsInE,Seat* s);

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
Flight::Flight(string &flightNumber,string &departureDateTime,string &dAirportCode,string &aAirportCode,int &numAvailableSeatsInB,int &numAvailableSeatsInE,Seat* s){
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
	cout <<"|```````````````````````````````````````````````````````" << right << "|" << endl;
	cout <<"|"<<setw(30)<< setw(30)<<"Flight Number : " << setw(25)<< left<<this->flightNumber << right << "|" << endl;
	cout <<"|"<<setw(30)<< "Departure Date and Time : " << setw(25)<< left<< this->departureDateTime  << right << "|" << endl;
	cout <<"|"<<setw(30)<< "Departure AirportCode : " << setw(25)<< left<< this->dAirportCode  << right << "|" << endl;
	cout <<"|"<<setw(30)<< "Arrival AirportCode : " << setw(25)<< left<< this->aAirportCode  << right << "|" << endl;
	cout <<"|"<<setw(30)<< "Available seats in class B : " << setw(25)<< left<< this->numAvailableSeatsInB  << right << "|" << endl;
	cout <<"|"<<setw(30)<< "Available seats in class E : " << setw(25)<< left<< this->numAvailableSeatsInE  << right << "|" << endl;
	cout <<"|" <<setw(56) <<"|"<<endl;
}

// Display seat informations in the Flight
void Flight::displayAvailableSeats(){
	cout << "|    Available seats : " <<setw(34)<< "|" <<endl <<"|";
	Seat * tmp = availableSeats;
	int count=0;
	while(tmp != NULL){
		if(count==4){
			cout << setw(8) << "|"<<endl<< "|";
				count=0;
		}
		count++;
		cout << setw(8) << tmp->getRow() << setw(2) << tmp->getClas() << setw(2) << tmp->getCol();
		tmp = tmp->getNext();
	}
	if(count==1){
		cout << setw(44)<< "|"<<endl<< "|";
	}else if(count==2){
		cout << setw(32) << "|"<<endl<< "|";
	}else if(count == 3){
		cout << setw(20) << "|"<<endl<< "|";
	}else{
		cout << setw(8) << "|"<<endl<< "|";
	}

	cout <<setw(56) <<"|"<<endl;
	cout <<"|```````````````````````````````````````````````````````" << right << "|" << endl<<endl;	
}

// check requested seats are available in selected Flight
// seats is available, then return 1.. otherwise return 0
int Flight::checkBooking(int row,char clas,string col,int pos){
	//pre - previous node  current - current node
	Seat* pre=availableSeats,*current=availableSeats;

	if(col != "\0" && col[pos] == '\0'){
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
						numAvailableSeatsInB--;
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
	ofstream f("saved.dat",ios::app);
	f << flightNumber << endl;
	f << departureDateTime << endl;
	f << dAirportCode << endl;
	f << aAirportCode;
	Seat *tmp;

	int row = 0;
	char clas = '\0';

	//write seat information to the  file
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

		//if encounter a empty line.
		// that's mean finished reading a flight informations
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
			//get flight number
			case 1:
				flightNumber = line;
				break;
			//get departure Date Time
			case 2:
				departureDateTime = line;
				break;
			//get  departure AirportCode
			case 3:
				dAirportCode = line;
				break;
			//get Arrival AirportCode
			case 4:
				aAirportCode = line;
				break;

			//read seats informations
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

//option 2: view given filght
void viewFlight(vector <Flight *> allFlights){
	string searchFlight;
	int numOfFlight = allFlights.size();
	input2:cout << "Search Flight ( -c to cancel): " ;
	getline(cin,searchFlight);
	if(searchFlight==""){
		cout << "ERROR : Please enter a flight number..." <<endl;
		goto input2;
	}else if(searchFlight == "-c"){
		cout << "Operation canceled." <<endl<<endl;
		return;
	}
	int flag=0;

	for(int i=0; i<numOfFlight; i++){
		if(searchFlight == allFlights[i]->getFlightNumber()){
			cout << endl << "\e[35m~~~~~~~~~~~ RESULT ~~~~~~~~~~~~\e[37m" << endl << endl;
			allFlights[i]->displayBasic();
			allFlights[i]->displayAvailableSeats();					
			flag = 1;
			break;
		}
	}
	if(flag==0){
		cout << "ERROR : Requiested flight is not found. Please check and try again..."<<endl<<endl;
		goto input2;
	}
}

//option 3: cheak seat availability in given flight
void cheakAvailability(vector<Flight *> &allFlights){

	string searchFlight;
	input3:cout << "Search Flight (-c to cancel) : " ;
	getline(cin,searchFlight);
	if(searchFlight==""){
		cout << "ERROR : Please enter a flight number..." <<endl;
		goto input3;
	}else if(searchFlight == "-c"){
		cout << "Operation canceled." <<endl<<endl;
		return;
	}

	int numOfFlight = allFlights.size();
	int flag=0;

	for(int i=0; i<numOfFlight; i++){
		if(searchFlight == allFlights[i]->getFlightNumber()){
			flag = 1;
			string strNumSeats;
			int numSeats;
			input4:cout << "Number of seats ( -c to cancel): ";
			getline(cin,strNumSeats);
			if(strNumSeats==""){
				cout << "ERROR : Please enter a number of seats..." <<endl;
				goto input4;
			}else if(strNumSeats == "-c"){
				cout << "Operation canceled." <<endl<<endl;
				return;
			}
			stringstream s(strNumSeats);

			s >> numSeats;
			//if user input is not integer
			if(numSeats == 0 || numSeats > 1000){
				cout << "ERROR : Invalid Number of seats." <<endl;
				goto input4;
				break;
			}
			if((allFlights[i]->getNumAvailableSeats()) >= numSeats){
				cout << endl << "\e[35m~~~~~~~~~~~ RESULT ~~~~~~~~~~~~\e[37m" << endl << endl;
				cout <<"|```````````````````````````````````````````````````````" << right << "|" << endl;
				cout << "|Number of seats available in class B : " << setw(2)<<(allFlights[i]->getNumAvailableSeatsInB()) << setw(15)<<"|"<<endl;
				cout << "|Number of seats available in class E : " << setw(2)<<(allFlights[i]->getNumAvailableSeatsInE()) << setw(15)<<"|"<<endl<<"|"<<setw(56)<<"|"<<endl;
				allFlights[i]->displayAvailableSeats();
			}else{
				cout << "Requiested number of seats are not available in this movement.." << endl <<endl;
			}
			break;
		}
	}
	//if user input Flight is not found
	if(flag == 0)
		cout << "Input Flight number is not found..." <<endl << endl;
	goto input3;
}

//option 4: book the given seat in given flight
void bokingSeats(vector<Flight *> allFlights){
	string searchFlight;
	int numOfFlight = allFlights.size();
	input5:cout << "Search Flight (-c to cancel) : ";
	getline(cin,searchFlight);
	if(searchFlight==""){
		cout << "ERROR : Please enter a flight number..." <<endl;
		goto input5;
	}else if(searchFlight == "-c"){
		cout << "Operation canceled." <<endl<<endl;
		return;
	}
	int bokingCheckFlag=0;
	int flag=0;

	for(int i=0; i<numOfFlight; i++){
		if(searchFlight == allFlights[i]->getFlightNumber()){
			flag = 1;
			int row;
			char clas;
			string col;
			string userInput2;
			int pos=0;
			input6:cout << "Enter seat(row class col) -( -c to cancel ) : ";
			getline(cin,userInput2);
			if(userInput2==""){
				cout << "ERROR : Please enter seat(row class col)..." <<endl;
				goto input6;
			}else if(userInput2 == "-c"){
				cout << "Operation canceled." <<endl<<endl;
				return;
			}
			// getline(cin,userInput2);
			stringstream s(userInput2);
			s >> row >> clas >> col;
			// cout << row << clas << col<<endl;
			if(row == 0 || row >1000){
				cout << "ERROR : Row number is invalid" << endl;
				goto input6;
			}else if((clas != 'E' && clas != 'B')){
				cout << "ERROR : Class Type is invalid" << endl;
				goto input6;
			}else if(col == "\0"){
				cout << "ERROR : Seat column is invalid" << endl;
				goto input6;
			}

			bokingCheckFlag = allFlights[i]->checkBooking(row,clas,col,0);
			if(bokingCheckFlag == 0){
				cout << "ERROR : Requested Seat not found iin this movement."<<endl;
				goto input6;
			}else{
				cout << endl << "\e[35m~~~~~~~~~~~ RESULT ~~~~~~~~~~~~\e[37m" << endl << endl;
				while(col[pos] != '\0'){
					allFlights[i]->booking(row,clas,col[pos]);
					pos++;
				}
			}
		}
	}
	if(flag == 0){
		cout << "ERROR : Requested Flight is not found.." << endl;
		goto input5;
	}
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
		input1:cout << endl  << "\e[32mSelect a option : ";
		getline(cin,ops);
		stringstream s(ops);
		s >> op ;
		if(op == 0 | op >5){
			cout << "ERROR : Invalid input." << endl;
			goto input1;
		}
		switch(op){

			// view all available flights
			case 1 :{
				cout << endl << "\e[35m~~~~~~~~~~~ RESULT ~~~~~~~~~~~~\e[37m" << endl << endl;
				for (int i=0; i<numOfFlight; i++){
					if((allFlights[i]->getNumAvailableSeats()) >0){
						allFlights[i]->displayBasic();
					}
				}
				cout <<"|```````````````````````````````````````````````````````" << right << "|" << endl;
				break;
			}
			// view a specified flight
			case 2 :{
				viewFlight(allFlights);
				break;
			}
			// cheack seat availability in a given flight
			case 3 :{	
				cheakAvailability(allFlights);
				break;
			}
			// book the given seat in given flight
			case 4:{
				bokingSeats(allFlights);
				break;
			}
			// sava data to the file and exit
			case 5:{

				ofstream f("saved.dat");
				f.close();

				for(int i=0; i<numOfFlight; i++){
					allFlights[i]->~Flight();
					free(allFlights[i]);
				}
				return 0;
				break;
			}
		}
		op=0;
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