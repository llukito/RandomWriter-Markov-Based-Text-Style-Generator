#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include "console.h"
#include "simpio.h"
#include "map.h"
#include "vector"
#include "random.h"
#include "unordered_set"

// constants of file names (we train our program on these)
const string HAMLET_FILE = "HamletTrain.txt";
const string SPONGEBOB_FILE = "SpongeBob.txt";
const string ELONMUSK_FILE = "ElonMusk.txt";

// constants of different barriers I've set
const int NUM_OF_TRAINING_FILES = 3;
const int NUM_OF_WORDS_MERGED = 10;
const int MIN_WORD_TO_GENERATE = 100;

// for better readebility
enum { shakespeare = 1, spongebob = 2, elonMusk = 3 };

// function prototypes
int getValidChoice();
void trainOnStyle(unordered_map<string, vector<string>>& , int);
string getFile(int);
void storeFileContent(ifstream&, unordered_map<string, vector<string>>&);
string cleanWord(const string&);
bool isValidChar(char&);
ifstream getFileToGenerate();
void countFrequencies(ifstream&, unordered_map<string, int>&);
void storeFrequencies(istream&, unordered_map<string, int>&);
void mergeTrainingMapWithTopWords(unordered_map<string, vector<string>>&, unordered_map<string, int>&, int);
void getTopNWordsFromFile(unordered_map<string, int>&, int&, vector<string>&);
void addTopWordsInMap(unordered_map<string, vector<string>>&, vector<string>&);
void generateLogicalText(unordered_map<string, vector<string>>&);
int getValidNumber();
void generateFinalText(unordered_map<string, vector<string>>&, int&);
string getMostFrequentWord(unordered_map<string, vector<string>>&);
void printNextWord(vector<string>&, string&, int&);


int main() {
	while (true) { // testing
		int choice = getValidChoice();
		unordered_map<string, vector<string>> trainingMap;
		trainOnStyle(trainingMap, choice);

		unordered_map<string, int> inputFileMap;
		ifstream file = getFileToGenerate();
		countFrequencies(file, inputFileMap);

		mergeTrainingMapWithTopWords(trainingMap, inputFileMap, NUM_OF_WORDS_MERGED);
		generateLogicalText(trainingMap);
	}
	return 0;
}

/*
* Makes sure user enters valid number,
* which is from 1 to 3, cause right now
* we have three different writing styles
* (Notice that if you change number of training files,
* you need to change constants and program will work fine)
*/
int getValidChoice() {
	while (true) {
		cout << "Choose writing style" << endl;
		cout << "1)Shakespeare 2)Spongebob 3)ElonMusk" << endl;
		int n = getInteger("Choice : ");
		if (n >= 1 && n <= NUM_OF_TRAINING_FILES) {
			return n;
		}
		cout << "Enter number in the range of 1-"<< NUM_OF_TRAINING_FILES << endl;
	}
}

/*
* traininMap is a map which will remember what strings come after specific string,
* it differs from markov model, cause we don't store chars but full words themselves
*/
void trainOnStyle(unordered_map<string, vector<string>>& trainingMap, int choice) {
	string fileName = getFile(choice);
	ifstream file(fileName.c_str());
	if (file.is_open()) {
		storeFileContent(file, trainingMap);
		file.close();
	}
	else {
		cout << "Error occured while opening training file" << endl;
	}
}

/*
* Returns right name for file (if we add more files we just
* need to write more if-esle conditions)
*/
string getFile(int choice) {
	if (choice == shakespeare) {
		return HAMLET_FILE;
	}
	else if (choice == spongebob) {
		return SPONGEBOB_FILE;
	}
	else { // elonMusk :)
		return ELONMUSK_FILE;
	}
}

/*
* This function extracts words from lines with string stream and then
* stores in our traininMap in markov style ( but whole strings not chars)
*/
void storeFileContent(ifstream& file, unordered_map<string, vector<string>>& trainingMap) {
	string line;
	string prevWord;
	while (getline(file, line)) {
		stringstream stream(line);
		string word;
		while (stream >> word) {
			word = cleanWord(word);
			if (!word.empty()) {
				if (!prevWord.empty()) {
					trainingMap[prevWord].push_back(word);
				}
				prevWord = word;
			}
		}
	}
}

/*
* This function cleans words from non alphanumeric and digit chars.
* We just need row words without commas, dots, ...
*/
string cleanWord(const string& word) {
	string result;
	for (char c : word) {
		if (isValidChar(c)) {
			result += tolower(c);
		}
	}
	return result;
}

/*
* I know that isalpha, isdigit and tolower functions exist, but they errored
* program when chars with non defined aski code were entered, so I preferred
* to manually check with aski values
*/
bool isValidChar(char& c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' || c <= '9');
}

