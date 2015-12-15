/*
 * graph_builder.cpp
 *
 *  Created on: Dec 14, 2015
 *      Author: rdu
 */

#include "graph_builder.h"

using namespace srcl_ctrl;

GraphBuilder::GraphBuilder():
		graph_(nullptr)
{

}

GraphBuilder::~GraphBuilder()
{

}

Graph* GraphBuilder::BuildFromQuadTree(QuadTree *tree)
{
	graph_ = new Graph();

	std::vector<TreeNode*> leaf_nodes;

	// Get all empty leaf nodes
	std::vector<TreeNode*>::iterator it;
	for(it = tree->leaf_nodes_.begin(); it != tree->leaf_nodes_.end(); it++)
	{
		if((*it)->occupancy_ == OccupancyType::FREE)
			leaf_nodes.push_back((*it));
	}

	// Find neighbors of each leaf node
	for(it = leaf_nodes.begin(); it != leaf_nodes.end(); it++)
	{
		std::vector<TreeNode*> neighbours;
		std::vector<TreeNode*>::iterator itn;

		neighbours = tree->FindNeighbours((*it));

		for(itn = neighbours.begin(); itn != neighbours.end(); itn++)
		{
			graph_->AddEdge((*it)->node_id_, (*itn)->node_id_, 1.0);
		}
	}

	return graph_;
}


