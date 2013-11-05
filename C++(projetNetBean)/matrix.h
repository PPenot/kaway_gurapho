#ifndef HEADER_MATRIX
#define HEADER_MATRIX

#include <vector>
#include <iostream>
#include <cmath>

class Matrix
{
	private:
		std::vector<std::vector<float> > values;
		int rows;
		int cols;
		float default_value;

	public:
		Matrix(int rows=0,int cols=0,float d_v=NAN);
		Matrix(const Matrix& m);
		~Matrix(){};

		void addRow(int i);
		void removeRow(int i);

		void addColumn(int i);
		void removeColumn(int i);

		float& operator()(int i, int j);
		const float& operator()(int i, int j) const;

		//remplit la matrice avec val
		void fill(const float val);

		int getRows() const { return rows; };
		int getCols() const { return cols; };
		float getDefaultValue() const { return default_value; };

		//affiche la matrice
		void print() const;
		void fprint(std::ostream& file) const;

		//affiche les dimensions de la matrice au format lignes x colonnes et la valeur par défaut
		void info()  const { std::cout << rows << "x" << cols << " matrix, with default value " << default_value << std::endl; };

                float Matrix::getMinCols(unsigned int c);
                float Matrix::getMinRows(int r);
                void Matrix::subRows (int r,const float val);
                void Matrix::subCols (int c,const float val);

};

#endif
