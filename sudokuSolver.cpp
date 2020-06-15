
#include <iostream>
#include <cmath>
#include <vector>
#include <stack>
#include <unordered_set>

using std::cout;
using std::cin;
using std::vector;
using std::stack;
using std::unordered_set;





class Sudoku{
public:
	class SudokuEntry{
		//field to determine if this is a given or not
	public:
		bool given;
		//number which the entry is set to (if it is -1, then considered non-set)
		int value;
		//numbers that are prohibited for this entry to take
		//based on sudoku rules
		unordered_set <int> availableNums;

		SudokuEntry(Sudoku* parent){
			given=false;
			value=-1;
			int size = parent->size;
			//availableNums=new std::unordered_set <int>;	
			for (int i=1;i<=size;i++){
				availableNums.insert(i);
			}
		}

	};
public:
	//sudoku is split up into blocks of nxn
	//the entire board is n^2 x n^2
	int size;
	//storing the square of the size (n^2), so we do not have to calculate it everytime
	int sizeSquared;	
	
	//class which represents a sudoku entry (inner class)


	
	//sudoku board has a list of "givens" to help find the solution
	//basically we have an array with positions:
	//row ---- column ---- value
	//where row and column are from 0 to sizeSquared-1
	vector <int> givens;

	//sudoku board has a stack of operations to help backtrack when we hit a dead end
	//the stack stores the position of the last modified SudokuEntry
	//WE NEED SOME WAY TO FIGURE OUT EXACTLY WHICH NUMS TO TRY
	stack<int> ops;

	//sudoku board has a matrix of SudokuEntries 
	//size is n^2 x n^2
	//dynamically created
	//vector <SudokuEntry> entries;
	vector<SudokuEntry> entries;


	Sudoku(int mag){
		//creating our sudoku object
		//initializing the size and size squared
		size=mag;
		sizeSquared=pow(mag,2);

		//initializing our SudokuEntries
		int bigSize = sizeSquared*sizeSquared;
		entries.reserve(bigSize);
		for (int i=0;i<bigSize;i++){
			entries.push_back(SudokuEntry(this));
		}
		//we have sizeSquared x sizeSquared entries
		//entries = new SudokuEntry[sizeSquared*sizeSquared](this);	

		//givens = new vector <int>;
		
	}

	// methods to help us with sudoku operations
	
	//method that sets a whole column an int as a forbidden value
	//col from 0 to sizeSquared-1
	void setColumn(int forbidden,int col){
		for (int i=0;i<sizeSquared;i++){
			SudokuEntry toConsider =(entries)[col+(i*(sizeSquared))];
			if (toConsider.given==false){
				//then we set the forbidden value
				(toConsider.availableNums).erase(forbidden);
			}
		}		
	}

	//method that sets a whole row an int as a forbidden value
	void setRow(int forbidden,int row){
		for (int i=0;i<sizeSquared;i++){
			SudokuEntry toConsider =(entries)[i+(row*(sizeSquared))];
			if (toConsider.given==false){
				//then we set the forbidden value
				(toConsider.availableNums).erase(forbidden);
			}
		}
	}

	//method that sets a whole block as a forbidden value
	//column from 0-sizeSquared-1 and row from 0-sizeSquared-1
	void setBlock(int forbidden,int row, int col){
		//block is nxn or size x size
		//row should be a multiple of size and same with column
		//if not, then I should pick the last multiple of size for both and iterate through that block
		int rowOffset = row%size;
		int colOffset=col%size;
		int fixedRow=row-rowOffset;
		int fixedCol=col-colOffset;
		for (int i=fixedRow;i<fixedRow+size;i++){
			for (int j=fixedCol;j<fixedCol+size;j++){

				//setting forbidden values for each entry
				SudokuEntry toConsider = entries[i+(j*sizeSquared)];
				if (toConsider.given==false){
					toConsider.availableNums.erase(forbidden);
				}
			}

		}
	}
	// method to set entry details based on the entries in the same column and row and block
	void setInitialEntries(){
		//first order of business is to set the givens
		//once I set a given, I should iterate along the row and column of the given to "remove" that given value from the entries possible values
		//I should also iterate in the block of the given and "remove" the given value from the entries possible values
		//I will just use the helper methods created above
		
		//first index has row, second index has column, third index has the value
		for (int i=0;i<givens.size();i+=3){
			int row=givens[i];	
			int col=givens[i+1];
			int val=givens[i+2];

			//setting the entry
			SudokuEntry toConsider = entries[col+(row*sizeSquared)];
			toConsider.given=true;
			toConsider.value=val;
			
			//setting forbidden entries in the same block, column, and row as this entry
			setRow(val,row);
			setColumn(val,col);
			setBlock(val,row,col);


		}
		//now all the given entries should be set
	}

