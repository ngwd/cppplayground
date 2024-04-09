#include <string>
#include <stack>
#include <memory>
#include <iostream>
#include <tuple>


struct TreeNode {
    TreeNode *left, *right;
    int val;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Codec {
public:
    std::string serialize_stk(TreeNode* root) {

        std::string ret;
        if (!root) return ret;
        std::stack<TreeNode*> stk;
        stk.push(root);
        while (stk.size()) {
            TreeNode* cur = nullptr; 
            while (stk.size() && !(cur = stk.top())) {
                ret += "N,";
                stk.pop();
            }
            if (!stk.size()) break;
            ret += std::to_string(cur->val) + ",";
            stk.pop();
            stk.push(cur->right);
            stk.push(cur->left);
        }
        std::cout << ret << std::endl;
        return ret;
    }

    std::string serialize(TreeNode* root) {
        auto dfs = [](TreeNode* root, std::string& ret, auto&& dfs)->void {
            if (!root) {
                ret += "N,";
                return;
            }
            ret += std::to_string(root->val) + ",";
            dfs(root->left,  ret, dfs);
            dfs(root->right, ret, dfs);
        };
        std::string ret;
        dfs(root, ret, dfs);
        return ret;
    }

    TreeNode* deserialize(const std::string& str) {
        auto dfs = [&str](int &start, auto&& dfs)-> TreeNode*{
            int i = start, num = 0;
            for (; isdigit(str[i]); ++i) {
                num *= 10;
                num += str[i] - '0';
            }
            if (i == start) {
                start += 2;
                return nullptr;
            }
            else {
                ++start;
                TreeNode* root = new TreeNode(num);
                root->left = dfs(start, dfs);

                ++start;
                root->right = dfs(start, dfs);
                return root;
            }
        };
        int start = 0;
        return dfs(start, dfs);
    }

    TreeNode* deserialize_stk(const std::string& str) {
        using DIR = enum {left = 0, right = 1};
        std::stack<std::pair<TreeNode*, DIR>> stk;
        int num = 0, ncnt = 0; // ncnt: 'N' count
        bool in_num = false;

        TreeNode* cur = nullptr;
        DIR dir = left;
        for (char c : str) {
            if (isdigit(c)) {
                ncnt = 0;
                num *= 10;
                num += c - '0';
                in_num = true;
            }
            else if (c == ',') {
                if (in_num) {
                    in_num = false;
                    stk.push(std::make_pair(new TreeNode(num), left));
                    num = 0;
                }
            }
            else if (c == 'N') {
                ++ncnt;
                if (ncnt == 1 ) {
                    if (stk.size())
                        stk.top().second = right;
                }
                else if (ncnt == 2) {
                    std::tie(cur, dir) = stk.top();
                    stk.pop();
                    if (!stk.size()) return cur;
                    if (stk.top().second == left) {
                        stk.top().first->left = cur;
                        stk.top().second = right;
                    }
                    else {
                        do {
                            dir = stk.top().second;
                            if (dir == right)
                                stk.top().first->right = cur;
                            else 
                                stk.top().first->left = cur;
                            cur = stk.top().first;
                            stk.pop();
                        } while (stk.size());
                        stk.push(std::make_pair(cur, right));
                    }
                    ncnt = 0;
                }
            }
        }
        std::tie(cur, dir) = stk.top();
        stk.pop();
        return cur;
    }
};

void UT_BST_serialize() {
    Codec codec;
    TreeNode* t5 = new TreeNode(5);
    TreeNode* t2 = new TreeNode(2);
    TreeNode* t3 = new TreeNode(3);
    TreeNode* t1 = new TreeNode(1);
    TreeNode* t4 = new TreeNode(4);
    t5->left = t2; t2->right = t3; t3->left = t1; t3->right = t4;
    std::string ret = codec.serialize(t5);
    // 5,2,N,3,1,N,N,4,N,N,N,
    delete(t5); delete(t4); delete(t3); delete(t2); delete(t1);

};
void UT_BST_deserialize() {
    // const std::string s("1,2,N,N,N");
    // const std::string s("1,2,N,N,3,4,N,N,5,N,N,");
    // const std::string s("5,2,N,3,1,N,N,4,N,N,N,");
    // const std::string s("2,3,1,N,N,4,5,N,N,6,N,N,7,N,N");
    const std::string s("1,2,N,N,3,4,6,N,N,7,N,N,5,N,N,");
    Codec codec;
    TreeNode* ptn = codec.deserialize(s);
    std::string ss = codec.serialize(ptn);
    std::cout << ss << std::endl;
}
