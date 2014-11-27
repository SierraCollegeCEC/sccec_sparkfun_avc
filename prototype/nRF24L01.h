/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * nRF24L01 - Radio module used for telemetry information
 * Authors: James Smith
 */

#ifndef _NRF24L01_H_
#define _NRF24L01_H_
#include <stdint.h>

/* Register map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define RPD         0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD       0x1C
#define FEATURE     0x1D

typedef struct s_nRF24L01
{
  uint8_t pinCE;
  uint8_t pinCSN;
} nRF24L01;

void rf24Setup( nRF24L01* radio, uint8_t pinCE, uint8_t pinCSN );

#endif
