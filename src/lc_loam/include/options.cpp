#include "options.h"

#include <vector>
#include <memory>
#include "absl/memory/memory.h"
#include "config/configuration_file_resolver.h"
#include "glog/logging.h"

namespace common
{

    NodeOptions CreateNodeOptions(
    common::LuaParameterDictionary* const
        lua_parameter_dictionary) {

        LidarOptions lidar_param;
        OdomOptions odom_options;
        LoopClosureOptions loop_closureOptions;
        PoseGraphOptions pose_graph_options;
        lidar_param.max_distance =
            lua_parameter_dictionary->GetDouble("max_distance");
        lidar_param.min_distance =
                lua_parameter_dictionary->GetDouble("min_distance");
        lidar_param.num_lines =
                lua_parameter_dictionary->GetInt("scan_lines");
        lidar_param.scan_period =
            lua_parameter_dictionary->GetDouble("scan_period");
        lidar_param.vertical_angle =
            lua_parameter_dictionary->GetDouble("vertical_angle");

        odom_options.map_resolution_edge =
             lua_parameter_dictionary->GetDouble("map_resolution_edge");
        odom_options.map_resolution_surf =
             lua_parameter_dictionary->GetDouble("map_resolution_surf");
        odom_options.num_range_data =
            lua_parameter_dictionary->GetInt("num_range_data");
        odom_options.num_thread_pool =
            lua_parameter_dictionary->GetInt("num_thread_pool");
        odom_options.speed_filter =
            lua_parameter_dictionary->GetDouble("speed_filter");
        odom_options.scan_period =
                                 lidar_param.scan_period;

        loop_closureOptions.distance_threshold =
            lua_parameter_dictionary->GetDouble("distance_threshold");
        loop_closureOptions.loop_closure_rotation_weight =
            lua_parameter_dictionary->GetDouble("loop_closure_rotation_weight");
        loop_closureOptions.loop_closure_translation_weight =
            lua_parameter_dictionary->GetDouble("loop_closure_translation_weight");
        loop_closureOptions.submap_threshold =
            lua_parameter_dictionary->GetInt("submap_threshold");
        loop_closureOptions.node_id_threshold =
            lua_parameter_dictionary->GetInt("node_id_threshold");
        loop_closureOptions.scan_to_scan_min =
            lua_parameter_dictionary->GetDouble("scan_to_scan_min");
        loop_closureOptions.num_of_submap_threshold =
            lua_parameter_dictionary->GetInt("num_of_submap_threshold");
        loop_closureOptions.num_range_data = odom_options.num_range_data;

        pose_graph_options.loop_closureOptions = loop_closureOptions;
        pose_graph_options.matcher_rotation_weight =
            lua_parameter_dictionary->GetDouble("matcher_rotation_weight");
        pose_graph_options.matcher_translation_weight =
            lua_parameter_dictionary->GetDouble("matcher_translation_weight");
        pose_graph_options.optimize_every_n_nodes =
            lua_parameter_dictionary->GetInt("optimize_every_n_nodes");
        pose_graph_options.fix_z_in_3d =
            lua_parameter_dictionary->GetBool("fix_z_in_3d");

        return NodeOptions{lidar_param, odom_options, pose_graph_options};
    }

    NodeOptions LoadOptions(const std::string &configuration_directory,
                        const std::string &configuration_basename){
      auto file_resolver =
      absl::make_unique<common::ConfigurationFileResolver>(
          std::vector<std::string>{configuration_directory});

    const std::string code =
      file_resolver->GetFileContentOrDie(configuration_basename);

      common::LuaParameterDictionary lua_parameter_dictionary(
      code, std::move(file_resolver));

      return CreateNodeOptions(&lua_parameter_dictionary);
}

} // namespace common
