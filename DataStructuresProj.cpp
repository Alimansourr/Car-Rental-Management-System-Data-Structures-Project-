#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include<chrono>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

struct date {
    int day, month, year;
    date* next;
};

struct car {
    string name;
    string plateNumber;
    string color;
    date* tDate;
    car* next;
};

struct user {
    int userID;
    string fname;
    string lname;
    car* rentedCar;
    user* next, * previous;
};

struct userList {
    user* head, * tail;
};


void DisplayList(user* head) {
	user* cur = head;
	while (cur != nullptr) {
		cout << cur->userID << "," << cur->fname << "," << cur->lname << "\n";
		cur = cur->next;
	}
}

// print list
void printUsers(userList* ul) {
	user* cur = ul->head;
	while (cur != NULL) {
		cout << "User ID: " << cur->userID << ", Name: " << cur->fname << " " << cur->lname << endl;
		cur = cur->next;
	}
}


bool isEmpty(user* head) { // just check if the list is empty
	return (head == NULL);
}

bool isEmpty(userList* ul) {
	return (ul->head == NULL);
}
user* InsertatTail(user* head, user tmp) {
	

	user* pointertmp = new user;
	pointertmp->userID = tmp.userID;
	pointertmp->fname = tmp.fname;
	pointertmp->lname = tmp.lname;
	pointertmp->next = nullptr;

	if (isEmpty(head)) {
		return pointertmp;
	}

	user* cur = head;
	while (cur->next != nullptr) {
		cur = cur->next;
	}

	cur->next = pointertmp;
	return head;
}



// Function to sign in a user
void Signin(user& userr) {
	bool validSignIn = false;
	do {
		cout << "Enter Your ID: ";
		string inputStr;
		getline(cin, inputStr);
		if (inputStr.empty()) {
			cout << "Invalid ID. Please try again." << endl;
			continue;
		}
		userr.userID = stoi(inputStr);

		ifstream file("users.txt");
		if (!file) {
			cerr << "Error: Unable to open user file." << endl;
			return;
		}
		string line;
		bool found = false;
		while (getline(file, line)) {
			istringstream iss(line);
			string userIDStr, fname, lname;
			getline(iss, userIDStr, ',');
			getline(iss, fname, ',');
			getline(iss, lname);
			int tempUserID = stoi(userIDStr);
			if (tempUserID == userr.userID) {
				userr.fname = fname;
				userr.lname = lname;
				found = true;
				break;
			}
		}
		file.close();
		if (found) {
			cout << "Successful login" << endl;
			validSignIn = true;
		}
		else {
			cout << "Invalid ID. Please try again." << endl;
		}
	} while (!validSignIn);
}
void Signup(user& tmp) {
	cout << "Enter your first name: ";
	getline(cin, tmp.fname);
	cout << "Enter your last name: ";
	getline(cin, tmp.lname);
	ifstream file("users.txt");
	int maxID = 0;
	string line;

	while (getline(file, line)) {
		istringstream iss(line);
		string userIDStr, fname, lname;
		getline(iss, userIDStr, ',');
		getline(iss, fname, ',');
		getline(iss, lname, ',');
		int id = stoi(userIDStr);
		if (id > maxID) {
			maxID = id;

		}

	}

	file.close();

	tmp.userID = maxID + 1;
	cout << "Your User ID: " << tmp.userID << endl;
	ofstream outputFile("users.txt", ios::app);

	if (outputFile.is_open()) {
		outputFile << tmp.userID << "," << tmp.fname << "," << tmp.lname << endl;
		outputFile.close();

	}

	else {
		cerr << "Unable to open file." << endl;
	}

	

	user* userList = NULL;
	ifstream readFile("users.txt");
	if (readFile.is_open()) {
		while (getline(readFile, line)) {

			istringstream iss(line);
			string userIDStr, fname, lname;
			getline(iss,userIDStr, ',');
			getline(iss, fname, ',');
			getline(iss, lname, ',');
			int id = stoi(userIDStr);
			user newUser = { id, fname, lname };
			userList = InsertatTail(userList, newUser);
		

		}

		readFile.close();

	}

	else {
		cerr << "Unable to open file." << endl;
	}

	cout << "The list of users in the car rental system: " << endl;
	DisplayList(userList);

}
void printUserDataAndRentedCars(userList* ul) {
	user* currentUser = ul->head;

	while (currentUser != nullptr) {
		cout << "User ID: " << currentUser->userID << endl;
		cout << "Name: " << currentUser->fname << " " << currentUser->lname << endl;

		if (currentUser->rentedCar == nullptr) {
			cout << "No cars rented" << endl;
		}
		else {
			cout << "Rented cars:" << endl;
			car* currentCar = currentUser->rentedCar;

			while (currentCar != nullptr) {
				cout << " - Plate Number: " << currentCar->plateNumber << endl;
				date* currentDate = currentCar->tDate;
				cout << "   Start Date: " << currentDate->day << "-" << currentDate->month << "-" << currentDate->year << endl;
				currentDate = currentDate->next;
				if (currentDate != nullptr) {
					cout << "   End Date: " << currentDate->day << "-" << currentDate->month << "-" << currentDate->year << endl;
				}
				else {
					cout << "   End Date: Not specified" << endl;
				}
				currentCar = currentCar->next;
			}
		}

		cout << endl;
		currentUser = currentUser->next;
	}
}



