/*
 * collision_field.cpp
 *
 * Created on: Nov 17, 2017 11:18
 * Description:
 *
 * Copyright (c) 2017 Ruixiang Du (rdu)
 */

#include "field/collision_field.hpp"
#include "field/threat_distribution.hpp"

#include <cmath>
#include <iostream>

#include <Eigen/Dense>

using namespace librav;

CollisionField::CollisionField(int64_t size_x, int64_t size_y)
    : ScalarField(size_x, size_y)
{
}

void CollisionField::AddTrafficParticipant(int32_t id, std::shared_ptr<TrafficParticipant> tfield)
{
  assert(tfield->SizeX() == this->size_x_ && tfield->SizeY() == this->size_y_);

  traffic_participants_.emplace(std::make_pair(id, tfield));
}

std::shared_ptr<TrafficParticipant> CollisionField::GetTrafficParticipant(int32_t id)
{
  assert(traffic_participants_.find(id) != traffic_participants_.end());

  return traffic_participants_[id];
}

void CollisionField::RemoveTrafficParticipant(int32_t id)
{
  traffic_participants_.erase(id);
}

void CollisionField::UpdateCollisionField()
{
  for (int64_t i = 0; i < size_x_; ++i)
    for (int64_t j = 0; j < size_y_; ++j)
    {
      double threat_val = 0;
      for (const auto &tfd : traffic_participants_)
        threat_val += tfd.second->GetValueAtCoordinate(i, j);
      SetValueAtCoordinate(i, j, threat_val);
    }
}
