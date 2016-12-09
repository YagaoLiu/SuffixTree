#include <iostream>
#include <string>
#include <unordered_map>
#include "stNode.h"

using namespace std;

stNode::stNode()
{
	this->start = 0;
	this->depth = 0;
	this->label = '$';
	this->parent = NULL;
	this->slink = NULL;
}

stNode::stNode( int i, int d, char c )
{
	this->start = i;
	this->depth = d;
	this->label = c;
	this->parent = NULL;
	this->slink = NULL;
}

int stNode::getStart()
{
	return this->start;
}

int stNode::getDepth()
{
	return this->depth;
}

char stNode::getLabel()
{
	return this->label;
}

stNode * stNode::getChild( char c )
{
	unordered_map<char, stNode*>::iterator it;
	it = this->child.find( c );
	if ( it == this->child.end() )
		return NULL;
	else
		return it->second;
}

stNode * stNode::getParent()
{
	return this->parent;
}

stNode * stNode::getSLink()
{
	return this->slink;
}

void stNode::setSLink( stNode * slinkNode )
{
	this->slink = slinkNode;
}

void stNode::addChild( stNode * childNode, char c )
{
	auto it = this->child.find( c );
	if ( it == this->child.end() )
	{
		pair <char, stNode*> insertChild( c, childNode );
		this->child.insert( insertChild );
	}
	else
	{
			it->second = childNode;
	}
	childNode->label = c;
	childNode->parent = this;
}

void stNode::addLeaf( int i )
{
	auto it = this->child.find( '$' );
	if ( it == this->child.end() )
	{
		stNode * leaf = new stNode( i, this->depth+1, '$' );
		leaf->parent = this;
		leaf->Occ.push_back( i );
		pair<char, stNode*> insertLeaf( '$', leaf );
		this->child.insert( insertLeaf );
	}
	else
	{
		it->second->Occ.push_back( i );
	}
}

void stNode::removeChild( char c )
{
	auto it = this->child.find( c );
	if ( it != this->child.end() )
	{
		this->child.erase( it );
	}
}

int stNode::printOcc()
{
	if ( this->Occ.size() == 0 )
	{
		cout << "No Occ, failed" << endl;
		return 0;
	}
	else
	{
		for( auto it = this->Occ.begin(); it != this->Occ.end(); it++ )
			cout << *it << '\t';
		return 1;
	}
}

void stNode::setParent( stNode * parentNode )
{
	this->parent = parentNode;
}

stNode::~stNode()
{
	auto it = this->parent->child.find( this->label );
	this->parent->child.erase( it );
	for ( auto it = this->child.begin(); it != this->child.end(); ++it )
	{
		cout << "delete " << it->second << endl;
		delete it->second;
	}
	this->child.clear();
	if ( this->parent->child.empty() )
	{
		cout << "delete " << this->parent << endl;
		delete this->parent;
	}
}

int stNode::numChild()
{
	return this->child.size();
}

void stNode::printChild()
{
	for ( auto it = this->child.begin(); it != this->child.end(); it++ )
		cout << it->first << '\t' << it->second << endl;
}

stNode ** stNode::allChild()
{
	int n = this->child.size();
	if ( n == 0 )
		return NULL;
	else
	{
		stNode ** allChild = new stNode * [n];
		int i = 0;
		auto it = this->child.begin();
		for ( ; it != this->child.end(); ++it, ++i )
		{
			allChild[i] = it->second;
		}
		return allChild;
	}
}
