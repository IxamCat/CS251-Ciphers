#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "include/caesar_dec.h"
#include "include/caesar_enc.h"
#include "include/subst_dec.h"
#include "include/subst_enc.h"
#include "utils.h"

using namespace std;

// Initialize random number generator in .cpp file for ODR reasons
std::mt19937 Random::rng;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// helper function to load dictionary

void decodeSubstCipherFromFile(const QuadgramScorer& scorer);

vector<string> loadInDictionary(const string& dictFileName) {
  vector<string> dict;
  ifstream dictFile(dictFileName);

  if (!dictFile) {
    return {};
  }
  string entry;
  while (getline(dictFile, entry)) {
    dict.push_back(clean(entry));
  }
  dictFile.close();
  return dict;
}

// quadgramscorer constructor

QuadgramScorer initializeQuadgramScorer(const string& quadFileName) {
  ifstream quadFile(quadFileName);
  string quadLine;
  vector<string> quadgrams;
  vector<int> counts;

  while (getline(quadFile, quadLine)) {
    stringstream quadStream(quadLine);
    string quadSec;
    int countStr;

    if (getline(quadStream, quadSec, ',') && quadStream >> countStr) {
      quadgrams.push_back(quadSec);
      counts.push_back(countStr);
    }
  }
  return QuadgramScorer(quadgrams, counts);
}
// Function declarations go at the top of the file so we can call them
// anywhere in our program, such as in main or in other functions.
// Most other function declarations are in the included header
// files.

// When you add a new helper function, make sure to declare it up here!

/**
 * Print instructions for using the program.
 */
void printMenu();

int main() {
  Random::seed(time(NULL));
  string command;

  cout << "Welcome to Ciphers!" << endl;
  cout << "-------------------" << endl;
  cout << endl;

  vector<string> dictionary = loadInDictionary("dictionary.txt");

  QuadgramScorer scorer = initializeQuadgramScorer("english_quadgrams.txt");

  do {
    printMenu();
    cout << endl << "Enter a command (case does not matter): ";

    // Use getline for all user input to avoid needing to handle
    // input buffer issues relating to using both >> and getline
    getline(cin, command);
    cout << endl;

    if (command == "R" || command == "r") {
      string seed_str;
      cout << "Enter a non-negative integer to seed the random number "
              "generator: ";
      getline(cin, seed_str);
      Random::seed(stoi(seed_str));
    } else if (command == "C" || command == "c") {
      caesarEncryptCommand();
    } else if (command == "D" || command == "d") {
      caesarDecryptCommand(dictionary);
    } else if (command == "A" || command == "a") {
      applyRandSubstCipherCommand();
    } else if (command == "E" || command == "e") {
      computeEnglishnessCommand(scorer);
    } else if (command == "S" || command == "s") {
      decryptSubstCipherCommand(scorer);
    } else if (command == "F" || command == "f") {
      decodeSubstCipherFromFile(scorer);
    }

    cout << endl;

  } while (!(command == "x" || command == "X") && !cin.eof());

  return 0;
}

void printMenu() {
  cout << "Ciphers Menu" << endl;
  cout << "------------" << endl;
  cout << "C - Encrypt with Caesar Cipher" << endl;
  cout << "D - Decrypt Caesar Cipher" << endl;
  cout << "E - Compute English-ness Score" << endl;
  cout << "A - Apply Random Substitution Cipher" << endl;
  cout << "S - Decrypt Substitution Cipher from Console" << endl;
  cout << "F - Decrypt Substitution Cipher from File" << endl;
  cout << "R - Set Random Seed for Testing" << endl;
  cout << "X - Exit Program" << endl;
}

// "#pragma region" and "#pragma endregion" group related functions in this file
// to tell VSCode that these are "foldable". You might have noticed the little
// down arrow next to functions or loops, and that you can click it to collapse
// those bodies. This lets us do the same thing for arbitrary chunks!
#pragma region CaesarEnc

char rot(char c, int amount) {
  // TODO: student
  int pos = ALPHABET.find(c);
  int new_pos = (pos + amount) % 26;

  return ALPHABET.at(new_pos);
}