userList* parseInputFiles(string usersFile,string carsFilePath, string rentedCarsFile) {
	userList* ul = new struct userList;
	ul->head = nullptr;
	ul->tail = nullptr;

	// Parse users file
	ifstream usersFileStream(usersFile);
	string line;
	while (getline(usersFileStream, line)) {
		stringstream ss(line);
		string token;
		getline(ss, token, ',');
		int userID = stoi(token);
		getline(ss, token, ',');
		string fname = token;
		getline(ss, token, ',');
		string lname = token;

		user* newUser = new user;
		newUser->userID = userID;
		newUser->fname = fname;
		newUser->lname = lname;
		newUser->rentedCar = nullptr;
		newUser->next = nullptr;
		newUser->previous = nullptr;

		if (ul->head == nullptr) {
			ul->head = newUser;
			ul->tail = newUser;
		}
		else {
			ul->tail->next = newUser;
			newUser->previous = ul->tail;
			ul->tail = newUser;
		}
	}

	// Parse rented cars file
	ifstream rentedCarsFileStream(rentedCarsFile);
	while (getline(rentedCarsFileStream, line)) {
		stringstream ss(line);
		string token;
		getline(ss, token, ':');
		int userID = stoi(token);

		user* currentUser = ul->head;
		while (currentUser != nullptr && currentUser->userID != userID) {
			currentUser = currentUser->next;
		}

		if (currentUser != nullptr) {
			getline(ss, token);
			stringstream rentedCarsSS(token);
			string rentedCarToken;
			while (getline(rentedCarsSS, rentedCarToken, ',')) {
				stringstream rentedCarSS(rentedCarToken);
				getline(rentedCarSS, token, '#'); // Extract plate number before '#'
				string plateNumber = token;
				getline(rentedCarSS, token, '/');
				string startDate = token;
				getline(rentedCarSS, token, '/');
				string endDate = token;

				car* newCar = new car;
				newCar->plateNumber = plateNumber;

				date* startDateNode = new date;
				stringstream startDateSS(startDate);
				getline(startDateSS, token, '-');
				startDateNode->day = stoi(token);
				getline(startDateSS, token, '-');
				startDateNode->month = stoi(token);
				getline(startDateSS, token, '-');
				startDateNode->year = stoi(token);
				startDateNode->next = nullptr;

				date* endDateNode = new date;
				stringstream endDateSS(endDate);
				getline(endDateSS, token, '-');
				endDateNode->day = stoi(token);
				getline(endDateSS, token, '-');
				endDateNode->month = stoi(token);
				getline(endDateSS, token, '-');
				endDateNode->year = stoi(token);
				endDateNode->next = nullptr;

				startDateNode->next = endDateNode;
				newCar->tDate = startDateNode;

				// Append the new rented car to the list of cars for the current user
				if (currentUser->rentedCar == nullptr) {
					newCar->next = nullptr; // Initialize the next pointer of the new car
					currentUser->rentedCar = newCar;
				}
				else {
					car* currentCar = currentUser->rentedCar;
					while (currentCar->next != nullptr) {
						currentCar = currentCar->next;
					}
					newCar->next = nullptr; // Initialize the next pointer of the new car
					currentCar->next = newCar;
				}
			}
		}
	}

	return ul;
}



