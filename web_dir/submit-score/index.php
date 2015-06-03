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
    $sql =	"SELECT u.name AS name, r.time AS time, r.mapid AS map ". 
    		"FROM rjr_runs r INNER JOIN rjr_players p ON p.playerid=r.playerid INNER JOIN cms_users u ON u.uid=p.userid ".
    		"WHERE r.mapid=? ".
    		"ORDER BY r.time ASC";
    
    $topstmt = $conn->prepare($sql);
    if ($topstmt) {
    	$topstmt->bind_param('s', $_REQUEST['mapid']);
    	
    	if ($topstmt->execute()) {
    		$result = $topstmt->get_result();
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
    	$topstmt->close();
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;;
    }
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;;
}

mysqli_close($conn);
?>
