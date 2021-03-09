//  Main page. Displays info and buttons to try different functions
void HandleRoot() {
  Serial.println("Sending Homepage...");

  String responseHTML = R"foo(
	<html>
		<head>
			<style>
				html { font-family: Helvetica;
					display: inline-block;
					margin: 0px auto;
					text-align: center;
					font-size: 50px;
					background-color: #202020;
					color: white;
				}
                .container {
                    text-align: center;
                    padding: 5px 10px 0 10px;
                }
				a.button {
					background-color: #195B6A;
					display: block;
					border: none;
					color: white;
					padding: 16px 40px;
					text-decoration: none;
					font-size: 75px;
					cursor: pointer;
					-webkit-user-select: none; /* Safari */
					-moz-user-select: none; /* Firefox */
					-ms-user-select: none; /* IE10+/Edge */
					user-select: none; /* Standard */
				}
                .options {
                    list-style: none;
                    padding: 0;
                }
                .options > * {
                    margin-bottom: 25px;
                }

                .input-option {
                    display:flex;
                    flex-direction:row;
                    border:1px solid grey;
                    padding:2px;
                }
                .input-option-text {
                    flex-grow:2;
                    border:none;
                    font-size: 50px;
                    text-align: center;
                }
                .submit-button {
                    border:1px solid darkgreen;
                    background:darkgreen;
                    color:white;
                    font-size: 50px;
                    padding: 0px 20px;
                }
                .submit-button:disabled {
                    border:1px solid gray;
                    background:gray;
                }

			</style>

			<script>
				var targetTemp = 100.0;
				function SendRequest(url) {
					var http = new XMLHttpRequest();
					http.open('GET', url, true);
					http.send();
				}

				function Update() {
					var xhttp = new XMLHttpRequest();
					xhttp.onreadystatechange = function() {
						if (this.readyState == 4 && this.status == 200) {
							let data = JSON.parse(this.responseText);
							let ambientTemp = data["ambientTemp"];
							let objectTemp = data["objectTemp"];
							targetTemp = data["targetTemp"];

							document.getElementById("ambientTemp").innerHTML = ambientTemp.toFixed(2).toString() + "&deg;F";
							document.getElementById("objectTemp").innerHTML = objectTemp.toFixed(2).toString() + "&deg;F";
							document.getElementById("targetTemp").innerHTML = targetTemp.toFixed(2).toString() + "&deg;F";
						}
					};
					xhttp.open("GET", "temperature", true);
					xhttp.send();
				}

				function SendRequest(url) {
                    console.log("Sending request to", url);
					var xhttp = new XMLHttpRequest();
					xhttp.open("GET", url, true);
					xhttp.send();
				}

				function UpdateInput(textbox) {
                    if (textbox.value)
                        textbox.nextElementSibling.disabled = false;
                    else
                        textbox.nextElementSibling.disabled = true;
                }

                function SendCmd(btn) {
                    let url = btn.getAttribute('data-url');
                    let param = btn.getAttribute('data-param')
                    let data = btn.previousElementSibling.value;
                    SendRequest("/" + url + "?" + param + "=" + data)

                    btn.previousElementSibling.value = "";
                    btn.disabled = true;
                }
				setInterval(Update, 1000);
				Update();
			</script>
		</head>

		<body>
            <div class="container">
                <h3>ESP8266 Tea Server</h3>
                <hr>
                <h3>Ambient Temperature: <span id="ambientTemp">?</span></h3>
                <h3>Object Temperature: <span id="objectTemp">?</span></h3>
                <h3>Target Tea Temperature: <span id="targetTemp">?</span></h3>

                <hr>
                <ul class="options">
                    <div class="input-option">
                        <input class="input-option-text" type="text" placeholder="New Target Tea Temperature" oninput="UpdateInput(this)">
                        <button class="submit-button" onclick="SendCmd(this);" data-url="set_temperature" data-param="temperature" disabled>&#10004;</button>
                    </div>

                    <div class="input-option">
                        <input class="input-option-text" type="text" placeholder="New AP Name" oninput="UpdateInput(this)"  maxlength="20">
                        <button class="submit-button" onclick="SendCmd(this);" data-url="set_name" data-param="network_name" disabled>&#10004;</button>
                    </div>

                    <a class='button' onclick='SendRequest("test_buzzer")' style='background-color:red'>Test Buzzer</a>
                </ul>
            </div>
		</body>
	</html>
    )foo";

  //oninput for continuous updates

  server.send(200, "text/html", responseHTML);
}

void HandleGetTemperature() {
  String responseHTML = GetStatus();
  server.send(200, "text/html", responseHTML);
}

void HandleSetTemperature() {
  if (!server.hasArg("temperature"))
    server.send(400);
  float newTemp = server.arg("temperature").toFloat();
  saveData.targetTemp = newTemp;
  SaveData();
  server.send(200);
}

void HandleSetNetworkName() {
  if (!server.hasArg("network_name"))
    server.send(400);
  String networkName = server.arg("network_name");
  char buf[20];
  networkName.toCharArray(buf, 20);
  strncpy(saveData.networkName, buf, 20);
  SaveData();
  server.send(200);
  ESP.restart();
}

void HandleTestBuzzer() {
  for (int i = 1; i < 15; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100 / (i / 2.0));
    digitalWrite(BUZZER_PIN, LOW);
    delay(100 / (i / 2.0));
  }
  for (int i = 15; i >= 1; i--) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100 / (i / 2.0));
    digitalWrite(BUZZER_PIN, LOW);
    delay(100 / (i / 2.0));
  }
  int noteLength = 7;
  for(int i = 31; i < 1500; i += 5) {
  	tone(BUZZER_PIN, i, noteLength);
  	delay(noteLength);
  }
  for(int i = 1500; i >= 31; i -= 5) {
  	tone(BUZZER_PIN, i, noteLength);
  	delay(noteLength);
  }

  server.send(200);
}

void HandleNotFound() {
  server.send(404, "text/html", "<h1>Not Found</h1>");
}
