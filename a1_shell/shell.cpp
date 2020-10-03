#include<bits/stdc++.h>
#include<unistd.h>  
#include <dirent.h>

using namespace std;
const string WHITESPACE = " \n\r\t\f\v";
extern char **environ;
typedef void (*script_function)(vector<string>&); 
map<string, script_function> functions;
vector<string> history_data;

string ltrim(const string& s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == string::npos) ? "" : s.substr(start);
}

string rtrim(const string& s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string trim(const string& s)
{
	return rtrim(ltrim(s));
}


/**
 *  split the input command into array of tokens
*/
void tokenize(vector<string> &tokens, string input) {
  history_data.push_back(input);
  string first_token = input.substr(0, input.find(' '));
  string second_token;
  tokens.push_back(first_token);
  if(first_token.size() != input.size()) {
    second_token = input.substr(input.find(' ') + 1, input.find('\n'));
    if(trim(second_token).size())
      tokens.push_back(trim(second_token));
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
    cout<<endl;
  } else if(tokens.size() == 2) {
    char path[1024];
    int k = 0;
    for(int i = 0 ; i < tokens[1].size();  i++) {
      if(tokens[1][i] == '\\') {
        if(i+1 < tokens[1].size()) {
          path[k++] = tokens[1][++i];
        }
      } else if(tokens[1][i] != ' ') {
        path[k++] = tokens[1][i];
      }
    }
    path[k] = '\0';
    if(chdir(path) != 0) {
      cout<<"Directory is not valid :/"<<endl;
    }
  }
}

void dir(vector<string> &tokens) {
  struct dirent *entry;
  char path[1024] = ".";
  if(tokens.size() == 2) {
    strcpy(path, tokens[1].c_str());
  }
  DIR *dir = opendir(path);
  if(dir == NULL) {
    cout<<"Directory is not valid :/"<<endl;
  } else {
   while ((entry = readdir(dir)) != NULL) {
    cout << entry->d_name << endl;
   }
   closedir(dir);
  }
}

void echo(vector<string> &tokens) {
  // print the remaining tokens
  if(tokens.size() == 2) {
    cout<<tokens[1];
  }
  cout<<endl;
}

void history(vector<string> &tokens) {
  for(int i = 0 ; i < history_data.size() - 1 ; i++){
    cout<<history_data[i]<<endl;
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

void environ_(vector<string> &tokens) {
  for (char **env = environ; *env != 0; env++)
  {
    char *thisEnv = *env;
    printf("%s\n", thisEnv);    
  }

}

/**
 * map command to function pointers
*/
void init_setup() {
  functions["cd"] = &cd;
  functions["clr"] = &clr;
  functions["dir"] = &dir;
  functions["environ"] = &environ_;
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
  if (argc == 1) {
    while(1) {
      current_dir();
      vector<string> tokens;
      string input;
      getline(cin, input);
      tokenize(tokens, input);
      call_commands(tokens);
    }
  }
  else if (argc == 2) {
    string filename = argv[1];
    ifstream f;
    f.open(filename);

    if (f.is_open()) {
      string buffer;

      while (getline (f, buffer)) {
        current_dir();
        vector<string> tokens;
        tokenize(tokens, buffer);
        call_commands(tokens);
      }
      f.close();
    }
    else {
      cout<<"File does not exist"<<endl;
    }
  }
  else {
    cout<<"Invalid number of command line arguments"<<endl;
  }

  return 0;
}
