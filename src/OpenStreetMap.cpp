#include "OpenStreetMap.h"
#include <unordered_map>

struct COpenStreetMap::SImplementation{
    const std::string DOSMTag = "osm";
    const std::string DNodeTag = "node";
    const std::string DWayTag = "way";
    const std::string DTagTag = "tag";
    const std::string DNodeIDAttr = "id";
    const std::string DNodeLatAttr = "lat";
    const std::string DNodeLonAttr = "lon";
    const std::string DWayIDAttr = "id";
    const std::string DTagKeyAttr = "k";
    const std::string DTagValueAttr = "v";
    const std::string DWayNodeTag = "nd";
    const std::string DWayNodeRefAttr = "ref";

    struct SNode: public CStreetMap::SNode{
        TNodeID DID;
        TLocation DLocation;
        std::vector<std::string> DAttributeKeys;
        std::unordered_map<std::string, std::string> DAttributes;

        ~SNode() override{};
        TNodeID ID() const noexcept override{
            return DID;
        }

        TLocation Location() const noexcept override{
            return DLocation;
        }

        std::size_t AttributeCount() const noexcept override{
            return DAttributeKeys.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept override{
            if(index >= DAttributeKeys.size()){
                return "";
            }
            return DAttributeKeys[index];
        }

        bool HasAttribute(const std::string &key) const noexcept override{
            return DAttributes.find(key) != DAttributes.end();
        }

        std::string GetAttribute(const std::string &key) const noexcept override{
            auto It = DAttributes.find(key);
            if(It == DAttributes.end()){
                return "";
            }
            return It->second;
        }
    };

    struct SWay: public CStreetMap::SWay{
        TWayID DID;
        std::vector<TNodeID> DNodeIDs;
        std::vector<std::string> DAttributeKeys;
        std::unordered_map<std::string, std::string> DAttributes;

        ~SWay() override{};
        TWayID ID() const noexcept override{
            return DID;
        }

        std::size_t NodeCount() const noexcept override{
            return DNodeIDs.size();
        }

        TNodeID GetNodeID(std::size_t index) const noexcept override{
            if(index >= DNodeIDs.size()){
                return InvalidNodeID;
            }
            return DNodeIDs[index];
        }

        std::size_t AttributeCount() const noexcept override{
            return DAttributeKeys.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept override{
            if(index >= DAttributeKeys.size()){
                return "";
            }
            return DAttributeKeys[index];
        }

        bool HasAttribute(const std::string &key) const noexcept override{
            return DAttributes.find(key) != DAttributes.end();
        }

        std::string GetAttribute(const std::string &key) const noexcept override{
            auto It = DAttributes.find(key);
            if(It == DAttributes.end()){
                return "";
            }
            return It->second;
        }
    };
    std::vector<std::shared_ptr<SNode>> DNodesByIndex;
    std::unordered_map<TNodeID,std::shared_ptr<SNode>> DNodesByID;
    std::vector<std::shared_ptr<SWay>> DWaysByIndex;
    std::unordered_map<TWayID,std::shared_ptr<SWay>> DWaysByID;

    void ParseTags(std::shared_ptr<CXMLReader> src, std::vector<std::string> &keys, std::unordered_map<std::string, std::string> &attrs, const std::string &parentTag){
        SXMLEntity TempEntity;
        while(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DTagTag){
                std::string k = TempEntity.AttributeValue(DTagKeyAttr);
                std::string v = TempEntity.AttributeValue(DTagValueAttr);
                keys.push_back(k);
                attrs[k] = v;
            }
            else if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == parentTag){
                break;
            }
        }
    }