	//finds the sudoku entry with the least number of available choices
	int findEntryToFill(){
		//basically iterates through all entries
		//skips those which are assigned and given
		//then returns the position of the entry with the lowest number of possibilities
		int min =-1;
		int len= sizeSquared*sizeSquared;
		for (int i=0;i<len;i++){
			SudokuEntry toConsider = entries[i];
			if ((toConsider.given==false)){
				if (toConsider.value==-1){
					//then we found an unassigned value with possibilities
					if (min==-1){
						min=i;
					} else{
						//we should compare
						if ((entries[min].availableNums).size()>(toConsider.availableNums).size()){
							//then we can set this as the min
							min=i;
						}
					}

				}
			}
		}
		if (min==-1){
			//then there is nothing left in the sudoku board to choose
			//meaning we have a solution!
			return -2;
		} else if ((entries[min].availableNums).size()==0){
			//then we hit a dead end
			return -1;
		}
		return min;
	}

	//does an iteration of the sudoku algorithm based on the entry with least possibilities
	void setSudokuValue(int minPosition){
		//basically if the value is -1, we pick the first value
		//else we pick the next value
		int toSet;
		if ((entries[minPosition].value)==-1){
			//then we iterate through the values and see if they are in the unordered_set
			for (int i=1;i<=sizeSquared;i++){
				unordered_set<int>::const_iterator found = (entries[minPosition].availableNums).find(i);

				if (found!=(entries[minPosition].availableNums).end()){
					//then this is in the availableNums
					toSet = *found;
					(entries[minPosition].value)=toSet;
					break;
				}
			}
		} else {
			//then a value is already set
			int val = (entries[minPosition].value);
			for (int i=val+1;i<=sizeSquared;i++){
				unordered_set<int>::const_iterator found = (entries[minPosition].availableNums).find(i);

				if (found!=(entries[minPosition].availableNums).end()){
					//then this is in the availableNums
					toSet = *found;
					(entries[minPosition].value)=toSet;
					break;
				}
			}
		}
		//setting forbidden entries in the same block, column, and row as this entry

		//we should also push this operation to the stack/queue for backtracking
		//we just push the position of the change onto the stack
		ops.push(minPosition);
		//minPostion = col + (sizeSquared*row)
		int col = minPosition%sizeSquared;
		int row = (minPosition-col)/sizeSquared;
		setRow(toSet,row);
		setColumn(toSet,col);
		setBlock(toSet,row,col);

	}
	
	//method to check if a value is in a row
	bool checkRow(int val,int row){
		for (int i=0;i<sizeSquared;i++){
			SudokuEntry toConsider =(entries)[i+(row*(sizeSquared))];
			if (toConsider.value==val){
				//then we set the forbidden value
				return true;
			}
		}
		return false;
	}

	//method to check if a value is in a column
	bool checkCol(int val,int col){
		for (int i=0;i<sizeSquared;i++){
			SudokuEntry toConsider =(entries)[col+(i*(sizeSquared))];
			if (toConsider.value==val){
				//then we set the forbidden value
				return true;
			}
		}
		return false;
	}

	//method to check if a value is in a block
	bool checkBlock(int val,int row, int col){
		int rowOffset = row%size;
		int colOffset=col%size;
		int fixedRow=row-rowOffset;
		int fixedCol=col-colOffset;
		for (int i=fixedRow;i<fixedRow+size;i++){
			for (int j=fixedCol;j<fixedCol+size;j++){

				//setting forbidden values for each entry
				SudokuEntry toConsider = entries[i+(j*sizeSquared)];
				if (toConsider.value==val){
					return true;
				}
			}

		}
		return false;
	}


	void setSpecificRow(int value, int row){
		for (int i=0;i<sizeSquared;i++){
			int pos = (i+(row*(sizeSquared)));
			SudokuEntry toConsider =(entries)[pos];
			if (toConsider.given==false){
				//then we set the forbidden value
				if (!(checkCol(value,i) && checkBlock(value,i,row))){
					//then we can safely insert	
					(toConsider.availableNums).insert(value);
				}
			}
		}
	}

	void setSpecificCol(int value,int col ){
		for (int i=0;i<sizeSquared;i++){
			SudokuEntry toConsider =(entries)[col+(i*(sizeSquared))];
			if (toConsider.given==false){
				if (!(checkRow(value,i) && checkBlock(value,col,i))){
					//then we insert value
					(toConsider.availableNums).insert(value);

				}
			}
		}
	}

