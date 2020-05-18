#include "sico/conversions/lla_mercator.hpp"

#include "sico/types/angles.hpp"

#include <algorithm>
#include <complex>

using namespace sico;
#pragma warning(disable : 4626)
#pragma warning(disable : 5027)
#pragma warning(disable : 5045)

namespace {
/*
 * Constants and formulas from
 * https://geographiclib.sourceforge.io/
 * Charles Karney (2008-2017) <charles@karney.com>
 * */
struct {
    double const a  = 6378137.0;
    double const f  = 0.0033528107; // ellipsoid flattening
    double const k0 = 0.9996;       // scale factor

    double const e2  = (f * (2.0 - f));
    double const es  = sqrt(e2);
    double const e2m = 1 - e2;
    double const c   = sqrt(e2m) * exp(es * atanh(es));
    double const n   = (f / (2.0 - f));

    double const b1coeff[5]   = { 1, 4, 64, 256, 256 }; // b1*(n+1), polynomial in n2 of order 3
    double const alpcoeff[27] = {
        // alp[1]/n^1, polynomial in n of order 5
        31564,
        -66675,
        34440,
        47250,
        -100800,
        75600,
        151200,
        // alp[2]/n^2, polynomial in n of order 4
        -1983433,
        863232,
        748608,
        -1161216,
        524160,
        1935360,
        // alp[3]/n^3, polynomial in n of order 3
        670412,
        406647,
        -533952,
        184464,
        725760,
        // alp[4]/n^4, polynomial in n of order 2
        6601661,
        -7732800,
        2230245,
        7257600,
        // alp[5]/n^5, polynomial in n of order 1
        -13675556,
        3438171,
        7983360,
        // alp[6]/n^6, polynomial in n of order 0
        212378941,
        319334400,
    };
    double const betcoeff[27] = {
        // bet[1]/n^1, polynomial in n of order 5
        384796,
        -382725,
        -6720,
        932400,
        -1612800,
        1209600,
        2419200,
        // bet[2]/n^2, polynomial in n of order 4
        -1118711,
        1695744,
        -1174656,
        258048,
        80640,
        3870720,
        // bet[3]/n^3, polynomial in n of order 3
        22276,
        -16929,
        -15984,
        12852,
        362880,
        // bet[4]/n^4, polynomial in n of order 2
        -830251,
        -158400,
        197865,
        7257600,
        // bet[5]/n^5, polynomial in n of order 1
        -435388,
        453717,
        15966720,
        // bet[6]/n^6, polynomial in n of order 0
        20648693,
        638668800,
    };

    double const b1     = 0.99832429841364134;
    double const a1     = b1 * a;
    double const alp[7] = { 0.00000000000000000,    0.00083773182942767257, 7.6085279335393839e-07,
                            1.1976455411087170e-09, 2.4291707093878136e-12, 5.7117579782417441e-15,
                            1.4911178254848753e-17 };
    double const bet[7] = { 0.00000000000000000,    0.00083773217286116234, 5.9058702769485497e-08,
                            1.6734827181222106e-10, 2.1647981312298670e-13, 3.7879782455786610e-16,
                            7.2487493487559978e-19 };

} Earth;
} // namespace

template<typename T>
T taupf(T tau)
{
    T const tau1    = std::hypot(T(1), tau);
    T const eatanhe = Earth.es * std::atanh(Earth.es * (tau / tau1));
    T const sig     = std::sinh(eatanhe);
    return std::hypot(T(1), sig) * tau - sig * tau1;
}

pos_enu_m sico::to_enu(radians lon0, pos_lla const& p)
{
    double const lat     = raw(p.lat);
    double const lonDiff = raw(angleDiff(lon0, p.lon));

    bool const   backside = lonDiff > SICO_PI2;
    double const lon      = backside ? SICO_PI - lonDiff : lonDiff;

    double const latsign = (backside && lat == 0) ? -1.0 : std::copysign(1.0, lat);
    double const lonsign = std::copysign(1.0, lon);
    double const sphi    = sin(lat);
    double const cphi    = cos(lat);
    double const slam    = sin(lon);
    double const clam    = cos(lon);

    double const tau = sphi / cphi, taup = taupf(tau);
    double const xip    = atan2(taup, clam);
    double const etap   = asinh(slam / hypot(taup, clam));
    double const gamma1 = atan2(slam * taup, clam * hypot(1.0, taup));
    double const k1
        = sqrt(Earth.e2m + Earth.e2 * cphi * cphi) * hypot(1.0, tau) / hypot(taup, clam);
    double const c2xi = cos(2.0 * xip);
    double const ch2e = cosh(2.0 * etap);
    double const s2xi = sin(2.0 * xip);
    double const sh2e = sinh(2.0 * etap);

    using complexd = std::complex<double>;
    complexd y0;
    complexd y1;
    {
        complexd const a(2 * c2xi * ch2e, -2 * s2xi * sh2e);    // 2 * cos(2*zeta')
        complexd const a2 = a / double(2);                      // cos(2*zeta')
        complexd const a3 = complexd(s2xi * ch2e, c2xi * sh2e); // sin(2*zeta')

        complexd z0;
        complexd z1;

        int n = 6;
        while (n) {
            y1 = a * y0 - y1 + Earth.alp[n];
            z1 = a * z0 - z1 + 2 * n * Earth.alp[n];
            --n;
            y0 = a * y1 - y0 + Earth.alp[n];
            z0 = a * z1 - z0 + 2 * n * Earth.alp[n];
            --n;
        }
        z1 = double(1) - z1 + a2 * z0;
        y1 = complexd(xip, etap) + a3 * y0;
    }
    // Fold in change in convergence and scale for Gauss-Schreiber TM to
    // Gauss-Krueger TM.
    double const xi  = y1.real();
    double const eta = y1.imag();
    double const y   = Earth.a1 * Earth.k0 * xi * latsign;
    double const x   = Earth.a1 * Earth.k0 * eta * lonsign;
    return pos_enu_m { meters(x), meters(y), p.alt };
}

