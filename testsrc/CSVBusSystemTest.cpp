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
