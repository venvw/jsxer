//
// Created by Angelo DeLuca on 12/1/21.
//

#include "jsxbin.h"
#include "nodes/RootNode.h"

#include <string>
#include <algorithm>
#include <regex>

using namespace std;

const regex h_identify("^@JSXBIN@ES@([\\d.]+)@");
const regex h_replace("^@JSXBIN@ES@[\\d.]+@");

string decompile(const string &input) {

    // Normalize the input by removing line-breaks, carriage returns, and backslashes.
    string normalized;
    remove_copy(input.begin(), input.end(), back_inserter(normalized), '\n');
    remove_copy(normalized.begin(), normalized.end(), back_inserter(normalized), '\r');
    remove_copy(normalized.begin(), normalized.end(), back_inserter(normalized), '\\');

    // Identify the JSXBIN Version.
    int version;
    smatch matches;
    if(regex_search(normalized, matches, h_identify)){
        string match = matches[1].str();
        version = stoi(&match[0]);
    }
    // TODO: Throw exception if not found.

    // Remove the header from the normalized input.
    string body = regex_replace(normalized, h_replace, "");
    ScanState *scanState = new ScanState(body);

    // Start decompilation.
    RootNode *root = new RootNode(*scanState);

    return root->jsx();
}