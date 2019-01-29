#include<iostream>
#include<stdlib.h>
#include<unordered_map>
#include<vector>
#include<math.h>
#include<time.h>
using namespace std;

struct point{

	int x;
	int y;

	long long id;
};

struct gridID{

	int x;
	int y;

	bool operator==(const gridID &other) const{

		return x == other.x && y == other.y;

	}
};

namespace std{

	template<>
	struct hash<gridID>
	{
		size_t operator()(const gridID& g)const
		{

			return (hash<int>()(g.x)) ^ (hash<int>()(g.y) << 1);
		}


	};


}

double distance(point p1, point p2);
bool comparePointToCell(point p, vector<point>& cellPoints, int alpha);
void successMsg(point p1, point p2);
void equalMsg(point p1, point p2);
void failMsg();

int main(){

	srand(time(NULL));
	cout << "Enter number of points." << endl;
	long long n;
	cin >> n;

	cout << "Enter alpha to check."  << endl;
	int alpha;
	cin >> alpha;

	point* points;
	points = new point[n];


	for(int i=0; i<n; i++){ // generate n points in the 100000*100000 plane

		points[i].x = rand()%100000;
		points[i].y = rand()%100000;

		points[i].id = i;

	}

	unordered_map<gridID,vector<point> > gridCells; // hashmap: each gridID -> vector of points

	gridID gridCell0, gridCell1; // find which grid does the first two points locate
	gridCell0.x = floor(points[0].x/alpha); 
	gridCell0.y = floor(points[0].y/alpha);

	gridCell1.x = floor(points[1].x/alpha);
    gridCell1.y = floor(points[1].y/alpha);

	vector<point> v0; // create a vector of points for the first point
	v0.push_back(points[0]);
	if(gridCell1 == gridCell0){ // if two points are in the same grid, then also put second point into the first vector

		v0.push_back(points[1]);
	}
	else{ // otherwise put the second point into another new-created vector
		vector<point> v1;
		v1.push_back(points[1]);
		gridCells[gridCell1] = v1; // mapping
	}

	gridCells[gridCell0] = v0; // mapping
	
	for(int i=2; i<n; i++){

		//Find i's grid cell.
		gridID iGridCell;
		iGridCell.x = floor(points[i].x/alpha);
		iGridCell.y = floor(points[i].y/alpha);

		auto search = gridCells.find(iGridCell);
		if(search != gridCells.end()){ //Grid cell already exists.  Compare i to all points in this vector.
			if(comparePointToCell(points[i],search->second,alpha))
				return 0;
			gridCells[iGridCell].push_back(points[i]);
		}
		else{
			vector<point> newCell;
			newCell.push_back(points[i]);
			gridCells[iGridCell] = newCell;
		}


		//left
		gridID left;
		left.x = iGridCell.x-1;
		left.y = iGridCell.y;
		search = gridCells.find(left);
		if(search!=gridCells.end() && comparePointToCell(points[i],search->second,alpha))
			return 0;

		gridID right;
		right.x = iGridCell.x+1;
        right.y = iGridCell.y;
        search = gridCells.find(right);
        if(search!=gridCells.end() && comparePointToCell(points[i],search->second,alpha))
            return 0;

		gridID top;
        top.x = iGridCell.x;
        top.y = iGridCell.y+1;
        search = gridCells.find(top);
        if(search!=gridCells.end() && comparePointToCell(points[i],search->second,alpha))
            return 0;

		gridID bottom;
        bottom.x = iGridCell.x;
        bottom.y = iGridCell.y-1;
        search = gridCells.find(bottom);
        if(search!=gridCells.end() && comparePointToCell(points[i],search->second,alpha))
            return 0;

		gridID upright;
        upright.x = iGridCell.x+1;
        upright.y = iGridCell.y+1;
        search = gridCells.find(upright);
        if(search!=gridCells.end() && comparePointToCell(points[i],search->second,alpha))
            return 0;

		gridID downright;
        downright.x = iGridCell.x+1;
        downright.y = iGridCell.y-1;
        search = gridCells.find(downright);
        if(search!=gridCells.end() && comparePointToCell(points[i],search->second,alpha))
            return 0;

		gridID upleft;
        upleft.x = iGridCell.x-1;
        upleft.y = iGridCell.y+1;
        search = gridCells.find(upleft);
        if(search!=gridCells.end() && comparePointToCell(points[i],search->second,alpha))
            return 0;

		gridID downleft;
        downleft.x = iGridCell.x-1;
        downleft.y = iGridCell.y-1;
        search = gridCells.find(downleft);
        if(search!=gridCells.end() && comparePointToCell(points[i],search->second,alpha))
            return 0;
	}
	failMsg();

	return 0;
}

bool comparePointToCell(point p, vector<point>& cellPoints, int alpha){
	for(int j=0; j<cellPoints.size(); j++){
             if(distance(p,cellPoints[j]) < alpha){
                     successMsg(p,cellPoints[j]);
                     return true;
             }
             if(distance(p,cellPoints[j]) == alpha){
                     equalMsg(p,cellPoints[j]);
                     return true;
             }
        }

	return false;
}

void successMsg(point p1, point p2){
	cout << "Point " << p1.id << ": (" << p1.x << ", " << p1.y << ") is closer than alpha to point " << p2.id << ": (" << p2.x << ", " << p2.y << ") " << endl;
	return;
}

void equalMsg(point p1, point p2){
	cout << "Point " << p1.id << ": (" << p1.x << ", " << p1.y << ") is equal to alpha to point " << p2.id << ": (" << p2.x << ", " << p2.y << ") " << endl;
	return;
}

void failMsg() {
	cout << "CP(P) > alpha" << endl;
}

double distance(point p1, point p2){
	return sqrt(pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2));
}
