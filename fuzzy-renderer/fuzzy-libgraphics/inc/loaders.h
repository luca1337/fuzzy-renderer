#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace libgraphics::loaders
{
	static auto load_obj(const std::string& file_name, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals) -> void
	{
        std::ifstream file(file_name);
        if (!file.is_open())
        {
            std::cout << "Error while opening file" << std::endl;
            return;
        }

        auto temp_v = std::vector<glm::vec3>{};
        auto temp_vn = std::vector<glm::vec3>{};
        auto temp_vt = std::vector<glm::vec2>{};
        auto indices_v = std::vector<unsigned>{};
        auto indices_vn = std::vector<unsigned>{};
        auto indices_vt = std::vector<unsigned>{};

        std::string line = {};
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string header = {};
            iss >> header;

            if (header == "v")
            {
                glm::vec3 vertex = {};
                iss >> vertex.x >> vertex.y >> vertex.z;
                temp_v.emplace_back(vertex);
            }
            else if (header == "vt")
            {
                glm::vec2 texture = {};
                iss >> texture.x >> texture.y;
                temp_vt.emplace_back(texture);
            }
            else if (header == "vn")
            {
                glm::vec3 normal = {};
                iss >> normal.x >> normal.y >> normal.z;
                temp_vn.emplace_back(normal);
            }
            else if (header == "f")
            {
                std::string face_indices = {};
                while (iss >> face_indices)
                {
                    std::ranges::replace(face_indices, '/', ' ');
                    std::istringstream face_indices_stream(face_indices);
                    int v_index, vt_index, vn_index;
                    face_indices_stream >> v_index >> vt_index >> vn_index;
                    indices_v.emplace_back(v_index);
                    indices_vt.emplace_back(vt_index);
                    indices_vn.emplace_back(vn_index);
                }
            }
        }

        for (unsigned int vertex_index : indices_v)
        {
            out_vertices.emplace_back(temp_v[vertex_index - 1]);
        }

        for (unsigned int vertex_normal_index : indices_vn)
        {
            out_normals.emplace_back(temp_vn[vertex_normal_index - 1]);
        }

        for (size_t i = 0; i < indices_v.size(); i++)
        {
            out_uvs.emplace_back(temp_vt[indices_vt[i] - 1]);
        }
	}
}
