# CStreetMap

The `CStreetMap` class is an abstract interface for accessing Open Street Map data, organized into nodes and ways.

## Interface Definition

```cpp
class CStreetMap {
public:
    using TNodeID = uint64_t;
    using TWayID = uint64_t;
    using TLocation = std::pair<double, double>;

    struct SNode {
        virtual ~SNode() {};
        virtual TNodeID ID() const noexcept = 0;
        virtual TLocation Location() const noexcept = 0;
        virtual std::size_t AttributeCount() const noexcept = 0;
        virtual std::string GetAttributeKey(std::size_t index) const noexcept = 0;
        virtual bool HasAttribute(const std::string &key) const noexcept = 0;
        virtual std::string GetAttribute(const std::string &key) const noexcept = 0;
    };

    struct SWay {
        virtual ~SWay() {};
        virtual TWayID ID() const noexcept = 0;
        virtual std::size_t NodeCount() const noexcept = 0;
        virtual TNodeID GetNodeID(std::size_t index) const noexcept = 0;
        virtual std::size_t AttributeCount() const noexcept = 0;
        virtual std::string GetAttributeKey(std::size_t index) const noexcept = 0;
        virtual bool HasAttribute(const std::string &key) const noexcept = 0;
        virtual std::string GetAttribute(const std::string &key) const noexcept = 0;
    };

    virtual ~CStreetMap() {};

    virtual std::size_t NodeCount() const noexcept = 0;
    virtual std::size_t WayCount() const noexcept = 0;
    virtual std::shared_ptr<SNode> NodeByIndex(std::size_t index) const noexcept = 0;
    virtual std::shared_ptr<SNode> NodeByID(TNodeID id) const noexcept = 0;
    virtual std::shared_ptr<SWay> WayByIndex(std::size_t index) const noexcept = 0;
    virtual std::shared_ptr<SWay> WayByID(TWayID id) const noexcept = 0;
};
```

## Usage Example

```cpp
void PrintNodeLocation(std::shared_ptr<CStreetMap> map, CStreetMap::TNodeID id) {
    auto node = map->NodeByID(id);
    if (node) {
        auto loc = node->Location();
        std::cout << "Node " << id << " is at (" << loc.first << ", " << loc.second << ")" << std::endl;
    }
}
```
