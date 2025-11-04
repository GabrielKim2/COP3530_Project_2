#include "KDTree.h"

KDTree::Node::Node(const Point& pt, int d)
    : point(pt), left(nullptr), right(nullptr), depth(d) {}

KDTree::KDTree(const std::vector<Point>& points, size_t dim)
    : root(nullptr), dimension(dim) {
    std::vector<Point> pts = points;
    root = build(pts, 0);
}

KDTree::~KDTree() {
    destroy(root);
}

KDTree::Node* KDTree::build(std::vector<Point>& pts, int depth) {
    if (pts.empty()) return nullptr;

    int axis = depth % dimension;
    size_t median = pts.size() / 2;

    std::nth_element(pts.begin(), pts.begin() + median, pts.end(),
        [axis](const Point& a, const Point& b) {
            return (axis == 0) ? a.x < b.x : a.y < b.y;
        });

    Node* node = new Node(pts[median], depth);

    std::vector<Point> left(pts.begin(), pts.begin() + median);
    std::vector<Point> right(pts.begin() + median + 1, pts.end());

    node->left = build(left, depth + 1);
    node->right = build(right, depth + 1);

    return node;
}

void KDTree::destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

std::vector<Point> KDTree::rangeSearch(const Point& target, double radius) const {
    std::vector<Point> result;
    rangeSearchHelper(root, target, radius, result);
    return result;
}

void KDTree::rangeSearchHelper(Node* node, const Point& target, double radius, std::vector<Point>& result) const {
    if (!node) return;

    double dist = std::sqrt(std::pow(node->point.x - target.x, 2) + std::pow(node->point.y - target.y, 2));
    if (dist <= radius) result.push_back(node->point);

    int axis = node->depth % dimension;
    double diff = (axis == 0) ? target.x - node->point.x : target.y - node->point.y;

    if (diff <= radius) rangeSearchHelper(node->left, target, radius, result);
    if (diff >= -radius) rangeSearchHelper(node->right, target, radius, result);
}

Point KDTree::nearestNeighbor(const Point& target) const {
    Point bestPoint;
    double bestDist = std::numeric_limits<double>::max();
    nearestNeighborHelper(root, target, bestPoint, bestDist);
    return bestPoint;
}

void KDTree::nearestNeighborHelper(Node* node, const Point& target, Point& bestPoint, double& bestDist) const {
    if (!node) return;

    double dist = std::sqrt(std::pow(node->point.x - target.x, 2) + std::pow(node->point.y - target.y, 2));
    if (dist < bestDist) {
        bestDist = dist;
        bestPoint = node->point;
    }

    int axis = node->depth % dimension;
    double diff = (axis == 0) ? target.x - node->point.x : target.y - node->point.y;

    Node* first = (diff < 0) ? node->left : node->right;
    Node* second = (diff < 0) ? node->right : node->left;

    nearestNeighborHelper(first, target, bestPoint, bestDist);

    if (std::fabs(diff) < bestDist) {
        nearestNeighborHelper(second, target, bestPoint, bestDist);
    }
}