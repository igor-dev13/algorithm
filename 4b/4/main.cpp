#include "stdafx.h"
#include "hash.h"

#include <iostream>
#include <string>

using namespace std;
const int maxNameLength = 30;

class Person // Класс, который содержит немного информации о человеке.
{
public:
	string name;
	int number;
	int deleted;

	Person()
	{
		this->name = "";
		this->deleted = 0;
	}

	Person(string name, int age = 0)
	{
		this->name = name;
		this->number = number;
		this->deleted = 0;
	}

	~Person()
	{
	}
};

int GetPersonCount()
{
	char *str = new char[1024];
	int stringCount = 0;

	ifstream base("names.txt");
	if (!base)
		return -1;

	while (!base.eof())
	{
		base.getline(str, 1024, '\n');
		stringCount++;
	}
	base.close();
	delete str;
	return stringCount - 1;
}

int Hash(int &indexForHash, int number, Person* &persons, int tableSize)
{
	int power = 2;
	int attempt = 1;
	int index = number % (tableSize - 1);

	if (indexForHash == 0)
	{
		indexForHash = index;
	}	
	while (persons[index].name != "")
	{
		cout << "Коллизия, индекс - " << index << " Занят " << persons[index].name << endl;

		index = (indexForHash + (int)pow(attempt, power)) % tableSize;
		attempt++;
		// запись следующего элемента при коллизии
	}
	return index;
}

void CreateIndexes(int &indexForHash, int &personCount, Person* &arrPersons, map<int, int> &indexes)
{
	FILE *input = 0;
	input = fopen("names.txt", "r");

	int number = 0;
	char name[maxNameLength] = "";

	for (int i = 0; i < personCount; i++)
	{
		fscanf(input, "%d %s", &number, &name);		
		int placeInArray = Hash(indexForHash, number, arrPersons, personCount * 2);
		indexes.insert(make_pair(number, placeInArray));
		arrPersons[placeInArray].name = name;
		arrPersons[placeInArray].number = number;
	}
}

int GetHashNumber(const int &searchingNumber, map<int, int> &indexes)
{
	try
	{
		map<int, int>::iterator iterator;
		iterator = indexes.find(searchingNumber);

		if (iterator != indexes.end())
		{
			return indexes.find(searchingNumber)->second;
		}
		else
		{
			return -1;
		}
	}
	catch (const std::exception&e)
	{
		cout << e.what() << endl;
	}
	return -1;
}

int CreateBook(map<int, int> &indexes, int &indexForHash)
{
	try
	{
		int persCount = GetPersonCount();
		if (persCount == -1)
			return -1;
		
		Person* tmpPersons = new Person[persCount * 2];		
		indexes.clear();		
		CreateIndexes(indexForHash, persCount, tmpPersons, indexes);

		ofstream out("index.bin", ios::binary | ios::out);
		out.write((char*)&persCount, sizeof(int));
		out.write((char*)&indexForHash, sizeof(int));

		map<int, int>::iterator iterator;
		for (iterator = indexes.begin(); iterator != indexes.end(); iterator++)
		{
			out.write((char*)&iterator->first, sizeof(int));
			out.write((char*)&iterator->second, sizeof(int));
		}

		for (int i = 0; i < persCount * 2; i++)
		{
			out.write((char*)&tmpPersons[i], sizeof(Person));
		}
		out.close();
		delete[] tmpPersons;
		indexes.clear();
	}
	catch (const std::exception&e)
	{
		cout << e.what() << endl;
	}
	cout << "Indexes has been created" << endl;
	return 1;
}

int LoadBook(int &personCount, int &indexForHash, Person* &arrPersons, map<int,int> &indexes)
{
	FILE *iofile = NULL;
	iofile = fopen("index.bin", "rb");
	fread(&personCount, sizeof(int), 1, iofile);
	fread(&indexForHash, sizeof(int), 1, iofile);
	arrPersons = new Person[personCount * 2];

	for (int i = 0; i < personCount; i++)
	{
		int first, second;
		fread(&first, sizeof(int), 1, iofile);
		fread(&second, sizeof(int), 1, iofile);
		indexes.insert(pair<int, int>(first, second));		
	}

	for (int i = 0; i < personCount * 2; i++)
	{
		if (!feof(iofile))
		{
			fread(&arrPersons[i], sizeof(Person), 1, iofile);
			// cout << arrPersons[i].number << " " << arrPersons[i].name << endl;
		}
		else
		{
			arrPersons[i].name = "";
			arrPersons[i].number = 0;
			arrPersons[i].deleted = 0;			
		}
	}

	fclose(iofile);
	cout << "Book loaded..." << endl;
	return 1;
}

