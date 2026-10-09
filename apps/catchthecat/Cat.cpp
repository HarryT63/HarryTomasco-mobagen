#include "Cat.h"
#include <cstdlib>
#include "World.h"

Point2D Cat::Move(CatWorld* world) {
  auto path = generatePath(world);
  if (!path.empty()) return path.back();  // first step toward the closest exit

  // No exit exists
  const Point2D pos = world->getCat();
  const int half = world->getWorldSideSize() / 2;
  for (const Point2D& n : {CatWorld::NE(pos), CatWorld::NW(pos), CatWorld::E(pos),
                           CatWorld::W(pos), CatWorld::SW(pos), CatWorld::SE(pos)}) {
    if (abs(n.x) <= half && abs(n.y) <= half && !world->getContent(n)) return n;
                           }
  return pos;
}