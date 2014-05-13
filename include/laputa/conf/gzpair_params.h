/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic 
 * Semiconductor ASA.Terms and conditions of usage are described in detail 
 * in NORDIC SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT. 
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRENTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *              
 * $LastChangedRevision: 133 $
 */

/** @file
 * @brief Gazell pairing library parameters template file
 */
 
#ifndef __GZPAIR_SETUP_H
#define __GZPAIR_SETUP_H

/**
  Definition of secret xor cipher vector used for encrypting AES key exchange during 
  pairing. 
*/
#define GZPAIR_XOR_CIPHER_KEY {56, 56, 78, 18, 98, 26, 89, 14, 48, 95, 26, 11, 59, 29, 44, 27}

/** 
  Definition of the first static selected pairing channel. Should be located in
  the lower Nth of the channel range, where N is the size if the channel subset
  selected by the application.
*/  
#define GZPAIR_CHANNEL_LOW 3

/** 
  Definition of the second static selected pairing channel. Should be located in
  the upper Nth of the channel range, where N is the size if the channel subset
  selected by the application.
*/  
#define GZPAIR_CHANNEL_HIGH 77

/** 
  Definition of the static "global" pairing address.
*/ 
#define GZPAIR_ADDRESS {1, 2, 3, 4, 5}

/** 
  Definition of output power to be used when sendning a pairing request. 
*/ 
#define GZPAIR_POWER HAL_NRF_18DBM

/** 
  Definition of pairing request timeout. 
*/ 
#define GZPAIR_REQ_TX_TIMEOUT 200

/** 
  Definition of the period a device in close proximity shall back off on the pairing 
  address after a backoff packet has been received. 
*/  
#define GZPAIR_CLOSE_PROXIMITY_BACKOFF_RX_TIMEOUT ((GZPAIR_REQ_TX_TIMEOUT / 2) + 50)

/**
  Definition of the period a device shall wait before attempting to send the packet for
  fetching the pairing response (step 1).
*/
#define GZPAIR_TX_ACK_WAIT_TIMEOUT (GZPAIR_CLOSE_PROXIMITY_BACKOFF_RX_TIMEOUT + 50)

/**
  Definition of the time the host resides witinb for a device to complete
  transmission of the pairing request step 1 packet.  
*/
#define GZPAIR_STEP1_RX_TIMEOUT (((GZPAIR_REQ_TX_TIMEOUT / 2) + GZPAIR_TX_ACK_WAIT_TIMEOUT) + 50)

/** 
  Definition of the period a device NOT in close proximity shall back off on the pairing 
  address after a backoff packet has been received.  
*/   
#define GZPAIR_NOT_PROXIMITY_BACKOFF_RX_TIMEOUT (GZPAIR_CLOSE_PROXIMITY_BACKOFF_RX_TIMEOUT + GZPAIR_STEP1_RX_TIMEOUT)

/** 
  Definition of the maximimum number of "backoff" packets (step 0) to be transmitted. 
*/  
#define GZPAIR_MAX_BACKOFF_PACKETS 100

/** 
  Definition of the lowest boundary for the channel range to be used.
*/
#define GZPAIR_CHANNEL_MIN 2

/** 
  Definition of the upper boundary for the channel range to be used.
*/  
#define GZPAIR_CHANNEL_MAX 80

/** 
  Definition of the minimum channel spacing for the channel subset generated
  during pairing.
*/  
#define GZPAIR_CHANNEL_SPACING_MIN 5

/**
  Definition of the flash page to be reserved for pairing parameter storage
  in a when using the nRF24LU1. 
*/
#define GZPAIR_FLASH_PARAMETER_PAGE 31

/**
  Definition of the start address for the pairing data stored in non volatile
  memory on a nrf24LE1 using lib_eeprom255.
*/
#define GZPAIR_EEPROM_BASE_ADDRESS 0

#endif // PAIRING_SETUP_H_