//fucntion 2
void addUser(userList* ul, user u) {
	while (true) {
		bool exists = false;
		user* cur = ul->head;
		while (cur != NULL) {
			if (cur->userID == u.userID) {
				exists = true;
				cout << "User with ID " << u.userID << " already exists. Please enter a different ID: ";
				cin >> u.userID;
				break;
			}
			cur = cur->next;
		}
		if (!exists) {
			user* newUser = new user;
			newUser->userID = u.userID;
			newUser->fname = u.fname;
			newUser->lname = u.lname;
			newUser->rentedCar = u.rentedCar;
			newUser->next = NULL;

			if (isEmpty(ul)) {
				ul->head = newUser;
				ul->tail = newUser;
			}
			else {
				ul->tail->next = newUser;
				newUser->previous = ul->tail;
				ul->tail = newUser;
			}
			break; // Exit the loop if the user ID doesn't exist
		}
	}
}




// Function to delete a user from the user list
void deleteUser(userList* ul, int userIDToDelete) {
	user* cur = ul->head;

	
	while (cur != NULL) {
		if (cur->userID == userIDToDelete) {
			
			if (cur == ul->head && cur == ul->tail) {
				// Only one user in the list
				ul->head = NULL;
				ul->tail = NULL;
			}
			else if (cur == ul->head) {
				// Deleting the first user in the list
				ul->head = cur->next;
				ul->head->previous = NULL;
			}
			else if (cur == ul->tail) {
				// Deleting the last user in the list
				ul->tail = cur->previous;
				ul->tail->next = NULL;
			}
			else {
				// Deleting a user from somewhere in the middle
				cur->previous->next = cur->next;
				cur->next->previous = cur->previous;
			}
			delete cur; 
			cout << "User with ID " << userIDToDelete << " has been deleted.\n";
			return;
		}
		cur = cur->next;
	}

	// User not found
	cout << "User with ID " << userIDToDelete << " not found.\n";
}

// Function to update user information (first and last names)
void updateUser(userList* ul) {
	int userIDToUpdate;
	cout << "Enter the ID of the user you want to update: ";
	cin >> userIDToUpdate;

	user* cur = ul->head;

	// Search for the user with the specified user ID
	while (cur != NULL) {
		if (cur->userID == userIDToUpdate) {
			string newFirstName, newLastName;
			cout << "Enter the new first name: ";
			cin >> newFirstName;
			cout << "Enter the new last name: ";
			cin >> newLastName;

			// Update user information
			cur->fname = newFirstName;
			cur->lname = newLastName;

			cout << "User with ID " << userIDToUpdate << " has been updated.\n";
			return;
		}
		cur = cur->next;
	}

	// User not found
	cout << "User with ID " << userIDToUpdate << " not found. Unable to update.\n";
}







// print cars list
void printCars(car* carsList) {
	cout << "List of Cars:" << endl;
	while (carsList != NULL) {
		cout << "Plate Number: " << carsList->plateNumber << ", Name: " << carsList->name << ", Color: " << carsList->color << endl;
		carsList = carsList->next;
	}
}

car* parseCarsFile(const string& carsFilePath) {
	ifstream carsFile(carsFilePath);
	if (!carsFile.is_open()) {
		cout << "Error: Unable to open cars' file." << endl;
		return NULL;
	}
	car* carsList = NULL;
	string line;
	while (getline(carsFile, line)) {
		stringstream ss(line);
		string plateNumber, name, color;
		if (getline(ss, plateNumber, ',') &&
			getline(ss, name, ',') &&
			getline(ss, color)) {
			// Check if plate number, name, and color are empty
			if (!plateNumber.empty() && !name.empty() && !color.empty()) {
				car* newCar = new car;
				newCar->plateNumber = plateNumber;
				newCar->name = name;
				newCar->color = color;
				newCar->next = carsList;
				carsList = newCar;
			}
			else {
				cout << "Error: Invalid format in cars' file." << endl;
			}
		}
		else {
			cout << "Error: Unable to parse line in cars' file." << endl;
		}
	}
	carsFile.close();
	return carsList;
}



void Insertionsort(userList* ul) {
	if (ul == NULL || ul->head == NULL || ul->head->next == NULL) {
		return;
	}
	user* cur = ul->head->next;
	while (cur != NULL) {
		user* key = cur;
		user* prev = cur->previous;
		while (prev != NULL && prev->lname > key->lname) {
			if (key->next != NULL) {
				key->next->previous = prev;
			}
			prev->next = key->next;
			key->previous = prev->previous;
			key->next = prev;
			if (prev->previous != NULL) {
				prev->previous->next = key;
			}
			else {
				ul->head = key;
			}
			prev->previous = key;
			prev = key->previous;
		}
		cur = cur->next;
	}
	user* tail = ul->head;
	while (tail->next != NULL) {
		tail = tail->next;
	}
	ul->tail = tail;
}

