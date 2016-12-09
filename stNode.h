#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

#ifndef stNode_T
#define stNode_T

class stNode
{
	private:
		int start;
		int depth;
		char label;
		unordered_map<char, stNode*> child;
		vector<int> Occ;
		stNode * parent;
		stNode * slink;
	public:
		stNode();
		stNode( int i, int d, char c );
		int getStart();
		int getDepth();
		char getLabel();
		stNode * getChild( char c );
		stNode * getParent();
		stNode * getSLink();
		void setSLink( stNode * slinkNode );
		void addChild( stNode * childNode, char c );
		void addLeaf( int i );
		void removeChild( char c );
		int printOcc();
		void setParent ( stNode * parentNode );
		int numChild();
		void printChild();
		stNode** allChild();
		~stNode();
};

#endif
