#include "RenderItem.h"

using namespace asuna;
using namespace std;

void RenderItem::SetMaterial(int index, const shared_ptr<Material> &material)
{
    {
        if (index >= m_Materials.size())
        {
            return;
        }
        else
        {
            m_Materials[index] = material;
        }
    }
}

std::shared_ptr<Material> RenderItem::GetMaterial(int index) const
{
    if (index >= m_Materials.size())
    {
        return nullptr;
    }
    return m_Materials[index];
}

int RenderItem::GetMaterialCount() const
{
    return m_Materials.size();
}

void RenderItem::AllocateMaterials(int count)
{
    m_Materials.clear();
    for (int i = 0; i < count; ++i)
    {
        m_Materials.push_back(nullptr);
    }
}
