#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <queue>
#include <math.h>
using namespace std;
  struct ListNode {
      int val;
      ListNode *next;
      ListNode(int x) : val(x), next(NULL) {}
  };
vector<int> calCulate(vector<vector<int>>& matrix) {
        
    }
int main(){
    vector<vector<int>> t{{0,1,1,1,1,1},{0,1,1,1,1,1},{0,1,1,1,1,1},{0,1,1,1,1,1},{0,1,1,1,1,1},{0,1,1,1,1,1}};
    int a =(1<<7)-1;
    int b =(a<<2)+3;
    int c = (1<<(7+1*2))-1;
    cout<<a;
    return 0;
}