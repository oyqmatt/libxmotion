/*
 * map_utils.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: rdu
 */

#include "map/map_utils.h"

using namespace librav;

bool MapUtils::ReadImageFromFile(std::string map_path, cv::OutputArray _dst)
{
	cv::Mat image;

	if(!map_path.empty()) {
		// read map image
		image = imread(map_path, cv::IMREAD_GRAYSCALE);

		if (!image.data) {
			printf("No image data \n");
			return false;
		}
		else
		{
			image.copyTo(_dst);
			return true;
		}
	}
	else
		return false;
}

std::shared_ptr<SquareGrid> MapUtils::CreateSquareGrid(uint32_t row_size, uint32_t col_size, uint32_t cell_size)
{
	return std::make_shared<SquareGrid>(row_size,col_size,cell_size);
}

Position2Dd MapUtils::CoordinatesFromMapToMapWorld(Position2Di map_pos, MapInfo info)
{
	Position2Dd rpos;

	rpos.x = map_pos.x / info.scale_x;
	rpos.y = map_pos.y / info.scale_y;

	if(rpos.x > info.world_size_x)
		rpos.x = info.world_size_x;
	if(rpos.y > info.world_size_y)
		rpos.y = info.world_size_y;

//	std::cout << std::endl;
//	std::cout << "map scale: " << info.scale_x << " , " << info.scale_y << std::endl;
//	std::cout << "input: " << map_pos.x << " , " << map_pos.y << std::endl;
//	std::cout << "convertion result: " << rpos.x << " , " << rpos.y << std::endl;

	return rpos;
}

Position2Di MapUtils::CoordinatesFromMapWorldToMap(Position2Dd world_pos, MapInfo info)
{
	Position2Di rpos;

	rpos.x = world_pos.x * info.scale_x;
	rpos.y = world_pos.y * info.scale_y;

	if(rpos.x > info.map_size_x)
		rpos.x = info.map_size_x;
	if(rpos.y > info.map_size_y)
		rpos.y = info.map_size_y;

//	std::cout << std::endl;
//	std::cout << "input: " << world_pos.x << " , " << world_pos.y << std::endl;
//	std::cout << "output: " << rpos.x << " , " << rpos.y << std::endl;

	return rpos;
}

Position2Di MapUtils::CoordinatesFromPaddedToOriginal(Position2Di pad_pos, MapInfo info)
{
	Position2Di rpos;

	if(pad_pos.x > info.map_size_x + info.padded_left)
		rpos.x = info.map_size_x;
	else if(pad_pos.x <= info.padded_left)
		rpos.x = 0;
	else
		rpos.x = pad_pos.x - info.padded_left;

	if(pad_pos.y > info.map_size_y + info.padded_top)
		rpos.y = info.map_size_y;
	else if(pad_pos.y <= info.padded_top)
		rpos.y = 0;
	else
		rpos.y = pad_pos.y - info.padded_top;

//	std::cout << std::endl;
//	std::cout << "input: " << pad_pos.x << " , " << pad_pos.y << std::endl;
//	std::cout << "output: " << rpos.x << " , " << rpos.y << std::endl;

	return rpos;
}

Position2Di MapUtils::CoordinatesFromOriginalToPadded(Position2Di ori_pos, MapInfo info)
{
	Position2Di rpos;

	rpos.x = ori_pos.x + info.padded_left;
	rpos.y = ori_pos.y + info.padded_top;

//	std::cout << std::endl;
//	std::cout << "input: " << ori_pos.x << " , " << ori_pos.y << std::endl;
//	std::cout << "output: " << rpos.x << " , " << rpos.y << std::endl;

	return rpos;
}

Position2Dd MapUtils::CoordinatesFromMapWorldToRefWorld(Position2Dd map_world_pos, MapInfo info)
{
	Position2Dd rpos;

	// origin_offset_x/y defined relative to map world
	rpos.x = - (map_world_pos.y - info.origin_offset_y);
	rpos.y = - (map_world_pos.x - info.origin_offset_x);

	return rpos;
}

Position2Dd MapUtils::CoordinatesFromRefWorldToMapWorld(Position2Dd ref_world_pos, MapInfo info)
{
	Position2Dd mpos;

	// origin_offset_x/y defined relative to map world, so exchange x and y values
	//	when calculating with ref world position
	mpos.x = - (ref_world_pos.y - info.origin_offset_x);
	mpos.y = - (ref_world_pos.x - info.origin_offset_y);

	return mpos;
}

Position2Dd MapUtils::CoordinatesFromMapPaddedToRefWorld(Position2Di map_pos, MapInfo info)
{
	Position2Di original_map_pos;
	Position2Dd mapw_pos, refw_pos;

	original_map_pos = MapUtils::CoordinatesFromPaddedToOriginal(map_pos, info);
	//std::cout << "conversion input: " << map_pos.x << " , " << map_pos.y << std::endl;
	mapw_pos = MapUtils::CoordinatesFromMapToMapWorld(original_map_pos, info);
	//std::cout << "conversion intermediate: " << mapw_pos.x << " , " << mapw_pos.y << std::endl;
	refw_pos = MapUtils::CoordinatesFromMapWorldToRefWorld(mapw_pos, info);
	//std::cout << "conversion output: " << refw_pos.x << " , " << refw_pos.y << std::endl;

	return refw_pos;
}

Position2Di MapUtils::CoordinatesFromRefWorldToMapPadded(Position2Dd world_pos, MapInfo info)
{
	Position2Dd mapw_pos;
	Position2Di map_pos;
	Position2Di map_padded_pos;

	//std::cout << "conversion input: " << world_pos.x << " , " << world_pos.y << std::endl;

	mapw_pos = MapUtils::CoordinatesFromRefWorldToMapWorld(world_pos, info);
	//std::cout << "map world: " << mapw_pos.x << " , " << mapw_pos.y << std::endl;

	map_pos = MapUtils::CoordinatesFromMapWorldToMap(mapw_pos, info);
	//std::cout << "map: " << map_pos.x << " , " << map_pos.y << std::endl;

	map_padded_pos = MapUtils::CoordinatesFromOriginalToPadded(map_pos, info);
	//std::cout << "padded map: " << map_padded_pos.x << " , " << map_padded_pos.y << std::endl;

	return map_padded_pos;
}