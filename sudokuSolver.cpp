
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
		unordered_set <int> availableNums;

		SudokuEntry(){
			given=false;
			num=-1;
			availableNums=new unordered_set <int>;	
			for (int i=1;i<10;i++){
				availableNums.insert(i);
			}
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
	//the stack stores the position of the last modified SudokuEntry
	//WE NEED SOME WAY TO FIGURE OUT EXACTLY WHICH NUMS TO TRY
	stack<int> ops;

	Sudoku(size){
		//creating our sudoku object
		//initializing the size and size squared
		this->size=size;
		this->sizeSquared=pow(size,2)

		//initializing our SudokuEntries
		//we have sizeSquared x sizeSquared entries
		entries = new SudokuEntry[sizeSquared*sizeSquared];	

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
	void setBlock(int forbidden,int row, int col){
		//block is nxn or size x size
		//row should be a multiple of size and same with column
		//if not, then I should pick the last multiple of size for both and iterate through that block
		rowOffset = row%size;
		colOffset=col%size;
		fixedRow=row-rowOffset;
		fixedCol=col-colOffset;
		for (int i=fixedRow;i<fixedRow+size;i++){
			for (int j=fixedCol;j<fixedCol+size;j++){

				//setting forbidden values for each entry
				SudokuEntry toConsider = entries[i+(j*sizeSquared)];
				if (toConsider->given==false){
					toConsider->availableNums.erase(forbidden);
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
			row=givens[i];	
			col=givens[i+1];
			value=givens[i+2];

			//setting the entry
			SudokuEntry toConsider = entries[col+(row*sizeSquared)];
			toConsider->given=true;
			toConsider->num=value;
			
			//setting forbidden entries in the same block, column, and row as this entry
			setRow(value,row);
			setColumn(value,col);
			setBlock(value,row,col);


		}
		//now all the given entries should be set
	}

	//finds the sudoku entry with the least number of available choices
	int findEntryToFill(){

	}

	
};

int main(){
		//user I/O to prompt the user to enter sudoku size and givens in the sudoku
		//then the program will print the solution to the sudoku puzzle	
	return 0;
}
