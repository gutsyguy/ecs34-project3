# CCSVBusSystem

`CCSVBusSystem` is an implementation of the `CBusSystem` interface that loads bus stop and route information from CSV files using `CDSVReader`.

## Constructor

```cpp
CCSVBusSystem(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc);
```
- `stopsrc`: A DSV reader for the stops CSV file (expected headers: `stop_id`, `node_id`).
- `routesrc`: A DSV reader for the routes CSV file (expected headers: `route`, `stop_id`).

## Implementation Details

The class parses the CSV files during construction and stores the data in efficient lookup structures (vectors for index-based access and unordered maps for ID/name-based access).

## Usage Example

```cpp
auto stopDataSource = std::make_shared<CStringDataSource>("stop_id,node_id\n1,1001\n2,1002");
auto stopReader = std::make_shared<CDSVReader>(stopDataSource, ',');

auto routeDataSource = std::make_shared<CStringDataSource>("route,stop_id\nRouteA,1\nRouteA,2");
auto routeReader = std::make_shared<CDSVReader>(routeDataSource, ',');

CCSVBusSystem busSystem(stopReader, routeReader);

auto stop = busSystem.StopByID(1);
if (stop) {
    std::cout << "Stop 1 maps to Node " << stop->NodeID() << std::endl;
}
```
