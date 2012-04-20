#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* gcc closest-pair-among-various-points-in-a-graph.c -lm */

/* random large distance */
#define DISTANCE 99999

#define INVALID 0
#define VALID   1


/* n = number of input points */
#define n 15


typedef struct point {
    int a;
    int b;
}p;


typedef struct pair {
    p p1;
    p p2;
}pp;


void copy_pt(p *p1, p *p2)
{
    p1->a = p2->a;
    p1->b = p2->b;
}


void print_cp(pp cp)
{
    printf("{%3d, %3d}, {%3d, %3d} \n", 
            cp.p1.a, cp.p1.b,
            cp.p2.a, cp.p2.b);
    return;
}


void print_points(p pts[], int from, int to)
{
    int i;
    for(i=from ; i<to ; i++)
        printf("{%3d, %3d}, ", pts[i].a, pts[i].b);
    printf("\n");
    
    return;
}


void copy_points(p dest[], p src[], int from, int to)
{
    int i, j;
    
    for (i=from, j=0 ; i<to ; i++, j++)
        copy_pt(&dest[j], &src[i]);

    return;
}


/*
A simple merge sort to sort the given pointes based on
its x and y coordinates
*/
void sort_points_merge(p pts[], int first, int last, int sorta)
{
    int mid = (first+last)/2;
    int ai = first;
    int bi = mid+1;
    p final[last-first+1];
    int finali=0, i;

    while (ai <= mid && bi <= last) {
        if ((sorta  && pts[ai].a <= pts[bi].a) || 
            (!sorta && pts[ai].b <= pts[bi].b)) {
            copy_pt(&final[finali], &pts[ai]);
            finali++; ai++;
        } else {
            copy_pt(&final[finali], &pts[bi]);
            finali++; bi++;
        }
    }

    while (ai <= mid) {
        copy_pt(&final[finali], &pts[ai]);
        finali++; ai++;
    }
            
    while (bi <= last) {
        copy_pt(&final[finali], &pts[bi]);
        finali++; bi++;
    }

    for (i=0 ; i<last-first+1 ; i++)
        copy_pt(&pts[i+first], &final[i]);

    return;
}


void sort_points(p pts[], int a, int b, int sorta)
{
    int x, y, z, mid;
    if (a >= b) return;

    mid = (a+b)/2;
    
    sort_points(pts, a, mid, sorta);
    sort_points(pts, mid+1, b, sorta);
    sort_points_merge(pts, a, b, sorta);

    return;
}


double distance(p p1, p p2)
{
    return sqrt(pow((p2.a-p1.a), 2) + pow((p2.b-p1.b), 2));
}


pp closest_2(pp cp1, pp cp2, double *sd)
{
    double d1, d2;
    d1 = distance(cp1.p1, cp1.p2);
    d2 = distance(cp2.p1, cp2.p2);
    *sd = d1 < d2 ? d1 : d2;
    return d1 < d2 ? cp1 : cp2;
}


pp closest_3(pp cp1, pp cp2, pp cp3, double *sd)
{
    double d1, d2, d3;
    return closest_2(closest_2(cp1, cp2, sd), cp3, sd);
}


/*
Given 2 points, p1 and p2, compute the distance 
between then and if that is lesser than the current
shortest distance (which is stored in sd), then update
the sd and update cp (closest pair) with p1 and p2.

If distance between p1 and p2 are greater than sd then
do nothing.
*/
void compute_min_distance(p p1, p p2, double *sd, pp *cp)
{
    double dist = distance(p1, p2);
    if (dist < *sd) {
        *sd = dist;
        copy_pt(&cp->p1, &p1);
        copy_pt(&cp->p2, &p2);
    }

    return;
}


