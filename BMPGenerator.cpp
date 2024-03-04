#include "BMPGenerator.h"

BMPGenerator::BMPGenerator(int width, int height, const std::vector<Vertex>& vertices, const std::vector<Edge>& edges)
    : m_width(width), m_height(height), m_vertices(vertices), m_edges(edges) {}

void BMPGenerator::generate(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    writeHeader(file);
    writeImageData(file);
    file.close();
}

void BMPGenerator::writeHeader(std::ofstream& file) {
     int imageDataSize = m_width * m_height * 3; // ���������� ������� ������ �����������

    // ������ ��������� BMP
    file.put('B').put('M'); // ����� ��� ����������� ������� �����
    writeInt(file, 14 + 40 + imageDataSize); // ������ �����
    writeInt(file, 0); // ����������������� ����
    writeInt(file, 14 + 40); // �������� �� ������ ������ �����������

    // ������ ���������� � ��������� �����������
    writeInt(file, 40); // ������ ��������������� ���������
    writeInt(file, m_width); // ������ �����������
    writeInt(file, m_height); // ������ �����������
    writeShort(file, 1); // ����� ����������
    writeShort(file, 24); // ������� ����� (24 ���� �� �������)
    writeInt(file, 0); // ��� ������ 
    writeInt(file, imageDataSize); // ������ ������ �����������
    writeInt(file, 2835); // �������������� ���������� (�������� �� ����)
    writeInt(file, 2835); // ������������ ���������� (�������� �� ����)
    writeInt(file, 0); // ���������� ������������ ������ (0 - ��� �����)
    writeInt(file, 0); // ���������� �������� ������ (0 - ��� �����)
}

void BMPGenerator::writeImageData(std::ofstream& file) {
    std::vector<std::vector<bool>> bitmap(m_height, std::vector<bool>(m_width, false)); // �������� ������� �����

    // ��������� ����� ����� �� �����������
    for (const auto& edge : m_edges) {
        drawLine(bitmap, m_vertices[edge.vertex1].x, m_vertices[edge.vertex1].y,
            m_vertices[edge.vertex2].x, m_vertices[edge.vertex2].y);
    }

    // ��������� ������ ����� �� �����������
    for (size_t i = 0; i < m_vertices.size(); ++i) {
        drawCircle(bitmap, m_vertices[i].x, m_vertices[i].y); // ��������� ������� �����

        int labelX = m_vertices[i].x + 7;
        int labelY = m_vertices[i].y + 7;

        drawText(bitmap, m_vertices[i].label, labelX, labelY); // ��������� ����� �������
    }

    // ������ ������ ����������� � ����
    for (int y = m_height - 1; y >= 0; --y) {
        for (int x = 0; x < m_width; ++x) {
            file.put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255)) // ������ ����� �������
                .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255)) // ������ ����� �������
                .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255)); // ������ ����� �������
        }
    }
}

void BMPGenerator::drawText(std::vector<std::vector<bool>>& bitmap, const std::string& text, int x, int y) {
    int labelWidth = text.length() * 10;
    int labelHeight = 20;
    int labelX = x - labelWidth / 10;
    int labelY = y - labelHeight / 10;

    // �������� �� ����� �� ������� �����������
    if (labelX < 0 || labelX + labelWidth >= m_width || labelY < 0 || labelY + labelHeight >= m_height) {
        return;
    }
    // ��������� ����� ������ ������
    for (int i = 0; i < labelWidth; ++i) {
        bitmap[labelY][labelX + i] = true; // ������� �������
        bitmap[labelY + labelHeight][labelX + i] = true; // ������ �������
    }
    for (int i = 0; i < labelHeight; ++i) {
        bitmap[labelY + i][labelX] = true; // ����� �������
        bitmap[labelY + i][labelX + labelWidth] = true; // ������ �������
    }
    // ��������� �������� ������
    for (size_t i = 0; i < text.length(); ++i) {
        drawCharacter(bitmap, text[i], labelX + i * 6, labelY + labelHeight / 2 - 3); // ��������� ���������� �������
    }
}

