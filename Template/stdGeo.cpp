#include <bits/stdc++.h>

#if __has_include("../Util/print.h")
#include "../Util/print.h"
#endif

using namespace std;

double EPS = 1e-9;
int INF = INT_MAX / 4;
unsigned long long MOD = 1e9 + 7;
#define TAM 1e6

#define ll long long
#define vi vector<int>
#define vvi vector<vector<int>>
#define ull unsigned long long
#define vll vector<ll>
#define vvll vector<vector<ll>>

#define mp make_pair

#define pb push_back
#define sz size

#define all(x) x.begin(), x.end()

//----------------- Constants & Macros ------------------------------

const double PI = atan(1.0)*4;
const double PI_2 = PI*2;
const double DEG_to_RAD_CONST = PI/180;
const double RAD_TO_DEG_CONST = 180/PI;
#define DEG_TO_RAD(x) x * DEG_to_RAD_CONST
#define RAD_TO_DEG(x) x * RAD_TO_DEG_CONST
#define hypot(x, y) sqrt(x *x + y * y)

//----------------- Point ------------------------------

struct Point{
    double x, y;

    Point() { x = 0, y = 0; }
    Point(double _x, double _y) : x(_x), y(_y) {}

    Point operator + (Point other) const{
        return Point(x + other.x, y + other.y);
    }
    Point operator - (Point other) const{
        return Point(x - other.x, y - other.y);
    }
    Point operator*(double k) const{
        return Point(x * k, y * k);
    }

    double norm(){
        return hypot(x, y);
    }

    Point normalized(){
        return *this * (1 / norm());
    }
};

struct PointPolar{
    double r, a;

    PointPolar() { r = 0, a = 0; }
    PointPolar(double _r, double _a) : r(_r), a(_a) {}
};

#define Vec Point
//----------------- Conversions ------------------------------

PointPolar Polar(Point p){
    return PointPolar(hypot(p.x, p.y), atan2(p.y, p.x));
}

Point toRect(PointPolar p){
    return Point(p.r * cos(p.a), p.r * sin(p.a));
}

//----------------- Vector ------------------------------

double inner(Vec p1, Vec p2){
    return p1.x*p2.x + p1.y*p2.y;
}
double cross(Vec p1, Vec p2){
    return p1.x*p2.y + p2.x*p1.x;
}
Point proj(Vec u, Vec v){
    return v * (inner(u, v) / inner(v, v));
}

//----------------- Misc ------------------------------

double dist(Point &p1, Point &p2){
    return hypot((p1.x - p2.x), (p1.y - p2.y));
}

#define COLINEAR 0
#define LEFT 1
#define RIGHT 2

//Counter clockwise
bool orientation(Point p1, Point p2, Point p3){
    int val = cross(p3 - p2, p2 - p1);
    
    if (abs(val) < EPS) return COLINEAR;
    return (val > 0) ? LEFT : RIGHT; // clock or counterclock wise
}

Point rotateArround(Point &p, Point &o, double theta){
    Point r;
    r.x = cos(theta) * (p.x - o.x) - sin(theta) * (p.y - o.y) + o.x;
    r.y = sin(theta) * (p.x - o.x) + cos(theta) * (p.y - o.y) + o.y;
    return r;
}


double angle(Point a, Point o, Point b){
    return acos(inner(a - o, b - o) / (dist(o, a) * dist(o, b)));
}

//works for int coordinates
bool polarComp(Point a, Point b){
    if (b.y * a.y > 0)
        return cross(a, b) > 0;
    else if (b.y == 0 && b.x > 0)
        return false;
    else if (a.y == 0 && a.x > 0)
        return true;
    else
        return b.y < a.y;
}

//----------------- Lines ------------------------------


// ax+by+c=0
struct Line{
    double a, b, c;
    Point p1,p2;
    Vec v;

