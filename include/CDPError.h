#ifndef CDPERROR_H
#define CDPERROR_H


/// No Error
#define ERR_NONE 0
// Feature not supported
#define ERR_NOT_SUPPORTED -5000
// Failed to setup device
#define ERR_SETUP         -5100
// Device Id is too long
#define ERR_ID_TOO_LONG   -5101
#define ERR_OTA           -5200

/// Radio module initialization error
#define LORA_ERR_BEGIN          -1000
/// Radio module configuration error
#define LORA_ERR_SETUP          -1001
/// Failure to read data from the Radio module
#define LORA_ERR_RECEIVE        -1002
/// Radio module timeout error
#define LORA_ERR_TIMEOUT        -1003
// Failed to send data
#define LORA_ERR_TRANSMIT       -1004
// Failed to handle data received from the Radio module
#define LORA_ERR_HANDLE_PACKET  -1050
// Attempted to send a message larger than 256 bytes
#define LORA_ERR_MSG_TOO_LARGE  -1051
// Radio is busy sending data
#define LORA_ERR_TX_BUSY        -1052
#define LORA_ERR_INVALID_FREQUENCY -1053
#define LORA_ERR_INVALID_BANDWIDTH -1054
#define LORA_ERR_INVALID_SPREADING_FACTOR -1055
#define LORA_ERR_INVALID_OUTPUT_POWER -1056
#define LORA_ERR_INVALID_GAIN -1057
#define LORA_ERR_PACKET_TOO_LONG -1058
#define LORA_ERR_TX_TIMEOUT -1059


// Wifi network is not available
#define WIFI_ERR_NOT_AVAILABLE  -2000
// Wifi is disconnected
#define WIFI_ERR_DISCONNECTED   -2001
// Wifi generic setup error
#define WIFI_ERR_AP_CONFIG      -2002
// DNS initialization failed
#define DNS_ERR_STARTING        -3000


#define PACKET_ERR_SIZE_INVALID  -4000
#define PACKET_ERR_TOPIC_INVALID -4001
#define PACKET_ERR_MAX_HOPS      -4002

#define INTERNET_ERR_SETUP      -6000
#define INTERNET_ERR_SSID       -6001
#define INTERNET_ERR_CONNECT    -6002

#endif