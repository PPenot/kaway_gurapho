//~ ctx.measureText("    ") mesure la largeur en pixels
//~ ctx.textBaseline = "middle" dessine le caractère à partir du milieu et non d'un coin


  /**********************/
 /* GRAPH CONSTRUCTORS */
/**********************/

function Graph(oriented, weighted)
{
	if( (typeof(oriented) != typeof(true)) || (typeof(weighted) != typeof(true)) )
		throw new Exception('Graph (constructeur)','les paramètres doivent être des booléens');

	this.oriented = oriented;
	this.weighted = weighted;
	this.default_value = undefined;
	
	this.order = 0;
	this.firstNode = undefined;
	
	this.getOrder = function getOrder() { return this.order };
	
	this.addNode = addNode;
	this.removeNode = removeNode;
	
	this.addEdge = addEdge;
	this.removeEdge = removeEdge;
	
	this.getNodeValue = getNodeValue;
	this.setNodeValue = setNodeValue;
	
	this.getEdgeValue = getEdgeValue;
	this.setEdgeValue = setEdgeValue;
	
	this.getNeighborhoodSize = getNeighborhoodSize;
	
	this.getNeighbor = getNeighbor;
	
	this.searchNode = searchNode;
	this.searchEdge = searchEdge;
	
	this.print = print;
	
	this.clear = clear;
	
	this.getSaveFormatedString = getSaveFormatedString;
	
	function getSaveFormatedString()
	{
		var s = '';
		var node = this.firstNode;
		
		for(var i=0; i<this.order; i++)
		{
			s += node.getSaveFormatedString();

			node = node.next;
		}

		return s;
	}
}

  /*****************/
 /* GRAPH METHODS */
/*****************/

function clear()
{
	
}

function print()
{
	console.log('--------------');
	var s;
	var node = this.firstNode;
	var edge;
	while(node != undefined)
	{		
		s = node.id + '(' + node.value + ')' + ': '
		edge = node.firstEdge;
		while(edge != undefined)
		{
			s += edge.node.id + '(' + edge.value + ')' + ', ';
			edge = edge.next;
		}
		console.log(s);
		node = node.next;
	}
}


//prend l'id du noeud cherché en paramètre
//renvoit un tableau à 2 cases si le noeud existe :
//la première case est le prédécesseur, la deuxième le noeud
//Si le noeud n'existe pas, la deuxième case est le noeud suivant le plus proche de
//l'id donné en paramètre ou undefined
function searchNode(i)
{
	var node = new Array(2);

	if(i<0)
		throw new Exception('searchNode','l\'id ne peut pas être négatif');

	node[0] = undefined;
	node[1] = this.firstNode;

	while(node[1] != undefined && node[1].id<i)
	{
		node[0] = node[1];
		node[1] = node[1].next;
	}
		
	return node;
}


//prend l'id du nouveau noeud à rajouter
//tmp[0] est le prédécesseur du nouveau noeud,
//tmp[1] est le noeud suivant
function addNode(i,x,y)
{
	try
	{
		var tmp = this.searchNode(i);

		if(tmp!=undefined && tmp[1]!=undefined && tmp[1].id==i)
			throw new Exception('addNode','le noeud d\'id ' + i + ' existe déjà');
		
		
		
		var newNode;
		
		newNode = new Node(i,x,y);

		if(tmp[0]==undefined)
		{
			if([1]!=undefined) //au moins un arc
			{
				newNode.next = tmp[1];
			}
			this.firstNode = newNode;
		}
		else
		{
			newNode.next = tmp[1];
			tmp[0].next = newNode;
		}

		this.order++;
		delete tmp;
	}
	catch(e)
	{
		console.error(e.what());
	}
}

function removeNode(i)
{
	try
	{
		var nodes = this.searchNode(i);
		
		if(nodes[1] == undefined)
			throw new Exception('removeNode','le noeud d\'id ' + i + ' n\'existe pas');
		
		var node = this.firstNode;
		while(node != undefined)
		{
			this.removeEdge(node.id,i);
			node = node.next;
		}
		
		delete node;

		
		if(nodes[0] != undefined)
			nodes[0].next = nodes[1].next;
		else
			this.firstNode = this.firstNode.next;
			
		var tmp;
		while(nodes[1].firstEdge != undefined)
		{
			tmp = nodes[1].firstEdge.next;
			delete nodes[1].firstEdge;
			nodes[1].firstEdge = tmp;
		}
		delete nodes;
		
		this.order--;
	}
	catch(e)
	{
		console.error('Dans la fonction removeNode : ' +e.what());
	}
}

//recherche un arc entre les noeuds d'id i et j
//prend une référence sur un noeud i et un l'id (entier) de j
//renvoit un tableau de deux Edge
function searchEdge(nodei,j)
{
	try
	{
		var edge = new Array(2);
		edge[0] = undefined;
		edge[1] = nodei.firstEdge;

		while(edge[1] != undefined && edge[1].node.id < j)
		{
			edge[0] = edge[1];
			edge[1] = edge[1].next;
		}
		
		return edge;
	}
	catch(e)
	{
		console.error('Dans la fonction searchEdge : ' + e.what());
	}
}


