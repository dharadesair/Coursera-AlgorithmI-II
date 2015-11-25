#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Quick Union
class UnionFind {
public:
    UnionFind(int N){
        for(int i = 0; i < N ; i++) top.push_back(i);
    }
    int Find(int i){
        if(top[i] == 1 || top[i] == -1)
            return i;
        else if(top[i] == i)
            return i;
        else
            return Find(top[i]);
    }
    int Union(int x, int y){
        int xInd = Find(x);
        int yInd = Find(y);
        // Detects Percolation
        if((top[xInd] == 1 && top[yInd] == -1) || (top[xInd] == -1 && top[yInd] == 1))
            return 1;
        // If not Same root
        else if(xInd != yInd){
            top[xInd] = yInd;
            return -1;
        }
        // Already in same union
        return 0;
    }
    vector<int> top;
};

class Percolation {
public:
    Percolation(int N);             // create N-by-N grid, with all sites blocked
    void open(int i, int j);        // open site (row i, column j) if it is not open already
    bool isOpen(int i, int j);      // is site (row i, column j) open?
    bool isFull(int i, int j);      // is site (row i, column j) full?
    bool percolates();              // does the system percolate?
    int getNum(int row, int col);
private:
    vector<vector<int>> grid;
};

// create N-by-N grid, with all sites blocked (0)
Percolation::Percolation(int N){
    vector<int> row(N,0);
    for(int index = 0; index < N; index++)
        grid.push_back(row);
}

// is site (row i, column j) open?
bool Percolation::isOpen(int i, int j){
    return (grid[i][j] == 1);
}

// is site (row i, column j) full?
bool Percolation::isFull(int i, int j){
    return (grid[i][j] == 0);
}

// open site (row i, column j) if it is not open already
void Percolation::open(int i, int j){
    grid[i][j] = 1;
}
// Mapping the grid to Union array
int Percolation::getNum(int row, int col){
    return (row * grid[0].size() + col);
}
// does the system percolate?
bool Percolation::percolates(){
    int rows = grid.size();
    if(rows == 0) return false;
    int cols = grid[0].size();
    
    int N = (rows-1)*cols + (cols-1);
    UnionFind uf(N);
    
    // BFS from both top
    queue<int> top,bottom;
    for(int col = 0; col < cols; col++){
        if(isOpen(0,col)){
            uf.top[col] = 1;
            top.push(0);top.push(col);
        }
    }
    // BFS from bottom
    for(int col = 0; col < cols; col++){
        if(isOpen(rows-1,col)){
            uf.top[(rows-1)*cols + col] = -1;
            bottom.push(rows-1);bottom.push(col);
        }
    }
    
    // BFS algorithm
    int tI, tJ, ret;
    while(!top.empty() || !bottom.empty()){
        if(!top.empty()){
            tI = top.front();top.pop();
            tJ = top.front(); top.pop();
            // Basic Percolation Flows
            // Check Left
            if(tI>=0 && tI<rows && tJ>=0 && tJ < cols){
            if(tI-1>=0 && grid[tI-1][tJ] == 1){
                if((ret = uf.Union(getNum(tI-1,tJ),getNum(tI,tJ))) == -1){
                    top.push(tI-1);top.push(tJ);
                } else if(ret == 1)
                    return true;
                
            }
            // Check Right
            if(tJ+1 < cols && grid[tI][tJ + 1] == 1){
                if((ret = uf.Union(getNum(tI,tJ+1),getNum(tI,tJ)))==-1){
                    top.push(tI);top.push(tJ+1);
                }else if(ret == 1)
                    return true;
                
            }
            // Check Bottom
            if(tI+1 < rows && grid[tI+1][tJ] == 1){
                if(( ret = uf.Union(getNum(tI+1,tJ),getNum(tI,tJ)))==-1){
                    top.push(tI+1);top.push(tJ);
                }else if(ret == 1)
                    return true;
                
            }
            }
        }
        
        if(!bottom.empty()){
            tI = bottom.front();bottom.pop();
            tJ = bottom.front(); bottom.pop();
            
            // Basic Percolation Flows
            // Check Left
            if(tI>=0 && tI<rows && tJ>=0 && tJ < cols){
            if(tI-1>=0 && grid[tI-1][tJ] == 1){
                if((ret = uf.Union(getNum(tI-1,tJ),getNum(tI,tJ)))== -1){
                    bottom.push(tI-1);bottom.push(tJ);
                }else if(ret == 1)
                    return true;
                
            }
            // Check Right
            if(tJ+1 < cols && grid[tI][tJ+1] == 1){
                if((ret = uf.Union(getNum(tI,tJ+1),getNum(tI,tJ)))==-1){
                    bottom.push(tI);bottom.push(tJ+1);
                }else if(ret == 1)
                    return true;
                
            }
            // Check top
            if(tI-1 >=0 && grid[tI-1][tJ] == 1){
                if((ret =uf.Union(getNum(tI-1,tJ),getNum(tI,tJ)))==-1){
                    bottom.push(tI+1);bottom.push(tJ);
                }else if(ret == 1)
                    return true;
                
            }
            }

        }
    }
    return false;
}

int main(int argc, char** argv){
    int N = 20;
    
    Percolation testPercolation(N);
    int openSites = 0;
    bool canPercolate = false;
    
    srand (time(NULL));
    do{
        // Random number between 0 and N-1
        int row = rand() % (N);
        int col = rand() % (N);
        
        if( !testPercolation.isOpen(row, col)){
            //cout<<"Opening "<<row<<" "<<col;
            openSites++;
            testPercolation.open(row,col);
            canPercolate = testPercolation.percolates();
            if(canPercolate) cout<<"Can Percolate with ";
            //else cout<<"  Cannot Percolate"<<endl;
        }
        
    }while(!canPercolate);
    
    cout<<"open Sites ="<<openSites<<endl;
}