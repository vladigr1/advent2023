#pragma once

#include <map>
#include <string>
#include <fstream>
#include <tuple>

struct Header{
    std::string source;
    std::string destintion;
};
Header getHeader(std::string_view myline);

struct FirstRange{
    unsigned start_destination;
    unsigned length;
    auto operator<=>(const FirstRange&) const = default;
};
using MapFirstRange = std::map<unsigned, FirstRange>;
using MapCategory = std::map<std::string, std::string>;
using MapDestination = std::map<std::string, MapFirstRange>;
MapFirstRange getMapFirstRange(std::ifstream &input_file);
unsigned getDestintion(MapFirstRange &mapFirstRange, unsigned source);
FirstRange getDestintionAndLenghExtra(const MapFirstRange &mapFirstRange, unsigned source);

std::tuple<MapCategory, MapDestination> readFile(std::ifstream &input_file);

MapFirstRange mapLocationStartToRangeSeed(const MapCategory& mCategory, const MapDestination& mDestination);


MapFirstRange vectorSeed(std::ifstream &input_file);
MapFirstRange mergeVectorSeed(MapFirstRange &vectorSeed);
MapFirstRange genVectorSeedFromMapDestination(MapFirstRange &vseed, MapFirstRange& curMapdist);