pos_lla sico::to_lla(radians lon0, pos_enu_m const& p)
{
    double const x    = raw(p.east);
    double const y    = raw(p.north);
    double const xi00 = y / (Earth.a1 * Earth.k0);
    double const eta0 = x / (Earth.a1 * Earth.k0);
    // Explicitly enforce the parity
    int const    xisign  = (xi00 < 0) ? -1 : 1;
    int const    etasign = (eta0 < 0) ? -1 : 1;
    double const xi0     = xi00 * xisign;
    double const eta     = eta0 * etasign;
    bool const   back    = xi0 > SICO_PI2;
    double const xi      = back ? SICO_PI - xi0 : xi0;
    double const c2xi    = cos(2 * xi);
    double const ch2e    = cosh(2 * eta);
    double const s2xi    = sin(2 * xi);
    double const sh2e    = sinh(2 * eta);

    using complexd = std::complex<double>;
    complexd a(2 * c2xi * ch2e, -2 * s2xi * sh2e); // 2 * cos(2*zeta)
    int      n = 6;
    complexd y0(n & 1 ? -Earth.bet[n] : 0), y1, // default initializer is 0+i0
        z0(n & 1 ? -2 * n * Earth.bet[n] : 0), z1;
    if (n & 1)
        --n;
    while (n) {
        y1 = a * y0 - y1 - Earth.bet[n];
        z1 = a * z0 - z1 - 2 * n * Earth.bet[n];
        --n;
        y0 = a * y1 - y0 - Earth.bet[n];
        z0 = a * z1 - z0 - 2 * n * Earth.bet[n];
        --n;
    }
    a /= double(2); // cos(2*zeta)
    z1 = double(1) - z1 + a * z0;
    a  = complexd(s2xi * ch2e, c2xi * sh2e); // sin(2*zeta)
    y1 = complexd(xi, eta) + a * y0;
    // Convergence and scale for Gauss-Schreiber TM to Gauss-Krueger TM.
    double       gamma = atan2(z1.imag(), z1.real());
    double       k     = Earth.b1 / abs(z1);
    double const xip   = y1.real();
    double const etap  = y1.imag();
    double const s     = sinh(etap);
    double const c     = std::max(0.0, cos(xip)); // cos(pi/2) might be negative
    using std::hypot;
    double const r = hypot(s, c);
    double       lon, lat;
    if (r != 0) {
        lon               = atan2(s, c);
        double const sxip = sin(xip);
        double const tol  = sqrt(std::numeric_limits<double>::epsilon()) / 10.0;
        double const taup = sxip / r;
        double       tau  = taup / Earth.e2m;
        double const stol = tol * std::max(1.0, abs(taup));
        for (int i = 0; i < 5; ++i) {
            double const taupa = taupf(tau);
            double const dtau  = (taup - taupa) * (1.0 + Earth.e2m * (tau * tau))
                / (Earth.e2m * std::hypot(1.0, tau) * std::hypot(1.0, taupa));
            tau += dtau;
            if (!(abs(dtau) >= stol))
                break;
        }

        gamma += atan2(sxip * tanh(etap), c); // Krueger p 19 (31)
        lat = atan(tau);
        // Note cos(phi') * cosh(eta') = r
        k *= sqrt(Earth.e2m + Earth.e2 / (1 + (tau * tau))) * hypot(1.0, tau) * r;
    }
    else {
        lat = SICO_PI2;
        lon = 0;
        k *= Earth.c;
    }
    lat *= xisign;
    if (back) {
        lon = SICO_PI - lon;
    }
    lon *= etasign;
    pos_lla lla;
    lla.lon = normalizeAnglePi(radians(lon) + lon0);
    lla.lat = radians(lat);
    lla.alt = p.up;
    return lla;
}