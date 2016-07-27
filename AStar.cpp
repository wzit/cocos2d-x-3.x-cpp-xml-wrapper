/******************************************************************************/
/*
* Copyright 2014-2015 Vladimir Tolmachev
*
* Author: Vladimir Tolmachev
* Project: ml
* e-mail: tolm_vl@hotmail.com
* If you received the code is not the author, please contact me
*/
/******************************************************************************/

#include "AStar.h"

NS_CC_BEGIN;

namespace AStar
{
	Map::Cells find( Map & map, Map::Cell_ptr start, Map::Cell_ptr goal )
	{
		map.clearCells();
		Map::Cells closedset;
		Map::Cells openset;
		support::add( openset, start );

		start->g = 0;
		start->h = map.heuristic_cost_estimate( start, goal );
		start->f = start->g + start->h;

		while( openset.empty() == false )
		{
			support::sort_f( openset );

			Map::Cell_ptr c = openset.front();
			if( c == goal )
			{
				return reconstruct_path( start, goal );
			}

			support::remove( openset, c );
			support::add( closedset, c );

			Map::Cells neighbors = map.neighbors( c, goal );
			for( auto n : neighbors )
			{
				auto exist = support::exist( closedset, n );
				if( exist )
					continue;

				auto tentative_g_score = c->g + map.heuristic_cost_estimate( c, n );

				bool tentative_is_better( false );
				exist = support::exist( openset, n );
				if( exist == false )
				{
					support::add( openset, n );
					tentative_is_better = true;
				}
				else
				{
					tentative_is_better = tentative_g_score < n->g;
				}

				if( tentative_is_better == true )
				{
					n->came_from = c;
					n->g = tentative_g_score;
					n->h = map.heuristic_cost_estimate( n, goal );
					n->f = n->g + n->h;
				}
			}
		}

		if( closedset.empty() == false )
		{
			support::sort_h( closedset );
			auto agoal = closedset.front();
			return reconstruct_path( start, agoal );
		}
		return Map::Cells();
	}

	Map::Cells reconstruct_path( Map::Cell_ptr start, Map::Cell_ptr goal )
	{
		Map::Cell_ptr current_node = goal;
		Map::Cells path_map;
		while( current_node )
		{
			support::add( path_map, current_node );
			current_node = current_node->came_from;
		}

		std::reverse( path_map.begin(), path_map.end() );
		if( goal->isPassed() == false )
			support::remove( path_map, goal );
		return path_map;
	}

	Map::Map( int rows, int cols)
		: _rows( rows )
		, _cols( cols )
	{
		for( int i = 0; i < _rows * _cols; ++i )
		{
			Cell_ptr c = new Cell( i / _cols, i % _cols );
			_data.push_back( c );
		}
	}

	Map::~Map()
	{
		for( auto cell : _data )
		{
			delete cell;
		}
	}

	Map::Cell_ptr Map::cell( int row, int col )
	{
		assert( row < _rows );
		assert( col < _cols );
		return _data[row * _cols + col];
	}

	Map::Cells Map::neighbors( Cell_ptr cell, Cell_ptr goal )
	{
		Cells Cells;

		int array[][2] = {
			//{ -1, -1 },
			{ -1, +0 },
			//{ -1, +1 },
			{ +0, -1 },
			{ +0, +1 },
			//{ +1, -1 },
			{ +1, +0 },
			//{ +1, +1 },
		};

		for( auto c : array )
		{
			int row = cell->_row + c[0];
			int col = cell->_col + c[1];
			if( row == goal->row() && col == goal->col() )
			{
				auto cell = _data[row * _cols + col];
				Cells.push_back( cell );
				break;
			}
			if( row < 0 || row > _rows - 1 ) continue;
			if( col < 0 || col > _cols - 1 ) continue;
			auto cell = _data[row * _cols + col];
			if( cell->passed == false ) continue;
			Cells.push_back( cell );
		}

		return Cells;
	}

	float Map::heuristic_cost_estimate( Cell_ptr a, Cell_ptr b )
	{
		int r = a->_row - b->_row;
		int c = a->_col - b->_col;
		return std::sqrt(float( r*r + c*c ));
	}

	void Map::clearCells()
	{
		for( auto cell : _data )
		{
			cell->g = 0;
			cell->h = 0;
			cell->f = 0;
			cell->came_from = nullptr;
		}
	}

	Cell::Cell( int _row, int _col )
		: _row( _row )
		, _col( _col )
		, g( 0 )
		, h( 0 )
		, f( 0 )
		, came_from()
		, passed( false )
	{}

	void Cell::setPassed( bool value )
	{
		this->passed = value;
	}
	
	bool Cell::isPassed()const
	{
		return this->passed;
	}

	void support::remove( Map::Cells & Cells, Map::Cell_ptr cell )
	{
		auto i = std::find( Cells.begin(), Cells.end(), cell );
		if( i != Cells.end() ) 
			Cells.erase( i );
	}

	void support::add( Map::Cells & Cells, Map::Cell_ptr cell )
	{
		Cells.push_back( cell );
	}

	bool support::exist( Map::Cells & Cells, Map::Cell_ptr cell )
	{
		auto i = std::find( Cells.begin(), Cells.end(), cell );
		return i != Cells.end();
	}

	void support::sort_g( Map::Cells & Cells )
	{
		std::sort( Cells.begin(), Cells.end(), Cell::compare_g );
	}

	void support::sort_h( Map::Cells & Cells )
	{
		std::sort( Cells.begin(), Cells.end(), Cell::compare_h );
	}

	void support::sort_f( Map::Cells & Cells )
	{
		std::sort( Cells.begin(), Cells.end(), Cell::compare_f );
	}
}

NS_CC_END;