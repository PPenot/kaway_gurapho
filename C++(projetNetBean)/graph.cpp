#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include "graph.h"
#include "exception.h"
#include "fwrite.h"
#include "tour.h"

using namespace std;

int _rand(int min, int max)
{
	return (int) (min + ((float) rand() / RAND_MAX * (max - min + 1)));
}

ostream &operator<<(ostream &out, const Graph g)
{
	g.print(out);
	return out;
}

Graph::Graph(bool oriented, bool weighted, float default_value)
{
	this->oriented = oriented;
	this->weighted = weighted;
	try
	{
		nodes = Matrix(2,0,default_value);

		if(weighted)
			edges = Matrix(0,0,default_value);
		else
			edges = Matrix(0,0,0);
	}
	catch(const Exception& e)
	{
		cerr << "\nerror: " << e.what() << endl;
	}

}

int Graph::searchColById(int i) const
{
	if(i<0)
		throw Exception("negative id i","int Graph::searchColById(int i) const");

	int col=1;
	int order = nodes.getCols();
	while(col <= order && nodes(1,col) < i)
		col++;

        
	return col;
}

void Graph::addNode(int i)
{
	if(i<0)
		throw Exception("negative id","void Graph::addNode(int i)");

	try
	{
		int pos = searchColById(i);

		//si le noeud noeud existe
		if(i < nodes.getCols()+1 && i == nodes(1,pos))
			throw Exception("already existing node","void Graph::addNode(int i)");

		nodes.addColumn(pos);
		nodes(1,pos) = i;
                nodes(2,pos) = NAN;
		edges.addRow(pos);
		edges.addColumn(pos);
	}
	catch(const Exception& e)
	{
		cerr << "error: " << e.what() << endl;
	}
}

void Graph::removeNode(int i)
{
	if(i<0)
		throw Exception("negative id","void Graph::removeNode(int i)");

	try
	{
		int pos = searchColById(i);

		//le noeud n'existe pas
		if(nodes(1,pos) != i)
			throw Exception("the node does not exist","void Graph::removeNode(int i)");

		nodes.removeColumn(pos);
		edges.removeRow(pos);
		edges.removeColumn(pos);
	}
	catch(const Exception& e)
	{
		cerr << "\nerror: " << e.what() << endl;
	}
}

void Graph::addEdge(int i, int j)
{
	if(i<0)
		throw Exception("negative id i","void Graph::addEdge(int i, int j)");
	if(j<0)
		throw Exception("negative id j","void Graph::addEdge(int i, int j)");

	try
	{
		int posi = searchColById(i);
		int posj = searchColById(j);

		if(nodes(1,posi) != i)
			throw Exception("the node i does not exist","void Graph::addEdge(int i, int j)");
		if(nodes(1,posj) != j)
			throw Exception("the node j does not exist","void Graph::addEdge(int i, int j)");
		if(edges(posi,posj)!=edges.getDefaultValue())
			throw Exception("the edge already exist","void Graph::addEdge(int i, int j)");

		edges(posi,posj) = 1;

		if(!oriented)
			edges(posj,posi) = 1;
	}
	catch(const Exception& e)
	{
		//cerr << "\nerror: " << e.what() << endl;
	}
}

void Graph::removeEdge(int i, int j)
{
	if(i<0)
		throw Exception("negative id i","void Graph::removeEdge(int i, int j)");
	if(j<0)
		throw Exception("negative id j","void Graph::removeEdge(int i, int j)");

	try
	{
		int posi = searchColById(i);
		int posj = searchColById(j);


		if(nodes(1,posi) != i)
			throw Exception("the node i does not exist","void Graph::removeEdge(int i, int j)");
		if(nodes(1,posj) != j)
			throw Exception("the node j does not exist","void Graph::removeEdge(int i, int j)");

		float default_value = edges.getDefaultValue();

		edges(posi,posj) = default_value;

		if(!oriented)
			edges(posj,posi) = default_value;
	}
	catch(const Exception& e)
	{
		cerr << "\nerror: " << e.what() << endl;
	}
}

float Graph::getNodeValue(int i) const
{
	if(i<0)
		throw Exception("negative id i","float Graph::getNodeValue(int i) const");

	try
	{
		int posi = searchColById(i);


		if(nodes(1,posi) != i)
			throw Exception("the node i does not exist","float Graph::getNodeValue(int i) const");

		return nodes(2,posi);
	}
	catch(const Exception& e)
	{
		cerr << "\nerror: " << e.what() << endl;
	}

}

void Graph::setNodeValue(int i, float v)
{
	if(i<0)
		throw Exception("negative id i","void Graph::setNodeValue(int i, float v)");

	try
	{
		int posi = searchColById(i);

		if(nodes(1,posi) != i)
			throw Exception("the node i does not exist","void Graph::setNodeValue(int i, float v)");

		nodes(2,posi) = v;
	}
	catch(const Exception& e)
	{
		cerr << "\nerror: " << e.what() << endl;
	}
}

float Graph::getEdgeValue(int i, int j) const
{
	if(i<0)
		throw Exception("negative id i","float Graph::getEdgeValue(int i, int j) const");
	if(j<0)
		throw Exception("negative id j","float Graph::getEdgeValue(int i, int j) const");

	try
	{
		int posi = searchColById(i);
		int posj = searchColById(j);

		if(nodes(1,posi) != i)
			throw Exception("the node i does not exist","float Graph::getEdgeValue(int i, int j) const");
		if(nodes(1,posj) != j)
			throw Exception("the node j does not exist","float Graph::getEdgeValue(int i, int j) const");

		return edges(posi,posj);
	}
	catch(const Exception& e)
	{
		cerr << "\nerror: " << e.what() << endl;
	}
}

