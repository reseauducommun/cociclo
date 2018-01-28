
<html>
<head>
<title>
cociclo
</title>


</head>


<body>


<form><select id="maListe"> <!-- les ID des cociclos sont affichées ici -->

<?php

echo "<option value=''>Cociclos</option>";

// Load configuration as an array. Use the actual location of your configuration file
$config = parse_ini_file('../../Documents/config.ini');

// Try and connect to the database
$connection = mysqli_connect('localhost',$config['username'],$config['password'],$config['dbname']);

// Check connection
if (!$connection) {
    die("Connection failed: " . mysqli_connect_error());
}

$sql = "SELECT distinct(ID) FROM `Cociclo`";
$result = mysqli_query($connection, $sql);

if (mysqli_num_rows($result) > 0) {
    // output data of each row
    while($row = mysqli_fetch_assoc($result)) {

echo "<option value='".$row["ID"]."'>".$row["ID"]."</option>";

}

} else {
    echo "0 results";
}

    

mysqli_close($connection);
    
    ?>

            </select></form>

</body>

<script>


//////////////////////////////////////////////////////////////////////////////////////
//                      On écoute les changements de la liste                       //
//////////////////////////////////////////////////////////////////////////////////////

        document.getElementById("maListe").onchange = function() {
            if (this.selectedIndex !== 0) {
            alert(this.value);
            //lesDonnees(this.value); // passe le nom du fichier à la fonction lesDonnees
            }
        }

</script>


</html>
