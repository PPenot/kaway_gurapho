
<?php
$connect = mysql_connect('localhost','root','');
mysql_select_db("bdd",$connect);
$result=mysql_query("SELECT identifiant FROM GRAPHE")or die (mysql_error());
while($identifiant=mysql_fetch_array($result))
{
    echo "<option>".$identifiant['identifiant']."</option>";
}

?>
