/*
 * quad_path_repair.cpp
 *
 *  Created on: Sep 9, 2016
 *      Author: rdu
 */

#include <iostream>

#include "path_repair/quad_path_repair.h"
#include "map/map_utils.h"

using namespace srcl_ctrl;

QuadPathRepair::QuadPathRepair():
		active_graph_planner_(GraphPlannerType::NOT_SPECIFIED),
		gstart_set_(false),
		ggoal_set_(false),
		world_size_set_(false),
		auto_update_pos_(true),
		update_global_plan_(false)
{

}

QuadPathRepair::QuadPathRepair(std::shared_ptr<lcm::LCM> lcm):
		lcm_(lcm),
		active_graph_planner_(GraphPlannerType::NOT_SPECIFIED),
		gstart_set_(false),
		ggoal_set_(false),
		world_size_set_(false),
		auto_update_pos_(true),
		update_global_plan_(false)
{
	if(!lcm_->good())
		std::cerr << "ERROR: Failed to initialize LCM." << std::endl;
	else {
		lcm_->subscribe("vis_data_quad_transform",&QuadPathRepair::LcmTransformHandler, this);
	}
}

QuadPathRepair::~QuadPathRepair()
{

}

void QuadPathRepair::ConfigGraphPlanner(MapConfig config, double world_size_x, double world_size_y)
{
	qtree_planner_.map_.info.SetWorldSize(world_size_x, world_size_y);
	sgrid_planner_.map_.info.SetWorldSize(world_size_x, world_size_y);
	world_size_set_ = true;

	if(config.GetMapType().data_model == MapDataModel::QUAD_TREE)
	{
		bool result = qtree_planner_.UpdateMapConfig(config);

		if(result)
		{
			std::cout << "quad tree planner activated" << std::endl;
			active_graph_planner_ = GraphPlannerType::QUADTREE_PLANNER;

			//gcombiner_.SetBaseGraph(qtree_planner_.graph_, qtree_planner_.map_.data_model, qtree_planner_.map_.data_model->leaf_nodes_.size(), qtree_planner_.map_.info);
		}
	}
	else if(config.GetMapType().data_model == MapDataModel::SQUARE_GRID)
	{
		bool result = sgrid_planner_.UpdateMapConfig(config);

		if(result)
		{
			std::cout << "square grid planner activated" << std::endl;
			active_graph_planner_ = GraphPlannerType::SQUAREGRID_PLANNER;

			gcombiner_.SetBaseGraph(sgrid_planner_.graph_, sgrid_planner_.map_.data_model, sgrid_planner_.map_.data_model->cells_.size(), sgrid_planner_.map_.info);
		}
	}

	srcl_msgs::Graph_t graph_msg = GenerateLcmGraphMsg();
	lcm_->publish("quad/quad_planner_graph", &graph_msg);
}

void QuadPathRepair::SetStartMapPosition(Position2D pos)
{
	if(pos == start_pos_)
		return;

	start_pos_.x = pos.x;
	start_pos_.y = pos.y;

	gstart_set_ = true;

	if(gstart_set_ && ggoal_set_)
		update_global_plan_ = true;
}

void QuadPathRepair::SetGoalMapPosition(Position2D pos)
{
	goal_pos_.x = pos.x;
	goal_pos_.y = pos.y;

	ggoal_set_ = true;

	if(gstart_set_ && ggoal_set_)
		update_global_plan_ = true;
}

void QuadPathRepair::SetStartRefWorldPosition(Position2Dd pos)
{
	Position2Dd mpos;
	mpos = MapUtils::CoordinatesFromRefWorldToMapWorld(pos, GetActiveMapInfo());

	Position2D map_pos;
	map_pos = MapUtils::CoordinatesFromMapWorldToMap(mpos, GetActiveMapInfo());

	Position2D map_padded_pos;
	map_padded_pos = MapUtils::CoordinatesFromOriginalToPadded(map_pos, GetActiveMapInfo());

	SetStartMapPosition(map_padded_pos);
}

