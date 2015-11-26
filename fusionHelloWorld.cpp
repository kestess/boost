#include <boost/fusion/adapted/adt/adapt_adt.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/support/detail/result_of.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/view/zip_view.hpp>

#include <iostream>
#include <cmath>

/*
    https://www.youtube.com/watch?v=6V73Q7ULFi0 from video and pdf
    This is the first example that Michael Caisse presents. I just wanted to see it
    working and have a copy since I couldn't find the source code. 
    Thank you Michael Caisse.	
*/

using namespace boost::fusion;

struct pythagoras
{
	typedef double result_type;

	template<typename T>
	double operator()(double acc, T const& axis) const
	{
		double d = at_c<0>(axis) - at_c<1>(axis);
		return acc + d * d;
	}
};

template<typename P1, typename P2>
double pdistance(P1 p1, P2 p2)
{
	static_assert(result_of::size<P1>::value == result_of::size<P2>::value,
		           "error: point dimensions must match.");

	typedef vector<P1&, P2&> zip_t;
	double accumulated = fold(zip_view<zip_t>(zip_t(p1, p2)),
		                      0,
		                      pythagoras() );
	return sqrt(accumulated);
}

struct mypoint
{
	double x, y;
};

BOOST_FUSION_ADAPT_STRUCT(
	mypoint,
	(double, x)
	(double, y))

class secret_point
{
public:
	secret_point(double x, double y): x_(x), y_(y) {}

	double get_x() const { return x_; }
	void set_x(double d) { x_ = d; }

	double get_y() const { return y_; }
	void set_y(double d) { y_ = d; }

private:
	double x_, y_;
};

BOOST_FUSION_ADAPT_ADT (
	secret_point,
	(double, double, obj.get_x(), obj.set_x(val) )
	(double, double, obj.get_y(), obj.set_y(val) )
)

int main(int argc, char *argv[]) {

	typedef tuple<int, int> point_2d_t;
	typedef tuple<int, int, int> point_3d_t;

    // 2d tuples
	point_2d_t a1(2, 2);
	point_2d_t b1(3, 3);
	double d1 = pdistance(a1, b1);
	std::cout << d1 << std::endl;

    // 3d tuples
	point_3d_t a2(2, 2, 2);
	point_3d_t b2(4, 4, 4);
	double d2 = pdistance(a2, b2);
	std::cout << d2 << std::endl;

    // direct access struct
    point_2d_t a3(2, 2);
    mypoint    b3{3, 3};
    double d3 = pdistance(a3, b3);
    std::cout << d3 << std::endl;

    // getters/setters class
    secret_point a4(2, 2);
    mypoint      b4{3, 3};
    double d4 = pdistance(a4, b4);
    std::cout << d4 << std::endl;

	return 0;
}