int ClosestSplitPair(p px[], p py[], int px_size, int py_size, double delta, pp *cp) 
{
    #define DELIMITER 7
    int xbar = px[(px_size/2)-1].a;
    p new_py[py_size];
    int new_py_size = 0;
    int i, j;
    double sd = DISTANCE, dist;
    int retval = INVALID;

    for (i=0 ; i<py_size ; i++) {
        if ((py[i].a >= xbar - delta) &&
            (py[i].a <= xbar + delta)) {
            copy_pt(&new_py[new_py_size], &py[i]);
            new_py_size++;
        }
    }

    for (i=0 ; i<new_py_size-DELIMITER; i++) {
        for (j=0 ; j<DELIMITER; j++) {
            compute_min_distance(new_py[i], new_py[i+j], &sd, cp);
            retval = VALID;
        }
    }

    return retval;
}


/*
px[]    = elements sorted with x coordinates
px_size = number of elements on px[] array
py[]    = elements sorted with y coordinates
py_size = number of elements on py[] array
*/
pp ClosestPair(p px[], p py[], int px_size, int py_size)
{
    /* sd = shortest distance */
    double sd = DISTANCE; 

    /* cp = closest pair */
    pp cp;

    int i, j;
    double dist;

    /* base case */
    /* when px or py array are less than 2 elements then
       go for brute force method to find the closest pair 
    */
    if (px_size <= 2 || py_size <= 2) {
        
        /* find the shortest distance between all 
           the points in px and py
        */
        for (i=0 ; i<px_size ; i++) 
            for (j=0 ; j<py_size ; j++) 
                compute_min_distance(px[i], py[j], &sd, &cp);

        /* find the shortest distance between
           all the points in px
        */
        if (px_size == 2)
            compute_min_distance(px[0], px[1], &sd, &cp);

        /* find the shortest distance between
           all the points in py
        */
        if (py_size == 2)
            compute_min_distance(py[0], py[1], &sd, &cp);

        return cp;
    } 


    /* size of left/right half of px/py arrays */
    int px_lsize = px_size/2;
    int px_rsize = (px_size+1)/2;
    int py_lsize = py_size/2;
    int py_rsize = (py_size+1)/2;


    /* left/right half of px/py arrays sorted using x/y coordinates */
    p px_l[px_lsize], px_r[px_rsize], py_l[py_lsize], py_r[py_rsize];


    /* DIVIDE: split the array into two sub-arrays */ 
    copy_points(px_l, px, 0, px_lsize);
    copy_points(px_r, px, px_lsize, px_size);
    copy_points(py_l, py, 0, py_lsize);
    copy_points(py_r, py, py_lsize, py_size);


    /* The following are the closest pair */
    pp cp1, cp2, cp3;
    

    cp1 = ClosestPair(px_l, px_r, px_lsize, px_rsize);
    cp2 = ClosestPair(py_l, py_r, py_lsize, py_rsize);
    closest_2(cp1, cp2, &sd);

    return ClosestSplitPair(px, py, px_size, py_size, sd, &cp3) == VALID ? 
           closest_3(cp1, cp2, cp3, &sd) :
           closest_2(cp1, cp2, &sd);

}


int main(void)
{
    /* pts is the input to the program */
    /* there should be n elements to this pts array */
    //p pts[n] = {{1, 3}, {-2, 5}, {4, 2}, {-5, 1}, {3, -4}, {-1, 2}};
    p pts[n] = {{1, 3}, {-2, 5}, {4, 2}, {-5, 1}, {3, -4}, {-1, 2}, {4, 4}, {-2, 9},
                {3, 1}, {5, -2}, {2, 4}, {1, -5}, {-4, 3}, {2, -1}, {2, -2}};

    printf("Given points: \n");
    print_points(pts, 0, n);

    /* px = elements sorted by x coordinates */
    /* py = elements sorted by y coordinates */
    p px[n], py[n];
    
    /* cp = closest pair */
    pp cp;

    /* copy the pts to px and py arrays */
    copy_points(px, pts, 0, n);
    copy_points(py, pts, 0, n);     

    /* sort px based on x coordinate */
    /* sort py based on y coordinate */
    sort_points(px, 0, n-1, 1);
    sort_points(py, 0, n-1, 0);

    // printf("sort x : "); print_points(px, 0, n);
    // printf("sort y : "); print_points(py, 0, n);

    cp = ClosestPair(px, py, n, n);

    printf("closest pair : \n");
    print_cp(cp);

    return 0;
}

