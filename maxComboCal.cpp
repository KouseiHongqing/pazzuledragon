#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <queue>
#include <math.h>
#include <time.h>
#include <fstream>
#include <stack>
#include "util.cpp"
#include "pazzuleDragon.cpp"
using namespace std;
int logicMaxCombo,maxStep;
pair<int,int> firstStep;
vector<vector<int>> randomVector(int m,int n,int l,int r)//生成M*N范围在l:r的随机数组
{
	srand(time(0));  //设置时间种子
	vector<vector<int>> res;
    for(int i=0;i<m;i++){
        vector<int> tmp;
        for(int j=0;j<n;j++){
            tmp.push_back(rand()%(r-l+1)+l);
        }
        res.push_back(tmp);
    }
    return res;
}
//生成初始数组
vector<vector<int>> initGenerator(int m,int n,int l,int r){
    int limit = 100;
    vector<vector<int>> init;
    while(limit--){
        init = randomVector(m,n,l,r);
        vector<vector<int>> tmp(init);
        if(calCulate(tmp,r-l+1,false)==0) return init;
    }
    
}
//计算理论最大combo
int calculateLogicMaxcombo(vector<vector<int>> matrix,int limit=3){
    unordered_map<int,int> mymap;
    for(auto it:matrix) for(auto iit:it){
        mymap[iit]++;
    }
    int res = 0;
    unordered_map<int,int>::iterator iter=mymap.begin();
    while(iter != mymap.end()){
        if(iter->second>=limit) res+=iter->second/limit;;
        iter++;
    }
    return res;
}
//起手珠选取，这里待补全，先用随机函数模拟
pair<int,int> chooseFirstStep(vector<vector<int>> matrix){
    srand(time(0));  //设置时间种子
    return pair<int,int>{rand()%matrix.size()+1,rand()%matrix[0].size()+1};
}
unordered_map<string,string> readTxt(int m,int n,int maxStep){
    unordered_map<string,string> res;
    ifstream* ifStr = new ifstream;
    string fileName ="\.\\"+ to_string(m) + "x" + to_string(n) + "x"+to_string(maxStep)+".txt";
    ifStr->open(fileName);//打不开文件
    if (!ifStr->is_open()) return res;
    string tmpstr;
    while(!ifStr->eof()){//判断是否是文件的结尾
    getline(*ifStr,tmpstr);
    vector<string> tmpvec = customsplit(tmpstr," ");
    if(!tmpvec.empty()) res[tmpvec[0]] = tmpvec[1];
    }
    ifStr->close();
    return res;
}
string transStatus(vector<vector<int>> matrix){
    string res ="";
    for(auto it:matrix)for(auto iit:it) res+=to_string(iit);
    return res;
}
void saveRecord(pair<int,int> firstStep,unordered_map<string,string> record,int maxStep){

}
//从matrix 通过top移动后 达到nowposition,返回最后操作点+上一步操作 0上1下2左3右
inline pair<pair<int,int>,int> fetchMatrix(vector<vector<int>> &matrix,long long oldtop,vector<vector<int>> &nowPosition){ 
    pair<pair<int,int>,int> res;
    //深拷贝
    nowPosition = matrix;
    long long top = oldtop;
    stack<int> movement;
    //截取操作
    while(top+1>(1<<7)){
        movement.push(top&3);
        top>>=2;
    }
    //截取起始位
    int y = (top&7) - 1;
    top>>=3;
    int x = (top&7) - 1;
    //最后一步操作
    int tmp=-1;
    if(oldtop<128){
        //没有操作则直接返回
         return (pair<pair<int,int>,int>){(pair<int,int>){x+1,y+1},tmp};
    }
    while(!movement.empty()){
        tmp = nowPosition[x][y],top=movement.top();
        movement.pop();
        //交换操作 0上1下2左3右
        switch (top)
        {
        case 0:
            nowPosition[x][y] = nowPosition[x-1][y];
            nowPosition[x-1][y] = tmp;
            x -= 1;
            break;
        case 1:
            nowPosition[x][y] = nowPosition[x+1][y];
            nowPosition[x+1][y] = tmp;
            x += 1;
            break;
        case 2:
            nowPosition[x][y] = nowPosition[x][y-1];
            nowPosition[x][y-1] = tmp;
            y -=1;
            break;
        case 3:
            nowPosition[x][y] = nowPosition[x][y+1];
            nowPosition[x][y+1] = tmp;
            y += 1;
            break;
        }
    }
    return (pair<pair<int,int>,int>){(pair<int,int>){x+1,y+1},tmp};
}
string showResult(long long res){
    string message;
    stack<int> movement;
    //截取操作
    while(res+1>(1<<7)){
        movement.push(res&3);
        res>>=2;
    }
    //截取起始位
    int y = (res&7) - 1;
    res>>=3;
    int x = (res&7) - 1;
    //最后一步操作
    int tmp;
    message = "从坐标\("+to_string(x+1)+"\,"+to_string(y+1)+"\)"+"开始";
    while(!movement.empty()){
        int top = movement.top();
        movement.pop();
        string es;
        //更改这里调整返回值
        switch (top)
        {
        case 0:
            es = "上";
            break;
        case 1:
            es = "下";
            break;
        case 2:
            es = "左";
            break;
        case 3:
            es = "右";
            break;
        }
        
        message += +"\," +es;
    }
    return message;
}
long long goDFS(vector<vector<int>> &matrix){
    //因为时间复杂度为m^n,为指数级，传统递归可能会造成内存泄漏，这里改用手动压栈
    //使用位优化long long 64位中 转珠起始位置占7位(如(i,j)表示为1<<6+(i<<3+j))
    //剩余56位中每2位表示移动方向，如11表示右上移1格，00左下移一格
    stack<long long> mystack;
    int combo,maxcombo=-1e9;
    long long res = 1e13;
    vector<vector<int>> nowPosition;
    //挂载初始数据
    long long init = (1<<6)+(firstStep.first<<3)+firstStep.second;
    mystack.push(init);
    while(!mystack.empty()){
        long long top = mystack.top();
        mystack.pop();
        //如果已被访问(=0)
        //先获取当前数组
        pair<pair<int,int>,int> tmpPosit = fetchMatrix(matrix,top,nowPosition);
        combo = calCulate(nowPosition,0,false);
        //到达最大步数
        if(combo==logicMaxCombo) return top;
        if(top>127){
        if(combo>maxcombo){
            printf("深度更新，当前combo数为=%d\n",combo);
            maxcombo = combo;
            res = top;
        }
        else if(combo==maxcombo){
            if(top<res){
            //存储最大combo 并且步数最少的结果
            maxcombo = combo;
            res = top;
            }
        }
        }
        //最大递归深度
        if(((top<<2)+3)>(((long long)1<<(7+maxStep*2)))-1) continue;
        //向上00
        if(tmpPosit.first.first-1>0 && tmpPosit.second!=1){
            mystack.push(top<<2);
        }
        
        //向下01
        if(tmpPosit.first.first+1<=matrix.size() && tmpPosit.second!=0){
            mystack.push((top<<2)+1);
        }
        
        //向左10
        if(tmpPosit.first.second-1>0 && tmpPosit.second!=3){
            mystack.push((top<<2)+2);
        }
        
        //向右11
        if(tmpPosit.first.second+1<=matrix[0].size() && tmpPosit.second!=2){
            mystack.push((top<<2)+3);
        }
        
        
    }
    cout<<"最大combo数为:"<<maxcombo<<endl;
    return res;
}
//dfs模拟，暴力+剪枝，带记忆化
string findWayByGreedySearch(vector<vector<int>> matrix,int step){
    maxStep = step;
    //记忆化存储
    unordered_map<string,string> record = readTxt(matrix.size(),matrix[0].size(),maxStep);
    //图像转换
    string status = transStatus(matrix);
    //有结果直接返回
    if(record.count(status)>0) return record[status];
    //结果集
    long long res;
    //理论最大combo
    logicMaxCombo = calculateLogicMaxcombo(matrix);
    //起手珠选取,因为理论上任意珠起手都没影响，但是会影响算法效率，这里等记忆集合达到一定规模可以进行优化
    // firstStep = chooseFirstStep(matrix);
    firstStep = (pair<int,int>{4,1});
    //暴力搜索+剪枝
    int x=firstStep.first,y=firstStep.second;
    int col=matrix.size(),row=matrix[0].size();
    //递归 有限步骤内寻找最优解
    // if(matrix.size()>9||matrix[0].size()>9) return NULL;
    res = goDFS(matrix);
    //保存记忆化结果
    // saveRecord(firstStep,record,maxStep);
    return showResult(res);
}