void writeToFiles(userList* ul, string usersOutputFile, string rentedCarsOutputFile) {
	// Open output files
	ofstream usersFileStream(usersOutputFile);
	ofstream rentedCarsFileStream(rentedCarsOutputFile);

	// Write users to the users file
	user* currentUser = ul->head;
	while (currentUser != nullptr) {
		usersFileStream << currentUser->userID << "," << currentUser->fname << "," << currentUser->lname << endl;
		currentUser = currentUser->next;
	}

	// Write rented cars to the rented cars file
	currentUser = ul->head;
	while (currentUser != nullptr) {
		if (currentUser->rentedCar != nullptr) {
			rentedCarsFileStream << currentUser->userID << ":";
			car* currentCar = currentUser->rentedCar;
			bool isFirst = true;
			while (currentCar != nullptr) {
				if (!isFirst) {
					rentedCarsFileStream << ",";
				}
				isFirst = false;
				rentedCarsFileStream << currentCar->plateNumber << "#";
				date* currentDate = currentCar->tDate;
				rentedCarsFileStream << currentDate->day << "-" << currentDate->month << "-" << currentDate->year << "/";
				currentDate = currentDate->next;
				if (currentDate != nullptr) {
					rentedCarsFileStream << currentDate->day << "-" << currentDate->month << "-" << currentDate->year;
				}
				currentCar = currentCar->next;
			}
			rentedCarsFileStream << endl;
		}
		currentUser = currentUser->next;
	}

	// Close output files
	usersFileStream.close();
	rentedCarsFileStream.close();
}



int compareDates(const date d1, const date d2) {
	if (d1.year > d2.year) return 1;
	else if (d1.year < d2.year) return -1;
	else if (d1.month > d2.month) return 1;
	else if (d1.month < d2.month) return -1;
	else if (d1.day > d2.day) return 1;
	else if (d1.day < d2.day) return -1;
	else return 0;
}

bool isDateBetween(const date& dateToCheck, const date& startDate, const date& endDate) {
	return compareDates(dateToCheck, startDate) >= 0 && compareDates(dateToCheck, endDate) <= 0;
}

bool isOverlapping(const date& start, const date& end, const date& rentedStart, const date& rentedEnd) {
	return isDateBetween(start, rentedStart, rentedEnd) || isDateBetween(end, rentedStart, rentedEnd);
}

// parse date
date parseDate(const string& dateString) {
	date d;
	stringstream ss(dateString);
	char discard;
	ss >> d.day >> discard >> d.month >> discard >> d.year;
	return d;
}

void deleteCarsList(car* head) {
	while (head != nullptr) {
		car* temp = head;
		head = head->next;
		delete temp;
	}
}


car* addCarToList(car* head, const string& plateNumber, const string& name, const string& color) {
	
	car* newCar = new car;
	newCar->plateNumber = plateNumber;
	newCar->name = name;
	newCar->color = color;
	newCar->next = nullptr;

	
	ofstream outFile("cars.txt", ios::app);
	if (outFile.is_open()) {
		outFile << plateNumber << "," << name << "," << color << endl;
		outFile.close();
	}
	else {
		cout << "Error: Unable to open cars.txt file." << endl;
	}

	
	if (head == nullptr) {
		return newCar;
	}
	else {
		
		car* currentCar = head;
		while (currentCar->next != nullptr) {
			currentCar = currentCar->next;
		}
	
		currentCar->next = newCar;
		return head;
	}
}


