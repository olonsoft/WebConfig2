﻿const char update_html[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8"/>
<meta name=viewport content="width=device-width, initial-scale=1" />
<link href=style.css rel=stylesheet type=text/css>
<script src=common.js></script>
<title>Update configuration</title>
<script>window.onload=function(){loadSettings().then(json=>{_set("c",json.firmwareconfurl);_set("ui",json.firmwareupdatecheckinterval);}).catch(e=>console.log(e.message));}
function saveVars(){var obj={firmwareconfurl:_getS("c"),firmwareupdatecheckinterval:_getN("ui")};saveSettings(obj);home();}</script>
</head>
<body>
<a href=admin.html class="btn btn--home txt--l">&#11013;</a>
<strong>&nbsp;&nbsp;Update config</strong>
<hr>
<div class="page box">
&nbsp;URL holding firmware update conf file:<br>
<input id=c name=c size=23 maxlength=128 placeholder='firmware .conf URL'>
<br><br>
&nbsp;Check for updates every:<br>
<input id=ui name=ui size=3 maxlength=3 placeholder=interval> minutes
<br>
</div>
<br>
<p onclick=saveVars() class="btn btn--green m">Save</p>
</body>
</html>)=====";