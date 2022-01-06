#include<bits/stdc++.h>
using namespace std;

void display(int a[9][9]){
    cout<<"------------------------";
    for(int i=0;i<9;i++){
        if(i%3==0) cout<<"\n";
        for(int j=0;j<9;j++){
            if(j%3==0) cout<<"  ";
            if(a[i][j]!=0) cout<<a[i][j]<<" ";
            else cout<<". ";
        }
        cout<<"\n";
    }
    cout<<"------------------------\n";
}

void sit(bool a[9][9]){
    cout<<"------------------------";
    for(int i=0;i<9;i++){
        if(i%3==0) cout<<"\n";
        for(int j=0;j<9;j++){
            if(j%3==0) cout<<"  ";
            if(a[i][j]!=0) cout<<a[i][j]<<" ";
            else cout<<". ";
        }
        cout<<"\n";
    }
    cout<<"------------------------\n";
}


bool isinRow(int a[9][9], int num, int r, int c){
    bool present = false;
    for(int i=0;i<9;i++){
        if(i==c) continue;
        if(a[r][i]==num) {present = true; break;}
    }
    return present;
}

bool isinColumn(int a[9][9], int num, int r, int c){
    bool present = false;
    for(int i=0;i<9;i++){
        if(i==r) continue;
        if(a[i][c]==num) {present = true; break;}
    }
    return present;
}

bool isinBox(int a[9][9], int num, int r, int c){
    bool present = false;
    for(int i=3*(r/3); i<3*((r+3)/3); i++){
        for(int j=3*(c/3); j<3*((c+3)/3); j++){
            if(i==c && j==r) continue;
            if(a[i][j]==num) {present = true; break;}
        }
        if(present) break;
    }
    return present;
}

bool isValid(int a[9][9], int num, int r, int c){
    return (!isinRow(a, num, r, c)) && (!isinColumn(a, num, r, c)) && (!isinBox(a, num, r, c));
}

pair<int,int> previous(int i, int j, bool orig[9][9]){
    if(j>0) j--;
    else if(j==0 && i>0) {i--; j=8;}
    else if(j==0 && i==0) {i=-1; j=-1;}
    while(orig[i][j]){
        if(j>0) j--;
        else if(j==0 && i>0) {i--; j=8;}
        else if(j==0 && i==0) {i=-1; j=-1;}
    }
    return make_pair(i,j);
}

pair<int, int> next(int i, int j, bool orig[9][9]){
    if(j<8) j++;
    else if(j==8 && i<8) {i++; j = 0;}
    else if(j==8 && i==8) {i = 10; j = 10;}
    while(orig[i][j]){
        if(j<8) j++;
        else if(j==8 && i<8) {i++; j = 0;}
        else if(j==8 && i==8) {i = 10; j = 10;}   
    }
    return make_pair(i, j);
}

void solve(int a[9][9], bool orig[9][9]){
    int i=0, j=0;
    bool solvable = false;
    while(true){
        if(i==-1 && j==-1) {solvable = false; break;}
        if(i*j==100) {solvable = true; break;}
        if(orig[i][j]){
            pair<int, int> nex = next(i, j, orig);
            i = nex.first;
            j = nex.second;
            continue;
        }
        if(a[i][j]==0){ 
            int num;
            bool filled = false;
            for(num=1;num<10;num++){
                filled = isValid(a, num, i, j);
                if(filled) {
                    a[i][j] = num; 
                    pair<int, int> nex = next(i, j, orig);
                    i = nex.first;
                    j = nex.second;
                    break;
                } 
            }
            if(!filled) {
                a[i][j] = 0;
                pair<int, int> prev = previous(i, j, orig);
                i = prev.first;
                j = prev.second;
            }
            continue;
        }
        if(a[i][j]!=0 && !orig[i][j]){
            int num = a[i][j]+1;
            bool filled = false;
            for(; num<10; num++){
                filled = isValid(a, num, i, j);
                if(filled) {
                    a[i][j] = num;
                    pair<int, int> nex = next(i, j, orig);
                    i = nex.first;
                    j = nex.second;
                    break;
                } 
            }
            if(!filled) {
                a[i][j] = 0;
                pair<int, int> prev = previous(i, j, orig);
                i = prev.first;
                j = prev.second;
            }
            continue;
        }
    }
    if(solvable) {cout<<"Sudoku is solvable\n"; display(a);}
    else cout<<"Sudoku is unsolvable.\n";
}

int main(){
    int s[9][9] = {
        { 8, 0, 0,   0, 0, 0,   0, 0, 0 },
        { 0, 0, 3,   6, 0, 0,   0, 0, 0 },
        { 0, 7, 0,   0, 9, 0,   2, 0, 0 },
            
        { 0, 5, 0,   0, 0, 7,   0, 0, 0 },
        { 0, 0, 0,   0, 4, 5,   7, 0, 0 },
        { 0, 0, 0,   1, 0, 0,   0, 3, 0 },
            
        { 0, 0, 1,   0, 0, 0,   0, 6, 8 },
        { 0, 0, 8,   5, 0, 0,   0, 1, 0 },
        { 0, 9, 0,   0, 0, 0,   4, 0, 0 }
    };
    bool orig[9][9];
    for(int i=0;i<9;i++) for(int j=0;j<9;j++) orig[i][j] = s[i][j];
    solve(s,orig);
}