    Line() {}
    Line(double _a, double _b, double _c) : a(_a), b(_b), c(_c) {}
    Line(Point _p1, Point _p2){ 
        p1 = _p1, p2 = _p2;              // If b == 0 is vertical, b == 1 otherwise
        if (abs(p1.x - p2.x) < EPS)      // Vertical
            a = 1.0, b = 0.0, c = -p1.x; // default values
        else
        {
            a = -(p1.y - p2.y) / (p1.x - p2.x);
            b = 1.0;
            c = -(a * p1.x) - p1.y;
        }
        v = p1-p2;
    }

    double angCoef(){
        return -a / b;
    }
    double ang(){
        return atan(this->angCoef());
    }
    double offset(){
        return -c / b;
    }
};


double angle(Line l1, Line l2){
    return acos(inner(l1.v, l2.v) / (l1.v.norm()*l2.v.norm()));
}

bool areParallel(Line l1, Line l2){
    return (abs(l1.a - l2.a) < EPS) && (abs(l1.b - l2.b) < EPS);
}

bool areSame(Line l1, Line l2){
    return areParallel(l1, l2) && (abs(l1.c - l2.c) < EPS);
}

// a1x + b1y + c1 = 0 and a2x + b2y + c2 = 0.
Point intersectPoint(Line &l1, Line &l2){
    Point p;

    p.x = (l2.b * l1.c - l1.b * l2.c) / (l2.a * l1.b - l1.a * l2.b);

    if (abs(l1.b) > EPS) // If vertical
        p.y = -(l1.a * p.x + l1.c);
    else
        p.y = -(l2.a * p.x + l2.c);

    return p;
}

Point projPointToLine(Point u, Line l){
    Point a, b;
    if (abs(l.b - 1.0) < EPS){
        a = Point(-l.c / l.a, 0.0);
        b = Point(-l.c / l.a, 1.0);
    }
    else{
        a = Point(0, -l.c / l.b);
        b = Point(1, -(l.c + 1.0) / l.b);
    }
    return a + proj(u - a, b - a);
}

//----------------- Segment ------------------------------

#define Segment Line

/*
Point closestToSegment(Segment s, Point p, ){
    double u = inner(p - s.p1, s.p2 - s.p1) / inner(s.p2 - s.p1, s.p2 - s.p1);

    if (u < 0.0)
        return s.p1;
    if (u > 1.0)
        return s.p2;
    return s.p1 + ((s.p2 - s.p1) * u);
}


bool inside(Segment s, Point p){
    return abs(inner(s.p1-p,s.p2-p) - 1) < EPS;
}

bool insideSeg(Segment s, point r){
    return collinear(s.p1, s.p2, r) && inner(s.p1 - s.p2, r - s.p2) <= 0;
}

bool intersectsSegLine(Segment s, Line l){
    return insideSeg(s, intersec(s, l));
}
bool intersectsSegSeg(Segment s, Segment l){
    Point p = intersec(s, l);
    return insideSeg(s, p) & intersec(l, p);
}
//----------------- Circles ------------------------------

struct Circle
{
    Point center;
    double r;

    Circle() {}
    Circle(Point _center, double _r) : center(_center), r(_r) {}
};

bool inside(Circle &c, Point &p){
    return dist(c.center, p) < c.r;
}

bool intersec(Circle &c1, Circle &c2){
    return (dist(c1.center, c2.center) <= (c1.r + c2.r));
}

//----------------- Triangles ------------------------------

struct Triangle
{
    double ab, ac, bc;
    Point a, b, c;

    Triangle() {}
    Triangle(double _ab, double _ac, double _bc) : ab(_ab), ac(_ac), bc(_bc) {}
    Triangle(Point &_a, Point &_b, Point &_c)
    {
        a = _a, b = _b, c = _c;
        ab = dist(a, b), ac = dist(a, c), bc = dist(b, c);
    }
};

bool exists(Triangle& t){
    return (t.ab + t.bc > t.ac) && (t.ab + t.ac > t.bc) && (t.bc + t.ac > t.ab);
}

double perimeter(Triangle& t){
    return t.ab+t.ac+t.bc;
}

//Cossine law c^2 = a^2 + b^2 - 2ab cos C
Triangle byAngle(double a, double b, double C){
    return Triangle(
        a,b,
        sqrt(a*a+b*b-2*a*b*cos(C))
    );
}

//Heron's
double area(Triangle t){
    double s = perimeter(t) / 2;
    return sqrt(s * (s - t.ab) * (s - t.bc) * (s - t.ac));
}

double rInCircle(double ab, double bc, double ca){
    Triangle t;
    return area(t) / (0.5 * perimeter(t));
}
double rCircumCircle(Triangle &t){
    return t.ab * t.bc * t.ac / (4.0 * area(t));
}

bool inside(Triangle& t, Point& p){
    return (
        area(t) == 
            area(Triangle(t.a, t.b, p))+
            area(Triangle(t.a, p, t.c))+
            area(Triangle(p, t.b, t.c))
    );
}

//----------------- Polygon ------------------------------

typedef vector<Point> Polygon;

double perimeter(Polygon& p){
    double result = 0.0;
    for (int i = 0; i < p.size() - 1; i++) // remember that P[0] = P[n-1]
        result += dist(p[i], p[i + 1]);

    return result;
}

double area(Polygon &p){
    double area = 0.0;
    int j = p.size() - 1;

    for (int i = 0; i < p.size(); i++){
        area += (p[j].x + p[i].x) * (p[j].y - p[i].y);
        j = i;
    }

    return abs(area / 2.0);
}*/

