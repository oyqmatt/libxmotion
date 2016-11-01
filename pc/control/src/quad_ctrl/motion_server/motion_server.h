/*
 * motion_server.h
 *
 *  Created on: May 23, 2016
 *      Author: rdu
 */

#ifndef CONTROL_SRC_QUAD_CTRL_MOTION_SERVER_MOTION_SERVER_H_
#define CONTROL_SRC_QUAD_CTRL_MOTION_SERVER_MOTION_SERVER_H_

#include <vector>
#include <memory>

#include <lcm/lcm-cpp.hpp>
#include "lcmtypes/comm.hpp"

#include "common/control_types.h"
#include "quad_ctrl/motion_server/quad_poly_traj_handler.h"
#include "quad_ctrl/motion_server/waypoint_path_handler.h"

namespace srcl_ctrl {

enum class MotionMode {
	POLYNOMIAL,
	WAYPOINTS,
	USER_CMDS,
	POS_STEP_RESPONSE
};

class MotionServer {
public:
	MotionServer();
	MotionServer(std::shared_ptr<lcm::LCM> lcm);
	~MotionServer();

private:
	std::shared_ptr<lcm::LCM> lcm_;
	std::unique_ptr<QuadPolyTrajHandler> polytraj_handler_;
	std::unique_ptr<WaypointPathHandler> wppath_handler_;

private:
	bool goal_completed_;
	UAVTrajectory active_goal_;
	uint64_t ms_count_;
	uint64_t waypoint_idx_;
	time_stamp current_sys_time_;

	MotionMode mode_;

private:
	UAVTrajectory GenerateTestTrajectory();

public:
	void SetMotionMode(MotionMode mode) { mode_ = mode; };
	void LcmSysTimeHandler(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const srcl_lcm_msgs::TimeStamp_t* msg);
	UAVTrajectoryPoint GetCurrentDesiredState(time_stamp t);

	void LcmUserGoalHandler(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const srcl_lcm_msgs::UAVTrajectory_t* msg);
	void SetMotionGoal(UAVTrajectory& goal);
	void AbortActiveMotion();
	double ReportActiveMotionProgress();
	void SetGoalCompleted();
	UAVTrajectoryPoint GetCurrentUserDefinedPose();
};

}


#endif /* CONTROL_SRC_QUAD_CTRL_MOTION_SERVER_MOTION_SERVER_H_ */
