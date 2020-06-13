
#include <iostream>
#include <cmath>
using namespace std;



class Sudoku{
	//sudoku is split up into blocks of nxn
	//the entire board is n^2 x n^2
	int size;
	//storing the square of the size (n^2), so we do not have to calculate it everytime
	int sizeSquared;	
	
	//class which represents a sudoku entry (inner class)
	class SudokuEntry{
		//field to determine if this is a given or not
		bool given;
		//number which the entry is set to (if it is -1, then considered non-set)
		int num;
		//numbers that are prohibited for this entry to take
		//based on sudoku rules
		unordered_set <int> prohibitedNums;
		//length of possibleNums saved
		int numPossible;

		SudokuEntry(){
			this->given=false;
			this->num=-1;
			availableNums=new unordered_set <int>;	
			for (int i=1;i<10;i++){
				availableNums.insert(i);
			}
			this->numPossible=9;
		}

	}

	//sudoku board has a matrix of SudokuEntries 
	//size is n^2 x n^2
	SudokuEntry entries[];

	//sudoku board has a list of "givens" to help find the solution
	//basically we have an array with positions:
	//row ---- column ---- value
	//where row and column are from 0 to sizeSquared-1
	
	vector <int> givens;

	//sudoku board has a stack of operations to help backtrack when we hit a dead end
	stack<> ops;

	Sudoku(size){
		//creating our sudoku object
		//initializing the size and size squared
		this->size=size;
		this->sizeSquared=pow(size,2)

		//initializing our SudokuEntries
		entries = new SudokuEntry[size];	

		givens = new vector <int>;
		
	}

	// methods to help us with sudoku operations
	
	//method that sets a whole column an int as a forbidden value
	//col from 0 to sizeSquared-1
	void setColumn(int forbidden,int col){
		for (int i=0;i<this->sizeSquared;i++){
			SudokuEntry toConsider =(this->entries)[col+(i*(this->sizeSquared))];
			if (toConsider->given==false){
				//then we set the forbidden value
				(toConsider->availableNums).erase(forbidden);
			}
		}		
	}

	//method that sets a whole row an int as a forbidden value
	void setRow(int forbidden,int row){
		for (int i=0;i<this->sizeSquared;i++){
			SudokuEntry toConsider =(this->entries)[i+(row*(this->sizeSquared))];
			if (toConsider->given==false){
				//then we set the forbidden value
				(toConsider->availableNums).erase(forbidden);
			}
		}
	}

	//method that sets a whole block as a forbidden value
	//column from 0-sizeSquared-1 and row from 0-sizeSquared-1
	void setBlock(int forbidden,int row, int column){
		//block is nxn or size x size
	}
	// method to set entry details based on the entries in the same column and row and block
	void setInitialEntries(){
		//first order of business is to set the givens
		//once I set a given, I should iterate along the row and column of the given to "remove" that given value from the entries possible values
		//I should also iterate in the block of the given and "remove" the given value from the entries possible values
		//I will just use the helper methods created above
	}
};

int main(){
		//user I/O to prompt the user to enter sudoku size and givens in the sudoku
		//then the program will print the solution to the sudoku puzzle	
	return 0;
}