void rentCar(userList* ul, user* usr, const string startDate, const string endDate) { //RENT THE CAR FOR THE HEAD OF THE USER LIST
	// Load car data from the cars.txt file
	car* availableCars = parseCarsFile("cars.txt");

	// Parse start and end dates
	date start = parseDate(startDate);
	date end = parseDate(endDate);

	// Filter out rented cars from available cars list
	user* currentUser = ul->head;
	while (currentUser != nullptr) {
		car* rentedCars = currentUser->rentedCar;
		while (rentedCars != nullptr) {
			date* rentedStart = rentedCars->tDate;
			date* rentedEnd = rentedCars->tDate->next;

			// Check if the rented car overlaps with the specified rental period
			if (isOverlapping(start, end, *rentedStart, *rentedEnd)) {
				
				car** currentCarPtr = &availableCars;
				car* currentCar = availableCars;
				while (currentCar != nullptr) {
					if (currentCar->plateNumber == rentedCars->plateNumber) {
						*currentCarPtr = currentCar->next;
						delete currentCar;
						break;
					}
					currentCarPtr = &(currentCar->next);
					currentCar = currentCar->next;
				}
			}
			rentedCars = rentedCars->next;
		}
		currentUser = currentUser->next;
	}

	// Print the details of available cars
	if (availableCars == nullptr) {
		cout << "No available cars for rent from " << startDate << " to " << endDate << endl;
		return;
	}

	cout << "Available cars from " << startDate << " to " << endDate << ":" << endl;
	car* currentCar = availableCars;
	while (currentCar != nullptr) {
		cout << "Plate Number: " << currentCar->plateNumber << ", Name: " << currentCar->name << ", Color: " << currentCar->color << endl;
		currentCar = currentCar->next;
	}

	// Asking user for plate number
	bool validPlateNumber = false;
	string plateNumber;
	car* selectedCar = nullptr;

	while (!validPlateNumber) {
		cout << "Enter the plate number of the car you want to rent: ";
		cin >> plateNumber;

		
		currentCar = availableCars;
		selectedCar = nullptr;
		while (currentCar != nullptr) {
			if (currentCar->plateNumber == plateNumber) {
				selectedCar = currentCar;
				validPlateNumber = true;
				break;
			}
			currentCar = currentCar->next;
		}

		if (!validPlateNumber) {
			cout << "Invalid plate number. Please try again." << endl;
		}
	}

	// Adding rented car to user's rented cars list
	car* newRentedCar = new car;
	newRentedCar->plateNumber = selectedCar->plateNumber;
	newRentedCar->name = selectedCar->name;
	newRentedCar->color = selectedCar->color;
	newRentedCar->tDate = new date;
	newRentedCar->tDate->day = start.day;
	newRentedCar->tDate->month = start.month;
	newRentedCar->tDate->year = start.year;
	newRentedCar->tDate->next = new date;
	newRentedCar->tDate->next->day = end.day;
	newRentedCar->tDate->next->month = end.month;
	newRentedCar->tDate->next->year = end.year;
	newRentedCar->tDate->next->next = nullptr;

	if (usr->rentedCar == nullptr) {
		newRentedCar->next = nullptr;
		usr->rentedCar = newRentedCar;
	}
	else {
		car* currentCar = usr->rentedCar;
		while (currentCar->next != nullptr) {
			currentCar = currentCar->next;
		}
		newRentedCar->next = nullptr;
		currentCar->next = newRentedCar;
	}

	cout << "Car rented successfully" << endl;

	
	deleteCarsList(availableCars);
}


date getCurrentDate() {
	time_t now = time(0);
	tm currentTime;
	localtime_s(&currentTime, &now);

	date currentDate;
	currentDate.day = currentTime.tm_mday;
	currentDate.month = currentTime.tm_mon + 1;
	currentDate.year = currentTime.tm_year + 1900;

	return currentDate;
}



void deleteRentedCarsBeforeDate(userList* ul, const string& specifiedDate) {
	time_t t = time(nullptr);
	//tm* now = localtime(&t); //THEY ARE TELLLING ME THAT IT IS UNSAFE SO I MAKE ANOTHER FUNCTION TO GET THE CURRET DATE FROM IT
	//date currentDate = { now->tm_mday, now->tm_mon + 1, now->tm_year + 1900 };
	date d = getCurrentDate();

	// Check if the specified date is before the current date
	if (compareDates(d, parseDate(specifiedDate)) == 1) {
		cout << "Specified date is before the current date, so no changes are done." << endl;
		return;
	}

	user* currentUser = ul->head;
	int deletedDatesCounter = 0;

	// loop through each user in the user list
	while (currentUser != nullptr) {
		car* currentCar = currentUser->rentedCar;
		car* prevCar = nullptr;

	
		while (currentCar != nullptr) {
			date* startDate = currentCar->tDate;

		
			if (compareDates(*startDate, parseDate(specifiedDate)) < 0 && compareDates(*startDate, d) > 0) {
				
				if (prevCar == nullptr) {
				
					currentUser->rentedCar = currentCar->next;
				}
				else {
					
					prevCar->next = currentCar->next;
				}

				car* temp = currentCar;
				currentCar = currentCar->next;
				delete temp;

				
				deletedDatesCounter++;
			}
			else {
				
				prevCar = currentCar;
				currentCar = currentCar->next;
			}
		}

		currentUser = currentUser->next;
	}

	if (deletedDatesCounter == 0) {
		cout << "No rented cars found with start date before " << specifiedDate << " and not before the current date." << endl;
	}
	else {
		cout << "Deleted " << deletedDatesCounter << " rented car(s) before " << specifiedDate << " successfully." << endl;
	}
}





