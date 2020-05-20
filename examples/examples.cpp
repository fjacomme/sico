#include "sico/sico.hpp"
#include "sico/types/stream.hpp"
#include <iostream>

using namespace sico;

int main()
{
    using namespace sico::literals;
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

    std::cout << "radar_pos:      " << radar_pos << "\n";
    std::cout << "radar_ori:      " << radar_ori << "\n";
    std::cout << "antenna_pos:    " << antenna_pos << "\n";
    std::cout << "antenna_ori:    " << antenna_ori << "\n";
    std::cout << "plane_pos:      " << plane_pos << "\n";
    std::cout << "radar_to_plane: " << radar_to_plane << "\n";
    std::cout << "plane_from_rad: " << plane_from_radar << "\n";
    std::cout << "plane_from_ant: " << plane_from_antenna << "\n";
    return 0;
}

void on_gps_position_received(double longitude_deg, double latitude_deg, double altitude_m)
{
    // going to safe types
    pos_lla const pos{ 
        degrees(latitude_deg), 
        degrees(longitude_deg), 
        meters(altitude_m) 
    };

    // some fake computation
    seconds const elapsed_time(0.02);
    vel_enu const velocity { meters_ps(12.3), meters_ps(12.3), meters_ps(1.2) };

    pos_lla const new_pos = pos + (velocity * elapsed_time);
    // ...
}