string rot(const string& line, int amount) {
  // TODO: student
  string res = "";

  for (char c : line) {
    if (isalpha(c)) {
      // converting here to uppercase and then rotating
      res += rot(toupper(c), amount);
    } else if (isspace(c)) {  // if the character is a space, append it to a
                              // result string
      res += c;
    }
  }
  return res;
}

void caesarEncryptCommand() {
  // TODO: student
  string textInput;
  string rotationInput;

  cout << "enter text for encryption: ";
  getline(cin, textInput);

  cout << "enter an amount for rotation (choice of 0-25): ";
  getline(cin, rotationInput);
  int amountRotate = stoi(rotationInput);

  string textEncryption = rot(textInput, amountRotate);
  string text;
  cout << "displaying encrypted text: " << textEncryption << endl;
}

#pragma endregion CaesarEnc

#pragma region CaesarDec

void rot(vector<string>& strings, int amount) {
  // TODO: student
  for (string& word : strings) {
    word = rot(word, amount);
  }
}

string clean(const string& s) {
  // TODO: student
  string cleaned = "";
  for (char c : s) {
    if (isalpha(c)) {
      cleaned += toupper(c);
    }
  }
  return cleaned;
}

vector<string> splitBySpaces(const string& s) {
  // TODO: student
  vector<string> resVec;
  string tempStr;

  for (char c : s) {
    if (isspace(c)) {
      if (!tempStr.empty()) {
        resVec.push_back(tempStr);
        tempStr.clear();
      }
    } else {
      tempStr += c;
    }
  }
  if (!tempStr.empty()) {
    resVec.push_back(tempStr);
  }
  return resVec;
}

string joinWithSpaces(const vector<string>& words) {
  // TODO: student
  string output;

  for (size_t i = 0; i < words.size(); i++) {
    output += words[i];
    if (i < words.size() - 1) {
      output += " ";
    }
  }
  return output;
}

int numWordsIn(const vector<string>& words, const vector<string>& dict) {
  // TODO: student
  int numCount = 0;

  for (const string& word : words) {
    if (find(dict.begin(), dict.end(), word) != dict.end()) {
      numCount++;
    }
  }
  return numCount;
}

void caesarDecryptCommand(const vector<string>& dict) {
  // TODO: student

  cout << "enter text for decryption: ";
  string input;
  getline(cin, input);

  vector<string> words = splitBySpaces(input);

  bool foundDecrypt = false;  // tracking if any valid decrypt was found

  // trying all possible rotations
  for (int rotate = 0; rotate < 26; rotate++) {
    vector<string> rotatedWords = words;  // creating a copy of original words
    rot(rotatedWords, rotate);  // rotating words by current rotation val

    int decryptCount = numWordsIn(rotatedWords, dict);

    if (decryptCount > rotatedWords.size() / 2) {
      // if more than half are valid
      // prnt the decrypted text for the valid rotations
      // string decryptedText = joinWithSpaces(rotatedWords);
      cout << joinWithSpaces(rotatedWords) << endl;
      foundDecrypt = true;
    }
  }
  if (!foundDecrypt) {
    cout << "No good decryptions found" << endl;
  }
}

#pragma endregion CaesarDec

#pragma region SubstEnc

string applySubstCipher(const vector<char>& cipher, const string& s) {
  // TODO: student
  string encodedText = s;

  for (char& ch : encodedText) {
    if (isalpha(ch)) {
      ch = cipher[toupper(ch) - 'A'];
    }
  }
  return encodedText;
}

void applyRandSubstCipherCommand() {
  // TODO: student
  string Inputuser;
  cout << "please enter text for encryption: ";
  getline(cin, Inputuser);

  vector<char> randoCipher = genRandomSubstCipher();
  string cipherOutput = applySubstCipher(randoCipher, Inputuser);

  cout << "displaying encrypted text: " << cipherOutput << endl;
}

#pragma endregion SubstEnc

#pragma region SubstDec

double scoreString(const QuadgramScorer& scorer, const string& s) {
  // TODO: student

  double cumulativeScore = 0.0;

  for (size_t ind = 0; ind <= s.size() - 4; ind++) {
    string quadStr = s.substr(ind, 4);
    cumulativeScore += scorer.getScore(quadStr);
  }
  return cumulativeScore;
}

