#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
using namespace std;

const string DB_DIR = "db";

void ensure_db_dir() {
    struct stat st = {0};
    if (stat(DB_DIR.c_str(), &st) == -1) {
        mkdir(DB_DIR.c_str(), 0755);
    }
}

string file_path(const string &idx) {
    return DB_DIR + "/" + idx;
}

bool entry_exists(const string &idx, int val) {
    ifstream in(file_path(idx));
    string line;
    while (getline(in, line)) {
        string cur_idx; int cur_val;
        stringstream ss(line);
        if (!(ss >> cur_idx >> cur_val)) continue;
        if (cur_idx == idx && cur_val == val) return true;
    }
    return false;
}

void insert_entry(const string &idx, int val) {
    if (entry_exists(idx, val)) return; // avoid duplicate
    ofstream out(file_path(idx), ios::app);
    out << idx << ' ' << val << '\n';
}

void delete_entry(const string &idx, int val) {
    ifstream in(file_path(idx));
    string line;
    bool removed = false;
    const string tmp_file = DB_DIR + "/tmp_" + idx;
    ofstream out(tmp_file);
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
    // Replace original file with temp (if any entry left, otherwise delete file)
    if (removed) {
        // Check if temp file is empty
        ifstream check(tmp_file);
        bool empty = check.peek() == EOF;
        check.close();
        if (empty) {
            remove(file_path(idx).c_str());
            remove(tmp_file.c_str());
        } else {
            rename(tmp_file.c_str(), file_path(idx).c_str());
        }
    }
}

void find_entries(const string &idx) {
    ifstream in(file_path(idx));
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
    ensure_db_dir();
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
