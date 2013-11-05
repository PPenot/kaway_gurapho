#include "Matrix.h"
#include "exception.h"

using namespace std;

Matrix::Matrix(int rows, int cols, float d_v)
{
	if(rows<0)
		throw Exception((char*)"rows<0",(char*)"Matrix::Matrix(int rows, int cols, float d_v)");
	if(cols<0)
		throw Exception((char*)"cols<0",(char*)"Matrix::Matrix(int rows, int cols, float d_v)");

	this->rows = rows;
	this->cols = cols;
	this->default_value = d_v;
	this->values = vector<vector<float> >(rows, vector<float>(cols,d_v));
}

Matrix::Matrix(const Matrix& m)
{
	rows = m.rows;
	cols = m.cols;
	default_value = m.default_value;
	values = vector<vector<float> >(rows, vector<float>(cols,default_value));

	for(int i=0; i<rows; i++)
		for(int j=0; j<cols; j++)
			values[i][j] = m.values[i][j];
}

void Matrix::print() const
{
	if(rows==0)
		cout << "Matrix with no row" << endl;
	else if(cols==0)
		cout << "Matrix with no column" << endl;
	else
	{
		for(int i=0;i<rows;i++)
		{
			cout << "( ";
			for(int j=0;j<cols;j++)
				cout << values[i][j] << ' ';
			cout << ')' << endl;
		}
	}
}

void Matrix::fprint(ostream& file) const
{
	if(rows==0)
		file << "Matrix with no row" << endl;
	else if(cols==0)
		file << "Matrix with no column" << endl;
	else
	{
		for(int i=0;i<rows;i++)
		{
			file << "( ";
			for(int j=0;j<cols;j++)
				file << values[i][j] << ' ';
			file << ')' << endl;
		}
	}
}


void Matrix::addRow(int i)
{
	if(i<=0)
		throw Exception((char*)"i<=0",(char*)"void Matrix::addRow(int i)");
	else if(i>rows+1)
		throw Exception((char*)"i>rows+1",(char*)"void Matrix::addRow(int i)");

	i--;
	values.insert(values.begin()+i,vector<float>(cols,default_value));
	rows++;
}

void Matrix::removeRow(int i)
{
	if(i<=0)
		throw Exception((char*)"i<=0",(char*)"void Matrix::addRow(int i)");
	else if(i>rows)
		throw Exception((char*)"i>rows",(char*)"void Matrix::addRow(int i)");

	i--;
	values.erase(values.begin()+i);
	rows--;
}

void Matrix::addColumn(int i)
{
	if(i<=0)
		throw Exception((char*)"i<=0",(char*)"void Matrix::addColumn(int i)");
	else if(i>cols+1)
		throw Exception((char*)"i>cols+1",(char*)"void Matrix::addColumn(int i)");

	i--;
	for(int j=0; j<rows; j++)
		values[j].insert(values[j].begin()+i,default_value);
	cols++;
}

void Matrix::removeColumn(int i)
{
	if(i<=0)
		throw Exception((char*)"i<=0",(char*)"void Matrix::removeColumn(int i)");
	else if(i>cols)
		throw Exception((char*)"i>cols",(char*)"void Matrix::removeColumn(int i)");

	i--;
	for(int j=0; j<rows; j++)
		values[j].erase(values[j].begin()+i);
	cols--;
}

float& Matrix::operator()(int i, int j)
{
	if(i<=0)
		throw Exception((char*)"i<=0",(char*)"float& Matrix::operator()(int i, int j)");
	else if(i>rows)
		throw Exception((char*)"i>rows",(char*)"float& Matrix::operator()(int i, int j) const");
	if(j<=0)
		throw Exception((char*)"j<=0",(char*)"float& Matrix::operator()(int i, int j)");
	else if(j>cols)
		throw Exception((char*)"j>cols",(char*)"float& Matrix::operator()(int i, int j)");

	return values[i-1][j-1];
}

const float& Matrix::operator()(int i, int j) const
{
	if(i<=0)
		throw Exception((char*)"i<=0",(char*)"float& Matrix::operator()(int i, int j)");
	else if(i>rows)
		throw Exception((char*)"i>rows",(char*)"float& Matrix::operator()(int i, int j) const");
	if(j<=0)
		throw Exception((char*)"j<=0",(char*)"float& Matrix::operator()(int i, int j)");
	else if(j>cols)
		throw Exception((char*)"j>cols",(char*)"float& Matrix::operator()(int i, int j)");

	return values[i-1][j-1];
}

void Matrix::fill(const float val)
{
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols;j++)
			values[i][j] = val;
}

//~ int main(int argc, char** argv)
//~ {
	//~ try
	//~ {
		//~ Matrix m(2,3,0);
		//~
		//~ m.print();
		//~ cout << endl;
		//~
		//~ m.fill(1);
		//~ m.print();
		//~ cout << endl;
//~
		//~ m.addColumn(4);
		//~ m.print();
		//~ cout << endl;
		//~
		//~ m.removeColumn(1);
		//~ m.print();
		//~ cout << endl;
	//~ }
	//~ catch(const Exception& e)
	//~ {
		//~ cout << "error: in " << e.what() << endl;
	//~ }
	//~
	//~ return 0;
//~ }
