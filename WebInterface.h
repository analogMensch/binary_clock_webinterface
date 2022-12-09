// WebInterface.h

#ifndef _WEBINTERFACE_H
#define _WEBINTERFACE_H

const char requestWeb[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
    <html>
      <head>
        <style>
          body {
            margin-top: 25px;
            margin-bottom: 25px;
            margin-left: 25px;
            margin-right: 25px;
            background-color: #333333;
            font-family: Helvetica;
            color: #AAAAAA;
          }
          .text {
            font-family: Helvetica;
            color: #AAAAAA;
          }
          .feedback {
            margin-top: 2.0em;
            color: #009090;
          }
          .form-r {
            font-family: Helvetica;
            color: #AAAAAA;
            line-height: 1.0em;
          }
          .form-s{ 
            font-family: Helvetica;
            color: #AAAAAA;
            line-height: 1.2em;
          }
          .button {
            background-color: #AAAAAA;
            color: #333333;
            font-weight: bold;
            width: 10.5em;
            border-radius: 10%;
          }
          .div-left {
            float: left;
            margin-top: 0.0em;
            margin-left: 0.0em;
          }
          .div-right {
            margin-top: 0.25em;
            margin-left: 10.5em;
          }
          .div-bottom {
            float: left;
            position: fixed;
            bottom: 25px;
            margin-top: 0.0em;
            margin-left: 0.0em;
          }
          .parameter-label {
            float: left;
            margin-top: 0.25em;
            margin-left: 0.0em;
          }
          .parameter-label-r {
            float: left;
            margin-top: 0.15em;
            margin-left: 0.0em;
          }
          .parameter-label-s {
            float: left;
            margin-top: 0.05em;
            margin-left: 0.0em;
          }
          .parameter-label-headline {
            float: left;
            margin-left: 0.0em;
            color: #EEEEEE;
            font-weight: bolder;
          }
          .parameter {
            margin-left: 10.5em;
          }
          .parameter-headline {
            margin-left: 10.5em;
            color: #EEEEEE;
          }
        </style>
        <link rel="icon" href="data:,">
        <title>BINARY CLOCK</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <script>
          var Validation;
          var iMode;
          function readHeader() {
            var req = new XMLHttpRequest();req.open('GET', document.location, false);
            req.send(null);
            Validation = req.getResponseHeader('Validation');
            iMode = parseInt(req.getResponseHeader('Mode'));
          }
          readHeader();
          if (Validation == "valid") {
            var meta = document.createElement('meta');
            meta.httpEquiv = "refresh";
            meta.content = "5; url=/";
            document.getElementsByTagName('head')[0].appendChild(meta);
          }
        </script>
      </head>
      <body>
        <p class="text">
          <div class="parameter-label-headline">
            binary clock
          </div>
          <br><br>
          <form class="text">
            <div class="div-left">
              <button type="button" class="button" onclick="window.location.href='/alarm-r';">repeated alarm</button>
            </div>
            <div class="div-right">
              <button type="button" class="button" onclick="window.location.href='/display';">display</button>
            </div>
          </form>
          <br>
          <form class="text">
            <div class="div-left">
              <button type="button" class="button" onclick="window.location.href='/alarm-s';">single alarm</button>
            </div>
          </form>
          <br>
          <div id ="feedback" class="feedback">
            <script>
              var feedbackText;
              if (Validation == "valid") {
                switch (iMode) {
                  case 1:
                    feedbackText = "settings saved"
                    break;
                  case 2:
                    feedbackText = "settings displayed"
                    break;
                  case 3:
                    feedbackText = "settings displayed"
                    break;
                  case 4:
                    feedbackText = "settings displayed"
                    break;
                  case 5:
                    feedbackText = "settings displayed"
                    break;
                  case 8:
                    feedbackText = "settings saved"
                    break;
                  case 9:
                    feedbackText = "settings saved"
                    break;
                }
                document.write(feedbackText);
              }
              else if (Validation == "invalid") {
                document.getElementById("feedback").style.color = "#FF0000"
                document.write("invalid parameters");
              }
            </script>
          </div>
          <form class="text">
            <div class="div-bottom">
              <button type="button" class="button" onclick="window.location.href='/settings';">settings</button>
            </div>
          </form>
        </p>
      </body>
    </html>
  )rawliteral";

