<?php
// return a list of files in a directory
$dir    = 'uploads';
$textfiles = scandir($dir);

$x = sizeof($textfiles)-2;
//echo '<form> <select id="maListe">';

// echo '<option value="default">elige un archivo</option>';
echo '<option value="default">choisissez un fichier</option>';
for ($i=0;$i<$x;$i++){
echo '<option value="';
echo $textfiles[($i+2)];
echo '">';
echo $textfiles[($i+2)];
echo '</option>';
}
//echo '</select></form>';

?>
