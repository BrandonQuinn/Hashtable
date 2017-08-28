/* 
	==========================================
	               Brandon Quinn
	                  bq415
	                  Lab 5
	               28 Aug 2017

	    Marking Notes: storing the length 
	    inside each node is wasteful on
	    space, instead it's better to use
	    a global integer which keeps track of
	    the longest chain. Other than that,
	    not bad.
	========================================== 
*/

#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;

const int MAX_FILENAME_LEN = 32;
const int TABLE_LEN = 100;

struct HashNode 
{
	int length; // number of values in the chain
	int value;
	HashNode* next;
	HashNode* end;
};

HashNode hashtable[TABLE_LEN];

void Insert(const int);
int inline Hash(const int);

int main() 
{
	char filename[MAX_FILENAME_LEN];
	ifstream file;

	cout << "Enter filename: ";
	cin.getline(filename, MAX_FILENAME_LEN, '\n');

	file.open(filename);
	if (file.bad()) {
		cout << "Error: could not open file. Exiting." << endl;
		exit(EXIT_FAILURE);
	}

	// init hashtable
	for (int i = 0; i < TABLE_LEN; i++) {
		hashtable[i].length = 0;
		hashtable[i].value = 0;
		hashtable[i].next = NULL;
		hashtable[i].end = &hashtable[i]; // point to itself first
	}

	// read in values and insert
	int newValue = 0;
	while (!file.eof()) {
		file >> newValue;
		Insert(newValue);
	}

	// get empty entries and longest chain
	int emptyEntriesCount = 0;
	int longestChainLen = 0;
	for (int i = 0; i < TABLE_LEN; i++) {
		if (hashtable[i].length > longestChainLen)
			longestChainLen = hashtable[i].length;

		if (hashtable[i].length == 0)
			emptyEntriesCount++;
	}

	cout << "Empty entries: " << emptyEntriesCount << endl;
	cout << "Longest chain length: " << longestChainLen << endl;

	// free memory
	HashNode* deleteNode = NULL;
	for (int i = 0; i < TABLE_LEN; i++) {
		HashNode* currentNode = hashtable[i].next;

		if (hashtable[i].length <= 1) continue; // skip node not using dynamic allocation

		while (currentNode != NULL) {
			deleteNode = currentNode;
			currentNode = currentNode->next;
			delete deleteNode;
		}
	}

	return 0;
}

void Insert(const int value) 
{
	HashNode* node = &hashtable[Hash(value)];
	if (node->length == 0) { // if 0 then the value is free to write to
		node->value = value;
		node->length++;
		return;
	}

	// append a new dynamically allocated node
	node->length++;
	HashNode* endNode = node->end; 	// end node
	endNode->next = new HashNode; 	// the next value of the end node is the new node
	endNode->next->value = value;	// set the value of the new end node
	endNode->next->next = NULL;
	node->end = endNode->next; 		// move the end over 1 to the new node
}

int inline Hash(const int value) 
{
	return value % TABLE_LEN;
}