#ifndef SICO_H
#define SICO_H

/*
 * C wrappers around conversion functions
 * All units are SI (meters, radians...)
 */

void sico_lla2ecef(double lat, double lon, double alt, double* x, double* y, double* z);
void sico_ecef2lla(double x, double y, double z, double* lat, double* lon, double* alt);

#endif