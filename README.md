# sico

![Ubuntu](https://github.com/fjacomme/sico/workflows/Ubuntu/badge.svg)
![Windows](https://github.com/fjacomme/sico/workflows/Windows/badge.svg)

## overview

The **Simulation Coordinates** library offers:

* Strong types for units and coordinates
* Conversion between coordinate systems (Geocentric, Geodetic, UTM...)
* Strong conventions to work within

This library can be used to:

* do conversions between geographic coordinates
* integrate a 3D engine into a distributed simulation
* develop simulation components

## motivation

In distributed (or even after a certain size monolotic) simulations,
a common issue is making sure every simulated entity is in the same coordinate system, with the same unit.

A classic error is to use a sort of `Vector3D` for each and every position, velocity, etc... 

This leads to adding meters to radians and divide them by feet.

This also leads to lots of headaches regarding directions: is `x` the front or the right ? is `y` up ?

The goal of **sico** is to offer the basic types to prevent such mistakes, with the help of the compiler.

It goes further by providing operations on and conversion between those types.

This library **is not** meant to be a comprehensive set of geographical conversion functions (see *GeographicLib*
or *Sedris-SRM* for this).

This library is meant to offer the basic default to work within the latest good practices and conventions in the
simulation world.

## examples

Simulating a basic movement:
```C++
#include "sico/types/enu.hpp"
#include "sico/types/lla.hpp"

using namespace sico;

void on_gps_position_received(double longitude_deg, double latitude_deg, double altitude_m)
{
    // going to safe types
    pos_lla const pos{ 
        degrees(latitude_deg), 
        degrees(longitude_deg), 
        meters(altitude_m) 
    };

    // some fake computation
    seconds const elapsed_time = time_since_last_pos();
    vel_enu const velocity { meters_ps(12.3), meters_ps(12.3), meters_ps(1.2) };

    pos_lla const new_pos = pos + (velocity * elapsed_time);
    // ...
}

```

Converting between coordinates systems:
```C++
#include "sico/sico.hpp"

using namespace sico;
using namespace sico::literals;

void main()
{
    pos_lla const radar_pos { 42_deg, 2_deg, 123_m };
    ori_enu const radar_ori { 346_deg, 0_deg, 0_deg };
    pos_local const antenna_pos { 0_m, -3_m, 5_m };
    ori_local const antenna_ori { 45_deg, 30_deg, 0_deg };

    pos_lla const plane_pos { 42.1_deg, 2.1_deg, 12345_m };

    frame_enu local_tangent(radar_pos);
    pos_enu const radar_to_plane = local_tangent.to_enu(plane_pos);

    frame_object radar_frame(radar_pos, radar_ori);
    frame_child_object antenna_frame(antenna_pos, antenna_ori);

    pos_local const plane_from_radar = radar_frame.to_local(plane_pos);
    pos_local const plane_from_antenna = antenna_frame.to_child(plane_from_radar);
}

```

## documentation

More information about:

* [types](doc/types.md)
* [frames](doc/frames.md)

## build

By default, **sico** requires nothing more than cmake and a **c++14** compiler.

To build the tests, you need [Catch2](https://github.com/catchorg/Catch2), and run
cmake with `-DSICO_BUILD_TESTS=ON`.

Just extract the release in a `deps/Catch2-2.12.1` folder.

If you want to use [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) 
instead of the included linear algebra function, you can extract the release in
a `deps/eigen-3.3.7` folder and run cmake with `-DSICO_USE_EIGEN=ON`.

If you want to use [N.Holthaus Units](https://github.com/nholthaus/units) 
instead of the included units types, you can extract the release in
a `deps/units-3.0.0.alpha-2` folder and run cmake with `-DSICO_USE_HOLTHAUS_UNITS=ON`.

If you want to use [Boost Units](https://www.boost.org/doc/libs/1_73_0/doc/html/boost_units.html) 
instead of the included units types, you can install boost and
run cmake with `-DSICO_USE_BOOST_UNITS=ON`.

## TODO

* C wrappers
* angular acceleration types
