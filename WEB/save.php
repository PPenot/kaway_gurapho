<?php

$selection =$_GET['val'];

$i = $_GET['nb'];
/*while (isset($_GET['nodei'*/
$connect = mysql_connect('localhost','root','');
mysql_select_db("bdd",$connect);

mysql_query("insert into graphe(identifiant,oriente,pondere) values ('$selection',false,true);")or die (mysql_error());
// insertion des noeuds
for ($n=0;$n<=$i;$n++)
	{
	if(isset($_GET['node'.$n]))
	{
	$nodeid =$_GET['node'.$n];
	$X = $_GET['coordx'.$n];	
	$Y = $_GET['coordy'.$n];
	mysql_query("insert into noeud (id_noeud,node_value,x,y,identifiant) values ('$n','$nodeid','$X','$Y','$selection');")or die (mysql_error());
	}
	//$i =$i+1;
}
echo ("la valeur de i".$i);

$result=mysql_query("select id,id_noeud from noeud WHERE noeud.identifiant = '$selection'");
while($noeud=mysql_fetch_array($result))
{
/*
$noeudid =$noeud['id_noeud'];
print_r($noeudid);
*/
$node[$noeud['id_noeud']] = $noeud['id'];
print_r($node);
}

for ($a=0;$a<=$i;$a++)
{
//echo("test boucle 1");
	for ($b=0;$b<=$i;$b++)
	{
		if(isset($_GET['edge'.$a."-".$b]))
		{
		//echo("test");
			$dep = $node[$a];
			echo ($node[$a]);
			$arr = $node[$b];
			echo ($node[$b]);
			$evaleur = $_GET['edge'.$a."-".$b];
			echo ("val".$evaleur);
			mysql_query("insert into arc (valeur,noeud_aller,noeud_retour) values ('$evaleur','$dep','$arr');")or die (mysql_error());
		}
	}
}


?>