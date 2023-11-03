#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>


//The plain html and css is begin converted to c text using a website , visit this link for more info https://tomeko.net/online_tools/cpp_text_escape.php?lang=en 
const char index_html[] PROGMEM={"<!DOCTYPE html>\n<html lang=\"en\">\n\n<head>\n  <meta charset=\"UTF-8\">\n  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n  <title>Attendance Report</title>\n  <link href=\"style.css\" rel=\"stylesheet\">\n</head>\n\n<body>\n  <h1>Student Attendance System</h1>\n\n  <div class=\"p-2\">\n    <form\n      action=\"https://script.google.com/macros/s/AKfycbwaHvOfDPAN-4zaylXYtmajYEsHw3uzPRKQm12-8ijoEHQ_a9eyz5v2GyZCPpHIo8D2RQ/exec\"\n      method=\"POST\" class=\"gform\">\n      <div>\n        <label>Name:</label>\n        <input id=\"Name\" name=\"Name\" type=\"text\">\n      </div>\n      <div>\n        <label>RollNumber:</label>\n        <input id=\"RollNumber\" name=\"RollNumber\" type=\"text\">\n      </div>\n      <div>\n        <input id=\"submit\" type=\"submit\">\n      </div>\n    </form>\n    <hr>\n    <script>\n      (function () {\n        // get all data in form and return object\n        function getFormData(form) {\n          var elements = form.elements;\n          var honeypot;\n\n          var fields = Object.keys(elements).filter(function (k) {\n            if (elements[k].name === \"honeypot\") {\n              honeypot = elements[k].value;\n              return false;\n            }\n            return true;\n          }).map(function (k) {\n            if (elements[k].name !== undefined) {\n              return elements[k].name;\n              // special case for Edge's html collection\n            } else if (elements[k].length > 0) {\n              return elements[k].item(0).name;\n            }\n          }).filter(function (item, pos, self) {\n            return self.indexOf(item) == pos && item;\n          });\n\n          var formData = {};\n          fields.forEach(function (name) {\n            var element = elements[name];\n\n            // singular form elements just have one value\n            formData[name] = element.value;\n\n            // when our element has multiple items, get their values\n            if (element.length) {\n              var data = [];\n              for (var i = 0; i < element.length; i++) {\n                var item = element.item(i);\n                if (item.checked || item.selected) {\n                  data.push(item.value);\n                }\n              }\n              formData[name] = data.join(', ');\n            }\n          });\n\n          // add form-specific values into the data\n          formData.formDataNameOrder = JSON.stringify(fields);\n          formData.formGoogleSheetName = form.dataset.sheet || \"responses\"; // default sheet name\n          formData.formGoogleSendEmail\n            = form.dataset.email || \"\"; // no email by default\n\n          return { data: formData, honeypot: honeypot };\n        }\n\n        function handleFormSubmit(event) {  // handles form submit without any jquery\n          event.preventDefault();           // we are submitting via xhr below\n          var form = event.target;\n          var formData = getFormData(form);\n          var data = formData.data;\n\n          // If a honeypot field is filled, assume it was done so by a spam bot.\n          if (formData.honeypot) {\n            return false;\n          }\n\n          disableAllButtons(form);\n          var url = form.action;\n          var xhr = new XMLHttpRequest();\n          xhr.open('POST', url);\n          // xhr.withCredentials = true;\n          xhr.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded\");\n          xhr.onreadystatechange = function () {\n            if (xhr.readyState === 4 && xhr.status === 200) {\n              form.reset();\n              var formElements = form.querySelector(\".form-elements\")\n              if (formElements) {\n                formElements.style.display = \"none\"; // hide form\n              }\n              var thankYouMessage = form.querySelector(\".thankyou_message\");\n              if (thankYouMessage) {\n                thankYouMessage.style.display = \"block\";\n              }\n            }\n          };\n          // url encode form data for sending as post data\n          var encoded = Object.keys(data).map(function (k) {\n            return encodeURIComponent(k) + \"=\" + encodeURIComponent(data[k]);\n          }).join('&');\n          xhr.send(encoded);\n        }\n\n        function loaded() {\n          // bind to the submit event of our form\n          var forms = document.querySelectorAll(\"form.gform\");\n          for (var i = 0; i < forms.length; i++) {\n            forms[i].addEventListener(\"submit\", handleFormSubmit, false);\n          }\n        };\n        document.addEventListener(\"DOMContentLoaded\", loaded, false);\n\n        function disableAllButtons(form) {\n          var buttons = form.querySelectorAll(\"button\");\n          for (var i = 0; i < buttons.length; i++) {\n            buttons[i].disabled = true;\n          }\n        }\n      })();\n    </script>\n</body>\n\n</html>"};
const char style_css[] PROGMEM={"form label {\n  display: inline-block;\n  width: 100px;\n  }\n  form div {\n  margin-bottom: 10px;\n  }"};
const char* ssid = "IIESTS";// Give the ssid and password you want to connect to
const char* password = "";
 
ESP8266WebServer server(80);

void handleRoot()
{
  server.send_P(200,"text/html",index_html);
}

void handleCSS(){
  server.send_P(200,"text/css",style_css);
}

void handleNotFound(){
  String errorMessage="Resources cannot be found\n\n";
  server.send(404,"text/plain",errorMessage);  
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.println("");

  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.on("/",handleRoot);
  server.on("/style.css",handleCSS);
  
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("Server Listening, Open");
  Serial.println(WiFi.localIP());
  Serial.println("in your browser.");
}

void loop() {
  server.handleClient();
}