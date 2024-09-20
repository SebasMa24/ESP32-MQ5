#include <pgmspace.h>

#define THINGNAME "esp32-TeleIII" // Nombre del objeto IOT

const char WIFI_SSID[] = "Nombre de red"; // Nombre de la red wifi             
const char WIFI_PASSWORD[] = "Contraseña"; // contraseña de la red wifi

const char AWS_IOT_ENDPOINT[] = ""; // endpoint de AWS IoT

// Amazon Root CA 1 : 
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----

)KEY";

// Device Private key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----

)KEY";
