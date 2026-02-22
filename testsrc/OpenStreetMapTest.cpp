#include <gtest/gtest.h>
#include "OpenStreetMap.h"
#include "StringDataSource.h"

TEST(OSMTest, SimpleFiles){
    auto OSMDataSource = std::make_shared< CStringDataSource >("<?xml version='1.0' encoding='UTF-8'?>\n"
                                                                "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
                                                                "  <way id=\"100\">\n"
		                                                        "    <nd ref=\"258592863\"/>\n"
		                                                        "    <nd ref=\"4399281377\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"
                                                                );
    auto OSMReader = std::make_shared< CXMLReader >(OSMDataSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeCount(),2);
    EXPECT_EQ(OpenStreetMap.WayCount(),1);
    EXPECT_NE(OpenStreetMap.NodeByIndex(0),nullptr);
    EXPECT_NE(OpenStreetMap.NodeByIndex(1),nullptr);
    EXPECT_EQ(OpenStreetMap.NodeByIndex(2),nullptr);
    EXPECT_NE(OpenStreetMap.WayByIndex(0),nullptr);
    EXPECT_EQ(OpenStreetMap.WayByIndex(1),nullptr);
    auto TempNode = OpenStreetMap.NodeByIndex(0);
    ASSERT_NE(TempNode,nullptr);
    EXPECT_EQ(TempNode, OpenStreetMap.NodeByID(TempNode->ID()));
    EXPECT_EQ(TempNode->ID(),1);
    EXPECT_EQ(TempNode->Location(),std::make_pair(38.5,-121.7));

}

TEST(OSMTest, ObscureFiles){
    auto OSMDataSource = std::make_shared< CStringDataSource >("<?xml version='1.0' encoding='UTF-8'?>\n"
                                                                "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
                                                                "  <way id=\"100\">\n"
		                                                        "    <nd ref=\"258592863\"/>\n"
		                                                        "    <nd ref=\"4399281377\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"
                                                                );
    auto OSMReader = std::make_shared< CXMLReader >(OSMDataSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeCount(),2);
    EXPECT_EQ(OpenStreetMap.WayCount(),1);
    EXPECT_NE(OpenStreetMap.NodeByIndex(0),nullptr);
    EXPECT_NE(OpenStreetMap.NodeByIndex(1),nullptr);
    EXPECT_EQ(OpenStreetMap.NodeByIndex(2),nullptr);
    EXPECT_NE(OpenStreetMap.WayByIndex(0),nullptr);
    EXPECT_EQ(OpenStreetMap.WayByIndex(1),nullptr);
    auto TempNode = OpenStreetMap.NodeByIndex(0);
    ASSERT_NE(TempNode,nullptr);
    EXPECT_EQ(TempNode, OpenStreetMap.NodeByID(TempNode->ID()));
    EXPECT_EQ(TempNode->ID(),1);
    EXPECT_EQ(TempNode->Location(),std::make_pair(38.5,-121.7));
}

TEST(OSMTest, EmptyFiles){
    auto OSMDataSource = std::make_shared< CStringDataSource >("");
    auto OSMReader = std::make_shared< CXMLReader >(OSMDataSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeCount(),0);
    EXPECT_EQ(OpenStreetMap.WayCount(),0);
    EXPECT_EQ(OpenStreetMap.NodeByIndex(0),nullptr);
    EXPECT_EQ(OpenStreetMap.NodeByIndex(1),nullptr);
    EXPECT_EQ(OpenStreetMap.WayByIndex(0),nullptr);
    EXPECT_EQ(OpenStreetMap.WayByIndex(1),nullptr);
    EXPECT_EQ(OpenStreetMap.NodeByID(1),nullptr);
    EXPECT_EQ(OpenStreetMap.WayByID(1),nullptr);
}

TEST(OSMTest, EmptyFile_NoNodesOrWays){
    auto OSMDataSource = std::make_shared< CStringDataSource >("<?xml version='1.0' encoding='UTF-8'?>\n"
                                                                "<osm version=\"0.6\" generator=\"unit_test\">\n"
                                                                "</osm>\n"
                                                                );
    auto OSMReader = std::make_shared< CXMLReader >(OSMDataSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeCount(),0);
    EXPECT_EQ(OpenStreetMap.WayCount(),0);
    EXPECT_EQ(OpenStreetMap.NodeByIndex(0), nullptr);
    EXPECT_EQ(OpenStreetMap.WayByIndex(0), nullptr);
    EXPECT_EQ(OpenStreetMap.NodeByID(1), nullptr);
}

TEST(OSMTest, NodesOnlyFile){
    auto OSMDataSource = std::make_shared< CStringDataSource >("<?xml version='1.0' encoding='UTF-8'?>\n"
                                                                "<osm version=\"0.6\" generator=\"unit_test\">\n"
                                                                "  <node id=\"10\" lat=\"38.10\" lon=\"-121.10\"/>\n"
                                                                "  <node id=\"20\" lat=\"38.20\" lon=\"-121.20\"/>\n"
                                                                "  <node id=\"30\" lat=\"38.30\" lon=\"-121.30\"/>\n"
                                                                "</osm>\n"
                                                                );
    auto OSMReader = std::make_shared< CXMLReader >(OSMDataSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeCount(),3);
    EXPECT_EQ(OpenStreetMap.WayCount(),0);

    auto node0 = OpenStreetMap.NodeByIndex(0);
    auto node1 = OpenStreetMap.NodeByIndex(1);
    auto node2 = OpenStreetMap.NodeByIndex(2);
    ASSERT_NE(node0, nullptr);
    ASSERT_NE(node1, nullptr);
    ASSERT_NE(node2, nullptr);

    EXPECT_EQ(OpenStreetMap.NodeByIndex(3), nullptr);
    EXPECT_EQ(OpenStreetMap.WayByIndex(0), nullptr);

    EXPECT_NE(OpenStreetMap.NodeByID(10), nullptr);
    EXPECT_NE(OpenStreetMap.NodeByID(20), nullptr);
    EXPECT_NE(OpenStreetMap.NodeByID(30), nullptr);
    EXPECT_EQ(OpenStreetMap.NodeByID(676), nullptr); // haha 67
}

TEST(OSMTest, WaysOnlyFile){
    auto OSMDataSource = std::make_shared< CStringDataSource >("<?xml version='1.0' encoding='UTF-8' ?>\n"
                                                                "<osm version=\"0.6\" generator=\"unit_test\">\n"
                                                                "  <way id=\"100\">\n"
                                                                "    <nd ref=\"1\"\>\n"
                                                                "    <nd ref=\"2\"\>\n"
                                                                "  </way>\n"
                                                                "</osm>\n"
                                                                );
    auto OSMReader = std::make_shared< CXMLReader >(OSMDataSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeCount(),0);
    EXPECT_EQ(OpenStreetMap.WayCount(),1);
    EXPECT_EQ(OpenStreetMap.NodeByIndex(0), nullptr);
    EXPECT_NE(OpenStreetMap.WayByIndex(0), nullptr);
    EXPECT_EQ(OpenStreetMap.WayByIndex(1), nullptr);
}

