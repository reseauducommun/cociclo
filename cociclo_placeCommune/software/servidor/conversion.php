<?php

// conversion de valeurs mysql en JSON

header('Content-type:application/json');

// Load configuration as an array. Use the actual location of your configuration file
$config = parse_ini_file('../../Documents/config.ini');
$value_ID = $_GET["ID"];
/////echo $value_ID;


// Try and connect to the database
$connection = mysqli_connect('localhost',$config['username'],$config['password'],$config['dbname']);


// Check connection
if (!$connection) {
    die("Connection failed: " . mysqli_connect_error());
}
//echo "Connected successfully";

//mysql_select_db('Cociclo');

////$sql = "SELECT * FROM Cociclo";

////$result = mysqli_query($connection, $sql);
/////$sql = "SELECT * FROM `Cociclo` WHERE ID='alexhuggett_1'";

$sql = "SELECT * FROM `Cociclo` WHERE ID = '$value_ID'";
$result = mysqli_query($connection, $sql);

if (mysqli_num_rows($result) > 0) {

echo "[";
    // output data of each row
    while($row = mysqli_fetch_assoc($result)) {
 //       echo "ID: " . $row["id"]. " - Name: " . $row["firstname"]. " " . $row["lastname"]. "<br>";

echo json_encode($row);
echo ", \n";      
   
 }

$value_Date = new DateTime();
$value_Date =  $value_Date->format('Y-m-d H:i:s');
echo '{"ID":"foubar","Date":"';
echo $value_Date;
echo '","CO":"200","t":"21.2","h":"29","La":"42","Lo":"42"}]';
//echo '{"ID":"foubar","Date":"2016-04-3 21:00:00","CO":"91","t":"21.2","h":"29","La":"42","Lo":"42"}]';

    
} else {
    echo "0 results";
}



mysqli_close($connection);




?>
