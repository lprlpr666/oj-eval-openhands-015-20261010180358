#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

const string DATA_FILE = "data.txt";

void insert_entry(const string &idx, int val) {
    // Check for duplicate before inserting
    ifstream in(DATA_FILE);
    string line;
    while (getline(in, line)) {
        string cur_idx; int cur_val;
        stringstream ss(line);
        if (!(ss >> cur_idx >> cur_val)) continue;
        if (cur_idx == idx && cur_val == val) return; // duplicate, do nothing
    }
    // Append new entry
    ofstream out(DATA_FILE, ios::app);
    out << idx << ' ' << val << '\n';
}

void delete_entry(const string &idx, int val) {
    // Remove first matching entry
    ifstream in(DATA_FILE);
    string line;
    bool removed = false;
    // Use a temporary file
    const string TMP_FILE = "data_tmp.txt";
    ofstream out(TMP_FILE);
    while (getline(in, line)) {
        string cur_idx; int cur_val;
        stringstream ss(line);
        if (!(ss >> cur_idx >> cur_val)) continue;
        if (!removed && cur_idx == idx && cur_val == val) {
            removed = true; // skip this line
            continue;
        }
        out << line << '\n';
    }
    in.close();
    out.close();
    // Replace original file
    rename(TMP_FILE.c_str(), DATA_FILE.c_str());
}

void find_entries(const string &idx) {
    ifstream in(DATA_FILE);
    string line;
    vector<int> vals;
    while (getline(in, line)) {
        string cur_idx; int cur_val;
        stringstream ss(line);
        if (!(ss >> cur_idx >> cur_val)) continue;
        if (cur_idx == idx) vals.push_back(cur_val);
    }
    if (vals.empty()) {
        cout << "null\n";
        return;
    }
    sort(vals.begin(), vals.end());
    for (size_t i = 0; i < vals.size(); ++i) {
        if (i) cout << ' ';
        cout << vals[i];
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // Ensure data file exists
    {
        ifstream f(DATA_FILE);
        if (!f) {
            ofstream out(DATA_FILE);
        }
    }
    int n;
    if (!(cin >> n)) return 0;
    string cmd, idx;
    int val;
    for (int i = 0; i < n; ++i) {
        cin >> cmd >> idx;
        if (cmd == "insert") {
            cin >> val;
            insert_entry(idx, val);
        } else if (cmd == "delete") {
            cin >> val;
            delete_entry(idx, val);
        } else if (cmd == "find") {
            find_entries(idx);
        }
    }
    return 0;
}
