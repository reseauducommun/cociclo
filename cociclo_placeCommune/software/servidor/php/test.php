<html>
<head>
<title>test d'appel des plus récentes valeurs</title>
</head>
<body>

<?php

// appel des dernières valeurs

// Load configuration as an array. Use the actual location of your configuration file
$config = parse_ini_file('../../Documents/config.ini');

/////$value_ID = $_GET["ID"];   //// hardcodé pour l'instant
/////echo $value_ID;

// Try and connect to the database
$connection = mysqli_connect('localhost',$config['username'],$config['password'],$config['dbname']);

// Check connection
if (!$connection) {
    die("Connection failed: " . mysqli_connect_error());
}
//// echo "Connected successfully";

$sql = "SELECT Date,CO,t,h,La,Lo FROM `Cociclo` WHERE ID = 'alx1' ORDER BY Date DESC LIMIT 1";

$result = mysqli_query($connection, $sql);

if (mysqli_num_rows($result) > 0) {

//echo "[";
    // output data of each row
    while($row = mysqli_fetch_assoc($result)) {
        echo "
Date: " . $row["Date"]. " 
CO: " . $row["CO"]. " ppm 
Temperature: " . $row["t"]. "*C 
Humidite: " . $row["h"]. "% 
Latitude: " . $row["La"]. " 
Longitude: " . $row["Lo"]. " 
    <br>";

echo " \n";

 }

} else {
    echo "0 results";
}


mysqli_close($connection);
    

?>

</body>
</html>
