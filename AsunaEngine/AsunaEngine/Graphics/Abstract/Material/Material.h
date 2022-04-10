//
// Created by xiao on 2022/4/9.
//
#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "../../../Foundation/Math/AMath.h"
#include "../Shader.h"
#include "../ConstantBuffer.h"

namespace asuna
{
    enum class MaterialParameterType
    {
        Unknown,
        Float,
        Vector4
    };

    struct MaterialParameter
    {
        int m_Offset;
        MaterialParameterType m_Type;
    };


    class Material
    {
    public:
        explicit Material(const std::string& materialPath);
        static std::shared_ptr<Material> Create(const std::string& materialPath);
        void Apply();
        const std::string& GetName();

        void SetFloat(const std::string& name, float value);
        float GetFloat(const std::string& name);
        void SetVector4(const std::string& name, Vector4f value);
        Vector4f GetVector4(const std::string& name);

        std::shared_ptr<Shader> GetVertexShader()
        {
            return m_VS;
        }

        void SetVertexShader(const std::shared_ptr<Shader>& vs)
        {
            m_VS = vs;
        }

        std::shared_ptr<Shader> GetPixelShader()
        {
            return m_PS;
        }

        void SetPixelShader(const std::shared_ptr<Shader>& ps)
        {
            m_PS = ps;
        }

        int GetParamOffset(const std::string& name);
        MaterialParameterType GetParamType(const std::string& name);

    private:
        void BuildMaterialParametersLayout();

    private:
        std::string m_MaterialName;
        std::shared_ptr<Shader> m_VS;
        std::shared_ptr<Shader> m_PS;
        std::shared_ptr<ConstantBuffer> m_PerMaterial;
        std::unordered_map<std::string, float> m_FloatParams;
        std::unordered_map<std::string, Vector4f> m_Vector4Params;
        std::unordered_map<std::string, MaterialParameter> m_ParamDefines;

    };
}




