//Originated from the official example WebUpdate of esp8266.

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* host = "esp8266-webupdate";
const char* ssid="........";
const char* password="........";

ESP8266WebServer server(80);
const char* serverIndex="<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

void setup(void)
{
	Serial.begin(115200);
	WiFi.mode(WIFI_AP_STA);
	WiFi.begin(ssid,password);
	if(WiFi.waitForConnectResult()==WL_CONNECTED)
	{
		MDNS.begin(host);
		server.on("/",HTTP_GET,[](){
			server.sendHeader("Connection","close");
			server.sendHeader("Access-Control-Allow-Origin","*");
			server.send(200,"text/html",serverIndex);
		});
		server.on("/update",HTTP_POST,[](){
			server.sendHeader("Connection","close");
			server.sendHeader("Access-Control-Allow-Origin","*");
			server.send(200,"text/plain",(Update.hasError())?"FAIL":"OK");
		},[](){
			HTTPUpload& upload=server.upload();
			if(upload.status==UPLOAD_FILE_START)
			{
				WiFiUDP::stopAll();
				//TODO
			}
			else if(upload.status==UPLOAD_FILE_WRITE)
			{
				//TODO
			}
			else if(upload.status==UPLOAD_FILE_END)
			{
				//TODO
			}
			yield();
		});
		server.begin();
		MDNS.addService("http", "tcp", 80);
	}
	else
	{
		ESP.restart();
	}
}

void loop(void)
{
	server.handleClient();
	delay(1);
}
