#include <iostream> 
#include <fstream> 
#include <vector> 
#include <cmath> 
#include <ctime> 
#include <cstdlib> 
#include <sstream> 

// Структура, представляющая вершину графа
struct Vertex {
    int x; 
    int y;
    std::string label; 
};

// Структура, представляющая ребро графа
struct Edge {
    size_t vertex1; 
    size_t vertex2; 
};

// Класс для генерации изображения BMP
class BMPGenerator {
public:
    // Конструктор класса
    BMPGenerator(int width, int height, const std::vector<Vertex>& vertices, const std::vector<Edge>& edges)
        : m_width(width), m_height(height), m_vertices(vertices), m_edges(edges) {}

    // Метод для генерации изображения
    void generate(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary); // Открытие файла для записи в бинарном режиме

        writeHeader(file);
        writeImageData(file); 
        file.close();
    }

private:
    // Метод для записи заголовка BMP
    void writeHeader(std::ofstream& file) {
        int imageDataSize = m_width * m_height * 3; // Вычисление размера данных изображения

        // Запись заголовка BMP
        file.put('B').put('M'); // Числа для определения формата файла
        writeInt(file, 14 + 40 + imageDataSize); // Размер файла
        writeInt(file, 0); // Зарезервированное поле
        writeInt(file, 14 + 40); // Смещение до начала данных изображения

        // Запись информации о заголовке изображения
        writeInt(file, 40); // Размер информационного заголовка
        writeInt(file, m_width); // Ширина изображения
        writeInt(file, m_height); // Высота изображения
        writeShort(file, 1); // Число плоскостей
        writeShort(file, 24); // Глубина цвета (24 бита на пиксель)
        writeInt(file, 0); // Тип сжатия 
        writeInt(file, imageDataSize); // Размер данных изображения
        writeInt(file, 2835); // Горизонтальное разрешение (пикселей на метр)
        writeInt(file, 2835); // Вертикальное разрешение (пикселей на метр)
        writeInt(file, 0); // Количество используемых цветов (0 - все цвета)
        writeInt(file, 0); // Количество основных цветов (0 - все цвета)
    }

    // Метод для записи данных изображения
    void writeImageData(std::ofstream& file) {
        std::vector<std::vector<bool>> bitmap(m_height, std::vector<bool>(m_width, false)); // Создание битовой карты

        // Отрисовка ребер графа на изображении
        for (const auto& edge : m_edges) {
            drawLine(bitmap, m_vertices[edge.vertex1].x, m_vertices[edge.vertex1].y,
                m_vertices[edge.vertex2].x, m_vertices[edge.vertex2].y);
        }

        // Отрисовка вершин графа на изображении
        for (size_t i = 0; i < m_vertices.size(); ++i) {
            drawCircle(bitmap, m_vertices[i].x, m_vertices[i].y); // Отрисовка вершины графа

            int labelX = m_vertices[i].x + 7; 
            int labelY = m_vertices[i].y + 7; 

            drawText(bitmap, m_vertices[i].label, labelX, labelY); // Отрисовка метки вершины
        }

        // Запись данных изображения в файл
        for (int y = m_height - 1; y >= 0; --y) {
            for (int x = 0; x < m_width; ++x) {
                file.put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255)) // Запись цвета пикселя
                    .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255)) // Запись цвета пикселя
                    .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255)); // Запись цвета пикселя
            }
        }
    }

    // Метод для отрисовки текста на изображении
    void drawText(std::vector<std::vector<bool>>& bitmap, const std::string& text, int x, int y) {
        int labelWidth = text.length() * 10; 
        int labelHeight = 20; 
        int labelX = x - labelWidth / 10; 
        int labelY = y - labelHeight / 10; 

        // Проверка на выход за границы изображения
        if (labelX < 0 || labelX + labelWidth >= m_width || labelY < 0 || labelY + labelHeight >= m_height) {
            return; 
        }
        // Отрисовка рамки вокруг текста
        for (int i = 0; i < labelWidth; ++i) {
            bitmap[labelY][labelX + i] = true; // Верхняя граница
            bitmap[labelY + labelHeight][labelX + i] = true; // Нижняя граница
        }
        for (int i = 0; i < labelHeight; ++i) {
            bitmap[labelY + i][labelX] = true; // Левая граница
            bitmap[labelY + i][labelX + labelWidth] = true; // Правая граница
        }
        // Отрисовка символов текста
        for (size_t i = 0; i < text.length(); ++i) {
            drawCharacter(bitmap, text[i], labelX + i * 6, labelY + labelHeight / 2 - 3); // Отрисовка отдельного символа
        }
    }

    // Метод для отрисовки отдельного символа текста на изображении
    void drawCharacter(std::vector<std::vector<bool>>& bitmap, char character, int x, int y) {
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

        // Проверка на то, что символ является цифрой
        if (character >= '0' && character <= '9') {
            int index = character - '0'; // Индекс символа
            // Отрисовка символа по матрице
            for (size_t i = 0; i < charTemplates[index].size(); ++i) {
                for (size_t j = 0; j < charTemplates[index][i].size(); ++j) {
                    if (charTemplates[index][i][j]) { // Если в матрице на данной позиции стоит единица
                        if (x + j >= 0 && x + j < bitmap[0].size() && y + i >= 0 && y + i < bitmap.size()) {
                            bitmap[y + i][x + j] = true; // Отрисовка пикселя
                        }
                    }
                }
            }
        }
    }

    // Метод для отрисовки линии на изображении
    void drawLine(std::vector<std::vector<bool>>& bitmap, int x0, int y0, int x1, int y1) {
        int dx = std::abs(x1 - x0); // Приращение по X
        int dy = std::abs(y1 - y0); // Приращение по Y
        int sx = x0 < x1 ? 1 : -1; // Направление по X
        int sy = y0 < y1 ? 1 : -1; // Направление по Y
        int err = dx - dy; // Ошибка

        // Отрисовка линии методом Брезенхэма
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

    // Метод для отрисовки круга на изображении
    void drawCircle(std::vector<std::vector<bool>>& bitmap, int xc, int yc) {
        // Отрисовка круга с радиусом 5 пикселей
        for (int y = yc - 5; y <= yc + 5; ++y) {
            for (int x = xc - 5; x <= xc + 5; ++x) {
                if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
                    double distance = std::sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc)); // Вычисление расстояния от центра круга
                    if (distance <= 5) { // Если расстояние меньше или равно радиусу круга
                        bitmap[y][x] = true; 
                    }
                }
            }
        }
    }

    // Метод для записи 4-байтового целого числа в файл
    void writeInt(std::ofstream& file, int value) {
        file.put(static_cast<char>(value & 0xFF)) // Младший байт
            .put(static_cast<char>((value >> 8) & 0xFF)) // Второй байт
            .put(static_cast<char>((value >> 16) & 0xFF)) // Третий байт
            .put(static_cast<char>((value >> 24) & 0xFF)); // Старший байт
    }

    // Метод для записи 2-байтового целого числа в файл
    void writeShort(std::ofstream& file, short value) {
        file.put(static_cast<char>(value & 0xFF)) // Младший байт
            .put(static_cast<char>((value >> 8) & 0xFF)); // Старший байт
    }