    bool ParseNodes(std::shared_ptr<CXMLReader> src, SXMLEntity &nextentity){
        if(nextentity.DType == SXMLEntity::EType::StartElement && nextentity.DNameData == DNodeTag){
            do {
                if(nextentity.DType == SXMLEntity::EType::StartElement && nextentity.DNameData == DNodeTag){
                    auto NodeID = std::stoull(nextentity.AttributeValue(DNodeIDAttr));
                    auto NodeLat = std::stod(nextentity.AttributeValue(DNodeLatAttr));
                    auto NodeLon = std::stod(nextentity.AttributeValue(DNodeLonAttr));
                    auto NewNode = std::make_shared<SNode>();
                    NewNode->DID = NodeID;
                    NewNode->DLocation = std::make_pair(NodeLat,NodeLon);
                    
                    // Possible attributes (tags)
                    SXMLEntity TagEntity;
                    while(src->ReadEntity(TagEntity)){
                        if(TagEntity.DType == SXMLEntity::EType::StartElement && TagEntity.DNameData == DTagTag){
                            std::string k = TagEntity.AttributeValue(DTagKeyAttr);
                            std::string v = TagEntity.AttributeValue(DTagValueAttr);
                            NewNode->DAttributeKeys.push_back(k);
                            NewNode->DAttributes[k] = v;
                        }
                        else if(TagEntity.DType == SXMLEntity::EType::EndElement && TagEntity.DNameData == DNodeTag){
                            break;
                        }
                    }

                    DNodesByIndex.push_back(NewNode);
                    DNodesByID[NodeID] = NewNode;
                }
                else if(nextentity.DType == SXMLEntity::EType::StartElement && nextentity.DNameData == DWayTag){
                    return true; // Start of ways
                }
            } while(src->ReadEntity(nextentity));
        }
        return true;
    }


    bool ParseWays(std::shared_ptr<CXMLReader> src, SXMLEntity &nextentity){
        while(nextentity.DType == SXMLEntity::EType::StartElement && nextentity.DNameData == DWayTag){
            auto WayID = std::stoull(nextentity.AttributeValue(DWayIDAttr));
            auto NewWay = std::make_shared<SWay>();
            NewWay->DID = WayID;

            while(src->ReadEntity(nextentity)){
                if(nextentity.DType == SXMLEntity::EType::StartElement && nextentity.DNameData == DWayNodeTag){
                    NewWay->DNodeIDs.push_back(std::stoull(nextentity.AttributeValue(DWayNodeRefAttr)));
                }
                else if(nextentity.DType == SXMLEntity::EType::StartElement && nextentity.DNameData == DTagTag){
                    std::string k = nextentity.AttributeValue(DTagKeyAttr);
                    std::string v = nextentity.AttributeValue(DTagValueAttr);
                    NewWay->DAttributeKeys.push_back(k);
                    NewWay->DAttributes[k] = v;
                }
                else if(nextentity.DType == SXMLEntity::EType::EndElement && nextentity.DNameData == DWayTag){
                    break;
                }
            }
            DWaysByIndex.push_back(NewWay);
            DWaysByID[WayID] = NewWay;
            if(!src->ReadEntity(nextentity)){
                break;
            }
        }
        return true;
    }

    bool ParseOpenStreetMap(std::shared_ptr<CXMLReader> src){
        SXMLEntity TempEntity;
        
        while(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DOSMTag){
                while(src->ReadEntity(TempEntity)){
                    if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DNodeTag){
                        ParseNodes(src, TempEntity);
                        // ParseNodes might have already consumed the start of ways in TempEntity
                        ParseWays(src, TempEntity);
                    }
                    else if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DWayTag){
                        ParseWays(src, TempEntity);
                    }
                    else if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DOSMTag){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    SImplementation(std::shared_ptr<CXMLReader> src){
        ParseOpenStreetMap(src);
    }

    std::size_t NodeCount() const noexcept{
        return DNodesByIndex.size();
    }

    std::size_t WayCount() const noexcept{
        return DWaysByIndex.size();
    }
    
    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept{
        if(index >= DNodesByIndex.size()){
            return nullptr;
        }
        return DNodesByIndex[index];
    }
    
    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept{
        auto It = DNodesByID.find(id);
        if(It == DNodesByID.end()){
            return nullptr;
        }
        return It->second;
    }
    
    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept{
        if(index >= DWaysByIndex.size()){
            return nullptr;
        }
        return DWaysByIndex[index];
    }
    
    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
        auto It = DWaysByID.find(id);
        if(It == DWaysByID.end()){
            return nullptr;
        }
        return It->second;
    }
};
    
COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src){
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){

}

std::size_t COpenStreetMap::NodeCount() const noexcept {
    return DImplementation->NodeCount();
}
std::size_t COpenStreetMap::WayCount() const noexcept{
    return DImplementation->WayCount();
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept{
    return DImplementation->NodeByIndex(index);
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept{
    return DImplementation->NodeByID(id);
}
std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept{
    return DImplementation->WayByIndex(index);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept{
    return DImplementation->WayByID(id);
}