void QuadPathRepair::SetGoalRefWorldPosition(Position2Dd pos)
{
	Position2Dd mpos;
	mpos = MapUtils::CoordinatesFromRefWorldToMapWorld(pos, GetActiveMapInfo());

	Position2D map_pos;
	map_pos = MapUtils::CoordinatesFromMapWorldToMap(mpos, GetActiveMapInfo());

	Position2D map_padded_pos;
	map_padded_pos = MapUtils::CoordinatesFromOriginalToPadded(map_pos, GetActiveMapInfo());

	SetGoalMapPosition(map_padded_pos);
}

std::vector<Position2D> QuadPathRepair::SearchForGlobalPath()
{
	std::vector<Position2D> waypoints;

//	std::cout << "----> start: " << start_pos_.x << " , " << start_pos_.y << std::endl;
//	std::cout << "----> goal: " << goal_pos_.x << " , " << goal_pos_.y << std::endl;

	if(active_graph_planner_ == GraphPlannerType::QUADTREE_PLANNER)
	{
		auto traj_vtx = qtree_planner_.Search(start_pos_, goal_pos_);
		for(auto& wp:traj_vtx)
			waypoints.push_back(wp->bundled_data_->location_);
	}
	else if(active_graph_planner_ == GraphPlannerType::SQUAREGRID_PLANNER)
	{
		auto traj_vtx = sgrid_planner_.Search(start_pos_, goal_pos_);
		for(auto& wp:traj_vtx)
			waypoints.push_back(wp->bundled_data_->location_);
	}

	srcl_msgs::Path_t path_msg = GenerateLcmPathMsg(waypoints);
	lcm_->publish("quad/quad_planner_graph_path", &path_msg);

	update_global_plan_ = false;

	return waypoints;
}

std::vector<uint64_t> QuadPathRepair::SearchForGlobalPathID()
{
	std::vector<uint64_t> waypoints;

//		std::cout << "----> start: " << start_pos_.x << " , " << start_pos_.y << std::endl;
//		std::cout << "----> goal: " << goal_pos_.x << " , " << goal_pos_.y << std::endl;

	if(active_graph_planner_ == GraphPlannerType::QUADTREE_PLANNER)
	{
		auto traj_vtx = qtree_planner_.Search(start_pos_, goal_pos_);
		for(auto& wp:traj_vtx)
			waypoints.push_back(wp->vertex_id_);
	}
	else if(active_graph_planner_ == GraphPlannerType::SQUAREGRID_PLANNER)
	{
		auto traj_vtx = sgrid_planner_.Search(start_pos_, goal_pos_);
		for(auto& wp:traj_vtx)
			waypoints.push_back(wp->vertex_id_);
	}

	update_global_plan_ = false;

	return waypoints;
}

cv::Mat QuadPathRepair::GetActiveMap()
{
	if(active_graph_planner_ == GraphPlannerType::QUADTREE_PLANNER)
	{
		return qtree_planner_.map_.padded_image;
	}
	else if(active_graph_planner_ == GraphPlannerType::SQUAREGRID_PLANNER)
	{
		return sgrid_planner_.map_.padded_image;
	}
	else
		return cv::Mat::zeros(10, 10, CV_8UC1);
}

MapInfo QuadPathRepair::GetActiveMapInfo()
{
	MapInfo empty_info;

	if(active_graph_planner_ == GraphPlannerType::QUADTREE_PLANNER)
	{
		return qtree_planner_.map_.info;
	}
	else if(active_graph_planner_ == GraphPlannerType::SQUAREGRID_PLANNER)
	{
		return sgrid_planner_.map_.info;
	}
	else
		return empty_info;
}

