<!DOCTYPE html>
<html lang="en">
    <head>
        <base target="_top" />
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <title>Map tracker by CKD - Leaflet</title>
        <link rel="shortcut icon" type="image/x-icon" href="favicon.ico" />
        <!--
         <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" integrity="sha256-p4NxAoJBhIIN+hmNHrzRCf9tD/miZyoHS5obTRR9BMY=" crossorigin=""/>
         <script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js" integrity="sha256-20nQCchB9co0qIjJZRGuk2/Z9VM+kNiyxNV1lvTlZBo=" crossorigin=""></script>
         -->
        <link rel="stylesheet" href="leaflet/leaflet.css" />
        <script src="leaflet/leaflet.js"></script>
        <style>
            html,
            body {
                height: 100%;
                margin: 0;
            }
            .leaflet-container {
                height: 400px;
                width: 600px;
                max-width: 100%;
                max-height: 100%;
            }
            -ms-box-sizing: content-box;
            -moz-box-sizing: content-box;
            -webkit-box-sizing: content-box;
            box-sizing: content-box;
            .style1 {
                color: #ff0000;
                font-weight: bold;
                font-size: 24px;
            }
            .style2 {
                color: #0000ff;
                font-size: 24px;
            }
        .style3 {font-size: 24}
        </style>
    </head>
    <body>
        <table width="100%" border="0" cellpadding="5">
            <tr>
                <td align="center"><span class="style1">MAP TRACKER DEMO</span> by <span class="style2">CKD</span></td>
            </tr>
            <tr>
                <td>
                    <div id="map" style="width: 100%; height: 550px;"></div>
                </td>
            </tr>
            <tr>
                <td width="100%">
                    <div align="left">
                        Select device &nbsp;
                        <select id="devices" name="devices" style="width: 200px;" onChange="selectChange()">
                            <option value="0" selected>----------</option>
                        </select>
                        &nbsp;&nbsp;&nbsp; Auto update &nbsp;
                        <input id="checkbox" type="checkbox" onChange="checkboxChange()" checked />
                        &nbsp;
                        <input type="button" value="Update" onClick="getGPSandDrawPolyline()" />
                        &nbsp;
                        <input type="button" value="Clear" onClick="clearMap()" />
                    </div>
                    <table width="100%" border="0">
                        <tr align="center">
                            <td width="200px" bgcolor="#00FFFF"><span class="style3">device</span></td>
                          <td width="100px" bgcolor="#00FF99"><span class="style3">auto update</span></td>
                            <td><span class="style3"></span></td>
                          <td width="250px" bgcolor="#DDDDDD"><span class="style3">local time</span></td>
                      </tr>
                        <tr align="center">
                            <td bgcolor="#00FFFF">
                                <div id="dev"><span class="style3"></span></div>                          </td>
                            <td bgcolor="#00FF99">
                                <div class="style3" id="auto">TRUE</div>                          </td>
                          <td><span class="style3"></span></td>
                            <td bgcolor="#DDDDDD">
                                <div id="localTime"><span class="style3"></span></div>                            </td>
                      </tr>
                    </table>
                  <div id="data">HTML</div>
                </td>
            </tr>
        </table>
        <script>
// https://webformatter.com/html
// https://leafletjs.com/
// https://tile.openstreetmap.org
// Titops CT 10.008158, 105.777671
// CKD home	 10.288455, 105.755692
// [10.2840213,105.7468539]

var updateInterval = 1000;
var updateIntervalMap = 300000;

const ckd = [10.288455, 105.755692];
const titops = [10.008158, 105.777671];

const vn = [16.362702, 106.270887];
const map = L.map('map').setView(vn, 5);
const tiles = L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
	maxZoom: 19,
	attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
}).addTo(map);

var popup = L.popup();
map.on('click', onMapClick);

var polyline;

// center of map
var marker = L.marker(ckd).addTo(map);
marker.bindPopup("<b>CKD's here!</b>.").openPopup();
marker.on('click', onMarkerClick);
/**
var circle = L.circle([10.284, 105.756], {
color: 'red',
fillColor: '#f03',
fillOpacity: 0.5,
radius: 100
}).addTo(map);

var circleM = L.circleMarker([10.244, 105.736], {
color: 'red',
fillColor: '#f03',
fillOpacity: 0.5,
radius: 10
}).addTo(map);

var polygon = L.polygon([
[10.284, 105.756],
[10.244, 105.736],
[10.264, 105.796]
]).addTo(map);
**/
//circle.bindPopup("I am a circle.");
//circleM.bindPopup("I am a circleMarker.");
//polygon.bindPopup("I am a polygon.");