const char requestWebSettings[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
    <html>
      <head>
        <style>
          body {
            margin-top: 25px;
            margin-bottom: 25px;
            margin-left: 25px;
            margin-right: 25px;
            background-color: #333333;
            font-family: Helvetica;
            color: #AAAAAA;
          }
          .text {
            font-family: Helvetica;
            color: #AAAAAA;
          }
          .feedback {
            margin-top: 2.0em;
            color: #009090;
          }
          .form-r {
            font-family: Helvetica;
            color: #AAAAAA;
            line-height: 1.0em;
          }
          .form-s{ 
            font-family: Helvetica;
            color: #AAAAAA;
            line-height: 1.2em;
          }
          .mode-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #AAAAAA;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .mode-select:checked {
            background-color: #333333;
          }
          .button {
            background-color: #AAAAAA;
            color: #333333;
            font-weight: bold;
            width: 10.5em;
            border-radius: 10%;
          }
          .div-left {
            float: left;
            margin-top: 0.0em;
            margin-left: 0.0em;
          }
          .div-right {
            margin-top: 0.25em;
            margin-left: 10.5em;
          }
          .parameter-label {
            float: left;
            margin-top: 0.25em;
            margin-left: 0.0em;
          }
          .parameter-label-r {
            float: left;
            margin-top: 0.15em;
            margin-left: 0.0em;
          }
          .parameter-label-s {
            float: left;
            margin-top: 0.05em;
            margin-left: 0.0em;
          }
          .parameter-label-headline {
            float: left;
            margin-left: 0.0em;
            color: #EEEEEE;
            font-weight: bolder;
          }
          .parameter {
            margin-left: 10.5em;
          }
          .parameter-headline {
            margin-left: 10.5em;
            color: #EEEEEE;
          }
        </style>
        <link rel="icon" href="data:,">
        <title>BINARY CLOCK [settings]</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <script>
          var Validation;
          var iMode;
          var Day;
          var Night;
          var Dark;
          var req = new XMLHttpRequest();req.open('GET', document.location, false);
          req.send(null);
          Validation = req.getResponseHeader('Validation');
          iMode = parseInt(req.getResponseHeader('Mode'));
          iDay = parseInt(req.getResponseHeader('Day'));
          iNight = parseInt(req.getResponseHeader('Night'));
          iDark = parseInt(req.getResponseHeader('Dark'));
        </script>
      </head>
      <body>
        <p>
          <div class="parameter-label-headline">
            settings
          </div>
          <div class="parameter-headline">
            day/night / bright/dark
          </div>
          <br><br>
          <form id="mode-selection" class="form-s">
            <div class="parameter-label-s">
              <label for="mode-selection">day/night</label>
            </div>
            <div class="parameter">
              <input type="radio" id="day" name="daynight" class="mode-select">
              <label for="day">day</label><br>
              <input type="radio" id="night" name="daynight" class="mode-select">
              <label for="night">night</label><br>
            </div>
          </form>
          <br>
          <form id="mode-selection" class="form-s">
            <div class="parameter-label-s">
              <label for="mode-selection">bright/dark</label>
            </div>
            <div class="parameter">
              <input type="radio" id="bright" name="brightdark" class="mode-select">
              <label for="bright">bright</label><br>
              <input type="radio" id="dark" name="brightdark" class="mode-select">
              <label for="dark">dark</label><br>
            </div>
          </form>
          <br>
          <br>
          <br>
          <form class="text">
            <div class="div-left">
              <button type="button" id="discard" onclick="window.location.href='/'" class="button">discard settings</button>
            </div>
            <div class="div-right">
              <button type="button" id="save" onclick="saveSettings();" class="button">save settings</button>
            </div>
          </form>
           <div id ="feedback" class="feedback">
            <span id="feedbackShow"></span>
          </div>
        <script>
          if (iDay == 1) {
            document.getElementById("day").checked = true;
          }
          if (iNight == 1) {
            document.getElementById("night").checked = true;
          }
          if (iDark == 0) {
            document.getElementById("bright").checked = true;
          }
          if (iDark == 1) {
            document.getElementById("dark").checked = true;
          }
          function saveSettings() {
            document.getElementById("feedbackShow").innerHTML = "saving settings...";
            let setDay = document.getElementById("day").checked;
            let setNight = document.getElementById("night").checked;
            let setBright = document.getElementById("bright").checked;
            let setDark = document.getElementById("dark").checked;
            var setDayNight;
            var setBrightDark;
            if (setDay && !setNight) {
              setDayNight = "/change?mode=6";
            }
            else if (!setDay && setNight) {
              setDayNight = "/change?mode=7";
            }
            if (setBright && !setDark) {
              setBrightDark = "/change?mode=10";
            }
            else if (!setBright && setDark) {
              setBrightDark = "/change?mode=11";
            }
            var changeDayNight = iNight != setNight;
            var changeBrightDark = iDark != setDark;
            async function DayNight() {
              await fetch(setDayNight)
              .then(res => {return res.text(); })
              .catch(status, err => {return console.log(status, err);})
            }
            async function BrightDark() {
              await fetch(setBrightDark)
              .then(res => {return res.text(); })
              .catch(status, err => {return console.log(status, err);})
            }
            function rootPage() {
              window.open("/change?mode=1", "_self");
            }
            var delay = 0;
            if (!iDark) {
              if (changeDayNight) {
                DayNight();
                delay = delay + 3500;
              }
              if (changeBrightDark) {
                setTimeout(BrightDark, delay);
              }
              else {
                delay = 0;
              }
            }
            if (iDark) {
              if (changeBrightDark) {
                BrightDark();
                delay = delay + 3500;
              }
              if (changeDayNight) {
                setTimeout(DayNight, delay);
              }
              else {
                delay = 0;
              }
            }
            setTimeout(rootPage, delay + 1500);
          }
        </script>
      </body>
    </html>
  )rawliteral";

