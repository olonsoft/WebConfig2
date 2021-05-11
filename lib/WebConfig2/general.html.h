const char general_html[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8"/>
<meta name=viewport content="width=device-width, initial-scale=1" />
<link href=style.css rel=stylesheet type=text/css>
<script src=common.js></script>
<title>General configuration</title>
<script>window.onload=function(){loadSettings().then(json=>{_set("n",json.devicename);_set("p",json.adminpass)}).catch(e=>console.log(e.status))}
function saveVars(){var obj={devicename:_getS("n"),adminpass:_getS("p")};saveSettings(obj);home();}</script>
</head>
<body>
<a href=admin.html class="btn btn--home txt--l">&#11013;</a>
<strong>&nbsp;&nbsp;General config</strong>
<hr>
<div class="page box">
<div>
&nbsp;Device name:<br>
<input id=n name=n size=20 maxlength=32 placeholder='device name'><br><br>
&nbsp;Admin password:<br>
<input id=p name=p size=20 maxlength=32 type=password placeholder=Password>
<br><br>
</div>
</div>
<br>
<p onclick=saveVars() class="btn btn--green m">Save</p>
</body>
</html>)=====";