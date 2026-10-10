#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin >> n)) return 0;
    map<string, set<int>> db;
    string cmd, idx;
    int val;
    for(int i=0;i<n;++i){
        cin >> cmd >> idx;
        if(cmd=="insert"){
            cin >> val;
            db[idx].insert(val);
        } else if(cmd=="delete"){
            cin >> val;
            auto it = db.find(idx);
            if(it!=db.end()){
                it->second.erase(val);
                if(it->second.empty()) db.erase(it);
            }
        } else if(cmd=="find"){
            auto it = db.find(idx);
            if(it==db.end() || it->second.empty()){
                cout << "null\n";
            } else {
                bool first=true;
                for(int v: it->second){
                    if(!first) cout << ' ';
                    first=false;
                    cout << v;
                }
                cout << '\n';
            }
        }
    }
    return 0;
}
