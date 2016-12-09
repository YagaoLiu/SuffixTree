#include <string>
#include <map>

#include "stNode.h"

using namespace std;

class suffixTree
{
	private:
		stNode * root;
		string * T;
		int n;
		int num;
		map<int, stNode*> leaves;
		void bfs_leaves();

	public:
		suffixTree( string& x );
		stNode * createNode( stNode * u, int d );
		stNode * createBranch( stNode * u, int i, int d );
		stNode * createLeaf( stNode *u, int i );
		stNode * createNode_trim(stNode * u, int d );
		void ComputeSuffixLink( stNode * u );
		int forward_search( string x );
		stNode * forward_search_node( stNode * s, int l );
		void trimST( int * ME );
		void printLeaves();
		~suffixTree();
};