/*
* Makes user enter valid file name with which
* we generate random text
*/
ifstream getFileToGenerate() {
	while (true) {
		string fileName = getLine("Enter file name : ");
		ifstream file(fileName.c_str());
		if (file.is_open()) {
			return file;
		}
		cout << "Please enter valid file name" << endl;
	}
}

/*
* With the file user inputed, we will just store frequencies.
* We don't need markov model for it
*/
void countFrequencies(ifstream& file, unordered_map<string, int>& wordFreq) {
	if (file.is_open()) {
		storeFrequencies(file, wordFreq);
		file.close();
	}
	else {
		cout << "Error opening file!" << endl;
	}
}

void storeFrequencies(istream& file, unordered_map<string, int>& wordFreq) {
	string line;
	while (getline(file, line)) {
		stringstream stream(line);
		string word;
		while (stream >> word) {
			word = cleanWord(word);
			if (!word.empty()) {
				wordFreq[word]++;
			}
		}
	}
}

/*
* This function is the main part of our program. What we do here is that
* we choose top N most frequently seens words in userInputFile and then
* add these into our trainingMap. We just merge these two texts, cause
* we want inputFile to adopt chosen writing style
*/
void mergeTrainingMapWithTopWords(
	unordered_map<string, vector<string>>& trainingMap,
	unordered_map<string, int>& frequencyMap,
	int topN) {

	vector<string> topWords;
	getTopNWordsFromFile(frequencyMap, topN, topWords);
	addTopWordsInMap(trainingMap, topWords);
}

/*
* We extract top N numbers from user input file. To do that, we first turn it into pairs
* and then sort them in descending order
*/
void getTopNWordsFromFile(unordered_map<string, int>& frequencyMap, int& topN, vector<string>& topWords) {
	vector<pair<string, int>> wordList(frequencyMap.begin(), frequencyMap.end());
	// we sort it by frequency descending
	sort(wordList.begin(), wordList.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
		return a.second > b.second;
	});
	// store top N words in topWords vector
	for (int i = 0; i < topN && i < wordList.size(); i++) {
		topWords.push_back(wordList[i].first);
	}
}

/*
* We add topWords for every key in values of trainingMap
*/
void addTopWordsInMap(unordered_map<string, vector<string>>& trainingMap, vector<string>& topWords) {
	for (auto& entry : trainingMap) {
		vector<string>& nextWords = entry.second;
		for (string& word : topWords) {
			nextWords.push_back(word);
		}
	}
}

void generateLogicalText(unordered_map<string, vector<string>>& trainingMap) {
	int totalWordsToGenerate = getValidNumber();
	generateFinalText(trainingMap, totalWordsToGenerate);
	cout << endl;
}

/*
* It's better if user enters more than 100 words to be generated, so 
* our program returns a bit more interesting text, but you can change
* constant if you want to change barrier
*/
int getValidNumber() {
	while (true) {
		int n = getInteger("Enter total words to be generated: ");
		if (n >= MIN_WORD_TO_GENERATE)return n;
		cout << "Choose at least "<< MIN_WORD_TO_GENERATE << endl;
	}
}

/*
* We will generate text with same logic as markov, but we will
* add strings and not chars.
*/
void generateFinalText(unordered_map<string, vector<string>>& trainingMap, int& totalStrings) {
	string mostFrequent = getMostFrequentWord(trainingMap);
	cout << mostFrequent;
	int printedWords = 1; // we already printed one word
	string currentSeed = mostFrequent;
	while (printedWords < totalStrings) {
		vector<string> followedStrings = trainingMap[currentSeed];
		if (followedStrings.empty()) {
			break; // if seed appears at the end of text(early exit)
		}
		printNextWord(followedStrings, currentSeed, printedWords);
		printedWords++;
	}
}

/*
* Returns most frequently seen word to start generating text from
* that word(similar to markov model)
*/
string getMostFrequentWord(unordered_map<string, vector<string>>& markovMap) {
	int maxSize = 0;
	string resultSequence = "";
	for (auto& entry : markovMap) {
		string sequence = entry.first;
		int sizeOfVector = entry.second.size();
		if (sizeOfVector > maxSize) {
			maxSize = sizeOfVector;
			resultSequence = sequence;
		}
	}
	return resultSequence;
}

/*
* Prints next number, it chooses randomly on followedStrings, but probabilities
* have a big role here( there might be same word in vector for mutliple times)
*/
void printNextWord(vector<string>& followedStrings, string& currentSeed, int& printedWords) {
	int randomIndex = randomInteger(0, followedStrings.size() - 1);
	string randomString = followedStrings[randomIndex];
	cout <<" " << randomString;
	currentSeed = randomString;
}
