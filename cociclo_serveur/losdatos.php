<?php
// lecture d'un fichier pour envoi à javascript 

$camino = $_GET['fichier']; // le parametre envoye depuis le javascript

echo file_get_contents("uploads/$camino");

?>
