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
$value_ID = "";
//echo $value_ID;
//$value_Date =  $_GET["Date"];
$value_Date = new DateTime();
$value_Date =  $value_Date->format('Y-m-d H:i:s');
$value_MAC = "";
//$value_MAC = $_GET["MAC_char"];
$value_CO = "";
$value_maxCO = "";
//$value_maxCO = $_GET["MCO"];
$value_Temp = "";
$value_Hum = "";
$value_Latitude = "";
$value_Longitude = "";

 if (isset($_GET["MAC_char"])) 
 {
  $value_MAC = $_GET["MAC_char"];  
 }
 if (isset($_GET["ID"])) 
 {
  $value_ID = $_GET["ID"];  
 }
  if (isset($_GET["CO"])) 
 {
  $value_CO = $_GET["CO"];  
 }
  if (isset($_GET["MCO"])) 
 {
  $value_maxCO = $_GET["MCO"];  
 }
  if (isset($_GET["t"])) 
 {
  $value_Temp = $_GET["t"];  
 }
  if (isset($_GET["h"])) 
 {
  $value_Hum = $_GET["h"];  
 }
  if (isset($_GET["La"])) 
 {
  $value_Latitude = $_GET["La"];  
 }
  if (isset($_GET["Lo"])) 
 {
  $value_Longitude= $_GET["Lo"];  
 }
 
  $paramValides=false;
 //vérifier que chaque variable possède une valeur
 //sinon l'insertion est annulée
 if( 
($value_ID!="")&&($value_MAC!="")&&($value_CO!="")&&($value_maxCO!="")&&($value_Temp!="")&&($value_Hum!="")&&($value_Latitude!="")&&($value_Longitude!="") 
)
 {
   $paramValides=true;  
 }
 //validation contre injection sql
 //vérifier chaque variable selon son type de donnée
 if ((!is_numeric($value_ID))&&(!is_numeric($value_CO)))
 {
     $paramValides=false; 
 }
 
 
if ($paramValides==true)
{
    $sql = "INSERT INTO Cociclo (MAC_char,ID, Date, CO, MCO, t, h, La, Lo)
    VALUES ('$value_MAC','$value_ID', '$value_Date', '$value_CO', '$value_maxCO', '$value_Temp', '$value_Hum', '$value_Latitude', '$value_Longitude')";
    //$sql = "INSERT INTO Cociclo (ID, Date, CO, t, h, La, Lo)
    //VALUES ('$value_ID', '$value_Date', '$value_CO', '$value_Temp', '$value_Hum', '$value_Latitude', '$value_Longitude')";
    //VALUES ('test', '2016-03-23 00:59:42', '127', '5', '70')";
    if ($connection->query($sql) == TRUE) {
        echo "funciona";
        echo $value_Longitude;
    } else {
        echo "Error: " . $sql . "<br>" . $connection->error;
    }
   
} else {
        echo "Error: opration annulee , donnees non valides.<br>"; 
    }
 $connection->close();
?>
