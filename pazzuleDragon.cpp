#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <queue>
#include <math.h>
using namespace std;
// 点集合 包含是否存在加珠，珠子类型等
struct binary
{
    //珠类型
    int cata;
    //是否存在combo增加
    bool comboPlus = false;
    int i,j;
    binary(int x ,int i,int j) : cata(x), i(i), j(j){}
};
//单论消珠完成后进行掉落，每次迭代返回掉落后的数组
void finish(vector<vector<int>> &matrix,bool isprint)
{
    int sizeCol = matrix.size(), sizeRow = matrix[0].size();
    for (int i = sizeCol - 1; i >= 0; --i)
        for (int j = sizeRow - 1; j >= 0; --j)
        if(matrix[i][j]==0){
            int ii=i-1;
            while(ii>=0){
                if(matrix[ii][j]>0) {
                    matrix[i][j] = matrix[ii][j];
                    matrix[ii][j] = 0;
                    break;
                }
                ii--;
            }
        }
    if(isprint){
        cout<<endl;
        cout<<"--------------next combo---------"<<endl;
        for(auto it:matrix){
            for(auto iit:it) cout<<iit<<" ";
            cout<<endl;
        }
    }
}
// 联通图是否满足combo条件 返回布尔值并对原数组进行修改(修改处置为-1)
// @param res:combo数组,limit:最低消除珠数(默认3)
bool comboDFS(vector<vector<int>> &res,int limit=3){
    vector<vector<int>> remod(res);
    bool isCombo = false;
    //先横排扫描
    for(int i=0;i<remod.size();i++){
        for(int j=0;j<remod[0].size();j++){
            if(remod[i][j]>0){
                int count = 1 , jj = j+1;
                while(jj<remod[0].size() && remod[i][jj]>0){
                    count++;
                    jj++;
                }
                if(count>=limit){
                    for(int k=0;k<count;k++) res[i][j+k] = -1;
                    isCombo = true;
                }
            }
        }
    }
    //竖排扫描
    for(int j=0;j<remod[0].size();j++){
        for(int i=0;i<remod.size();i++){
            if(remod[i][j]>0){
                int count = 1 , ii = i+1;
                while(ii<remod.size() && remod[ii][j]>0){
                    count++;
                    ii++;
                }
                if(count>=limit){
                    for(int k=0;k<count;k++) res[i+k][j] = -1;
                    isCombo = true;
                }
            }
        }
    }
    return isCombo;
}
//消珠算法 @matrix:图像矩阵 @catag 最大种类(如 火+水，则catag=2)
//return 最大combo
int calCulate(vector<vector<int>> &matrix, int catag, bool isprint)
{
    int count = 0;
    int sizeCol = matrix.size(), sizeRow = matrix[0].size();
    //已访问记录,0=未访问
    vector<vector<int>> visited(sizeCol, vector<int>(sizeRow, 0));
    //连通集 bfs处理
    queue<binary> unionSet;
    //因为掉落原因 反向遍历
    for (int i = sizeCol - 1; i >= 0; --i)
        for (int j = sizeRow - 1; j >= 0; --j)
        {
            //访问过了则跳过
            if(visited[i][j] || matrix[i][j] <= 0) continue;
            visited[i][j] = 1;
            // 未访问 添加到访问队列
            unionSet.push(binary{matrix[i][j],i,j});
            // 结果联通图
            vector<vector<int>> res(sizeCol, vector<int>(sizeRow, 0));
            res[i][j]=1;
            //bfs
            while(!unionSet.empty()){
                auto top = unionSet.front();
                unionSet.pop();
                int ii = top.i;
                int jj = top.j;
                //向下
                if(ii+1<sizeCol && visited[ii+1][jj]==0)
                if(matrix[ii+1][jj] == top.cata){
                    unionSet.push(binary{matrix[ii+1][jj],ii+1,jj});
                    res[ii+1][jj]=1;
                    visited[ii+1][jj]=1;
                }
                //向左
                if(jj-1>=0 && visited[ii][jj-1]==0)
                if(matrix[ii][jj-1] == top.cata){
                    unionSet.push(binary{matrix[ii][jj-1],ii,jj-1});
                    res[ii][jj-1]=1;
                    visited[ii][jj-1]=1;
                }
                //向右
                if(jj+1<sizeRow && visited[ii][jj+1]==0)
                if(matrix[ii][jj+1] == top.cata){
                    unionSet.push(binary{matrix[ii][jj+1],ii,jj+1});
                    res[ii][jj+1]=1;
                    visited[ii][jj+1]=1;
                }
                //向上
                if(ii-1>=0 && visited[ii-1][jj]==0)
                if(matrix[ii-1][jj] == top.cata){
                    unionSet.push(binary{matrix[ii-1][jj],ii-1,jj});
                    res[ii-1][jj]=1;
                    visited[ii-1][jj]=1;
                }
            }

            //判断结果联通图是否满足combo条件，若满足修改原数组
            bool combo = comboDFS(res);
            if(combo){
                count++;
                for (int a = sizeCol - 1; a >= 0; --a)
                for (int b = sizeRow - 1; b >= 0; --b)
                if(res[a][b]<0)matrix[a][b] = 0;
            }
        }
        //如果发生了消除 则进行掉落处理 并且进行递归
        if(count>0){
            finish(matrix,isprint);
            count+=calCulate(matrix, catag,isprint);
        }
    return count;
}
inline void printVector(vector<vector<int>> a){
    cout<<endl;
    cout<<"---------开始打印数组----------"<<endl;
    for(auto it:a){
        for(auto iit:it) cout<<iit<<" ";
        cout<<endl;
    }
}
