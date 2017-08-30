/*
 * path_manager.h
 *
 *  Created on: Oct 31, 2016
 *      Author: rdu
 */

#ifndef PLANNING_SRC_MISSION_PATH_MANAGER_H_
#define PLANNING_SRC_MISSION_PATH_MANAGER_H_

#include <vector>
#include <memory>
#include <cstdint>

// headers for lcm
#include <lcm/lcm-cpp.hpp>
#include "lcmtypes/librav.hpp"

#include "common/planning_types.h"
#include "planning/polyopt/quad_polyopt.h"

namespace librav {

class PathManager {
public:
	PathManager(std::shared_ptr<lcm::LCM> lcm);
	~PathManager() = default;

private:
	std::shared_ptr<lcm::LCM> lcm_;
	uint64_t user_path_id_;
	double CalcFlightTime(Position3Dd start, Position3Dd goal, double vel);
	std::vector<Position3Dd> GetKeyTurningWaypoints(std::vector<Position3Dd>& wps);

private:
	void LcmWaypointsHandler(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const srcl_lcm_msgs::Path_t* msg);
	void LcmKeyframeSetHandler(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const srcl_lcm_msgs::KeyframeSet_t* msg);

public:
	void GenerateTrajectory(KeyframeSet& kfs, uint64_t traj_id);
};

}

#endif /* PLANNING_SRC_MISSION_PATH_MANAGER_H_ */