void Graph::setEdgeValue(int i, int j, float v)
{
	if(i<0)
		throw Exception("negative id i","void Graph::setEdgeValue(int i, int j, float v)");
	if(j<0)
		throw Exception("negative id j","void Graph::setEdgeValue(int i, int j, float v)");

	try
	{
		int posi = searchColById(i);
		int posj = searchColById(j);

		if(nodes(1,posi) != i)
			throw Exception("the node i does not exist","void Graph::setEdgeValue(int i, int j, float v)");
		if(nodes(1,posj) != j)
			throw Exception("the node j does not exist","void Graph::setEdgeValue(int i, int j, float v)");

		edges(posi,posj) = v;

		if(!oriented)
			edges(posj,posi) = v;
	}
	catch(const Exception& e)
	{
		cerr << "\nerror: " << e.what() << endl;
	}
}

int Graph::getNeighborhoodSize(int i) const
{
	if(i<0)
		throw Exception("negative id i","int Graph::getNeighborhoodSize(int i) const");

	try
	{
		int size = 0;
		int posi = searchColById(i);

		if(nodes(1,posi) != i)
			throw Exception("the node i does not exist","int Graph::getNeighborhoodSize(int i) const");

		int cols = edges.getCols();
		float d_v = edges.getDefaultValue();

		for(int c=1;c <= cols; c++)
			if(edges(posi,c) != d_v)
				size++;

		return size;
	}
	catch(const Exception& e)
	{
		cerr << "\nerror: " << e.what() << endl;
	}
}

int Graph::getNeighbor(int i, int k) const
{
	if(i<0)
		throw Exception("negative id i","int getNeighbor(int i, int k) const");
	if(k<=0)
		throw Exception((char*)"k is negative of null","int getNeighbor(int i, int k) const");

	try
	{
		int posi = searchColById(i);

		if(nodes(1,posi) != i)
			throw Exception("the node i does not exist","int getNeighbor(int i, int k) const");

		int cols = edges.getCols();
		int j=0,c=1;
		float d_v = edges.getDefaultValue();

		while(c<=cols && j<k)
		{
			if(edges(posi,c) != d_v)
				j++;

			c++;
		}

		if(j!=k)
			throw Exception("neighbor not found","int getNeighbor(int i, int k) const");

		return (int)nodes(1,c-1);
	}
	catch(const Exception& e)
	{
		cerr << "\nerror: " << e.what() << endl;
	}
}

void Graph::connectAll()
{
	int order = getOrder();
	float d_v = edges.getDefaultValue();

	for(int i=1; i<=order; i++)
	{
		for(int j=1; j<=order; j++)
		{
			try
			{
				if(edges(i,j)==d_v)
					addEdge((int)nodes(1,i),(int)nodes(1,j));
			}
			catch(const Exception& e)
			{
				cout << endl << "error: " << endl;
			}
		}
	}
}

void Graph::print(ostream& out) const
{
	try
	{
		int order = getOrder();
		int nbr, node;
		float d_v = edges.getDefaultValue();

		out << "oriented: ";
		if(oriented)
			out << "true";
		else
			out << "false";

		 out << endl;

		 out << "weighted: ";
		 if(weighted)
			out << "true";
		else
			out << "false";

		out << endl << "order: " << order << endl;
		out << "default value: " << nodes.getDefaultValue() << endl << endl;
		for(int i=1; i<= order; i++)
		{
			node = (int)nodes(1,i);
			out << node << '(' << nodes(2,i) << ')' <<": ";

			for(int j=1; j<=order; j++)
				if(edges(i,j)!=d_v)
					out << nodes(1,j) << '(' << edges(i,j) << ") ";
			out << endl;
		}
	}
	catch(const Exception& e)
	{
		cerr << "\nerror: " << e.what() << endl;
	}
}

void Graph::mprint(ostream& out) const
{
	if(getOrder()==0)
		out << "Empty graph" << endl;
	else
	{
		out << endl;
		out << "nodes:" << endl;
		nodes.print();
		out << "edges:" <<endl;
		edges.print();
	}
}

bool Graph::isEdge(int i, int j) const
{
	if(i<0)
		throw Exception("negative id i","float Graph::getEdgeValue(int i, int j) const");
	if(j<0)
		throw Exception("negative id j","float Graph::getEdgeValue(int i, int j) const");


}


