#include <iostream>
#include <conio.h>
using namespace std;
int main()
{
int n, num=0,pass=1234; //holder for numeric password you can change its value whatever you want
cout<<"Enter the length of Password : ";
cin>>n; /* taking this input for passwords of different lengths say 4 or 5 it depends on your need */
cout<<"\nEnter password : ";
for(int i=1; i<=n; i++)
{
num=num*10+(getch()-48); /*this statement is used to convert the digits entered by the user into a number and subtracting
48 because if we subtract 48 from the ASCII value we get the original number.. for example ASCII value
of 5 is 53 so if we subtract 48 from it 53-48=5 and so on.. :) */
cout<<"*";
}
if(num==pass)
cout<<"\n\nCorrect Password\n\n";
else
cout<<"\n\nIncorrect Password\n\n";
return 0;
}
