/*
 * NMEA_handler.c
 *
 *  Created on: May 24, 2024
 *      Author: karl.yamashita
 */


#include "main.h"

extern UartBufferStruct lpuart1;

int NMEA_GNGLL(char *msg)
{
	char str[UART_TX_BYTE_BUFFER_SIZE] = {0};
	char *nmea = strtok_f(msg, ',');
	char *lat = strtok_f(NULL, ',');
	char *ns = strtok_f(NULL, ',');
	char *lon = strtok_f(NULL, ',');
	char *ew = strtok_f(NULL, ',');
	char *time = strtok_f(NULL, ',');
	char *status = strtok_f(NULL, ',');
	char *posMode = strtok_f(NULL, '*');

	sprintf(str, "%s = lat: %s, ns: %s, lon: %s, ew: %s, time: %s, status: %s, posMode: %s", nmea, lat, ns, lon, ew, time, status, posMode);

	NotifyUser(&lpuart1, str, true);

	return 0;
}

int NMEA_GNVTG(char *msg)
{
	char str[UART_TX_BYTE_BUFFER_SIZE] = {0};
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

	NotifyUser(&lpuart1, str, true);

	return 0;
}


int NMEA_GNRMC(char *msg)
{
	char str[UART_TX_BYTE_BUFFER_SIZE] = {0};
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

	sprintf(str, "%s = time: %s, status: %s, lat: %s, ns: %s, lon: %s, ew: %s, spd: %s, cog: %s, date: %s, mvew: %s, posMode: %s, navStatus: %s",
						nmea, time, status, lat, ns, lon, ew, spd, cog, date, mvew, posMode, navStatus);

	NotifyUser(&lpuart1, str, true);

	return 0;
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
