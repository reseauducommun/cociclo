<?php 
// Load configuration as an array. Use the actual location of your configuration file
$config = parse_ini_file('../../Documents/config.ini'); 
//echo $config['username'];
// Try and connect to the database
$connection = mysqli_connect('localhost',$config['username'],$config['password'],$config['dbname']);
if($connection == true) { 
//echo "success"; 
}
// If connection was not successful, handle the error
if($connection == false) {
    // Handle error - notify administrator, log to a file, show an error screen, etc.
echo "argh";
}
$value_ID = $_GET["ID"];
//echo $value_ID;
//$value_Date =  $_GET["Date"];
$value_Date = new DateTime();
$value_Date =  $value_Date->format('Y-m-d H:i:s');
$value_CO = $_GET["CO"];
$value_Temp = $_GET["t"];
$value_Hum = $_GET["h"];
$value_Latitude = $_GET["La"];
$value_Longitude = $_GET["Lo"];
$sql = "INSERT INTO Cociclo (ID, Date, CO, t, h, La, Lo)
VALUES ('$value_ID', '$value_Date', '$value_CO', '$value_Temp', '$value_Hum', '$value_Latitude', '$value_Longitude')";
//VALUES ('test', '2016-03-23 00:59:42', '127', '5', '70')";
if ($connection->query($sql) == TRUE) {
    echo "funciona";
echo $value_Longitude;
} else {
    echo "Error: " . $sql . "<br>" . $connection->error;
}
$connection->close();
?>