int main() {
	
	date currentDate = getCurrentDate();

	cout << "Current Date: " << currentDate.day << "-" << currentDate.month << "-" << currentDate.year << endl;

	userList* ul;
	user tmp;
	string usersFilePath = "users.txt";
	string rentedCarsFilePath = "rented_cars.txt";
	string carsFilePath = "cars.txt";

	cout << "-------------------------------------Welcome To Our Car Rental System--------------------------------" << endl;
	string yesorno;
	do {
		cout << "If you want to continue the login process enter the command 'Yes' and if you want to signup, enter the command 'No' " << endl;
		getline(cin, yesorno);
		if (yesorno == "Yes") {
			Signin(tmp);
		}
		else if (yesorno == "No") {
			cout << "-----------------Sign up---------------------" << endl;
			Signup(tmp);
			cout << "Now you should sign in " << endl;
			Signin(tmp);
		}
	} while (yesorno != "Yes" && yesorno != "No");

	ul = parseInputFiles(usersFilePath, carsFilePath, rentedCarsFilePath);
	printUserDataAndRentedCars(ul);

	char choice;
	do {
		cout << "\nPlease select an option:" << endl;
		cout << "1. Add a new user" << endl;
		cout << "2. Delete a user" << endl;
		cout << "3. Update user information" << endl;
		cout << "4. Sort user list by last name" << endl;
		cout << "5. Rent a car" << endl;
		cout << "6. Delete rented cars before a specified date" << endl;
		cout << "7. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case '1': {
			user newUser;
			cout << "Enter user ID: ";
			cin >> newUser.userID;
			cout << "Enter first name of the user: ";
			cin >> newUser.fname;
			cout << "Enter the last name of the user: ";
			cin >> newUser.lname;
			newUser.rentedCar = nullptr;
			addUser(ul, newUser);
			writeToFiles(ul, usersFilePath, rentedCarsFilePath);
			printUserDataAndRentedCars(ul);
			break;
		}
		case '2': {
			int userIDToDelete;
			cout << "Enter user ID to delete: ";
			cin >> userIDToDelete;
			deleteUser(ul, userIDToDelete);
			writeToFiles(ul, usersFilePath, rentedCarsFilePath);
			printUserDataAndRentedCars(ul);
			break;
		}
		case '3': {
			updateUser(ul);
			writeToFiles(ul, usersFilePath, rentedCarsFilePath);
			printUserDataAndRentedCars(ul);
			break;
		}
		case '4': {
			Insertionsort(ul);
			writeToFiles(ul, usersFilePath, rentedCarsFilePath);
			cout << "\nUser list after sorting by last name:" << endl;
			printUserDataAndRentedCars(ul);
			break;
		}
		case '5': {
			string startDate, endDate;
			cout << "Enter start date (DD-MM-YYYY): ";
			cin >> startDate;
			cout << "Enter end date (DD-MM-YYYY): ";
			cin >> endDate;
			rentCar(ul, ul->head, startDate, endDate);
			writeToFiles(ul, usersFilePath, rentedCarsFilePath);
			printUserDataAndRentedCars(ul);
			break;
		}
		case '6': {
			string specifiedDate;
			cout << "Enter the specified date (DD-MM-YYYY): ";
			cin >> specifiedDate;
			deleteRentedCarsBeforeDate(ul, specifiedDate);
			writeToFiles(ul, usersFilePath, rentedCarsFilePath);
			printUserDataAndRentedCars(ul);
			break;
		}
		case '7': {
			cout << "Exiting program. Thank you!" << endl;
			break;
		}
		default: {
			cout << "Invalid choice. Please enter a valid option (1-7)." << endl;
		}
		}
	} while (choice != '7');

	return 0;
	
}