#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <math.h>
#include <sstream>
#include <stack>
#include <regex>
#include <set>
#include <tuple>
#include <optional>
#include <unordered_map>
#include <numeric>
#include <utility>

//Thanks to HappyCerberus and codefrommars for alot of inspiration

using Rotation = std::tuple<int, int, int, int, int, int>;

struct Point3D
{
    int x;
    int y;
    int z;

    bool operator < (Point3D const& a) const
    {
        return std::tie(x,y,z) < std::tie(a.x, a.y, a.z);
    }

    bool operator > (Point3D const& a) const
    {
        return std::tie(x,y,z) > std::tie(a.x, a.y, a.z);
    }

    bool operator == (Point3D const& a) const
    {
        return (x == a.x) && (y == a.y) && (z == a.z);
    }
    
    Point3D abs()
    {
        return {std::abs(x), std::abs(y), std::abs(z)};
    }

    friend Point3D operator+(const Point3D&, const Point3D&);

    friend Point3D operator-(const Point3D&, const Point3D&);

    Point3D rotate(const Rotation& r) const
    {
        const int *vals[3] = {&x, &y, &z};
        return {(*vals[std::get<0>(r)]) * std::get<3>(r), 
                (*vals[std::get<1>(r)]) * std::get<4>(r), 
                (*vals[std::get<2>(r)]) * std::get<5>(r)};
    }

};

Point3D operator-(const Point3D& a,const Point3D& b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Point3D operator+(const Point3D& a, const Point3D& b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

using Beacons = std::set<Point3D>;
using Deltas = std::vector<Point3D>;

template <> struct std::hash<Point3D> {
  std::size_t operator()(Point3D const &s) const noexcept {
    std::size_t h1 = std::hash<int64_t>{}(s.x);
    std::size_t h2 = std::hash<int64_t>{}(s.y);
    std::size_t h3 = std::hash<int64_t>{}(s.z);
    return h1 ^ (h2 << 1) ^ (h3 << 2); // or use boost::hash_combine
  }
};

struct ScannerPosOrient //Will contain the rotation and position of next scanner
{                       //relative the first scanner
    Rotation rotation;
    Point3D position;
};

auto loadInput(const std::string& s)
{
    std::vector<Beacons> scanners;
    std::string line;
    Beacons view;
    std::fstream f{s};
    while (std::getline(f, line))
    {
        if (line.size() < 2)
        {
            scanners.push_back(view);
            view.clear();
        }
        else if (line[1] != '-')
        {
            int x, y, z;
            std::sscanf(line.c_str(), "%d,%d,%d\n", &x, &y, &z);
            view.insert({x, y, z});
        }
    }
    scanners.push_back(view);
    return scanners;
}

std::vector<Rotation> calcRots()
{
    std::vector<Rotation> axis = {
        {0, 1, 2, 1, 1, 1},    //  x  y  z
        {0, 1, 2, 1, -1, -1},  //  x -y -z
        {0, 2, 1, 1, 1, -1},   //  x  z -y
        {0, 2, 1, 1, -1, 1},   //  x -z  y
        {0, 1, 2, -1, 1, -1},  // -x  y -z
        {0, 1, 2, -1, -1, 1},  // -x -y  z
        {0, 2, 1, -1, 1, 1},   // -x  z  y
        {0, 2, 1, -1, -1, -1}, // -x -z -y
    };

    std::vector<Rotation> rot;
    for (int a = 0; a < 3; a++)
        for (const auto &[dx, dy, dz, sx, sy, sz] : axis)
            rot.push_back({(a + dx) % 3, (a + dy) % 3, (a + dz) % 3, sx, sy, sz});
            
    return rot;
}

std::optional<ScannerPosOrient> checkOverlaps(Beacons& beaconMap, 
                                              const Beacons &other,  
                                              std::vector<Rotation>const& rots) 
{
    for (auto& r:rots)
    {
        std::unordered_map<Point3D, uint32_t> counts = {};
        for (auto &lhs:beaconMap)
        {
            for (auto &rhs:other)
            {
                counts[lhs-rhs.rotate(r)]++;
            }
        }
        for (auto& el:counts)
        {
            if (el.second >= 12)
            {
                //printMap(counts);
                return ScannerPosOrient{r,el.first};
            }
        }
    }
    return {};
}


void normalize(const ScannerPosOrient& orienation, Beacons& beaconMap, Beacons& currentView)
{
    for (auto& b:currentView)
    {
        beaconMap.insert(b.rotate(orienation.rotation) + orienation.position);
    }
}

int manhattan(std::vector<Point3D>& v)
{
    std::vector<int> manhattans;
    for (int i = 0; i < v.size(); ++i)
    {
        for (int j = i + 1; v.size(); ++j)
        {
            if (j >= v.size()) break;
            Point3D delta = v[i] - v[j];
            manhattans.push_back((std::abs(delta.x) + std::abs(delta.y) + std::abs(delta.z)));
        }
    }
    return *std::max_element(manhattans.begin(), manhattans.end());
}

int main()
{
    std::vector<Beacons> ScannerView = loadInput("day19.txt");  //Contains all viewed beacons
    Beacons beaconMap;                                          //from each scanner
    std::vector<Rotation> rots = calcRots(); //Vector all rotations (24)

    

    for (auto b:ScannerView[0])     //Insert beacons from scanner 0 (used to normalize the rest)
    {
        beaconMap.insert(b);
    }
    ScannerView.erase(ScannerView.begin());

    std::vector<Point3D> scanner_position;
    while(!ScannerView.empty())
    {
        for (int i=0; i < ScannerView.size(); ++i)
        {
            auto orienation = checkOverlaps(beaconMap, ScannerView[i], rots);
            if (!orienation.has_value())
                continue;
            normalize(orienation.value(), beaconMap, ScannerView[i]); 
            scanner_position.push_back(orienation.value().position);
            ScannerView.erase(ScannerView.begin() + i);
        }
    }

    std::cout << "The total amount of unique beacons is " << beaconMap.size() << std::endl;
    std::cout << "The largest manhattan distance is " << manhattan(scanner_position) << std::endl;
    return 0;
}