void testGraph()
{
	//~ ofstream file("testGraph.txt",ios::out | ios::trunc);
//~
	//~ {
		//~ file << title1("constructeurs");
		//~ file << endl;
		//~
		//~ {
			//~ file << title2("Constructeur par défaut") << endl;
			//~
			//~ file << title2("Code");
			//~ file << "Graph g;" << endl;
			//~ file << "g.fprint(file);" << endl;
			//~ file << "g.finfo(file);" << endl << endl;
			//~
			//~ Graph g;
			//~
			//~ file << title2("Result") << endl;
			//~ file << title3("fprint():");
			//~ g.fprint(file);
//~
			//~ file << endl;
			//~ file << title3("finfo():");
			//~ g.finfo(file);
			//~ file << endl;
		//~ }
		//~ {
			//~ file << title2("Constructeur classique") << endl;
			//~
			//~ file << title2("Code");
			//~ file << "Graph g(true,false,0);" << endl;
			//~ file << "g.fprint(file);" << endl;
			//~ file << "g.finfo(file);" << endl;
			//~
			//~ Graph g(true,false,0);
			//~
			//~ file << endl;
			//~ file << title2("Result") << endl;
			//~ file << title3("fprint():");
			//~ g.fprint(file);
			//~ file << endl;
			//~
			//~ file << title3("finfo():");
			//~ g.finfo(file);
			//~ file << endl;
		//~ }
		//~ {
			//~ file << title1("add/remove node");
			//~ file << endl;
//~
			//~ file << title2("Code");
			//~ file << "Graph g(true,false,0);" << endl;
			//~ file << "g.addNode(3);"<< endl;
			//~ file << "g.addNode(2);"<< endl;
			//~ file << "g.addNode(0);"<< endl;
			//~ file << "g.addNode(7);"<< endl;
			//~ file << "g.fprint(file);"<< endl;
			//~ file << endl;
			//~ file << "g.removeNode(2);" << endl;
			//~ file << "g.removeNode(0);" << endl;
			//~ file << "g.removeNode(7);" << endl;
			//~ file << endl;
			//~
			//~ Graph g(true,false,0);
			//~
			//~ g.addNode(3);
			//~ g.addNode(2);
			//~ g.addNode(0);
			//~ g.addNode(7);
//~
			//~ file << title2("Result") << endl;
			//~ file << title3("fprint():");
			//~ g.fprint(file);
			//~ file << endl;
			//~
			//~ file << title3("finfo():");
			//~ g.finfo(file);
			//~ file << endl;
			//~
			//~ g.removeNode(2);
			//~ file << title3("g.removeNode(2):");
			//~ g.fprint(file);
			//~ file << endl;
			//~
			//~ g.removeNode(0);
			//~ file << title3("g.removeNode(0):");
			//~ g.fprint(file);
			//~ file << endl;
//~
			//~ g.removeNode(7);
			//~ file << title3("g.removeNode(7):");
			//~ g.fprint(file);
			//~ file << endl;
//~
			//~ file << title3("finfo():");
			//~ g.finfo(file);
			//~ file << endl;
//~
		//~ }
		//~ {
			//~ file << title1("add/remove edge");
			//~ file << endl;
			//~
			//~ {
				//~ file << title2("Oriented graph");
				//~ file << "--------------" << endl;
				//~ file << "Oriented graph" << endl;
				//~ file << "--------------" << endl;
				//~ file << endl;
				//~
				//~ file << "[code]" << endl;
				//~ file << "| Graph g(true,false,0);" << endl;
				//~ file << "| g.addNode(3);"<< endl;
				//~ file << "| g.addNode(2);"<< endl;
				//~ file << "| g.addNode(0);"<< endl;
				//~ file << "| g.addNode(7);"<< endl;
				//~ file << "| ;"<< endl;
				//~ file << "| g.addEdge(0,3);" << endl;
				//~ file << "| g.addEdge(0,0);" << endl;
				//~ file << "| g.addEdge(2,7);" << endl;
				//~ file << "| g.addEdge(7,7);" << endl;
				//~ file << "| " << endl;
				//~ file << "| g.removeEdge(0,3);" << endl;
				//~ file << "| g.removeEdge(0,0);" << endl;
				//~ file << "| g.removeEdge(2,7);" << endl;
				//~ file << "| g.removeEdge(7,7);" << endl;
				//~ file << endl;
				//~
				//~ Graph g(true,false,0);
				//~
				//~ file << "[result:]" << endl;
//~
				//~ g.addNode(3);
				//~ g.addNode(2);
				//~ g.addNode(0);
				//~ g.addNode(7);
				//~ file << "fprint():" << endl;
				//~ file << "---------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.addEdge(0,3);
				//~ file << "g.addEdge(0,3):" << endl;
				//~ file << "---------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.addEdge(0,0);
				//~ file << "g.addEdge(0,0):" << endl;
				//~ file << "---------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.addEdge(2,7);
				//~ file << "g.addEdge(2,7):" << endl;
				//~ file << "---------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.addEdge(7,7);
				//~ file << "g.addEdge(7,7):" << endl;
				//~ file << "---------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.addEdge(2,3);
				//~ file << "g.addEdge(2,3):" << endl;
				//~ file << "---------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.removeEdge(0,3);
				//~ file << "g.removeEdge(0,3):" << endl;
				//~ file << "------------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.removeEdge(0,0);
				//~ file << "g.removeEdge(0,0):" << endl;
				//~ file << "------------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.removeEdge(2,7);
				//~ file << "g.removeEdge(2,7):" << endl;
				//~ file << "------------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.removeEdge(7,7);
				//~ file << "g.removeEdge(7,7):" << endl;
				//~ file << "------------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.removeEdge(2,3);
				//~ file << "g.removeEdge(2,3):" << endl;
				//~ file << "------------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
//~
			//~ }
			//~ {
				//~ file << "------------------" << endl;
				//~ file << "Non-oriented graph" << endl;
				//~ file << "------------------" << endl;
				//~ file << endl;
				//~
				//~ file << "[code]" << endl;
				//~ file << "| Graph g(false,false,0);" << endl;
				//~ file << "| g.addNode(3);"<< endl;
				//~ file << "| g.addNode(2);"<< endl;
				//~ file << "| g.addNode(0);"<< endl;
				//~ file << "| g.addNode(7);"<< endl;
				//~ file << "|"<< endl;
				//~ file << "| g.addEdge(0,3);" << endl;
				//~ file << "| g.addEdge(0,0);" << endl;
				//~ file << "| g.addEdge(2,7);" << endl;
				//~ file << "| g.addEdge(7,7);" << endl;
				//~ file << "| g.removeEdge(0,3);" << endl;
				//~ file << "| g.removeEdge(0,0);" << endl;
				//~ file << "| g.removeEdge(2,7);" << endl;
				//~ file << "| g.removeEdge(7,7);" << endl;
				//~ file << endl;
				//~
				//~ Graph g(false,false,0);
				//~
				//~ file << "[result:]" << endl;
//~
				//~ g.addNode(3);
				//~ g.addNode(2);
				//~ g.addNode(0);
				//~ g.addNode(7);
				//~ file << "fprint():" << endl;
				//~ file << "---------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.addEdge(0,3);
				//~ file << "g.addEdge(0,3):" << endl;
				//~ file << "---------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.addEdge(0,0);
				//~ file << "g.addEdge(0,0):" << endl;
				//~ file << "---------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.addEdge(2,7);
				//~ file << "g.addEdge(2,7):" << endl;
				//~ file << "---------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.addEdge(7,7);
				//~ file << "g.addEdge(7,7):" << endl;
				//~ file << "---------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.addEdge(2,3);
				//~ file << "g.addEdge(2,3):" << endl;
				//~ file << "---------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.removeEdge(0,3);
				//~ file << "g.removeEdge(0,3):" << endl;
				//~ file << "------------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.removeEdge(0,0);
				//~ file << "g.removeEdge(0,0):" << endl;
				//~ file << "------------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.removeEdge(2,7);
				//~ file << "g.removeEdge(2,7):" << endl;
				//~ file << "------------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.removeEdge(7,7);
				//~ file << "g.removeEdge(7,7):" << endl;
				//~ file << "------------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
				//~ g.removeEdge(2,3);
				//~ file << "g.removeEdge(2,3):" << endl;
				//~ file << "------------------" << endl;
				//~ g.fprint(file);
				//~ file << endl;
				//~
			//~ }
		//~ }
		//~ {
			//~ file << "----------------------" << endl;
			//~ file << "| GET/SET NODE VALUE |" << endl;
			//~ file << "----------------------" << endl;
			//~ file << endl;
			//~ file << "[code]" << endl;
			//~ file << "| Graph g(false,false,0);" << endl;
			//~ file << "| g.addNode(3);"<< endl;
			//~ file << "| g.addNode(2);"<< endl;
			//~ file << "| g.addNode(0);"<< endl;
			//~ file << "| g.addNode(7);"<< endl;
			//~ file << "| "<< endl;
			//~ file << "| g.getNodeValue(2);" << endl;
			//~ file << "| g.setNodeValue(2,7);" << endl;
			//~ file << "| g.getNodeValue(0);" << endl;
			//~ file << "| g.setNodeValue(0,1);" << endl;
			//~ file << "| g.getNodeValue(7);" << endl;
			//~ file << "| g.setNodeValue(7,-1);" << endl;
			//~ file << endl;
			//~
			//~ Graph g(false,false,0);
			//~
			//~ file << "[result:]" << endl;
//~
			//~ g.addNode(3);
			//~ g.addNode(2);
			//~ g.addNode(0);
			//~ g.addNode(7);
			//~ file << "fprint():" << endl;
			//~ file << "---------" << endl;
			//~ g.fprint(file);
			//~
			//~ file << "g.getNodeValue(2):" << endl;
			//~ file << "------------------" << endl;
			//~ file << g.getNodeValue(2) << endl;
			//~ file << endl;
//~
			//~ file << "g.setNodeValue(2,7):" << endl;
			//~ file << "------------------" << endl;
			//~ g.setNodeValue(2,7);
			//~ g.fprint(file);
			//~ file << endl;
//~
			//~ file << "g.getNodeValue(0):" << endl;
			//~ file << "------------------" << endl;
			//~ file << g.getNodeValue(0) << endl;
			//~ file << endl;
//~
			//~ file << "g.setNodeValue(0,1):" << endl;
			//~ file << "------------------" << endl;
			//~ g.setNodeValue(0,1);
			//~ g.fprint(file);
			//~ file << endl;
			//~
			//~ file << "g.getNodeValue(7):" << endl;
			//~ file << "------------------" << endl;
			//~ file << g.getNodeValue(7) << endl;
			//~ file << endl;
//~
			//~ file << "g.setNodeValue(7,-1):" << endl;
			//~ file << "------------------" << endl;
			//~ g.setNodeValue(7,-1);
			//~ g.fprint(file);
			//~ file << endl;
		//~ }
		//~ {
			//~ file << "----------------------" << endl;
			//~ file << "| GET/SET EDGE VALUE |" << endl;
			//~ file << "----------------------" << endl;
			//~ file << endl;
			//~ file << "[code]" << endl;
			//~ file << "| Graph g(false,false,0);" << endl;
			//~ file << "| g.addNode(3);"<< endl;
			//~ file << "| g.addNode(2);"<< endl;
			//~ file << "| g.addNode(0);"<< endl;
			//~ file << "| g.addNode(7);"<< endl;
			//~ file << "| g.addEdge(0,3);" << endl;
			//~ file << "| g.addEdge(0,0);" << endl;
			//~ file << "| g.addEdge(2,7);" << endl;
			//~ file << "| g.addEdge(7,7);" << endl;
			//~ file << "| "<< endl;
			//~ file << "| g.getEdgeValue(2);" << endl;
			//~ file << "| g.setEdgeValue(2,7);" << endl;
			//~ file << "| g.getEdgeValue(0);" << endl;
			//~ file << "| g.setEdgeValue(0,1);" << endl;
			//~ file << "| g.getEdgeValue(7);" << endl;
			//~ file << "| g.setEdgeValue(7,-1);" << endl;
			//~ file << endl;
			//~
			//~ Graph g(false,false,0);
			//~
			//~ file << "[result:]" << endl;
//~
			//~ g.addNode(3);
			//~ g.addNode(2);
			//~ g.addNode(0);
			//~ g.addNode(7);
			//~
			//~ g.addEdge(0,3);
			//~ g.addEdge(0,0);
			//~ g.addEdge(2,7);
			//~ g.addEdge(7,7);
			//~ g.addEdge(2,3);
//~
			//~ file << "fprint():" << endl;
			//~ file << "---------" << endl;
			//~ g.fprint(file);
			//~
			//~ file << "g.getEdgeValue(0,3):" << endl;
			//~ file << "------------------" << endl;
			//~ file << g.getEdgeValue(0,3) << endl;
			//~ file << endl;
//~
			//~ file << "g.setEdgeValue(0,3,9):" << endl;
			//~ file << "------------------" << endl;
			//~ g.setEdgeValue(0,3,9);
			//~ g.fprint(file);
			//~ file << endl;
//~
			//~ file << "g.getEdgeValue(2,7):" << endl;
			//~ file << "------------------" << endl;
			//~ file << g.getEdgeValue(2,7) << endl;
			//~ file << endl;
//~
			//~ file << "g.setEdgeValue(2,7,5):" << endl;
			//~ file << "------------------" << endl;
			//~ g.setEdgeValue(2,7,5);
			//~ g.fprint(file);
			//~ file << endl;
			//~
		//~ }
		//~ {
			//~ file << "-------------------------" << endl;
			//~ file << "| GET NEIGHBORHOOD SIZE |" << endl;
			//~ file << "-------------------------" << endl;
			//~ file << endl;
			//~ file << "[code]" << endl;
			//~ file << "| Graph g(true,false,0);" << endl;
			//~ file << "| g.addNode(3);"<< endl;
			//~ file << "| g.addNode(2);"<< endl;
			//~ file << "| g.addNode(0);"<< endl;
			//~ file << "| g.addNode(7);"<< endl;
			//~ file << "| g.addEdge(0,3);" << endl;
			//~ file << "| g.addEdge(0,0);" << endl;
			//~ file << "| g.addEdge(2,7);" << endl;
			//~ file << "| g.addEdge(7,7);" << endl;
			//~ file << "| "<< endl;
			//~ file << "| g.getNeighborhoodSize(0);" << endl;
			//~ file << "| g.getNeighborhoodSize(3);" << endl;
			//~ file << endl;
//~
			//~ Graph g(true,false,0);
			//~
			//~ file << "[result:]" << endl;
//~
			//~ g.addNode(3);
			//~ g.addNode(2);
			//~ g.addNode(0);
			//~ g.addNode(7);
			//~
			//~ g.addEdge(0,3);
			//~ g.addEdge(0,0);
			//~ g.addEdge(2,7);
			//~ g.addEdge(7,7);
			//~ g.addEdge(2,3);
//~
			//~ file << "fprint():" << endl;
			//~ file << "---------" << endl;
			//~ g.fprint(file);
			//~ file << endl;
//~
			//~ file << "g.getNeighborhoodSize(0):" << endl;
			//~ file << "-------------------------" << endl;
			//~ file << g.getNeighborhoodSize(0) << endl;
			//~ file << endl;
			//~
			//~ file << "g.getNeighborhoodSize(3):" << endl;
			//~ file << "-------------------------" << endl;
			//~ file << g.getNeighborhoodSize(3) << endl;
			//~ file << endl;
//~
		//~ }
		//~ {
			//~ file << "----------------" << endl;
			//~ file << "| GET NEIGHBOR |" << endl;
			//~ file << "----------------" << endl;
			//~ file << endl;
			//~ file << "[code]" << endl;
			//~ file << "| Graph g(true,false,0);" << endl;
			//~ file << "| g.addNode(3);"<< endl;
			//~ file << "| g.addNode(2);"<< endl;
			//~ file << "| g.addNode(0);"<< endl;
			//~ file << "| g.addNode(7);"<< endl;
			//~ file << "| g.addEdge(0,3);" << endl;
			//~ file << "| g.addEdge(0,0);" << endl;
			//~ file << "| g.addEdge(2,7);" << endl;
			//~ file << "| g.addEdge(7,7);" << endl;
			//~ file << "| "<< endl;
			//~ file << endl;
//~
			//~ Graph g(true,false,0);
			//~
			//~ file << "[result:]" << endl;
//~
			//~ g.addNode(3);
			//~ g.addNode(2);
			//~ g.addNode(0);
			//~ g.addNode(7);
			//~
			//~ g.addEdge(0,3);
			//~ g.addEdge(0,0);
			//~ g.addEdge(2,7);
			//~ g.addEdge(7,7);
			//~ g.addEdge(2,3);
//~
			//~ file << "fprint():" << endl;
			//~ file << "---------" << endl;
			//~ g.fprint(file);
			//~ file << endl;
//~
			//~ file << "g.getNeighbor(0,2):" << endl;
			//~ file << "-------------------------" << endl;
			//~ file << g.getNeighbor(0,2) << endl;
			//~ file << endl;
			//~
			//~ file << "g.getNeighbor(7,1):" << endl;
			//~ file << "-------------------------" << endl;
			//~ file << g.getNeighbor(7,1) << endl;
			//~ file << endl;
//~
		//~ }
		//~
	//~ }
	//~ cout << "done" << endl;
	//~ file.close();
}