private:
    int m_width; // Ширина изображения
    int m_height; // Высота изображения
    std::vector<Vertex> m_vertices; // Вершины графа
    std::vector<Edge> m_edges; // Ребра графа
};

// Шаблонная функция для преобразования объекта в строку
template<typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

// Функция для чтения входных данных о графе из файла
void readInputFromFile(const std::string& filename, int& numVertices, int& width, int& height) {
    std::ifstream file(filename); // Открытие файла для чтения
    if (file.is_open()) { 
        file >> numVertices >> width >> height; // Чтение данных из файла
        file.close(); 
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl; 
    }
}

// Функция для чтения ребер графа из файла
void readEdgesFromFile(const std::string& filename, std::vector<Edge>& edges) {
    std::ifstream file(filename);
    if (file.is_open()) {
        int vertex1, vertex2;
        while (file >> vertex1 >> vertex2) { // Чтение данных из файла
            Edge edge;
            edge.vertex1 = vertex1; // Заполнение первой вершины ребра
            edge.vertex2 = vertex2; // Заполнение второй вершины ребра
            edges.push_back(edge); // Добавление ребра в вектор
        }
        file.close(); 
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl; 
    }
}


int main() {
    int numVertices, width, height; 
    readInputFromFile("graphs_data.txt", numVertices, width, height);

    std::vector<Vertex> vertices; 
    std::vector<Edge> edges; 

    srand(time(NULL)); // Инициализация генератора случайных чисел

    // Генерация случайных координат и меток для вершин графа
    for (int i = 0; i < numVertices; ++i) {
        vertices.push_back({ rand() % width, rand() % height, to_string(i) }); // Добавление вершины в вектор
    }

    readEdgesFromFile("edge_data.txt", edges); 

    BMPGenerator bmpGenerator(width, height, vertices, edges); // Создание объекта генератора изображения
    bmpGenerator.generate("graph.bmp"); // Генерация изображения и запись в фай

    return 0; 
}