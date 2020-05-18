# sico frames

It is possible to convert directly from *LLA* to *ECEF*, but other coordinates require a reference frame.

The frames are simple wrapper classes around the conversion functions, and keeping the references.

## local tangent

This frames converts a geodetic *LLA* position to an *east-north-up* local position, relative to local tangent plane centered on the reference point.

See [frames/local_tangent.hpp](include/sico/frames/local_tangent.hpp).

## mercator

This frames converts a geodetic *LLA* position to an *east-north-up* local position, relative to mercator projection centered on the reference longitude.

See [frames/mercator.hpp](include/sico/frames/mercator.hpp).

## object

This frames converts a geodetic *LLA* position or an *east-north-up* to a local position, relative to the position and orientation of the object.

See [frames/object_local.hpp](include/sico/frames/object_local.hpp).

## child object

This frames converts a local position to a local position, relative to the position and orientation of the parent object.

For example, to obtain the relative position of the wheel of a car, itself on a ferry boat.

See [frames/object_local.hpp](include/sico/frames/object_local.hpp).