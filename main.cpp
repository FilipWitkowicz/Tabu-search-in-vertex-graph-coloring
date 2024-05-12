#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <deque>
#include <unordered_map>
#include <stdlib.h>
#include <unordered_set>
#include <utility>
#include <set>
#include <time.h>
#include <iostream>
#include <utility>
using namespace std;

void randomizeInstance(int n, double density) {
    int numberOfEdges = density * n * (n - 1) * 0.5;
    std::vector<std::vector<int>> edges;
    int i = 0;

    while (i < numberOfEdges) {
        int vertex1 = rand() % n;
        int vertex2 = rand() % n;
        //find zwraca pierwsze wystapienie czegos w czyms, wiec porownujemy do konca, bo jak przeszlo na sam koniec to znaczy ze nie znalazlo
        if (find(edges.begin(), edges.end(), std::vector<int>{vertex1, vertex2}) == edges.end()
            && find(edges.begin(), edges.end(), std::vector<int>{vertex2, vertex1}) == edges.end()
            && vertex1 != vertex2) {
            edges.push_back({ vertex1, vertex2 });
            i++;
        }
    }

    std::ofstream file("own_instance.txt");
    if (file.is_open()) {
        file << n << "\n";
        for (const auto& edge : edges) {
            file << edge[0] + 1 << " " << edge[1] + 1 << "\n";
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file\n";
    }
    printf("DZIALA :DD");
}


class Graph {
public:
    int numVertices;
    std::vector<std::vector<int>> adjacencyList;
    std::vector<int> vertexes;
    std::vector<std::vector<int>> matrix;
    int maksimum;
    Graph(int n) : numVertices(n), adjacencyList(n), vertexes(numVertices, 0), matrix(numVertices, std::vector<int>(numVertices, 0)) {}

    int IsInTabu(std::deque<std::pair<int, int>> y, std::pair<int, int> x) {
        for (int i = 0; i < y.size(); i++) {
            if (y[i] == x) {
                return 1;
            }
        }
        return 0;
    }
    void addEdge(int u, int v) {
        if (not std::count(adjacencyList[u].begin(), adjacencyList[u].end(), v)) {
            adjacencyList[u].push_back(v);
            adjacencyList[v].push_back(u);
        }
    }
    void printGraph() {
        for (int i = 0; i < numVertices; ++i) {
            for (int neighbor : adjacencyList[i]) {
                std::cout << " " << neighbor;
            }
            std::cout << std::endl;
        }
    }
    std::vector<int> greedy_coloring() {
        std::vector<int> colors(numVertices);
        for (int i = 0; i < numVertices; ++i) {
            colors[i] = i + 1;
        }

        for (int i = 0; i < numVertices; ++i) {
            for (int j : colors) {
                bool colorIsAvailable = true;
                for (int k : adjacencyList[i]) {
                    if (vertexes[k] == j) {
                        colorIsAvailable = false;
                        break;
                    }
                }
                if (colorIsAvailable) {
                    vertexes[i] = j;
                    break;
                }
            }
        }
        for (int i = 0; i < vertexes.size(); i++) {
            //printf("%d ", vertexes[i]);
        }
        //printf("\n");
        return vertexes;
    }
    int number_of_colours() {
        int maksimum = 0;
        for (int i = 0; i < vertexes.size(); i++) {
            if (vertexes[i] > maksimum) {
                maksimum = vertexes[i];
            }
        }
        return maksimum;
    }
    void LtoM() {
        for (int i = 0; i < numVertices; ++i) {
            for (int j : adjacencyList[i]) {
                matrix[i][j] = 1;
                matrix[j][i] = 1;
            }
        }
    }

    bool coloring_valid_test(std::vector<int> verts) {
        for (int i = 0; i < numVertices; i++) {
            int color = verts[i];
            for (int j = 0; j < adjacencyList[i].size(); j++) {
                if (verts[adjacencyList[i][j]] == color) {
                    return false;
                }
            }
        }
        return true;
    }


    std::vector<int> tabucol(int wanted, int tabu_size, int reps, int nb_max, int czas) {
        srand((unsigned)time(NULL));
        std::vector<int> colors;
        std::unordered_map<int, int> A;
        std::deque<std::pair<int, int>> tabu;
        std::vector<int> solution;
        std::pair<int, int> para;
        std::pair<int, int> para2;
        std::vector<int> new_solution;
        int vertex;
        int conflict_count;
        int nb_iter = 0;

        time_t begin, end;

        time(&begin);

        int vertex_count = vertexes.size();

        for (int i = 1; i <= wanted; i++) {
            colors.push_back(i);
        }

        for (int i = 0; i < vertex_count; i++) {
            int temp;
            temp = vertexes[i];
            solution.push_back(temp);
        }

        for (int i = 0; i < solution.size(); i++) {
            if (solution[i] > wanted) {
                solution[i] = std::rand() % wanted + 1;
            }
            //printf("%d ", solution[i]);
        }
        //printf("\n");

        while (nb_iter < nb_max) {
            conflict_count = 0;
            std::set<int> move_candidates;

            for (int i = 0; i < vertex_count; i++) {
                for (int j = i + 1; j < vertex_count; j++) {
                    if (matrix[i][j] > 0) {
                        if (solution[i] == solution[j]) {
                            move_candidates.insert(i);
                            move_candidates.insert(j);
                            conflict_count += 1;
                        }
                    }
                }
            }

            std::vector<int> move_candidatesL(move_candidates.begin(), move_candidates.end());

            if (conflict_count == 0) {
                //printf("Znaleziono rozwiazanie:\n");
                int maxkol = 0;
                for (int i = 0; i < solution.size(); i++) {
                    //printf("%d ", solution[i]);
                    if (maxkol < solution[i]) {
                        maxkol = solution[i];
                    }
                }

                //printf("\nliczba kolorow: %d\n", maxkol);
                time(&end);
                time_t elapsed = end - begin;
                //printf("pomierzony czas: %ld sekund/y.\n", elapsed);
                return solution;
            }


            printf("Znaleziona liczba konfliktow: %d\n", conflict_count);



            for (int r = 0; r < reps; r++) {
                vertex = move_candidatesL[std::rand() % move_candidatesL.size()];
                int new_color = colors[std::rand() % colors.size()];

                if (solution[vertex] == new_color) {
                    new_color = colors[std::rand() % colors.size()];
                }

                new_solution.clear();

                for (int c = 0; c < solution.size(); c++) {
                    new_solution.push_back(solution[c]);
                }
                para = std::make_pair(vertex, new_color);
                while (IsInTabu(tabu, para)) {
                    new_color = colors[std::rand() % colors.size()];
                    para = std::make_pair(vertex, new_color);
                }

                new_solution[vertex] = new_color;
                int new_conflicts = 0;

                for (int i = 0; i < vertex_count; i++) {
                    for (int j = i + 1; j < vertex_count; j++) {
                        if (matrix[i][j] > 0 and new_solution[i] == new_solution[j]) {
                            new_conflicts += 1;
                        }
                    }
                }

                if (new_conflicts <= conflict_count) {
                    if (A.count(conflict_count) == 0) {
                        A.insert({ conflict_count, conflict_count - 1 });
                    }

                    if (new_conflicts <= A[conflict_count]) {
                        A[conflict_count] = new_conflicts - 1;
                    }
                    break;
                }
            }
            //for (int z = 0; z < tabu.size(); z++) {
              //  printf("(%d, %d) ", tabu[z].first, tabu[z].second);
            //}
            //printf("\n");
            for (int k = 0; k < solution.size(); k++) {
                solution[k] = new_solution[k];
                //printf("%d ", solution[k]);
            }
            para2 = std::make_pair(vertex, solution[vertex]);
            tabu.insert(tabu.begin(), para2);
            if (tabu.size() >= tabu_size) {
                tabu.pop_back();
            }
            //printf("\n");
            //for (int z = 0; z < tabu.size(); z++) {
              //  printf("(%d, %d) ", tabu[z].first, tabu[z].second);
            //}
            //printf("\n");

            nb_iter += 1;

            time(&end);
            time_t elapsed = end - begin;
            if (elapsed > czas) {
                printf("CZAS MINAL ");
                return solution;
            }
        }



        if (conflict_count != 0) {
            printf("No coloring found\n");
            time(&end);
            time_t elapsed = end - begin;
            printf("pomierzony czas: %ld seconds.\n", elapsed);
            return solution;
        }
        else {
            printf("Znaleziono rozwiazanie:\n");
            int maxkol = 0;
            for (int i = 0; i < solution.size(); i++) {
                printf("%d ", solution[i]);
                if (maxkol < solution[i]) {
                    maxkol = solution[i];
                }
            }

            printf("\nliczba kolorow: %d\n", maxkol);
            time(&end);
            time_t elapsed = end - begin;
            printf("pomierzony czas: %ld seconds.\n", elapsed);
            return solution;
        }
    }
};

int main() {
    std::ifstream file("gc500.txt");
    int n;
    file >> n;
    Graph graph(n);
    int u, v;
    if (file.is_open()) {
        while (file >> u >> v) {
            graph.addEdge(u - 1, v - 1);
        }
        file.close();
    }
    //graph.printGraph();

    //int colors[n];
    //colors = graph.greedy_coloring();
    std::vector<int> verts = graph.greedy_coloring();
    bool valid = graph.coloring_valid_test(verts);

    if (valid == true) {
        printf("Kolorowanie poprawne\n");
    }
    else {
        printf("Kolorowanie NIE poprawne\n");
    }

    int num_of_colors = graph.number_of_colours();

    printf("Algorytm zachlanny znalazl rowzwiazanie poprawne z liczba kolorow: %d\n", num_of_colors);
    for (int i = 0; i < graph.vertexes.size(); i++) {
        printf("%d ", graph.vertexes[i]);
    }
    printf("\n");
    graph.LtoM();

    // FUNKCJA PRZYJMUJE: (int wanted, int tabu_size, int reps, int nb_max, int czas)
    verts = graph.tabucol(74, 5, 120, 100000, 180);
    valid = graph.coloring_valid_test(verts);
    if (valid == true) {
        printf("Kolorowanie poprawne\n");
    }
    else {
        printf("Kolorowanie NIE poprawne\n");
    }

    return 0;
}