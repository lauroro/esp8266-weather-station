// ------------------ General --------------------

/*
* <ESP-IP-ADDRESS>[/?name=name-value]
*
* // Fields
* name-value : everything
*/

// Defaults
var deviceName = "ESP8266 - Weather Station";


var queryString = decodeURIComponent(window.location.search); //parsing
queryString = queryString.substring(1); 
var queries = queryString.split("="); 
if(queries[0] == "name" && queries[1] != "")
  deviceName = queries[1];

document.getElementById("header").textContent = deviceName;


// ----- INIT ------

var tValue;
var hValue;
var label;
var labelsNum = 0;
var ctx = document.getElementById('chart').getContext('2d');   

var chart = new Chart(ctx, {
  type: 'line',
  data: {
    datasets:[
      {
        label: 'Temperature',
        borderColor: 'rgb(255, 38, 38)',
      },
      {
        label: 'Humidity',
        borderColor: 'rgb(51, 80, 255)',
      }
    ]
  },
  options: {
    maintainAspectRatio: false,
    responsive:true,
    y: {
      beginAtZero: true
    }
  }
});


// ------- CHART UPDATE LOOP --------

setInterval(function ( ) {
  // https://www.w3schools.com/xml/xml_http.asp
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById('temperature').innerHTML = this.responseText;
      tValue = this.responseText;
    }
  };
  xhttp.open('GET', '/temperature', true);
  xhttp.send();
}, 5000);

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById('humidity').innerHTML = this.responseText;
      hValue = this.responseText;
    }
  };
  xhttp.open('GET', '/humidity', true);
  xhttp.send();
}, 5000);

setInterval(function(){
  updateChart();
},5000);

function updateChart(){
  if(labelsNum == 20){
    chart.data.labels.shift();
    chart.data.datasets[0].data.shift();
    chart.data.datasets[1].data.shift();
  }
  else{
    labelsNum++;
  }
  chart.data.labels.push(new Date().toLocaleTimeString());
  chart.data.datasets[0].data.push(tValue);
  chart.data.datasets[1].data.push(hValue);
  chart.update();
}
