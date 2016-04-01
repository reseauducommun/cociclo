<?php

// conversion de valeurs mysql en JSON

header('Content-type:application/json');

// Load configuration as an array. Use the actual location of your configuration file
$config = parse_ini_file('../../Documents/config.ini');

// Try and connect to the database
$connection = mysqli_connect('localhost',$config['username'],$config['password'],$config['dbname']);


// Check connection
if (!$connection) {
    die("Connection failed: " . mysqli_connect_error());
}
//echo "Connected successfully";

//mysql_select_db('Cociclo');

$sql = "SELECT * FROM Cociclo";
$result = mysqli_query($connection, $sql);


if (mysqli_num_rows($result) > 0) {

echo "[";
    // output data of each row
    while($row = mysqli_fetch_assoc($result)) {
 //       echo "ID: " . $row["id"]. " - Name: " . $row["firstname"]. " " . $row["lastname"]. "<br>";

echo json_encode($row);
echo ", \n";      
   
// echo "yep";
 }
echo "{}]";
    
} else {
    echo "0 results";
}



mysqli_close($connection);




?>
