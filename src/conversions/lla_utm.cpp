#include "sico/conversions/lla_utm.hpp"

#include "sico/types/angles.hpp"
#include "sico/types/units.hpp"

using namespace sico;
using namespace literals;

namespace {
struct {
    double const a = 6378137.0;
    double const b = 6356752.31424518;

    double const f  = 0.0033528107; // ellipsoid flattening
    double const e  = 0.0818191908; // first eccentricity
    double const ep = 0.0820944379; // second eccentricity

    double const k0   = 0.9996;          // scale factor
    double const fe   = 500000.0;        // false easting
    double const fn_n = 0.0;             // false northing, northern hemisphere
    double const fn_s = 10000000.0;      // false northing, southern hemisphere
    double const e2   = (e * e);         // e^2
    double const e4   = (e2 * e2);       // e^4
    double const e6   = (e4 * e2);       // e^6
    double const ep2  = (e2 / (1 - e2)); // e'^2
} Earth;
} // namespace

// Equations from USGS Bulletin 1532
pos_utm sico::to_utm(pos_lla const& lla)
{
    // Make sure the longitude is between -180.00 .. 179.9
    degrees const lat_deg = lla.lat;
    degrees const lon_deg = lla.lon;
    degrees const lon_norm
        = (lon_deg + 180_deg) - degrees(int((raw(lon_deg) + 180) / 360) * 360) - 180_deg;

    int const zone_num = utm_zone_number(lat_deg, lon_norm);

    // +3 puts origin in middle of zone
    radians const lon_ori(degrees((zone_num - 1) * 6) - 180_deg + 3_deg);

    double const lat    = raw(lla.lat);
    double const sinlat = sin(lat);
    double const coslat = cos(lat);
    double const tanlat = tan(lat);

    double const N = Earth.a / sqrt(1 - Earth.e2 * sinlat * sinlat);
    double const T = tanlat * tanlat;
    double const C = Earth.ep2 * coslat * coslat;
    double const A = coslat * raw(lla.lon - lon_ori);

    double const M = Earth.a
        * ((1 - Earth.e2 / 4 - 3 * Earth.e2 * Earth.e2 / 64
            - 5 * Earth.e2 * Earth.e2 * Earth.e2 / 256)
               * lat
           - (3 * Earth.e2 / 8 + 3 * Earth.e2 * Earth.e2 / 32
              + 45 * Earth.e2 * Earth.e2 * Earth.e2 / 1024)
               * sin(2 * lat)
           + (15 * Earth.e2 * Earth.e2 / 256 + 45 * Earth.e2 * Earth.e2 * Earth.e2 / 1024)
               * sin(4 * lat)
           - (35 * Earth.e2 * Earth.e2 * Earth.e2 / 3072) * sin(6 * lat));

    pos_utm utm;
    utm.easting
        = meters(Earth.k0 * N
                     * (A + (1 - T + C) * A * A * A / 6
                        + (5 - 18 * T + T * T + 72 * C - 58 * Earth.ep2) * A * A * A * A * A / 120)
                 + 500000.0);

    utm.northing = meters(Earth.k0
                          * (M
                             + N * tanlat
                                 * (A * A / 2 + (5 - T + 9 * C + 4 * C * C) * A * A * A * A / 24
                                    + (61 - 58 * T + T * T + 600 * C - 330 * Earth.ep2) * A * A * A
                                        * A * A * A / 720)));
    if (lat_deg < 0_deg) {
        utm.northing += 10000000.0_m; // offset for southern hemisphere
    }
    utm.altitude    = lla.alt;
    utm.zone_number = zone_num;
    utm.zone_letter = utm_zone_letter(lat_deg);
    return utm;
}

