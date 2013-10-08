/**
 ******************************************************************************
 *
 * @file       GPS_DJI.ino
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

#include "GPS_DJI.h"


static char gps_rx_buffer[sizeof(struct DJIPacket)];
static struct GPS_VALUES GPSValues;
static struct GPS_RX_STATS gpsRxStats; 


short decodeShort(byte* d, unsigned char mask)
{
    union {
	short s;
	unsigned char b[2];
    } val;
    for (int i=0; i<2; i++)
	val.b[i] = *d++ ^ mask;
    return val.s;
}


long decodeLong(byte* d, unsigned char mask)
{
    union {
	long l;
	unsigned char b[4];
    } val;
    for (int i=0; i<4; i++)
	val.b[i] = *d++ ^ mask;
    return val.l;
} 


void parse_dji_gps(struct DJI_GPS *gps)
{
	int mask = gps->mask;
	
        if (gps->flags ^ mask & STATUS_FLAGS_GPSFIX_OK) {
            switch (gps->gpsFix ^ mask) {
		case STATUS_GPSFIX_2DFIX:
			GPSValues.Status = GPSPOSITIONSENSOR_STATUS_FIX2D;
                break;
		case STATUS_GPSFIX_3DFIX:
			GPSValues.Status = GPSPOSITIONSENSOR_STATUS_FIX3D;
                break;
		default:
			GPSValues.Status = GPSPOSITIONSENSOR_STATUS_NOFIX;
            }
        } else {	// fix is not valid so we make sure to treat it as NOFIX
            GPSValues.Status = GPSPOSITIONSENSOR_STATUS_NOFIX;
        }

	GPSValues.Satellites	= gps->numSV;
	GPSValues.Latitude	= (float)decodeLong((byte*)&gps->lat, mask) / 10000000.0;
	GPSValues.Longitude	= (float)decodeLong((byte*)&gps->lon, mask) / 10000000.0;
	GPSValues.Altitude	= (float)decodeLong((byte*)&gps->hMSL, mask) * 0.001f;
	GPSValues.Down		= (float)decodeLong((byte*)&gps->velD, mask) / 100.0f;
	
	float velN		= (float)decodeLong((byte*)&gps->velN, mask) / 100.0f;
	float velE		= (float)decodeLong((byte*)&gps->velE, mask) / 100.0f;
	
	// calculate groundspeed
	GPSValues.Groundspeed	= sqrt(velN * velN + velE * velE);
	
#ifndef DJI_HEADING_FROM_MAG
	// calculate heading
	float heading = atan2(velE, velN) * 180.0 / M_PI;
	if (heading < 0.0) heading += 360.0;
	GPSValues.Heading	= heading;
#endif
}


// TODO not correctly working yet
void parse_dji_mag(struct DJI_MAG *mag)
{
#ifdef DJI_HEADING_FROM_MAG
	int mask = mag->mask;
	
	short x = decodeShort((byte*)&mag->magX, mask);
	x ^= 0x0100;
	
	short y = decodeShort((byte*)&mag->magY, mask);
	y ^= 0x0100;
	
	float heading = -atan2(y, x) * 180.0 / M_PI;
	if (heading < 0.0) heading += 360.0;
	
	// TODO add magnetic declination
	// TODO add tilt compensation
	
	GPSValues.Heading	= heading;
#endif
}


// DJI message parser
void parse_dji_message(struct DJIPacket *dji)
{
    switch (dji->header.id) {
    case DJI_GPS_DATA:
        parse_dji_gps(&dji->payload.dji_gps);
        break;
    case DJI_MAG_DATA:
        parse_dji_mag(&dji->payload.dji_mag);
        break;
    case DJI_XXX_DATA:
        break;
    }
}


bool checksum_dji_message(struct DJIPacket *dji)
{
    int i;
    uint8_t ck_a = 0;
    uint8_t ck_b = 0;

    ck_a += dji->header.id;
    ck_b += ck_a;

    ck_a += dji->header.len;
    ck_b += ck_a;

    for (i = 0; i < dji->header.len; i++) {
        ck_a += dji->payload.payload[i];
        ck_b += ck_a;
    }

    if (dji->header.ck_a == ck_a && dji->header.ck_b == ck_b) {
        return true;
    } else {
        return false;
    }
}


// parse incoming character stream for messages in DJI format
int parse_dji_stream(uint8_t c, char *gps_rx_buffer, struct GPS_RX_STATS *gpsRxStats)
{
    enum proto_states {
        START,
        DJI_SY2,
        DJI_ID,
        DJI_LEN,
        DJI_PAYLOAD,
        DJI_CHK1,
        DJI_CHK2,
        FINISHED
    };

    static enum proto_states proto_state = START;
    static uint8_t rx_count = 0;
    struct DJIPacket *dji   = (struct DJIPacket *)gps_rx_buffer;

    switch (proto_state) {
    case START:							// detect protocol
        if (c == DJI_SYNC1) {					// first DJI sync char found
            proto_state = DJI_SY2;
        }
        break;
    case DJI_SY2:
        if (c == DJI_SYNC2) {					// second DJI sync char found
            proto_state = DJI_ID;
        } else {
            proto_state = START;				// reset state
        }
        break;
    case DJI_ID:
        dji->header.id   = c;
        proto_state      = DJI_LEN;
        break;
    case DJI_LEN:
        dji->header.len  = c;
        if (dji->header.len > sizeof(DJIPayload)) {
            gpsRxStats->gpsRxOverflow++;
            proto_state = START;
        } else {
            rx_count    = 0;
            proto_state = DJI_PAYLOAD;
        }
        break;
    case DJI_PAYLOAD:
        if (rx_count < dji->header.len) {
            dji->payload.payload[rx_count] = c;
            if (++rx_count == dji->header.len) {
                proto_state = DJI_CHK1;
            }
        } else {
            gpsRxStats->gpsRxOverflow++;
            proto_state = START;
        }
        break;
    case DJI_CHK1:
        dji->header.ck_a = c;
        proto_state = DJI_CHK2;
        break;
    case DJI_CHK2:
        dji->header.ck_b = c;
        if (checksum_dji_message(dji)) {			// message complete and valid
            parse_dji_message(dji);
            proto_state = FINISHED;
        } else {
            gpsRxStats->gpsRxChkSumError++;
            proto_state = START;
        }
        break;
    default: break;
    }

    if (proto_state == START) {
        return PARSER_ERROR;					// parser couldn't use this byte
    } else if (proto_state == FINISHED) {
        gpsRxStats->gpsRxReceived++;
        proto_state = START;
	return PARSER_COMPLETE_SET;				// message set complete & processed
    }

    return PARSER_INCOMPLETE;					// message not (yet) complete
}


// wrapper und getter

int parse_dji(uint8_t c)
{
	return parse_dji_stream(c, gps_rx_buffer, &gpsRxStats);
}

uint8_t get_dji_status(void)
{
	return GPSValues.Status;
}

uint8_t get_dji_satellites(void)
{
	return GPSValues.Satellites;
}

float get_dji_latitude(void)
{
	return GPSValues.Latitude;
}

float get_dji_longitude(void)
{
	return GPSValues.Longitude;
}

float get_dji_altitude(void)
{
	return GPSValues.Altitude;
}

float get_dji_heading(void)
{
	return GPSValues.Heading;
}

float get_dji_groundspeed(void)
{
	return GPSValues.Groundspeed;
}

float get_dji_down(void)
{
	return GPSValues.Down;
}


#endif // GPS_PROTOCOL_DJI
