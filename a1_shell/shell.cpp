#include<bits/stdc++.h>
#include<unistd.h>  
#include <dirent.h>

using namespace std;
const string WHITESPACE = " \n\r\t\f\v";
typedef void (*script_function)(vector<string>&); 
map<string, script_function> functions;
map<string, string> env_data;
vector<string> history_data;

void set_env(string left, string right) {
  env_data[left]=right;
}

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
    } else {
      char current_dir[100];
      getcwd(current_dir, 100);
      string str(current_dir);
      set_env("PWD", str);
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
  cout<<"Below is a list of commands and their functions:"<<endl;
  cout<<"1. cd <path> : Change current working directory to <path>"<<endl;
  cout<<"2. clr : Clear Screen"<<endl;
  cout<<"3. dir : Display all files and folders in current working directory"<<endl;
  cout<<"4. echo <string> : Displays <string>"<<endl;
  cout<<"5. history : Shows a list of recently used commands"<<endl;
  cout<<"6. quit : Exit shell"<<endl;
  cout<<"7. environ : Display list of environment variables"<<endl;
  cout<<"8. pause: Pauses shell, enter key to resume"<<endl;

}

void clr(vector<string> &tokens) {
  system("clear");
}

void environ_(vector<string> &tokens) {
  for(auto it : env_data) {
    cout<<it.first<<"="<<it.second<<endl;
  }
}

void pause(vector<string> &tokens) {
  int n;
  while(n = cin.get()) {
    if(n == int('\n'))
      break;
  }
}

/**
 * map command to function pointers
*/
void init_setup(string env_path) {
  extern char **environ;
  int i = 0;
  while(environ[i]) {
    string s(environ[i++]);
    string first_token = s.substr(0, s.find('='));
    string second_token = s.substr(s.find('=') + 1, s.find('\0'));    
    env_data[first_token] = second_token;
  }
  char path[100];
  getcwd(path, 100);
  string directory(path);
  set_env("SHELL", directory + env_path.substr(1, env_path.size()));
  functions["cd"] = &cd;
  functions["clr"] = &clr;
  functions["dir"] = &dir;
  functions["environ"] = &environ_;
  functions["echo"] = &echo;
  functions["pause"] = &pause;
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
  init_setup(argv[0]);
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
