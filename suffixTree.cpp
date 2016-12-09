#include <iostream>
#include <string>
#include <stack>
#include <cstring>
#include <unordered_set>

#include "suffixTree.h"

using namespace std;

suffixTree::suffixTree( string& x )
{
	this->T = &x;
	this->n = x.size();
	
	this->root = new stNode;
	this->root->setSLink( this->root );
	
	stNode * u = this->root;
	stNode * v = NULL;
	stNode * w = NULL;
	int d = 0;
	for ( int i = 0; i < this->n; i++ )
	{
		while ( d == u->getDepth() && u->getChild( x[i+d] ) != NULL )
		{
			u = u->getChild( x[i+d] );
//			u->addOcc( i );
			d = d+1;
			while (	d < u->getDepth() && x[u->getStart()+d] == x[i+d] )
				d = d + 1;
		}
		if ( d < u->getDepth() )
		{
			v = createNode ( u, d );
			u = v;
		}
		w = createBranch ( u, i, d );
		
		if ( u->getSLink() == NULL )
		{
			ComputeSuffixLink( u );
		}
		u = u->getSLink();
		d = max( d-1, 0 );
	}
}

void suffixTree::bfs_leaves()
{
	stNode * u = NULL;
	stNode * v = NULL;
	stack<stNode*> s;
	u = this->root;
	s.push( u );
	while( !s.empty() )
	{
		v = s.top();
		s.pop();
		cout << v << endl;
		if ( v->numChild() == 0 )
		{
			pair<int, stNode*> leaf ( v->getStart(), v );
			this->leaves.insert( leaf );
		}
		else
		{
			stNode ** children = v->allChild();
			for ( int i = 0; i < v->numChild(); i++ )
				s.push( children[i] );
			delete[] children;
		}
	}
}

suffixTree::~suffixTree()	
{
//	delete this->root;
}

void suffixTree::ComputeSuffixLink( stNode * u )
{
	int d = u->getDepth();
	stNode * v = u->getParent()->getSLink();
	while ( v->getDepth() < d-1 )
	{	
		v = v->getChild( this->T->at( u->getStart() + v->getDepth() + 1 ) );
	}
	if ( v->getDepth() > d-1 )
	{
		v = createNode ( v, d-1 );
		v->setSLink ( NULL );
	}
	u->setSLink ( v );
}

stNode * suffixTree::createNode( stNode * u, int d )
{
	int i = u->getStart();
	stNode * p = u->getParent();
	stNode * v = new stNode( i, d, this->T->at( i+p->getDepth() ) );
	v->addChild( u, this->T->at( i+d ) );
//	u->setParent( v, this->T->at( i+d ) );
	p->addChild( v, this->T->at( i+p->getDepth() ) );
//	v->setParent( p );
	return v;
}	

stNode * suffixTree::createNode_trim( stNode * u, int d )
{
	int i = u->getStart();
	stNode * p = u->getParent();
	stNode * v = new stNode( i, d, this->T->at( i+p->getDepth() ) );
	v->setSLink( p->getSLink() );
	v->addChild( u, this->T->at( i+d ) );
	p->addChild( v, this->T->at( i+p->getDepth() ) );
	return v;
}	

stNode * suffixTree::createBranch( stNode * u, int i, int d )
{
	stNode * w = new stNode(i, this->n-i, this->T->at( i+d ) ) ;
	u->addChild( w, this->T->at( i+d ) );
//	w->setParent( u, this->T->at( i+d ) );
	return w;
}

//stNode * suffixTree::createLeaf( int i )
//{
//	u->addLeaf( i );
//	return w;
//}

int suffixTree::forward_search( string x )
{
	//TODO find all occurrences
	int m = x.size();
	int d = 0;
	stNode * u = this->root;
	stNode * v = NULL;
	bool match = true;
	while ( match )
	{
		v = u->getChild( x[d] );
		if ( v == NULL )
		{
			match = false;
			break;
		}
		int i, j;
		for ( i = d, j = 0; i < m && j < v->getDepth() ; i++, j++ )
		{
			if ( x[i] != this->T->at( v->getStart()+j ) )
			{
				match = false;
				break;
			}
		}
		d = v->getDepth();
		u = v;
		if ( d >= m )
		{
			if ( !u->printOcc() )
			{
				return u->getStart()+m-1;
			}
		}
	}
	return -1;
}

stNode * suffixTree::forward_search_node( stNode * s, int l )
{
	int start = s->getStart();
	int position = s->getStart() + s->getDepth();
	stNode * u = s;
	int extent = l - u->getDepth();
	while ( extent > 0 )
	{
		u = u->getChild( this->T->at( position ) );
		extent = l - u->getDepth();
		position = u->getStart() + u->getDepth();
	}
	return u;	
}

void suffixTree::trimST( int * ME )
{
	bfs_leaves();
	stNode * u = this->root;
	int node_move_type = 0;
	for ( int i = 0; i < this->n; i++ )
	{
		u = forward_search_node( u, ME[i] );
		if ( ME[i] < u->getDepth() )
		{
			u = createNode_trim( u, ME[i] );
			u->addLeaf( i );
			auto it = this->leaves.find( i );
		//	it->second->getParent()->removeChild( this->T->at(i) );

			delete it->second;
			this->leaves.erase( it );
			node_move_type = 1;
		}
		else
		{
			if ( u->numChild() != 0 )
			{
				u->addLeaf( i );
				node_move_type = 2;
			}
			else
			{
				stNode * v = createNode_trim( u, ME[i]-1 );
				u = v;
				u->addLeaf( i );
				node_move_type = 1;
			}
			auto it = this->leaves.find( i );
		//	it->second->getParent()->removeChild( this->T->at(i) );
			delete it->second;
			this->leaves.erase( it );
		}
		if ( u == this->root )
			u = u->getSLink();
		else if ( node_move_type == 1 )
			u = u->getParent()->getSLink();
		else if ( node_move_type == 2 )
			u = u->getSLink();
	}
}

void suffixTree::printLeaves()
{
	this->leaves.clear();
	cout << "________________" << endl;
	bfs_leaves();
	for ( auto it = this->leaves.begin(); it != this->leaves.end(); it++ )
	{
		cout << it->first << ":" << it->second << ":\t";
		it->second->printOcc();
		cout << endl;
	}
}

