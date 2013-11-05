
<?php
header("Content-type: text/xml");
$selection =$_GET['val'];
//echo $selection;

//"ouverture de la bdd
$connect = mysql_connect('localhost','root','');
mysql_select_db("bdd",$connect);
//info graph


echo "<graph>";


$result=mysql_query("select * from graphe WHERE graphe.identifiant = '$selection'")or die (mysql_error());
while($graph=mysql_fetch_array($result))
{
	echo "<weighted>".$graph['pondere']."</weighted>";
	echo "<oriented>".$graph['oriente']."</oriented>";	
}

//info noeud
$result=mysql_query("select * from noeud WHERE noeud.identifiant = '$selection'")or die (mysql_error());
while($noeud=mysql_fetch_array($result))
{
	echo "<node>";
	echo "<node_id>".$noeud['id']."</node_id>";
    echo "<label>".$noeud['id_noeud']."</label>";
	echo "<x>".$noeud['x']."</x>";
	echo "<y>".$noeud['y']."</y>";
	echo "</node>";
}

//"info arc
$result=mysql_query("SELECT arc.valeur,arc.noeud_aller,arc.noeud_retour
                               FROM arc,noeud
                               WHERE noeud.id=arc.noeud_aller
                               AND noeud.identifiant= '$selection'")or die (mysql_error());
							   
while($arc=mysql_fetch_array($result))
{
echo "<edge>";
    echo "<weight>".$arc['valeur']."</weight>";
	echo "<id_source>".$arc['noeud_aller']."</id_source>";
	echo "<id_dest>".$arc['noeud_retour']."</id_dest>";
echo "</edge>";
}

echo "</graph>";

?>