/*****************************************************************************/
/*                             ALGORITHMES                                   */
/*****************************************************************************/


  /****************/
 /* WELSH-POWELL */
/****************/
//~ m: (matrice des noeuds non colorés ordonnés par degré)
//~ sommet | couleur | degré
//~ -------+---------+------
//~  s1    |  c1     | d1
//~  ...   | ...     | ...
//~  sn    | cn      | dn

//crée la matrice représentant les noeuds avec leurs couleurs triés par ordre
//décroissant de degré, et affecte la valeur 0 (absence de couleur) aux noeuds
//du graphe
//La colonne degré est supprimée à la fin de la fonction
void Graph::sortNodesByDegreeAndInitColor(Matrix& m)
{
	try
	{
		int order = getOrder();
		int deg = 0;

		m.addColumn(3);

		m(1,3) = (int)getNeighborhoodSize(nodes(1,1));
		m(1,1) = (int)nodes(1,1);
		nodes(2,1) = 0;

		for(int i=2; i<=order; i++)
		{
			deg = getNeighborhoodSize((int)nodes(1,i));

			int k = 1;
			while(k<i && deg <= m(k,3))
				k++;

			m.addRow(k);
			nodes(2,i) = 0;
			m(k,1) = nodes(1,i);
			m(k,3) = deg;
		}

		m.removeColumn(3);
	}
	catch(const Exception& e)
	{
		cerr << "error: " << e.what() << endl;
		throw;
	}
}