pos_lla sico::to_lla(pos_utm const& utm)
{
    double e1 = (1 - sqrt(1 - Earth.e2)) / (1 + sqrt(1 - Earth.e2));

    double const x = raw(utm.easting) - 500000.0; // remove 500,000 meter offset for longitude
    double const y
        = ((utm.zone_letter - 'N') < 0) ? raw(utm.northing) - 10000000.0 : raw(utm.northing);

    double const M  = y / Earth.k0;
    double const mu = M
        / (Earth.a
           * (1 - Earth.e2 / 4 - 3 * Earth.e2 * Earth.e2 / 64
              - 5 * Earth.e2 * Earth.e2 * Earth.e2 / 256));

    double const phi1 = mu
        + ((3 * e1 / 2 - 27 * e1 * e1 * e1 / 32) * sin(2 * mu)
           + (21 * e1 * e1 / 16 - 55 * e1 * e1 * e1 * e1 / 32) * sin(4 * mu)
           + (151 * e1 * e1 * e1 / 96) * sin(6 * mu));
    double const sinphi1 = sin(phi1);
    double const cosphi1 = cos(phi1);
    double const tanphi1 = tan(phi1);
    double const N1      = Earth.a / sqrt(1 - Earth.e2 * sinphi1 * sinphi1);
    double const T1      = tanphi1 * tanphi1;
    double const C1      = Earth.ep2 * cosphi1 * cosphi1;
    double const R1      = Earth.a * (1 - Earth.e2) / pow(1 - Earth.e2 * sinphi1 * sinphi1, 1.5);
    double const D       = x / (N1 * Earth.k0);

    pos_lla lla;
    lla.lat = radians(
        phi1
        - ((N1 * tanphi1 / R1)
           * (D * D / 2 - (5 + 3 * T1 + 10 * C1 - 4 * C1 * C1 - 9 * Earth.ep2) * D * D * D * D / 24
              + (61 + 90 * T1 + 298 * C1 + 45 * T1 * T1 - 252 * Earth.ep2 - 3 * C1 * C1) * D * D * D
                  * D * D * D / 720)));

    lla.lon = radians((D - (1 + 2 * T1 + C1) * D * D * D / 6
                       + (5 - 2 * C1 + 28 * T1 - 3 * C1 * C1 + 8 * Earth.ep2 + 24 * T1 * T1) * D * D
                           * D * D * D / 120)
                      / cosphi1);

    //+3 puts origin in middle of zone
    radians const lon_ori(degrees((utm.zone_number - 1) * 6) - 180_deg + 3_deg);
    lla.lon += lon_ori;
    lla.alt = utm.altitude;
    return lla;
}

char sico::utm_zone_letter(degrees lat)
{
    if ((84_deg >= lat) && (lat >= 72_deg))
        return 'X';
    else if ((72_deg > lat) && (lat >= 64_deg))
        return 'W';
    else if ((64_deg > lat) && (lat >= 56_deg))
        return 'V';
    else if ((56_deg > lat) && (lat >= 48_deg))
        return 'U';
    else if ((48_deg > lat) && (lat >= 40_deg))
        return 'T';
    else if ((40_deg > lat) && (lat >= 32_deg))
        return 'S';
    else if ((32_deg > lat) && (lat >= 24_deg))
        return 'R';
    else if ((24_deg > lat) && (lat >= 16_deg))
        return 'Q';
    else if ((16_deg > lat) && (lat >= 8_deg))
        return 'P';
    else if ((8_deg > lat) && (lat >= 0_deg))
        return 'N';
    else if ((0_deg > lat) && (lat >= -8_deg))
        return 'M';
    else if ((-8_deg > lat) && (lat >= -16_deg))
        return 'L';
    else if ((-16_deg > lat) && (lat >= -24_deg))
        return 'K';
    else if ((-24_deg > lat) && (lat >= -32_deg))
        return 'J';
    else if ((-32_deg > lat) && (lat >= -40_deg))
        return 'H';
    else if ((-40_deg > lat) && (lat >= -48_deg))
        return 'G';
    else if ((-48_deg > lat) && (lat >= -56_deg))
        return 'F';
    else if ((-56_deg > lat) && (lat >= -64_deg))
        return 'E';
    else if ((-64_deg > lat) && (lat >= -72_deg))
        return 'D';
    else if ((-72_deg > lat) && (lat >= -80_deg))
        return 'C';
    else
        return 'Z';
}

int sico::utm_zone_number(degrees lat, degrees lon)
{
    if (lat >= 56.0_deg && lat < 64.0_deg && lon >= 3.0_deg && lon < 12.0_deg)
        return 32;

    // Svalbard
    if (lat >= 72.0_deg && lat < 84.0_deg) {
        if (lon >= 0.0_deg && lon < 9.0_deg)
            return 31;
        else if (lon >= 9.0_deg && lon < 21.0_deg)
            return 33;
        else if (lon >= 21.0_deg && lon < 33.0_deg)
            return 35;
        else if (lon >= 33.0_deg && lon < 42.0_deg)
            return 37;
    }

    return int((raw(lon) + 180) / 6) + 1;
}