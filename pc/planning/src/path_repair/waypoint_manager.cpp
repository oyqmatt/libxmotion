/*
 * waypoint_manager.cpp
 *
 *  Created on: Oct 31, 2016
 *      Author: rdu
 */

#include <iostream>
#include <cmath>

#include "eigen3/Eigen/Core"
#include "eigen3/Eigen/Geometry"

#include "path_repair/waypoint_manager.h"

using namespace srcl_ctrl;

WaypointManager::WaypointManager(std::shared_ptr<lcm::LCM> lcm):
		lcm_(lcm)
{
	lcm_->subscribe("quad_planner/geo_mark_graph_path",&WaypointManager::LcmWaypointsHandler, this);
}

WaypointManager::~WaypointManager()
{

}

std::vector<Position3Dd> WaypointManager::WaypointSelector(std::vector<Position3Dd>& wps)
{
	std::vector<Position3Dd> smoothed_points;

	// first remove undesired points at the connections between 2d/3d geomarks
	for(auto it = wps.begin(); it != wps.end() - 1; ++it)
	{
		Position3Dd pt1 = *it;
		Position3Dd pt2 = *(it+1);

		smoothed_points.push_back(pt1);

		// fix the connections between 2d and 3d vertices
		if(std::abs(pt1.z - pt2.z) > 0.05)
		{
			// ignore the last point if there is a 2d/3d connection
			//	between the last two points
			if(it + 2 != wps.end())
			{
				Position3Dd pt3 = *(it + 2);

				Eigen::Vector3d v1(pt2.x - pt1.x, pt2.y - pt1.y, pt2.z - pt1.z);
				Eigen::Vector3d v2(pt3.x - pt2.x, pt3.y - pt2.y, pt3.z - pt2.z);

				// skip next point if direction is opposite
				if(v1.dot(v2) <= 0)
					it++;
			}
		}
		else if(it + 2 == wps.end())
			smoothed_points.push_back(pt2);
	}

	// std::cout << "selected points: " << smoothed_points.size() << std::endl;

	// then remove intermediate points in a straight line
	std::vector<Position3Dd> minimum_points;

	if(smoothed_points.size() <= 2)
	{
		minimum_points = smoothed_points;
	}
	else
	{
		// add first waypoint
		minimum_points.push_back(smoothed_points.front());
		// check intermediate waypoints
		for(int cid = 1; cid < smoothed_points.size() - 1; cid++)
		{
			Position3Dd pt1 = smoothed_points[cid - 1];
			Position3Dd pt2 = smoothed_points[cid];
			Position3Dd pt3 = smoothed_points[cid + 1];

			Eigen::Vector3d v1 = Eigen::Vector3d(pt2.x - pt1.x, pt2.y - pt1.y, pt2.z - pt1.z).normalized();
			Eigen::Vector3d v2 = Eigen::Vector3d(pt3.x - pt2.x, pt3.y - pt2.y, pt3.z - pt2.z).normalized();
			Eigen::Vector3d e = v1 - v2;

			// |e| = sqrt[sin(theta)^2 + (1 - cos(theta))^2], |e| ~= 0.082 when theta = 5 degree
			if(e.norm() > 0.082)
			{
				minimum_points.push_back(smoothed_points[cid]);
			}
		}
		// add last waypoint
		minimum_points.push_back(smoothed_points.back());
	}

	// std::cout << "minimum waypoints: " << minimum_points.size() << std::endl;

	srcl_lcm_msgs::Path_t path_msg;

	path_msg.waypoint_num = minimum_points.size();
	for(auto& wp : minimum_points)
	{
		srcl_lcm_msgs::WayPoint_t waypoint;
		waypoint.positions[0] = wp.x;
		waypoint.positions[1] = wp.y;
		waypoint.positions[2] = wp.z;

		path_msg.waypoints.push_back(waypoint);
	}

	lcm_->publish("quad_planner/geomark_wp_selected", &path_msg);

	return minimum_points;
}

void WaypointManager::LcmWaypointsHandler(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const srcl_lcm_msgs::Path_t* msg)
{
	std::cout << "waypoints received: " << msg->waypoint_num << std::endl;

	//std::vector<Position3Dd> waypoints;
	last_path_.clear();
	for(int i = 0; i < msg->waypoint_num; i++)
		last_path_.push_back(Position3Dd(msg->waypoints[i].positions[0],msg->waypoints[i].positions[1],msg->waypoints[i].positions[2]));

	std::vector<Position3Dd> opt_wps = WaypointSelector(last_path_);
	CalcTrajFromWaypoints(opt_wps);
}