//prend l'id du noeud à colorer et la couleur
bool Graph::canBeColored(int i, int color) const
{
	try
	{
		int nbr = getNeighborhoodSize(i);
		int coli = searchColById(i);
		bool result = ( nodes(2,coli)==0 ); //si le noeud n'est pas déjà coloré

		int j=1;
		while(result && j<=nbr)
		{
			result = (nodes(2,searchColById(getNeighbor(i,j))) != color); //si aucun de ses voisins n'est de la même couleur
			j++;
		}

		return result;
	}
	catch(const Exception& e)
	{
		cerr << "error:\nbool Graph::canBeColored(int i, int color) const : " << e.what() << endl << endl;
		throw;
	}
}

void Graph::welshPowell()
{
	try
	{
		if(getOrder() == 0)
			return;

		Matrix m(1,2,0);

		sortNodesByDegreeAndInitColor(m);

		int i=1;
		int j;
		int color = 0;
		int order = getOrder();

		while(i<=order)
		{
			color++;

			j=1;
			while(j<=order && m(j,2)!=0)
				j++;

			m(j,2) = color;
			nodes(2,searchColById(m(j,1))) = color;

			for(int k=j+1; k<=order; k++)
			{
				if(canBeColored(m(k,1),color))
				{
					m(k,2) = color;
					nodes(2,searchColById(m(k,1))) = color;
					i++;
				}
			}

			i++;
		}
	}
	catch(const Exception& e)
	{
		cerr << "error: " << e.what() << endl;
		throw;
	}
}

