/*
 * NMEA_Handler.h
 *
 *  Created on: May 24, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_NMEA_HANDLER_H_
#define INC_NMEA_HANDLER_H_

int NMEA_GLL(char *msg);
int NMEA_VTG(char *msg);
int NMEA_GSA(char *msg);
int NMEA_GP_GSV(char *msg);
int NMEA_GL_GSV(char *msg);

char *strtok_fr (char *s, char delim, char **save_ptr);
char *strtok_f (char *s, char delim);

#endif /* INC_NMEA_HANDLER_H_ */