void FindPerson(int &number, Person &singlePerson, int &personCount, map<int,int> &indexes)
{
	FILE *iofile = NULL;
	
	iofile = fopen("index.bin", "rb");
	int hashNumber = GetHashNumber(number, indexes);

	fseek(iofile, (sizeof(int) + sizeof(int) + sizeof(int) * 2 * personCount + sizeof(Person) * (1 * hashNumber)), SEEK_SET);
	fread(&singlePerson, sizeof(Person), 1, iofile);
	if (singlePerson.name != "" && singlePerson.deleted == 0)
	{
		cout << singlePerson.name << endl;
	}
	else
	{
		cout << "can't find eny result" << endl;
	}
	fclose(iofile);	
}

void SaveToBinary(int indexForHash, int &personCount, Person* &arrPersons, map <int, int> &indexes)
{
	ofstream out("index.bin", ios::binary | ios::out);
	out.write((char*)&personCount, sizeof(int));
	out.write((char*)&indexForHash, sizeof(int));

	map<int, int>::iterator iterator;
	for (iterator = indexes.begin(); iterator != indexes.end(); iterator++)
	{
		out.write((char*)&iterator->first, sizeof(int));
		out.write((char*)&iterator->second, sizeof(int));
	}

	for (int i = 0; i < personCount * 2; i++)
	{
		out.write((char*)&arrPersons[i], sizeof(Person));
	}
	out.close();
}

void InsertToBook(int &indexForHash, map<int,int> &indexes, Person* &arrPersons, int &personCount)
{
	cout << "Enter phone number 1st line and name 2nd line:" << endl;
	int number = 0;
	char name[30] = "";

	cin >> number;
	cin >> name;

	ofstream fileOut;
	char numberTxt[maxNameLength];
	sprintf(numberTxt, "%d", number);

	fileOut.open("names.txt", ios::app);
	fileOut.write(numberTxt, strlen(numberTxt));
	fileOut.write(" ", strlen(" "));
	fileOut.write(name, strlen(name));
	fileOut << endl;
	fileOut.close();
}

void DeleteFromBook(Person &singlePerson, const int &personCount, map<int,int> &indexes, Person* &arrPersons)
{
	cout << "Enter phone number:" << endl;
	int number;
	string name = "";
	FILE *iofile = NULL;
	cin >> number;

	int hashNumber = GetHashNumber(number, indexes);
	cout << hashNumber << endl;

	if (hashNumber != -1)
	{
		arrPersons[hashNumber].deleted = 1;
		cout << arrPersons[hashNumber].name << " number removed" << endl;
	}
	else 
	{
		cout << "Number not found" << endl;
	}
}

void HandleCommand(istream &input, int indexForHash, int &personCount, Person &singlePerson, Person* &arrPersons, map <int,int> &indexes)
{
	cout << "possible commands:\n"
		<< "1. Create Book\n"
		<< "2. Open Book\n"
		<< "3. Add record to Book\n"
		<< "4. Delete record from Book\n"
		<< "5. Find record\n"
		<< "Ctrl + C - exit\n";

	Person* searchingPerson;
	bool isBookOpen = false;
	int commandInt;
	string exitCommand = "...";
	cout << ">";

	while (cin >> commandInt)
	{
		switch (commandInt)
		{
		case 1:
			CreateBook(indexes, indexForHash);
			break;
		case 2:
			LoadBook(personCount, indexForHash, arrPersons, indexes);
			break;
		case 3:
			InsertToBook(indexForHash, indexes, arrPersons, personCount);
			CreateBook(indexes, indexForHash);
			LoadBook(personCount, indexForHash, arrPersons, indexes);
			break;
		case 4:
			DeleteFromBook(singlePerson, personCount, indexes, arrPersons);
			SaveToBinary(indexForHash, personCount, arrPersons, indexes);
			break;
		case 5:
			cout << "Enter number or 0 to exit:\n>";
			while (cin >> commandInt)
			{
				if (commandInt == 0)
					break;
				FindPerson(commandInt, singlePerson, personCount, indexes);
				cout << ">";
			}
			break;
		default:
			cout << "Error, bad input, quitting\n";
			break;
		}
		cout << "\n" << ">";
	}
}

int main()
{
	setlocale(LC_ALL, "rus");

	int indexForHash = 0;
	int personCount;
	Person singlePerson;	
	Person* arrPersons = 0;
	map<int, int> indexes;	

	HandleCommand(cin, indexForHash, personCount, singlePerson, arrPersons, indexes);

	delete[] arrPersons;
	return 0;
}

