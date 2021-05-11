const char mqtt_html[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
<head>
<meta charset=utf-8 />
<meta name=viewport content="width=device-width, initial-scale=1" />
<link href=style.css rel=stylesheet type=text/css>
<script src=common.js></script>
<title>MQTT configuration</title>
<script>window.onload=function(){loadSettings().then(json=>{_set("b",json.broker);_set("p",json.mqttport);_set("u",json.mqttuser);_set("q",json.mqttpass);_set("bb",json.bbroker);_set("bp",json.bmqttport);_set("bu",json.bmqttuser);_set("bq",json.bmqttpass);_set("g",json.mqttglobaltopic);_set("i",json.statusupdateinterval);}).catch(e=>console.log(e.message));}
function saveVars(){var obj={broker:_getS("b"),mqttport:_getN("p"),mqttuser:_getS("u"),mqttpass:_getS("q"),bbroker:_getS("bb"),bmqttport:_getN("bp"),bmqttuser:_getS("bu"),bmqttpass:_getS("bq"),mqttglobaltopic:_getS("g"),statusupdateinterval:_getN("i")};saveSettings(obj);home();}</script>
</head>
<body>
<a href=admin.html class="btn btn--home txt--l">&#11013;</a>
<strong>MQTT configuration</strong>
<hr>
<div class="page box">
<form method=post action=/mqtt>
Main MQTT Broker:
<div class=box>
&nbsp;MQTTT Broker:
<input id=b name=b size=25 maxlength=64 placeholder=Broker>
<br>
&nbsp;MQTTT Port:
<input id=p name=p size=5 maxlength=5 placeholder=Port>
<br>
&nbsp;MQTT User:
<input id=u name=u size=12 maxlength=64 placeholder=User>
<br>
&nbsp;MQTT Password:
<input id=q name=q size=7 maxlength=64 type=password placeholder=Password>
</div>
<br>
Backup MQTT broker<br>
<div class=box>
<input id=bb name=bb size=25 maxlength=64 placeholder='Backup Broker'>
<br>
&nbsp;MQTTT Port:
<input id=bp name=bp size=5 maxlength=5 placeholder=Port>
<br>
&nbsp;MQTT User:
<input id=bu name=bu size=12 maxlength=64 placeholder=User>
<br>
&nbsp;MQTT Password:
<input id=bq name=bq size=7 maxlength=64 type=password placeholder=Password>
</div>
<br>
&nbsp;MQTT Global Topic:<br>
<input id=g name=g size=20 maxlength=64 placeholder='Global Topic'>
<br><br>
&nbsp;System status report interval (min):
<small>(status will be sent at topic: GlobalTopic/DeviceName/status)</small><br>
<input id=i name=i size=3 maxlength=3 placeholder=interval>
<br>
</form>
</div>
<br>
<p onclick=saveVars() class="btn btn--green m">Save</p>
</body>
</html>)=====";