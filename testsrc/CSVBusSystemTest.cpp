#include <gtest/gtest.h>
#include "CSVBusSystem.h"
#include "StringDataSource.h"

TEST(CSVBusSystem, SimpleFiles)
{
    auto StopDataSource = std::make_shared<CStringDataSource>("stop_id,node_id\n"
                                                              "1,123\n"
                                                              "2,124");
    auto StopReader = std::make_shared<CDSVReader>(StopDataSource, ',');
    auto RouteDataSource = std::make_shared<CStringDataSource>("route,stop_id\n"
                                                               "A,1\n"
                                                               "A,2\n"
                                                               "B,1");
    auto RouteReader = std::make_shared<CDSVReader>(RouteDataSource, ',');

    CCSVBusSystem BusSystem(StopReader, RouteReader);

    EXPECT_EQ(BusSystem.StopCount(), 2);
    EXPECT_EQ(BusSystem.RouteCount(), 2);

    auto StopObj = BusSystem.StopByID(1);
    ASSERT_NE(StopObj, nullptr);
    EXPECT_EQ(StopObj->ID(), 1);
    EXPECT_EQ(StopObj->NodeID(), 123);

    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj, nullptr);
    EXPECT_EQ(RouteObj->Name(), "A");
    EXPECT_EQ(RouteObj->StopCount(), 2);
    EXPECT_EQ(RouteObj->GetStopID(0), 1);
    EXPECT_EQ(RouteObj->GetStopID(1), 2);
    EXPECT_EQ(RouteObj->GetStopID(2), CBusSystem::InvalidStopID);

    RouteObj = BusSystem.RouteByName("B");
    ASSERT_NE(RouteObj, nullptr);
    EXPECT_EQ(RouteObj->Name(), "B");
    EXPECT_EQ(RouteObj->StopCount(), 1);
    EXPECT_EQ(RouteObj->GetStopID(0), 1);

    EXPECT_EQ(BusSystem.RouteByName("C"), nullptr);
}

TEST(CSVBusSystem, EmptyFiles)
{
    auto StopDataSource = std::make_shared<CStringDataSource>("stop_id,node_id\n");
    auto StopReader = std::make_shared<CDSVReader>(StopDataSource, ',');
    auto RouteDataSource = std::make_shared<CStringDataSource>("route,stop_id\n");
    auto RouteReader = std::make_shared<CDSVReader>(RouteDataSource, ',');

    CCSVBusSystem BusSystem(StopReader, RouteReader);

    EXPECT_EQ(BusSystem.StopCount(), 0);
    EXPECT_EQ(BusSystem.RouteCount(), 0);
}

TEST(CSVBusSystem, MalformedCSV)
{
    // Test with non-numeric IDs and missing values
    auto StopDataSource = std::make_shared<CStringDataSource>("stop_id,node_id\n"
                                                              "1,123\n"
                                                              "abc,456\n"
                                                              "2,def\n"
                                                              "3"); // Missing value
    auto StopReader = std::make_shared<CDSVReader>(StopDataSource, ',');
    auto RouteDataSource = std::make_shared<CStringDataSource>("route,stop_id\n"
                                                               "A,1\n"
                                                               "B,xyz");
    auto RouteReader = std::make_shared<CDSVReader>(RouteDataSource, ',');

    CCSVBusSystem BusSystem(StopReader, RouteReader);

    // Only the first valid stop (1, 123) should be loaded
    EXPECT_EQ(BusSystem.StopCount(), 1);
    EXPECT_EQ(BusSystem.RouteCount(), 1);
    
    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj, nullptr);
    EXPECT_EQ(RouteObj->StopCount(), 1);
}

TEST(CSVBusSystem, OrphanedStopID)
{
    // Test with routes referencing stop IDs that don't exist
    auto StopDataSource = std::make_shared<CStringDataSource>("stop_id,node_id\n"
                                                              "1,123");
    auto StopReader = std::make_shared<CDSVReader>(StopDataSource, ',');
    auto RouteDataSource = std::make_shared<CStringDataSource>("route,stop_id\n"
                                                               "A,1\n"
                                                               "A,999"); // 999 doesn't exist
    auto RouteReader = std::make_shared<CDSVReader>(RouteDataSource, ',');

    CCSVBusSystem BusSystem(StopReader, RouteReader);

    EXPECT_EQ(BusSystem.StopCount(), 1);
    EXPECT_EQ(BusSystem.RouteCount(), 1);

    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj, nullptr);
    EXPECT_EQ(RouteObj->StopCount(), 1); // Only stop 1 should be in route A
}

TEST(CSVBusSystem, DuplicateRouteName)
{
    // Test merging rows with the same route name
    auto StopDataSource = std::make_shared<CStringDataSource>("stop_id,node_id\n"
                                                              "1,123\n"
                                                              "2,124");
    auto StopReader = std::make_shared<CDSVReader>(StopDataSource, ',');
    auto RouteDataSource = std::make_shared<CStringDataSource>("route,stop_id\n"
                                                               "A,1\n"
                                                               "A,2\n"
                                                               "A,1"); // Duplicate route entry
    auto RouteReader = std::make_shared<CDSVReader>(RouteDataSource, ',');

    CCSVBusSystem BusSystem(StopReader, RouteReader);

    EXPECT_EQ(BusSystem.RouteCount(), 1);
    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj, nullptr);
    EXPECT_EQ(RouteObj->StopCount(), 3);
}

TEST(CSVBusSystem, OutOfOrderHeaders)
{
    // Test headers in different order
    auto StopDataSource = std::make_shared<CStringDataSource>("node_id,stop_id\n"
                                                              "123,1");
    auto StopReader = std::make_shared<CDSVReader>(StopDataSource, ',');
    auto RouteDataSource = std::make_shared<CStringDataSource>("stop_id,route\n"
                                                               "1,A");
    auto RouteReader = std::make_shared<CDSVReader>(RouteDataSource, ',');

    CCSVBusSystem BusSystem(StopReader, RouteReader);

    EXPECT_EQ(BusSystem.StopCount(), 1);
    EXPECT_EQ(BusSystem.RouteCount(), 1);
    
    auto StopObj = BusSystem.StopByID(1);
    ASSERT_NE(StopObj, nullptr);
    EXPECT_EQ(StopObj->NodeID(), 123);

    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj, nullptr);
}
