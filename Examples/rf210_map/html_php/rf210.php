<?php
// https://onlinephp.io/
// https://reqbin.com/
// https://codebeautify.org/php-beautifier
// HTTP PUSH https://thingsboard.cloud/api/v1/integrations/http/0f3f1663-3ffa-1e15-738f-f88dcdf64816
include "mysql.php";
//include "function.php";

$dbtable_data = "_rf210_Data";
$dbtable_dev = "_rf210_Dev";

$max_Records = 1440; // max records to save on table (max 5 min/record -> 288 rec/day)
$requestTimeG = 30; // seccond, 60 -> max 1440 requests per day, 300 -> 288r per day

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

$devid = "";
$tem = 0;
$hum = 0;
$ax = 0;
$ay = 0;
$az = 0;
$sat = 0;
$lat = 0;
$lon = 0;
$alt = 0;
$time = "2000-01-01 00:00:00"; // YYYY-MM-DD HH:MI:SS
$bat = 0;

// check MOTHOD is OK
if (
    $_SERVER["REQUEST_METHOD"] == "GET" ||
    $_SERVER["REQUEST_METHOD"] == "POST"
) {
    if (isset($_POST["devid"])) {
        $devid = $_POST["devid"];
    }
    if (isset($_POST["tem"])) {
        $tem = $_POST["tem"];
    }
    if (isset($_POST["hum"])) {
        $hum = $_POST["hum"];
    }
    if (isset($_POST["ax"])) {
        $ax = $_POST["ax"];
    }
    if (isset($_POST["ay"])) {
        $ay = $_POST["ay"];
    }
    if (isset($_POST["az"])) {
        $az = $_POST["az"];
    }
    if (isset($_POST["sat"])) {
        $sat = $_POST["sat"];
    }
    if (isset($_POST["lat"])) {
        $lat = $_POST["lat"];
    }
    if (isset($_POST["lon"])) {
        $lon = $_POST["lon"];
    }
    if (isset($_POST["alt"])) {
        $alt = $_POST["alt"];
    }
    if (isset($_POST["time"])) {
        $time = date("Y-m-d H:i:s", $_POST["time"]);
    }
    if (isset($_POST["bat"])) {
        $bat = $_POST["bat"];
    }

    if (isset($_GET["devid"])) {
        $devid = $_GET["devid"];
    }
    if (isset($_GET["tem"])) {
        $tem = $_GET["tem"];
    }
    if (isset($_GET["hum"])) {
        $hum = $_GET["hum"];
    }
    if (isset($_GET["ax"])) {
        $ax = $_GET["ax"];
    }
    if (isset($_GET["ay"])) {
        $ay = $_GET["ay"];
    }
    if (isset($_GET["az"])) {
        $az = $_GET["az"];
    }
    if (isset($_GET["sat"])) {
        $sat = $_GET["sat"];
    }
    if (isset($_GET["lat"])) {
        $lat = $_GET["lat"];
    }
    if (isset($_GET["lon"])) {
        $lon = $_GET["lon"];
    }
    if (isset($_GET["alt"])) {
        $alt = $_GET["alt"];
    }
    if (isset($_GET["time"])) {
        $time = date("Y-m-d H:i:s", $_GET["time"]);
    }
    if (isset($_GET["bat"])) {
        $bat = $_GET["bat"];
    }

    $okMethod = true;
} else {
    echo "<br>Method invalid.";
}

// check DevID is OK
if (!empty($devid)) {
    $now = date("Y-m-d H:i:s", time()); // YYYY-MM-DD HH:MI:SS
    $sql = "SELECT * FROM $dbtable_dev WHERE devid = '$devid'";
    if ($result = $conn->query($sql)) {
        $row = $result->fetch_assoc();
        if ($devid === $row["devid"]) {
            $last = $row["last"];

            // echo "DevID đã tồn tại<br>";
            // echo "Now  ";
            // echo $now;
            // echo "<br>";
            // echo "Last ";
            // echo $last;
            // echo "<br>";

            if (strtotime($now) > strtotime($last) + $requestTimeG) {
                // echo "Time OK ";
                // echo strtotime($now) - strtotime($last);
                // echo "s<br>";

                $sql = "UPDATE $dbtable_dev SET last = ? WHERE devid = ?";
                $stmt = $conn->prepare($sql);
                if ($stmt) {
                    // i	corresponding variable has type int
                    // d	corresponding variable has type float
                    // s	corresponding variable has type string
                    // b	corresponding variable is a blob and will be sent in packets
                    $stmt->bind_param("ss", $now, $devid); // ss -> 2 string
                    if ($stmt->execute()) {
                        $okDev = true;
                        echo "Insert last OK<br>";
                    } else {
                        echo "Inset data ERROR " . $stmt->error;
                    }
                } else {
                    echo "Prepare sql ERROR " . $conn->error;
                }
                $stmt->close();
            }
        } else {
            $sql = "INSERT INTO $dbtable_dev (devid, last) VALUES (?,?)";
            // Statement
            $stmt = $conn->prepare($sql);
            if ($stmt) {
                // i	corresponding variable has type int
                // d	corresponding variable has type float
                // s	corresponding variable has type string
                // b	corresponding variable is a blob and will be sent in packets
                $stmt->bind_param("ss", $devid, $now); // ss -> 2 string
                if ($stmt->execute()) {
                    $okDev = true;
                    echo "Insert new Dev OK<br>";
                } else {
                    echo "Inset data ERROR " . $stmt->error;
                }
            } else {
                echo "Prepare sql ERROR " . $conn->error;
            }
            $stmt->close();
        }
    }
} else {
    echo "DevID empty<br>";
}
// is all OK, inset new data record
if ($okDev && $okDB && $okMethod) {
    $sql = "INSERT INTO $dbtable_data (devid, tem, hum, ax, ay, az, sat, lat, lon, alt, `time`, bat) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)";
    // Statement
    $stmt = $conn->prepare($sql);
    if ($stmt) {
        // i	corresponding variable has type int
        // d	corresponding variable has type float
        // s	corresponding variable has type string
        // b	corresponding variable is a blob and will be sent in packets
        $stmt->bind_param(
            "sdddddissssi",
            $devid,
            $tem,
            $hum,
            $ax,
            $ay,
            $az,
            $sat,
            $lat,
            $lon,
            $alt,
            $time,
            $bat
        );
        if ($stmt->execute()) {
            $okDev = true;
            echo "Insert data OK<br>";

            // check max records
            $sql = "SELECT COUNT(*) as id FROM $dbtable_data WHERE devid = '$devid'";
            $result = $conn->query($sql);
            $row = $result->fetch_assoc();

            $total_rows = $row["id"];
            if ($total_rows > $max_Records) {
                $delete_rows = $total_rows - $max_Records;
                $delete_query = "DELETE FROM $dbtable_data
								WHERE `devid` = '$devid'
								ORDER BY `id`
								LIMIT $delete_rows
								";
                if ($conn->query($delete_query) === true) {
                    echo "Delete records ROWs OK.";
                } else {
                    echo "Delete records ERROR." . $conn->error;
                }
            }
            // end check max records
        } else {
            echo "Inset data ERROR " . $stmt->error;
        }
    } else {
        echo "Prepare sql ERROR " . $conn->error;
    }
    $stmt->close();
}

$conn->close();
?>
