#include "../World.h"
#include "../SeededRandom.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>

// Recursive backtracker, in grid units: (0, 0) is the top-left cell, x grows
// right, y grows down — the same units as the World API. The caller seeds
// SeededRandom before the first Step; every decision consumes the seed in
// order, so the maze is deterministic.
//
// Procedure per Step, on the cell at the top of the path stack:
//   1. mark it visited;
//   2. list its visitable (unvisited) neighbors in clockwise order starting
//      from the top: UP, RIGHT, DOWN, LEFT (getVisitables does this);
//   3. none        -> dead end: pop the stack (backtrack). Empty stack = done;
//   4. exactly one -> move to it, do not consume a random number;
//   5. two or more -> consume SeededRandom::next() and pick
//      next() % visitableCount;
//   6. moving opens the wall between the two cells
//      (World::SetNorth/SetEast/SetSouth/SetWest with false).

void RecursiveBacktrackerExample::Clear(World* world) {
  // todo: reset the walk
  // hint:
  //   clear visited and the path stack, then start the walk at the
  //   top-left cell: stack.push_back({0, 0})
  // begin solution
  visited.clear();
  stack.clear();
  stack.push_back({0, 0});
  // end solution
}

std::vector<Point2D> RecursiveBacktrackerExample::getVisitables(World* w, const Point2D& point) {
  // todo: list the unvisited neighbors of point, in clockwise order
  // hint:
  //   candidates in order: UP {x, y-1}, RIGHT {x+1, y}, DOWN {x, y+1}, LEFT {x-1, y}
  //   keep a candidate only if it is inside the grid
  //   (0 <= x < w->GetWidth(), 0 <= y < w->GetHeight()) and not visited
  // begin solution
std::vector<Point2D> visitables;
  const Point2D candidates[4] = {
    {point.x, point.y - 1},  // UP
    {point.x + 1, point.y},  // RIGHT
    {point.x, point.y + 1},  // DOWN
    {point.x - 1, point.y},  // LEFT
  };

  for (const auto& c : candidates) {
    bool inside = c.x >= 0 && c.x < w->GetWidth() && c.y >= 0 && c.y < w->GetHeight();
    if (inside && !visited[c.x][c.y]) visitables.push_back(c);
  }
  return visitables;
  // end solution
  return {};
}

bool RecursiveBacktrackerExample::Step(World* w) {
  static const Color32 kActive  = {1.0f, 0.0f, 0.0f, 1.0f};  // bright red: top of the stack
  static const Color32 kOnStack = {0.5f, 0.0f, 0.0f, 1.0f};  // dark red: rest of the path
  static const Color32 kDone    = {0.0f, 0.0f, 0.0f, 1.0f};  // black: fully explored
  // todo: implement one iteration of the recursive backtracker
  // hint:
  //   empty stack  -> the maze is done, return false
  //   otherwise, on the cell at the top of the stack:
  //   1. mark it visited;
  //   2. list its visitable neighbors with getVisitables
  //      (already in clockwise order: UP, RIGHT, DOWN, LEFT);
  //   3. none        -> dead end: pop the stack (backtrack);
  //   4. exactly one -> move to it, do not consume a random number;
  //   5. two or more -> consume SeededRandom::next() and pick
  //      next() % visitables.size();
  //   moving = opening the wall between the two cells:
  //     UP    -> w->SetNorth(current, false)
  //     RIGHT -> w->SetEast(current, false)
  //     DOWN  -> w->SetSouth(current, false)
  //     LEFT  -> w->SetWest(current, false)
  //   return true while there is still work (stack not empty after the move)
  // begin solution

  if (stack.empty()) return false;

  Point2D current = stack.back();

  visited[current.x][current.y] = true;
  w->SetNodeColor(current, kActive);

  std::vector<Point2D> visitables = getVisitables(w, current);

  if (visitables.empty()) {
    w->SetNodeColor(current, kDone);
    stack.pop_back();
    if (!stack.empty()) w->SetNodeColor(stack.back(), kActive);
    return !stack.empty();
  }

  Point2D next = visitables.size() == 1
                   ? visitables[0]
                   : visitables[SeededRandom::next() % visitables.size()];

  if (next.y < current.y) {
    w->SetNorth(current, false);  // UP
  } else if (next.x > current.x) {
    w->SetEast(current, false);   // RIGHT
  } else if (next.y > current.y) {
    w->SetSouth(current, false);  // DOWN
  } else {
    w->SetWest(current, false);   // LEFT
  }

  w->SetNodeColor(current, kOnStack);
  w->SetNodeColor(next, kActive);
  stack.push_back(next);
  return true;
  // end solution
}
