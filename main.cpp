/* CSI 3334
 * Project 4 -- AVL Tree
 * Filename: main.cpp
 * Name: Eugene Pak
 * Version 1.3
 * Due: 3/22/24
 * This project reads in instruction letters to create an avl-balanced BST object.
 * It also allows for a path key to be used to decrypt a message, and also use a
 * stream of words to encrypt to a path key.
 */

#include <iostream>
#include <sstream>
#include "avl-tree-student-proj4.h"

using namespace std;

/* main
 * This project reads in instruction letters to create a binary search tree with
 * inserts and removes until the letter q is read. When the letter i is read, the
 * word that follows is inserted into the tree. When the letter r is read, the word
 * that follows is removed from the tree. When the letter e is read, a stream of
 * words is read and encrypted into a path of keys. When the letter d is read, a
 * stream of keys are read and decrypted into a stream of words.
 *  parameters:
 *      argc -- the number of arguments from the command line
 *      argv -- the command line argument values
 *  return value: 0 (indicating a successful run)
 *
 */
const int MAX = 10000;
int main(int argc, char**argv) {
    EncryptionTree<string> tree;
    int count = 0;
    char instruction;
    string word, word2;
    bool done = false;
    cin >> instruction;
    while (!done && count <= MAX){
        count++;
        if (instruction == 'i'){
            cin >> word;
            tree.insert(word);
        }
        else if (instruction == 'r'){
            cin >> word;
            tree.remove(word);
        }
        else if (instruction == 'e'){
            cin.get();
            getline(cin, word);
            word.erase(word.begin(), word.begin() + 1);
            word.erase(word.begin() + word.length() - 1);
            istringstream buffer (word);
            while (buffer >> word2){
                cout << tree.encrypt(word2);
                if (!buffer.eof()){
                    cout  << " ";
                }
            }
            cout << endl;
        }
        else if (instruction == 'd'){
            cin.get();
            getline(cin, word);
            word.erase(word.begin(), word.begin() + 1);
            word.erase(word.begin() + word.length() - 1);
            istringstream buffer (word);
            while (buffer >> word2){
                if (tree.decrypt(word2)){
                    cout << *(tree.decrypt(word2));
                }
                else {
                    cout << "?";
                }
                if (!buffer.eof()){
                    cout  << " ";
                }
            }
            cout << endl;
        }
        else if (instruction == 'p'){
            tree.printPreorder();
        }
        else if (instruction == 'l'){
            tree.printLevelOrder();
        }
        if (instruction == 'q'){
            done = true;
        }
        else {
            cin >> instruction;
        }
    }
    return 0;
}
