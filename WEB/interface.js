/*****************************************************************************/
/*                          VARIABLES GLOBALES                               */
/*****************************************************************************/

var GUI;
var KONAMICODE = new Array(1,38,38,40,40,37,39,37,39,66,65);
var requester;
var loader;
var saver;
var lol;

/*****************************************************************************/
/*                         FONCTION PRINCIPALES                              */
/*****************************************************************************/

function zoom(event)
{
	GUI.graph.print();
}


function changeCurPos(event)
{
	var canvas = GUI.getCanvas();
	var tdgraph = document.getElementById('tdGraph');
	var tdmenu = document.getElementById('tdaddNode');

	GUI.cursor.setX( event.pageX - canvas.offsetLeft - tdgraph.offsetLeft - tdmenu.offsetLeft);
	GUI.cursor.setY( event.pageY - canvas.offsetTop - tdgraph.offsetTop - tdmenu.offsetTop);
}

function addListener(element, event, listener)
{
	if(element.addEventListener) // Netscape, Firefox, ...
	{ 
		element.addEventListener(event, listener, false);
	}
	else // IE
	{
		element.attachEvent("on"+event, listener);
	}
}


function clearGraph(event)
{
	location.reload();
}


function mouseMoveReact(event)
{
	GUI.display();
}

function clickReact(event)
{
	var mode = GUI.getMode();

	var tdmode = document.getElementById('tdMode');
	var s ='';
	if(mode=='addNode' && GUI.cursor.canANodeBeHere == 0)
	{
		GUI.incMaxNodes();
		var max = GUI.getMaxNodes();

		GUI.graph.addNode(max,GUI.cursor.getX(),GUI.cursor.getY());

		GUI.display();
	}
	else if(mode == 'addEdge')
	{		
		var lastClicked = GUI.cursor.getLastClicked();
		
		lastClicked.setX(GUI.cursor.getX());
		lastClicked.setY(GUI.cursor.getY());
		
	
		
		if(GUI.selectedNode != 0 && GUI.browsedNode[0] != 0)
		{
			GUI.graph.addEdge(GUI.selectedNode,GUI.browsedNode[0]);
			GUI.cursor.setLastClicked(-50,-50);
			GUI.selectedNode = 0;
		}
		
		GUI.display();
		if(BOULE!=0)
			s = 'Noeud sélectionné : ' + GUI.selectedNode;
		else
			s = 'Noeud sélectionné : aucun';

	}
	else if(mode == 'select')
	{		
		var lastClicked = GUI.cursor.getLastClicked();
		
		lastClicked.setX(GUI.cursor.getX());
		lastClicked.setY(GUI.cursor.getY());
		GUI.display();
		
		s = '<b>Objet sélectionné :</b> ';
		if(GUI.selectedNode !=0 && BOULE!=0)
		{
			s += 'noeud d\'identifiant <input type=text id="nodeId" value="' + GUI.selectedNode + '"><br/>';
			s += '<b>Valeur :</b> <input type="text" id="nodeValue" value="' + GUI.graph.getNodeValue(GUI.selectedNode) + '"><button id="changeNodeValue">Modifier</button><br/>';
			s += '<button id="del">Supprimer</button>';
		}
		else if(GUI.selectedEdge!= [0,0])
		{
			s += 'arc de ' + GUI.selectedEdge[0] + ' vers ' + GUI.selectedEdge[1] + '<br/>';
			s += '<b>Poids :</b> <input type="text" id="edgeValue" value="' + GUI.graph.getEdgeValue(GUI.selectedEdge[0],GUI.selectedEdge[1]) + '"><button id="changeEdgeValue">Modifier</button><br/>';
			s += '<button id="del">Supprimer</button>';
		}
		else
			s+='Aucun';		
	}
	else if(mode == 'algo')
	{
	}
	else if(mode == 'save')
	{
	}
	else if(mode == 'load')
	{
	sendRequest();
	}
	tdmode.innerHTML = s;
	
	var del = document.getElementById('del');
	if(del != undefined) addListener(del,'click',removeElement);
	
	var ndVal = document.getElementById('changeNodeValue');
	if(ndVal != undefined) addListener(ndVal,'click',changeValue);
	
	var egVal = document.getElementById('changeEdgeValue');
	if(egVal != undefined) addListener(egVal,'click',changeValue);
	
	var save = document.getElementById('saveButton');
	if(save != undefined) addListener(save,'click',saveGraph);
	
	var loadButton = document.getElementById('charger');
	if(loadButton != undefined) addListener(loadButton,'click',loadgraph);
	
	
}

