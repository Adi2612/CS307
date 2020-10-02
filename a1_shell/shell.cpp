#include<bits/stdc++.h>
#include<unistd.h>  

using namespace std;

typedef void (*script_function)(vector<string>&); 
map<string, script_function> functions;
vector<string> history_data;

/**
 *  split the input command into array of tokens
*/
void tokenize(vector<string> &tokens) {
  string input;
  getline(cin, input);
  history_data.push_back(input);
  string first_token = input.substr(0, input.find(' '));
  string second_token;
  tokens.push_back(first_token);
  if(first_token.size() != input.size()) {
    second_token = input.substr(input.find(' ') + 1, input.find('\n'));
    bool white_spaces_only = second_token.find_first_not_of(' ') == std::string::npos;
    if(!white_spaces_only)
      tokens.push_back(second_token);
  }
}

void current_dir() {
  char path[100];
  getcwd(path, 100);
  cout<<path<<"$ ";
}

void cd(vector<string> &tokens) {
  // todo : change PWD shell env
  if(tokens.size() == 1) {
    current_dir();
  } else if(tokens.size() == 2) {
    char path[1024];
    int i = 0;
    for(auto it : tokens[1]) {
      if(it != '\\') {
        path[i++] = it;
      }
    }
    path[i] = '\0';
    if(chdir(path) != 0) {
      cout<<"Directory is not valid :/"<<endl;
    }
  }
}

void dir(vector<string> &tokens) {
  
}

void echo(vector<string> &tokens) {
  // print the remaining tokens
  if(tokens.size() == 2) {
    cout<<tokens[1];
  }
  cout<<endl;
}

void history(vector<string> &tokens) {
  // todo : skip last element in array.
  for(auto it : history_data){
    cout<<it<<endl;
  }
}

void quit(vector<string> &tokens) {
  exit(0);
}

void help(vector<string> &tokens) {
  // todo : add necessary stuff here.
  cout<<" Fill it with helps :)"<<endl;
}

void clr(vector<string> &tokens) {
  system("clear");
}

/**
 * map command to function pointers
*/
void init_setup() {
  functions["cd"] = &cd;
  functions["clr"] = &clr;
  functions["dir"] = &dir;
  // functions["environ"] = &environ;
  functions["echo"] = &echo;
  // functions["pause"] = &pause;
  functions["help"] = &help;
  functions["quit"] = &quit;
  functions["history"] = &history;
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
    current_dir();
    vector<string> tokens;
    tokenize(tokens);
    call_commands(tokens);
  }

  return 0;
}
