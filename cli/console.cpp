#include <console.hpp>

#include <cstdlib>
using std::size_t;

#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;

#include <ios>
using std::left;

#include <cstring>
using std::strlen;
using std::strcpy;

#include <tuple>
using std::get;

// Waiting for libstdc++ to add <regex> support
//#include <regex>
//using std::regex;
//using std::sregex_token_iterator;

// Workaround for the problem mentioned above (start)
#include <algorithm>
using std::find_if;
using std::find_if_not;

#include <cctype>
// Workaround (end)

#include <vector>
using std::vector;

#include <readline/readline.h>
// rl_readline_name
// rl_attempted_completion_function
// rl_completion_matches
// rl_attempted_completion_over

#include <readline/history.h>
// add_history

#include <cstdlib>
// free

#include <sys/select.h>
// timeval
// fd_set
// select
// FD_SET
// FD_ZERO
// FD_ISSET

#include <unistd.h>
// STDIN_FILENO

decltype(Console::_commands) Console::_commands = {
    make_tuple("help", function<void(vector<string>)>(Console::help), "Returns a list of supported commands, with a short description"),
    make_tuple("exit", function<void(vector<string>)>(Console::exit), "Exit command-line interface")
    };

decltype(Console::_isRunning) Console::_isRunning;

Console::Console() {
  rl_readline_name = "HOP";
  rl_attempted_completion_function = Console::getCompletionList;
  _isRunning = true;
}

bool Console::isRunning() const {
  return _isRunning;
}

void Console::addCommand(const tuple<std::string, function<void(vector<std::string>)>, std::string>& command) {
  _commands.push_back(command);
}


void Console::parseInput() {
  char* buffer = readline("HOP> ");
  string input(buffer);
  free(buffer);

  input = trim(input);

  size_t delimiter = input.find_first_of(' ');
  string inputCommand = input.substr(0, delimiter);

  for(tuple<string, function<void(vector<string>)>, string> command : _commands) {
    if(inputCommand == get<0>(command)) {

      if(delimiter != string::npos) {
        string inputArguments = trim(input.substr(delimiter));

        // Waiting for libstdc++ to add <regex> support
        // get<1>(command)(vector<string>(sregex_token_iterator(inputArguments.begin(), inputArguments.end(), regex(" +"), -1), sregex_token_iterator()));

        vector<string> arguments;
        auto currentPosition = inputArguments.begin();
        while(currentPosition != inputArguments.end()){
          currentPosition = find_if_not(currentPosition, inputArguments.end(), isspace);
          if(currentPosition == inputArguments.end()) {
            break;
          }
          auto nextPosition = find_if(currentPosition, inputArguments.end(), isspace);
          arguments.push_back(string(currentPosition, nextPosition));
          currentPosition = nextPosition;
        }

        get<1>(command)(arguments);
      } else {
        get<1>(command)({});
      }

      break;
    }
  }

  if(!input.empty()) {
    add_history(input.c_str());
  }
}

string Console::trim(const string& input) const {
  size_t firstNonSpaceChar = input.find_first_not_of(' ');
  size_t lastNonSpaceChar  = input.find_last_not_of(' ');
  return input.substr(firstNonSpaceChar, lastNonSpaceChar - firstNonSpaceChar + 1);
}

void Console::help(const vector<string>& ignored) {
  cout << setw(15) << left << "Command" << "Short description" << endl;
  cout << "---------------------------------" << endl;

  for(tuple<string, function<void(vector<string>)>, string> command : _commands) {
    cout << setw(15) << left << get<0>(command) << get<2>(command) << endl;
  }
}

void Console::exit(const vector<std::string> &ignored) {
  _isRunning = false;
}

char** Console::getCompletionList(const char* input, const int start, const int end) {
  if (start == 0) {
    rl_attempted_completion_over = 1;
    return rl_completion_matches(input, Console::getCompletionEntry);
  }

  return nullptr;
}

char* Console::getCompletionEntry(const char* input, const int state) {
  static int commandIndex, inputLength;

  if(state == 0) {
    commandIndex = 0;
    inputLength = strlen(input);
  }

  while(commandIndex < _commands.size()) {
    string command(get<0>(_commands.at(commandIndex++)));
    if(command.compare(0, inputLength, input) == 0) {
      return strcpy(new char[command.size() + 1], command.c_str());
    }
  }

  return nullptr;
}
