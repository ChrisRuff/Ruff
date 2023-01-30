#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <type_traits>

#include <ruff/core/models/point.hpp>
#include <ruff/geometry/line.hpp>
#include <ruff/geometry/face.hpp>
namespace ruff
{
namespace geometry
{
	template<class P>
	class Mesh
	{
	private:
		std::vector<P> vertices{};
		std::vector<Face> faces{};
		size_t n_verts{ 0 };
		size_t n_edges{ 0 };
		size_t n_faces{ 0 };

	public:
		Mesh() = default;

		template<typename It>
		requires std::
		  is_same_v<P, typename std::iterator_traits<It>::value_type>
		void add_vertices(It begin, const It end)
		{
			while(begin != end)
			{
				vertices.push_back(*begin);
				++n_verts;
				++begin;
			}
		}
		void add_face(const Face& face)
		{
			faces.push_back(face);
			++n_faces;
			n_edges += face.nedges();
		}
		size_t nfaces() { return n_faces; };
		size_t nedges() { return n_edges; };
		size_t nverts() { return n_verts; };
		Face face(size_t idx) { return faces[idx]; };
		P vert(size_t idx) { return vertices[idx]; };

		void write_ply(const std::filesystem::path& out_path)
		{
			std::ofstream out_file(out_path);
			if(out_file.is_open())
			{
				out_file << std::string("ply\n");
				out_file << std::string("format ascii 1.0\n");
				out_file << std::string("element vertex ")
				              + std::to_string(n_verts) + "\n";
				out_file << std::string("property float x\n");
				out_file << std::string("property float y\n");
				out_file << std::string("property float z\n");
				out_file << std::string("property uchar red\n");
				out_file << std::string("property uchar green\n");
				out_file << std::string("property uchar blue\n");
				out_file << std::string("element face ")
				              + std::to_string(n_faces) + "\n";
				out_file << std::string(
				  "property list uchar int "
				  "vertex_index\n");
				out_file << std::string("element edge ")
				              + std::to_string(n_edges) + "\n";
				out_file << std::string("property int vertex1\n");
				out_file << std::string("property int vertex2\n");
				out_file << std::string("property uchar red\n");
				out_file << std::string("property uchar green\n");
				out_file << std::string("property uchar blue\n");
				out_file << std::string("end_header\n");
				for(const auto pt : vertices)
				{
					for(const auto val : pt.vals) { out_file << val << " "; }
					out_file << "255 255 255\n";
				}
				for(size_t i = 0; i < n_faces; ++i)
				{
					const Face& f = faces[i];
					const auto& verts = f.getVerts();
					out_file << std::to_string(verts.size()) + " ";
					for(const auto& v : verts)
					{
						out_file << std::to_string(v) + " ";
					}
					out_file << "\n";
				}
				for(size_t i = 0; i < n_faces; ++i)
				{
					const Face& f = faces[i];
					for(size_t j = 0; j < f.nedges(); ++j)
					{
						out_file << std::to_string(f.edge(j).start()) << " ";
						out_file << std::to_string(f.edge(j).end()) << " ";
						out_file << "255 255 255\n";
					}
				}
				out_file.close();
			}
		}
	};

	template<typename T, size_t dim>
	Mesh<Point<T, dim>> read_obj(const std::filesystem::path& file)
	{
		Mesh<Point<T, dim>> mesh;
		std::ifstream in_file(file);

		if(in_file.is_open())
		{
			std::vector<Point<T, dim>> vertices;
			std::string line;

			while(getline(in_file, line))
			{
				std::string command;
				std::array<std::string, dim> input;
				std::stringstream line_stream(line);

				line_stream >> command >> input[0] >> input[1] >> input[2];
				if(input[2].size() == 0) { continue; }
				if(!command.compare("v"))
				{
					std::array<T, dim> vert;
					for(size_t i = 0; i < dim; ++i)
					{
						std::stringstream inputss(input[i]);
						inputss >> vert[i];
					}
					vertices.emplace_back(vert);
				}
				else if(!command.compare("f"))
				{
					std::array<size_t, dim> face_points{};
					for(size_t i = 0; i < dim; ++i)
					{
						// Take the first value
						std::stringstream inputss(
						  input[i].substr(0, input[i].find("/")));
						size_t idx;
						inputss >> idx;
						face_points[i] = idx - 1;
					}

					Face face(face_points.begin(), face_points.end());
					mesh.add_face(face);
				}
			}
			mesh.add_vertices(vertices.begin(), vertices.end());
		}
		return mesh;
	}
};// namespace geometry
};// namespace ruff
