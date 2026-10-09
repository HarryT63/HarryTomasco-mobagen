#include "Catcher.h"
#include <cstdlib>
#include "World.h"

Point2D Catcher::Move(CatWorld* world) {
  const Point2D cat = world->getCat();
  const int half = world->getWorldSideSize() / 2;

  auto path = generatePath(world);
  if (!path.empty()) return path.front();  // block the cat's shortest exit

  // Cat can't reach the border
  for (const Point2D& n : {CatWorld::NE(cat), CatWorld::NW(cat), CatWorld::E(cat),
                           CatWorld::W(cat), CatWorld::SW(cat), CatWorld::SE(cat)}) {
    if (abs(n.x) <= half && abs(n.y) <= half && !world->getContent(n)) return n;
                           }

  // Fallback: any free cell that isn't the cat
  for (int y = -half; y <= half; ++y)
    for (int x = -half; x <= half; ++x) {
      Point2D p = {x, y};
      if (p != cat && !world->getContent(p)) return p;
    }
  return {half, half};
}