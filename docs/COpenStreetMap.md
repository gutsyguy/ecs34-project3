# COpenStreetMap

`COpenStreetMap` is an implementation of the `CStreetMap` interface that parses OSM XML data using `CXMLReader`.

## Constructor

```cpp
COpenStreetMap(std::shared_ptr<CXMLReader> src);
```
- `src`: An XML reader providing the OSM data. It processes `node` and `way` tags, along with their nested `tag` (for attributes) and `nd` (for way nodes) elements.

## Implementation Details

The class performs a single pass over the XML data during construction, populating internal data structures for nodes and ways. It handles both simple attributes (like `id`, `lat`, `lon` on nodes) and nested `tag` elements for key-value pairs.

## Usage Example

```cpp
auto osmDataSource = std::make_shared<CStringDataSource>(
    "<osm>"
    "  <node id='1' lat='38.5' lon='-121.7'>"
    "    <tag k='amenity' v='cafe'/>"
    "  </node>"
    "  <way id='100'>"
    "    <nd ref='1'/>"
    "    <tag k='highway' v='residential'/>"
    "  </way>"
    "</osm>"
);
auto osmReader = std::make_shared<CXMLReader>(osmDataSource);
COpenStreetMap streetMap(osmReader);

auto node = streetMap.NodeByID(1);
if (node && node->HasAttribute("amenity")) {
    std::cout << "Node 1 is a " << node->GetAttribute("amenity") << std::endl;
}
```
