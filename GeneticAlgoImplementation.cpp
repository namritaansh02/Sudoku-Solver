#include<bits/stdc++.h>
using namespace std;

class game{
public:
    int a[9][9];
    bool fixed[9][9];
    game(int input[9][9]){
        for(int i=0;i<9;i++) for(int j=0;j<9;j++) {
            a[i][j] = input[i][j]; 
            if(a[i][j]!=0) fixed[i][j] = 1;
        }
    }
    void show(game);
};

void show(game board){
    for(int i=0;i<9;i++){
        if(i%3==0) cout<<"---------------------------\n";
        for(int j=0;j<9;j++){
            if(j%3==0) cout<<"| ";
            cout<<board.a[i][j]<<" ";
        }
        cout<<"\n";
    }
    cout<<"---------------------------\n";
}

class solution{
public:
    int sol[9][9], fitness=0;
    solution(){
        for(int i=0;i<9;i++) for(int j=0;j<9;j++) sol[i][j] = 1+rand()%9;
    }
    solution(game board){
        for(int i=0;i<9;i+=3){
            for(int j=0;j<9;j+=3){
                unordered_set<int> box_val;
                for(int x=i;x<i+3;x++){
                    for(int y=j;y<j+3;y++) {
                        box_val.insert(board.a[x][y]);
                        sol[x][y] = board.a[x][y];
                    }
                }
                int iter = 1;
                for(int x=i;x<i+3;x++){
                    for(int y=j;y<j+3;y++) {
                        while(box_val.find(iter) != box_val.end()) iter++;
                        if(sol[x][y]==0) sol[x][y] = iter++;
                    }
                }
            }
        }

        for(int i=0;i<9;i++) {
            unordered_set<int> s;
            for(int j=0;j<9;j++) s.insert(sol[i][j]);
            fitness += 9-s.size();
        }
        
        for(int j=0;j<9;j++) {
            unordered_set<int> s;
            for(int i=0;i<9;i++) s.insert(sol[i][j]);
            fitness += 9-s.size(); 
        }
    }
    bool operator<(const solution& x) const{
        return fitness < x.fitness;
    }
    solution update_score(solution);
    solution mutate(solution, game);
    solution mate(solution, solution, game); 
};

solution update_score(solution current_sol){
    //(1) Each block should have digits from 1-9 (Already Satsified)
    //(2) Each row should have digits from 1-9
    //(3) Each column should have digits from 1-9

    current_sol.fitness = 0;

    for(int i=0;i<9;i++) {
        unordered_set<int> s;
        for(int j=0;j<9;j++) s.insert(current_sol.sol[i][j]);
        current_sol.fitness += 9-s.size();
    }
    
    for(int j=0;j<9;j++) {
        unordered_set<int> s;
        for(int i=0;i<9;i++) s.insert(current_sol.sol[i][j]);
        current_sol.fitness += 9-s.size(); 
    }
    
    return current_sol;
}

solution mutate(solution current_sol, game board){
    int num_mutate = 8; 
    if(current_sol.fitness<=15) num_mutate = 4;
    for(int n=0;n<num_mutate;n+=2){
        int x = 3*(rand()%3), y = 1+rand()%2;
        for(int i=0;i<9;i++){
            swap(current_sol.sol[i][x],current_sol.sol[i][x+y]);
        }
        for(int i=0;i<9;i++){
            swap(current_sol.sol[x][i],current_sol.sol[x+y][i]);
        }
    }
    current_sol = update_score(current_sol);
    return current_sol;
}

solution mate(solution sol1, solution sol2, game board){
    solution child_sol;
    for(int i=0;i<9;i+=3){
        for(int j=0;j<9;j+=3){
            unordered_set<int> box_val;
            for(int x=i;x<i+3;x++){
                for(int y=j;y<j+3;y++) {
                    if(sol1.sol[x][y] == sol2.sol[x][y]) {
                        box_val.insert(sol1.sol[x][y]);
                        child_sol.sol[x][y] = sol1.sol[x][y];
                    }
                    else {
                        int iter = 1+rand()%2;
                        if(iter == 1) child_sol.sol[x][y] = sol1.sol[x][y];
                        else child_sol.sol[x][y] = sol2.sol[x][y];
                    }
                }
            }
        }
    }
    child_sol = update_score(child_sol);
    return child_sol;
}

int main(){
    int iterations = 100000;
    cout<<"Enter the sudoku with zero as empty element.\n";
    int a[9][9] = { {0,8,0, 0,0,0, 2,0,0},
                    {0,0,0, 0,8,4, 0,9,0},
                    {0,0,6, 3,2,0, 0,1,0},
                    {0,9,7, 0,0,0, 0,8,0},
                    {8,0,0, 9,0,3, 0,0,2},
                    {0,1,0, 0,0,0, 9,5,0},
                    {0,7,0, 0,4,5, 8,0,0},
                    {0,3,0, 7,1,0, 0,0,0},
                    {0,0,8, 0,0,0, 0,4,0} };
    game board(a);
    show(board);
    vector<solution> sols;
    sols.push_back(solution(board));
    show(game(sols[0].sol));
    while(iterations-- ||sols[0].fitness == 0){
        for(int i=0;i+2<sols.size() && i<=30;i+=2) sols.push_back(mate(sols[i], sols[sols.size()+2], board));
        for(int i=0;i<sols.size() && i<=20;i++) sols.push_back(mutate(sols[i], board));
        sort(sols.begin(), sols.end());
        cout<<sols[0].fitness<<"\n";
        show(game(sols[0].sol));
    }
}

    // for(int i=0;i<9;i+=3){
    //     for(int j=0;j<9;j+=3){
    //         unordered_set<int> s;
    //         for(int x=i;x<i+3;x++){
    //             for(int y=j;y<j+3;y++) s.insert(current_sol.sol[x][y]);
    //         }
    //         current_sol.box_score[(i-1)+(j/3)] = 9-s.size(); 
    //     }
    // }

//    int box = rand()%9; //0-8 box numbers
//         int i = 3*(n/3), j = 3*(n%3);
//         unordered_set<int> box_val;
//         for(int x=i;x<i+3;x++){
//             for(int y=j;y<j+3;y++) {
//                 box_val.insert(board.a[x][y]);
//                 current_sol.sol[x][y] = board.a[x][y];
//             }
//         }
//         int iter = 1;
//         for(int x=i;x<i+3;x++){
//             for(int y=j;y<j+3;y++) {
//                 while(box_val.find(iter) != box_val.end()) iter++;
//                 if(board.a[x][y]==0) current_sol.sol[x][y] = iter++;
//             }
//         }
//     }