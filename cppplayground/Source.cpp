#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

#include "BST_deserial_serial.h"
#include "BST_traverse.h"
#include "RingBuffer.h"
#include "max_in_stack.h"
using namespace std;

bool isPalindrome(const std::wstring& str) {
    
    for (size_t i = 0, j = str.size() - 1; i <= j; ++i, --j)
    {
        if (str[i] != str[j]) 
            return false;
    }
    return true;
}
bool isPalindrome_1(const std::wstring& str) {
    std::wstring T(str); 
    std::reverse(T.begin(), T.end());
    return T == str;
}
string largestTimeFromDigits(vector<int>& A) {
    sort(begin(A), end(A), greater<int>());
    do {
        if ((A[0] < 2 || (A[0] == 2 && A[1] < 4)) && A[2] < 6)
            return to_string(A[0]) + to_string(A[1]) + ":" + to_string(A[2]) + to_string(A[3]);
    } while (prev_permutation(begin(A), end(A)));
    return "";
}

void arrage_strings_in_given_order(vector<string>& S, vector<int>& ords) {
    /*
        strings {"ccc", "bbb", "ddddd", "aaaa"};
        orders    [2      1      3        0]
        returns {"aaaa",  "bbb", "ccc", "ddddd"}
    */
    for (int i = 0; i < ords.size(); ++i) {
      while (i != ords[i])
      {
          swap(S[i], S[ords[i]]);
          swap(ords[i], ords[ords[i]]);
      }
    }
    for (auto& s : S) cout << s << ": ";
}
int GetMaxNegative(vector<int> P) {

    // a positive int array, negate max number of the items, 
    // meanwhile make sure the partial sum keep > 0
    //     5  2  3   5  2   3
    // #1  5 -2  3  -5  2   3, the negative number here is 2 
    // #1  5  2 -3   5 -2  -3, the negative number here is 3, so return 3


    // at index i, there is a collection of <neg count, set of possible partial sum by far>
    /*
       5              -> {0: 5}
       5, 2           -> {1: 3, 0: 7}      {0: 7, 1:3}
       5, 2, 3        -> {1: 6, 0: 10}
       5, 2, 3, 5     -> {2: 1, 1: 11, 0: 15}
       5, 2, 3, 5, 2  -> {2: 9, 1: 13, 0: 17}
       5, 2, 3, 5, 2 3-> {2: 10, 1: 16, 0: 20}
    */ 
    unordered_map<int, int> D;
    D.insert({0, P[0]});
    int ret = 0;
    for (int i = 1, cur; i < P.size(); ++i) {
        cur = P[i];

        unordered_map<int, int> D0;
        int neg_cnt, momentum; 
        for (auto& p : D) {
            tie(neg_cnt, momentum) = p;

            if (momentum > cur) {
                if(D.find(neg_cnt+1) != D.end())
                    D0[neg_cnt + 1] = max(D[neg_cnt + 1], momentum - cur);
                else
                    D0[neg_cnt + 1] = momentum - cur;
            }
            D0[neg_cnt] = max(D0[neg_cnt], momentum + cur);
        }
        // cout << "curr : " << cur << endl; for (auto& d : D0) { cout << "\tD0 neg cnt: " << d.first  << ", momentumn: " << d.second << endl; }

        // merge D and D0
        for (auto& p : D0) { 
            ret = max(ret, p.first);
            D[p.first] = max(D[p.first], p.second); 
        }

        // for (auto& d : D) { cout << "\tD neg cnt: " << d.first  << ", momentumn: " << d.second << endl; }
    }
    return ret;
}
void UT_GetMaxNegative() {
    vector<int> a = { 5, 2, 3, 5, 2, 3 };
    cout << GetMaxNegative(a) << endl;
}

void UT_arrange_strings_in_given_order() {
    vector<string> S = { "ccc", "bbb", "dddd", "aaa" };
    vector<int> O = { 2, 1, 3, 0 };
    arrage_strings_in_given_order(S, O);
}

std::string i2s(int i) {
    std::string s = "";
    while (i) {
        int j = i/2;
        if (j*2 == i) {
            s.push_back('0');
        }
        else s.push_back('1');
        i = j;
    }
    std::reverse(begin(s), end(s));
    return s;
}
int main() {
    std::wstring str = L"上中上";
    if (isPalindrome_1(str)) {
        std::wcout << L"Palindrome" << std::endl;
    }
    else {
        std::wcout << L"Not a palindrome" << std::endl;
    }
    vector<int> arr = { 1, 2, 3, 6 };

    // string a = largestTimeFromDigits(arr);
    // UT_arrange_strings_in_given_order();
    // UT_GetMaxNegative();
    // UT_BST_serialize();
    // UT_BST_deserialize();
    // UT_ring_buffer();
    // UT_preorder_traverse();
    // UT_InOrder_traverse_Rc();
    // std::cout << i2s(8) << endl;
    UT_stack_with_max();
    return 0;
}