void computeEnglishnessCommand(const QuadgramScorer& scorer) {
  // TODO: student

  string userInput;

  cout << "please enter a string to score: ";
  getline(cin, userInput);

  string formattedText = clean(userInput);

  if (formattedText.size() < 4) {
    cout << "the englishness score is: 0.0" << endl;
    return;
  }
  double computedScore = scoreString(scorer, formattedText);

  cout << "displaying englishness score: " << computedScore << endl;
}

// helper functions for substitution cipher decrypter (console input)

void randomLetterSwap(vector<char>& cipher) {
  int firstIndex = Random::randInt(25);
  int secondIndex;

  do {
    secondIndex = Random::randInt(25);
  } while (firstIndex == secondIndex);

  swap(cipher[firstIndex], cipher[secondIndex]);
}

double calculateScore(const QuadgramScorer& scorer, const vector<char>& cipher,
                      const string& ciphertext) {
  string textDecryption = applySubstCipher(cipher, ciphertext);
  return scoreString(scorer, clean(textDecryption));
}

vector<char> refineSubstCipher(const QuadgramScorer& scorer,
                               const string& ciphertext) {
  int unsucessfulAttempts = 0;
  vector<char> optimalCipher = genRandomSubstCipher();
  double optimalScore = calculateScore(scorer, optimalCipher, ciphertext);

  while (unsucessfulAttempts < 1000) {
    vector<char> newCipher = optimalCipher;
    randomLetterSwap(newCipher);

    double evaluatedScore = calculateScore(scorer, newCipher, ciphertext);

    if (evaluatedScore > optimalScore) {
      optimalCipher = newCipher;
      optimalScore = evaluatedScore;
      unsucessfulAttempts = 0;
    } else {
      unsucessfulAttempts++;
    }
  }
  return optimalCipher;
}
vector<char> decryptSubstCipher(const QuadgramScorer& scorer,
                                const string& ciphertext) {
  // TODO: student
  return refineSubstCipher(scorer, ciphertext);
}

void decryptSubstCipherCommand(const QuadgramScorer& scorer) {
  // TODO: student

  string ciphertext;
  cout << "please enter text for substituion cipher decrypt: ";
  getline(cin, ciphertext);

  vector<char> bestCipherKey;
  double bestCipherScore = -1000000.0;

  for (int k = 0; k < 25; k++) {
    vector<char> canidateKey = refineSubstCipher(scorer, ciphertext);
    double canidateScore = calculateScore(scorer, canidateKey, ciphertext);

    if (canidateScore > bestCipherScore) {
      bestCipherKey = canidateKey;
      bestCipherScore = canidateScore;
    }
  }
  string finalDecryption = applySubstCipher(bestCipherKey, ciphertext);
  cout << "displaying decrypted text: " << finalDecryption << endl;
}

#pragma endregion SubstDec

// implementing the decrypt substitution cipher file
void decodeSubstCipherFromFile(const QuadgramScorer& scorer) {
  string cipherFileName;
  string decodedFileName;

  cout << "please enter the filename with the text to decrypt: ";
  getline(cin, cipherFileName);
  cout << "please ener filename to save the decrypted text: ";
  getline(cin, decodedFileName);

  ifstream cipherinputFile(cipherFileName);
  if (!cipherinputFile) {
    cout << "unable to open the file " << cipherFileName << endl;
    return;
  }
  string ciphertext;
  stringstream buffer;
  buffer << cipherinputFile.rdbuf();
  ciphertext = buffer.str();
  cipherinputFile.close();

  vector<char> bestCipherKey;
  double CipherScore = -1000000.0;

  for (int j = 0; j < 20; j++) {
    vector<char> cipherKey = refineSubstCipher(scorer, ciphertext);
    double score = calculateScore(scorer, cipherKey, ciphertext);

    if (score > CipherScore) {
      bestCipherKey = cipherKey;
      CipherScore = score;
    }
  }

  string decryptionText = applySubstCipher(bestCipherKey, ciphertext);
  ofstream decodeFile(decodedFileName);

  if (!decodeFile) {
    cout << "unable to create file " << decodedFileName << endl;
    return;
  }
  decodeFile << decryptionText;
  decodeFile.close();

  cout << "the decryption is complete. results are saved to: "
       << decodedFileName << endl;
}