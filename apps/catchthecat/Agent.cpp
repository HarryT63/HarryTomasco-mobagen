#include "Agent.h"
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "World.h"

using namespace std;

namespace {
  struct Node {
    Point2D pos;
    int g;  // steps from the cat
    int f;  // g + heuristic

    bool operator<(const Node& o) const { return f > o.f; }
  };

  bool inBounds(const Point2D& p, int half) { return abs(p.x) <= half && abs(p.y) <= half; }
  bool isBorder(const Point2D& p, int half) { return abs(p.x) == half || abs(p.y) == half; }

  int heuristic(const Point2D& p, int half) { return min(half - abs(p.x), half - abs(p.y)); }

  vector<Point2D> neighbors(const Point2D& p) {
    return {CatWorld::NE(p), CatWorld::NW(p), CatWorld::E(p),
            CatWorld::W(p),  CatWorld::SW(p), CatWorld::SE(p)};
  }
}

std::vector<Point2D> Agent::generatePath(CatWorld* w) {
  const int half = w->getWorldSideSize() / 2;
  const Point2D cat = w->getCat();

  priority_queue<Node> frontier;
  unordered_map<Point2D, Point2D> cameFrom;
  unordered_map<Point2D, int> gScore;
  unordered_set<Point2D> closed;

  frontier.push({cat, 0, heuristic(cat, half)});
  gScore[cat] = 0;
  Point2D borderExit = {INT_MAX, INT_MAX};

  while (!frontier.empty()) {
    Node cur = frontier.top();
    frontier.pop();
    if (closed.count(cur.pos)) continue;
    closed.insert(cur.pos);

    if (cur.pos != cat && isBorder(cur.pos, half)) {
      borderExit = cur.pos;
      break;
    }

    for (const Point2D& n : neighbors(cur.pos)) {
      if (!inBounds(n, half) || closed.count(n) || w->getContent(n)) continue;

      int g = cur.g + 1;
      auto it = gScore.find(n);
      if (it != gScore.end() && g >= it->second) continue;

      gScore[n] = g;
      cameFrom[n] = cur.pos;
      frontier.push({n, g, g + heuristic(n, half)});
    }
  }

  if (borderExit.x == INT_MAX) return {};

  vector<Point2D> path;
  for (Point2D p = borderExit; p != cat; p = cameFrom.at(p)) path.push_back(p);
  return path;
}