var NODE_RADIUS = 20;
var BOULE = 0;

function Exception(fname,type)
{
	this.fname = fname;
	this.type = type;
	this.what = function() { return 'erreur lors de l\opération ' + fname + ': ' + type; };
}

//crée une équation de droite à partir de deux points
function Line(Xa,Ya,Xb,Yb)
{
	var x = Xb-Xa; //coordonnées du vecteur directeur
	var y = Yb-Ya; //
	
	this.a = y;
	this.b = -x;
	this.c = -( (y*Xa) + (-x)*Ya);
	
	this.getA = function getA() { return this.a; };
	this.getB = function getB() { return this.b; };
	this.getC = function getC() { return this.c; };

	this.setA = function setA(a) { this.a = a; };
	this.setB = function setB(b) { this.b = b; };
	this.setC = function setC(c) { this.c = c; };
	
	this.isOn = function isOn(x,y) { return (this.a*x + this.b*y + this.c) == 0; };
	
	this.d = function(x,y) { return Math.round((Math.abs(this.a*x + this.b*y + this.c))/(Math.sqrt(this.a*this.a + this.b*this.b))); } //retourne la distance du point (x,y) à la droite
}

//cercle de centre (a,b) et de rayon r
function Disk(a,b,r)
{
	this.radius = r;
	this.center = new Position(a,b);
	
	this.getRadius = function getRadius() { var radius = this.radius; return radius; };
	this.setRadius = function setRadius(r) { this.radius = r; };
	this.getCenter = function getCenter() { var center = this.center; return center; };
	this.setCenter = function setCenter(a,b) { this.center.setX(a); this.center.setY(b); };
	
	this.isIn = function isIn(x,y)
	{
		var X = x-this.center.getX();
		var Y = y-this.center.getY();
		return (X*X + Y*Y) <= r*r;
	}
}


function Node(id,x,y)
{
	if(id<0)
		throw new Exception('Node (constructeur)','l\'id d\'un noeud ne peut être négatif');
	
	this.inheritFromDisk = Disk;
	if(x==undefined || y==undefined)
		this.inheritFromDisk(GUI.cursor.getX(),GUI.cursor.getY(),NODE_RADIUS);
	else
		this.inheritFromDisk(x,y,NODE_RADIUS);
	
	this.value = undefined;
	this.next = undefined;
	
	this.id = id;
	this.firstEdge = undefined;
	
	this.getSaveFormatedString = getSaveFormatedString;
	
	function getSaveFormatedString()
	{
	//chaine += '&node'+this.id + '='
		var s = '&node'+this.id + '='
		if(this.value == undefined)
			s+='0';
		else
			s+=this.value;
		s += '&coordx' + this.id + '=' + this.center.getX()+ '&coordy' + this.id + '=' + this.center.getY() + '&';
			
		var edge = this.firstEdge;
		while(edge != undefined)
		{
			s += 'edge' + this.id + '-' + edge.node.id + '=' + edge.value + '&';
			edge = edge.next;
		}
		//console.log (s.substring(0,s.length-1));
		return s.substring(0,s.length-1);
	}
	
	
	this.isSelected = function()
	{
		var lastClicked = GUI.cursor.getLastClicked();
		var mode = GUI.getMode();
		
		if(mode=='addEdge')
		{
			if(GUI.selectedNode==0)
				return this.isIn(lastClicked.getX(),lastClicked.getY());
			else
			{
				return false;
			}
			
		}
		
		return this.isIn(lastClicked.getX(),lastClicked.getY());
	}
	
	this.isBrowsed = function()
	{
		return this.isIn(GUI.cursor.getX(),GUI.cursor.getY());
	}
	
	this.display = function()
	{
		var ctx = GUI.getContext();
		var mode = GUI.getMode();
		var isCursorOnANode = this.isBrowsed();
		var tmp = new Disk(this.center.getX(),this.center.getY(),2*this.radius);
		var isSelected = this.isSelected();

		if(isSelected == true)
		{
			GUI.selectedNode = this.id;
			BOULE++;
		}
		

		if(isCursorOnANode == true)
		{
			GUI.browsedNode[0] = this.id;
			GUI.browsedNode[1]=true;
		}
				
		var cx = GUI.cursor.getX();
		var cy = GUI.cursor.getY();
		var cnv = GUI.getCanvas();
		if(tmp.isIn(cx,cy) || (cx<this.radius || cx>cnv.width-this.radius || cy<this.radius || cy>cnv.height-this.radius))
			GUI.cursor.canANodeBeHere++;

		var ctx = GUI.getContext();
		ctx.save();
		if(  ((mode == "select" || mode == 'addEdge') && (isSelected == true || isCursorOnANode == true)) || ((mode == 'addNode' ) && isSelected == true))
		{
			ctx.lineWidth="3";
			GUI.selectedEdge = [0,0];
		}
		else
		{
			ctx.lineWidth="1";
		}
		
		ctx.beginPath();
		ctx.fillStyle="white";
		ctx.arc(this.center.getX(),this.center.getY(),this.radius,0,2*Math.PI,true);
		ctx.stroke();
		ctx.fill();
		
		ctx.closePath();

		ctx.lineWidth = "1";
		ctx.textBaseline = 'middle';
		ctx.strokeText(this.id,this.center.getX() - Math.round(ctx.measureText(this.id).width/2),this.center.getY());
		ctx.restore();
	}

}


