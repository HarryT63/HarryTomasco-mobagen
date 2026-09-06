#include "SeparationRule.h"
#include "imgui.h"
#include <glm/glm.hpp>

glm::vec2 SeparationRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 separatingForce(0.f);

  // Step 1: accumulate per-neighbor contributions
  for (const BoidView& neighbor : neighborhood)
  {
      glm::vec2 offset = boid.position - neighbor.position; // P_agent - P_i
      float distance = glm::length(offset);

      if (distance > 0.0001f) // epsilon guard
      {
          glm::vec2 direction = offset / distance;
          separatingForce += direction / distance; // unit vector / distance
      }
  }

  // Step 2: clamp the accumulated force to F_Smax (= desiredMinimalDistance)
  float forceLength = glm::length(separatingForce);
  if (forceLength > desiredMinimalDistance)
  {
      separatingForce = (separatingForce / forceLength) * desiredMinimalDistance;
  }

  return separatingForce;
}

bool SeparationRule::drawImguiRuleExtra() {
  bool valueHasChanged = false;
  if (ImGui::DragFloat("Desired Separation", &desiredMinimalDistance, 0.05f)) {
    valueHasChanged = true;
  }
  return valueHasChanged;
}