int main()
{
   vector<vector<int>> test{
        {5,2,6,2,4,2},
        {1,6,4,4,1,3},
        {1,6,3,3,2,1},
        {3,4,6,6,2,1},
        {1,5,5,3,3,3}
    };
     vector<vector<int>> test2{
        {5,2,6,2,4,2},
        {1,6,4,4,1,3},
        {6,4,3,3,2,1},
        {1,6,5,6,2,1},
        {1,5,3,3,3,3}
    };
    // calCulate(test2,0,true);
    pair<pair<int,int>,int> sp;
    vector<vector<int>> res;
    // long long top =  (1<<6)+(2<<3)+6;
    // //左下左上
    // top<<=2;
    // top+=2;
    // top<<=2;
    // top+=1;
    // top<<=2;
    // top+=2;
    // top<<=2;
    // top+=0;
    // sp = fetchMatrix(test,top,res);
    // printVector(res);
    // string oo = showResult(top);
    // cout<<oo;
    // vector<vector<int>> test2{
    //     {0,0,0,0,0,0},
    //     {1,0,0,0,0,0},
    //     {1,0,0,0,0,0},
    //     {1,1,0,1,0,0},
    //     {1,1,1,1,0,1}
    // };
    //生成M*N范围在l:r的随机数组
    vector<vector<int>> rv = initGenerator(5,6,1,3);
    printVector(test);
    cout<<findWayByGreedySearch(test,14);
    // cout<<"max combo:"<<calculateLogicMaxcombo(rv);
    // double res= calCulate(rv,6,true);;
    // for(int i=0;i<1000;i++){
    //     rv = randomVector(5,6,1,6);
    //     int tmp = calCulate(rv,6,false);
    //     res += tmp;
    // }
    // printVector(rv);
    // cout<<"average combo is ="<<res;
    // finish(test);
    // for(auto it:test){
    //     for(auto iit:it) cout<<iit<<" ";
    //     cout<<endl;
    // // }
    // cout<<comboDFS(test2)<<endl;
    // string a = transStatus(test2);
    // long long num = (long long)1<<39;
    // cout<<num;
    return 0;
}