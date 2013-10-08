/**
 ******************************************************************************
 *
 * @file       GPS_DJI.h
 * @author     Joerg-D. Rothfuchs
 * @brief      GPS module, handles GPS DJI stream
 *	       interpretation of packets see http://www.rcgroups.com/forums/showthread.php?t=1995704
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


#ifdef GPS_PROTOCOL_DJI

#ifndef GPS_DJI_H
#define GPS_DJI_H


//#define DJI_HEADING_FROM_MAG


#define DJI_SYNC1	0x55  // DJI protocol synchronization characters
#define DJI_SYNC2	0xAA


#define DJI_GPS_DATA	0x10  // DJI packet types
#define DJI_MAG_DATA	0x20
#define DJI_XXX_DATA	0x30


#define NO_PARSER          -3 // no parser available
#define PARSER_OVERRUN     -2 // message buffer overrun before completing the message
#define PARSER_ERROR       -1 // message unparsable by this parser
#define PARSER_INCOMPLETE   0 // parser needs more data to complete the message
#define PARSER_COMPLETE     1 // parser has received a complete message and finished processing
#define PARSER_COMPLETE_SET 2 // parser has received a complete message set and finished processing


#define GPSPOSITIONSENSOR_STATUS_NOGPS	0
#define GPSPOSITIONSENSOR_STATUS_NOFIX	1
#define GPSPOSITIONSENSOR_STATUS_FIX2D	2
#define GPSPOSITIONSENSOR_STATUS_FIX3D	3


// Receiver Navigation Status
#define STATUS_GPSFIX_NOFIX    0x00
#define STATUS_GPSFIX_DRONLY   0x01
#define STATUS_GPSFIX_2DFIX    0x02
#define STATUS_GPSFIX_3DFIX    0x03
#define STATUS_GPSFIX_GPSDR    0x04
#define STATUS_GPSFIX_TIMEONLY 0x05

#define STATUS_FLAGS_GPSFIX_OK (1 << 0)
#define STATUS_FLAGS_DIFFSOLN  (1 << 1)
#define STATUS_FLAGS_WKNSET    (1 << 2)
#define STATUS_FLAGS_TOWSET    (1 << 3)


struct GPS_VALUES {
    uint8_t	Status;
    uint8_t	Satellites;
    float	Latitude;
    float	Longitude;
    float	Altitude;
    float	Heading;
    float	Groundspeed;
    float	Down;
};


struct GPS_RX_STATS {
    uint16_t gpsRxReceived;
    uint16_t gpsRxChkSumError;
    uint16_t gpsRxOverflow;
    uint16_t gpsRxParserError;
};


// DJI GPS data
struct DJI_GPS {
    uint32_t iTOW;		// GPS Millisecond Time of Week (ms)
    int32_t  lon;		// Longitude (deg*1e-7)
    int32_t  lat;		// Latitude (deg*1e-7)
    int32_t  hMSL;		// Height above mean sea level (mm)
    uint32_t hAcc;		// Horizontal Accuracy Estimate (mm)
    uint32_t vAcc;		// Vertical Accuracy Estimate (mm)
    uint32_t uk01;		// ??? (seems to be always 0)
    int32_t  velN;		// cm/s NED north velocity
    int32_t  velE;		// cm/s NED east velocity
    int32_t  velD;		// cm/s NED down velocity
    uint16_t pDOP;		// Position DOP
    uint16_t vDOP;		// Vertical DOP
    uint16_t nDOP;		// Northing DOP
    uint16_t eDOP;		// Easting DOP
    uint8_t  numSV;		// Number of SVs used in Nav Solution
    uint8_t  uk02;		// ??? (seems to be always 0)
    uint8_t  gpsFix;		// GPS fix type
    uint8_t  uk03;		// ??? (seems to be always 0)
    uint8_t  flags;		// Navigation Status Flags
    uint16_t uk04;		// ??? (seems to be always 0)
    uint8_t  mask;		// XOR mask
    uint16_t uk05;		// ??? sequence number (not XORed), once there is a lock - increases with every message
};


// DJI mag data
struct DJI_MAG {
    int16_t magX;		// compass X axis data (signed)
    int16_t magY;		// compass Y axis data (signed)
    uint8_t uk01;		// ??? (possibly compass Z data)
    uint8_t mask;		// XOR mask
};


struct DJIHeader {
    uint8_t  id;
    uint16_t len;
    uint8_t  ck_a;
    uint8_t  ck_b;
};


typedef union {
    uint8_t payload[0];
    struct DJI_GPS  dji_gps;
    struct DJI_MAG  dji_mag;
} DJIPayload;


struct DJIPacket {
    struct DJIHeader header;
    DJIPayload payload;
};


int parse_dji(uint8_t);
uint8_t get_dji_status(void);
uint8_t get_dji_satellites(void);
float get_dji_latitude(void);
float get_dji_longitude(void);
float get_dji_altitude(void);
float get_dji_heading(void);
float get_dji_groundspeed(void);
float get_dji_down(void);

#endif /* GPS_DJI_H */

#endif // GPS_PROTOCOL_DJI