void BMPGenerator::drawCharacter(std::vector<std::vector<bool>>& bitmap, char character, int x, int y) {
    const std::vector<std::vector<std::vector<bool>>> charTemplates = {
             {
                {0, 1, 1, 1, 0},
                {1, 0, 0, 0, 1},
                {1, 0, 0, 0, 1},
                {1, 0, 0, 0, 1},
                {0, 1, 1, 1, 0}
            },
            {
                {0, 0, 1, 0, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 1, 1, 0}
            },
            {
                {1, 1, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 0, 0, 0},
                {1, 0, 0, 0, 0},
                {1, 1, 1, 1, 0}
            },
            {
                {0, 1, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 1, 1, 1, 0}
            },
            {
                {0, 1, 0, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 1, 0}
            },
            {
                {0, 0, 1, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 1, 1, 0}
            },
            {
                {0, 1, 1, 1, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 1, 1, 1, 0}
            },
            {
                {0, 1, 1, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 1, 0}
            },
            {
                {0, 1, 1, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 1, 1, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 1, 1, 1, 0}
            },
            {
                {0, 1, 1, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 1, 1, 1, 0}
            },
    };

    // �������� �� ��, ��� ������ �������� ������
    if (character >= '0' && character <= '9') {
        int index = character - '0'; // ������ �������
        // ��������� ������� �� �������
        for (size_t i = 0; i < charTemplates[index].size(); ++i) {
            for (size_t j = 0; j < charTemplates[index][i].size(); ++j) {
                if (charTemplates[index][i][j]) { // ���� � ������� �� ������ ������� ����� �������
                    if (x + j >= 0 && x + j < bitmap[0].size() && y + i >= 0 && y + i < bitmap.size()) {
                        bitmap[y + i][x + j] = true; // ��������� �������
                    }
                }
            }
        }
    }
}

void BMPGenerator::drawLine(std::vector<std::vector<bool>>& bitmap, int x0, int y0, int x1, int y1) {
    int dx = std::abs(x1 - x0); // ���������� �� X
    int dy = std::abs(y1 - y0); // ���������� �� Y
    int sx = x0 < x1 ? 1 : -1; // ����������� �� X
    int sy = y0 < y1 ? 1 : -1; // ����������� �� Y
    int err = dx - dy; // ������

    // ��������� ����� ������� ����������
    while (x0 != x1 || y0 != y1) {
        if (x0 >= 0 && x0 < m_width && y0 >= 0 && y0 < m_height) {
            bitmap[y0][x0] = true;
        }
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void BMPGenerator::drawCircle(std::vector<std::vector<bool>>& bitmap, int xc, int yc) {
    // ��������� ����� � �������� 5 ��������
    for (int y = yc - 5; y <= yc + 5; ++y) {
        for (int x = xc - 5; x <= xc + 5; ++x) {
            if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
                double distance = std::sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc)); // ���������� ���������� �� ������ �����
                if (distance <= 5) { // ���� ���������� ������ ��� ����� ������� �����
                    bitmap[y][x] = true;
                }
            }
        }
    }
}

void BMPGenerator::writeInt(std::ofstream& file, int value) {
    file.put(static_cast<char>(value & 0xFF)) // ������� ����
        .put(static_cast<char>((value >> 8) & 0xFF)) // ������ ����
        .put(static_cast<char>((value >> 16) & 0xFF)) // ������ ����
        .put(static_cast<char>((value >> 24) & 0xFF)); // ������� ����
}

void BMPGenerator::writeShort(std::ofstream& file, short value) {
    file.put(static_cast<char>(value & 0xFF)) // ������� ����
        .put(static_cast<char>((value >> 8) & 0xFF)); // ������� ����
}