void QuadPathRepair::LcmTransformHandler(
		const lcm::ReceiveBuffer* rbuf,
		const std::string& chan,
		const srcl_msgs::QuadrotorTransform* msg)
{
	Position2Dd rpos;
	rpos.x = msg->base_to_world.position[0];
	rpos.y = msg->base_to_world.position[1];

//	Position2Dd mapw_pos;
//	mapw_pos = MapUtils::CoordinatesFromRefWorldToMapWorld(rpos, GetActiveMapInfo());
//
//	Position2D map_pos;
//	map_pos = MapUtils::CoordinatesFromMapWorldToMap(mapw_pos, GetActiveMapInfo());
//
//	Position2D map_padded_pos;
//	map_padded_pos = MapUtils::CoordinatesFromOriginalToPadded(map_pos, GetActiveMapInfo());

//	std::cout << "quadrotor position in sim: " << msg->base_to_world.position[0] << " , "
//			<< msg->base_to_world.position[1] << " , "
//			<< msg->base_to_world.position[2] << std::endl;
//	std::cout << "quadrotor position in image world: " << mpos.x << " , "
//				<< mpos.y << std::endl;

	if(auto_update_pos_)
	{
		SetStartRefWorldPosition(rpos);
		gcombiner_.UpdateFlightHeight(Position3Dd(msg->base_to_world.position[0],msg->base_to_world.position[1],msg->base_to_world.position[2]),
				Eigen::Quaterniond(msg->base_to_world.quaternion[0] , msg->base_to_world.quaternion[1] , msg->base_to_world.quaternion[2] , msg->base_to_world.quaternion[3]));
	}
}

template<typename PlannerType>
srcl_msgs::Graph_t QuadPathRepair::GetLcmGraphFromPlanner(const PlannerType& planner)
{
	srcl_msgs::Graph_t graph_msg;

	graph_msg.vertex_num = planner.graph_->GetGraphVertices().size();
	for(auto& vtx : planner.graph_->GetGraphVertices())
	{
		srcl_msgs::Vertex_t vertex;
		vertex.id = vtx->vertex_id_;

		Position2Dd ref_world_pos = MapUtils::CoordinatesFromMapPaddedToRefWorld(vtx->bundled_data_->location_, planner.map_.info);
		vertex.position[0] = ref_world_pos.x;
		vertex.position[1] = ref_world_pos.y;

		graph_msg.vertices.push_back(vertex);
	}

	graph_msg.edge_num = planner.graph_->GetGraphUndirectedEdges().size();
	for(auto& eg : planner.graph_->GetGraphUndirectedEdges())
	{
		srcl_msgs::Edge_t edge;
		edge.id_start = eg.src_->vertex_id_;
		edge.id_end = eg.dst_->vertex_id_;

		graph_msg.edges.push_back(edge);
	}

	return graph_msg;
}

srcl_msgs::Graph_t QuadPathRepair::GenerateLcmGraphMsg()
{
	srcl_msgs::Graph_t graph_msg;

	if(active_graph_planner_ == GraphPlannerType::QUADTREE_PLANNER)
	{
		graph_msg = GetLcmGraphFromPlanner(this->qtree_planner_);
	}
	else if(active_graph_planner_ == GraphPlannerType::SQUAREGRID_PLANNER)
	{
		graph_msg = GetLcmGraphFromPlanner(this->sgrid_planner_);
	}

	return graph_msg;
}

srcl_msgs::Path_t QuadPathRepair::GenerateLcmPathMsg(std::vector<Position2D> waypoints)
{
	srcl_msgs::Path_t path_msg;

	if(active_graph_planner_ == GraphPlannerType::QUADTREE_PLANNER)
	{
		path_msg.waypoint_num = waypoints.size();
		for(auto& wp : waypoints)
		{
			Position2Dd ref_world_pos = MapUtils::CoordinatesFromMapPaddedToRefWorld(wp, this->qtree_planner_.map_.info);
			srcl_msgs::WayPoint_t waypoint;
			waypoint.positions[0] = ref_world_pos.x;
			waypoint.positions[1] = ref_world_pos.y;

			path_msg.waypoints.push_back(waypoint);
		}
	}
	else if(active_graph_planner_ == GraphPlannerType::SQUAREGRID_PLANNER)
	{
		path_msg.waypoint_num = waypoints.size();
		for(auto& wp : waypoints)
		{
			Position2Dd ref_world_pos = MapUtils::CoordinatesFromMapPaddedToRefWorld(wp, this->sgrid_planner_.map_.info);
			srcl_msgs::WayPoint_t waypoint;
			waypoint.positions[0] = ref_world_pos.x;
			waypoint.positions[1] = ref_world_pos.y;

			path_msg.waypoints.push_back(waypoint);
		}
	}

	return path_msg;
}
