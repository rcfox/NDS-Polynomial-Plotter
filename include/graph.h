#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <vector>
#include <boost/intrusive_ptr.hpp>

#define INTRUSIVE_PTR_REQS long references; template <class T> friend void ::boost::intrusive_ptr_add_ref(T* p); template <class T> friend void ::boost::intrusive_ptr_release(T* p);

namespace boost
{
 template <class T> inline void intrusive_ptr_add_ref(T* p)
  {
    ++(p->references);
  }

 template <class T> inline void intrusive_ptr_release(T* p)
  {
   if (--(p->references) == 0)
     delete p;
  }
} // namespace boost

class Point
{
public:
    double X;
    double Y;

    INTRUSIVE_PTR_REQS;

    Point(double x, double y) : X(x), Y(y), references(0) {}
};

typedef boost::intrusive_ptr<Point> PointPtr;
typedef std::vector<PointPtr> PointVector;

class Node
{
private:
    typedef boost::intrusive_ptr<Node> NodePtr;
public:
    PointPtr point;
    NodePtr left;
    NodePtr right;

    double value;
    bool calculated;

    INTRUSIVE_PTR_REQS;

    Node(PointPtr p) : point(p), value(0), calculated(false), references(0) {}
    Node(NodePtr l, NodePtr r) : left(l), right(r), value(0), calculated(false), references(0) {}
    double difference();
    double leftX();
    double rightX();
};

typedef boost::intrusive_ptr<Node> NodePtr;
typedef std::vector<NodePtr> NodeVector;

int factorial(int n);
int choose(int n, int k);
double evaluateFunction(int x, std::vector<double> coefficients, PointVector points);

#endif // GRAPH_H_INCLUDED
