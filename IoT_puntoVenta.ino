#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <RestClient.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 254);
IPAddress myDns(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
RestClient restClient = RestClient("192.168.0.106", 2021);

String response = "";
int statusCode = 0;
char path[100];
String url;
char body[100];

void setup() {
  Serial.begin(9600);
  initEthernet();
}

void loop() {
  menu();
  String opcionString = obtenerTexto();
  procesarOpcionMenu(opcionString.toInt());
}

void menu() {
  Serial.println("Menu \n 1. Nuevo registro \n 2. Mostrar registro \n 3. Eliminar registro \n Seleccione una opcion.");
}

String obtenerTexto() {
  String input = "";
  while (Serial.available() <= 0) {}
  input = Serial.readString();
  return input;
}

void procesarOpcionMenu(int opcion) {
  String barcode = "";
  String nameItem = "";
  String idString = "";
  switch (opcion) {
    case 1:
      Serial.print("Ingresa ID: ");
      idString = obtenerTexto();
      Serial.print("Ingresa codigo de barras: ");
      barcode = obtenerTexto();
      Serial.print("Ingresa nombre del producto: ");
      nameItem = obtenerTexto();
      save(idString.toInt(),barcode, nameItem);
    break;
    case 2:
      Serial.print("Ingresa ID a buscar: ");
      idString = obtenerTexto();
      show(idString.toInt());
    break;
    case 3:
      Serial.print("Ingresa ID a eliminar: ");
      idString = obtenerTexto();
      deleteItem(idString.toInt());
    break;
    default:
      Serial.println("Opcion no valida!");
    break;
  }
}

void initEthernet() {
  Serial.println("Inicia Ethernet");
  if (Ethernet.begin(mac) == 0) {
    Ethernet.begin(mac, ip, myDns, gateway, subnet);
  }
}

void save(long id, String barcode, String nameItem) {
  response = "";
  restClient.setHeader("Accept: application/json");
  restClient.setHeader("Content-Type: application/json");
  DynamicJsonDocument dynamicJsonDocument(100);
  if(id > 0) {dynamicJsonDocument["id"] = id;}
  dynamicJsonDocument["code"] = barcode;
  dynamicJsonDocument["description"] = nameItem;
  serializeJson(dynamicJsonDocument, body);
  //Serial.println(body);
  statusCode = restClient.post("/product/save", body, &response);
  Serial.println(response);
}

void show(long id) {
  response = "";
  restClient.setHeader("Accept: application/json");
  restClient.setHeader("Content-Type: application/json");
  url = "/product/show/"; 
  url = url + id;
  url.toCharArray(path, sizeof(path));
  //Serial.println(path);
  statusCode = restClient.get(path, &response);
  Serial.println(response);
}

void deleteItem(long id) {
  response = "";
  restClient.setHeader("Accept: application/json");
  restClient.setHeader("Content-Type: application/json");
  url = "/product/delete/"; 
  url = url + id;
  url.toCharArray(path, sizeof(path));
  statusCode = restClient.del(path, &response);
  Serial.println(response);
}

/*
void getHome() {
  
     //https://github.com/csquared/arduino-restclient/
     //https://github.com/bblanchon/ArduinoJson
     //https://www.arduino.cc/en/Reference/Ethernet
     //https://arduinojson.org/v6/api/misc/deserializationerror/
     //https://stackoverflow.com/questions/56776547/arduinojson-does-not-deserialize-string-read-in-from-an-http-get-but-does-deseri
  
  String response = "";
  statusCode = restClient.get("/product/home", &response);
  Serial.print("RSP: ");
  Serial.println(response);
}
*/

/*
void findByBarcode(String barcode) {
  response = "";
  restClient.setHeader("Accept: application/json");
  restClient.setHeader("Content-Type: application/json");
  url = "/product/findByCode/?code=";
  url = url + barcode;
  url.toCharArray(path, sizeof(path));
  statusCode = restClient.get(path, &response);
  Serial.println(response);
}
*/

/*
void deserializeObject(String response) {
  DynamicJsonDocument dynamicJsonDocument(256);
  response.replace("4c", "");
  response.replace("0", "");
  response.replace("\"", "\\\"");
  DeserializationError error = deserializeJson(dynamicJsonDocument, response);

  //if (error) {
      //Serial.print(F("Problema al convertir: "));
      //Serial.println(error.c_str());
  //}
  long id = dynamicJsonDocument["id"];
  const char* barcode = dynamicJsonDocument["barcode"];
  const char* name = dynamicJsonDocument["name"];
  const char* description = dynamicJsonDocument["description"];
  Serial.println("Objeto");
  Serial.print(id);
  Serial.print(barcode);
  Serial.print(name);
  Serial.print(description);
}
*/

/*
void updateItem(long id, String barcode, String name, String description) {
  String response = "";
  char body[256];
  DynamicJsonDocument dynamicJsonDocument(256);
  dynamicJsonDocument["id"] = id;
  dynamicJsonDocument["barcode"] = barcode;
  dynamicJsonDocument["name"] = name;
  dynamicJsonDocument["description"] = description;
  serializeJson(dynamicJsonDocument, body);
  //Serial.println(body);
  restClient.setHeader("Accept: application/json");
  restClient.setHeader("Content-Type: application/json");
  statusCode = restClient.put("/product/update", body, &response);
  Serial.print("RSP: ");
  Serial.println(response);
}
*/
