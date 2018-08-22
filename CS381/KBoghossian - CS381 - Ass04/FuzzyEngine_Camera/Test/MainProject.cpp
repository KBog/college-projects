#include "Headers.h"

using namespace std;

/*************************FUNCTIONS TO BE IMPLEMENTED*****************************/

/*
	bool LineSegment3D::Coplanar(LineSegment3D & lns);
	CLASSIFY LineSegment3D::ClassifyToPlane(Plane & pln,Point3D & pinter);
	CLASSIFY Plane::ClassifyToPlane(Plane & pln,Line3D & linter);
	CLASSIFY Sphere::ClassifyToPlane(Plane & pln,Circle3D & cinter,Point3D & pinter);
	CLASSIFY Point2D::ClassifyToLine(Line2D & lin);
	float Point2D::ProjectionOnLineSegment(LineSegment2D & lns,Point2D & pro,bool & onlinesegment);
	float Point3D::ProjectionOnRay(Ray3D & ray,Point3D & pro,bool & onray);
	CLASSIFY Point3D::ClassifyToSphere(Sphere & sph);
	CLASSIFY Point3D::ClassifyToTriangle(Triangle3D & tri,LineSegment3D & segmentintersection,Point3D & pointintersection);
	float Ray2D::AngleBetween(Ray2D & ray)//in radian;
	CLASSIFY Sphere::ClassifyToSphere(Sphere & sph,Circle3D & cinter);
	CLASSIFY Triangle2D::ClassifyToTriangle(Triangle2D & tri,Point2D pinter[6],LineSegment2D segmentoverlap[3],int & numpoints,int & numsegments);
	bool Triangle3D::Parallel(Triangle3D & tri);
	bool LineSegment3D::Alligned(LineSegment3D & lns);
	CLASSIFY Box::ClassifyToPlane(Plane & pln);
	void Box::GetWrappedSphere(Sphere & sph);
	CLASSIFY Circle2D::ClassifyToCircle(Circle2D & cir,Point2D pinter[2]);
	CLASSIFY LineSegment2D::ClassifyToLine(Line2D & lin,Point2D & pinter);
	CLASSIFY Line3D::ClassifyToSphere(Sphere & sph,Point3D pinter[2]);
	CLASSIFY LineSegment2D::ClassifyToRay(Ray2D & ray,Point2D & pinter,LineSegment2D & segmentoverlap);
}

/*************************HELPER FUNCTIONS****************************/

void DisplayClassify(CLASSIFY cla)
{
	switch((int) cla)
	{
	case 0:
		printf("INTERSECT\n");
		break;
	case 1:
		printf("NO_INTERSECT\n");
		break;
	case 2:
		printf("OUTSIDE\n");
		break;
	case 3:
		printf("INSIDE\n");
		break;
	case 4:
		printf("LEFT\n");
		break;
	case 5:
		printf("RIGHT\n");
		break;
	case 6:
		printf("BEHIND\n");
		break;
	case 7:
		printf("BEYOND\n");
		break;
	case 8:
		printf("FRONT\n");
		break;
	case 9:
		printf("BACK\n");
		break;
	case 10:
		printf("INTERSECT_ORIGIN\n");
		break;
	case 11:
		printf("INTERSECT_A\n");
		break;
	case 12:
		printf("INTERSECT_B\n");
		break;
	case 13:
		printf("TANGENT\n");
		break;
	case 14:
		printf("ONE_POINT\n");
		break;
	case 15:
        printf("TWO_POINTS\n");
		break;
	case 16:
		printf("ONE_FACE_INTERSECT\n");
		break;
	case 17:
		printf("ONE_EDGE_INTERSECT\n");
		break;
	case 18:
		printf("ONE_CORNER_INTERSECT\n");
		break;
	case 19:
		printf("OVERLAP\n");
		break;
	case 20:
		printf("PARALLEL\n");
		break;
	case 21:
		printf("ALLIGNED\n");
		break;
	case 22:
		printf("SEGMENT_OVERLAP\n");
		break;
	case 23:
		printf("RAY_OVERLAP\n");
		break;
	case 24:
		printf("SEGMENT_INTERSECT\n");
		break;
	case 25:
		printf("POINT_INTERSECT\n");
		break;
	case 26:
		printf("ENVELOP\n");
		break;
	case 27:
		printf("INTERN_TANGENT\n");
		break;
	case 28:
		printf("EXTERN_TANGENT\n");
		break;
	case 29:
		printf("BACK_INTERSECT\n");
		break;
	case 30:
		printf("FRONT_INTERSECT\n");
		break;
	case 31:
		printf("EXTERN_TANGENT_AND_ENVELOP\n");
		break;
	case 32:
		printf("FUN_ERROR\n");
		break;
	case 33:
		printf("FRONT_TANGENT\n");
		break;
	case 34:
		printf("BACK_TANGENT\n");
		break;
	case 35:
		printf("SEGMENT_POINT_INTERSECT\n");
		break;
	case 36:
		printf("ENVELOP_TANGENT\n");
		break;
	case 37:
		printf("INSIDE_INTERSECT\n");
		break;
	case 38:
		printf("ONE_POINT_INTERSECT\n");
		break;
	case 39:
		printf("ONE_SEGMENT_INTERSECT\n");
		break;
	case 40:
		printf("TWO_POINTS_INTERSECT\n");
		break;
	case 41:
		printf("TWO_SEGMENT_INTERSECT\n");
		break;
	case 42:
		printf("ON_TRIANGLE\n");
		break;
	case 43:
		printf("INSIDE_TRIANGLE\n");
		break;
	case 44:
		printf("COPLANAR_NO_INTERSECT\n");
		break;
	case 45:
		printf("SIDE_TANGENT\n");
		break;
	case 46:
		printf("COPLANAR_SIDE_TANGENT\n");
		break;
	case 47:
		printf("BOTH\n");
		break;
	}
}

void Display2DPoint(Point2D pt)
{
	printf("Point: X=%f, Y=%f\n",pt.X,pt.Y);
}

void Display3DPoint(Point3D pt)
{
	printf("Point: X=%f, Y=%f, Z=%f\n",pt.X,pt.Y,pt.Z);
}

void DisplaySphereInfo(Sphere sph)
{
	printf("Sphere: radius=%f, center.X=%f, center.Y=%f, center.Z=%f\n",sph.radius,sph.center.X,sph.center.Y,sph.center.Z);
}

void Display2DSegment(LineSegment2D lns)
{
	printf("Segment: A.X=%f, A.Y=%f, B.X=%f, B.Y=%f\n",lns.pointA.X,lns.pointA.Y,lns.pointB.X,lns.pointB.Y);
}

void Display3DSegment(LineSegment3D lns)
{
	printf("Segment: A.X=%f, A.Y=%f, A.Z=%f, B.X=%f, B.Y=%f, B.Z=%f\n",lns.pointA.X,lns.pointA.Y,lns.pointA.Z,lns.pointB.X,lns.pointB.Y,lns.pointB.Z);
}