void WaypointManager::CalcTrajFromWaypoints(std::vector<Position3Dd>& wps)
{
	uint8_t kf_num = wps.size();

	if(kf_num < 2)
		return;

	//	traj_opt_.InitOptJointMatrices(kf_num);
	traj_opt_.InitOptWithCorridorJointMatrices(kf_num, 20, 0.01);
	Eigen::Vector3d goal_vec(wps.back().x, wps.back().y, 0);

	for(int i = 0; i < wps.size(); i++)
	{
		traj_opt_.keyframe_x_vals_(0,i) = wps[i].x;
		traj_opt_.keyframe_y_vals_(0,i) = wps[i].y;
		traj_opt_.keyframe_z_vals_(0,i) = wps[i].z;

		traj_opt_.keyframe_x_vals_(1,i) = std::numeric_limits<float>::infinity();
		traj_opt_.keyframe_y_vals_(1,i) = std::numeric_limits<float>::infinity();
		traj_opt_.keyframe_z_vals_(1,i) = std::numeric_limits<float>::infinity();

		traj_opt_.keyframe_x_vals_(2,i) = std::numeric_limits<float>::infinity();
		traj_opt_.keyframe_y_vals_(2,i) = std::numeric_limits<float>::infinity();
		traj_opt_.keyframe_z_vals_(2,i) = std::numeric_limits<float>::infinity();

		traj_opt_.keyframe_x_vals_(3,i) = std::numeric_limits<float>::infinity();
		traj_opt_.keyframe_y_vals_(3,i) = std::numeric_limits<float>::infinity();
		traj_opt_.keyframe_z_vals_(3,i) = std::numeric_limits<float>::infinity();

		if((i == 0) || (i == wps.size() - 1))
			traj_opt_.keyframe_yaw_vals_(0,i) = 0;
		else {
			Eigen::Vector3d pos_vec(wps[i].x, wps[i].y, 0);
			Eigen::Vector3d dir_vec = goal_vec - pos_vec;
			Eigen::Vector3d x_vec(1,0,0);
			double angle = - std::acos(dir_vec.normalized().dot(x_vec));
//			std::cout << "angle: " << angle << std::endl;
//			std::cout << "goal vector: " << goal_vec(0) << " , " << goal_vec(1) << " , " << goal_vec(2) << std::endl;
//			std::cout << "pos vector: " << pos_vec(0) << " , " << pos_vec(1) << " , " << pos_vec(2) << std::endl;
//			std::cout << "dir vector: " << dir_vec(0) << " , " << dir_vec(1) << " , " << dir_vec(2) << std::endl;
			traj_opt_.keyframe_yaw_vals_(0,i) = angle;
		}
		traj_opt_.keyframe_yaw_vals_(1,i) = std::numeric_limits<float>::infinity();

		traj_opt_.keyframe_ts_(0,i) = i * 0.5;
	}

	traj_opt_.keyframe_x_vals_(1,0) = 0.0;
	traj_opt_.keyframe_y_vals_(1,0) = 0.0;
	traj_opt_.keyframe_z_vals_(1,0) = 0.0;

	traj_opt_.keyframe_x_vals_(2,0) = 0.0;
	traj_opt_.keyframe_y_vals_(2,0) = 0.0;
	traj_opt_.keyframe_z_vals_(2,0) = 0.0;

	traj_opt_.keyframe_x_vals_(3,0) = 0.0;
	traj_opt_.keyframe_y_vals_(3,0) = 0.0;
	traj_opt_.keyframe_z_vals_(3,0) = 0.0;

	traj_opt_.keyframe_yaw_vals_(1,0) = 0;

	traj_opt_.keyframe_x_vals_(1,kf_num - 1) = 0.0;
	traj_opt_.keyframe_y_vals_(1,kf_num - 1) = 0.0;
	traj_opt_.keyframe_z_vals_(1,kf_num - 1) = 0.0;

	traj_opt_.keyframe_x_vals_(2,kf_num - 1) = 0.0;
	traj_opt_.keyframe_y_vals_(2,kf_num - 1) = 0.0;
	traj_opt_.keyframe_z_vals_(2,kf_num - 1) = 0.0;

	traj_opt_.keyframe_x_vals_(3,kf_num - 1) = 0;
	traj_opt_.keyframe_y_vals_(3,kf_num - 1) = 0;
	traj_opt_.keyframe_z_vals_(3,kf_num - 1) = 0;

	traj_opt_.keyframe_yaw_vals_(1,kf_num - 1) = 0;

	//traj_opt_.OptimizeFlatTrajJoint();
	traj_opt_.OptimizeFlatTrajWithCorridorJoint();

	// send results to LCM network
//	srcl_msgs::Path_t path_msg;
//
//	path_msg.waypoint_num = wps.size();
//	for(auto& wp : wps)
//	{
//		srcl_msgs::WayPoint_t waypoint;
//		waypoint.positions[0] = wp.x;
//		waypoint.positions[1] = wp.y;
//		waypoint.positions[2] = wp.z;
//
//		path_msg.waypoints.push_back(waypoint);
//	}
//
//	lcm_->publish("quad_planner/geomark_wp_selected", &path_msg);

	srcl_lcm_msgs::PolynomialCurve_t poly_msg;

	poly_msg.seg_num = traj_opt_.flat_traj_.traj_segs_.size();
	for(auto& seg : traj_opt_.flat_traj_.traj_segs_)
	{
		srcl_lcm_msgs::PolyCurveSegment_t seg_msg;

		seg_msg.coeffsize_x = seg.seg_x.param_.coeffs.size();
		seg_msg.coeffsize_y = seg.seg_y.param_.coeffs.size();
		seg_msg.coeffsize_z = seg.seg_z.param_.coeffs.size();
		seg_msg.coeffsize_yaw = seg.seg_yaw.param_.coeffs.size();
		for(auto& coeff:seg.seg_x.param_.coeffs)
			seg_msg.coeffs_x.push_back(coeff);
		for(auto& coeff:seg.seg_y.param_.coeffs)
			seg_msg.coeffs_y.push_back(coeff);
		for(auto& coeff:seg.seg_z.param_.coeffs)
			seg_msg.coeffs_z.push_back(coeff);
		for(auto& coeff:seg.seg_yaw.param_.coeffs)
			seg_msg.coeffs_yaw.push_back(coeff);

		seg_msg.t_start = seg.t_start;
		seg_msg.t_end = seg.t_end;

		poly_msg.segments.push_back(seg_msg);
	}

	lcm_->publish("quad_planner/trajectory_polynomial", &poly_msg);
}


