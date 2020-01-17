#include <iostream>
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <omp.h>

#include "grams_computing.h"

using namespace std;

int main(int argc, char *argv[]) {
    const char *kInputPath = argv[1];
    const char *kOutputPath = argv[2];
    ifstream input, size;
    ofstream output_words, output_letters;
    string ngram[NGRAM_LENGTH], line, words_ngram;
    int words_index, letters_index = 0;
    char next_char, letters_ngram[NGRAM_LENGTH + 1] = "";
    char *buffer;
    unordered_map<string, int> letters_hashtable, words_hashtable;

    //start time
    double start = omp_get_wtime();

    //get file size
    size.open(kInputPath, ios::ate);
    const long int file_size = size.tellg();
    size.close();

    //read file
    buffer = new char[file_size];
    input.open(kInputPath, ios::binary);
    input.read(buffer, file_size);
    input.close();

    for (int i = 0; i < file_size; i++) {
        next_char = buffer[i];
        //words N-grams
        if (GramsComputing::computeWords(next_char, ngram, words_index)) {
            words_ngram = ngram[0] + " " + ngram[1];
            words_hashtable[words_ngram] += 1;
            if (!(isalpha(next_char) || isspace(next_char))) //check if it is a group terminator
                words_index = 0;
            else {
                words_index = NGRAM_LENGTH - 1;
                GramsComputing::shiftArrayOfStrings(ngram);
            }
        }
        //letters N-grams
        if (GramsComputing::computeLetters(next_char, letters_ngram, letters_index)) {
            letters_hashtable[letters_ngram] += 1;
            letters_ngram[0] = letters_ngram[1];
            letters_index = 1;
        }
    }

    //end time
    double elapsed_time = omp_get_wtime() - start;

    /* PRINT RESULTS */
    output_words.open((string) kOutputPath + "output_words.txt", ios::binary);
    output_letters.open((string) kOutputPath + "output_letters.txt", ios::binary);

    for (auto map_iterator = words_hashtable.begin(); map_iterator != letters_hashtable.end(); ++map_iterator)
        output_words << map_iterator->first << " : " << map_iterator->second << endl;
    output_words << words_hashtable.size() << " elements found." << endl;

    for (auto &map_iterator : letters_hashtable)
        output_letters << map_iterator.first << " : " << map_iterator.second << endl;
    output_letters << letters_hashtable.size() << " elements found." << endl;

    output_words.close();
    output_letters.close();

    cout << elapsed_time << endl;

    return 0;
}