function addEdge(i,j,weight)
{	
	try
	{
		var nodei = this.searchNode(i);
		if(nodei[1] == undefined && node[1].id != i)
			throw new Exception('addEdge','le noeud d\'id ' + i + ' n\'existe pas');

		var nodej = this.searchNode(j);
		if(nodej[1] == undefined && nodej[1].id != j)
			throw new Exception('addEdge','le noeud d\'id ' + j + ' n\'existe pas');

		var tmp = this.searchEdge(nodei[1],j);

		if(tmp[1] != undefined && tmp[1].node.id==j)
			throw new Exception('addEdge',"l'arc entre les noeuds " + i + " et " + j + " existe déjà");
		
		var newEdge = new Edge(nodej[1],nodei[1]);

		if(weight==undefined)
			newEdge.value = 1;
		else
			newEdge.value = weight;
		newEdge.next = tmp[1];
		
		if(tmp[0]!=undefined)
			tmp[0].next = newEdge;
		else
			nodei[1].firstEdge = newEdge;
			
		if(this.oriented === false)
		{
			tmp = this.searchEdge(nodej[1],i);
			
			if(tmp[1] == undefined)
			{
				var otherNewEdge = new Edge(nodei[1],nodej[1]);
				otherNewEdge.value = 1;
				otherNewEdge.next = tmp[1];
			
				if(tmp[0]!=undefined)
					tmp[0].next = otherNewEdge;
				else
					nodej[1].firstEdge = otherNewEdge;
			}
		}

		delete tmp;
	}
	catch(e)
	{
		console.error('Dans la fonction addEdge: ' + e.what());
	}
}


function removeEdge(i, j)
{
	var nodei = this.searchNode(i);
	if(nodei[1] == undefined && node[1].id != i)
	{
		alert("removeEdge: le noeud i n'existe pas");
		return;
	}

	var tmp = this.searchEdge(nodei[1],j);

	if(tmp[1] != undefined && tmp[1].node.id==j)
	{	
		if(tmp[0] != undefined)
			tmp[0].next = tmp[1].next;
		else
			nodei[1].firstEdge = tmp[1].next;
		delete tmp[1];
		
		if(this.oriented==false)
			this.removeEdge(j,i);
	}
}

function getNodeValue(i)
{
	var tab = this.searchNode(i)
	
	if(tab[1] == undefined)
		throw new Exception('getNodeValue','le noeud d\'id '+ i + ' n\'existe pas');
		
	return tab[1].value;
}

function setNodeValue(i,val)
{
	val = parseInt(val);
	
	if(val<=0)
		throw new Exception('setNodeValue','la valeur du noeud '+i+' doit être positive');

	var tab = this.searchNode(i)
	
	if(tab[1] == undefined)
		throw new Exception('setNodeValue','le noeud d\id ' + i + ' n\existe pas');
		
	tab[1].value = val;
}

function getEdgeValue(i, j)
{
	if(i<=0 || j<=0)
		throw new Exception('getEdgeValue','les id des noeuds sont toujours positifs');

	var node = this.searchNode(i);	
	
	var tab = this.searchEdge(this.searchNode(i)[1],j)

	if(tab[1] == undefined)
		throw new Exception('getEdgeValue','le noeud d\'id ' + i + ' n\'existe pas');
		
	return tab[1].value;
}

function setEdgeValue(i, j,val)
{
	val = parseFloat(val);
	
	if(i<=0 || j<=0)
		throw new Exception('setEdgeValue','les id des noeuds sont toujours positifs');

	if(val<0)
		throw new Exception('setEdgeValue','le poids d\'un arc doit être positif ou nul');
		
	var tab = this.searchEdge(this.searchNode(i)[1],j)
	
	if(tab[1] == undefined || tab[1].node.id != j)
		throw new Exception('setEdgeValue','le noeud d\'id ' + i + ' n\'existe pas');
		
	tab[1].value = val;
}


function getNeighborhoodSize(i)
{
	var size=0;
	var node;
	try
	{
		node = this.searchNode(i);

		if(node[1]== undefined || node[1].id!=i)
			throw new Exception("getNeigborhoodSize",'le noeud d\'id '+i+' n\'existe pas');
			
		var edge = node[1].firstEdge;
		while(edge!=undefined)
		{
			size++;
			edge=edge.next;
		}
		return size;
	}
	catch(e)
	{
		console.error('Dans la fonction getNeighborhoodSize: ' + e.what());
	}
}

function getNeighbor(i,k)
{
	var node = this.searchNode(i);
	
	try
	{
		if(node[1]== undefined || node[1].id!=i)
			throw new Exception("getNeigbor",'le noeud d\'id '+i+' n\'existe pas');
		
		var size = this.getNeighborhoodSize(i);

		if(k>size)
			throw new Exception("getNeighbor",'k est trop grand : k='+k+'>i='+i+')');
		
		var edge = node[1].firstEdge;
		for(var i=1;i<k;i++)
			edge = edge.next;
		
		return edge.node.id;
	}
	catch(e)
	{
		console.error(e.what());
	}
}
