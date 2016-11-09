/*
 * mission_tracker.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: rdu
 */

#include "mission/mission_tracker.h"

using namespace srcl_ctrl;

MissionTracker::MissionTracker(std::shared_ptr<lcm::LCM> lcm):
		lcm_(lcm),
		mission_started_(false),
		replan_needed_(true),
		remaining_path_length_(std::numeric_limits<double>::infinity()),
		path_id_(0),
		trajectory_id_(0)
{
		lcm_->subscribe("quad_ctrl/mission_info",&MissionTracker::LcmMissionInfoHandler, this);
};

double MissionTracker::CalcRemainingPathLenght(uint32_t current_idx)
{
	double dist = 0;
	for(int i = current_idx; i < active_path_.size() - 1; i++)
	{
		Position3Dd pos1 = active_path_[i]->bundled_data_.position;
		Position3Dd pos2 = active_path_[i+1]->bundled_data_.position;
		dist += std::sqrt(std::pow(pos1.x - pos2.x, 2) +
				std::pow(pos1.y - pos2.y, 2) + std::pow(pos1.z - pos2.z, 2));
	}
	std::cout << "remaining path length: " << dist << std::endl;

	return dist;
}

void MissionTracker::UpdateActivePathWaypoints(Path_t<GeoMark>& path)
{
	path_id_++;
	active_path_ = path;
	mission_started_ = true;
	replan_needed_ = true;

	double cost = 0;
	for(int i = 0; i < active_path_.size() - 1; i++)
	{
		Position3Dd pos1 = active_path_[i]->bundled_data_.position;
		Position3Dd pos2 = active_path_[i+1]->bundled_data_.position;
		cost += std::sqrt(std::pow(pos1.x - pos2.x, 2) +
				std::pow(pos1.y - pos2.y, 2) + std::pow(pos1.z - pos2.z, 2));
	}
	std::cout << "remaining path length: " << cost << std::endl;
};

//void MissionTracker::UpdateCurrentPosition(Position3Dd pos)
//{
//	current_position_ = pos;
//
//	if(mission_started_)
//	{
//		//uint32_t vtx_idx = FindNearestNextWaypoint();
////		if(vtx_idx == active_path_.size() - 1)
////		{
////			replan_needed_ = false;
////			remaining_path_length_ = 0;
////		}
////		else
////		{
////			UpdateRemainingPathLenght(vtx_idx);
////		}
//	}
//}

void MissionTracker::LcmMissionInfoHandler(
		const lcm::ReceiveBuffer* rbuf,
		const std::string& chan,
		const srcl_lcm_msgs::MissionInfo_t* msg)
{
	if(msg->trajectory_id == path_id_)
	{
		remaining_path_length_ = CalcRemainingPathLenght(msg->next_wp_id);//msg->dist_to_goal;
	}
}
