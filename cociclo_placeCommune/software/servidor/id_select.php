
<html>
<head>
<title>
cociclo
</title>
</head>


<body>


<form><select id="maListe"> <!-- les valeurs qui emanent de lecture.php sont affichées ici -->

<?php
 //echo "<option value= "test">testing</option>";
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
//echo '<option value="'.$row['ID'].'">'.$row['ID'].'</option>';

//echo '<option value="';
//echo $row['ID'];
//echo '">';
//echo $row['ID'];
//echo '</option>';

}

} else {
    echo "0 results";
}

    

mysqli_close($connection);
    
    ?>

            </select></form>

</body>
</html>
