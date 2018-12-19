#include "vector3D.h"
#include <vector>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>


bool loadOBJ(
    const char * path,
    std::vector < Vector3D > & out_vertices,
    std::vector < Vector3D > & out_normals
){

	std::vector< unsigned int > vertexIndices, normalIndices;
	std::vector< Vector3D > temp_vertices;
	std::vector< Vector3D > temp_normals;

	unsigned int vertexIndex[3], normalIndex[3];

	fstream file;
	file.open(path);

	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return false;
	}

	char lineHeader[128];
	while(!file.eof()){

		file >> lineHeader;
		if ( strcmp( lineHeader, "v" ) == 0 ){
			Vector3D vertex;
			file >> vertex.vX >> vertex.vY >> vertex.vZ;
			temp_vertices.push_back(vertex);
			std::cout << "Vertex pushed : " << vertex << endl;
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 ){
			Vector3D normal;
			file >> normal.vX >> normal.vY >> normal.vZ;
			temp_normals.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 ){
			int ent;
			char c;
			for(int i=0; i<3; i++){
				file >> ent;
				cout << "entier:" <<  ent << endl;
				vertexIndex[i] = ent;
				file  >> c;
				cout << "char:" << c << endl;
				file  >> c;
				cout << "char:" << c << endl;
				file >> ent;
				cout << "entier:" <<  ent << endl;
				normalIndex[i] = ent;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		
	}
	

	// For each vertex of each triangle
	for( unsigned int i=0; i < vertexIndices.size(); i++ ){
		unsigned int vertexIndex = vertexIndices[i];
		Vector3D vertex = temp_vertices[ vertexIndex-1 ];
		out_vertices.push_back(vertex);
	}
	for( unsigned int i=0; i < normalIndices.size(); i++ ){
		unsigned int normalIndex = normalIndices[i];
		Vector3D normal = temp_normals[ normalIndex-1 ];
		out_normals.push_back(normal);
	}

	return true;
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}