#include <bits/stdc++.h>
#include <math.h>
#include <cmath>
// #define INF numeric_limits<double>::infinity()
using namespace std;
 
vector<pair<double,double>> points;
int nPoints=1000;
vector<vector<double>> errors(nPoints+1,vector<double>(nPoints+1));
vector<double> opt(nPoints+1);
vector<double> optTill(nPoints+1);
vector<vector<double>> slopes(nPoints+1,vector<double>(nPoints+1)),intercepts(nPoints+1,vector<double>(nPoints+1));
double c;
 
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
 
//print Points
void printPoints(int s,int e)
{
    for(int i=s;i<=e;i++)
    {
        cout << "x: " << points[i].first << " y: " << points[i].second << endl;
    }
}
 
//calculate error
double calculateError(int s,int e)
{
    double xySum = 0 , xSum=0 , ySum=0 , xsqSum=0 , n=e-s+1;
    for(int k=s;k<e;k++)
    {
        xySum+=points[k].first*points[k].second;
        xSum+=points[k].first;
        ySum+=points[k].second;
        xsqSum+=(points[k].first)*(points[k].first);
        // cout << "k: " << k << endl;
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
    // cout << "Slopes found" << endl;
    double intercept=(ySum - (slope*xSum))/n;
    intercepts[s][e]=intercept;
    // cout << "Intercept found" << endl;
    double sumSqError=0;
    for(int k=s;k<=e;k++)
    {
        double yExp=slope*points[k].first + intercept;
        double error=(points[k].second-yExp);
        double sqError=error*error;
        sumSqError+=sqError;
    }
    // cout << "sumSqerror found" << endl;
    return sumSqError;
}
 
//find least sqaure error
void leastSqError()
{
    for(int j=1;j<=nPoints;j++)
    {
        for(int i=1;i<=j;i++)
        {
            // cout << "i: " << i << "\t" << "j: " << j << endl;
            if(i==j) {
                // cout << "i==j" << endl;
                errors[i][j]=INFINITY;
            }
            else {
                // cout << "i!=j" << endl;
                errors[i][j]=calculateError(i,j);
            }
        }
    }
}
 
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
    // printf("done ebfore printing");
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
 
int main()
{
    input();
    // cout << "After input" << endl;
    opt.push_back(0);
    // optTill.push_back(0);
    // printPoints(0,points.size()-1);
    // cout << "After prinintg" << endl;
    leastSqError();
    // cout << "After leastSq" << endl;
    computeOpt();
    makeSegments();
    return 0;
}