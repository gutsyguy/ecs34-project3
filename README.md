# Project 3 README

Student 1: Yalamber (923528180)

Student 2: Kevin Jia (921898728)

## Project Status
The project is complete

## Known Issues
Kevin: One issue I ran into was determining test cases and its syntax as, working with OSM files are something new to me.
Yalamber: One issue I ran into was dealing with duplicate routes. 

## Code References
* [Different ways to initialize variables](https://stackoverflow.com/questions/22543875/different-ways-to-initialize-variables)
* [C++ Refernce](https://cplusplus.com/reference/)
* [DSV Reference](https://www.partow.net/programming/dsvfilter/index.html)

## Generative AI Use
I used ChatGPT for Prompt 1

### Prompt 1
In an implementation for openstreetmap files, what are some common corner cases?

### Response 1
When implementing or processing OpenStreetMap (OSM) data, there are several corner cases and edge cases you may encounter. These can vary based on the specific task you're tackling (e.g., rendering, routing, data import, or analysis). Below are some common corner cases to be aware of:

1. Missing or Invalid Tags

Empty or missing tags: Some OSM elements (nodes, ways, or relations) may have missing or incomplete tags.

Example: A road might not have a highway tag or a building might be missing the building tag.

Impact: If critical tags are missing, the element might not render or be processed correctly, or incorrect assumptions may be made about its characteristics.

Incorrect or non-standard tags: Sometimes, the tags used might not conform to expected standards or conventions, making interpretation tricky.

Example: A highway might have a highway=residential tag but also have conflicting information about the lane count or speed limit.

2. Self-Intersecting or Invalid Geometries

Self-intersecting ways: A way (a polyline) may accidentally intersect itself, especially in complex or poorly edited OSM data.

Impact: These errors can break geometries, cause issues in routing algorithms, or cause rendering artifacts.

Example: A road might loop around and intersect itself in a non-meaningful way.

Non-closed ways: Some ways (like roads or building outlines) are expected to be closed (i.e., their start and end nodes should be the same), but they may be open or improperly closed.

Impact: This can cause errors in analysis (e.g., area calculations, rendering) or errors when generating polygons from ways.

3. Overlapping and Duplicated Data

Duplicate nodes/ways/relations: Sometimes, OSM data can have duplicate nodes or ways that represent the same physical feature.

Example: A road may have multiple identical representations due to editing conflicts or imports from different sources.

Impact: Duplicate elements can cause redundancy in processing or misrepresentation on the map.

Overlapping areas or ways: Multiple ways or areas may overlap when they shouldn’t.

Example: Two buildings might be incorrectly tagged or drawn overlapping each other.

Impact: This can cause problems in visualization or spatial analysis.

4. Nodes Not Associated with Ways or Relations

Dangling nodes: Sometimes nodes are part of the OSM database but aren’t included in any way or relation.

Impact: These nodes might be left out of map rendering and analysis.

Orphaned ways: Ways that aren’t associated with any relations but are meant to be part of one (e.g., a road that should be part of a route relation).

Impact: These ways may not be used in routing or other spatial queries.

5. Complex Relations and Nested Relations

Nested relations: Relations in OSM can be nested within other relations, and understanding how they relate to each other can be challenging.

Example: A route relation might include ways that are themselves part of larger multipolygon relations (e.g., areas with holes like parks or buildings).

Impact: Complexity increases when interpreting or rendering these nested relations. Incorrect processing of nested relations might lead to data loss or errors.

Incomplete or conflicting relations: Sometimes relations like multipolygon, route, or boundary can be incomplete or contain conflicting data.

Example: A route relation might not include all necessary ways or might conflict with other relations for the same set of ways.

Impact: Routing algorithms can break or generate incorrect results.

6. Different Coordinate Reference Systems (CRS)

Coordinate system mismatches: OSM data is usually in the WGS 84 coordinate reference system, but if you are combining it with data in a different CRS, there can be discrepancies.

Example: You might be using OSM data in WGS 84 alongside local data in a projected CRS like UTM.

Impact: Coordinates may not align properly when attempting to merge or visualize data.

7. Tagging Ambiguities

Inconsistent or ambiguous tagging: Some tags may be ambiguous, or the meaning of tags can change over time as OSM evolves.

Example: A building tagged as building=house might be interpreted as a residential building in one area, but as a commercial house in another.

Impact: Interpretations of data might differ across contexts, and assumptions may be incorrect for specific use cases.

8. Way Direction Issues

Incorrect way direction: In many cases, the direction of a way matters (e.g., for one-way streets, highways, or bike lanes). Incorrect directionality can cause issues in routing.

Example: A road might be tagged oneway=yes, but the directionality might be flipped in the OSM data.

Impact: Routing algorithms might generate incorrect paths or navigational data if way directionality is improperly handled.

9. Unresolved Versioning or Conflict Resolution

Versioning conflicts: OSM is a collaborative editing platform, so conflicts can arise when multiple users edit the same element at the same time.

Example: A building’s shape could be modified by two users simultaneously, and when merging the changes, one version might get lost.

Impact: This can cause inconsistencies in the data and potential errors during imports.

10. Data Import/Export Issues

Inconsistent imports: When importing external data sources into OSM (e.g., from government agencies or other mapping projects), there can be inconsistencies or mismatches in data quality.

Example: Imported data may not follow OSM conventions or might lack proper tagging, leading to data quality issues.

Impact: Mismatched data may cause rendering problems, or incomplete tags may affect routing, analysis, or visualization.

11. Highly Detailed Data and Performance Issues

Too much detail: Some areas may contain excessive levels of detail (e.g., very small roads, every building labeled individually, etc.), which can make processing or rendering the data slower.

Impact: High-density data can cause performance issues in rendering or processing, especially in larger maps.

12. Edge of Data Coverage

Incomplete boundaries or data coverage: OSM data coverage might not extend to the full area you're interested in (e.g., rural areas, borders, or oceans).

Example: There might be a lack of detail at the edges of a city boundary or between different OSM datasets.

Impact: Missing or incomplete data can make rendering or analysis unreliable, especially near borders or the edges of imported regions.

Conclusion

Each of these corner cases requires specific handling based on the task you're performing with OSM data. Whether you're visualizing, analyzing, or using the data for routing, detecting and addressing these edge cases is key to ensuring that your application works smoothly and produces accurate results.

### Chagnes 1
Added a good portion of test cases