//Prend en paramètre une référence sur le noeud voisin et la référence du noeud duquel l'arc part
function Edge(node,src)
{
	var xn2 = node.center.getX();
	var yn2 = node.center.getY();

	var xn1 = src.center.getX();
	var yn1 = src.center.getY();
	var center = mid(xn1,yn1,xn2,yn2);
	
	// this.inheritFromEllipse = Ellipse;
	// this.inheritFromEllipse(center.getX(),center.getY(),Math.round(d(xn1,yn1,xn2,yn2)/2),10);
	this.inheritFromLine = Line;
	this.inheritFromLine(xn1,yn1,xn2,yn2);
	
	this.value = 1;
	this.next = undefined;

	this.node = node;
	this.src = src;
	
	this.isSelected = function()
	{
		if(GUI.getMode()!='addEdge')
		{
			var lastClicked = GUI.cursor.getLastClicked();
			var x = lastClicked.getX();
			var y = lastClicked.getY();
			return this.d(x,y)<=10 && !this.src.isIn(x,y) && !this.node.isIn(x,y);
		}
	}
	
	this.isBrowsed = function(r)
	{
		var x = GUI.cursor.getX();
		var y = GUI.cursor.getY();
		var tmp = new Disk(center.getX(),center.getY(),parseInt(Math.sqrt(Math.pow(center.getX()-this.node.center.getX(),2)+Math.pow(center.getY()-this.node.center.getY(),2))));
		return this.d(x,y)<=10 && !this.src.isIn(x,y) && !this.node.isIn(x,y) && tmp.isIn(x,y);
	}
	
	this.display = function()
	{
		var ctx = GUI.getContext();
		var mode = GUI.getMode();
		var isCursorOnAEdge = this.isBrowsed();
		// var tmp = new Ellipse(this.center.getX(),this.center.getY(),this.getSemimajorAxis(),this.getSemiminorAxis());

		if(this.isSelected()==true)
		{
			GUI.selectedEdge = [this.src.id,this.node.id];
		}
		
		ctx.save();
		if( (mode == 'select' )  && (this.isSelected() == true || isCursorOnAEdge == true))
		{
			ctx.lineWidth = '2';
			GUI.selectedNode = 0;
		}
		else
		{
			ctx.lineWidth = '1';
		}
		
		ctx.beginPath();
		
		ctx.moveTo(this.src.center.getX(),this.src.center.getY());
		ctx.lineTo(this.node.center.getX(),this.node.center.getY());
		ctx.stroke();
		
		ctx.closePath();
		
		ctx.lineWidth = '1';
		var m = mid(this.src.center.getX()+5,this.src.center.getY()+5,this.node.center.getX()+5,5+this.node.center.getY());
		ctx.strokeText(this.value,m.getX(),m.getY());

		// ctx.textBaseline = 'middle';
		// ctx.strokeText(this.id,this.center.getX() - Math.round(ctx.measureText(this.id).width/2),this.center.getY());
		ctx.restore();
	}
}

