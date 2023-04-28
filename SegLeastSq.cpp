#include <bits/stdc++.h>
#include <cmath>
#define MAXSIZE 1000
using namespace std;
 
int nPoints=MAXSIZE+1;          /*Number of points*/
vector<pair<double,double>> points(nPoints);    /*Points*/
vector<double> OPT(nPoints);        /*Used to calculate the set of lines which give the minimum error*/
vector<int> OptTill(nPoints);       /*Used to store the beginning point of the set of lines which gives most optimal solution*/
vector<double> xSum(nPoints);       /*Cumulative sum of all x values*/
vector<double> xySum(nPoints);      /*Cumulative sum of all xy values*/
vector<double> ySum(nPoints);       /*Cumulative sum of all y values*/
vector<double> xSqSum(nPoints);     /*Cumulative sum of all xSq values*/
vector<vector<double>> errors(nPoints,vector<double>(nPoints));     /*Error of the best fitting lines through the given points*/
vector<vector<double>> slopes(nPoints,vector<double>(nPoints));     /*Slope of the best fitting lines through the given points*/
vector<vector<double>> intercepts(nPoints,vector<double>(nPoints));     /*Intercepts of the best fitting lines through the given points*/
double c;       /*cost*/
 
/// @brief takes input from input.txt
void input()
{
    double x, y;
    string temp1,temp2;
    fstream ipfile;
    FILE* opfile;
 
    ipfile.open("input.txt",ios::in);
    opfile=fopen("output.txt","w");
    getline(ipfile,temp1);
    nPoints=stoi(temp1);
 
    for (int i = 1; i <= nPoints; i++)
    {
        getline(ipfile,temp1);
        int spind=temp1.find(" ");
        x=stof(temp1.substr(0,spind));
        y=stof(temp1.substr(spind));
        pair<double,double> p = make_pair(x,y);
        points[i]=p;       
    }
    sort(points.begin()+1,points.begin()+nPoints+1);
    getline(ipfile,temp1);
    c=stof(temp1);
}
 

/// @brief calculates least square error of the given line
/// @param s start point of the segment
/// @param e end point of the segment
void calculateError(int s,int e)
{
    double sqError;
    errors[s][e]=0;
    for(int m=s;m<=e;m++)
    {
        sqError=points[m].second - (slopes[s][e] * points[m].first + intercepts[s][e]);
        errors[s][e]+=sqError*sqError;
    }
}

/// @brief compute the optimised set of lines
void calculateOpt()
{
    OPT[0]=0;
    OptTill[0]=0;
    for(int j=1;j<=nPoints;j++)
    {
        double minC=INFINITY;
        int index=0;
        for(int i=1;i<=j;i++)
        {
            double curCost=errors[i][j]+OPT[i-1]+c;
            if(curCost<minC)
            {
                minC=curCost;
                index=i;
            }
        }
        OPT[j]=minC;
        OptTill[j]=index;
    }
}

/// @brief Computes all possible line segments of the given set of points
void findLine()
{
    xSum[0]=0;
    xySum[0]=0;
    ySum[0]=0;
    xSqSum[0]=0;
    for(int j=1;j<=nPoints;j++)
    {
        double xSummation,xySummation,ySummation,xSqSummation;
        xSum[j]=xSum[j-1]+points[j].first;
        ySum[j]=ySum[j-1]+points[j].second;
        xySum[j]=xySum[j-1]+points[j].first*points[j].second;
        xSqSum[j]=xSqSum[j-1]+points[j].first*points[j].first;
        for(int i=1;i<=j;i++)
        {
            xSummation=xSum[j]-xSum[i-1];
            xySummation=xySum[j]-xySum[i-1];
            xSqSummation=xSqSum[j]-xSqSum[i-1];
            ySummation=ySum[j]-ySum[i-1];
            double numerator,denominator;
            numerator=(j-i+1)*(xySummation)-xSummation*ySummation;
            if(numerator==0)
            {
                slopes[i][j]=0;
            }
            else
            {
                denominator=(j-i+1)*xSqSummation-(xSummation*xSummation);
                if(denominator==0)
                {
                    slopes[i][j]=INFINITY;
                }
                else
                {
                    slopes[i][j]=(numerator)/(denominator);
                }
            }
            intercepts[i][j]=(ySummation - slopes[i][j]*xSummation)/(j-i+1);
            calculateError(i,j);
        }
    }
}
 
/// @brief print the points (used for testing)
void printPoints()
{
    for(int i=1;i<=nPoints;i++)
    {
        cout << "x: " << points[i].first << "\t" << "y: " << points[i].second;
        cout << endl;
    }
}

/// @brief make the final optimised line segments that best approximates the given points 
void makeSegments()
{
    stack<double> st;
    FILE *opFile;
    opFile=fopen("output.txt","w");
    double i;
    int j;
    for(i=nPoints,j=OptTill[nPoints];i>0;i=j-1,j=OptTill[i])
    {
        st.push(i);
        st.push(j);
    }
    while(!st.empty())
    {
        double start=st.top();
        st.pop();
        double end=st.top();
        st.pop();
        printf("segment y=%lf*x+%lf form point %lf to %lf with sq Error %lf\n",slopes[start][end],intercepts[start][end],start,end,errors[start][end]);   
        fprintf(opFile,"%lf %lf %lf %lf\n",slopes[start][end],intercepts[start][end],points[start].first,points[end].first);
    }
}

/// @brief main function to call the requires subroutines in the required order
/// @return end of the program
int main()
{
    input();
    findLine();
    calculateOpt();
    makeSegments();
    return 0;
 
}
 
     