bool Graph::isWellColored() const
{
	int i=1;
	int order = getOrder();
	bool result = true;

	while(i<=order && result)
	{
		int idi = nodes(1,i);
		int nbr = getNeighborhoodSize(idi);
		int k=1;

		while(k<=nbr && result)
		{
			int idk = getNeighbor(idi,k);
			if(idi!=idk)
				result = (nodes(2,i) != nodes(2,searchColById(idk)));
			k++;
		}
		i++;
	}
	return result;
}

void testWelshPowell()
{
	ofstream file("testWelshPowel.txt",ios::out | ios::trunc);

	{
		file << title1("Algorithme de Welsh-Powel - Campagne de test") << endl;

		{
			file << "Les graphes sont affichés sous la forme :" << endl;
			file << "idDuNoeud1(valeurDuNoeud1): idVoisin1(poidsArc 1-Voisin1) ... idVoisinN(poidsArc 1-VoisinN)" << endl;
			file << '.' << endl << '.' << endl << '.' << endl;
			file << "idDuNoeudN(valeurDuNoeudN): idVoisinN(poidsArc N-Voisin1) ... idVoisinN(poidsArc N-VoisinN)" << endl << endl;
		}
		{
			file << title2("Cas normaux") << endl;

			{
				file << title3("Graphe vide :");

				Graph g;

				try
				{
					file << g;
				}
				catch(const Exception& e)
				{
					file << "erreur :" << e.what() << endl;
				}

				file << endl;
				file << title3("Résultat attendu :");
				file << "Aucun (aucune coloration)" << endl;

				file << endl;
				file << title3("Résultat obtenu :");

				try
				{
					g.welshPowell();
					file << g;
				}
				catch(const Exception& e)
				{
					file << "erreur :" << e.what() << endl;
				}
			}
			{
				file << title3("Graphe non orienté (de l'exemple) :");

				Graph g(false,false);

				try
				{

					for(int i=1;i<=7;i++)
						g.addNode(i);
					g.addEdge(1,2);
					g.addEdge(1,6);
					g.addEdge(1,7);
					g.addEdge(2,6);
					g.addEdge(2,3);
					g.addEdge(3,4);
					g.addEdge(3,5);
					g.addEdge(4,5);
					g.addEdge(5,6);
					g.addEdge(5,7);

					file << g;
				}
				catch(const Exception& e)
				{
					file << "erreur :" << e.what() << endl;
				}

				file << endl;
				file << title3("Résultat attendu :");
				file << "Coloration des noeuds 1 et 5 avec la couleur \'1\'" << endl;
				file << "Coloration des noeuds 2, 4 et 7 avec la couleur \'2\'" << endl;
				file << "Coloration des noeuds 3 et 6 avec la couleur \'3\'" << endl;

				file << endl;
				file << title3("Résultat obtenu :");

				try
				{
					g.welshPowell();
					file << g;
				}
				catch(const Exception& e)
				{
					file << "erreur :" << e.what() << endl;
				}
			}
			{
				file << endl;
				file << title3("Graphe non orienté avec plusieurs noeuds non reliés :");

				Graph g(false,false);

				try
				{
					for(int i=1; i<8; i++)
						g.addNode(i);

					g.addEdge(1,3);
					g.addEdge(1,4);
					g.addEdge(1,5);
					g.addEdge(1,6);
					g.addEdge(5,6);
					g.addEdge(5,3);

					file << g;
				}
				catch(const Exception& e)
				{
					file << "erreur :" << e.what() << endl;
				}

				file << endl;
				file << title3("Résultat attendu :");
				file << "Coloration des noeuds 1, 2, et 7 avec la couleur \'1\'" << endl;
				file << "Coloration des noeuds 4 et 5 avec la couleur \'2\'" << endl;
				file << "Coloration des noeuds 3 et 6 avec la couleur \'3\'" << endl;

				file << endl;
				file << title3("Résultat obtenu :");

				try
				{
					g.welshPowell();
					file << g;
				}
				catch(const Exception& e)
				{
					file << "erreur :" << e.what() << endl;
				}
			}
			{
				file << endl;
				file << title3("Graphe non orienté avec id non continus :");

				Graph g(false,false);

				try
				{
					g.addNode(1);
					g.addNode(2);
					g.addNode(5);
					g.addNode(9);
					g.addNode(10);
					g.addNode(17);

					g.addEdge(1,2);
					g.addEdge(1,9);
					g.addEdge(1,10);
					g.addEdge(2,5);
					g.addEdge(5,10);
					g.addEdge(9,17);
					g.addEdge(17,5);

					file << g;
				}
				catch(const Exception& e)
				{
					file << "erreur :" << e.what() << endl;
				}

				file << endl;
				file << title3("Résultat attendu :");
				file << "Coloration des noeuds 1 et 5 avec la couleur \'1\'" << endl;
				file << "Coloration des noeuds 2, 9, 10 avec la couleur \'2\'" << endl;
				file << "Coloration du noeud 17 avec la couleur \'3\'" << endl;

				file << endl;
				file << title3("Résultat obtenu :");

				try
				{
					g.welshPowell();
					file << g;
				}
				catch(const Exception& e)
				{
					file << "erreur :" << e.what() << endl;
				}
			}
			{
				file << endl;
				file << title3("Graphe orienté :");

				Graph g(true,false);

				try
				{
					for(int i=1;i<=7;i++)
						g.addNode(i);
					g.addEdge(1,2);
					g.addEdge(1,6);
					g.addEdge(1,7);
					g.addEdge(2,1);
					g.addEdge(4,3);
					g.addEdge(3,2);
					g.addEdge(2,6);
					g.addEdge(2,3);
					g.addEdge(3,4);
					g.addEdge(3,5);
					g.addEdge(4,5);
					g.addEdge(5,6);
					g.addEdge(6,5);
					g.addEdge(5,7);

					file << g;
				}
				catch(const Exception& e)
				{
					file << "erreur :" << e.what() << endl;
				}

				file << endl;
				file << title3("Résultat attendu :");
				file << "Coloration des noeuds 1, 3, 5 et 7 avec la couleur \'1\'" << endl;
				file << "Coloration des noeuds 2, 4 et 6 avec la couleur \'2\'" << endl;

				file << endl;
				file << title3("Résultat obtenu :");

				try
				{
					g.welshPowell();
					file << g;
				}
				catch(const Exception& e)
				{
					file << "erreur :" << e.what() << endl;
				}
			}
		}
		{
			file << endl;
			file << title2("Cas renvoyant des exceptions") << endl;
			file << "Aucun cas ne nécessite l'envoit d'une exception" << endl;
		}
	}

	file.close();
	cout << "done" << endl;
}


