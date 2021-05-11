const char settings_html[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8"/>
<meta name=viewport content="width=device-width, initial-scale=1" />
<link href=style.css rel=stylesheet type=text/css>
<script src=common.js></script>
<title>Settings information</title>
<script>var
H="innerHTML";function g(i){return document.getElementById(i);};window.onload=function(){var panel=g('info');panel[H]='Gettting info...';var s='';loadSettings().then(json=>{for(var prop in json){s+="<span class='left'>"+prop+"&nbsp;:</span><span class='right'>"+json[prop]+"</span><br>";}
panel[H]=s;}).catch(e=>console.log(e.message))}</script>
</head>
<body>
<a href=admin.html class="btn btn--home txt--l">&#11013;</a>
<strong>&nbsp;&nbsp;Settings info</strong>
<hr>
<div class="page box">
<div id=info></div>
</div>
</body>
</html>)=====";