const char requestWebDisplay[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
    <html>
      <head>
        <style>
          body {
            margin-top: 25px;
            margin-bottom: 25px;
            margin-left: 25px;
            margin-right: 25px;
            background-color: #333333;
            font-family: Helvetica;
            color: #AAAAAA;
          }
          .text {
            font-family: Helvetica;
            color: #AAAAAA;
          }
          .form-r {
            font-family: Helvetica;
            color: #AAAAAA;
            line-height: 1.0em;
          }
          .form-s{ 
            font-family: Helvetica;
            color: #AAAAAA;
            line-height: 1.2em;
          }
          .display-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #AAAAAA;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .display-select:checked {
            background-color: #333333;
          }
          .color1-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #FF0000;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .color1-select:checked {
            background-color: #333333;
          }
          .color2-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #00FF00;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .color2-select:checked {
            background-color: #333333;
          }
          .color3-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #0000FF;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .color3-select:checked {
            background-color: #333333;
          }
          .color4-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #FBA900;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .color4-select:checked {
            background-color: #333333;
          }
          .color5-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #C90076;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .color5-select:checked {
            background-color: #333333;
          }
          .input-box {
            background-color: #AAAAAA;
            color: #333333;
            font-weight: bold;
            width: 5.25em;
            border-radius: 10%;
          }
          .button {
            background-color: #AAAAAA;
            color: #333333;
            font-weight: bold;
            width: 10.5em;
            border-radius: 10%;
          }
          .div-left {
            float: left;
            margin-top: 0.0em;
            margin-left: 0.0em;
          }
          .div-right {
            margin-top: 0.25em;
            margin-left: 10.5em;
          }
          .parameter-label {
            float: left;
            margin-top: 0.25em;
            margin-left: 0.0em;
          }
          .parameter-label-r {
            float: left;
            margin-top: 0.15em;
            margin-left: 0.0em;
          }
          .parameter-label-s {
            float: left;
            margin-top: 0.05em;
            margin-left: 0.0em;
          }
          .parameter-label-headline {
            float: left;
            margin-left: 0.0em;
            color: #EEEEEE;
            font-weight: bolder;
          }
          .parameter {
            margin-left: 10.5em;
          }
          .parameter-headline {
            margin-left: 10.5em;
            color: #EEEEEE;
          }
        </style>
        <link rel="icon" href="data:,">
        <title>BINARY CLOCK [display settings]</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <script>
          var Validation;
          var iMode;
          var req = new XMLHttpRequest();req.open('GET', document.location, false);
          req.send(null);
          Validation = req.getResponseHeader('Validation');
          iMode = parseInt(req.getResponseHeader('Mode'));
        </script>
      </head>
      <body>
        <p>
          <div class="parameter-label-headline">
            display settings
          </div>
          <div class="parameter-headline">
            week day / date / numbers / cycle
          </div>
          <br><br>
          <form class="text">
            <div class="parameter-label">
              <label for="time">left/right side</label>
            </div>
            <div class="parameter">
              <input type="number" id="numberLeft" name="numberLeft" min="0" max="31" class="input-box" onfocus="resetInput('numberLeft')">
              &nbsp;
              <input type="number" id="numberRight" name="numberRight" min="0" max="63" class="input-box" onfocus="resetInput('numberRight')">
            </div>
          </form>
          <br>
          <form class="text">
            <div class="parameter-label">
              <label for="dur">display duration (sec)</label>
            </div>
            <div class="parameter">
              <input type="number" id="duration" name="duration" min="0" class="input-box" onfocus="resetInput('duration')">
            </div>
          </form>
          <br>
          <form class="text">
            <div class="parameter-label">
              <label for="color">display color</label>
            </div>
            <div class="parameter">
              <input type="text" id="color" name="color" value="" maxlength="6" class="input-box" onfocus="resetInput('color');">
              &nbsp;
              <input type="radio" id="color1" name="color" class="color1-select" onclick="colorButtonClick('FF0000');">
              <input type="radio" id="color2" name="color" class="color2-select" onclick="colorButtonClick('00FF00');">
              <input type="radio" id="color3" name="color" class="color3-select" onclick="colorButtonClick('0000FF');">
              <input type="radio" id="color4" name="color" class="color4-select" onclick="colorButtonClick('FF1F00');">
              <input type="radio" id="color5" name="color" class="color5-select" onclick="colorButtonClick('FF005F');">
            </div>
          </form>
          <br>
          <form id="display-selection" class="form-s">
            <div class="parameter-label-s">
              <label for="display-selection">display mode</label>
            </div>
            <div class="parameter">
              <input type="radio" id="displaymode-weekday" name="display" class="display-select" onfocus="resetInput('displaymode');" onclick="durationMinValue(5); numbersDisable(true); colorDisable(true);">
              <label for="displaymode-weekday">week day</label><br>
              <input type="radio" id="displaymode-date" name="display" class="display-select" onfocus="resetInput('displaymode');" onclick="durationMinValue(5); numbersDisable(true); colorDisable(true);">
              <label for="displaymode-date">date</label><br>
              <input type="radio" id="displaymode-numbers" name="display" class="display-select" onfocus="resetInput('displaymode');" onclick="durationMinValue(5); numbersDisable(false); colorDisable(false);">
              <label for="displaymode-numbers">numbers</label><br>
              <input type="radio" id="displaymode-cycle" name="display" class="display-select" onfocus="resetInput('displaymode');" onclick="durationMinValue(7); numbersDisable(true); colorDisable(false);">
              <label for="displaymode-cycle">cycle</label>
            </div>
          </form>
          <br>
          <br>
          <br>
          <form class="text">
            <div class="div-left">
              <button type="button" id="discard" onclick="window.location.href='/'" class="button">discard settings</button>
            </div>
            <div class="div-right">
              <button type="button" id="save" onclick="valueValidation();" class="button">display settings</button>
            </div>
          </form>
        <script>
          function colorButtonClick(chosenColor) {
            document.getElementsByName("color")[0].value=chosenColor;
          }
          function durationMinValue(durationMin) {
            document.getElementById("duration").min=durationMin;
            let setDuration = document.getElementById("duration").value;
            if (setDuration < durationMin) {
              document.getElementById("duration").value=durationMin;
            }
          }
          function numbersDisable(disable) {
            document.getElementById("numberLeft").disabled = disable;
            document.getElementById("numberRight").disabled = disable;
            if (disable) {
              document.getElementsByName("numberLeft")[0].value = "";
              document.getElementsByName("numberRight")[0].value = "";
            }
          }
          function colorDisable(disable) {
            document.getElementById("color").disabled = disable;
            if (disable) {
              document.getElementsByName("color")[0].value = "";
            }
          }
          function valueValidation() {
            let numberLeftValidation = false;
            let numberRightValidation = false;
            let durationValidation = false;
            let colorValidation = false;
            let modeValidation = false;
            let numberLeft = document.getElementById("numberLeft").value;
            if ((numberLeft >= 0 && numberLeft <= 31) || numberLeft == "") {
              numberLeftValidation = true;
            }
            else {
              document.getElementById("numberLeft").style.borderColor = "#FF0000";
            }
            let numberRight = document.getElementById("numberRight").value;
            if ((numberRight >= 0 && numberRight <= 63) || numberRight == "") {
              numberRightValidation = true;
            }
            else {
              document.getElementById("numberRight").style.borderColor = "#FF0000";
            }
            let setDisplayMode1 = document.getElementById("displaymode-weekday").checked;
            let setDisplayMode2 = document.getElementById("displaymode-date").checked;
            let setDisplayMode3 = document.getElementById("displaymode-numbers").checked;
            let setDisplayMode4 = document.getElementById("displaymode-cycle").checked;
            let durationMin;
            if (setDisplayMode1) {durationMin = 5;}
            if (setDisplayMode2) {durationMin = 5;}
            if (setDisplayMode3) {durationMin = 5;}
            if (setDisplayMode4) {durationMin = 7;}
            let setDuration = document.getElementById("duration").value;
            if (setDuration >= durationMin || setDuration == "") {
              durationValidation = true;
            }
            else {
              document.getElementById("duration").style.borderColor = "#FF0000";
            }
            let regex = /[0-9A-Fa-f]{6}/gm;
            let setColor = document.getElementById("color").value;
            if (setColor.length == 0 || (setColor.length == 6 && setColor.match(regex))) {
              colorValidation = true;
            }
            else {
              document.getElementById("color").style.borderColor = "#FF0000";
            }
            if (setDisplayMode1 || setDisplayMode2 || setDisplayMode3 || setDisplayMode4) {
              modeValidation = true;
            }
            else {
              document.getElementById("displaymode-weekday").style.borderColor = "#FF0000";
              document.getElementById("displaymode-date").style.borderColor = "#FF0000";
              document.getElementById("displaymode-numbers").style.borderColor = "#FF0000";
              document.getElementById("displaymode-cycle").style.borderColor = "#FF0000";
            }
            if (numberLeftValidation && numberRightValidation && durationValidation && colorValidation && modeValidation) {
              saveSettings();
            }
          }
          function resetInput(input) {
            if (input == "displaymode") {
              document.getElementById("displaymode-weekday").style.borderColor = "";
              document.getElementById("displaymode-date").style.borderColor = "";
              document.getElementById("displaymode-numbers").style.borderColor = "";
              document.getElementById("displaymode-cycle").style.borderColor = "";
            }
            else {
              document.getElementById(input).style.borderColor = "";
            }
          }
          function saveSettings() {
            let setNumberLeft = document.getElementById("numberLeft").value;
            let setNumberRight = document.getElementById("numberRight").value;
            let setDuration = document.getElementById("duration").value;
            let setColor = document.getElementById("color").value.toUpperCase();
            let setDisplayMode1 = document.getElementById("displaymode-weekday").checked;
            let setDisplayMode2 = document.getElementById("displaymode-date").checked;
            let setDisplayMode3 = document.getElementById("displaymode-numbers").checked;
            let setDisplayMode4 = document.getElementById("displaymode-cycle").checked;
            let setDisplay;
            if (setDisplayMode1) {setDisplay = 2;}
            if (setDisplayMode2) {setDisplay = 3;}
            if (setDisplayMode3) {setDisplay = 4;}
            if (setDisplayMode4) {setDisplay = 5;}
            let provedNumberLeft = "";
            let provedNumberRight = "";
            let provedDuration = "";
            let provedColor = "";
            if (setNumberLeft != "") {provedNumberLeft = "&hour="+setNumberLeft;}
            if (setNumberRight != "") {provedNumberRight = "&minute="+setNumberRight;}
            if (setDuration != "") {provedDuration = "&time="+setDuration;}
            if (setColor != "") {provedColor = "&color="+setColor;}
            let url=`/change?mode=${setDisplay}${provedNumberLeft}${provedNumberRight}${provedDuration}${provedColor}`;
            window.open(url, "_self");
          }
        </script>
      </body>
    </html>
  )rawliteral";