//retourne la position du milieu d'un segment
function mid(x1,y1,x2,y2)
{
	return new Position( Math.round( (x1+x2)/2 ), Math.round( (y1+y2)/2 ) );
}

//retourne la distance entre 2 points
function d(x1,y1,x2,y2)
{
	return Math.round(Math.sqrt(Math.pow(x1-x2,2)+Math.pow(y1-y2,2)));
}

//position dans le canvas
function Position(x,y)
{
	this.x = x;
	this.y = y;

	this.getX = function getX() { return this.x; };
	this.getY = function getY() { return this.y; };
	this.setX = function setX(x) { this.x = Math.abs(x); };
	this.setY = function setY(y) { this.y = Math.abs(y); };
}

function Cursor(x,y)
{
	this.inheritFromPosition = Position;
	this.inheritFromPosition(x,y);
	
	this.lastClicked = new Position(-50,-50);
	this.canANodeBeHere = 0;
			
	this.getLastClicked = function() { return this.lastClicked; };
	this.setLastClicked = function(x,y)
	{
		this.lastClicked.setX(x);
		this.lastClicked.setY(y);
	}
	
}

function GraphicalUserInterface()
{
	this.canvas = document.getElementById("graph");
	this.getCanvas = function() { return this.canvas; };
	
	this.ctx = this.canvas.getContext("2d");
	this.getContext = function() { return this.ctx; };
	
	this.graph = new Graph(false,true);
	
	this.mode = 'addNode';
	this.setMode = function(mode) { this.mode = mode; };
	this.getMode = function() { return this.mode; };
	
	this.maxNodes = 0;
	this.incMaxNodes = function() { this.maxNodes++; };
	this.getMaxNodes = function() { return this.maxNodes; };
	
	this.cursor = new Cursor(-50,-50);
	this.selectedNode = 0;
	this.browsedNode = new Array(0,false); //la première case contient l'id du noeud survolé, la deuxième case vaut true si au moins un noeud est survolé
	this.selectedEdge = new Array(0,0); //contient les id des noeuds reliés par l'arc
	
	this.display = function()
	{
		this.browsedNode[1] = false;
		this.cursor.canANodeBeHere = 0;
		this.ctx.save();
		
		this.ctx.strokeStyle = 'black';
		this.ctx.lineWidth = 1;
		this.ctx.clearRect(0,0,this.canvas.width,this.canvas.height);
		this.ctx.strokeRect(0,0,this.canvas.width,this.canvas.height);

		this.ctx.restore();
		
		this.drawGraph();
		if(this.browsedNode[1]==false)
			this.browsedNode[0]=0;
		
		if(this.mode == 'addNode' && this.cursor.canANodeBeHere == 0)
		{
			var ctx=GUI.getContext();
			ctx.save();
			ctx.beginPath();
			ctx.lineWidth="1";
			ctx.arc(this.cursor.getX(),this.cursor.getY(),NODE_RADIUS,0,2*Math.PI,true);
			ctx.stroke();
			ctx.closePath();
			ctx.restore();
		}
	}

	this.drawGraph = function()
	{
		var order = this.graph.getOrder();
		var node = this.graph.firstNode;

		for(var i=1;i<=order;i++)
		{
			var edge = node.firstEdge;
			while(edge != undefined)
			{
				edge.display();
				edge = edge.next;
			}
			node = node.next;
		}
		node = this.graph.firstNode;
		BOULE =0;

		for(var i=1;i<=order;i++)
		{
			node.display();
			node = node.next;
			
		}
	}
	
}


