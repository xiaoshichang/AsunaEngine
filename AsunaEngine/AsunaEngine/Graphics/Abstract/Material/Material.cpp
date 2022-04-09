//
// Created by xiao on 2022/4/9.
//

#include "Material.h"

using namespace asuna;
using namespace std;

Material::Material(const shared_ptr<Shader> &vs, const shared_ptr<Shader> &ps):
    m_VS(vs),
    m_PS(ps)
{

}

shared_ptr<Material> Material::Create(const shared_ptr<Shader> &vs, const shared_ptr<Shader> &ps)
{
    return make_shared<Material>(vs, ps);
}

void Material::Apply()
{
    m_VS->Bind();
    m_PS->Bind();
}

float Material::GetFloat(const string &name)
{
    auto pair = m_FloatParams.find(name);
    if (pair == m_FloatParams.end())
    {
        return 0;
    }
    else
    {
        return pair->second;
    }
}

void Material::SetFloat(const string &name, float value)
{
    auto pair = m_FloatParams.find(name);
    if (pair == m_FloatParams.end())
    {
        m_FloatParams.insert({name, value});
    }
    else
    {
        pair->second = value;
    }
}

void Material::SetVector4(const string &name, Vector4f value)
{
    auto pair = m_Vector4Params.find(name);
    if (pair == m_Vector4Params.end())
    {
        m_Vector4Params.insert({name, value});
    }
    else
    {
        pair->second = value;
    }
}

Vector4f Material::GetVector4(const string &name)
{
    auto pair = m_Vector4Params.find(name);
    if (pair == m_Vector4Params.end())
    {
        return {0, 0, 0, 0};
    }
    return m_Vector4Params[name];
}