const char requestWebAlarm[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
    <html>
      <head>
        <style>
          body {
            margin-top: 25px;
            margin-bottom: 25px;
            margin-left: 25px;
            margin-right: 25px;
            background-color: #333333;
            font-family: Helvetica;
            color: #AAAAAA;
          }
          .text {
            font-family: Helvetica;
            color: #AAAAAA;
          }
          .form-r {
            font-family: Helvetica;
            color: #AAAAAA;
            line-height: 1.0em;
          }
          .form-s{ 
            font-family: Helvetica;
            color: #AAAAAA;
            line-height: 1.2em;
          }
          .week-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #AAAAAA;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .week-select:checked {
            background-color: #333333;
          }
          .alarm-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #AAAAAA;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .alarm-select:checked {
            background-color: #333333;
          }
          .color1-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #FF0000;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .color1-select:checked {
            background-color: #333333;
          }
          .color2-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #00FF00;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .color2-select:checked {
            background-color: #333333;
          }
          .color3-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #0000FF;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .color3-select:checked {
            background-color: #333333;
          }
          .color4-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #FBA900;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .color4-select:checked {
            background-color: #333333;
          }
          .color5-select {
            width: 1.0em;
            height: 1.0em;
            margin-left: 0.0em;
            margin-bottom: 0.25em;
            background-color: #AAAAAA;
            border: 0.15em solid #C90076;
            border-radius: 35%;
            vertical-align: middle;
            appearance: none;
            -webkit-appearance: none;
            outline: none;
            cursor: pointer;
          }
          .color5-select:checked {
            background-color: #333333;
          }
          .input-box {
            background-color: #AAAAAA;
            color: #333333;
            font-weight: bold;
            width: 5.25em;
            border-radius: 10%;
          }
          .button {
            background-color: #AAAAAA;
            color: #333333;
            font-weight: bold;
            width: 10.5em;
            border-radius: 10%;
          }
          .div-left {
            float: left;
            margin-top: 0.0em;
            margin-left: 0.0em;
          }
          .div-right {
            margin-top: 0.25em;
            margin-left: 10.5em;
          }
          .parameter-label {
            float: left;
            margin-top: 0.25em;
            margin-left: 0.0em;
          }
          .parameter-label-r {
            float: left;
            margin-top: 0.15em;
            margin-left: 0.0em;
          }
          .parameter-label-s {
            float: left;
            margin-top: 0.05em;
            margin-left: 0.0em;
          }
          .parameter-label-headline {
            float: left;
            margin-left: 0.0em;
            color: #EEEEEE;
            font-weight: bolder;
          }
          .parameter {
            margin-left: 10.5em;
          }
          .parameter-headline {
            margin-left: 10.5em;
            color: #EEEEEE;
          }
        </style>
        <link rel="icon" href="data:,">
        <title>BINARY CLOCK [alarm settings]</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <script>
          var Validation;
          var iMode;
          var Hour;
          var Minute;
          var iWeek1;
          var iWeek2;
          var iWeek3;
          var iWeek4;
          var iWeek5;
          var iWeek6;
          var iWeek7;
          var Color;
          var Time;
          var iAlarm;
          var req = new XMLHttpRequest();req.open('GET', document.location, false);
          req.send(null);
          Validation = req.getResponseHeader('Validation');
          iMode = parseInt(req.getResponseHeader('Mode'));
          Hour = req.getResponseHeader('Hour');
          Minute = req.getResponseHeader('Minute');
          iWeek1 = parseInt((req.getResponseHeader('Week')).substring(0,1));
          iWeek2 = parseInt((req.getResponseHeader('Week')).substring(1,2));
          iWeek3 = parseInt((req.getResponseHeader('Week')).substring(2,3));
          iWeek4 = parseInt((req.getResponseHeader('Week')).substring(3,4));
          iWeek5 = parseInt((req.getResponseHeader('Week')).substring(4,5));
          iWeek6 = parseInt((req.getResponseHeader('Week')).substring(5,6));
          iWeek7 = parseInt((req.getResponseHeader('Week')).substring(6,7));
          Color = req.getResponseHeader('Color');
          Time = req.getResponseHeader('Time');
          iAlarm = parseInt(req.getResponseHeader('Alarm'));
        </script>
      </head>
      <body>
        <p>
          <div class="parameter-label-headline">
            alarm settings
          </div>
          <div class="parameter-headline">
            <script>
              if (iMode == 8) {
                document.write("repeated alarm");
              }
              else if (iMode == 9) {
                document.write("single alarm");
              }
            </script>
          </div>
          <br><br>
          <form class="text">
            <div class="parameter-label">
              <label for="time">alarm time</label>
            </div>
            <div class="parameter">
              <input type="time" id="time" name="time" class="input-box">
            </div>
          </form>
          <br>
          <form id="week-selection" class="form-r">
            <div id="week-label" class="parameter-label-r">
              <label for="week-selection">week days</label>
            </div>
            <div class="parameter">
              <input type="checkbox" id="monday" name="week" class="week-select">
              <label for="monday">monday</label><br>
              <input type="checkbox" id="tuesday" name="week" class="week-select">
              <label for="tuesday">tuesday</label><br>
              <input type="checkbox" id="wednesday" name="week" class="week-select">
              <label for="wednesday">wednesday</label><br>
              <input type="checkbox" id="thursday" name="week" class="week-select">
              <label for="thursday">thursday</label><br>
              <input type="checkbox" id="friday" name="week" class="week-select">
              <label for="friday">friday</label><br>
              <input type="checkbox" id="saturday" name="week" class="week-select">
              <label for="saturday">saturday</label><br>
              <input type="checkbox" id="sunday" name="week" class="week-select">
              <label for="sunday">sunday</label>
            </div>
          </form>
          <br>
          <form class="text">
            <div class="parameter-label">
              <label for="dur">alarm duration (sec)</label>
            </div>
            <div class="parameter">
              <input type="number" id="duration" name="duration" min="0" class="input-box" onfocus="resetInput('duration');">
            </div>
          </form>
          <br>
          <form class="text">
            <div class="parameter-label">
              <label for="color">alarm color</label>
            </div>
            <div class="parameter">
              <input type="text" id="color" name="color" value="" maxlength="6" class="input-box" onfocus="resetInput('color');">
              &nbsp;
              <input type="radio" id="color1" name="color" class="color1-select" onclick="colorButtonClick('FF0000');">
              <input type="radio" id="color2" name="color" class="color2-select" onclick="colorButtonClick('00FF00');">
              <input type="radio" id="color3" name="color" class="color3-select" onclick="colorButtonClick('0000FF');">
              <input type="radio" id="color4" name="color" class="color4-select" onclick="colorButtonClick('FF1F00');">
              <input type="radio" id="color5" name="color" class="color5-select" onclick="colorButtonClick('FF005F');">
            </div>
          </form>
          <br>
          <form id="alarm-selection" class="form-s">
            <div class="parameter-label-s">
              <label for="alarm-selection">alarm mode</label>
            </div>
            <div class="parameter">
              <input type="radio" id="alarmmode-off" name="alarm" class="alarm-select" onclick="durationMinValue(0);">
              <label for="alarmmode-off">off</label><br>
              <input type="radio" id="alarmmode-time" name="alarm" class="alarm-select" onclick="durationMinValue(5);">
              <label for="alarmmode-time">time</label><br>
              <input type="radio" id="alarmmode-cycle" name="alarm" class="alarm-select" onclick="durationMinValue(7);">
              <label for="alarmmode-cycle">cycle</label>
            </div>
          </form>
          <br>
          <br>
          <br>
          <form class="text">
            <div class="div-left">
              <button type="button" id="discard" onclick="window.location.href='/'" class="button">discard settings</button>
            </div>
            <div class="div-right">
              <button type="button" id="save" onclick="valueValidation();" class="button">save settings</button>
            </div>
          </form>
        <script>
          if (iMode == 9) {
            document.getElementById("monday").type = "radio";
            document.getElementById("tuesday").type = "radio";
            document.getElementById("wednesday").type = "radio";
            document.getElementById("thursday").type = "radio";
            document.getElementById("friday").type = "radio";
            document.getElementById("saturday").type = "radio";
            document.getElementById("sunday").type = "radio";
            document.getElementById("week-selection").className = "form-s";
            document.getElementById("week-label").className = "parameter-label-s";
          }
          if (iWeek1 > 0) {document.getElementById("monday").checked = true;}
          if (iWeek2 > 0) {document.getElementById("tuesday").checked = true;}
          if (iWeek3 > 0) {document.getElementById("wednesday").checked = true;}
          if (iWeek4 > 0) {document.getElementById("thursday").checked = true;}
          if (iWeek5 > 0) {document.getElementById("friday").checked = true;}
          if (iWeek6 > 0) {document.getElementById("saturday").checked = true;}
          if (iWeek7 > 0) {document.getElementById("sunday").checked = true;}
          let timeGet = Hour+":"+Minute;
          document.getElementsByName("time")[0].value=timeGet;
          document.getElementsByName("duration")[0].value=Time;
          document.getElementsByName("color")[0].value=Color.toUpperCase();
          if (iAlarm == 0) {
            document.getElementById("alarmmode-off").checked = true;
            durationMinValue(0);
          }
          if (iAlarm == 1) {
            document.getElementById("alarmmode-time").checked = true;
            durationMinValue(5);
          }
          if (iAlarm == 2) {
            document.getElementById("alarmmode-cycle").checked = true;
            durationMinValue(7);
          }
          function colorButtonClick(chosenColor) {
            document.getElementsByName("color")[0].value=chosenColor;
          }
          function durationMinValue(durationMin) {
            document.getElementById("duration").min=durationMin;
            let setDuration = parseInt(document.getElementById("duration").value);
            if (setDuration < durationMin) {
              document.getElementById("duration").value=durationMin;
            }
          }
          function valueValidation() {
            let durationValidation = false;
            let colorValidation = false;
            let setAlarmMode0 = document.getElementById("alarmmode-off").checked;
            let setAlarmMode1 = document.getElementById("alarmmode-time").checked;
            let setAlarmMode2 = document.getElementById("alarmmode-cycle").checked;
            let durationMin;
            if (setAlarmMode0) {durationMin = 0;}
            if (setAlarmMode1) {durationMin = 5;}
            if (setAlarmMode2) {durationMin = 7;}
            let setDuration = document.getElementById("duration").value;
            if (setDuration >= durationMin || setDuration == "") {
              durationValidation = true;
            }
            else {
              document.getElementById("duration").style.borderColor = "#FF0000";
            }
            let regex = /[0-9A-Fa-f]{6}/gm;
            let setColor = document.getElementById("color").value;
            if (setColor.length == 0 || (setColor.length == 6 && setColor.match(regex))) {
              colorValidation = true;
            }
            else {
              document.getElementById("color").style.borderColor = "#FF0000";
            }
            if (durationValidation && colorValidation) {
              saveSettings();
            }
          }
          function resetInput(input) {
            document.getElementById(input).style.borderColor = "";
          }
          function saveSettings() {
            let setHour = (document.getElementById("time").value).substring(0,2);
            let setMinute = (document.getElementById("time").value).substring(3,5);
            let setMonday = document.getElementById("monday").checked;
            let setTuesday = document.getElementById("tuesday").checked;
            let setWednesday = document.getElementById("wednesday").checked;
            let setThursday = document.getElementById("thursday").checked;
            let setFriday = document.getElementById("friday").checked;
            let setSaturday = document.getElementById("saturday").checked;
            let setSunday = document.getElementById("sunday").checked;
            let setWeek1;
            let setWeek2;
            let setWeek3;
            let setWeek4;
            let setWeek5;
            let setWeek6;
            let setWeek7;
            if (setMonday) {setWeek1 = "1";} else {setWeek1 = "0"}
            if (setTuesday) {setWeek2 = "1";} else {setWeek2 = "0"}
            if (setWednesday) {setWeek3 = "1";} else {setWeek3 = "0"}
            if (setThursday) {setWeek4 = "1";} else {setWeek4 = "0"}
            if (setFriday) {setWeek5 = "1";} else {setWeek5 = "0"}
            if (setSaturday) {setWeek6 = "1";} else {setWeek6 = "0"}
            if (setSunday) {setWeek7 = "1";} else {setWeek7 = "0"}
            let setWeek = setWeek1+setWeek2+setWeek3+setWeek4+setWeek5+setWeek6+setWeek7;
            let setDuration = document.getElementById("duration").value;
            let setColor = document.getElementById("color").value.toUpperCase();
            let setAlarmMode0 = document.getElementById("alarmmode-off").checked;
            let setAlarmMode1 = document.getElementById("alarmmode-time").checked;
            let setAlarmMode2 = document.getElementById("alarmmode-cycle").checked;
            let setAlarm;
            if (setAlarmMode0) {setAlarm = 0;}
            if (setAlarmMode1) {setAlarm = 1;}
            if (setAlarmMode2) {setAlarm = 2;}
            let provedTime = "";
            let provedDuration = "";
            let provedColor = "";
            if (setHour != "" && setMinute != "") {provedTime = "&hour="+setHour+"&minute="+setMinute;}
            if (setDuration != "") {provedDuration = "&time="+setDuration;}
            if (setColor != "") {provedColor = "&color="+setColor;}
            let url=`/change?mode=${iMode}${provedTime}&week=${setWeek}${provedDuration}${provedColor}&alarm=${setAlarm}`;
            window.open(url, "_self");
          }
        </script>
      </body>
    </html>
  )rawliteral";

#endif  // _WEBINTERFACE_H