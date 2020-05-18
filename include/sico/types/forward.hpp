#pragma once

namespace sico {

// forward declarations of every types

template<typename Unit>
struct vec_ecef;

template<typename Unit, typename Ref>
struct vec_enu;
struct ref_enu;
struct ref_tangent;
struct ref_mercator;

struct pos_lla;

template<typename Unit>
struct vec_local;
struct ref_local;

template<typename Unit>
struct vec_ned;

template<typename Ref>
struct ori;
template<typename Ref>
struct quat;

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//