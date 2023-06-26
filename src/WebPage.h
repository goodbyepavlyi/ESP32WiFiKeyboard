static const char PROGMEM WEBPAGE[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>USB Touch keyboard</title>

    <style>
      body {
        background: #1c1c28;
        font-family: 'Roboto', sans-serif;
        
        display: flex;
        align-items: center;
        justify-content: center;
        min-height: 100vh;
      }

      #loading-screen {
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        backdrop-filter: blur(5px); /* Add blur effect */
        background-color: rgba(0, 0, 0, 0.5); /* Add background color with transparency */
        display: flex;
        align-items: center;
        justify-content: center;
        z-index: 9999;
        transition: opacity 0.5s ease;
      }
      
      .loader-container {
        display: flex;
        flex-direction: column;
        align-items: center;
      }

      .loader {
        border: 16px solid #f3f3f3;
        border-top: 16px solid #3498db;
        border-radius: 50%;
        width: 120px;
        height: 120px;
        animation: spin 2s linear infinite;
      }

      .loading-text {
        color: #fff;
        margin-top: 20px;
        font-size: 24px;
        font-weight: 500;
      }

      @keyframes spin {
        0% { transform: rotate(0deg); }
        100% { transform: rotate(360deg); }
      }


      .row {
        width: 100%;
        margin: 10px 0 0 0;
      }

      button {
        width: 55px;
        height: 55px;
        margin: 0 5px;
        border-radius: 5%;

        background: #393646;
        color: white;
        box-shadow: 5px 5px 8px #0b0a0e;
        border: none;
        outline: none;
        
        font-weight: 500;
        font-size: 15px;
      }

      button:active,
      button.active {
        background: #454157;
      }

      button#PrintScr {
        width: 55px;
      }

      button#Tab,
      button#CapsLock,
      button#Backspace,
      button#Enter,
      button#ShiftRight,
      button#ShiftLeft {
        width: 125px;
      }

      button#Space {
        width: 400px;
        border-radius: 7.5px;
      }
    </style>
    
    <script>
      /** @type {WebSocket} */
      let socket;

      function connectWebSocket() {
        socket = new WebSocket(`ws://10.0.198.12:81`);
        // const socket = new WebSocket(`ws://${window.location.hostname}:81`);
        console.log(`[WebSocket] Connecting to ws://${window.location.hostname}:81..`);

        socket.onmessage = (event) => {
          console.log(`[WebSocket] Received message: ${event.data}`);
        };

        socket.onopen = (event) => {
          console.log('[WebSocket] Connected to WebSocket');
          document.getElementById("loading-screen").style.display = "none";
        };

        socket.onclose = (event) => {
          document.getElementById("loading-screen").style.display = "flex";
          console.log(`[WebSocket] Disconnected from WebSocket, Code: ${event.code}, Reason: ${event.reason}`);
          console.log(`[WebSocket] Reconnecting..`);

          setTimeout(connectWebSocket, 2500);
        };
      }

      function sendMessage(data) {
        if (socket.readyState != socket.OPEN)
          return console.log(`[WebSocket] The connection has been terminated. Unable to send the message.`);

        console.log(`[WebSocket] Sending message: ${data}`);
        socket.send(data);
      }

      function sendKeyPress(key) {
        if (!key)
          return console.log("[Key press] The function was called, but no key was passed!");
        
        console.log(`[Keyboard] Key "${key}" pressed`);

        sendMessage(JSON.stringify({
          event: 'KeyPress',
          key,
        }));
      }

      function sendKeyRelease(key) {
        if (!key)
          return console.log("[Key release] The function was called, but no key was passed!");
        
        console.log(`[Keyboard] Key "${key}" released`);

        sendMessage(JSON.stringify({
          event: 'KeyRelease',
          key,
        }));
      }
      
      document.addEventListener("DOMContentLoaded", () => {
        document.addEventListener("keyup", (event) => {
          event.preventDefault();
          
          const keyElement = document.querySelectorAll(`[keyValue="${event.key}"]`);
          if (keyElement)
            keyElement.forEach(element => element.classList.remove("active"));

          const key = getKeyName(event.key);
          sendKeyRelease(key);
        });

        document.addEventListener("keydown", (event) => {
          event.preventDefault();

          const keyElement = document.querySelectorAll(`[keyValue="${CSS.escape(event.key)}"]`);
          if (keyElement)
            keyElement.forEach(element => element.classList.add("active"));

          const key = getKeyName(event.key);
          sendKeyPress(key);
        });
        
        document.querySelectorAll("button").forEach((button) => {
          function buttonDown(event) {
            event.preventDefault();

            const key = getKeyName(button.getAttribute("keyValue"));
            sendKeyPress(key);
          }
          
          function buttonUp(event) {
            event.preventDefault();

            const key = getKeyName(button.getAttribute("keyValue"));
            sendKeyRelease(key);
          }

          button.addEventListener("touchstart", buttonDown);
          button.addEventListener("mousedown", buttonDown);

          button.addEventListener("mouseup", buttonUp);
          button.addEventListener("touchend", buttonUp);
        });
      });

      function getKeyName(keyCode) {
        const keyMap = {
          "Fn": null, // Not supported in Arduino Keyboard library
          "FnLock": null, // Not supported in Arduino Keyboard library
          "Hyper": null, // Not supported in Arduino Keyboard library
          "NumLock": null, // Not supported in Arduino Keyboard library
          "ScrollLock": null, // Not supported in Arduino Keyboard library
          "Symbol": null, // Not supported in Arduino Keyboard library
          "SymbolLock": null, // Not supported in Arduino Keyboard library
          "Alt": "KEY_LEFT_ALT",
          "AltGraph": "KEY_RIGHT_ALT",
          "CapsLock": "KEY_CAPS_LOCK",
          "Control": "KEY_LEFT_CTRL",
          "Meta": "KEY_LEFT_GUI",
          "Shift": "KEY_LEFT_SHIFT",
          "Super": "KEY_LEFT_GUI",
          "Tab": "KEY_TAB",
          "Backspace": "KEY_BACKSPACE",
          "Enter": "KEY_RETURN",
          "Insert": "KEY_INSERT",
          "Delete": "KEY_DELETE",
          "Home": "KEY_HOME",
          "End": "KEY_END",
          "PageUp": "KEY_PAGE_UP",
          "PageDown": "KEY_PAGE_DOWN",
          "ArrowUp": "KEY_UP_ARROW",
          "ArrowDown": "KEY_DOWN_ARROW",
          "ArrowLeft": "KEY_LEFT_ARROW",
          "ArrowRight": "KEY_RIGHT_ARROW",
          "NumLock": "KEY_NUM_LOCK",
          "Escape": "KEY_ESC",
          "F1": "KEY_F1",
          "F2": "KEY_F2",
          "F3": "KEY_F3",
          "F4": "KEY_F4",
          "F5": "KEY_F5",
          "F6": "KEY_F6",
          "F7": "KEY_F7",
          "F8": "KEY_F8",
          "F9": "KEY_F9",
          "F10": "KEY_F10",
          "F11": "KEY_F11",
          "F12": "KEY_F12",
          "F13": "KEY_F13",
          "F14": "KEY_F14",
          "F15": "KEY_F15",
          "F16": "KEY_F16",
          "F17": "KEY_F17",
          "F18": "KEY_F18",
          "F19": "KEY_F19",
          "F20": "KEY_F20",
          "F21": "KEY_F21",
          "F22": "KEY_F22",
          "F23": "KEY_F23",
          "F24": "KEY_F24",
        };

        return keyMap[keyCode] || keyCode;
      }

      function start() {
        connectWebSocket();
      }
    </script>
  </head>
  
  <body onload="start()">
    <div id="loading-screen">
      <div class="loader-container">
        <div class="loader"></div>
        <div class="loading-text">Connecting...</div>
      </div>
    </div>  
    
    <div class="container">
      <div class="row">
          <button id="Escape" keyValue="Escape">Esc</button>
          <button id="F1" keyValue="F1">F1</button>
          <button id="F2" keyValue="F2">F2</button>
          <button id="F3" keyValue="F3">F3</button>
          <button id="F4" keyValue="F4">F4</button>
          <button id="F5" keyValue="F5">F5</button>
          <button id="F6" keyValue="F6">F6</button>
          <button id="F7" keyValue="F7">F7</button>
          <button id="F8" keyValue="F8">F8</button>
          <button id="F9" keyValue="F9">F9</button>
          <button id="F10" keyValue="F10">F10</button>
          <button id="F11" keyValue="F11">F11</button>
          <button id="F12" keyValue="F12">F12</button>
          <button id="Insert" keyValue="Insert">Insert</button>
          <button id="PrintScr" keyValue="PrintScr">Print</button>
          <button id="Delete" keyValue="Delete">Delete</button>
      </div>

      <div class="row">
          <button id="Backtick" keyValue="`">`</button>
          <button id="Digit1" keyValue="1">1</button>
          <button id="Digit2" keyValue="2">2</button>
          <button id="Digit3" keyValue="3">3</button>
          <button id="Digit4" keyValue="4">4</button>
          <button id="Digit5" keyValue="5">5</button>
          <button id="Digit6" keyValue="6">6</button>
          <button id="Digit7" keyValue="7">7</button>
          <button id="Digit8" keyValue="8">8</button>
          <button id="Digit9" keyValue="9">9</button>
          <button id="Digit0" keyValue="0">0</button>
          <button id="Minus" keyValue="-">-</button>
          <button id="Equal" keyValue="=">=</button>
          <button id="Backspace" keyValue="Backspace">Backspace</button>
          <button id="Home" keyValue="Home">Home</button>
      </div>

      <div class="row">
          <button id="Tab" keyValue="Tab">Tab</button>
          <button id="q" keyValue="q">Q</button>
          <button id="w" keyValue="w">W</button>
          <button id="e" keyValue="e">E</button>
          <button id="r" keyValue="r">R</button>
          <button id="t" keyValue="t">T</button>
          <button id="y" keyValue="y">Y</button>
          <button id="u" keyValue="u">U</button>
          <button id="i" keyValue="i">I</button>
          <button id="o" keyValue="o">O</button>
          <button id="p" keyValue="p">P</button>
          <button id="SquareBracketLeft" keyValue="[">[</button>
          <button id="SquareBracketRight" keyValue="]">]</button>
          <button id="Backslash" keyValue="\">\</button>
          <button id="PageUp" keyValue="PageUp">Pg Up</button>
      </div>

      <div class="row">
          <button id="CapsLock" keyValue="CapsLock">Caps Lock</button>
          <button id="a" keyValue="a">A</button>
          <button id="s" keyValue="s">S</button>
          <button id="d" keyValue="d">D</button>
          <button id="f" keyValue="f">F</button>
          <button id="g" keyValue="g">G</button>
          <button id="h" keyValue="h">H</button>
          <button id="j" keyValue="j">J</button>
          <button id="k" keyValue="k">K</button>
          <button id="l" keyValue="l">L</button>
          <button id=";" keyValue=";">;</button>
          <button id="'" keyValue="'">'</button>
          <button id="Enter" keyValue="Enter">Enter</button>
          <button id="PageDown" keyValue="PageDown">Pg Dn</button>
      </div>

      <div class="row">
          <button id="ShiftLeft" keyValue="Shift">Shift</button>
          <button id="z" keyValue="z">Z</button>
          <button id="x" keyValue="x">X</button>
          <button id="c" keyValue="c">C</button>
          <button id="v" keyValue="v">V</button>
          <button id="b" keyValue="b">B</button>
          <button id="n" keyValue="n">N</button>
          <button id="m" keyValue="m">M</button>
          <button id="," keyValue=",">,</button>
          <button id="." keyValue=".">.</button>
          <button id="/" keyValue="/">/</button>
          <button id="ShiftRight" keyValue="Shift">Shift</button>
          <button id="ArrowUp" keyValue="ArrowUp">↑</button>
          <button id="End" keyValue="End">End</button>
      </div>

      <div class="row">
          <button id="ControlLeft" keyValue="Control">Ctrl</button>
          <button id="FunctionLeft" keyValue="Function">Fn</button>
          <button id="MetaLeft" keyValue="Meta">Win</button>
          <button id="AltLeft" keyValue="Alt">Alt</button>
          <button id="Space" keyValue=" ">Space</button>
          <button id="AltRight" keyValue="Alt">Alt</button>
          <button id="FunctionRight" keyValue="Function">Fn</button>
          <button id="ControlRight" keyValue="Control">Ctrl</button>
          <button id="ArrowLeft" keyValue="ArrowLeft">←</button>
          <button id="ArrowDown" keyValue="ArrowDown">↓</button>
          <button id="ArrowRight" keyValue="ArrowRight">→</button>
     </div>
  </body>
</html>
)rawliteral";

static const char PROGMEM NOT_FOUND[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="refresh" content="0;url=/">
    <title>USB Touch keyboard</title>
  </head>
  
  <body>
    <h1>Return to <a href="/">main page</a></h1>
  </body>
</html>
)rawliteral";