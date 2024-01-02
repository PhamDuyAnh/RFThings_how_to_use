<?php
include "mysql.php";

$dbtable_rf = "_rf210_Data";
$dbtable_dev = "_rf210_Dev";
$requestTimeG = 300; // seccond, 60 -> max 1440 requests per day, 300 -> 288r per day

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Database Connection failed: " . $conn->connect_error);
}

$sql = "CREATE TABLE $dbtable_dev(
	    `id` INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
	    `devid` VARCHAR(24) DEFAULT NULL,
	    `devname` LONGTEXT CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL DEFAULT NULL,
	    `last` DATETIME DEFAULT NULL,
	    `TimeStamp` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
	)";
// InnoDB
if ($conn->query($sql) === true) {
    echo "$dbtable_dev tables are ready to use";
} else {
    echo "Error creating table: " . $conn->error;
}
// format: YYYY-MM-DD HH:MI:SS
$sql = "CREATE TABLE $dbtable_rf(
	    `id` INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
	    `devid` VARCHAR(24) DEFAULT NULL,
	    `tem` FLOAT(6) DEFAULT NULL,
	    `hum` FLOAT(6) DEFAULT NULL,
	    `ax` FLOAT(6) DEFAULT NULL,
	    `ay` FLOAT(6) DEFAULT NULL,
	    `az` FLOAT(6) DEFAULT NULL,
	    `sat` INT(4) DEFAULT NULL,
	    `lat` VARCHAR(12) DEFAULT NULL,
	    `lon` VARCHAR(12) DEFAULT NULL,
	    `alt` VARCHAR(12) DEFAULT NULL,
	    `time` DATETIME DEFAULT NULL,
	    `bat` INT(4) DEFAULT NULL,
	    `TimeStamp` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
	)";

if ($conn->query($sql) === true) {
    echo "$dbtable_rf tables are ready to use";
} else {
    echo "Error creating table: " . $conn->error;
}

$conn->close();
?>
