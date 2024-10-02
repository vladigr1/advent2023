#include <advent2023/day5_lib.hpp>
// #include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <limits>


Header getHeader(std::string_view myline){
    std::string_view::const_iterator end_source = std::find(myline.cbegin(), myline.cend(), '-');
    std::string_view::const_iterator begin_destination = end_source + 4;
    std::string_view::const_iterator end_destination = std::find(begin_destination, myline.cend(), ' ');

    return {std::string{myline.cbegin(), end_source}, std::string{begin_destination, end_destination}};
}


MapFirstRange getMapFirstRange(std::ifstream &input_file){
    MapFirstRange res;

    std::string myline;
    while (std::getline(input_file, myline)) {
        std::stringstream extract {myline};
        // if (extract.view().empty()) break; // not support in clang
        if (extract.str().empty()) break;
        unsigned start_source, start_destination, length;
        extract >> start_destination >> start_source >> length;
// spdlog::debug::debug("getMapFirstRange: myline: {}", myline);
        res[start_source] = {start_destination, length};

    }
    return res;
}

unsigned getDestintion(MapFirstRange &mapFirstRange, unsigned source){
    MapFirstRange::iterator res = --mapFirstRange.upper_bound(source);
    if (res == mapFirstRange.end()) return source;
    unsigned delta = source - res->first;
    if(delta >= res->second.length){
// spdlog::debug::debug("getDestintion: not in range - {}", source);
        return source;
    }
    return res->second.start_destination + delta;
}

FirstRange getDestintionAndLenghExtra(const MapFirstRange &mapFirstRange, unsigned source){
    MapFirstRange::const_iterator res = --mapFirstRange.upper_bound(source);
    // not locate before
    if (res == mapFirstRange.cend()) return {source, mapFirstRange.begin()->first - source};
    unsigned delta = source - res->first;
    if(delta >= res->second.length){
// spdlog::debug::debug("getDestintion: not in range - {}", source);
        return {source , std::numeric_limits<unsigned>::max()};
    }
    unsigned desitination = res->second.start_destination + delta;
    return { desitination, res->second.length - delta};
}

std::tuple<MapCategory, MapDestination> readFile(std::ifstream &input_file){
    MapCategory mcategory;
    MapDestination dmap;
    std::string myline;
    while(!input_file.eof()){
        std::getline(input_file, myline);
        Header header = getHeader(myline);

        mcategory[header.source] = header.destintion;
        dmap[header.destintion] = getMapFirstRange(input_file);
        }
    return {mcategory, dmap};
}

MapFirstRange mapLocationStartToRangeSeed(const MapCategory& mCategory, const MapDestination& mDestination){
    std::string destination_category = mCategory.at("seed");
    MapFirstRange mSeedToLocation = mDestination.at(destination_category);
    mSeedToLocation[0] = getDestintionAndLenghExtra(mSeedToLocation, 0);
    do {
        destination_category = mCategory.at(destination_category);
        MapFirstRange localMSeedToLocation;
        for ( MapFirstRange::value_type &val : mSeedToLocation){
            unsigned seedFirst {val.first};
            FirstRange seedDestinationFirstRange {val.second};
            while (seedDestinationFirstRange.length){
                // search in next destination from last destination
                auto [destinationCur, lengthExtra] = getDestintionAndLenghExtra(mDestination.at(destination_category), seedDestinationFirstRange.start_destination);
                lengthExtra = std::min(lengthExtra, seedDestinationFirstRange.length);
                localMSeedToLocation[seedFirst] = {destinationCur, lengthExtra};
// spdlog::debug::debug("mapLocationStartToRangeSeed: seedFirst={} lengthExtra={} start_destination={} length={}", seedFirst, lengthExtra, seedDestinationFirstRange.start_destination, seedDestinationFirstRange.length);

                seedDestinationFirstRange.length -= lengthExtra;
                seedFirst += lengthExtra;
                seedDestinationFirstRange.start_destination += lengthExtra;
// spdlog::debug::debug("mapLocationStartToRangeSeed: seedDestinationFirstRange.length={}", seedDestinationFirstRange.length);
            } 
        }
        mSeedToLocation = localMSeedToLocation;
    } while (destination_category != "location");

    return mergeVectorSeed(mSeedToLocation);
}

MapFirstRange vectorSeed(std::ifstream &input_file){
    MapFirstRange res;
    while (! input_file.eof()) {
        unsigned seed_start, range;
        input_file >> seed_start >> range;
// spdlog::debug::debug("solver: seed_start={} range={}", seed_start, range);
        res[seed_start] = {seed_start, range};
    }
    return res;
}


MapFirstRange mergeVectorSeed(MapFirstRange &vectorSeed){
    MapFirstRange res;
    FirstRange entry = vectorSeed.begin()->second;
    for(MapFirstRange::iterator iter = ++vectorSeed.begin(); iter != vectorSeed.end(); ++iter){
        if (entry.start_destination + entry.length >= iter->first){
            entry.length += iter->second.length + entry.start_destination + entry.length - iter->first;
        }else{
            res[entry.start_destination] = entry;
            entry = iter->second;
        }
    }
    res[entry.start_destination] = entry;
    return res;
}


MapFirstRange genVectorSeedFromMapDestination(MapFirstRange &vseed, MapFirstRange& curMapdist){
    MapFirstRange vdestination; 
    for (auto value : vseed){
        FirstRange curSeedRange = value.second;
// spdlog::debug::debug("new curSeedRange.start_destination={} curSeedRange.length={}\n", curSeedRange.start_destination, curSeedRange.length);
        while(curSeedRange.length){
// spdlog::debug::debug("curSeedRange.start_destination={} curSeedRange.length={}\n", curSeedRange.start_destination, curSeedRange.length);
            FirstRange entry = getDestintionAndLenghExtra(curMapdist, curSeedRange.start_destination);
// spdlog::debug::debug("entry.start_destination={} entry.length={}\n", entry.start_destination, entry.length);
            unsigned minLengthExtra = std::min(curSeedRange.length, entry.length);
// spdlog::debug::debug("minLengthExtra={}\n", minLengthExtra);
            entry.length = minLengthExtra;

            vdestination[entry.start_destination] = entry;
            curSeedRange.length -= minLengthExtra;
            curSeedRange.start_destination += minLengthExtra;
        }
    }
    return vdestination; 
}