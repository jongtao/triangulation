#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FALSE 0
#define TRUE 1


/*
static void CircleApprox(double x0, double y0, double r0,
		double x1, double y1, double r1,
		double x2, double y2, double r2,
		ref double sol_x, ref double sol_y)
{
	// Get ratios for circle A
	double dx_a = x1 - x0;
	double dy_a = y1 - y0;
	double ratio_a = r1 / r0;

	dx_a = dx_a / ratio_a;
	dy_a = dy_a / ratio_a;

	// Compute new approximate circle A
	double x_a = x0 + dx_a;
	double y_a = y0 + dy_a;
	double r_a = (r1 + r2) / 2;

	// Get ratios for circle B
	double dx_b = x2 - x_a;
	double dy_b = y2 - y_a;
	double ratio_b = r2 / r_a;

	dx_b = dx_b / ratio_b;
	dy_b = dy_b / ratio_b;


	// Get approximate point
	double x_b = x_a + dx_b;
	double y_b = y_a + dy_b;

	sol_x = x_b;
	sol_y = y_b;
} // CircleApprox()
*/

#define IS_OK (1)
#define SEPARATE (-1)
#define CONTAINED (-2)
#define TOLERANCE_ERR (-3)


int CircleIntersect(double x0, double y0, double* r0,
		double x1, double y1, double* r1,
		double x2, double y2, double* r2,
		double* sol_x, double* sol_y, double tolerance, double* min_e)
{
	double a, dx, dy, d, h, rx, ry;
	double point2_x, point2_y;

	/* dx and dy are the vertical and horizontal distances between
	 * the circle centers.
	 */
	dx = x1 - x0;
	dy = y1 - y0;

	/* Determine the straight-line distance between the centers. */
	d = sqrt((dy * dy) + (dx * dx));

	/* Check for solvability. */
	if (d > (*r0 + *r1))
	{
		/* no solution. circles do not intersect. */
		return SEPARATE;
	}

	double contain_step_size = 0.05;
	int steps = 0;
	int max_steps = 100;

	while(d < fabsl(*r0 - *r1))
	{
		// The smaller circle is the inner one.
		// Increase the inner one's radius to touch the outer's radius.
		if(*r0 < *r1)
		{
			fprintf(stderr, "grow1\n");
			*r0 += contain_step_size;
			*r1 -= contain_step_size;
		}
		else
		{
			fprintf(stderr, "grow2\n");
			*r1 += contain_step_size;
			*r0 -= contain_step_size;
		}

		steps++;

		if(steps > max_steps)
			return CONTAINED;
	}

	/*
	if (d < abs(r0 - r1))
	{
		// no solution. one circle is contained in the other 
		return CONTAINED;
	}
	*/

	/* 'point 2' is the point where the line through the circle
	 * intersection points crosses the line between the circle
	 * centers.
	 */

	/* Determine the distance from point 0 to point 2. */
	a = ((*r0 * *r0) - (*r1 * *r1) + (d * d)) / (2.0 * d);

	/* Determine the coordinates of point 2. */
	point2_x = x0 + (dx * a / d);
	point2_y = y0 + (dy * a / d);

	/* Determine the distance from point 2 to either of the
	 * intersection points.
	 */
	h = sqrt((*r0 * *r0) - (a * a));

	/* Now determine the offsets of the intersection points from
	 * point 2.
	 */
	rx = -dy * (h / d);
	ry = dx * (h / d);

	/* Determine the absolute intersection points. */
	double intersectionPoint1_x = point2_x + rx;
	double intersectionPoint2_x = point2_x - rx;
	double intersectionPoint1_y = point2_y + ry;
	double intersectionPoint2_y = point2_y - ry;

/*
	Console.WriteLine("INTERSECTION Circle1 AND Circle2:" +
			"(" + intersectionPoint1_x + "," + intersectionPoint1_y + ")" +
			" AND (" + intersectionPoint2_x + "," + intersectionPoint2_y + ")");
*/

	/* Lets determine if circle 3 intersects at either of the above intersection points. */
	dx = intersectionPoint1_x - x2;
	dy = intersectionPoint1_y - y2;
	double d1 = sqrt((dy * dy) + (dx * dx));

	dx = intersectionPoint2_x - x2;
	dy = intersectionPoint2_y - y2;
	double d2 = sqrt((dy * dy) + (dx * dx));

	double e1 = fabsl(d1 - *r2);
	double e2 = fabsl(d2 - *r2);


	// examine the minimum error
	if(e1 < e2)
	{
		*sol_x = intersectionPoint1_x;
		*sol_y = intersectionPoint1_y;

		if(e1 > tolerance)
		{
			*min_e = e1;
			return TOLERANCE_ERR;
		}
	}
	else // e1 > e2
	{
		*sol_x = intersectionPoint2_x;
		*sol_y = intersectionPoint2_y;

		if(e2 > tolerance)
		{
			*min_e = e2;
			return TOLERANCE_ERR;
		}
	}

	return IS_OK;



/*
	if (abs(d1 - r2) < tolerance)
	{
		*sol_x = intersectionPoint1_x;
		*sol_y = intersectionPoint1_y;

		//Console.WriteLine("INTERSECTION Circle1 AND Circle2 AND Circle3:" + 
		//		"(" + intersectionPoint1_x + "," + intersectionPoint1_y + ")");
	}
	else if (abs(d2 - r2) < tolerance)
	{
		*sol_x = intersectionPoint1_x;
		*sol_y = intersectionPoint1_y;

		//Console.WriteLine("INTERSECTION Circle1 AND Circle2 AND Circle3:" + 
	//			"(" + intersectionPoint2_x + "," + intersectionPoint2_y + ")");
	}
	else
	{
		//Console.WriteLine("INTERSECTION Circle1 AND Circle2 AND Circle3: NONE");
		return FALSE;
	}
	*/

	//return TRUE;
}


