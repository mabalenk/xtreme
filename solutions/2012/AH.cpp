#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum Robot {
    Robot1,
    Robot2,
    Obstacle,
    Vide,
    Balle
};

struct Point {
    int x, y;
};

istream & operator >> (istream &in, Point &p) {
    in >> p.x;
    in.ignore(1); //virugle
    in >> p.y;

    return in;
}


/* Horizontal direction: Left, Straight ahead or Right */
enum Direction {
    L,
    S,
    R
};

/* Vertical direction: Up/Down */
enum DirVert {
    Haut,
    Bas
};

//Directions left
string directions;
//Directions done
string fait;

/* Read and returns a direction from the directions not yet done */
Direction lire_direction() {
    Direction  ret;
    char c  = directions[0];

    ret = c == 'R' ? R : (c == 'L' ? L : S);

    fait.push_back(c);
    directions.erase(0,1);

    return ret;
}

int main()
{
    Point dimensions;
    cin >> dimensions;

    /* positions robots au début */
    int y1, y2;
    cin >> y1 >> y2;

    int who;
    cin >> who;
    who = who == 1 ? Robot1 : Robot2;

    int num_of_obstacles;
    cin >> num_of_obstacles;

    vector<Point> obstacles;

    /* Rentre les obstacles */
    for (int i = 0; i < num_of_obstacles; i++) {
        Point p;
        cin >> p;
        obstacles.push_back(p);
    }

    cin >> directions;

    // grille [n_rows] [n_columns]
    int grille[dimensions.x][dimensions.y];

    for (int i = 0; i < dimensions.x; i++) {
        for (int j = 0; j < dimensions.y; j++) {
            grille[i][j] = Vide;
        }
    }

    for (int i = 0; i< num_of_obstacles; i++) {
        Point obs = obstacles[i];
        grille[obs.x][obs.y] = Obstacle;
    }

    Point balle;


    /* Initialize ball's direction and position */
    Direction dir_balle = lire_direction();
    DirVert vert;

    if (who == Robot1) {
        balle.x = 0;
        balle.y = y1;
        vert = Bas;
    } else {
        balle.x = dimensions.x-1;
        balle.y = y2;
        vert = Haut;
    }

    while (1) {
        /* Check if the ball hit a wall and change direction before moving */
        if (balle.y == 0 && dir_balle == L) {
            dir_balle = R;
        } else if (balle.y == dimensions.y -1 && dir_balle == R) {
            dir_balle = L;
        }
        /* Check if the ball hit an obstacle and change direction before moving */
        if (grille[balle.x][balle.y] == Obstacle) {
            if (dir_balle == L) {
                dir_balle = R;
            } else if (dir_balle == R) {
                dir_balle = L;
            } else if (dir_balle == S) {
                vert = (vert == Haut ? Bas : Haut);
            }
        }
        /* Move the ball */
        if (dir_balle == L) {
            balle.y -= 1;
        } else if (dir_balle == R) {
            balle.y += 1;
        }
        if (vert == Haut) {
            balle.x -= 1;
        } else {
            balle.x += 1;
        }
        /* Move the robots */
        if (balle.y < y1) {
            y1--;
        } else {
            y1++;
        }
        if (balle.y < y2) {
            y2--;
        } else {
            y2++;
        }

        /* If the ball is on the first row, check if Robot1 is there to catch it */
        if (balle.x == 0) {
            if (balle.y != y1) {
                who = Robot2;
                break;
            } else {
                if (directions.empty()) {
                    who = Vide; //Tie
                    break;
                } else {
                    dir_balle = lire_direction();
                    vert = Bas;
                }
            }
        } else if (balle.x == dimensions.x -1) {
            /* If the ball is on the last row, check if Robot2 is there to catch it */
            if (balle.y != y2) {
                who = Robot1;
                break;
            } else {
                if (directions.empty()) {
                    who = Vide; //Tie
                    break;
                } else {
                    dir_balle = lire_direction();
                    vert = Haut;
                }
            }
        }
    }

    if (who == Robot1) {
        cout << "Winner: Robot1" << endl;
    } else if (who == Robot2) {
        cout << "Winner: Robot2" << endl;
    } else {
        cout << "This game does not have a Winner" << endl;
    }

    cout << "Robot1 At [0," << y1 << "]" << endl;
    cout << "Robot2 At [" << (dimensions.x-1) << "," << y2 << "]" << endl;
    cout << "Balle At [" << balle.x << "," << balle.y << "]" << endl;
    cout << "Sequence: " << fait << endl;

    return 0;
}