	void setSpecificBlock(int value, int row, int col){
		//block is nxn or size x size
		//row should be a multiple of size and same with column
		//if not, then I should pick the last multiple of size for both and iterate through that block
		int rowOffset = row%size;
		int colOffset=col%size;
		int fixedRow=row-rowOffset;
		int fixedCol=col-colOffset;
		for (int i=fixedRow;i<fixedRow+size;i++){
			for (int j=fixedCol;j<fixedCol+size;j++){

				//setting forbidden values for each entry
				SudokuEntry toConsider = entries[i+(j*sizeSquared)];
				if (toConsider.given==false){
					if (!(checkRow(value,j) && checkCol(value,i))){
						//then we can safely add	
						toConsider.availableNums.insert(value);
					}
				}
			}

		}
	}

	//method to help with backtracking (stack operations)
	void backTrack(){
		//idea:
		//we first pop what was in the stack
		while (!ops.empty()){

		
			int lastOpPos = ops.top();
			ops.pop() ;
			//if there is another greater value for this we can choose, we choose it and push back to stack and continue
			//pos = col +(sizeSquared*row)
			int col=lastOpPos%sizeSquared;
			int row = (lastOpPos-col)/sizeSquared;
			SudokuEntry toConsider = entries[lastOpPos];
			int val = toConsider.value;
			int toSet = 0;
			for (int i=val+1;i<=sizeSquared;i++){ unordered_set<int>::const_iterator found = (toConsider.availableNums).find(val);
				if (found!=(toConsider.availableNums).end()){
					toSet=*found;
					break;
				}
			}
			//resetting the value
			(toConsider.value)=-1;
			//need to reset value for other entries
			setSpecificRow(val,row);
			setSpecificCol(val,col);
			setSpecificBlock(val,col,row);


			if (toSet==0){
				//move on in the stack
			} else {
				//set new value and push
				toConsider.value=toSet;
				setRow(toSet,row);
				setColumn(toSet,col);
				setBlock(toSet,col,row);
				ops.push(col+(sizeSquared*row));


			}
			//figure out a way to reset the add
				//idea: first, set the value to -1 and add the old value to the possible Values
				//then, for the column, row, and block, I add the old value to the possible Values
				//then, for the column, row , and block, I need to see if there is the value there, if so, I have to remove it from possible Values
			//otherwise, we set to -1 and pop the next part of the stack (if any and continue)
			//if stack is empty, then we just have to restart sudoku from the givens
		}
		// stack is guarunteed to not be empty if there is a valid solution (because we can always change some move to get a different path) 
	}

	//final method which uses our helper methods to solve the sudoku
	void solveSudoku(){
		//first we initialize the givens
		//then we find the entry with least possibilities	
		//we set the entry
		//continue finding next entry
		//if we find an entry, but zero possibilities, we need to backtrack
		//if we have no more entries in search, then we have our solution
		setInitialEntries();

		//while loop for algorithm
		while (true){
			int entryFound = findEntryToFill();
			
			if (entryFound==-2){
				//then we have found our solution
				//so, we can break and print
				break;
			} else if (entryFound==-1){
				//then we need to backtrack
				backTrack();
			} else {
				//then we can safely set the value
				setSudokuValue(entryFound);
			}
		}
		printSudoku();
	}

	//method to print our sudoku in fancy fashion (blocks are indicated)
	void printSudoku(){
		//desire:
		//x x x | x x x | x x x | 
		//x x x | x x x | x x x |
		//x x x | x x x | x x x |
		//_ _ _ | _ _ _ | _ _ _ | 
		//x x x | x x x | x x x |
		//x x x | x x x | x x x | 
		//x x x | x x x | x x x |
		//_ _ _ | _ _ _ | _ _ _ |
		//x x x | x x x | x x x |
		//x x x | x x x | x x x | 
		//x x x | x x x | x x x |
		//_ _ _ | _ _ _ | _ _ _ |	
		
		//basically we should go through each block and find the max # of digits
		//this way we can get a uniform padding for the printing
		//or we could just print out each line as is :)
		cout << " SUDOKU BOARD : \n";
		for (int i=0;i<sizeSquared*sizeSquared;i++){
			cout << entries[i].value;
			if ((i+1)%(sizeSquared)==0){
				//then this is the last num of a row
				cout << "\n";
			}
		}
		cout << "\n";
	}

	
};

int main(){
	//user I/O to prompt the user to enter sudoku size and givens in the sudoku
	//then the program will print the solution to the sudoku puzzle	
	cout << "Please enter the size of the sudoku blocks below. (I.E 9x9 sudoku has block size of 3, (3^2 x 3^2)) . \n";
	int size;
	cin >> size;
	//initializing my Sudoku game
	Sudoku game = Sudoku(size);

	//now getting user to enter givens
	return 0;
}
