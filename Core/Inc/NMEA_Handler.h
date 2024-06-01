/*
 * NMEA_Handler.h
 *
 *  Created on: May 24, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_NMEA_HANDLER_H_
#define INC_NMEA_HANDLER_H_

int NMEA_GNGLL(char *msg);
int NMEA_GNVTG(char *msg);
int NMEA_GNRMC(char *msg);

bool NMEA_CalculateChecksum(char *msg);

char *strtok_fr (char *s, char delim, char **save_ptr);
char *strtok_f (char *s, char delim);

#endif /* INC_NMEA_HANDLER_H_ */