function konamicode(event)
{
	if(event.keyCode == KONAMICODE[KONAMICODE[0]])
		KONAMICODE[0]++;
	else KONAMICODE[0]=1;
	if(KONAMICODE[0]==11) document.location.href="krachdufe.html";
	
	console.log(GUI.graph.getSaveFormatedString());

}

addListener(window, "load", main);

function setListeners()
{
	addListener(GUI.getCanvas(),"mouseout",function(){GUI.display();});
	addListener(GUI.getCanvas(),"click",clickReact);
	//addListener(document.getElementById('del'),'click',removeElement);
	addListener(GUI.getCanvas(),'mousemove',function(){GUI.display();});
	//addListener(document.getElementById("mode"),"change",changeMode);
	addListener(GUI.getCanvas(),'mousemove',changeCurPos);
	//addListener(document.getElementById("clear"),"click",clearGraph);
	addListener(window,"keydown",keyDownReact);
	//addListener(document.getElementById('changeId'),"click",changeId);
	//addListener(document.getElementById('changeValue'),'click',changeValue);
	
	addListener(document.getElementById('tdaddNode'),'click',changeMode);
	addListener(document.getElementById('tdaddEdge'),'click',changeMode);
	addListener(document.getElementById('tdselect'),'click',changeMode);
	addListener(document.getElementById('tdalgo'),'click',changeMode);
	addListener(document.getElementById('tdsave'),'click',changeMode);
	addListener(document.getElementById('tdload'),'click',changeMode);


	//addListener(document.getElementById("charger"),"click",loadgraph);
	//addListener(document.getElementById("saveButton"),"click",saveGraph);

	addListener(document.getElementById('clear'),'click',function(){ GUI=new GraphicalUserInterface(); GUI.display(); });
}

function removeElement()
{
	if(GUI.selectedNode!=0)
	{
		GUI.graph.removeNode(GUI.selectedNode);
		GUI.display();
		GUI.selectedNode=0;
	}
	else if(GUI.selectedEdge != [0,0])
	{
		GUI.graph.removeEdge(GUI.selectedEdge[0],GUI.selectedEdge[1]);
		GUI.selectedEdge = [0,0];
		GUI.display();
	}

}

function changeId(event)
{
	
	if(GUI.selectedNode != 0)
	{
		var val = parseInt(document.getElementById('id').value);
		GUI.display();
	}
}


function changeValue(event)
{
	
	if(GUI.selectedNode != 0)
	{
		var val = parseFloat(document.getElementById('nodeValue').value);
		GUI.graph.setNodeValue(GUI.selectedNode,val);
		GUI.display();
	}
	else if(GUI.selectedEdge != [0,0])
	{
		var val = parseFloat(document.getElementById('edgeValue').value);
		GUI.graph.setEdgeValue(GUI.selectedEdge[0],GUI.selectedEdge[1],val);
		GUI.display();
	}
}



function keyDownReact(event)
{
	konamicode(event);

	switch(event.keyCode)
	{
		case 83:
			GUI.setMode('select');
			//document.getElementById("mode").value = "Selection";
		break;
		case 78:
			GUI.setMode('addNode');
			//document.getElementById("mode").value = "Ajout d'un noeud";
		break;
		case 69:
			GUI.setMode('addEdge');
			//document.getElementById("mode").value = "Ajout d'un arc";
		break;
		case 46:
			if(GUI.selectedNode!=0 && BOULE !=0)
			{
				GUI.graph.removeNode(GUI.selectedNode);
				GUI.display();
				GUI.selectedNode=0;
			}
			else if(GUI.selectedEdge != [0,0])
			{
				GUI.graph.removeEdge(GUI.selectedEdge[0],GUI.selectedEdge[1]);
				GUI.selectedEdge = [0,0];
				GUI.display();
			}
		break;
	}
}

