#pragma once

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <tuple>
using std::tuple;
using std::make_tuple;

#include <functional>
using std::function;

class Console {
  public:
    Console();
    void addCommand(const tuple<string, function<void(vector<string>)>, string>& command);
    void parseInput();
    bool isRunning() const;

  protected:
    static bool _isRunning;
    static vector<tuple<string, function<void(vector<string>)>, string>> _commands;

    string trim(const string& input) const;

    static void help(const vector<string>& ignored);
    static void exit(const vector<string>& ignored);

    static char** getCompletionList(const char* text, const int start, const int end);
    static char* getCompletionEntry(const char* text, const int state);
};