//crée un tableau d'entiers de taille end-start dont les valeurs sont comprises
//entre start et end, trié par ordre croissant
vector<int> createArray(int start, int end)
{
	int size = end-start;
	vector<int> array(size);
	for(int i=0; i<size; i++)
		array[i] = i+start;
	return array;
}

Graph createRandomGraph(unsigned int order)
{
	srand(time(NULL));

	Graph g;

	for(int i=1; i<=order; i++)
		g.addNode(i);

	for(int i=1; i<=order; i++)
	{
		for(int k=1; k<=order; k++)
		{
			if(_rand(0,10)==1)
			{
				try
				{
					g.addEdge(i,k);
                                        g.setEdgeValue(i,k,_rand(1,100));
				}
				catch(const Exception& e)
				{
				}
			}
		}

	}

	return g;
}





Tour Graph::little(int start)
{
    Tour shortest_tour;

    if(getOrder()<2)
        return shortest_tour;

    Matrix costs(edges);

    {
        int order = getOrder();

        for(int i=1;i<=order; i++)
        {
            costs(i,i) = HUGE_VAL;
        }

    }


    Matrix sources(1,costs.getCols());
    Matrix dests(1,costs.getCols());

    {
        int tmp = costs.getCols();
        for(int i=1; i<=tmp; i++)
        {
            sources(1,i) = nodes(1,i);
            dests(1,i) = nodes(1,i);
        }
    }

/*    cout << "costs:"<<endl;
    costs.print();
    cout << "sources:"<<endl;
    sources.print();
    cout << "dests:"<<endl;
    dests.print();
    cout << "---------------\n";
    dests.print();
*/
    shortest_tour = realLittle(sources, dests, costs, shortest_tour);
   // cout << shortest_tour.toString() <<endl;
    shortest_tour.sort(start);
    return shortest_tour;
}


