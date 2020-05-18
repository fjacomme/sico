# sico types

## units

**sico** uses strong typing to enforce the correct usage of different units.

**sico** uses [SI units](https://en.wikipedia.org/wiki/International_System_of_Units):

* meters
* radians
* seconds
* etc.

The types provided in **sico** are only the most basic required by the libray.

If you can spare the compile time to enforce a more thorough dimensional analysis, **sico** can use :

* boost::units (cmake option *SICO_USE_BOOST_UNITS*)
* [nick holthaus's units library](https://github.com/nholthaus/units) (cmake option *SICO_USE_HOLTHAUS_UNITS*)

But hopefully we will have this feature standardized by **C++23**...

## coordinates

The coordinates in **sico** are defined using simple structs, with the following guidelines:

* strong types (meters, radians...)
* explicit field names (east, front... instead of x,y,z)
* simple and concise
* basic operators only when it makes sense

### geodetic

The triplet latitude/longitude/altitude is nowadays the most common way to position something of Earth.

The `pos_lla` type is defined in [types/lla.hpp](include/sico/types/lla.hpp).

The geoditic system used is [WGS84](https://en.wikipedia.org/wiki/World_Geodetic_System#WGS84).

### east-north-up

When working locally, it is preferable to have a more intuitive coordinate system.

These coordinates define a point as an offset in meters from another reference point and positive along the three axis east, north, and up.

![ENU](https://en.wikipedia.org/wiki/Local_tangent_plane_coordinates#/media/File:ECEF_ENU_Longitude_Latitude_relationships.svg)

These coordinates are used:

* in [Local Tangent Plane](https://en.wikipedia.org/wiki/Local_tangent_plane_coordinates)
* in [Mercator Projection](https://en.wikipedia.org/wiki/Mercator_projection)

The main difference lies in taking into account the earth curvature. 

If for example you have an aircraft moving along the *east* axis:

* in *local tangent*, after a long distance away from the reference point its real altitude would have increased, and this may be confusion because it velocity would have no *up* component.
* in *mercator*, the altitude will stay constant, but you may have some issues regarding distortion.

The choice depends mainly of the use case:
* use *local* tangent when working locally
* for 2D (or 3D for flat-earther) use mercator

For example, when positioning an entity into *Unreal Engine*, I would choose depending on my terrain size and curvature (or lack thereof).

The `pos_enu`, `vel_enu`, `acc_enu` (and `pos_enu_m`, `vel_enu_m`, `acc_enu_m` for mercator) types are defined
in [types/enu.hpp](include/sico/types/lla.hpp).

### local

We often have to define a position relative to the center of an object, for example a wheel, an antenna...

To stay coherent with *ENU*, **sico** uses the *right-front-up* system.

The `pos_local`, `vel_local` and `acc_local` types are defined in [types/enu.hpp](include/sico/types/local.hpp).

### other coordinates

For ease of integration with other systems, **sico** also offers types for:

 * [Earth-Centered-Earth-Fixed](https://en.wikipedia.org/wiki/ECEF)
 * *North-East-Down*
 * [UTM](https://en.wikipedia.org/wiki/Universal_Transverse_Mercator_coordinate_system)

## orientation

The convention in simulation is to use Euler Tait-Bryan angles.

**sico** defines the *yaw-pitch-roll* orientation using the [aeronautic conventions](https://en.wikipedia.org/wiki/Aircraft_principal_axes).

**sico** also provides a quaternion structure (for efficiency).

These types exists in two reference frames:

* `ori_enu` (and `quat_enu`), where
    * the yaw is the compass heading (0° north, 90° east)
    * the pitch is the angle from the east-north plane, positive up
    * the roll is the bank angle, positive clockwise
* `ori_local` (and `quat_local`), with the same conventions but around the up, right and front axis.

The common traits of these types are defined in [types/orientations.hpp](include/sico/types/orientations.hpp),
and their respective definitions in `enu.hpp` and `local.hpp`.