void Display3DLine(Line3D lin)
{
	printf("Vector: X=%f, Y=%f, Z=%f, Point: p.X=%f, p.Y=%f, p.Z=%f\n",lin.direction.X,lin.direction.Y,lin.direction.Z,lin.point.X,lin.point.Y,lin.point.Z);
}

void Display3DCircle(Circle3D cir)
{
	printf("Circle: radius=%f, center: cX=%f, cY=%f, cZ=%f\n",cir.radius,cir.center.X,cir.center.Y,cir.center.Z);
}

/**********************ASSIGNMENTS***********************/


void Task01()
{
	//Function Prototype
		//bool LineSegment3D::Coplanar(LineSegment3D & lns)
	//File Used
		//LineSegment3D.cpp and LineSegment3D.h
	//Function Description
		//The following function tests if both of the 3D line segments are coplanar, (belongs to the same plane).
		//Arguments
			//LineSegment3D & lns: a reference to a LineSegment3D object, it's the line segment to test with.			
		//Return value:
			//it returns true if both of the 3D line segments are coplanar, false otherwise.
			
	//Example1:
		//Input
			/*LineSegment3D lns1(Point3D(1.0f,1.0f,1.0f),Point3D(20.0f,1.0f,1.0f));
			LineSegment3D lns2(Point3D(1.0f,10.0f,1.0f),Point3D(10.0f,1.0f,1.0f));
			printf("Coplanar = %d\n",lns1.Coplanar(lns2));*/
		//Output
			//Coplanar = 1

	//Example2:
		//Input
			/*LineSegment3D lns3(Point3D(1.0f,1.0f,1.0f),Point3D(20.0f,1.0f,1.0f));
			LineSegment3D lns4(Point3D(1.0f,10.0f,1.0f),Point3D(10.0f,1.0f,2.0f));
			printf("Coplanar = %d\n",lns3.Coplanar(lns4));*/
		//Output
			//Coplanar = 0

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task02()
{
	//Function Prototype
		//CLASSIFY LineSegment3D::ClassifyToPlane(Plane & pln,Point3D & pinter)
	//File Used
		//LineSegment3D.cpp and LineSegment3D.h
	//Function Description
		//The following function classify a 3D line segment to a plane.
		//Arguments
			//Plane & pln: a reference to a Plane object, it's the plane to test against.
			//Point3D & pinter: a place to hold the intersection point if it exists.
		//Return value:
			//INTERSECT: the 3D line segment and the plane intersects in one point. 
			//NO_INTERSECT: the 3D line segment and the plane do not intersects.
			//OVERLAP: the 3D line segment lies completely in the plane.
			//PARALLEL: the 3D line segment and the plane are parallel.
			
	//Example1:
		//Input
			/*LineSegment3D lns(Point3D(100.0f,100.0f,0.0f),Point3D(100.0f,100.0f,100.0f));
			Plane pln(Point3D(40.0f,40.0f,40.0f),Vector3D(0.0f,1.0f,0.0f));
			Point3D pinter;
			DisplayClassify(lns.ClassifyToPlane(pln,pinter));
			Display3DPoint(pinter);*/
		//Output
			//PARALLEL
			//Point: X=0.000000, Y=0.000000, Z=0.000000

	//Example2:
		//Input
			/*LineSegment3D lns(Point3D(-100.0f,-100.0f,0.0f),Point3D(100.0f,100.0f,100.0f));
			Plane pln(Point3D(40.0f,40.0f,40.0f),Vector3D(0.0f,1.0f,0.0f));
			Point3D pinter;
			DisplayClassify(lns.ClassifyToPlane(pln,pinter));
			Display3DPoint(pinter);*/
		//Output
			//INTERSECT
			//Point: X=39.999996, Y=39.999996, Z=70.000000

	//Example3:
		//Input
			/*LineSegment3D lns(Point3D(100.0f,90.0f,0.0f),Point3D(100.0f,100.0f,100.0f));
			Plane pln(Point3D(40.0f,40.0f,40.0f),Vector3D(0.0f,1.0f,0.0f));
			Point3D pinter;
			DisplayClassify(lns.ClassifyToPlane(pln,pinter));
			Display3DPoint(pinter);*/
		//Output
			//NO_INTERSECT
			//Point: X=0.000000, Y=0.000000, Z=0.000000

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task03()
{
	//Function Prototype
		//CLASSIFY Plane::ClassifyToPlane(Plane & pln,Line3D & linter)
	//File Used
		//Plane.cpp and Plane.h
	//Function Description
		//The following function classify a plane to another plane.
		//Arguments
			//Plane & pln: a reference to a Plane object, it's the plane to test against.
			//Line3D & linter: a place to hold the intersection 3D line if it exists.
		//Return value:
			//INTERSECT: the 3D planes intersects in a 3D line.
			//OVERLAP: the 3D planes are overlapped (coincident).
			//PARALLEL: the 3D planes are parallel.
			
	//Example1:
		//Input
			/*Plane pln1(Point3D(10.0f,10.0f,10.0f),Vector3D(0.0f,1.0f,0.0f));
			Plane pln2(Point3D(-10.0f,-10.0f,-10.0f),Vector3D(1.0f,0.0f,0.0f));
			Line3D linter;
			DisplayClassify(pln1.ClassifyToPlane(pln2,linter));
			Display3DLine(linter);*/
		//Output
			//INTERSECT
			//Vector: X=0.000000, Y=0.000000, Z=1.000000, Point: p.X=-10.000000, p.Y=10.000000, p.Z=0.000000

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task04()
{
	//Function Prototype
		//CLASSIFY Sphere::ClassifyToPlane(Plane & pln,Circle3D & cinter,Point3D & pinter)
	//File Used
		//Sphere.cpp and Sphere.h
	//Function Description
		//The following function classify a sphere to a plane.
		//Arguments
			//Plane & pln: a reference to a Plane object, it's the plane to test against.
			//Circle3D & cinter: a place to hold the 3D circle intersection if it exists.
			//Point3D & pinter: a place to hold the 3D point intersection if it exist, (when it's tangent).
		//Return value:
			//INTERSECT: the 3D sphere intersect with the plane in a 3D Circle.
			//FRONT: the 3D sphere is in front of the plane, (to the plane-normal direction).
			//BACK: the 3D sphere is to the back of the plane, (to the opposite plane-normal direction).
			//FRONT_TANGENT: the 3D sphere is tangent to the front of the plane.
			//BACK_TANGENT: the 3D sphere is tangent to the back of the plane.
			
	//Example1:
		//Input
			/*Sphere sph(Point3D(10.0f,10.0f,10.0f),20.0f);
			Plane pln(Point3D(0.0f,0.0f,0.0f),Vector3D(0.0f,1.0f,0.0f));
			Circle3D cinter;
			Point3D pinter;
			DisplayClassify(sph.ClassifyToPlane(pln,cinter,pinter));
			Display3DCircle(cinter);
			Display3DPoint(pinter);*/
		//Output
			//INTERSECT
			//Circle: radius=17.320509, center: cX=10.000000, cY=0.000000, cZ=10.000000
			//Point: X=0.000000, Y=0.000000, Z=0.000000

	//Example2:
		//Input
			/*Sphere sph(Point3D(-10.0f,20.0f,-10.0f),20.0f);
			Plane pln(Point3D(0.0f,0.0f,0.0f),Vector3D(0.0f,-1.0f,0.0f));
			Circle3D cinter;
			Point3D pinter;
			DisplayClassify(sph.ClassifyToPlane(pln,cinter,pinter));
			Display3DCircle(cinter);
			Display3DPoint(pinter);*/
		//Output
			//BACK_TANGENT
			//Circle: radius=1.000000, center: cX=0.000000, cY=0.000000, cZ=0.000000
			//Point: X=-10.000000, Y=0.000000, Z=-10.000000

	//Example3:
		//Input
			/*Sphere sph(Point3D(-10.0f,20.0f,-10.0f),20.0f);
			Plane pln(Point3D(100.0f,0.0f,0.0f),Vector3D(-1.0f,0.0f,0.0f));
			Circle3D cinter;
			Point3D pinter;
			DisplayClassify(sph.ClassifyToPlane(pln,cinter,pinter));
			Display3DCircle(cinter);
			Display3DPoint(pinter);*/
		//Output
			//FRONT
			//Circle: radius=1.000000, center: cX=0.000000, cY=0.000000, cZ=0.000000
			//Point: X=0.000000, Y=0.000000, Z=0.000000

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task05()
{
	//Function Prototype
		//CLASSIFY Point2D::ClassifyToLine(Line2D & lin)
	//File Used
		//Point2D.cpp and Point2D.h
	//Function Description
		//The following function classify a 2D point to a 2D line.
		//Arguments
			//Line2D & lin: a reference to a Line2D object, it's the 2D line to test against.
		//Return value:
			//INTERSECT: the point is on the line.
			//RIGHT: the point is to the right of the line, (to the right of the vector direction of the line).
			//LEFT: the point is to the left of the line, (to the left of the vector direction of the line).
						
	//Example1:
		//Input
			/*Point2D pt(10.0f,50.0f);
			Line2D lin(Point2D(50.0f,50.0f),Vector2D(1.0f,0.0f));
			DisplayClassify(pt.ClassifyToLine(lin));*/
		//Output
			//INTERSECT

	//Example2:
		//Input
			/*Point2D pt(10.0f,50.0f);
			Line2D lin(Point2D(50.0f,50.0f),Vector2D(1.0f,-1.0f));
			DisplayClassify(pt.ClassifyToLine(lin));*/
		//Output
			//RIGHT

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task06()
{
	//Function Prototype
		//float Point2D::ProjectionOnLineSegment(LineSegment2D & lns,Point2D & pro,bool & onlinesegment)
	//File Used
		//Point2D.cpp and Point2D.h
	//Function Description
		//The following function gets the projected point of the 2D point on the line that contains the line segment, 
		//and gets if the projected point is on the segment or not. It returns the distance between the 2D point and the projected point.
		//Arguments
			//LineSegment2D & lns: a reference to a LineSegment2D object, it's the 2D line segment to project on.
			//Point2D & pro: a place to hold the projected point.
			//bool & onlinesegment: a boolean to hold if the projected point is on the segment or not.
		//Return value:
			//the returned value is the distance between the 2D point and the projected point on the line holding the line segment.
			//the last two arguments are passed by reference, to hold respectively the projected point and if it intersects with the line segment.

	//Example1:
		//Input
			/*Point2D pt(-60.0f,49.0f);
			LineSegment2D lns(Point2D(50.0f,50.0f),Point2D(50.0f,150.0f));
			Point2D pro;
			bool onlinesegment;
			printf("distance=%f\n",pt.ProjectionOnLineSegment(lns,pro,onlinesegment));
			Display2DPoint(pro);
			printf("onlinesegment=%d\n",onlinesegment);*/
		//Output
			//distance=110.000000
			//Point: X=50.000000, Y=49.000000
			//onlinesegment=0

	//Example2:
		//Input
			/*Point2D pt(-60.0f,50.0f);
			LineSegment2D lns(Point2D(50.0f,50.0f),Point2D(-150.0f,50.0f));
			Point2D pro;
			bool onlinesegment;
			printf("distance=%f\n",pt.ProjectionOnLineSegment(lns,pro,onlinesegment));
			Display2DPoint(pro);
			printf("onlinesegment=%d\n",onlinesegment);*/
		//Output
			//distance=0.000000
			//Point: X=-60.000000, Y=50.000000
			//onlinesegment=1

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task07()
{
	//Function Prototype
		//float Point3D::ProjectionOnRay(Ray3D & ray,Point3D & pro,bool & onray)
	//File Used
		//Point3D.cpp and Point3D.h
	//Function Description
		//The following function gets the projected point of the 3D point on the 3D line that contains the 3D ray, 
		//and gets if the projected point is on the ray or not. It returns the distance between the 3D point and the projected point.
		//Arguments
			//Ray3D & ray: a reference to a Ray3D object, it's the 3D ray to project on.
			//Point3D & pro: a place to hold the projected point.
			//bool & onray: a boolean to hold if the projected point is on the ray or not.
		//Return value:
			//the returned value is the distance between the 3D point and the projected point on the line holding the ray.
			//the last two arguments are passed by reference, to hold respectively the projected point and if it intersects with the ray.

	//Example1:
		//Input
			/*Point3D pt(2.0f,20.0f,2.0f);
			Ray3D ray(Point3D(2.0f,2.0f,2.0f),Vector3D(1.0f,0.0f,0.0f));
			Point3D pro;
			bool onray;
			printf("distance=%f\n",pt.ProjectionOnRay(ray,pro,onray));
			Display3DPoint(pro);
			printf("onray=%d\n",onray);*/
		//Output
			//distance=18.000000
			//Point: X=2.000000, Y=2.000000, Z=2.000000
			//onray=1

	//Example2:
		//Input
			/*Point3D pt(-112.0f,20.0f,2.0f);
			Ray3D ray(Point3D(2.0f,2.0f,200.0f),Vector3D(1.0f,2.0f,3.0f));
			Point3D pro;
			bool onray;
			printf("distance=%f\n",pt.ProjectionOnRay(ray,pro,onray));
			Display3DPoint(pro);
			printf("onray=%d\n",onray);*/
		//Output
			//distance=142.365724
			//Point: X=-45.999996, Y=-93.999992, Z=56.000004
			//onray=0

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task08()
{
	//Function Prototype
		//CLASSIFY Point3D::ClassifyToSphere(Sphere & sph)
	//File Used
		//Point3D.cpp and Point3D.h
	//Function Description
		//The following function classify a 3D point to a sphere.
		//Arguments
			//Sphere & sph: a reference to a Sphere object, it's the sphere to test against.
		//Return value:
			//OUTSIDE: the 3D point is outside the sphere.
			//INSIDE: the 3D point is inside the sphere.
			//INTERSECT: the 3D point intersects with the sphere.

	//Example1:
		//Input
			/*Point3D pt(24.0f,2.0f,2.0f);
			Sphere sph(Point3D(2.0f,2.0f,2.0f),21.0f);
			DisplayClassify(pt.ClassifyToSphere(sph));*/
		//Output
			//OUTSIDE

	//Example2:
		//Input
			/*Point3D pt(24.0f,2.0f,2.0f);
			Sphere sph(Point3D(3.0f,2.0f,2.0f),21.0f);
			DisplayClassify(pt.ClassifyToSphere(sph));*/
		//Output
			//INTERSECT

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task09()
{
	//Function Prototype
		//CLASSIFY Point3D::ClassifyToTriangle(Triangle3D & tri,LineSegment3D & segmentintersection,Point3D & pointintersection)
	//File Used
		//Point3D.cpp and Point3D.h
	//Function Description
		//The following function classify a 3D point to a 3D triangle.
		//Arguments
			//Triangle3D & tri: a reference to a Triangle3D object, it's the triangle to test against.
			//LineSegment3D & segmentintersection: a place to hold the intersected line segment of the triangle, if it exists.
			//Point3D & pointintersection: a place to hold the corner intersection of the triangle with the 3D point if it exists.
		//Return value:
			//OUTSIDE: the 3D point is outside the triangle (not in its plane, or in its plane but outside its boundaries).
			//INSIDE: the 3D point is inside the triangle (it's in the plane of the triangle and inside its boundaries).
			//SEGMENT_INTERSECT: the 3D point intersects with one segment of the triangle.
			//POINT_INTERSECT: the 3D point is on one of the corners of the triangles.

	//Example1:
		//Input
			/*Point3D pt(1.0f,1.0f,1.0f);
			Triangle3D tri(Point3D(1.0f,1.0f,1.0f),Point3D(100.0f,100.0f,100.0f),Point3D(-100.0f,100.0f,100.0f));
			LineSegment3D segmentinter;
			Point3D pointinter;
			DisplayClassify(pt.ClassifyToTriangle(tri,segmentinter,pointinter));
			Display3DSegment(segmentinter);
			Display3DPoint(pointinter);*/
		//Output
			//POINT_INTERSECT
			//Segment: A.X=0.000000, A.Y=0.000000, A.Z=0.000000, B.X=0.000000, B.Y=0.000000, B.Z=0.000000
			//Point: X=1.000000, Y=1.000000, Z=1.000000

	//Example2:
		//Input
			/*Point3D pt(100.0f,100.0f,100.0f);
			Triangle3D tri(Point3D(1.0f,1.0f,1.0f),Point3D(200.0f,200.0f,200.0f),Point3D(-100.0f,100.0f,100.0f));
			LineSegment3D segmentinter;
			Point3D pointinter;
			DisplayClassify(pt.ClassifyToTriangle(tri,segmentinter,pointinter));
			Display3DSegment(segmentinter);
			Display3DPoint(pointinter);*/
		//Output
			//SEGMENT_INTERSECT
			//Segment: A.X=1.000000, A.Y=1.000000, A.Z=1.000000, B.X=200.000000, B.Y=200.000000, B.Z=200.000000
			//Point: X=0.000000, Y=0.000000, Z=0.000000

	//Example3:
		//Input
			/*Point3D pt(100.0f,110.0f,100.0f);
			Triangle3D tri(Point3D(1.0f,1.0f,1.0f),Point3D(200.0f,200.0f,200.0f),Point3D(-100.0f,100.0f,100.0f));
			LineSegment3D segmentinter;
			Point3D pointinter;
			DisplayClassify(pt.ClassifyToTriangle(tri,segmentinter,pointinter));
			Display3DSegment(segmentinter);
			Display3DPoint(pointinter);*/
		//Output
			//OUTSIDE
			//Segment: A.X=0.000000, A.Y=0.000000, A.Z=0.000000, B.X=0.000000, B.Y=0.000000, B.Z=0.000000
			//Point: X=0.000000, Y=0.000000, Z=0.000000

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task10()
{
	//Function Prototype
		//float Ray2D::AngleBetween(Ray2D & ray)//in degree
	//File Used
		//Ray2D.cpp and Ray2D.h
	//Function Description
		//The following function returns the angle in degree between two 2D rays.
		//Arguments
			//Ray2D & ray: a reference to a Ray2D object, it's the ray to test with.
		//Return value:
			//the value of the angle between both of the 2D rays in degree, (the angle is between the vector directions of the rays).

	//Example1:
		//Input
			/*Ray2D ray1(Point2D(2.0f,2.0f),Vector2D(1.0f,0.0f));
			Ray2D ray2(Point2D(4.0f,0.0f),Vector2D(-1.0f,2.0f));
			printf("angle=%f\n",ray1.AngleBetween(ray2));*/
		//Output
			//angle=116.565056

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task11()
{
	//Function Prototype
		//CLASSIFY Sphere::ClassifyToSphere(Sphere & sph,Circle3D & cinter)
	//File Used
		//Sphere.cpp and Sphere.h
	//Function Description
		//The following function classify a sphere to another sphere.
		//Arguments
			//Sphere & sph: a reference to a Sphere object, it's the sphere to test against.
			//Circle3D & cinter: a place to hold the intersected 3D circle if it exists.
		//Return value:
			//INSIDE: the 3D sphere is inside the sphere (sph).
			//ENVELOP: the 3D sphere envelop the sphere (sph).
			//OVERLAP: both of the 3D spheres are overlapped.
			//INTERN_TANGENT: the 3D sphere is internally tangent to the sphere (sph) (one intersected point).
			//EXTERN_TANGENT: the 3D sphere is externally tangent to the sphere (sph) (one intersected point).
			//OUTSIDE: the 3D sphere is completely outside of the sphere (sph) (disjoint).
			//INTERSECT: both of the 3D spheres intersects with a 3D circle.

	//Example1:
		//Input
			/*Sphere sph1(Point3D(10.0f,10.0f,10.0f),20.0f);
			Sphere sph2(Point3D(30.0f,30.0f,30.0f),20.0f);
			Circle3D cinter;
			Point3D ptangent;
			DisplayClassify(sph1.ClassifyToSphere(sph2,cinter,ptangent));
			Display3DCircle(cinter);
			Display3DPoint(ptangent);*/
		//Output
			//INTERSECT
			//Circle: radius=9.999998, center: cX=20.000000, cY=20.000000, cZ=20.000000
			//Point: X=0.000000, Y=0.000000, Z=0.000000

	//Example2:
		//Input
			/*Sphere sph1(Point3D(10.0f,10.0f,10.0f),20.0f);
			Sphere sph2(Point3D(30.0f,10.0f,10.0f),40.0f);
			Circle3D cinter;
			Point3D ptangent;
			DisplayClassify(sph1.ClassifyToSphere(sph2,cinter,ptangent));
			Display3DCircle(cinter);
			Display3DPoint(ptangent);*/
		//Output
			//INTERN_TANGENT
			//Circle: radius=1.000000, center: cX=0.000000, cY=0.000000, cZ=0.000000
			//Point: X=-10.000000, Y=10.000000, Z=10.000000

	//Example3:
		//Input
			/*Sphere sph1(Point3D(10.0f,25.0f,20.0f),20.0f);
			Sphere sph2(Point3D(30.0f,10.0f,10.0f),70.0f);
			Circle3D cinter;
			Point3D ptangent;
			DisplayClassify(sph1.ClassifyToSphere(sph2,cinter,ptangent));
			Display3DCircle(cinter);
			Display3DPoint(ptangent);*/
		//Output
			//INSIDE
			//Circle: radius=1.000000, center: cX=0.000000, cY=0.000000, cZ=0.000000
			//Point: X=0.000000, Y=0.000000, Z=0.000000

	//Example4:
		//Input
			/*Sphere sph1(Point3D(25.0f,0.0f,1.0f),2.0f);
			Sphere sph2(Point3D(30.0f,0.0f,1.0f),3.0f);
			Circle3D cinter;
			Point3D ptangent;
			DisplayClassify(sph1.ClassifyToSphere(sph2,cinter,ptangent));
			Display3DCircle(cinter);
			Display3DPoint(ptangent);*/
		//Output
			//EXTERN_TANGENT
			//Circle: radius=1.000000, center: cX=0.000000, cY=0.000000, cZ=0.000000
			//Point: X=27.000000, Y=0.000000, Z=1.000000

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task12()
{
	//Function Prototype
		//CLASSIFY Triangle2D::ClassifyToTriangle(Triangle2D & tri,Point2D pinter[6],LineSegment2D segmentoverlap[3],int & numpoints,int & numsegments)
	//File Used
		//Triangle2D.cpp and Triangle2D.h
	//Function Description
		//The following function classify a 2D triangle to another 2D triangle.
		//Arguments
			//Triangle2D & tri: a reference to a Triangle2D object, it's the triangle to test against.
			//Point2D pinter[6]: a place to hold the intersected points between both of the triangles, (maximum of six points possible).
			//LineSegment2D segmentoverlap[3]: a place to hold the segments overlapped between both of the triangles, (maximum of three line segments could be overlapped).
			//int & numpoints: holds the number of points intersected.
			//int & numsegments: holds the number of segments overlapped.
		//Return value:
			//INSIDE: the triangle is completely inside the other triangle (tri).
			//ENVELOP: the triangle envelop the other triangle (tri).
			//SEGMENT_POINT_INTERSECT: there's segments overlapping and point intersections between both of the triangles.
			//SEGMENT_INTERSECT: there's segments overlapping between both of the triangles.
			//POINT_INTERSECT: there's points intersection between both of the triangles.
			//OUTSIDE: the triangle is completely outside the other triangle (tri).

	//Example1:
		//Input
			/*Triangle2D tri1(Point2D(0.0f,0.0f),Point2D(100.0f,0.0f),Point2D(50.0f,100.0f));
			Triangle2D tri2(Point2D(0.0f,75.0f),Point2D(100.0f,75.0f),Point2D(50.0f,-175.0f));
			Point2D pinter[6];
			LineSegment2D segmentoverlap[3];
			int numpoints;
			int numsegments;
			DisplayClassify(tri1.ClassifyToTriangle(tri2,pinter,segmentoverlap,numpoints,numsegments));
			printf("numsegments:%d\n",numsegments);
			printf("numpoints:%d\n",numpoints);
			Display2DPoint(pinter[0]);
			Display2DPoint(pinter[1]);
			Display2DPoint(pinter[2]);
			Display2DPoint(pinter[3]);
			Display2DPoint(pinter[4]);
			Display2DPoint(pinter[5]);
			Display2DSegment(segmentoverlap[0]);
			Display2DSegment(segmentoverlap[1]);
			Display2DSegment(segmentoverlap[2]);*/
		//Output
			//POINT_INTERSECT
			//numsegments:0
			//numpoints:6
			//Point: X=85.000000, Y=0.000000
			//Point: X=15.000001, Y=0.000000
			//Point: X=62.500000, Y=75.000000
			//Point: X=89.285713, Y=21.428572
			//Point: X=37.500000, Y=75.000000
			//Point: X=10.714287, Y=21.428574
			//Segment: A.X=0.000000, A.Y=0.000000, B.X=0.000000, B.Y=0.000000
			//Segment: A.X=0.000000, A.Y=0.000000, B.X=0.000000, B.Y=0.000000
			//Segment: A.X=0.000000, A.Y=0.000000, B.X=0.000000, B.Y=0.000000

	//Example2:
		//Input
			/*Triangle2D tri1(Point2D(0.0f,0.0f),Point2D(100.0f,0.0f),Point2D(50.0f,100.0f));
			Triangle2D tri2(Point2D(0.0f,0.0f),Point2D(100.0f,0.0f),Point2D(50.0f,-175.0f));
			Point2D pinter[6];
			LineSegment2D segmentoverlap[3];
			int numpoints;
			int numsegments;
			DisplayClassify(tri1.ClassifyToTriangle(tri2,pinter,segmentoverlap,numpoints,numsegments));
			printf("numsegments:%d\n",numsegments);
			printf("numpoints:%d\n",numpoints);
			Display2DPoint(pinter[0]);
			Display2DPoint(pinter[1]);
			Display2DPoint(pinter[2]);
			Display2DPoint(pinter[3]);
			Display2DPoint(pinter[4]);
			Display2DPoint(pinter[5]);
			Display2DSegment(segmentoverlap[0]);
			Display2DSegment(segmentoverlap[1]);
			Display2DSegment(segmentoverlap[2]);*/
		//Output
			//SEGMENT_INTERSECT
			//numsegments:1
			//numpoints:0
			//Point: X=0.000000, Y=0.000000
			//Point: X=0.000000, Y=0.000000
			//Point: X=0.000000, Y=0.000000
			//Point: X=0.000000, Y=0.000000
			//Point: X=0.000000, Y=0.000000
			//Point: X=0.000000, Y=0.000000
			//Segment: A.X=0.000000, A.Y=0.000000, B.X=100.000000, B.Y=0.000000
			//Segment: A.X=0.000000, A.Y=0.000000, B.X=0.000000, B.Y=0.000000
			//Segment: A.X=0.000000, A.Y=0.000000, B.X=0.000000, B.Y=0.000000

	//Example3:
		//Input
			/*Triangle2D tri1(Point2D(0.0f,0.0f),Point2D(100.0f,0.0f),Point2D(50.0f,100.0f));
			Triangle2D tri2(Point2D(0.0f,0.0f),Point2D(100.0f,0.0f),Point2D(70.0f,100.0f));
			Point2D pinter[6];
			LineSegment2D segmentoverlap[3];
			int numpoints;
			int numsegments;
			DisplayClassify(tri1.ClassifyToTriangle(tri2,pinter,segmentoverlap,numpoints,numsegments));
			printf("numsegments:%d\n",numsegments);
			printf("numpoints:%d\n",numpoints);
			Display2DPoint(pinter[0]);
			Display2DPoint(pinter[1]);
			Display2DPoint(pinter[2]);
			Display2DPoint(pinter[3]);
			Display2DPoint(pinter[4]);
			Display2DPoint(pinter[5]);
			Display2DSegment(segmentoverlap[0]);
			Display2DSegment(segmentoverlap[1]);
			Display2DSegment(segmentoverlap[2]);*/
		//Output
			//SEGMENT_POINT_INTERSECT
			//numsegments:1
			//numpoints:1
			//Point: X=58.333336, Y=83.333328
			//Point: X=0.000000, Y=0.000000
			//Point: X=0.000000, Y=0.000000
			//Point: X=0.000000, Y=0.000000
			//Point: X=0.000000, Y=0.000000
			//Point: X=0.000000, Y=0.000000
			//Segment: A.X=0.000000, A.Y=0.000000, B.X=100.000000, B.Y=0.000000
			//Segment: A.X=0.000000, A.Y=0.000000, B.X=0.000000, B.Y=0.000000
			//Segment: A.X=0.000000, A.Y=0.000000, B.X=0.000000, B.Y=0.000000

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task13()
{
	//Function Prototype
		//bool Triangle3D::Parallel(Triangle3D & tri)
	//File Used
		//Triangle3D.cpp and Triangle3D.h
	//Function Description
		//The following function returns if the 3D triangles are parallel.
		//Arguments
			//Triangle3D & tri: a reference to a Triangle3D object, it's the triangle to test with.
		//Return value:
			//it returns if the 3D triangles are parallel or not.

	//Example1:
		//Input
			/*Triangle3D tri1(Point3D(0.0f,0.0f,0.0f),Point3D(10.0f,10.0f,10.0f),Point3D(10.0f,10.0f,-10.0f));
			Triangle3D tri2(Point3D(15.0f,0.0f,0.0f),Point3D(10.0f,10.0f,10.0f),Point3D(122.0f,10.0f,-10.0f));
			printf("parallel=%d\n",tri1.Parallel(tri2));*/
		//Output
			//parallel=0

	//Example2:
		//Input
			/*Triangle3D tri1(Point3D(0.0f,0.0f,0.0f),Point3D(10.0f,0.0f,0.0f),Point3D(0.0f,0.0f,-10.0f));
			Triangle3D tri2(Point3D(15.0f,2.0f,0.0f),Point3D(25.0f,2.0f,0.0f),Point3D(15.0f,2.0f,-10.0f));
			printf("parallel=%d\n",tri1.Parallel(tri2));*/
		//Output
			//parallel=1

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task14()
{
	//Function Prototype
		//bool LineSegment3D::Alligned(LineSegment3D & lns)
	//File Used
		//LineSegment3D.cpp and LineSegment3D.h
	//Function Description
		//The following function returns true if both of the 3D line segments are alligned, (are on the same line but not intersecting).
		//Arguments
			//LineSegment3D & lns: a reference to a LineSegment3D object, it's the line segment to test with.
		//Return value:
			//It returns if both of the 3D line segments are alligned or not.

	//Example1:
		//Input
			/*LineSegment3D lns1(Point3D(1.0f,1.0f,1.0f),Point3D(20.0f,20.0f,20.0f));
			LineSegment3D lns2(Point3D(-1.0f,-1.0f,-1.0f),Point3D(-20.0f,-20.0f,-20.0f));
			printf("alligned=%d\n",lns1.Alligned(lns2));*/
		//Output
			//alligned=1

	//Example2:
		//Input
			/*LineSegment3D lns1(Point3D(1.0f,3.0f,1.0f),Point3D(20.0f,20.0f,20.0f));
			LineSegment3D lns2(Point3D(-1.0f,-1.0f,-1.0f),Point3D(-20.0f,-20.0f,-20.0f));
			printf("alligned=%d\n",lns1.Alligned(lns2));*/
		//Output
			//alligned=0

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task15()
{
	//Function Prototype
		//CLASSIFY Box::ClassifyToPlane(Plane & pln)
	//File Used
		//Box.cpp and Box.h
	//Function Description
		//The following function classify a 3d box to a plane.
		//Arguments
			//Plane & pln: a reference to a plane object.
		//Return value:
			//FRONT: the box is in front of the plane (to the side of the plane normal).
			//BACK: the box is to the back of the plane.
			//INTERSECT: the plane cut the box in two pieces.
			//FRONT_INTERSECT: the box intersects the plane from the front side (plane normal side) => front tangent.
			//BACK_INTERSECT: the box intersects the plane from the back side (opposite to the normal) => back tangent.

	//Example1:
		//Input
			/*Plane pln(Point3D(15,10,15),Vector3D(0,1,0));
			Point3D coord[8];
			coord[0].X = 10.0f;
			coord[0].Y = 10.0f;
			coord[0].Z = 10.0f;
			coord[1].X = -10.0f;
			coord[1].Y = 10.0f;
			coord[1].Z = 10.0f;
			coord[2].X = -10.0f;
			coord[2].Y = 10.0f;
			coord[2].Z = -10.0f;
			coord[3].X = 10.0f;
			coord[3].Y = 10.0f;
			coord[3].Z = -10.0f;
			coord[4].X = 10.0f;
			coord[4].Y = -10.0f;
			coord[4].Z = 10.0f;
			coord[5].X = -10.0f;
			coord[5].Y = -10.0f;
			coord[5].Z = 10.0f;
			coord[6].X = -10.0f;
			coord[6].Y = -10.0f;
			coord[6].Z = -10.0f;
			coord[7].X = 10.0f;
			coord[7].Y = -10.0f;
			coord[7].Z = -10.0f;
			Box box(coord);
			DisplayClassify(box.ClassifyToPlane(pln));*/
		//Output
			//BACK_INTERSECT

	//Example2:
		//Input
			/*Plane pln(Point3D(25,10,45),Vector3D(2,-1,4));
			Point3D coord[8];
			coord[0].X = 10.0f;
			coord[0].Y = 10.0f;
			coord[0].Z = 10.0f;
			coord[1].X = -10.0f;
			coord[1].Y = 10.0f;
			coord[1].Z = 10.0f;
			coord[2].X = -10.0f;
			coord[2].Y = 10.0f;
			coord[2].Z = -10.0f;
			coord[3].X = 10.0f;
			coord[3].Y = 10.0f;
			coord[3].Z = -10.0f;
			coord[4].X = 10.0f;
			coord[4].Y = -10.0f;
			coord[4].Z = 10.0f;
			coord[5].X = -10.0f;
			coord[5].Y = -10.0f;
			coord[5].Z = 10.0f;
			coord[6].X = -10.0f;
			coord[6].Y = -10.0f;
			coord[6].Z = -10.0f;
			coord[7].X = 10.0f;
			coord[7].Y = -10.0f;
			coord[7].Z = -10.0f;
			Box box(coord);
			DisplayClassify(box.ClassifyToPlane(pln));*/
		//Output
			//BACK

	//Example3:
		//Input
			/*Plane pln(Point3D(25,9,1),Vector3D(2,-1,4));
			Point3D coord[8];
			coord[0].X = 10.0f;
			coord[0].Y = 10.0f;
			coord[0].Z = 10.0f;
			coord[1].X = -10.0f;
			coord[1].Y = 10.0f;
			coord[1].Z = 10.0f;
			coord[2].X = -10.0f;
			coord[2].Y = 10.0f;
			coord[2].Z = -10.0f;
			coord[3].X = 10.0f;
			coord[3].Y = 10.0f;
			coord[3].Z = -10.0f;
			coord[4].X = 10.0f;
			coord[4].Y = -10.0f;
			coord[4].Z = 10.0f;
			coord[5].X = -10.0f;
			coord[5].Y = -10.0f;
			coord[5].Z = 10.0f;
			coord[6].X = -10.0f;
			coord[6].Y = -10.0f;
			coord[6].Z = -10.0f;
			coord[7].X = 10.0f;
			coord[7].Y = -10.0f;
			coord[7].Z = -10.0f;
			Box box(coord);
			DisplayClassify(box.ClassifyToPlane(pln));*/
		//Output
			//INTERSECT

		//To Test Your Function Implementation, Try The Above Examples Here
		//...
}

void Task16()
{
	//Function Prototype
		//void Box::GetWrappedSphere(Sphere & sph)
	//File Used
		//Box.cpp and Box.h
	//Function Description
		//The following function gets the smallest sphere that envelop the box.
		//Arguments
			//Sphere & sph: a reference to a sphere object.
		//Return value:
			//The calculated sphere should be returned in the argument itself, (since it is passed by reference).

	//Example1:
		//Input
			/*Point3D coord[8];
			coord[0].X = 10.0f;
			coord[0].Y = 10.0f;
			coord[0].Z = 10.0f;
			coord[1].X = -10.0f;
			coord[1].Y = 10.0f;
			coord[1].Z = 10.0f;
			coord[2].X = -10.0f;
			coord[2].Y = 10.0f;
			coord[2].Z = -10.0f;
			coord[3].X = 10.0f;
			coord[3].Y = 10.0f;
			coord[3].Z = -10.0f;
			coord[4].X = 10.0f;
			coord[4].Y = -10.0f;
			coord[4].Z = 10.0f;
			coord[5].X = -10.0f;
			coord[5].Y = -10.0f;
			coord[5].Z = 10.0f;
			coord[6].X = -10.0f;
			coord[6].Y = -10.0f;
			coord[6].Z = -10.0f;
			coord[7].X = 10.0f;
			coord[7].Y = -10.0f;
			coord[7].Z = -10.0f;
			Box box(coord);
			Sphere sph;
			box.GetWrappedSphere(sph);
			DisplaySphereInfo(sph);*/
		//Output
			//Sphere: radius=17.320509, center.X=0.000000, center.Y=0.000000, center.Z=0.000000

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task17()
{
	//Function Prototype
		//CLASSIFY Circle2D::ClassifyToCircle(Circle2D & cir,Point2D pinter[2])
	//File Used
		//Circle2D.cpp and Circle2D.h
	//Function Description
		//The following function classify a 2D circle to another 2D circle.
		//Arguments
			//Circle2D & cir: a reference to a Circle2D object, it's the circle to test against.
			//Point2D pinter[2]: a place to hold the 2 intersection points if they exists.
		//Return value:
			//INSIDE: the circle is completely inside the other circle (cir).
			//ENVELOP: the circle envelop the other circle (cir).
			//OVERLAP: both circles are overlapped.
			//INTERN_TANGENT: the circle is internally tangent to the other circle (cir) (one point of intersection).
			//OUTSIDE: the circle is completely outside the other circle (cir).
			//EXTERN_TANGENT: the circle is externally tangent to the other circle (cir) (one point of intersection).
			//TWO_POINTS: the circle intersects the other circle (cir) in two points.

	//Example1:
		//Input
			/*Circle2D cir1(Point2D(10.0f,10.0f),10.0f);
			Circle2D cir2(Point2D(30.0f,10.0f),10.0f);
			Point2D pinter[2];
			DisplayClassify(cir1.ClassifyToCircle(cir2,pinter));
			Display2DPoint(pinter[0]);
			Display2DPoint(pinter[1]);*/
		//Output
			//EXTERN_TANGENT
			//Point: X=20.000000, Y=10.000000
			//Point: X=0.000000, Y=0.000000

	//Example2:
		//Input
			/*Circle2D cir1(Point2D(10.0f,40.0f),40.0f);
			Circle2D cir2(Point2D(130.0f,10.0f),10.0f);
			Point2D pinter[2];
			DisplayClassify(cir1.ClassifyToCircle(cir2,pinter));
			Display2DPoint(pinter[0]);
			Display2DPoint(pinter[1]);*/
		//Output
			//OUTSIDE
			//Point: X=0.000000, Y=0.000000
			//Point: X=0.000000, Y=0.000000

	//Example3:
		//Input
			/*Circle2D cir1(Point2D(100.0f,40.0f),40.0f);
			Circle2D cir2(Point2D(130.0f,10.0f),10.0f);
			Point2D pinter[2];
			DisplayClassify(cir1.ClassifyToCircle(cir2,pinter));
			Display2DPoint(pinter[0]);
			Display2DPoint(pinter[1]);*/
		//Output
			//TWO_POINTS
			//Point: X=134.114380, Y=19.114378
			//Point: X=120.885620, Y=5.885623			

	//Example4:
		//Input
			/*Circle2D cir1(Point2D(100.0f,40.0f),40.0f);
			Circle2D cir2(Point2D(100.0f,10.0f),10.0f);
			Point2D pinter[2];
			DisplayClassify(cir1.ClassifyToCircle(cir2,pinter));
			Display2DPoint(pinter[0]);
			Display2DPoint(pinter[1]);*/
		//Output
			//INTERN_TANGENT
			//Point: X=100.000000, Y=0.000000
			//Point: X=0.000000, Y=0.000000

	//Example5:
		//Input
			/*Circle2D cir1(Point2D(50.0f,40.0f),40.0f);
			Circle2D cir2(Point2D(50.0f,20.0f),10.0f);
			Point2D pinter[2];
			DisplayClassify(cir1.ClassifyToCircle(cir2,pinter));
			Display2DPoint(pinter[0]);
			Display2DPoint(pinter[1]);*/
		//Output
			//ENVELOP
			//Point: X=0.000000, Y=0.000000
			//Point: X=0.000000, Y=0.000000

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task18()
{
	//Function Prototype
		//CLASSIFY LineSegment2D::ClassifyToLine(Line2D & lin,Point2D & pinter)
	//File Used
		//LineSegment2D.cpp and LineSegment2D.h
	//Function Description
		//The following function classify a 2D line segment to a 2D line.
		//Arguments
			//Line2D & lin: a reference to a Line2D object, it's the line to test against.
			//Point2D & pinter: a place to hold the intersection point if it exist.
		//Return value:
			//INTERSECT: the line segment intersects the line in one point.
			//NO_INTERSECT: no intersection between the line segment and the line.
			//OVERLAP: both of the line segment and the line are overlapped.
			//PARALLEL: the line segment and the line are parallel.
			
	//Example1:
		//Input
			/*Line2D lin(Point2D(60.0f,70.0f),Vector2D(-1.0f,1.0f));
			LineSegment2D seg(Point2D(0.0f,100.0f),Point2D(100.0f,200.0f));
			Point2D pinter;
			DisplayClassify(lin.ClassifyToLineSegment(seg,pinter));
			Display2DPoint(pinter);*/
		//Output
			//INTERSECT
			//Point: X=15.000001, Y=115.000000

	//Example2:
		//Input
			/*Line2D lin(Point2D(50.0f,50.0f),Vector2D(1.0f,1.0f));
			LineSegment2D seg(Point2D(0.0f,0.0f),Point2D(100.0f,100.0f));
			Point2D pinter;
			DisplayClassify(lin.ClassifyToLineSegment(seg,pinter));
			Display2DPoint(pinter);*/
		//Output
			//OVERLAP
			//Point: X=0.000000, Y=0.000000

	//Example3:
		//Input
			/*Line2D lin(Point2D(50.0f,50.0f),Vector2D(1.0f,1.0f));
			LineSegment2D seg(Point2D(10.0f,0.0f),Point2D(109.0f,100.0f));
			Point2D pinter;
			DisplayClassify(lin.ClassifyToLineSegment(seg,pinter));
			Display2DPoint(pinter);*/
		//Output
			//NO_INTERSECT
			//Point: X=0.000000, Y=0.000000

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task19()
{
	//Function Prototype
		//CLASSIFY Line3D::ClassifyToSphere(Sphere & sph,Point3D pinter[2])
	//File Used
		//Line3D.cpp and Line3D.h
	//Function Description
		//The following function classify a 3D line to a sphere.
		//Arguments
			//Sphere & sph: a reference to a Sphere object, it's the sphere to test against.
			//Point3D pinter[2]: a place to hold the intersection points if they exists.
		//Return value:
			//INTERSECT: the line intersects the sphere in two points.
			//NO_INTERSECT: the line and the sphere do not intersect.
			//TANGENT: the line is tangent to the sphere, one point of intersection.
			
	//Example1:
		//Input
			/*Line3D lin1(Point3D(100.0f,100.0f,0.0f),Vector3D(-1.0f,-1.0f,0.0f));
			Sphere sph(Point3D(10.0f,10.0f,0.0f),10.0f);
			Point3D pinter[2];
			DisplayClassify(lin1.ClassifyToSphere(sph,pinter));
			Display3DPoint(pinter[0]);
			Display3DPoint(pinter[1]);*/
		//Output
			//INTERSECT
			//Point: X=2.928932, Y=2.928932, Z=0.000000
			//Point: X=17.071068, Y=17.071068, Z=0.000000

	//Example2:
		//Input
			/*Line3D lin1(Point3D(100.0f,100.0f,100.0f),Vector3D(-1.0f,0.0f,-1.0f));
			Sphere sph(Point3D(0.0f,0.0f,0.0f),100.0f);
			Point3D pinter[2];
			DisplayClassify(lin1.ClassifyToSphere(sph,pinter));
			Display3DPoint(pinter[0]);
			Display3DPoint(pinter[1]);*/
		//Output
			//TANGENT
			//Point: X=0.000000, Y=100.000000, Z=0.000000
			//Point: X=0.000000, Y=0.000000, Z=0.000000

	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void Task20()
{
	//Function Prototype
		//CLASSIFY LineSegment2D::ClassifyToRay(Ray2D & ray,Point2D & pinter,LineSegment2D & segmentoverlap)
	//File Used
		//LineSegment2D.cpp and LineSegment2D.h
	//Function Description
		//The following function classify a 2D line segment to a 2D ray.
		//Arguments
			//Ray2D & ray: a reference to a Ray2D object, it's the ray to test against.
			//Point2D & pinter: a place to hold the intersection point if it exist.
			//LineSegment2D & segmentoverlap: a place to hold the line segment object when the line segment and the ray overlap.
		//Return value:
			//INTERSECT: the line segment intersects the ray in one point.
			//NO_INTERSECT: the line segment and the ray do not intersect
			//OVERLAP: the line segment and the ray are overlapped, (one intersection segment).
			//ALLIGNED: the line segment and the ray are alligned, (on the same line, but not intersected).
			//PARALLEL: the line segment and the ray are parallel.
			
	//Example1:
		//Input
			/*LineSegment2D lns(Point2D(10.0f,10.0f),Point2D(40.0f,10.0f));
			Ray2D ray(Point2D(20.0f,10.0f),Vector2D(0.0f,-1.0f));
			Point2D pinter;
			LineSegment2D segmentoverlap;
			DisplayClassify(lns.ClassifyToRay(ray,pinter,segmentoverlap));
			Display2DPoint(pinter);
			Display2DSegment(segmentoverlap);*/
		//Output
			//INTERSECT
			//Point: X=20.000000, Y=10.000000
			//Segment: A.X=0.000000, A.Y=0.000000, B.X=0.000000, B.Y=0.000000

	//Example2:
		//Input
			/*LineSegment2D lns(Point2D(10.0f,10.0f),Point2D(40.0f,10.0f));
			Ray2D ray(Point2D(20.0f,9.0f),Vector2D(0.0f,-1.0f));
			Point2D pinter;
			LineSegment2D segmentoverlap;
			DisplayClassify(lns.ClassifyToRay(ray,pinter,segmentoverlap));
			Display2DPoint(pinter);
			Display2DSegment(segmentoverlap);*/
		//Output
			//NO_INTERSECT
			//Point: X=0.000000, Y=0.000000
			//Segment: A.X=0.000000, A.Y=0.000000, B.X=0.000000, B.Y=0.000000

	//Example3:
		//Input
			/*LineSegment2D lns(Point2D(10.0f,10.0f),Point2D(44.0f,44.0f));
			Ray2D ray(Point2D(21.0f,21.0f),Vector2D(-1.0f,-1.0f));
			Point2D pinter;
			LineSegment2D segmentoverlap;
			DisplayClassify(lns.ClassifyToRay(ray,pinter,segmentoverlap));
			Display2DPoint(pinter);
			Display2DSegment(segmentoverlap);*/
		//Output
			//OVERLAP
			//Point: X=0.000000, Y=0.000000
			//Segment: A.X=10.000000, A.Y=10.000000, B.X=21.000000, B.Y=21.000000
    
	//To Test Your Function Implementation, Try The Above Examples Here
	//...
}

void main()
{
	/******test here each assignment alone******/

	Task01();
	Task02();
	Task03();
	Task04();
	Task05();
	Task06();
	Task07();
	Task08();
	Task09();
	Task10();
	Task11();
	Task12();
	Task13();
	Task14();
	Task15();
	Task16();
	Task17();
	Task18();
	Task19();
	Task20();
}