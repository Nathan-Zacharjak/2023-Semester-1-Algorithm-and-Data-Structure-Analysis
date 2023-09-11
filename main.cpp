#include <iostream>
#include <string>
#include <sstream>
#include <array>

using namespace std;

class hashtable{
private:
    array<string,26> table;
    array<string,26> statusTable;
    int hashFunction(string input);
    int Search(string input);
public:
    hashtable();
    ~hashtable();
    void Insert(string input);
    void Delete(string input);
    void Print();
};

hashtable::hashtable(){
    // Initialise status table values with "never used"
    for (int i = 0; i < 26; i++){
        statusTable.at(i) = "never used";
    }
}

hashtable::~hashtable(){
}

// Converts the input string into the appropriate index
// and returns that index
int hashtable::hashFunction(string input){
    char lastLetter = input.at(input.length() - 1);
    // ASCII conversion shenanigans, value of
    // character 'a' in ASCII is 97
    int index = lastLetter - 97;

    return index;
}

// Finds and returns the index the input string is stored at
// Returns -1 if the input string is not in the table
int hashtable::Search(string input){
    int index = hashFunction(input);

    // Search the corresponding table slot:
    if (table.at(index) == input){
        return index;
    // Else see if it was never used:
    } else if (statusTable.at(index) == "never used"){
        return -1;
    // Else loop through the table until we find it
    // or loop through the whole table trying
    } else if (statusTable.at(index) == "tombstone" || (statusTable.at(index) == "occupied" && table.at(index) != input)){
        int originalIndex = index;
        index += 1;
        if (index >= 26){
            index = 0;
        }

        while (index != originalIndex){
            if (table.at(index) == input){
                return index;
            }

            index += 1;
            if (index >= 26){
                index = 0;
            }
        }
    }
    
    // If the while loop terminates, we failed to
    // find the input word
    return -1;
}

// Inserts the input string into the appropriate position
// in the table, does nothing if it already exists in the table
void hashtable::Insert(string input){
    // cout << "Inserting: " << input << endl;
    int searchResult = Search(input);
    // cout << "Search result: " << searchResult << endl;

    // If the word is already in the table, do nothing
    if (searchResult != -1){
        return;
    }

    // Try inserting the word one value of the table at
    // a time, going down the table, starting with
    // the word's corresponding hash value
    int index = hashFunction(input);
    // cout << "Index: " << index << endl;

    for (int i = 0; i < 26; i++){
        // cout << index << " " << statusTable.at(index) << endl;
        if (statusTable.at(index) == "never used" || statusTable.at(index) == "tombstone"){
            table.at(index) = input;
            statusTable.at(index) = "occupied";
            return;
        }

        index += 1;
        if (index >= 26){
            index = 0;
        }
    }

    return;
}

// Removes the input string from the table, if not in the table,
// does nothing
void hashtable::Delete(string input){
    // cout << "Deleting: " << input << endl;
    int searchResult = Search(input);
    // cout << "Search result: " << searchResult << endl;

    // If the word is not in the table, do nothing
    if (searchResult == -1){
        return;
    } else {
    // Else set the value's table slot status to "tombstone"
        statusTable.at(searchResult) = "tombstone";
    }

    return;
}

// Prints out the contents of the hash table
void hashtable::Print(){
    // cout << "Printing table: " << endl;

    for (int i = 0; i < 26; i++){
        if (statusTable.at(i) == "occupied"){
            cout << table.at(i) << " ";
        }
    }
    cout << endl;

    // cout << "Printing status table: " << endl;
    // for (int i = 0; i < 26; i++){
    //     cout << statusTable.at(i) << " ";
    // }
    // cout << endl;

    return;
}

int main(int argc, char const *argv[]){
    hashtable Hashtable;

    // Processing input
    string input_str;
    getline(cin, input_str);  
    stringstream stream_object(input_str);  //stream object initialized with input_str
    string individual_word;

    while (stream_object >> individual_word) {
        // First letter is the command to add or delete
        char command = individual_word.at(0);
        string word = individual_word.substr(1);

        // Rest of the command is the word to insert
        if (command == 'A'){
            Hashtable.Insert(word);
        } else if (command == 'D'){
            Hashtable.Delete(word);
        }
        
    }
    Hashtable.Print();

    return 0;
}
