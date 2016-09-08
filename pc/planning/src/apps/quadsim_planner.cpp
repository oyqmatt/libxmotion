/*
 * quadsim_planner.cpp
 *
 *  Created on: Sep 7, 2016
 *      Author: rdu
 */

#include <string>
#include <memory>

// headers for lcm
#include <lcm/lcm-cpp.hpp>

#include "map/graph_builder.h"
#include "map/sgrid_builder.h"
#include "map/map_utils.h"
#include "map/map_config.h"
#include "map/map_info.h"
#include "planner/rrts_planner.h"
#include "planner/quad_planner.h"
#include "common/planning_types.h"

using namespace srcl_ctrl;

int main(int argc, char* argv[])
{
	// set up network first
	std::shared_ptr<lcm::LCM> lcm = std::make_shared<lcm::LCM>();
	if(!lcm->good())
	{
		std::cerr << "ERROR: Failed to initialize LCM." << std::endl;
		return 0;
	}

	// init quadrotor planner
	std::string image_dir = "/home/rdu/Workspace/srcl_rtk/srcl_ctrl/pc/planning/data/map_path_repair.png";

	QuadPlanner qplanner(lcm);
	MapConfig map_config;

	map_config.SetMapPath(image_dir);
	map_config.SetMapType(MapDataModel::SQUARE_GRID, 32);
	//map_config.SetMapType(MapDataModel::QUAD_TREE, 6);
	map_config.SetOriginOffset(2.5, 2.5);

	qplanner.ConfigGraphPlanner(map_config);
	qplanner.SetRealWorldSize(5.0, 5.0);
	qplanner.EnablePositionAutoUpdate(true);

	qplanner.SetGoalRefWorldPosition(Position2Dd(1.8, -2.0));

	if(qplanner.active_graph_planner_ == GraphPlannerType::NOT_SPECIFIED)
	{
		std::cout << "failed to init quad planner" << std::endl;
		return -1;
	}

	while(true)
	{
		lcm->handleTimeout(0);
	}
}


