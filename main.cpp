#include <iostream>
#include <string>

#include "suffixTree.h"

using namespace std;

int main()
{
	string x = "ssississi$";
	suffixTree ST(x);
	ST.forward_search( "ssi" );
	int * ME = new int [10];
	ME[0]=ME[1]=ME[3]=ME[4]=ME[6]=ME[7]= 3;
	ME[2]=ME[5]= 2;
	ME[8]=1;
	ME[9]=0;
	ST.trimST( ME );
	ST.printLeaves();
	return 0;
}
