#include <bits/stdc++.h>
#include <math.h>
#include <cmath>
using namespace std;


vector<pair<double,double>> points; /*Vector of pair of x and y coordinate of point*/
int nPoints=1000;   /*Number of Points*/
vector<vector<double>> errors(nPoints+1,vector<double>(nPoints+1));     /*Least square error of the best fitting lines passing through the given point*/
vector<double> opt(nPoints+1);      /*Used to calculate the set of lines which give the minimum error*/
vector<double> optTill(nPoints+1);      /*Used to store the beginning point of the set of lines which gives most optimal solution*/
vector<vector<double>> slopes(nPoints+1,vector<double>(nPoints+1)),intercepts(nPoints+1,vector<double>(nPoints+1));     /*Slope of the best fitting lines through the giveb point*/
double c;       /*cost*/
 
/// @brief Taking input from input.txt file
void input()
{
    double x, y;
    string temp1,temp2;
    fstream ipfile;
 
    ipfile.open("input.txt",ios::in);
    getline(ipfile,temp1);
    nPoints=stoi(temp1);
    for (int i = 0; i < nPoints; i++)
    {
        getline(ipfile,temp1);
        int spind=temp1.find(" ");
        x=stof(temp1.substr(0,spind));
        y=stof(temp1.substr(spind));
        pair<double,double> p = make_pair(x,y);
        points.push_back(p);       
    }
    sort(points.begin(),points.end());
    points.insert(points.begin(),make_pair(0,0));
    getline(ipfile,temp1);
    c=stof(temp1);
}
 

/// @brief print the points of the segment
/// @param s : starting point of the segment
/// @param e : ending point of the segment
void printPoints(int s,int e)
{
    for(int i=s;i<=e;i++)
    {
        cout << "x: " << points[i].first << " y: " << points[i].second << endl;
    }
}
 

/// @brief calculate least sqaure error of the given line
/// @param s start point of the segment
/// @param e ending point of the segment
/// @return least sqaure error of the line
double calculateError(int s,int e)
{
    double xySum = 0 , xSum=0 , ySum=0 , xsqSum=0 , n=e-s+1;
    for(int k=s;k<e;k++)
    {
        xySum+=points[k].first*points[k].second;
        xSum+=points[k].first;
        ySum+=points[k].second;
        xsqSum+=(points[k].first)*(points[k].first);
    }
    if(((n*xySum) - (xSum * ySum))==0)
        return 0;
    else if((n*xsqSum) - (xSum*xSum)==0)
    {
        if((n*xySum) - (xSum * ySum) < 0)
            return -INFINITY;
        else 
            return INFINITY;
    }
    double slope=((n*xySum) - (xSum * ySum)) / ((n*xsqSum) - (xSum*xSum));
    slopes[s][e]=slope;
    double intercept=(ySum - (slope*xSum))/n;
    intercepts[s][e]=intercept;
    double sumSqError=0;
    for(int k=s;k<=e;k++)
    {
        double yExp=slope*points[k].first + intercept;
        double error=(points[k].second-yExp);
        double sqError=error*error;
        sumSqError+=sqError;
    }
    return sumSqError;
}
 
/// @brief assign least square error to the errors vectors
void leastSqError()
{
    for(int j=1;j<=nPoints;j++)
    {
        for(int i=1;i<=j;i++)
        {
            if(i==j) {
                errors[i][j]=INFINITY;
            }
            else {
                errors[i][j]=calculateError(i,j);
            }
        }
    }
}
 

/// @brief compute the optimised set of lines
void computeOpt()
{
    for(int j=1;j<=nPoints;j++)
    {
        double minC=INFINITY;
        double x=0;
        for(int i=1;i<=j;i++)
        {
            double cost=errors[i][j] +opt[i-1]+c;
            minC=min(minC,cost);
            if(minC==cost)
                x=i;
        }
        opt[j]=minC;
        optTill[j]=x;
    }
}
 

/// @brief make the final optimised line segments that best approximates the given points 
void makeSegments()
{
    stack<double> st;
    FILE* opfile;
    opfile=fopen("output.txt","w");
    for(int s=optTill[nPoints],e=nPoints;e>=1;e=s-1,s=optTill[e])
    {
        st.push(e);
        st.push(s);
    }
    while(st.size())
    {
        int x=st.top();         //start
        st.pop();
        int y=st.top();         //end
        st.pop();
        printf("Segment is : y=%lf x + %lf from %d to %d with error %lf\n",slopes[x][y],intercepts[x][y],x,y,errors[x][y]);
        fprintf(opfile,"%lf %lf %lf %lf\n",slopes[x][y],intercepts[x][y],points[x].first,points[y].first);
    }
}
 
/// @brief main function to call the requires subroutines in the required order
/// @return end of the program
int main()
{
    input();
    opt.push_back(0);
    leastSqError();
    computeOpt();
    makeSegments();
    return 0;
}
