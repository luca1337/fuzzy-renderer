#pragma once

#include <framework.h>
#include <glm/glm.hpp>

namespace libgraphics
{
    class LIBGRAPHICS_API IShader
    {
    public:
	    virtual ~IShader() = default;
        virtual auto Bind() const -> void = 0;
        virtual auto Unbind() const -> void = 0;
        virtual auto SetMatrix4x4(const std::string_view name, const glm::mat4& m) -> void = 0;
        virtual auto SetFloat(const std::string_view name, const float value) -> void = 0;
        virtual auto SetVec3(const std::string_view name, const glm::vec3& value) -> void = 0;
        virtual auto SetInt(const std::string_view name, const int value) -> void = 0;
        virtual auto SetUint(const std::string_view name, const uint32_t value) -> void = 0;
        virtual auto SetBool(const std::string_view name, const bool value) -> void = 0;
        virtual auto GetID() const -> GLuint = 0;
        virtual auto GetLightsBufferID() const -> GLuint = 0;
    };
}