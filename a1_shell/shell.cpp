#include<bits/stdc++.h>
#include<unistd.h>  

using namespace std;

typedef void (*script_function)(vector<string>&); 
map<string, script_function> functions;

/**
 *  split the input command into array of tokens
*/
void tokenize(vector<string> &tokens) {
  string input;
  getline(cin, input);
  stringstream input_stream(input);
  string temp;
  
  // todo : make it work for double quote 
  // https://stackoverflow.com/questions/18675364/c-tokenize-a-string-with-spaces-and-quotes
  while(getline(input_stream, temp, ' ')){
    tokens.push_back(temp);
  }
}

void cd(vector<string> &tokens) {

}

void dir(vector<string> &tokens) {
  char path[100];
  getcwd(path, 100);
  cout<<path<<endl;
}

void echo(vector<string> &tokens) {
  // print the remaining tokens
}

/**
 * map command to function pointers
*/
void init_setup() {
  functions["cd"] = &cd;
  // functions["clr"] = &clr;
  functions["dir"] = &dir;
  // functions["environ"] = &environ;
  functions["echo"] = &echo;
  // functions["pause"] = &pause;
  // functions["help"] = &help;
  // functions["quit"] = &quit;
  // functions["history"] = &history;
}

/**
 * Invoke commands
 */
void call_commands(vector<string> &tokens) {
  if(tokens.size()) {
    auto fn = functions.find(tokens[0]);
    if(fn == functions.end()) {
      cout<<"Command not found :/"<<endl;
    } else {
      (*fn->second)(tokens);
    }
  }
}

int main(int argc, char const *argv[]) {

  init_setup();

  while(1) {
    cout<<">> ";
    vector<string> tokens;
    tokenize(tokens);
    call_commands(tokens);
  }

  return 0;
}
