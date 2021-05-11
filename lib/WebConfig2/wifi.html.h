const char wifi_html[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
<head>
<meta charset=utf-8 />
<meta name=viewport content="width=device-width, initial-scale=1, user-scalable=no" />
<link href=style.css rel=stylesheet type=text/css>
<script src=common.js></script>
<title>WiFi configuration</title>
<script>var S="setAttribute",A="appendChild",H="innerHTML",X,found,urlScan="scanwifi.json",urlNetwork="network.json",urlSave="savenetwork.json";function g(i){return document.getElementById(i);};function p(t,l){if(confirm(t))window.location=l;};function E(s){return document.createElement(s)};var cnt=0;function addSavedDiv(ssid,pass,dhcp,ip,netmask,gateway,dns){d=E('div');dn="d"+cnt;d[S]("id",dn);d[S]("class","box")
var checked=dhcp?" checked":"";d.innerHTML='&#128246;<input value="'+ssid+'" size=18 maxlength=32 placeholder="SSID">\
          <a class="q m" onClick=deleteDiv(\"'+dn+'\");>&#10060;</a><br>\
          &#128273;<input value="'+pass+'" size=18 maxlength=64 type="password" placeholder="Password"><br>\
          DHCP:<input type="checkbox"'+checked+'><br>\
          IP:<input size=15 maxlength=15 placeholder="ip address" value="'+ip+'"><br>\
          Netmask:<input size=15 maxlength=15 placeholder="netmask" value="'+netmask+'"><br>\
          Gateway:<input size=15 maxlength=15 placeholder="gateway" value="'+gateway+'"><br>\
          DNS:<input size=15 maxlength=15 placeholder="dns" value="'+dns+'">';g("saved").appendChild(d);cnt++;}
function deleteDiv(divID){var parent=g("saved");parent.removeChild(g(divID));}
window.onload=function(){fetch(urlNetwork).then(r=>r.json()).then(json=>{for(var n in json){addSavedDiv(json[n].ssid,json[n].pass,json[n].dhcp,json[n].ip,json[n].netmask,json[n].gateway,json[n].dns);}}).catch(e=>console.log(e.message))
scan();}
function saveVars(){var nets=[],parent=g("saved");for(var i=1,l=parent.childNodes.length;i<l;i++){let ssid=parent.childNodes[i].getElementsByTagName('input')[0].value;let psw=parent.childNodes[i].getElementsByTagName('input')[1].value;let dhcp=parent.childNodes[i].getElementsByTagName('input')[2].checked;let ip=parent.childNodes[i].getElementsByTagName('input')[3].value;let netmask=parent.childNodes[i].getElementsByTagName('input')[4].value;let gateway=parent.childNodes[i].getElementsByTagName('input')[5].value;let dns=parent.childNodes[i].getElementsByTagName('input')[6].value;nets.push({"ssid":ssid,"pass":psw,"dhcp":dhcp,"ip":ip,"netmask":netmask,"gateway":gateway,"dns":dns});}
fetch(urlSave,{method:'post',headers:{"Content-type":"application/json"},body:JSON.stringify(nets)}).then(r=>{console.log(r)});home();}
function securityStr(security){if(security==7){return"&#128275;";}
else if(security==5){return"&#128274;";}
else if(security==2){return"&#128274;";}
else if(security==4){return"\uD83D\uDD12";}
else if(security==8){return"\uD83D\uDD12";}else return"\u2753";}
function scan(){if(X)return;found=g('found');found[H]="Scanning...";X=fetch(urlScan).then(r=>r.json()).then(array=>{X=null;found[H]="";if(array.length==0){found[H]="No networks found";}else{for(var j=0;j<array.length;j++){let ssid=array[j].ssid;let rssi=array[j].rssi;var d=E('div'),i=E('a'),c=E('a');i[S]('class','s m');i[S]('onclick','addSavedDiv(\"'+ssid+'\", "",true,"","","","");');c[S]('class','q');i[A](document.createTextNode(ssid));c[H]=rssi+"dBm"+securityStr(array[j].secure);found[A](i);found[A](c);found[A](E('br'));};};}).catch(e=>console.log(e.message))}
function toggle_ip(){if(document.getElementById('dhcp').checked){document.getElementById('ip_info').style.display="none";}else{document.getElementById('ip_info').style.display="block";}};</script>
</head>
<body>
<a href=admin.html class="btn btn--home txt--l">&#11013;</a>
<strong>&nbsp;&nbsp;WiFi configuration</strong>
<hr>
<p onclick=scan() class=btn>&#128260; Scan</p>
<br>
<div class="page box">
&nbsp;Networks found:
<div id=found class=box>
</div>
</div>
<br>
<div id=saved class="page box">
&nbsp;Saved networks:
</div>
<br>
<p onclick=saveVars() class="btn btn--green m">Save</p>
</body>
</html>)=====";