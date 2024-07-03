#pragma once
#include <vector>
#include <stack>
#include <iostream>
std::vector<int> PreOrder_Traverse(TreeNode* root) {
    std::vector<int> ret;
    if (!root) return ret;
    std::stack<TreeNode*> stk;
    stk.push(root);
    while (stk.size()) {
        TreeNode* top = stk.top();
        ret.push_back(top->val);
        stk.pop();
        if (top->right) stk.push(top->right);
        if (top->left) stk.push(top->left);
    }
    return ret;
}
std::vector<int> PreOrder_Traverse_Rc(TreeNode* root) {
    std::vector<int> ret;
    auto dfs = [&ret](TreeNode* root, auto&& dfs)-> void{
        if (!root) return;
        ret.push_back(root->val);
        dfs(root->left, dfs);
        dfs(root->right, dfs);
    };
    dfs(root, dfs);
    return ret;
}
std::vector<int> InOrder_Traverse(TreeNode* root) {
    std::vector<int> ret;
    if (!root) return ret;
    std::stack<TreeNode*> stk;
    stk.push(root);
    while (stk.size()) {
        TreeNode* top = stk.top();
        while (top->left) {
            stk.push(top->left);
            top = stk.top();
        }
        ret.push_back(top->val);
        stk.pop();
        while (!top->right && stk.size()) 
        {
            top = stk.top();
            ret.push_back(top->val);
            stk.pop();
        }
        if (top->right) stk.push(top->right);
    }
    return ret;
}
std::vector<int> InOrder_Traverse_Rc(TreeNode* root) {
    std::vector<int> ret;
    if (!root) return ret;
    auto dfs = [&ret](TreeNode* root, auto&& dfs)->void {
        if (!root) return;
        dfs(root->left, dfs);
        ret.push_back(root->val);
        dfs(root->right, dfs);
    };
    dfs(root, dfs);
    return ret;
}

void UT_preorder_traverse() {
    TreeNode* t5 = new TreeNode(5);
    TreeNode* t2 = new TreeNode(2);
    TreeNode* t3 = new TreeNode(3);
    TreeNode* t1 = new TreeNode(1);
    TreeNode* t4 = new TreeNode(4);
    TreeNode* t6 = new TreeNode(6);
    TreeNode* t7 = new TreeNode(7);
    t5->left = t2; t2->right = t3; t3->left = t1; t3->right = t4;
    t5->right = t6;t2->left = t7;

    // 5,2,N,3,1,N,N,4,N,N,N,
    std::vector<int> vi = PreOrder_Traverse_Rc(t5);
    for (auto i : vi) std::cout << i << ", ";
    std::cout << std::endl;
    delete(t5); delete(t4); delete(t3); delete(t2); delete(t1);
    delete(t7); delete(t6);
}
void UT_InOrder_traverse_Rc() {
    TreeNode* t5 = new TreeNode(5);
    TreeNode* t2 = new TreeNode(2);
    TreeNode* t3 = new TreeNode(3);
    TreeNode* t1 = new TreeNode(1);
    TreeNode* t4 = new TreeNode(4);
    TreeNode* t6 = new TreeNode(6);
    TreeNode* t7 = new TreeNode(7);
    t5->left = t2; t2->right = t3; t3->left = t1; t3->right = t4;
    t5->right = t6;t2->left = t7;

    // 5,2,N,3,1,N,N,4,N,N,N,
    std::vector<int> vi = InOrder_Traverse(t5);
    for (auto i : vi) std::cout << i << ", ";
    std::cout << std::endl;
    delete(t5); delete(t4); delete(t3); delete(t2); delete(t1);
    delete(t7); delete(t6);
}

