#include <string>
#include <iostream>
#include <vector>
using namespace std;
int main() {
    string s = "dir\n\tsubdir1\n\tsubdir2\n\t\tfile.ext";
    string sep = "\n";
    vector<vector<size_t>> ps;
    vector<int> files;
    int m = 0;
    auto findex = [&](int i, int j, char c)->int {
        // find c in range of [i, j)
        int pos = string::npos;
        for (int t = i; t < j; ++t) {
            if (s[t] != c) continue;
            pos = t; break;
        }
        return pos;
    };

    for (size_t i= 0; m!=string::npos;) {
        m = s.find(sep, i);
        size_t j = m == string::npos ? s.size() : m;
        ps.push_back({ i, j - i });
        int dot = findex(i, j, '.');
        cout << dot << endl;
        i = (size_t)m + sep.size();
    }
    for (auto& p : ps) {
        string t = s.substr(p[0], p[1]);
        // cout << "t len " << p[1] <<'\t' << t << endl;
        cout << t << endl;
    }
    /*
    // find (SEP, start_pos, n_occurrence)

    // int p = s.find(".", s.size() - 6); // 28
    int pp = s.size() - 23;
    string dot = ".";
    for (int i = 1; i < 6; ++i) {
        pp = s.find(".", 23, i); // 
    }
    */

    return 0;
}