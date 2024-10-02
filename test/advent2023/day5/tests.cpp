#include <catch2/catch_test_macros.hpp>
#include <advent2023/day5_lib.hpp>
#include <fstream>

TEST_CASE("day5 single header")
{
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream input_file{ dir_path + "\\map.txt" };
    std::string myline;
    std::getline(input_file, myline);
    Header res = getHeader(myline);
    REQUIRE(res.source == "seed");
    REQUIRE(res.destintion == "soil");

    MapFirstRange expected_res {
        {98, {50, 2}},
        {50, {52, 48}}
    };

    REQUIRE( getMapFirstRange(input_file) == expected_res);
}

TEST_CASE("day5 two header")
{
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream input_file{ dir_path + "\\map2.txt" };
    std::string myline;
    std::getline(input_file, myline);
    Header res = getHeader(myline);
    REQUIRE(res.source == "seed");
    REQUIRE(res.destintion == "soil");

    MapFirstRange expected_res {
        {98, {50, 2}},
        {50, {52, 48}}
    };

    REQUIRE( getMapFirstRange(input_file) == expected_res);

    std::getline(input_file, myline);
    res = getHeader(myline);
    REQUIRE(res.source == "temperature");
    REQUIRE(res.destintion == "humidity");

    expected_res = {
        {69, {0, 1}},
        {0, {1, 69}}
    };

    REQUIRE( getMapFirstRange(input_file) == expected_res);
}

TEST_CASE("day5 two header readFile")
{
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream input_file{ dir_path + "\\map2.txt" };
    auto [mcategory, mdestination] = readFile(input_file);
    REQUIRE(mcategory.at("seed") == "soil");
}

TEST_CASE("day5 search destination")
{
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream input_file{ dir_path + "\\map3.txt" };
    std::string myline;
    std::getline(input_file, myline);
    MapFirstRange res = getMapFirstRange(input_file);

    REQUIRE( getDestintion(res,0) == 110);
    REQUIRE( getDestintion(res,1) == 111);
    REQUIRE( getDestintion(res,2) == 2);
    REQUIRE( getDestintion(res,10) == 0);
    REQUIRE( getDestintion(res,11) == 1);
    REQUIRE( getDestintion(res,49) == 49);
    REQUIRE( getDestintion(res,50) == 52);
    REQUIRE( getDestintion(res,97) == 99);
    REQUIRE( getDestintion(res,98) == 50);
    REQUIRE( getDestintion(res,99) == 51);
    REQUIRE( getDestintion(res,100) == 100);

}

// TEST_CASE("day5_b shorten search destination by mapLocationStartToRangeSeed")
// {
//     const std::string file_path = __FILE__;
//     const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
//     std::ifstream input_file{ dir_path + "\\map4.txt" };
//     auto [mcategory, mdestination] = readFile(input_file);

//     MapFirstRange mSeedToLocation = mapLocationStartToRangeSeed(mcategory, mdestination);

//     MapFirstRange expected_res = {
//         {0, {1, 50}},
//         {98, {51, 2}},
//         {50, {53, 17}},
//         {67, {69, 48-17}}
//     };

//     REQUIRE( mSeedToLocation == expected_res);
// }

TEST_CASE("day5_b seed map"){
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream input_file{ dir_path + "\\seed.txt" };

    MapFirstRange expected_res = {
        {3908507817, {3908507817, 37519573}},
        {3736161691, {3736161691, 172346126}},
        {2590035450, {2590035450, 66446591}}
    };
    MapFirstRange vseed = vectorSeed(input_file);
    REQUIRE( vseed == expected_res);
    vseed = mergeVectorSeed(vseed);
    REQUIRE( vseed == MapFirstRange{
        {3736161691, {3736161691, 209865699}},
        {2590035450, {2590035450, 66446591}}
    });

}

TEST_CASE("day5_b genVectorSeedFromMapDestination"){
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream map_file{ dir_path + "\\map5.txt" };
    std::ifstream seed_file{ dir_path + "\\seed5.txt" };

    MapFirstRange vseed = vectorSeed(seed_file);
    vseed = mergeVectorSeed(vseed);

    auto [mcategory, mdestination] = readFile(map_file);

    MapFirstRange curMapdist = mdestination.at("soil");

    MapFirstRange vdestination = genVectorSeedFromMapDestination(vseed, curMapdist);


    MapFirstRange expected_res = {
        {57, {57, 13}},
        {81, {81, 14}},
    };

    REQUIRE( vdestination == expected_res);
}