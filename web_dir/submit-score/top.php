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

$sql =	"SELECT u.name AS name, r.time AS time, r.mapid AS map ". 
		"FROM rjr_runs r INNER JOIN rjr_players p ON p.playerid=r.playerid INNER JOIN users u ON u.uid=p.userid ".
		"WHERE r.mapid=? ".
		"ORDER BY r.time ASC";

$stmt = $conn->prepare($sql);
if ($stmt) {
	$stmt->bind_param('s', $_REQUEST['mapid']);
	
	if ($stmt->execute()) {
		$result = $stmt->get_result();
		while($row = $result->fetch_assoc()) {
			echo "{\n";
			echo "	\"player\" \"" . $row["name"] . "\"\n";
			echo "	\"time\" \"" . $row["time"] . "\"\n";
			echo "	\"map\" \"" . $row["map"] . "\"\n";
			echo "}\n";
		}
	} else {
	    echo "Error: " . $sql . "<br>" . mysqli_error($conn);
	}
	$stmt->close();
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;;
}

?>