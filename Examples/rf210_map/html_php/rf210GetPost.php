<?php
// https://onlinephp.io/
// https://reqbin.com/
// https://codebeautify.org/php-beautifier
// HTTP PUSH https://thingsboard.cloud/api/v1/integrations/http/0f3f1663-3ffa-1e15-738f-f88dcdf64816
include "mysql.php";
//include "function.php";

$dbtable_data = "_rf210_Data";
$dbtable_dev = "_rf210_Dev";

//$max_Records = 288; // max records to save on table (max 5 min/record -> 288 rec/day)
//$requestTimeG = 240; // seccond, 60 -> max 1440 requests per day, 300 -> 288r per day

$okDev = false;
$okDB = false;
$okMethod = false;

$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    exit("Database Connection failed: " . $conn->connect_error);
} else {
    $okDB = true;
}

// check MOTHOD is OK
if (
    $_SERVER['REQUEST_METHOD'] == "POST" ||
    $_SERVER['REQUEST_METHOD'] == "GET"
) {
    // get Device ID & report devices id array
    if (isset($_POST['getdev']) || isset($_GET['getdev'])) {
        $sql = "SELECT `devid` FROM `$dbtable_dev` WHERE 1";
        //$sql = "SELECT * FROM `_rf210_Dev`";
        if ($result = $conn->query($sql)) {
            $data = [];
            while ($row = $result->fetch_assoc()) {
                $row_value = $row['devid'];
                array_push($data, $row_value);
            }
            $result->free();
            echo json_encode($data, JSON_NUMERIC_CHECK);
        }
    }

    if (
        (isset($_POST['gps']) || isset($_GET['gps'])) &&
        (isset($_POST['devid']) || isset($_GET['devid']))
    ) {
        if (isset($_POST['devid'])) {
            $devid = $_POST['devid'];
        }
        if (isset($_GET['devid'])) {
            $devid = $_GET['devid'];
        }
        if (!empty($devid)) {
            $sql = "SELECT `lat`, `lon` FROM `$dbtable_data` WHERE `devid`='$devid'";
            if ($result = $conn->query($sql)) {
                $data = [];
                while ($row = $result->fetch_assoc()) {
                    $row_value = array();
                    $row_value_lat = $row['lat'];
                    $row_value_lon = $row['lon'];
                    if ($row_value_lat != 0) {
                        $row_value[0] = $row_value_lat;
                        $row_value[1] = $row_value_lon;
                        array_push($data, $row_value);
                    }
                    
                }
                $result->free();
                echo json_encode($data, JSON_NUMERIC_CHECK);
                //echo json_encode($data, JSON_PRESERVE_ZERO_FRACTION | JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES);
            }
        }
    }
} else {
    echo "<br>Method invalid.";
}

$conn->close();
?>
