#include "FileReader.h"

#include <iostream>
#include <fstream>

void readInputFromFile(const std::string& filename, int& numVertices, int& width, int& height) {
    std::ifstream file(filename); // �������� ����� ��� ������
    if (file.is_open()) {
        file >> numVertices >> width >> height; // ������ ������ �� �����
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void readEdgesFromFile(const std::string& filename, std::vector<Edge>& edges) {
    std::ifstream file(filename);
    if (file.is_open()) {
        int vertex1, vertex2;
        while (file >> vertex1 >> vertex2) { // ������ ������ �� �����
            Edge edge;
            edge.vertex1 = vertex1; // ���������� ������ ������� �����
            edge.vertex2 = vertex2; // ���������� ������ ������� �����
            edges.push_back(edge); // ���������� ����� � ������
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}