function changeMode(event)
{
	// if(event.target.value == 'Ajout d\'un noeud')
		// GUI.setMode('addNode');
	// else if(event.target.value == 'Ajout d\'un arc')
		// GUI.setMode('addEdge');
	// else if(event.target.value == 'Selection')
		// GUI.setMode('select');
		
	var mode = event.target.id.substring(2);
	
	var oldmode = GUI.getMode();
	var oldcell = document.getElementById('td'+oldmode);
	oldcell.style.border = '3px solid black';
	oldcell.style.frame = 'box';
	
	GUI.setMode(mode);
	var cell = document.getElementById('td' + mode);
	cell.style.border = '3px solid red';
	cell.style.frame = 'box';
	
	
	var tdmode = document.getElementById('tdMode');
	var s = '';
	if(mode=='addNode')
	{
		
	}
	else if(mode == 'addEdge')
	{
		s = 'Noeud sélectionné : ';
		if(GUI.selectedNode!=0)
			s += GUI.selectedNode;
		else
			s += 'aucun';
	}
	else if(mode == 'select')
	{
		s = '<b>Objet sélectionné :</b> ';
		if(GUI.selectedNode !=0 && BOULE!=0)
		{
			s += 'noeud d\'identifiant <input type=text id="nodeId" value="' + GUI.selectedNode + '"><br/>';
			s += '<b>Valeur :</b> <input type="text" id="nodeValue" value="' + GUI.graph.getNodeValue(GUI.selectedNode) + '"><button id="changeNodeValue">Modifier</button><br/>';
			s += '<button id="del">Supprimer</button>';
		}
		else if(GUI.selectedEdge[0]!=0 && GUI.selectedEdge[1]!=0)
		{
			s += 'arc de ' + GUI.selectedEdge[0] + ' vers ' + GUI.selectedEdge[1] + '<br/>';
			s += '<b>Poids :</b> <input type="text" id="edgeValue" value="' + GUI.graph.getEdgeValue(GUI.selectedEdge[0],GUI.selectedEdge[1]) + '"><button id="changeEdgeValue">Modifier</button><br/>';
			s += '<button id="del">Supprimer</button>';
		}
		else
			s+='aucun';
	}
	else if(mode =='algo')
	{
		s = 'Algorithmes disponibles :<br/><br/>';
		s += '<select id="algo"></select><button>GO!!!</button>';
	}
	else if(mode == 'save')
	{
		s += 'Sauvegarder le graphe<br/><br/>';
		s += '<input type="text" id="name"><button id="saveButton">Sauvegarder</button>';
		//addListener(document.getElementById("saveButton"),"click",saveGraph);
	}
	else if(mode == 'load')
	{
		s = 'Graphes disponibles :<br/><br/>';
		s += '<select id="savedGraphs"></select><button id="charger">Charger</button>';
		sendRequest();
	}

	tdmode.innerHTML = s;
	var save = document.getElementById('saveButton');
	if(save != undefined) addListener(save,'click',saveGraph);
	
	var loadButton = document.getElementById('charger');
	if(loadButton != undefined) addListener(loadButton,'click',loadgraph);

}

function loadgraph()
{
	var name = document.getElementById("savedGraphs").value;
	console.log(name);
	requester = getRequester();
	requester.open("GET", "http://localhost/final/load.php?val="+name, true);
	requester.send(null);
	requester.onreadystatechange= loadTheGraph;
}