Tour Graph::realLittle(Matrix sources, Matrix dests, Matrix costs, Tour tmp_tour)
{
    if(costs.getCols()==2)
    {
        //on cherche les cases de 0...
        int size = costs.getCols();
        for(unsigned int i=1;i<=size;i++)
        {
            for(unsigned int j=1;j<=size;j++)
            {
                if(costs(i,j)==0)
                {
                    tmp_tour.addPath(sources(1,i),dests(1,j)); //...et on les ajoute à la tournée si on en trouve
                }
            }
        }

        return tmp_tour;
    }

    Matrix saved_sources(sources);
    Matrix saved_dests(dests);
    Matrix saved_costs(costs);
    Tour saved_tour(tmp_tour);

    float additional_cost = 0.0;
    int size = sources.getCols();

    //REDUCTIONS
    {
        float tmp;

        //réduction des lignes de la matrice, et somme des minimums (en ligne) à ajouter au coût de la tournée
        for(int i=1; i<=size; i++)
        {
            tmp = costs.getMinRows(i);
            additional_cost += tmp;
            costs.subRows(i,tmp);
        }

        //réduction des colonnes de la matrice, et somme des minimums (en colonne) à ajouter au coût de la tournée
        for(int i=1; i<=size; i++)
        {
            tmp = costs.getMinCols(i);
            additional_cost += tmp;
            costs.subCols(i,tmp);
        }
    }

    //coordonnees de la case de regret maximal
    int r_row=1, r_col=1;

    //REGRETS
    {
        float regret = -1;
        for(unsigned int i=1;i<=size;i++)
        {
            for(unsigned int j=1;j<=size;j++)
            {
                if(costs(i,j)==0)
                {
                    costs(i,j)=HUGE_VAL;
                    if(costs.getMinRows(i) + costs.getMinCols(j) > regret)
                    {
                        r_row = i;
                        r_col = j;
                    }
                    costs(i,j) = 0;
                }
            }
        }

        if(regret==-1)
        {
            regret = 0;
        }
        //Mise à jour des paramètres sauvés correspondant au choix de ne PAS faire le chemin
        saved_tour.setCost(saved_tour.getCost()+regret+additional_cost);
        saved_costs(r_row,r_col) = HUGE_VAL;
    }

    //Exploration de l'arbre : on choisit de faire le chemin de i vers j (en réalité de sources(1,i) vers dests(1,j))
    {
        //costs.print();

        //cout << "-----------------" << endl;

        int* parasite = tmp_tour.addPath(sources(1,r_row),dests(1,r_col));

        int i=1;
        int size = sources.getCols();
        bool* boule = new bool[2];
        boule[0] = false;
        boule[1] = false;
        while(i<=size && !boule[0])
        {
            boule[0] = sources(1,i)==parasite[0];
            i++;
        }
        parasite[0]=i-1;

        i=1;
        while(i<=size && !boule[1])
        {
            boule[1] = dests(1,i)==parasite[1];
            i++;
        }
        parasite[1]=i-1;

        if(boule[0] && boule[1])
        {
            costs(parasite[0],parasite[1]) = HUGE_VAL;
        }
        delete[] boule;
        
        tmp_tour.setCost(additional_cost+tmp_tour.getCost());

        costs.removeColumn(r_col);
        costs.removeRow(r_row);
        sources.removeColumn(r_row);
        dests.removeColumn(r_col);

        //cout << "avant l'appel suivant:" << endl;
        //costs.print();
        tmp_tour = realLittle(sources,dests,costs,tmp_tour);

        delete[] parasite;
    }

    //Condition d'exploration des autres branches de l'arbre en vue d'optimiser la tournée
    if(tmp_tour.getCost() > saved_tour.getCost())
    {
        tmp_tour = realLittle(saved_sources,saved_dests,saved_costs,saved_tour);
    }

    //cout<<"tkt"<<endl;
    return tmp_tour;
}