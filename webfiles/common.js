function g(i) {
  return document.getElementById(i);
}

function _set(s, v) {
  g(s).value = v;
}

function _setB(s, v) {
  g(s).checked = v;
}

function _getS(s) {
  return g(s).value.trim();
}

function _getN(s) {
  return parseInt(g(s).value);
}

function _getB(s) {
  return g(s).checked;
}

function home() {
  window.setTimeout(function () {
    window.location.replace("admin.html");
  }, 1000);
}

async function loadSettings() {
  let response = await fetch("json.json");
  let json = await response.json();
  return json;
}

function saveSettings(obj) {
  fetch("savesettings.json", {
    method: "post",
    headers: {
      "Content-type": "application/json",
    },
    body: JSON.stringify(obj),
  }).then((r) => {
    console.log(r);
  });
  home();
}
