#ifndef HEADER_GRAPH
#define HEADER_GRAPH
#include <iostream>
#include "matrix.h"
#include "tour.h"
using namespace std;
class Graph
{
	private:
		Matrix nodes;
		Matrix edges;
		bool oriented;
		bool weighted;
		int searchColById(int i) const;
		void sortNodesByDegreeAndInitColor(Matrix& m);
		bool canBeColored(int i, int color) const;
                Tour realLittle(Matrix sources, Matrix dests, Matrix costs, Tour tmp_tour);

	public:
		Graph(bool oriented=false, bool weighted=false, float default_value=HUGE_VAL);
		~Graph() {};

		int getOrder() const { return nodes.getCols();};
		float getDefaultValue() const { return nodes.getDefaultValue(); };

		void addNode(int i);
		void removeNode(int i);

		void addEdge(int i, int j);
		void removeEdge(int i, int j);

		float getNodeValue(int i) const;
		void setNodeValue(int i, float v);

		float getEdgeValue(int i, int j) const;
		void setEdgeValue(int i, int j, float v);

		int getNeighborhoodSize(int i) const;
		int getNeighbor(int i, int k) const;

		void print(std::ostream& out) const;
		void fprint(std::ostream& file) const;
		void mprint(std::ostream& out) const;
		void mfprint(std::ostream& file) const;
		void info(std::ostream& out) const;
		void finfo(std::ostream& file) const;

		void connectAll();

		void welshPowell();
		bool isWellColored() const;

		//renvoit true s'il existe un arc entre les noeuds d'id i et j
		bool isEdge(int i, int j) const;

                Tour little(int start);

                friend bool createGraph(Graph& g, vector<string>& args, vector<string>& values);
};

std::vector<int> createArray(int start, int end);
int _rand(int min, int max);
void testWelshPowell();
void testGraph();
Graph createRandomGraph(unsigned int max_order=100);

#endif
