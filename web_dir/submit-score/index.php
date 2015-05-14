<?php

// Predefine these vars, they are set in the config.php
$servername = "";
$username = "";
$password = "";
$dbname = "";

require_once("config.php");

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

$sql = "INSERT INTO rjr_runs (playerid, mapid, time) VALUES (?, ?, ?)";
$stmt = $conn->prepare($sql);
if ($stmt == false) {
    echo "Error: " . $sql . "<br>" . $conn->error;;
}
$stmt->bind_param('ssi', $_REQUEST['playerid'], $_REQUEST['mapid'], $_REQUEST['time']);

if ($stmt->execute()) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;;
}

mysqli_close($conn);
?>