function onMapClick(e) {
	popup
		.setLatLng(e.latlng)
		.setContent("<div align='center'>You clicked the map at<br>" + e.latlng.toString() + "</div>")
		.openOn(map);
}

function mapDrawPolyline() {
	// create a red polyline from an array of LatLng points
	var latlngs = [
		[10.285826, 105.745310],
		[10.288203, 105.748820],
		[10.289929, 105.751297],
		[10.289858, 105.751956],
		[10.287611, 105.753009],
		[10.288140, 105.754307],
		[10.288493, 105.755418],
		[10.288504, 105.755674],
		[10.288559, 105.756331],
		[10.288550, 105.757710],
		[10.290307, 105.757849]
	];

	polyline = L.polyline(latlngs, {
		color: 'red',
		"weight": 5,
		"opacity": 0.5
	}).addTo(map);
	// zoom the map to the polyline
	map.fitBounds(polyline.getBounds());
}

function clearMap() {
	polyline.remove();
	map.setView(ckd, 13);
}

function onMarkerClick(e) {
	map.setView(e.latlng, 15);
}


////////////////////////////////////////////////
function getMapData() {
	// time is minutes...
	var uID = 1;
	var device = "sensor1";
	var post = "getdev=1";
	$.ajax({
		url: "http://mkgreen.org/iot/rf210/rf210GetPost.php",
		type: "POST",
		data: post,
		cache: false,
		success: function(html) {
			//$("#my_time").html(html);
			parseJson(html);
			chart.render();
		}
	});
	/////////////
	setTimeout(btnOkFunction, 1000);
}
////////////////////////////////////////////////
var deviceID = "";
var autoUpdate = true;

function getGPSandDrawPolyline() {
	// create a red polyline from an array of LatLng points
	var latlngs = [];
	if (deviceID != 0) {
		var post = "gps=1&devid=" + deviceID;
		$.ajax({
			url: "rf210GetPost.php",
			type: "POST",
			data: post,
			cache: false,
			dataType: "json",
			success: function(data) {
				$.each(data, function(key, value) {
					//selectBox.append(value[0] + ">" + value[1] + ">>");
					latlngs.push(value);
				});
				//$("#data").html(latlngs);

				if (latlngs.length > 0) {
					polyline = L.polyline(latlngs, {
						color: "red",
						weight: 5,
						opacity: 0.5
					}).addTo(map);
					map.fitBounds(polyline.getBounds());
				}
			},
			error: function(xhr, status, error) {
				console.error('Lỗi khi tải dữ liệu: ' + error);
			}
		});
	}
}

function getDevID() {
	var post = "getdev=1";
	$.ajax({
		url: "rf210GetPost.php",
		type: "POST",
		data: post,
		cache: false,
		dataType: 'json',
		success: function(data) {
			//$("#data").html(data);
			var selectBox = $('#devices');
			$.each(data, function(key, value) {
				selectBox.append('<option value="' + value + '">' + value + '</option>');
			});
		},
		error: function(xhr, status, error) {
			console.error('Lỗi khi tải dữ liệu: ' + error);
		}
	});
}

function selectChange() {
	deviceID = $("#devices").val();
	$("#dev").html(deviceID);
}

function checkboxChange() {
	autoUpdate = document.getElementById("checkbox").checked;
	if (autoUpdate) $("#auto").html("TRUE");
	else $("#auto").html("FALSE");
}

function getLocalTime() {
	var d = new Date();
	$("#localTime").html(d);
}
/** Start window onload **/
window.onload = function() {
	getLocalTime();
	getDevID();
}
setInterval(function() {
	doInterval()
}, updateInterval);
setInterval(function() {
	doIntervalUpdateMap()
}, updateIntervalMap);
var doInterval = function() {
	getLocalTime();
};
var doIntervalUpdateMap = function() {
	if (autoUpdate) {
		getGPSandDrawPolyline();
	}
}        </script>
        <script src="./js/jquery-3.7.1.min.js"></script>
    </body>
</html>