ostream &operator<<(ostream &os, const Point &p)
{
    os << "(x: " << p.x << ", y: " << p.y << ")";
    return os;
}
ostream &operator<<(ostream &os, const PointPolar &p)
{
    os << "(r: " << p.r << ", a: " << p.a << ")";
    return os;
}
/*ostream &operator<<(ostream &os, const Circle &c)
{
    os << "(center: " << c.center << ", r: " << c.r << ")";
    return os;
}

ostream &operator<<(ostream &os, const Line &l)
{
    os << l.a << "x + " << l.b << "y + " << l.c << " = 0";
    return os;
}*/


int main(){
    ios_base::sync_with_stdio(false), cin.tie(0);
    cout << fixed << setprecision(2);

    int t,p,acum,gc;
    cin >> t;

    while(t--){
        Point e;
        cin >> e.x >> e.y
            >> p;

        vector<pair<int, int>> walls(p);

        for(auto& wall : walls)
            cin >> wall.first >> wall.second;


        sort(all(walls));

        acum = 0;
        cin >> gc;
        while(gc--){
            Point g;
            cin >> g.x >> g.y;

            Line eg(e,g);
            double o = eg.offset();
            pair<int,int> pp = mp((int) floor(o),0);
            
            int start = max(
                0,
                (lower_bound(walls.begin(),walls.end(), pp)-walls.begin() - 1)
            );

            for(int i = start; i < walls.size(); i++){
                auto wall = walls[i];
                if (wall.first < o){
                    if (o < wall.second){
                        acum++;
                        break;
                    }
                }
                else
                    break;
            }
                
        }

        cout << acum << "\n";
                

    }



    return 0;
}

//g++ -std=c++11 ../Template/stdGeo.cpp -o s.exe & s.exe < in.txt > out.txt




    /*double ang;
    cin >> ang;
    debug(DEG_TO_RAD(ang));
    cout << endl;

    Point p;
    cin >> p.x >> p.y;
    debug(p);
    debug(toPolar(p));
    cout << endl;

    Circle c;
    cin >> c.center.x >> c.center.y >> c.r;
    debug(c);
    cout << endl;

    Point p1,p2;
    cin >> p1.x >> p1.y >> p2.x >> p2.y;
    Line l1(p1,p2);
    debug(l1);
    debug(RAD_TO_DEG(l1.ang()));

    cin >> p1.x >> p1.y >> p2.x >> p2.y;
    Line l2(p1, p2);
    debug(RAD_TO_DEG(l2.ang()));

    debug(RAD_TO_DEG(angleBetween(l2,l1)));*/