function loadTheGraph()
{
	if(requester.readyState == 4 && requester.status == 200)
	{
		var xml = requester.responseXML;
		
		var nodes = new Array(4); //les id de la bdd sont sur la première ligne, les labels sur la deuxième, les coordonnées x et y respectivement sur la 3eme et la 4eme
		
		//console.log(xml.getElementsByTagName('node_id'));
		
		nodes[0] = xml.getElementsByTagName('node_id');
		nodes[1] = xml.getElementsByTagName('label');
		nodes[2] = xml.getElementsByTagName('x');
		nodes[3] = xml.getElementsByTagName('y');

		var getBool = function(x) { return x=='false'; };
		
		var newGraph = new Graph(getBool(xml.getElementsByTagName('oriented')[0].firstChild.nodeValue),getBool(xml.getElementsByTagName('weighted')[0].firstChild.nodeValue));
		for(var i=0;i<nodes[0].length; i++)
			newGraph.addNode(parseInt(nodes[1][i].firstChild.nodeValue),parseInt(nodes[2][i].firstChild.nodeValue),parseInt(nodes[3][i].firstChild.nodeValue));		
		
		var getLabelById = function(id)
		{
			var i = 0;
			var boule = false;
			var label;
			do
			{
				if(parseInt(nodes[0][i].firstChild.nodeValue) == id)
				{
					label = parseFloat(nodes[1][i].firstChild.nodeValue);
					boule = true;
				}
				i++;
			}
			while(i<nodes[0].length && boule==false);
			return label;
		}

		var edges = new Array(3);
		edges[0] = xml.getElementsByTagName('id_source');
		edges[1] = xml.getElementsByTagName('id_dest');
		edges[2] = xml.getElementsByTagName('weight');

		for(var i=0;i<edges[0].length;i++)
		{
			
			var label_source = getLabelById(parseInt(edges[0][i].firstChild.nodeValue));
			var label_dest = getLabelById(parseInt(edges[1][i].firstChild.nodeValue));
			var weight = parseFloat(edges[2][i].firstChild.nodeValue);

			newGraph.addEdge(label_source,label_dest,weight);
			GUI.display()
		}
		newGraph.print();
		GUI.graph = newGraph;
		GUI.maxNodes = newGraph.getOrder();
		GUI.display()
	}
//GUI.display()
}

function saveGraph()
{
	var name = document.getElementById("name").value;

	var infograph = GUI.graph.getSaveFormatedString();
	console.log(infograph);
	var order =GUI.getMaxNodes();
	saver = getRequester();
    saver.open("GET", "http://localhost/final/save.php?val="+name+"&"+infograph+"&nb="+order, true);
    saver.send(null);
    saver.onreadystatechange= saveTheGraph;
	
}

function saveTheGraph()
{
	var name = document.getElementById("name").value;
	
	//sendRequest(); //permet de mettre a jour les identifiants pour le chargement .
	//alert('la sauvegarde de '+name+' à été réussi'); //si la sauvegarde c'est bien passer une page le confirme.
}

function sendRequest()
{
    requester = getRequester();
    requester.open("GET", "http://localhost/final/identifiant.php", true);
    requester.send(null);
    requester.onreadystatechange= treatResponse;
}

function treatResponse()
{
    if(requester.readyState == 4 && requester.status == 200)
    {
        var graphselect = document.getElementById("savedGraphs");
        graphselect.innerHTML=requester.responseText;
		
    }
}


function getRequester(){

	try{
		return new ActiveXObject("Msxml2.XMLHTTP");
	} catch(e) {}

	try{
		return new ActiveXObject("Microsoft.XMLHTTP");		
	} catch(e) {}

	try{
		return new XMLHttpRequest();
	} catch(e) {}	
	
	return undefined;
}



function main()
{
	var tmp = document.getElementById("tdaddNode");	
	tmp.style.border = '3px solid red';	
	
	GUI = new GraphicalUserInterface();
	setListeners();
	GUI.display();
	
	
	var zob = document.getElementsByTagName('body')[0];
	//zob.setAttribute('background-color','black');
	//console.log(zob.bg);
}


// background-image:url(Kawaii_Wallpaper.png);
	// background-position:50% 50%;
	// background-repeat: no-repeat;