int get_circle_sol2(double x1, double y1, double r1,
	double x2, double y2, double r2,
	double x3, double y3, double r3,
	double* sol_x, double* sol_y, double tolerance, double step_size)
{
	int converges = !IS_OK;
	int max_steps = 1000;
	int steps = 0;

	double* min;
	double min_e;
	double last_min_e = tolerance;
	unsigned direction = 0;

	while (converges != IS_OK)
	{
		converges = CircleIntersect(
				x1, y1, &r1,
				x2, y2, &r2,
				x3, y3, &r3,
				sol_x, sol_y, tolerance, &min_e);

		if(last_min_e < min_e)
			direction ^= 1; // toggle direction

		last_min_e = min_e;

		if(direction == 0)
		{
			r1 += step_size;
			r2 += step_size;
			r3 += step_size;
		}
		else
		{
			r1 -= step_size;
			r2 -= step_size;
			r3 -= step_size;
		}

		fprintf(stderr, "%d: %lf %lf %lf\n", converges, r1, r2, r3);
		steps++;

		// if convergence fails
		if (steps > max_steps)
		{
			fprintf(stderr, "Give Up\n");
			//*sol_x = (x1 + x2 + x3)/3;
			//*sol_y = (y1 + y2 + y3)/3;
			/*
			// find circle with minimum radius
			min = &r1;

			if (r2 < *min)
				min = &r2;

			if (r3 < *min)
				min = &r3;

			// set solution to minimum radius
			if(min == &r1)
			{
				*sol_x = x1;
				*sol_y = y1;
			}

			if(min == &r2)
			{
				*sol_x = x2;
				*sol_y = y2;
			}


			if(min == &r3)
			{
				*sol_x = x3;
				*sol_y = y3;
			}
			*/

			return FALSE;
		}
	}

	fprintf(stderr, "%lf, %lf\n", *sol_x, *sol_y);

	return TRUE;
}


/*
static void get_circle_sol(double x1, double y1, double r1,
		double x2, double y2, double r2,
		double x3, double y3, double r3,
		ref double sol_x, ref double sol_y, double tolerance)
{

	bool converges = false;
	converges = CircleIntersect(
			x1, y1, r1,
			x2, y2, r2,
			x3, y3, r3,
			ref sol_x, ref sol_y, tolerance);

	if (!converges)
	{
		Console.WriteLine("Second Try;");
		// Second Try
		converges = CircleIntersect(
				x1, y1, r1,
				x3, y3, r3,
				x2, y2, r2,
				ref sol_x, ref sol_y, tolerance);

		if (!converges)
		{
			Console.WriteLine("Third Try;");
			// Third Try
			converges = CircleIntersect(
					x2, y2, r2,
					x3, y3, r3,
					x1, y1, r1,
					ref sol_x, ref sol_y, tolerance);

		} // third try
	} // second try

	if (!converges)
	{
		// Give up with approximation
		CircleApprox(
				x1, y1, r1,
				x2, y2, r2,
				x3, y3, r3,
				ref sol_x, ref sol_y);
		Console.WriteLine("Failed To converge: (" + sol_x + "," + sol_y + ")");
	} // give up
	else
		Console.WriteLine("Converge: (" + sol_x + "," + sol_y + ")");
} // get_circle_sol()
*/



void test(void)
{
	double x1 = 0.0;
	double y1 = 1.0;
	double r1 = 0.9; //1

	double x2 = 0.0;
	double y2 = -1.0;
	double r2 = 1; //1

	double x3 = 2.0;
	double y3 = 0.0;
	double r3 = 2; //2

	double step_size = 0.01;
	double tolerance = 0.01;
	double sol_x = 0.0;
	double sol_y = 0.0;


	get_circle_sol2(
			x1, y1, r1,
			x2, y2, r2,
			x3, y3, r3,
			&sol_x, &sol_y, step_size, tolerance);

	printf("%lf %lf", sol_x, sol_y);

}



int main(int argc, char** argv)
{
	double x1;
	double y1;
	double r1; //1

	double x2;
	double y2;
	double r2; //1

	double x3;
	double y3;
	double r3; //2

	double step_size;
	double tolerance;
	double sol_x;
	double sol_y;

	int count = 1;

	if(argc != 12)
	{
		printf("error %d\n", argc);
		return 1;
	}

	sscanf(argv[count++], "%lf", &x1);	
	sscanf(argv[count++], "%lf", &y1);	
	sscanf(argv[count++], "%lf", &r1);	

	sscanf(argv[count++], "%lf", &x2);	
	sscanf(argv[count++], "%lf", &y2);	
	sscanf(argv[count++], "%lf", &r2);	

	sscanf(argv[count++], "%lf", &x3);	
	sscanf(argv[count++], "%lf", &y3);	
	sscanf(argv[count++], "%lf", &r3);	

	sscanf(argv[count++], "%lf", &step_size);	
	sscanf(argv[count++], "%lf", &tolerance);	

/*
	printf("%lf", x1);	
	printf("%lf", y1);	
	printf("%lf", r1);	

	printf("%lf", x2);	
	printf("%lf", y2);	
	printf("%lf", r2);	

	printf("%lf", x3);	
	printf("%lf", y3);	
	printf("%lf", r3);	

	printf("%lf", step_size);	
	printf("%lf", tolerance);	
	*/
	
	get_circle_sol2(
			x1, y1, r1,
			x2, y2, r2,
			x3, y3, r3,
			&sol_x, &sol_y,
			tolerance, step_size);

	printf("%lf %lf", sol_x, sol_y);

	return 0;
}
