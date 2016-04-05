<!DOCTYPE html>
<html>
<head>
    <title>CO (ppm)</title>

<meta charset="utf-8">
<link rel="icon" type="image/png" href="http://cociclo.io/images/icon.png">
<link rel="stylesheet" type="text/css" href="css/index.css">
</head>

<body>

<img src="images/cociclo_logo_petit.png" height="30" width="115">

<form>

<select id="maListe"> <!-- les ID des cociclos sont affichées ici -->    

<?php
echo "<option value='Cociclos'>Cociclos</option>";


// Load configuration as an array. Use the actual location of your configuration file
$config = parse_ini_file('../Documents/config.ini');

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

<div id="utilisateur">

<?php
$value_ID = $_GET["ID"];
echo $value_ID;
?>

</div>


<!-- load the d3.js library -->    
<script src="http://d3js.org/d3.v3.min.js"></script>

<script>

// Set the dimensions of the canvas / graph
var margin = {top: 30, right: 20, bottom: 30, left: 50},
    width = 600 - margin.left - margin.right,
    height = 270 - margin.top - margin.bottom;

// Parse the date / time
/////var parseDate = d3.time.format("%d-%b-%y").parse;
var parseDate = d3.time.format("%Y-%m-%d %H:%M:%S").parse;

// Set the ranges
var x = d3.time.scale().range([0, width]);
var y = d3.scale.linear().range([height, 0]);

// Define the axes
var xAxis = d3.svg.axis().scale(x)
    .orient("bottom").ticks(5);

var yAxis = d3.svg.axis().scale(y)
    .orient("left").ticks(5);

// Define the line
var valueline = d3.svg.line()
    .interpolate("basis") 
    .x(function(d) { return x(d.Date); })
    .y(function(d) { return y(d.CO); });
    
// Adds the svg canvas
var svg = d3.select("body")
    .append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
    .append("g")
        .attr("transform", 
              "translate(" + margin.left + "," + margin.top + ")");

// Get the data

var div = document.getElementById("utilisateur");
var myData = div.textContent;
//console.log('hey'+myData);

d3.json("php/conversion.php?ID="+myData, function(error, data) {
//d3.json("php/conversion.php?ID=alexhuggett_1", function(error, data) {
    data.forEach(function(d) {
        d.Date = parseDate(d.Date);
        d.CO = +d.CO;
// console.log(d.length);
///////	console.log(d.Date + ", " + d.CO);
      
    });


    // Scale the range of the data
    x.domain(d3.extent(data, function(d) { return d.Date; }));
    y.domain([0, d3.max(data, function(d) { return d.CO; })]);

    // Add the valueline path.
    svg.append("path")
        .attr("class", "line")
        .attr("d", valueline(data));

    // Add the X Axis
    svg.append("g")
        .attr("class", "x axis")
        .attr("transform", "translate(0," + height + ")")
        .call(xAxis);

    // Add the Y Axis
    svg.append("g")
        .attr("class", "y axis")
        .call(yAxis);

    
    /////// text labels ////// Add the text label for the x axis
    
    svg.append("text")
        .attr("transform", "translate(" + (width / 2) + " ," + (height + margin.bottom) + ")")
        .style("text-anchor", "middle")
        .text("Date");

    svg.append("g")         // Add the Y Axis
        .attr("class", "y axis")
        .call(yAxis);

    // Add the text label for the Y axis
    svg.append("text")
        .attr("transform", "rotate(-90)")
        .attr("y", 0 - margin.left)
        .attr("x",0 - (height / 2))
        .attr("dy", "1em")
        .style("text-anchor", "middle")
        .text("CO (ppm)");

    ///////
    
});

    

//////////////////////////////////////////////////////////////////////////////////////
//                      On écoute les changements de la liste                       //
//////////////////////////////////////////////////////////////////////////////////////

        document.getElementById("maListe").onchange = function() {
            if (this.selectedIndex !== 0) {
/////            alert(this.value);

 window.location.href = "CO.php?ID="+this.value;

            }
     
  }      

</script>
</body>
</html>
