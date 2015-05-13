<?php

// Predefine these vars, they are set in the config.php
$servername = "";
$username = "";
$password = "";
$dbname = "";

require_once("config.php");

// Create connection
$conn = mysqli_connect($servername, $username, $password, $dbname);
// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

$sql = "INSERT INTO runs (playerid, mapid, time) VALUES ('".$_REQUEST['playerid']."', '".$_REQUEST['mapid']."', ".$_REQUEST['time'].")";

if (mysqli_query($conn, $sql)) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . mysqli_error($conn);
}

mysqli_close($conn);
?>
