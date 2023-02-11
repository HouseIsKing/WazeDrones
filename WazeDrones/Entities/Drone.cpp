#include "Drone.h"

#include "../Util/EngineDefaults.h"
#include "../WorldManager.h"
#include <memory>
#include <ranges>

#include "../CameraManager.h"

/**
 * \brief Drone constructor generates tessellation of drone body
 * \param worldManager World drone exists in
 * \param idNodeStart Start node
 * \param id Drone id
 */
Drone::Drone(WorldManager* worldManager, const uint32_t idNodeStart, const uint16_t id) : Collider(-0.5F, -0.5F, -0.5F, 0.5F, 0.5F, 0.5F),
    DroneTransform(EngineDefaults::GetShader()), World(worldManager),
    Start(World->GetGraph()->GetNode(idNodeStart)), End(nullptr), UpOffset(0), PreviousTarget(), Id(id)
{
    DroneTransform.GetTransform(0).SetPosition(Start->GetPosition());
    GenerateTessellationData();
}

/**
 * \brief Drone body tessellation data
 */
void Drone::GenerateTessellationData()
{
    const uint32_t index1 = DroneTransform.AddVertex(Vertex(Collider.GetMinX(), Collider.GetMinY(), Collider.GetMinZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index2 = DroneTransform.AddVertex(Vertex(Collider.GetMaxX(), Collider.GetMinY(), Collider.GetMinZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index3 = DroneTransform.AddVertex(Vertex(Collider.GetMaxX(), Collider.GetMaxY(), Collider.GetMinZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index4 = DroneTransform.AddVertex(Vertex(Collider.GetMinX(), Collider.GetMaxY(), Collider.GetMinZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index5 = DroneTransform.AddVertex(Vertex(Collider.GetMinX(), Collider.GetMinY(), Collider.GetMaxZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index6 = DroneTransform.AddVertex(Vertex(Collider.GetMaxX(), Collider.GetMinY(), Collider.GetMaxZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index7 = DroneTransform.AddVertex(Vertex(Collider.GetMaxX(), Collider.GetMaxY(), Collider.GetMaxZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index8 = DroneTransform.AddVertex(Vertex(Collider.GetMinX(), Collider.GetMaxY(), Collider.GetMaxZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    DroneTransform.AddTriangle(index1);
    DroneTransform.AddTriangle(index3);
    DroneTransform.AddTriangle(index2);
    DroneTransform.AddTriangle(index1);
    DroneTransform.AddTriangle(index4);
    DroneTransform.AddTriangle(index3);
    DroneTransform.AddTriangle(index5);
    DroneTransform.AddTriangle(index6);
    DroneTransform.AddTriangle(index7);
    DroneTransform.AddTriangle(index5);
    DroneTransform.AddTriangle(index7);
    DroneTransform.AddTriangle(index8);
    DroneTransform.AddTriangle(index1);
    DroneTransform.AddTriangle(index2);
    DroneTransform.AddTriangle(index6);
    DroneTransform.AddTriangle(index1);
    DroneTransform.AddTriangle(index6);
    DroneTransform.AddTriangle(index5);
    DroneTransform.AddTriangle(index3);
    DroneTransform.AddTriangle(index4);
    DroneTransform.AddTriangle(index8);
    DroneTransform.AddTriangle(index3);
    DroneTransform.AddTriangle(index8);
    DroneTransform.AddTriangle(index7);
    DroneTransform.AddTriangle(index2);
    DroneTransform.AddTriangle(index3);
    DroneTransform.AddTriangle(index7);
    DroneTransform.AddTriangle(index2);
    DroneTransform.AddTriangle(index7);
    DroneTransform.AddTriangle(index6);
    DroneTransform.AddTriangle(index4);
    DroneTransform.AddTriangle(index1);
    DroneTransform.AddTriangle(index5);
    DroneTransform.AddTriangle(index4);
    DroneTransform.AddTriangle(index5);
    DroneTransform.AddTriangle(index8);
}

/**
 * \brief Drone tick function, updates drone position according to path. And updates path if needed.
 */
void Drone::Tick(const float deltaTime)
{
    if (Path.empty())
    {
        return;
    }
    constexpr float x = 100.0F;
    constexpr float y = 15.0F;
    vec3 currentGoal = Path.front()->GetPosition();
    currentGoal.y += static_cast<float>(UpOffset) * y;
    vec3 currentPos = DroneTransform.GetTransform(0).GetPosition();
    float distanceFactor = 0.0F;
    distanceFactor = std::min(deltaTime * 0.41665F, std::abs(currentPos.y - currentGoal.y));
    DroneTransform.GetTransform(0).SetPosition(currentPos.x, currentPos.y > currentGoal.y ? currentPos.y - distanceFactor : currentPos.y + distanceFactor, currentPos.z);
    currentPos = DroneTransform.GetTransform(0).GetPosition();
    if (const float temp = distance(currentPos, currentGoal); temp < deltaTime * 0.41665F - distanceFactor)
    {
        DroneTransform.GetTransform(0).SetPosition(currentGoal);
        distanceFactor += temp;
        Path.pop_front();
        if (Path.empty())
        {
            World->RemoveDrone(Id);
            IsRemovingFromWorld = true;
            return;
        }
        PreviousTarget = currentGoal;
        currentGoal = Path.front()->GetPosition();
        currentGoal.y += static_cast<float>(UpOffset) * y;
        currentPos = DroneTransform.GetTransform(0).GetPosition();
    }
    DroneTransform.GetTransform(0).SetPosition(currentPos + normalize(currentGoal - currentPos) * (deltaTime * 0.41665F - distanceFactor));
    const vec3 center = DroneTransform.GetTransform(0).GetPosition();
    const std::array<vec3, 8> corners = {
        vec3(center.x - x, center.y - y, center.z - x), vec3(center.x + x, center.y - y, center.z - x),
        vec3(center.x + x, center.y + y, center.z - x), vec3(center.x - x, center.y + y, center.z - x),
        vec3(center.x - x, center.y - y, center.z + x), vec3(center.x + x, center.y - y, center.z + x),
        vec3(center.x + x, center.y + y, center.z + x), vec3(center.x - x, center.y + y, center.z + x)
    };
    for (auto& val : OctreeList | std::views::values)
    {
        val = false;
    }
    for (uint32_t i = 0; i < 8; ++i)
    {
        if (OctreeNode* node = World->GetRoot()->GetLeafAt(corners[i]); OctreeList.emplace(node, true).second)
        {
            node->AddDrone(this);
        }
        else
        {
            OctreeList[node] = true;
        }
    }
    std::vector<OctreeNode*> helper;
    for (auto& [node, isInside] : OctreeList)
    {
        if (!isInside)
        {
            helper.push_back(node);
        }
    }
    for (OctreeNode* node : helper)
    {
        node->RemoveDrone(this);
        OctreeList.erase(node);
    }
    for (const auto& node : OctreeList | std::views::keys)
    {
        for (Drone* drone : node->GetDronesInsideNode())
        {
            if (drone != this)
            {
                AddDroneCollision(drone);
            }
        }
    }
    std::vector<Drone*> temp;
    for (Drone* drone : DroneList)
    {
        if (drone->IsRemovingFromWorld || drone->GetDistanceFromNoY(DroneTransform.GetTransform(0).GetPosition()) > x * x * 2)
        {
            temp.push_back(drone);
        }
    }
    for (Drone* drone : temp)
    {
        DroneList.erase(drone);
    }
    BoundingBox droneCollider = Collider;
    vec3 pos = DroneTransform.GetTransform(0).GetPosition();
    droneCollider.Move(pos.x, pos.y, pos.z);
    for (Drone* drone : DroneList)
    {
        BoundingBox otherCollider = drone->Collider;
        vec3 otherPos = drone->DroneTransform.GetTransform(0).GetPosition();
        otherCollider.Move(otherPos.x, otherPos.y, otherPos.z);
        if (otherCollider.IsIntersecting(droneCollider))
        {
            std::cout << "Drone " << Id << " collided with drone " << drone->Id << std::endl;
            CameraManager::TeleportToCollision = pos;
        }
    }
    std::unordered_map<int, bool> offsetsAllowed;
    offsetsAllowed.reserve(20);
    for (int i = 0; i < 20; i++)
    {
        offsetsAllowed.emplace(i, UpOffset - 1 <= i && i <= UpOffset + 1);
    }
    for (const Drone* drone : DroneList)
    {
        offsetsAllowed[drone->GetUpOffset()] = false;
    }
    for (int i = 0; i < 20; i++)
    {
        if (offsetsAllowed[i])
        {
            UpOffset = i;
            break;
        }
        if (i == 19)
        {
            for (Drone* drone : DroneList)
            {
                if (drone->GetUpOffset() >= UpOffset)
                {
                    drone->ForceClimbUp();
                }
                else
                {
                    if (!drone->ForceClimbDown())
                    {
                        UpOffset += 1;
                    }
                }
            }
        }
    }
}

int Drone::GetUpOffset() const
{
    return UpOffset;
}

uint16_t Drone::GetId() const
{
    return Id;
}

float Drone::GetDistanceFromNoY(const vec3& point)
{
    const vec3 temp = DroneTransform.GetTransform(0).GetPosition() - point;
    const auto temp2 = vec2(temp.x, temp.z);
    return dot(temp2, temp2);
}

void Drone::AddDroneCollision(Drone* drone)
{
    DroneList.emplace(drone);
}

void Drone::RemoveDroneCollision(Drone* drone)
{
    DroneList.erase(drone);
}

void Drone::ForceClimbUp()
{
    UpOffset += 1;
}

bool Drone::ForceClimbDown()
{
    if (UpOffset > 0)
    {
        UpOffset -= 1;
        return true;
    }
    return false;
}

void Drone::Draw()
{
    DroneTransform.Draw();
}

/**
 * \brief Plans path according to end node given.
 * \param idNodeEnd End node(goal)
 */
void Drone::SetDestination(const uint32_t idNodeEnd)
{
    End = World->GetGraph()->GetNode(idNodeEnd);
    PlanPath();
}

/**
 * \brief Searches all neighbors of current node and add/updates them on the priority queue.
 * \param cells Current cells queue(used for picking next cell to search)
 * \param cellsSearched All the cells that have been searched
 * \param current Current cell being searched
 * \return The end A* node if found the goal, nullptr otherwise
 */
AStarNode* Drone::NeighborUpdateAStar(std::priority_queue<AStarNode*, std::vector<AStarNode*>, AStarCompare>& cells, std::unordered_map<GraphNode*, unique_ptr<AStarNode>>& cellsSearched, AStarNode* current) const
{
    for (const auto& connection : current->Node->GetConnections())
    {
        GraphNode* neighbor = World->GetGraph()->GetNode(connection);
        try
        {
            if (AStarNode* cell = cellsSearched.at(neighbor).get(); cell->G > current->G + neighbor->Distance(*current->Node))
            {
                cell->G = current->G + neighbor->Distance(*current->Node);
                cell->Parent = current;
            }
        }
        catch (...)
        {
            cellsSearched.emplace(neighbor, new AStarNode(neighbor, current, current->G + neighbor->Distance(*current->Node), neighbor->Distance(*End)));
            cells.emplace(cellsSearched.at(neighbor).get());
            if (neighbor == End)
            {
                return cellsSearched.at(neighbor).get();
            }
        }
    }
    return nullptr;
}

/**
 * \brief Plans path using A* algorithm.
 */
void Drone::PlanPath()
{
    std::priority_queue<AStarNode*, std::vector<AStarNode*>, AStarCompare> cellQueue;
    std::unordered_map<GraphNode*, unique_ptr<AStarNode>> cellsSearched;
    cellsSearched.emplace(Start, new AStarNode(Start, nullptr, 0, Start->Distance(*End)));
    cellQueue.emplace(cellsSearched.at(Start).get());
    AStarNode* pathFound = nullptr;
    while (pathFound == nullptr)
    {
        AStarNode* current = cellQueue.top();
        cellQueue.pop();
        pathFound = NeighborUpdateAStar(cellQueue, cellsSearched, current);
    }
    while (pathFound->Parent != nullptr)
    {
        Path.emplace_front(pathFound->Node);
        pathFound = pathFound->Parent;
    }
}

/**
 * \brief Draws the current path of the drone.
 * \param lineTessellation Drawing tool
 */
void Drone::DrawPath(TessellationHelper& lineTessellation)
{
    if (Path.empty())
    {
        return;
    }
    uint32_t prevIndex = lineTessellation.AddVertex(Vertex(DroneTransform.GetTransform(0).GetPosition(), 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 1));
    for (const auto& node : Path)
    {
        const uint32_t currentIndex = lineTessellation.AddVertex(Vertex(node->GetPosition(), 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 1));
        lineTessellation.AddTriangle(prevIndex);
        lineTessellation.AddTriangle(currentIndex);
        prevIndex = currentIndex;
    }
}
