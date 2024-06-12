/*
 * NMEA_handler.c
 *
 *  Created on: May 24, 2024
 *      Author: karl.yamashita
 */


#include "main.h"

extern UartBufferStruct lpuart1;
extern Flags_t flags;

int NMEA_GNGLL(char *msg)
{
	char str[UART_TX_DATA_SIZE] = {0};
	char *nmea = strtok_f(msg, ',');
	char *lat = strtok_f(NULL, ',');
	char *ns = strtok_f(NULL, ',');
	char *lon = strtok_f(NULL, ',');
	char *ew = strtok_f(NULL, ',');
	char *time = strtok_f(NULL, ',');
	char *status = strtok_f(NULL, ',');
	char *posMode = strtok_f(NULL, '*');

	sprintf(str, "%s = lat: %s, ns: %s, lon: %s, ew: %s, time: %s, status: %s, posMode: %s", nmea, lat, ns, lon, ew, time, status, posMode);

	UART_NotifyUser(&lpuart1, str, strlen(str), true);

	return 0;
}

int NMEA_GNVTG(char *msg)
{
	char str[UART_TX_DATA_SIZE] = {0};
	char *nmea = strtok_f(msg, ',');
	char *cogt = strtok_f(NULL, ',');
	char *cogtUnit = strtok_f(NULL, ',');
	char *cogm = strtok_f(NULL, ',');
	char *cogmUnit = strtok_f(NULL, ',');
	char *sogn = strtok_f(NULL, ',');
	char *sognUnit = strtok_f(NULL, ',');
	char *sogk = strtok_f(NULL, ',');
	char *sogkUnit = strtok_f(NULL, ',');
	char *posMode = strtok_f(NULL, '*');

	sprintf(str, "%s = cogt: %s, cogtUnit: %s, cogm: %s, cogmUnit: %s, sogn: %s, sognUnit: %s, sogk: %s, sogkUnit: %s, posMode: %s", \
						nmea, cogt, cogtUnit, cogm, cogmUnit, sogn, sognUnit, sogk, sogkUnit, posMode);

	UART_NotifyUser(&lpuart1, str, strlen(str), true);

	return 0;
}


int NMEA_GNRMC(char *msg)
{
	char str[UART_TX_DATA_SIZE] = {0};
	char *nmea = strtok_f(msg, ',');
	char *time = strtok_f(NULL, ',');
	char *status = strtok_f(NULL, ',');
	char *lat = strtok_f(NULL, ',');
	char *ns = strtok_f(NULL, ',');
	char *lon = strtok_f(NULL, ',');
	char *ew = strtok_f(NULL, ',');
	char *spd = strtok_f(NULL, ',');
	char *cog = strtok_f(NULL, ',');
	char *date = strtok_f(NULL, ',');
	//char *mv = strtok_f(NULL, ','); // supported in AD 4.10 and later
	char *mvew = strtok_f(NULL, ',');
	char *posMode = strtok_f(NULL, ',');
	char *navStatus = strtok_f(NULL, '*');
	double latitude;
	double longitude;
	double latTime;
	double lonTime;

	sprintf(str, "%s = time: %s, status: %s, lat: %s, ns: %s, lon: %s, ew: %s, spd: %s, cog: %s, date: %s, mvew: %s, posMode: %s, navStatus: %s",
						nmea, time, status, lat, ns, lon, ew, spd, cog, date, mvew, posMode, navStatus);

	UART_NotifyUser(&lpuart1, str, strlen(str), true);

	if(flags.googleMaps)
	{
		// parse lat/lon for Google maps
		latitude = (atof(lat) * .01);
		sprintf(str, "%f", latitude);
		latTime = (modf(latitude, &latitude)  / 60) * 1000000;

		longitude = (atof(lon) * .01);
		sprintf(str, "%f", longitude);
		lonTime = (modf(longitude, &longitude) / 60) * 1000000;

		Nop();

		sprintf(str, "Copy this to Google Maps-> %ld.%ld %s %ld.%ld %s", (uint32_t)latitude, (uint32_t)latTime, ns, (uint32_t)longitude, (uint32_t)lonTime, ew);

		UART_NotifyUser(&lpuart1, str, strlen(str), true);
	}

	return 0;
}

/*
 * Description: Calculates checksum of NMEA message between '$' and '*'.
 * Input: The NMEA message string
 * return: true if checksum matches, else false
 */
bool NMEA_CalculateChecksum(char *msg)
{
    uint8_t cksum = 0;
    uint8_t endCksum;
    int i = 0;
    char *_crc;

    for(i = 1; i < strlen(msg) - 5; i++) // minus the "*hh\r\n" characters
    {
        cksum ^= (uint8_t)msg[i];
    }

    _crc = strtok(&msg[++i], "\r"); // skip '*' character and save up to CR
    endCksum = strtol(_crc, NULL, 16); // save hex string as uint8_t value

    if(cksum == endCksum)
    {
        return true;
    }

    return false;
}

/*
* These two functions below was contributed by another user who figured out that the standard strtok_r 
*	would not work with consecutive ',' in a string correctly.
*/
char *strtok_fr (char *s, char delim, char **save_ptr)
{
    char *tail;
    char c;

    if (s == NULL)
    {
        s = *save_ptr;
    }
    tail = s;
    if ((c = *tail) == '\0')
    {
        s = NULL;
    }
    else
    {
		do {
			if (c == delim)
			{
				*tail++ = '\0';
				break;
			}
		} while ((c = *++tail) != '\0');
	}
    *save_ptr = tail;
    return s;
}

char *strtok_f (char *s, char delim)
{
    static char *save_ptr;

    return strtok_fr (s, delim, &save_ptr);
}
